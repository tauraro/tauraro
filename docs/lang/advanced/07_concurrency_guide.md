# Advanced — Concurrency: Models, Primitives, Patterns & Best Practices

> This is the comprehensive concurrency reference. For the per-feature syntax tour see [chapter 16](../16_concurrency.md); for the `Sendable` rules see [advanced/06](06_sendable.md) and for channel `select` see [advanced/03](03_channel_select.md).

---

## Overview

Tauraro gives you **two execution models** plus a **hybrid** that combines them. Picking the right one is the single most important concurrency decision.

| Model | Unit | Scheduling | Parallel? | Best for |
|-------|------|-----------|-----------|----------|
| **OS threads** | `spawn`, `task_group`, `Thread`, `ThreadPool` | OS preemptive | **Yes** (real cores) | CPU-bound work, blocking calls |
| **Green threads / async** | `async`/`await`, `Coro` | Cooperative (per worker) | No (one OS thread) | I/O-bound, many connections |
| **Thread-per-core (hybrid)** | N workers, each a green scheduler + reactor | Preemptive across cores, cooperative within | **Yes** | High-concurrency I/O servers |
| **Work-stealing pool** | `AsyncPool`, `await_all` | N persistent workers, idle ones steal queued work from busy siblings | **Yes** | Many independent async calls, unpredictable load |

The rule of thumb:

- **CPU-bound** (hashing, math, parsing in a loop) → **OS threads**. They run on separate cores, so you get true parallelism.
- **I/O-bound** (sockets, lots of waiting) → **async / green threads**. Thousands of tasks share one OS thread; a task that waits on I/O parks and yields instead of blocking the thread.
- **Both at scale** (a web server) → **thread-per-core async** (e.g. watax `listen_async`): one green-thread scheduler per core, each driving thousands of connections via a non-blocking reactor.
- **Many independent async calls with an unpredictable/uneven amount of work each** (a batch job, a fan-out of API calls) → **`AsyncPool`** (or `await_all`, which is built on it): a small, core-sized pool of workers where an idle one steals queued work from a busy sibling, so load balances automatically instead of pinning each call to a fixed worker or spawning a thread per call.

> Key fact: in Tauraro, `spawn` and `task_group` create **OS threads** (`_tr_thread_start`), while `await`, `Coro.spawn`, and `AsyncPool.spawn` create **green threads** (cooperative coroutines on fibers/`ucontext`). `await_all` and `AsyncPool.spawn` submit those green-thread calls onto a shared, core-sized, work-stealing pool (`AsyncPool`/`_TrDeque`) rather than the plain single-worker scheduler `await` alone uses. Don't confuse any of these.

---

## Model 1 — OS threads (true parallelism)

OS threads are preemptive and run on real cores. Use them when work is CPU-bound or calls a blocking API.

### Spawning

```tauraro
# Fire-and-forget detached OS thread:
spawn worker(42)

# Structured: spawn several, block until ALL finish at the end of the block:
task_group:
    spawn compute(1)
    spawn compute(2)
    spawn compute(3)
# <- execution only continues here once all three threads have joined

# Explicit joinable handle:
mut t: Thread = Thread.spawn(worker, 42)
t.join()        # wait for it
# or t.detach() # let the OS reclaim it

Thread.sleep(10)   # sleep the calling thread (ms)
Thread.id()        # calling thread's OS id

# Bounded pool (cap in-flight work at N threads):
mut pool: ThreadPool = ThreadPool.new(4)
pool.spawn(job, 1)
pool.spawn(job, 2)
pool.wait()
pool.free()
```

`task_group:` is the safest default: it's *structured* — no thread outlives the block, so you can't leak a worker or read a result before it's ready.

### Crossing the thread boundary: `Sendable`

A value handed to another thread (`spawn f(x)`, `Thread.spawn(f, x)`) must be safe to share. Types that are must implement the `Sendable` marker interface, and any mutable field must be a thread-safe primitive (`Atomic[T]`) or wrapped in a `Mutex[T]`/`RwLock[T]`:

```tauraro
class Counter implements Sendable:
    pub value: Atomic[int]          # lock-free, safe to share
```

The compiler enforces this and warns (T-3) if a `Sendable` class has a plain mutable primitive field. See [advanced/06_sendable.md](06_sendable.md).

### Shared-state primitives

```tauraro
# Atomic — lock-free integer (zero OS overhead):
mut a: Atomic[int] = Atomic.new(0)
a.add(1)         # fetch-and-add, returns PREVIOUS value
a.increment()
a.load()
a.store(7)
a.swap(0)        # set, return old
a.cas(0, 42)     # compare-and-swap -> bool
a.free()

# Mutex[T] — guards any value; .get() locks, .set()/.unlock() releases:
mut m: Mutex[int] = Mutex.init(0)
mut v = m.get()          # acquire + read
m.set(v + 1)             # write + release
# (the compiler auto-unlocks a Mutex value at scope exit if you forget)

# RwLock[T] — many readers OR one writer:
mut cfg: RwLock[int] = RwLock.init(42)
mut r = cfg.read(); cfg.read_unlock()
mut old = cfg.write(); cfg.write_set(100)   # write holds exclusive

# ThreadLocal[T] — each thread sees its own copy:
mut tl: ThreadLocal[int] = ThreadLocal.new(0)
tl.set(99); tl.get()
```

**Best practice:** reach for `Atomic` first (counters, flags), then `Mutex` (compound state), then `RwLock` (read-heavy state). A `Mutex` you hold across an I/O call is a latency bug — copy out, unlock, then do the slow thing.

### Coordination primitives (`std.async.*`)

```tauraro
from std.async.waitgroup import WaitGroup   # wait for N tasks to signal done
from std.async.semaphore import Semaphore   # cap concurrent access to N
from std.async.barrier import Barrier       # all-rendezvous at a phase boundary
from std.async.once import Once             # run an init exactly once
from std.async.channel import Channel       # CSP message passing (int payloads)
from std.async.timer import Timer, Ticker   # one-shot / repeating timers -> Channel
```

| Primitive | Use for |
|-----------|---------|
| `WaitGroup` | "wait until these N workers finish" (`.add(n)`, `.done()`, `.wait()`) |
| `Semaphore` | rate-limit / cap concurrency (`.acquire()`, `.release()`, `try_acquire`) |
| `Barrier` | phased/lock-step computation (`.wait()` until all arrive) |
| `Once` | thread-safe lazy init (`.do_once()` returns true to the first caller only) |
| `Channel` | producer/consumer, fan-out/fan-in (`.send`, `.recv`, `try_*`, `*_timeout`, `.close`) |
| `Timer`/`Ticker` | timeouts and periodic work (fire onto a `Channel`) |

There is also the builtin **`Chan[T]`** (typed, used with `Chan.init(cap)`, `for v in ch`) for ergonomic typed channels at the language level.

---

## Model 2 — async / await + green threads (non-blocking I/O)

`async`/`await` runs **cooperative green threads** (stackful coroutines on Windows Fibers / POSIX `ucontext`). Many coroutines share one OS thread; when one awaits I/O it **parks and yields** instead of blocking the thread.

```tauraro
async def fetch(id: int) -> str:
    return f"item-{id}"

async def handle(n: int) -> int:
    mut data = await fetch(n)     # suspends THIS coroutine, runs others
    return len(data)

# Run several async calls concurrently and wait for all of them:
await_all(handle(1), handle(2), handle(3))

# Bounded await:
mut r = await_timeout(handle(99), 500)   # ms
```

### The green-thread API (`std.async.coro`)

For servers and custom event loops, drive coroutines explicitly:

```tauraro
from std.async.coro import Coro

Coro.spawn(handler as Pointer[char], arg as Pointer[char])  # detached green thread
Coro.yield_now()                 # cooperatively give other tasks a turn
Coro.sleep_ms(100)               # park on a timer, yield the worker (non-blocking)
Coro.await_readable(fd)          # park until fd is readable, then resume
Coro.await_writable(fd)
Coro.run()                       # drive the scheduler until all tasks finish
```

### How non-blocking I/O actually works

The scheduler is cooperative; the **non-blocking** part comes from the I/O primitives. `Coro.await_readable(fd)` registers `fd` with the per-thread **reactor** — `epoll` (Linux), `kqueue` (macOS), or `WSAPoll` (Windows) — then suspends the coroutine. The scheduler only blocks the OS thread when *nothing* is runnable, waking the exact coroutine whose socket became ready. `std/net/tcp.tr`'s `recv_async`/`send_async`/`accept_async` are built on this.

> ⚠️ **The #1 async pitfall:** a *blocking* call inside an `async`/coroutine (a blocking `recv`, file read, or `Thread.sleep`) blocks the **whole worker** and every coroutine on it. Use the async variants (`recv_async`, `Coro.sleep_ms`, `Coro.await_readable`) for anything that waits. CPU-heavy work inside a coroutine also starves its peers — offload it to an OS thread (Model 1).

### Tasks & futures

```tauraro
from std.async.task import Task, Future
from std.async.group import TaskGroup
from std.async.structured import StructuredGroup
```

- `Future` / `Task` — a one-shot result you `resolve`/`reject` and `await_`.
- `TaskGroup` — a named set of tasks with `wait_all`, `wait_all_timeout`, `cancel_all`.
- `StructuredGroup` — `go(fn)` several tasks, `wait_all()`, then `ok()` / `rethrow()` to propagate the first panic. Use this when "if any child fails, fail the group."

---

## Model 3 — thread-per-core async (the server sweet spot)

Combine both models: **N OS worker threads, each running its own green-thread scheduler + reactor.** OS threads give you the cores; green threads give you cheap per-connection concurrency; the reactor gives you non-blocking I/O. This is how watax's `listen_async` works.

```tauraro
app.listen_async("0.0.0.0", 8080, 4)   # 4 workers (≈ cores), thousands of conns each
```

watax exposes the whole spectrum so you can match the model to the workload:

| watax API | Model | Use when |
|-----------|-------|----------|
| `listen(host, port)` | single-thread blocking | dev, lowest traffic |
| `listen_threaded(host, port)` | OS thread **per connection** | handlers block (file I/O, slow upstream) and connection count is modest |
| `listen_pooled(host, port, n)` | fixed pool of `n` OS threads | bounded blocking concurrency (back-pressure) |
| `listen_async(host, port, n)` | **thread-per-core green threads** | many concurrent connections, non-blocking handlers — **highest throughput/lowest memory** |
| `listen_tls(host, port, cert, key)` | thread-per-conn + OpenSSL | native HTTPS (blocking TLS) |

---

## Model 4 — work-stealing pool (uneven async workloads)

Thread-per-core (Model 3) is a great fit when work arrives evenly across workers (one accept-loop per worker, roughly equal connection counts). It has no answer for the opposite case: many independent async calls whose individual cost varies, arriving all at once or over time, where a fixed per-worker assignment would leave some workers idle while others fall behind. `AsyncPool` covers that case: N persistent worker threads, each with its own lock-free run queue, where an **idle worker steals a not-yet-started task from a busy sibling's queue** instead of waiting — the same load-balancing model Go's and Tokio's runtimes use.

```tauraro
mut pool = AsyncPool.auto()             # sized to the machine's cores
mut t1 = pool.spawn(fetch_page, url1)
mut t2 = pool.spawn(fetch_page, url2)
t1.join()                               # blocks the calling thread until done
t2.join()
```

`await_all(f1(), f2(), ...)` is built directly on `AsyncPool` — every sub-call is submitted onto the shared pool instead of getting its own OS thread, so a large `await_all` batch (hundreds of simultaneous sub-calls) stays bounded to a handful of worker threads rather than spawning one thread per call. `await_all`'s own syntax, semantics, and Sendable checking are unchanged; only its underlying execution engine changed.

`AsyncPool.spawn`'s argument, like `spawn`/`Thread.spawn`/`ThreadPool.spawn`, must be Sendable (checked at compile time, `[T-1]`/`[T-2]`/`[T-7]`) — the submitted call may end up running on a different worker than the one that called `.spawn`.

---

## Decision guide

```
Is the work mostly waiting on I/O (sockets, pipes)?
├── No  (CPU-bound):           OS threads — task_group + Atomic/Mutex; ThreadPool to bound it.
└── Yes (I/O-bound):
        Few connections / simple?              async def + await.
        Many independent calls, uneven load?   AsyncPool / await_all (work-stealing).
        Many connections (a server)?           thread-per-core async (listen_async / Coro per conn).
        Each handler does blocking work?       thread-per-connection (listen_threaded) or a ThreadPool.
```

| Need | Use |
|------|-----|
| Count something across threads | `Atomic[int]` |
| Protect compound shared state | `Mutex[T]` (or `RwLock[T]` if read-heavy) |
| Pass work/results between tasks | `Channel` / `Chan[T]` |
| "Wait for all of these" | `task_group:` (OS) or `await_all` (async) or `WaitGroup` |
| Limit concurrency to N | `Semaphore` (or `ThreadPool` / `listen_pooled`) |
| Lock-step phases | `Barrier` |
| One-time init | `Once` |
| Timeout an operation | `await_timeout`, `*_timeout` methods, or `Timer` |
| Fail-fast on first error | `StructuredGroup` + `rethrow()` |

---

## Best-practice combinations (by scenario)

### Parallel map-reduce (CPU-bound)
`task_group:` to fan out one thread per chunk, `Atomic[int]` (or per-chunk results + a `Mutex`) to combine. → `examples/concurrency/01_cpu_parallel.tr`

### Producer / consumer & pipelines
`Chan[T]` between stages; producers `close()` the channel, consumers `for v in ch`; a `WaitGroup` to know when all consumers drained. → `examples/concurrency/03_producer_consumer.tr`

### Bounded worker pool / fan-out-fan-in
A jobs `Channel` + a fixed set of worker threads (or `ThreadPool`) + a results `Channel`. Caps memory and concurrency. → `examples/concurrency/04_worker_pool.tr`

### Rate limiting / resource caps
`Semaphore.acquire()` before the protected section, `release()` after (even on error). → `examples/concurrency/05_sync_primitives.tr`

### High-concurrency I/O server
Thread-per-core async: `listen_async` (watax) or `Coro.spawn` one green thread per accepted connection over a non-blocking listener; `Atomic` for metrics. → `examples/concurrency/06_async_green_threads.tr` and `bench/tau_server.tr`

### Fail-fast structured work
`StructuredGroup.go(...)` several children, `wait_all()`, `rethrow()` — the first panic aborts the group. → `examples/concurrency/07_structured.tr`

---

## Common pitfalls (read before shipping)

1. **`spawn` is an OS thread, not a coroutine.** Don't `spawn` thousands of them — use a `ThreadPool`, or green threads for I/O fan-out.
2. **Blocking inside `async`/`Coro` blocks the whole worker.** Use `recv_async`, `Coro.sleep_ms`, `Coro.await_readable` — never a blocking call or `Thread.sleep` in a coroutine.
3. **Holding a `Mutex` across I/O** serializes everyone. Copy out, unlock, then do the slow thing.
4. **Mutating a shared field without `Atomic`/`Mutex`** is a data race even if it "works" in testing. The `Sendable` + T-3 checks catch the common cases — heed them.
5. **`free()` the explicit primitives** (`Atomic`, `Channel`, `WaitGroup`, `Semaphore`, …) when done; `Mutex`/`Chan` builtins auto-unlock but still own memory.
6. **CPU-bound work in a coroutine starves its peers.** Offload to an OS thread.
7. **Detached work outliving its data.** Prefer `task_group:` / `StructuredGroup` (structured) over bare `spawn` / `Coro.spawn` so nothing reads freed state.

---

## See also

- [`examples/concurrency/`](../../../examples/concurrency/) — runnable examples for every pattern above.
- [chapter 16 — Concurrency](../16_concurrency.md) — per-feature syntax reference.
- [advanced/03 — Channel `select`](03_channel_select.md), [advanced/06 — `Sendable`](06_sendable.md).
- `bench/tau_server.tr`, `../tauProject/watax/` — real thread-per-core async servers.
