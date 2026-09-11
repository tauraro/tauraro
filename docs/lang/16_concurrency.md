# 16 — Concurrency

> **See also:** [Concurrency Guide — Models, Primitives, Patterns & Best Practices](advanced/07_concurrency_guide.md) for the full picture (OS threads vs green threads vs thread-per-core, a decision matrix, and best-practice combinations), and [`examples/concurrency/`](../../examples/concurrency/) for runnable examples of every pattern.

---

## Overview

Tauraro provides a layered concurrency model:

| Primitive | Use when |
|-----------|---------|
| `async def` / `await` | Sequential async pipelines — I/O work, logical ordering |
| `spawn f(args)` | Fire-and-forget background task |
| `task_group:` | Structured concurrency — wait for a fixed set of spawned tasks |
| `await_all(f1(), f2(), ...)` | Parallel calls with a single wait-for-all |
| `await_timeout(fn(), ms)` | Async call with a hard millisecond deadline |
| `Chan[T]` | Producer-consumer pipelines between tasks |
| `Mutex[T]` | Exclusive lock guarding a shared value |
| `RwLock[T]` | Multiple concurrent readers or one exclusive writer |
| `Atomic[T]` | Lock-free atomic integer operations |
| `ThreadLocal[T]` | Per-thread storage — each thread sees its own value |
| `Shared[T]` | Reference-counted ownership across threads |
| `Sendable` | Compile-time marker: type is safe to send across thread boundaries |
| `StructuredGroup` | Fault-tolerant batch — panicked threads are caught, not fatal |
| `IOPoll` / `EventLoop` | Non-blocking event-driven I/O (epoll/IOCP/kqueue) |
| `Pool` | Thread pool for CPU-bound work |

**Compile-time safety errors:**

| Code | Meaning |
|------|---------|
| `[T-1]` | Value passed to `spawn`/`Thread.spawn`/`Shared[T]` is not `Sendable` |
| `[T-2]` | A class `implements Sendable` but declares a field whose type is not `Sendable` — checked **transitively** through `Shared[T]`/`Weak[T]`/`Chan[T]` (their inner `T` must also be Sendable, so non-thread-safe data can't be reached through a handle) |
| `[T-3]` | *(warning)* A `Sendable` class has a primitive field that may race if mutated from multiple threads — wrap it in `Atomic[T]` |
| `[T-6]` | A borrow (`ref`/`mut ref`) crosses a thread boundary — it could dangle or race. Pass an owned value, `Shared[T]`, or `Mutex[T]` instead (like Rust's `thread::spawn` requiring `'static`) |
| `[T-7]` | A **plain reference-counted class** crosses a thread boundary. Its refcount is non-atomic (`!Send`, exactly like Rust's `Rc`) so two threads retaining/releasing it would race the count. Checked **transitively** (through `Mutex`/`Vec`/`Dict`/fields). Use `Shared[T]` — the atomic `Arc` equivalent |

> **The concurrency guarantee.** `[T-1]`+`[T-2]` (only Sendable data crosses) with
> `[T-6]` (no borrows cross) and `[T-7]` (no non-atomic refcount crosses) mean that
> under `--strict`, **a program that compiles is free of thread-safety bugs** — no
> data race, no cross-thread use-after-free, no refcount race. Every value shared
> between threads is an atomic `Shared[T]` or a synchronization primitive. This is
> the same safety outcome as Rust's `Send`/`Sync`, reached with **zero lifetime
> annotations**. A framework that audits its own internals can opt a specific class
> out of `[T-7]`/`[T-2]` with `implements Sendable, UnsafeSendable` (Tauraro's
> `unsafe impl Send/Sync`); code that compiles without it keeps the full guarantee.

---

## `async def` / `await` — Cooperative Async

### When to use

Use `async def` / `await` for:

- I/O-bound pipelines where each step waits for the previous
- Code that benefits from a clean sequential style but still runs work on a dedicated thread
- Calling other async functions in sequence

Do not use `await` when you want truly parallel execution — use `await_all`, `spawn`, or `task_group:` for that.

### How it works

```python
async def fetch(id: int) -> str:
    return f"item-{id}"

async def process(n: int) -> int:
    mut data = await fetch(n)
    return len(data)

async def main():
    mut r1 = await process(1)
    mut r2 = await process(42)
    print(f"process(1)  = {r1}")    # 6
    print(f"process(42) = {r2}")    # 7
```

`await f(args)` launches `f` on an OS thread, blocks the caller until it completes, and returns the result. The calls run sequentially — `r2` does not start until `r1` is done.

Rules:
- `async def` can `await` other `async def` functions
- A non-async `def` can call `async def` without `await` — treated as a normal call
- `await` on a non-async function is a no-op pass-through

### Async methods

`async def` works on class methods too, not just free functions — declare it the same way inside an `extend` block:

```python
class Fetcher:
    pub base_url: str

extend Fetcher:
    pub async def fetch(self, id: int) -> str:
        return f"{self.base_url}/{id}"

    pub async def fetch_two(self, a: int, b: int) -> str:
        mut r1 = await self.fetch(a)
        mut r2 = await self.fetch(b)
        return r1 + "," + r2

async def main():
    mut f = Fetcher()
    f.base_url = "https://api.example.com"
    mut result = await f.fetch_two(1, 2)
    print(result)
```

### Common Mistakes

**Mistake: using sequential `await` when you wanted parallel execution.**
```python
async def main():
    mut r1 = await slow_task(1)    # waits for slow_task(1) to finish first
    mut r2 = await slow_task(2)    # then runs slow_task(2)
    # total time = time(1) + time(2)
```
If you want both to run at the same time, use `await_all`:
```python
await_all(slow_task(1), slow_task(2))
```

### Best Practices

1. Use `await` for sequential pipelines; use `await_all` or `task_group:` for parallel batches.
2. Mark a function `async def` only when it itself calls `await` — otherwise it adds no benefit.

---

## `spawn` — Fire-and-Forget Tasks

### When to use

Use `spawn f(arg)` when you want to start a background task and do not need to wait for it to finish or retrieve its result.

### How it works

`spawn f(arg)` starts a detached OS thread — the caller continues immediately.

```python
def worker(id: int) -> void:
    print(f"  worker {id} running")

def main():
    spawn worker(1)
    spawn worker(2)
    print("main continues — workers may still be running")
```

**Multi-argument spawn works natively:**
```python
def compute2(a: int, b: int) -> void:
    print(f"  compute2({a}, {b}) = {a * b}")

def main():
    spawn compute2(3, 4)
    spawn compute2(5, 6)
```

The compiler automatically heap-packs the arguments into an array and generates a wrapper function. The spawned function's arguments must satisfy `Sendable` — see the Sendable section below.

### Common Mistakes

**Mistake: spawning a function that accepts a non-Sendable argument.**
```python
class Config:
    pub data: List[int]    # List is NOT Sendable

def process(cfg: Config) -> void:
    ...

def main():
    mut cfg = Config()
    spawn process(cfg)     # [T-1] compile error — Config is not Sendable
```

**Mistake: expecting `spawn` to provide a result.**
```python
spawn compute(x)    # no return value — use Thread.spawn + join if you need a result
```

### Best Practices

1. Only pass `Sendable` values to `spawn`.
2. Use `task_group:` instead of bare `spawn` when you need to wait for the tasks to finish.
3. Use `Thread.spawn` + `.join()` when you need to retrieve results or enforce ordering.

---

## `task_group:` — Structured Concurrency

### When to use

Use `task_group:` when you want to spawn multiple tasks and wait for all of them to complete before continuing. This is the primary structured-concurrency primitive.

### How it works

All threads spawned inside a `task_group:` block are waited on before the block exits. The number of spawns is unlimited.

```python
def compute(n: int) -> void:
    print(f"  compute({n}) = {n * n}")

def main():
    task_group:
        spawn compute(4)
        spawn compute(7)
        spawn compute(12)
        spawn compute(20)
        spawn compute(50)
    print("all compute() calls finished")
```

### Common Mistakes

**Mistake: spawning work outside a `task_group:` when you later expect it to be done.**
```python
spawn compute(4)
spawn compute(7)
print("done?")    # NOT done — the spawned threads may still be running
```
Wrap in `task_group:` if you need the guarantee.

**Mistake: panicking inside a `task_group:` task without using `StructuredGroup`.**
A thread that calls `panic()` inside a `task_group:` will terminate the process. Use `StructuredGroup` (see below) if you need to tolerate panics.

### Best Practices

1. Prefer `task_group:` over bare `spawn` whenever you need completion guarantees.
2. Use `StructuredGroup` instead of `task_group:` when individual task failures should be recoverable.

---

## `await_all` — Parallel Concurrent Calls

### When to use

Use `await_all(f1(args), f2(args), ...)` when you have a fixed set of async calls and want all of them to run in parallel and complete before continuing.

### How it works

`await_all` runs all calls in parallel (via `task_group:` internally) and waits for all to finish.

```python
def heavy(n: int) -> void:
    mut s = 0
    mut i = 0
    while i < n:
        s += i
        i += 1
    print(f"  heavy({n}) sum = {s}")

def main():
    await_all(heavy(1000), heavy(2000), heavy(3000))
    print("all finished (ran in parallel)")
```

### Common Mistakes

**Mistake: using `await_all` for a dynamic number of tasks.**
`await_all` takes a fixed argument list. For a dynamic number of tasks use `task_group:` with a loop:
```python
task_group:
    for item in items:
        spawn process(item)
```

### Best Practices

1. Use `await_all` for a small, known set of parallel calls.
2. For dynamic task counts, use `task_group:` with a loop.

---

## `await_timeout` — Bounded Async Call

### When to use

Use `await_timeout(fn(args), ms)` when you need to run an async function but must not wait longer than a fixed deadline. Typical for network requests, external service calls, or watchdog logic.

### How it works

`await_timeout` returns `true` if the function completed within the deadline, `false` if it timed out. The underlying `_TrTaskState` uses a refcount-2 design so both the caller and the spawned thread can safely free state regardless of who finishes first.

```python
async def slow_fetch(n: int) -> int:
    Thread.sleep(200)
    return n * 2

async def main():
    mut ok = await_timeout(slow_fetch(10), 500)    # true — completes in ~200ms
    if ok:
        print("done in time")
    else:
        print("timed out")

    mut ok2 = await_timeout(slow_fetch(10), 50)    # false — 200ms > 50ms deadline
    if not ok2:
        print("timed out as expected")
```

### Common Mistakes

**Mistake: ignoring the return value.**
```python
await_timeout(slow_fetch(10), 50)    # result discarded — you don't know if it timed out
```

**Mistake: using `await_timeout` to cancel work.** The spawned thread continues running even after a timeout — the function only stops waiting, it does not terminate the thread.

### Best Practices

1. Always check the boolean return value and handle the timed-out case explicitly.
2. Design the async function to be idempotent or safe to run past a timeout.

---

## `Chan[T]` — Typed Buffered Channel

### When to use

Use `Chan[T]` for producer-consumer pipelines where one or more tasks produce values and others consume them. Channels decouple producers from consumers and provide natural backpressure when the buffer is full.

### How it works

```python
def producer(ch: Chan[int]) -> void:
    mut i = 0
    while i < 5:
        ch.send(i * 10)
        i += 1
    ch.close()

def consumer(ch: Chan[int]) -> void:
    mut total = 0
    for v in ch:          # blocks until item available or channel closed
        total += v
    print(f"  total = {total}")    # 0+10+20+30+40 = 100

def main():
    mut ch: Chan[int] = Chan.init(10)    # buffer size 10
    task_group:
        spawn producer(ch)
        spawn consumer(ch)
```

**`Chan[T]` API:**

| Method | Description |
|--------|-------------|
| `Chan.init(n)` | Create buffered channel with capacity `n` |
| `ch.send(v)` | Send value; blocks if buffer full |
| `ch.recv()` | Receive value; blocks if buffer empty |
| `ch.close()` | Signal that no more values will be sent |
| `for v in ch:` | Iterate until channel is closed and drained |

**Channel select** (advanced): The `select:` block lets you wait on multiple channel operations simultaneously:
```python
select:
    case ch1.recv() as x:
        print(f"received from ch1: {x}")
    case ch2.send(val):
        print("sent to ch2")
    case timeout(100):
        print("timed out")
    default:
        print("nothing ready")
```
See [advanced/03_channel_select.md](../advanced/03_channel_select.md) for full documentation.

### Common Mistakes

**Mistake: sending on a closed channel.**
```python
ch.close()
ch.send(42)    # runtime panic — cannot send on a closed channel
```

**Mistake: forgetting to close the channel.** If the consumer uses `for v in ch:`, it will block forever if the producer never calls `ch.close()`.

**Mistake: buffer size 0 (unbuffered).** `Chan.init(0)` creates an unbuffered channel — `send` blocks until a receiver is ready, and `recv` blocks until a sender is ready. This is intentional for rendezvous synchronization but easy to deadlock if misused.

### Best Practices

1. Always close the channel in the producer when it is done sending.
2. Size the buffer to the expected burst size to minimize blocking.
3. Use `for v in ch:` on the consumer side — it handles close detection automatically.

---

## `Mutex[T]` — Thread-Safe Guarded Value

### When to use

Use `Mutex[T]` when multiple threads need to read and write a shared value and you need exclusive access (only one thread at a time).

### How it works

`Mutex[T]` wraps a value and a mutex together. The only way to access the value is through the lock:

```python
def safe_inc(m: Mutex[int]) -> void:
    mut v = m.get()     # acquires lock, returns current value
    m.set(v + 1)        # stores new value, releases lock

def main():
    mut counter: Mutex[int] = Mutex.init(0)
    task_group:
        spawn safe_inc(counter)
        spawn safe_inc(counter)
        spawn safe_inc(counter)
    mut final = counter.get()
    counter.unlock()
    print(f"counter after 3 threads: {final}")    # 3
```

**`Mutex[T]` API:**

| Method | Description |
|--------|-------------|
| `Mutex.init(v)` | Create mutex wrapping initial value `v` |
| `m.get()` | Acquire lock and return current value |
| `m.set(v)` | Store new value and release lock |
| `m.unlock()` | Release lock without storing (use after `get()` if no update needed) |

**Rule:** Every `m.get()` must be followed by either `m.set(v)` or `m.unlock()`.

**Auto-unlock (RAII):** The compiler emits a cleanup guard that releases the lock when the binding goes out of scope — explicit `m.unlock()` is optional:

```python
def safe_read(m: Mutex[int]) -> void:
    mut v = m.get()
    # lock released automatically when v goes out of scope
    print(f"  value = {v}")
```

### Common Mistakes

**Mistake: calling `m.get()` twice without releasing the lock.**
```python
def bad(m: Mutex[int]) -> void:
    mut a = m.get()
    mut b = m.get()    # deadlock — lock is already held by this thread
    m.set(a + b)
    m.unlock()
```

**Mistake: using `shared` for mutable data instead of `Mutex[T]`.**
```python
shared s = obj
s.value += 1    # NOT safe — shared only protects the refcount, not the fields
```
Use `Mutex[T]` to protect mutable fields.

### Best Practices

1. Keep the locked region as short as possible — do not call slow functions between `get()` and `set()`.
2. Rely on auto-unlock: avoid explicit `m.unlock()` unless you have a specific reason.
3. Use `Atomic[int]` for simple counters — it is faster and avoids mutex overhead.

---

## `RwLock[T]` — Reader-Writer Lock

### When to use

Use `RwLock[T]` when reads are frequent and writes are rare. Multiple threads can read concurrently; a write acquires exclusive access.

### How it works

```python
def read_config(rw: RwLock[int]) -> void:
    mut v = rw.read()        # acquires read lock
    rw.read_unlock()         # release read lock
    print(f"  config: {v}")

def main():
    mut config: RwLock[int] = RwLock.init(42)
    task_group:
        spawn read_config(config)
        spawn read_config(config)    # concurrent reads are safe

    mut old = config.write()          # acquires write lock
    config.write_set(100)             # store and release write lock
    print(f"  updated from {old} to 100")
```

**`RwLock[T]` API:**

| Method | Description |
|--------|-------------|
| `RwLock.init(v)` | Create rwlock wrapping initial value `v` |
| `rw.read()` | Acquire read lock, return current value |
| `rw.read_unlock()` | Release read lock |
| `rw.write()` | Acquire write lock, return current value |
| `rw.write_set(v)` | Store new value and release write lock |

### Common Mistakes

**Mistake: forgetting `read_unlock()` before attempting a write.** If a thread holds a read lock and then tries to acquire a write lock, it will deadlock.

### Best Practices

1. Always call `read_unlock()` before acquiring the write lock.
2. Prefer `Mutex[T]` if writes are as frequent as reads — the RwLock overhead is only worthwhile when reads dominate.

---

## `Atomic[T]` — Lock-Free Integer

### When to use

Use `Atomic[int]` for counters, flags, and sequence numbers that multiple threads update concurrently. It is faster than `Mutex[T]` for these patterns because it uses CPU atomic instructions with no OS calls.

### How it works

```python
def inc_task(counter: Atomic[int]) -> void:
    mut i = 0
    while i < 1000:
        counter.add(1)
        i += 1

def main():
    mut counter: Atomic[int] = Atomic.new(0)
    task_group:
        spawn inc_task(counter)
        spawn inc_task(counter)
        spawn inc_task(counter)
    print(f"total: {counter.load()}")    # 3000

    # Compare-and-swap: atomically set to 42 only if current == 0
    mut ok = counter.cas(3000, 42)      # ok=true when swap succeeded
    counter.free()
```

**`Atomic[T]` API:**

| Method | Description |
|--------|-------------|
| `Atomic.new(v)` | Create atomic initialized to `v` |
| `a.load()` | Read current value |
| `a.store(v)` | Write value |
| `a.add(v)` | Atomic add, returns new value |
| `a.sub(v)` | Atomic subtract, returns new value |
| `a.swap(v)` | Exchange — stores `v`, returns old value |
| `a.cas(expected, desired)` | Compare-and-swap — returns `true` if swap happened |
| `a.increment()` | Shorthand for `a.add(1)` |
| `a.decrement()` | Shorthand for `a.sub(1)` |
| `a.fetch_add(n)` | Add `n` atomically, return the value before the add |
| `a.free()` | Free the atomic |

### Common Mistakes

**Mistake: using `Atomic[int]` to guard a multi-field update.**
```python
counter.add(1)
size.add(1)    # these two adds are NOT atomic together — another thread may see one without the other
```
Use `Mutex[T]` when you need to update multiple values atomically.

### Best Practices

1. Use `Atomic[int]` for counters, flags, and reference counts.
2. Use `cas` in a retry loop for lock-free algorithms.
3. Use `Mutex[T]` when you need to guard a complex object or multiple fields together.

---

## `ThreadLocal[T]` — Per-Thread Storage

### When to use

Use `ThreadLocal[T]` when each thread needs its own independent copy of a value — for example, per-thread caches, error buffers, or random-number generator state.

### How it works

```python
def worker(tl: ThreadLocal[int]) -> void:
    tl.set(999)    # only affects THIS thread's slot
    print(f"  worker sees: {tl.get()}")    # 999

def main():
    mut tl: ThreadLocal[int] = ThreadLocal.new(0)
    tl.set(100)    # main thread's value

    mut t: Thread = Thread.spawn(worker, tl)
    t.join()

    print(f"main still sees: {tl.get()}")    # still 100
    tl.free()
```

**`ThreadLocal[T]` API:**

| Method | Description |
|--------|-------------|
| `ThreadLocal.new(v)` | Create thread-local; `v` is the initial value for new threads |
| `tl.get()` | Read current thread's value |
| `tl.set(v)` | Write current thread's value |
| `tl.free()` | Free the storage slot |

### Common Mistakes

**Mistake: expecting a `ThreadLocal` set in one thread to be visible in another.**
```python
tl.set(42)       # main thread
spawn worker(tl) # worker calls tl.get() — gets the initial value (0), NOT 42
```
`ThreadLocal` slots are per-thread. Changes in one thread are never visible in another.

### Best Practices

1. Use `ThreadLocal` to eliminate lock contention for per-thread state.
2. Always call `tl.free()` when the storage is no longer needed.

---

## `Shared[T]` — Reference-Counted Cross-Thread Ownership

### When to use

Use `Shared[T]` when multiple threads need to hold a reference to the same object and you need the object to remain alive as long as any thread holds it. The refcount is thread-safe (uses `_Atomic int` internally).

**Warning:** `Shared[T]` makes the refcount thread-safe, but the underlying data fields are **not** protected. If multiple threads write to the same field, use `Mutex[T]` to guard the data.

### How it works

```python
class Counter:
    pub value: Atomic[int]    # use Atomic to safely mutate from multiple threads

extend Counter:
    pub def init() -> Counter:
        mut c = Counter()
        c.value = Atomic.new(0)
        return c

def increment_shared(c: Counter) -> void:
    c.value.add(1)

def main():
    mut sc = Counter.init()
    shared s1 = sc
    shared s2 = s1    # same underlying object

    task_group:
        spawn increment_shared(s1)
        spawn increment_shared(s2)
    print(f"count: {s1.value.load()}")    # 2
    sc.value.free()
```

### Common Mistakes

**Mistake: treating `Shared[T]` as a thread-safe data container.**
```python
shared s = obj
s.plain_int_field += 1    # data race — shared only protects the refcount
```
Wrap mutable fields in `Atomic[T]` or `Mutex[T]`.

### Best Practices

1. Only share objects whose mutable state is already protected by `Atomic[T]` or `Mutex[T]`.
2. Use `shared` when you need the object to outlive any single thread that holds it.

---

## `Sendable` — Compile-Time Data Race Prevention

### When to use

Declare `implements Sendable` on any class you intend to pass to `spawn`, `Thread.spawn`, `pool.spawn`, or `await_all`. The compiler enforces this statically at every spawn site — error `[T-1]` is raised if a non-Sendable value is passed.

### How it works

```python
from std.async.send import Sendable

class SafeStats implements Sendable:
    pub hits:   Atomic[int]
    pub misses: Atomic[int]
```

Rules:
- A class declaring `implements Sendable` must have **only Sendable fields** — the compiler rejects the definition otherwise.
- Primitive types (`int`, `float`, `bool`, `str`) are always Sendable.
- `Atomic[T]`, `Mutex[T]`, `RwLock[T]`, `Chan[T]`, `Thread`, `ThreadPool`, `ThreadLocal[T]` are always Sendable.
- `List[T]`, `Dict`, `Map` are **never Sendable** — they have no internal synchronization.

**Enforcement:**
```python
class UnsafeStats:
    pub hits: int              # plain int — NOT Sendable

def tally(s: UnsafeStats) -> void:
    s.hits += 1

def main():
    mut s = UnsafeStats()
    spawn tally(s)             # [T-1] compile error
    Thread.spawn(tally, s)     # [T-1] compile error
```

`[T-1]` fires for `spawn`, `Thread.spawn`, `pool.spawn`, and `await_all` sub-calls. It does **not** fire for plain `await f(...)` (sequential — no concurrent access possible).

**Full working example:**
```python
from std.async.send   import Sendable
from std.async.thread import Thread, Atomic

class Counter implements Sendable:
    pub value: Atomic[int]

extend Counter:
    pub def init() -> Counter:
        mut c = Counter()
        c.value = Atomic.new(0)
        return c

def increment(c: Counter) -> void:
    mut i = 0
    while i < 1000:
        c.value.add(1)
        i += 1

def main():
    mut c = Counter.init()
    mut t1: Thread = Thread.spawn(increment, c)
    mut t2: Thread = Thread.spawn(increment, c)
    t1.join()
    t2.join()
    print(f"total: {c.value.load()}")    # 2000
    c.value.free()
```

### Common Mistakes

**Mistake: adding a non-Sendable field to a Sendable class.**
```python
class Bad implements Sendable:
    pub data: List[int]    # compile error — List[int] is not Sendable
```

### Best Practices

1. Design classes that cross thread boundaries around `Atomic[T]` and `Mutex[T]` fields from the start.
2. Mark the class `implements Sendable` as soon as you know it will be passed to `spawn`.

---

## `Thread` — Explicit Joinable Thread

### When to use

Use `Thread.spawn` when you need to wait for a thread to complete (join) and optionally inspect whether it panicked. Use bare `spawn` for fire-and-forget work.

### How it works

```python
def work(ms: int) -> void:
    Thread.sleep(ms)
    print(f"  done after {ms}ms")

def main():
    mut t1: Thread = Thread.spawn(work, 50)
    mut t2: Thread = Thread.spawn(work, 100)
    t1.join()    # wait for t1
    t2.join()    # wait for t2
    print(f"current thread id: {Thread.id()}")
```

**`Thread` API:**

| Method | Description |
|--------|-------------|
| `Thread.spawn(fn, arg)` | Start a new thread running `fn(arg)`, returns handle |
| `t.join()` | Wait for thread to finish |
| `t.detach()` | Detach thread — runs independently, no join needed |
| `t.panicked()` | Returns `true` if the thread ended with a `panic()` |
| `t.panic_msg()` | Returns the panic message if `t.panicked()` is true |
| `t.free()` | Free the thread handle |
| `Thread.sleep(ms)` | Sleep current thread for `ms` milliseconds |
| `Thread.id()` | Return current thread's OS ID |

**Thread panic recovery:**
```python
def fragile_work(n: int) -> void:
    if n < 0:
        panic("negative input not allowed")
    print(f"  computed {n * n}")

def main():
    mut t = Thread.spawn(fragile_work, -1)
    t.join()
    if t.panicked():
        print("thread panicked: " + t.panic_msg())
    t.free()
```

A panic inside a thread never terminates the process — it terminates only that thread and records the message. The per-thread `setjmp` boundary installed by the runtime captures the `longjmp` from `panic`.

### Common Mistakes

**Mistake: calling `t.free()` before `t.join()`.**
Freeing the handle before joining leaves the OS thread dangling.

### Best Practices

1. Always call `t.join()` before `t.free()` unless you explicitly called `t.detach()`.
2. Check `t.panicked()` after join when calling functions that may panic.

---

## `ThreadPool` / `Pool` — Worker Pool

### When to use

Use a thread pool when you have many short jobs that arrive dynamically — creating a new OS thread per job is too expensive. The pool reuses worker threads across jobs.

Use `Pool` (from `std.async.task`) for the standard Tauraro async pool. Use `ThreadPool` for direct low-level pool control.

### How it works

```python
def job(id: int) -> void:
    print(f"  pool job {id} done")

def main():
    mut pool: ThreadPool = ThreadPool.new(4)    # 4 worker threads
    pool.spawn(job, 1)
    pool.spawn(job, 2)
    pool.spawn(job, 3)
    pool.wait()     # wait for all queued jobs to complete
    pool.free()     # shut down workers and free pool
    print("all pool jobs done")
```

**`ThreadPool` API:**

| Method | Description |
|--------|-------------|
| `ThreadPool.new(n)` | Create pool with `n` worker threads |
| `ThreadPool.auto()` | Create pool with one thread per CPU core |
| `pool.spawn(fn, arg)` | Dispatch `fn(arg)` to a worker |
| `pool.wait()` | Block until all queued jobs complete |
| `pool.free()` | Shutdown workers and free pool memory |

**When to use pool vs `task_group:`**
- `task_group:` — fixed, known set of tasks; short-lived
- `ThreadPool` — dynamic stream of tasks arriving over time; threads are reused

### Common Mistakes

**Mistake: forgetting `pool.wait()` before `pool.free()`.**
```python
pool.spawn(job, 1)
pool.free()    # workers may not have finished — undefined behavior
```

### Best Practices

1. Always call `pool.wait()` before `pool.free()`.
2. Use `ThreadPool.auto()` to automatically match the number of workers to CPU core count.

---

## `StructuredGroup` — Fault-Tolerant Batch

### When to use

Use `StructuredGroup` when you need to run a batch of tasks and tolerate individual failures — panicked threads are caught and recorded instead of crashing the process. Use plain `task_group:` when any panic should abort the whole batch.

### How it works

```python
from std.async.structured import StructuredGroup

def risky(id: int) -> void:
    if id == 2:
        panic("task 2 intentionally failed")
    print(f"  task {id} OK")

def main():
    mut sg = StructuredGroup.init()
    sg.spawn(risky, 1)
    sg.spawn(risky, 2)    # will panic — caught by group
    sg.spawn(risky, 3)
    sg.wait()             # blocks until all threads finish (panicked or not)

    print(f"panics: {sg.panic_count}")
    if sg.panic_count > 0:
        print("last panic: " + sg.last_panic_msg)
    sg.free()
```

**`StructuredGroup` API:**

| Method | Description |
|--------|-------------|
| `StructuredGroup.init()` | Create an empty group |
| `sg.spawn(fn, arg)` | Spawn `fn(arg)` in a thread; add to group |
| `sg.wait()` | Wait for all threads; panics are caught, not re-raised |
| `sg.free()` | Free group resources |

**Fields:** `panic_count: int`, `last_panic_msg: str`.

### Common Mistakes

**Mistake: using `StructuredGroup` when a panic should abort everything.**
Use `task_group:` for fail-fast semantics.

### Best Practices

1. After `sg.wait()`, always check `sg.panic_count` and log or handle panicked tasks.
2. Use `task_group:` (fail-fast) by default; switch to `StructuredGroup` only when partial failure is acceptable.

---

## Async I/O — `IOPoll` and `EventLoop`

> **Advanced topic.** For full documentation see [advanced/07_event_loop.md](../advanced/07_event_loop.md).

### When to use

Use `IOPoll` / `EventLoop` when you are building an event-driven server or network application that manages many file descriptors simultaneously with non-blocking I/O. It is backed by `epoll` (Linux), `IOCP` (Windows), and `kqueue` (macOS/BSD).

Use `task_group:` or `ThreadPool` when blocking I/O per thread is acceptable — they are simpler to use correctly.

### How it works

**IOPoll — raw event loop:**
```python
from std.io.poll import IOPoll, IOEvent

mut poll = IOPoll.init()
poll.add(fd, POLLIN, 0)       # watch fd for readable events

mut events = poll.wait(200)   # wait up to 200 ms
mut i = 0
while i < events.len:
    mut ev = events.get(i)
    print(f"  fd={ev.fd} events={ev.events} data={ev.userdata}")
    i += 1
poll.destroy()
```

**EventLoop — reactor style:**
```python
from std.io.event_loop import EventLoop

mut el = EventLoop.init()
el.add_fd(server_fd, POLLIN, 0)
el.run(5)     # run 5 poll cycles with a 100 ms timeout each
el.free()
```

**Non-blocking TCP:**
```python
from std.net.tcp import TcpStream

mut s = TcpStream.connect("127.0.0.1", 9000)
s.set_nonblocking()

mut n = s.recv_nb(256)    # returns immediately; -2 = WOULD_BLOCK
if n == -2:
    print("no data yet")
else:
    print(f"received {n} bytes")
s.close()
```

| Method | Returns | Description |
|--------|---------|-------------|
| `set_nonblocking()` | `bool` | Switch socket to non-blocking mode |
| `recv_nb(cap)` | `int` | Non-blocking receive: byte count, 0=disconnect, -1=error, -2=WOULD_BLOCK |
| `send_nb(data)` | `int` | Non-blocking send: bytes sent, 0=disconnect, -1=error, -2=buffer full |

### Common Mistakes

**Mistake: using `IOPoll` for CPU-bound work.** `IOPoll` is only for I/O readiness events — use `ThreadPool` for CPU work.

### Best Practices

1. Combine `IOPoll` with `ThreadPool` for a complete server: poll for I/O events, dispatch CPU work to the pool.
2. For a full tutorial see [advanced/07_event_loop.md](../advanced/07_event_loop.md).

---

## Thread Safety Summary

| Pattern | Thread safe? | Notes |
|---------|-------------|-------|
| `spawn f(val)` | Yes | Value is passed by copy |
| `Shared[T]` refcount | Yes | Refcount is `_Atomic int` |
| `Chan[T]` send/recv | Yes | Fully synchronized |
| `Mutex[T]` get/set | Yes | Exclusive lock per access |
| `RwLock[T]` read | Yes | Multiple concurrent readers |
| `RwLock[T]` write | Yes | Exclusive write lock |
| `ThreadPool` spawn | Yes | Queue is synchronized |
| `Atomic[int]` ops | Yes | C11 lock-free `_Atomic` |
| `ThreadLocal[T]` | Yes | Per-thread — no sharing |
| `Thread.spawn` | Yes | Joinable OS thread |
| `Shared[T].field` concurrent writes | **No** | Data race — use `Mutex[T]` |
| `List[T]` shared across threads | **No** | No synchronization |
| `StructuredGroup` spawns | Yes | Panics caught at thread boundary |
| `IOPoll` (epoll/IOCP/kqueue) | Yes | Platform-native event poll |
| `TcpStream` non-blocking | Yes | Returns WOULD_BLOCK instead of blocking |

---

Next: [Extern and FFI →](17_extern_and_ffi.md)
