#include "tauraro_types.h"

bool _expr_is_self_field(Expr* e);
bool _binop_is_float_name(TrStr n);
bool _hl_has_float(List_ptr* hl);
bool _block_mutates_self(Block* b);
bool _pblock_mutates_self(Block** pb);
bool _stmt_mutates_self(Stmt* s);

__attribute__((malloc,returns_nonnull,hot)) Symbol* Symbol_init(TrStr name, SymbolKind kind, AstType** ty) {
    /* pass */
    Symbol* s = ((Symbol*)_tr_obj_alloc(sizeof(Symbol)));
    /* pass */
    s->name = _tr_str_retain(name);
    /* pass */
    s->kind = kind;
    /* pass */
    s->ty = ty;
    /* pass */
    s->scope_depth = 0LL;
    /* pass */
    s->is_mut = false;
    /* pass */
    s->is_const = false;
    /* pass */
    s->is_shared = false;
    /* pass */
    s->is_moved = false;
    /* pass */
    s->active_borrows = 0LL;
    /* pass */
    s->borrowed_by = (void*)List_TrStr_new();
    /* pass */
    s->is_init = true;
    /* pass */
    s->is_param = false;
    /* pass */
    s->is_maybe_moved = false;
    /* pass */
    s->is_maybe_init = false;
    /* pass */
    s->ptr_region = 0LL;
    /* pass */
    s->is_freed = false;
    /* pass */
    s->decl_block_depth = 0LL;
    /* pass */
    s->decl_block_id = 0LL;
    /* pass */
    s->str_escaped = false;
    /* pass */
    s->coll_escaped = false;
    /* pass */
    s->borrows_region = _tr_str_lit("");
    /* pass */
    return s;
}

__attribute__((malloc,returns_nonnull,hot)) Scope* Scope_init() {
    /* pass */
    Scope* s = ((Scope*)_tr_obj_alloc(sizeof(Scope)));
    /* pass */
    s->variables = _tr_dict_new(32LL);
    /* pass */
    s->decl_order = (void*)List_TrStr_new();
    /* pass */
    return s;
}

__attribute__((hot)) AstType** Sema_build_ast_type(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (AstType**)(0LL);
    }
    /* pass */
    __auto_type _t329 = (*e);
    if (_t329.tag == Expr_EIdent) {
        __auto_type n = _t329.data.EIdent.name;
        /* pass */
        return box_asttype(AstType_init(n));
    } else if (_t329.tag == Expr_EIndex) {
        __auto_type obj = _t329.data.EIndex.obj;
__auto_type idx = _t329.data.EIndex._tr_v_index;
        /* pass */
        __auto_type _t330 = (*obj);
        if (_t330.tag == Expr_EIdent) {
            __auto_type on = _t330.data.EIdent.name;
            /* pass */
            AstType* at = AstType_init(on);
            /* pass */
            at->args = (void*)List_ptr_new();
            /* pass */
            __auto_type _t331 = (*idx);
            if (_t331.tag == Expr_ETuple) {
                __auto_type _bte = _t331.data.ETuple.items;
                /* pass */
                long long _bti = 0LL;
                /* pass */
                while ((_bti < _bte->len)) {
                    /* pass */
                    List_ptr_append(at->args, Sema_build_ast_type(self, ((Expr*)List_ptr_get(_bte, _bti))));
                    /* pass */
                    _bti = (_bti + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t331;
                /* pass */
                List_ptr_append(at->args, Sema_build_ast_type(self, idx));
            }
            /* pass */
            return box_asttype(at);
        } else if (1) {
            __auto_type _ = _t330;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t329;
        /* pass */
    }
    /* pass */
    return box_asttype(AstType_init(_tr_str_lit("void")));
}

__attribute__((hot)) AstType** Sema__targ_of(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_asttype(AstType_init(_tr_str_lit("void")));
    }
    /* pass */
    __auto_type _t332 = (*e);
    if (_t332.tag == Expr_EIdent) {
        __auto_type n = _t332.data.EIdent.name;
        return box_asttype(AstType_init(n));
    } else if (_t332.tag == Expr_EIndex) {
        return Sema_build_ast_type(self, e);
    } else if (1) {
        __auto_type _ = _t332;
        return box_asttype(AstType_init(_tr_str_lit("void")));
    }
}

__attribute__((malloc,returns_nonnull,hot)) Sema* Sema_init() {
    /* pass */
    Sema* s = ((Sema*)_tr_obj_alloc(sizeof(Sema)));
    /* pass */
    s->globals = _tr_dict_new(1024LL);
    /* pass */
    s->scopes = (void*)List_ptr_new();
    /* pass */
    s->errors = (void*)List_TrStr_new();
    /* pass */
    s->warnings = (void*)List_TrStr_new();
    /* pass */
    s->classes = _tr_dict_new(128LL);
    /* pass */
    s->enums = _tr_dict_new(64LL);
    /* pass */
    s->interfaces = _tr_dict_new(32LL);
    /* pass */
    s->type_aliases = _tr_dict_new(32LL);
    /* pass */
    s->type_alias_elem = _tr_dict_new(32LL);
    /* pass */
    s->current_file = _tr_str_lit("");
    /* pass */
    s->current_func_name = _tr_str_lit("");
    /* pass */
    s->current_class_name = _tr_str_lit("");
    /* pass */
    s->current_scope_depth = 0LL;
    /* pass */
    s->in_async_fn = false;
    /* pass */
    s->assign_froms = _tr_dict_new(32LL);
    /* pass */
    s->fn_sigs = _tr_dict_new(64LL);
    /* pass */
    s->extern_names = _tr_dict_new(128LL);
    /* pass */
    s->nested_classes = (void*)List_ptr_new();
    /* pass */
    s->nested_functions = (void*)List_ptr_new();
    /* pass */
    s->nested_enums = (void*)List_ptr_new();
    /* pass */
    s->nested_interfaces = (void*)List_ptr_new();
    /* pass */
    s->current_line = 0LL;
    /* pass */
    s->current_func_generics = (void*)List_TrStr_new();
    /* pass */
    s->current_func_constraints = (void*)List_ptr_new();
    /* pass */
    s->closure_boundary = (-1LL);
    /* pass */
    s->closure_caps = (void*)List_ptr_new();
    /* pass */
    s->closure_cap_set = _tr_dict_new(0LL);
    /* pass */
    s->in_assign_target = false;
    /* pass */
    s->in_recv_pos = false;
    /* pass */
    s->container_borrows = _tr_dict_new(16LL);
    /* pass */
    s->capturing_moves = false;
    /* pass */
    s->branch_moved_buf = (void*)List_TrStr_new();
    /* pass */
    s->capturing_inits = false;
    /* pass */
    s->branch_init_buf = (void*)List_TrStr_new();
    /* pass */
    s->copy_classes = _tr_dict_new(32LL);
    /* pass */
    s->in_unsafe = false;
    /* pass */
    s->cur_fn_is_lib = false;
    /* pass */
    s->current_func_ret_from = _tr_str_lit("");
    /* pass */
    s->current_func_ret_borrow_str = false;
    /* pass */
    s->current_func_ret_regions = (void*)List_TrStr_new();
    /* pass */
    s->current_func_outlives_a = (void*)List_TrStr_new();
    /* pass */
    s->current_func_outlives_b = (void*)List_TrStr_new();
    /* pass */
    s->current_region_params = (void*)List_TrStr_new();
    /* pass */
    s->cur_func_borrowers = (void*)List_TrStr_new();
    /* pass */
    s->cur_func_sources = (void*)List_TrStr_new();
    /* pass */
    s->strict_mode = false;
    /* pass */
    s->no_heap = false;
    /* pass */
    s->heap_boxes_tuples = false;
    /* pass */
    s->mutating_methods = _tr_dict_new(32LL);
    /* pass */
    s->fn_ret_owned = _tr_dict_new(64LL);
    /* pass */
    s->fn_param_consumes = _tr_dict_new(64LL);
    /* pass */
    s->ptr_aliased = _tr_dict_new(32LL);
    /* pass */
    s->decorator_names = _tr_dict_new(16LL);
    /* pass */
    s->variadic_fns = _tr_dict_new(8LL);
    /* pass */
    s->variadic_elem_ty = _tr_dict_new(8LL);
    /* pass */
    s->fn_defs = _tr_dict_new(32LL);
    /* pass */
    s->loop_scope_base = (void*)List_i64_new();
    /* pass */
    s->fn_scope_base = (void*)List_i64_new();
    /* pass */
    s->block_depth = 0LL;
    /* pass */
    s->block_depth_stack = (void*)List_i64_new();
    /* pass */
    s->next_block_id = 0LL;
    /* pass */
    s->block_stack = (void*)List_i64_new();
    /* pass */
    s->block_stack_base = (void*)List_i64_new();
    /* pass */
    s->do_temp_ctr = 0LL;
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("print")), Symbol_init(_tr_str_lit("print"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("input")), Symbol_init(_tr_str_lit("input"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("len")), Symbol_init(_tr_str_lit("len"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("range")), Symbol_init(_tr_str_lit("range"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("type")), Symbol_init(_tr_str_lit("type"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("str")), Symbol_init(_tr_str_lit("str"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("hex")), Symbol_init(_tr_str_lit("hex"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("oct")), Symbol_init(_tr_str_lit("oct"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("bin")), Symbol_init(_tr_str_lit("bin"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("kernel_launch")), Symbol_init(_tr_str_lit("kernel_launch"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("int")), Symbol_init(_tr_str_lit("int"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("float")), Symbol_init(_tr_str_lit("float"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("float")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("bool")), Symbol_init(_tr_str_lit("bool"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("bool")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("ord")), Symbol_init(_tr_str_lit("ord"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("chr")), Symbol_init(_tr_str_lit("chr"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("char")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("abs")), Symbol_init(_tr_str_lit("abs"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("max")), Symbol_init(_tr_str_lit("max"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("min")), Symbol_init(_tr_str_lit("min"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("sum")), Symbol_init(_tr_str_lit("sum"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("round")), Symbol_init(_tr_str_lit("round"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("float")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("sorted")), Symbol_init(_tr_str_lit("sorted"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("reversed")), Symbol_init(_tr_str_lit("reversed"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("iter")), Symbol_init(_tr_str_lit("iter"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("enumerate")), Symbol_init(_tr_str_lit("enumerate"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("zip")), Symbol_init(_tr_str_lit("zip"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("map")), Symbol_init(_tr_str_lit("map"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("filter")), Symbol_init(_tr_str_lit("filter"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("all")), Symbol_init(_tr_str_lit("all"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("bool")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("any")), Symbol_init(_tr_str_lit("any"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("bool")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert")), Symbol_init(_tr_str_lit("assert"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_eq")), Symbol_init(_tr_str_lit("assert_eq"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_ne")), Symbol_init(_tr_str_lit("assert_ne"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_lt")), Symbol_init(_tr_str_lit("assert_lt"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_le")), Symbol_init(_tr_str_lit("assert_le"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_gt")), Symbol_init(_tr_str_lit("assert_gt"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("assert_ge")), Symbol_init(_tr_str_lit("assert_ge"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Some")), Symbol_init(_tr_str_lit("Some"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("Option")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Ok")), Symbol_init(_tr_str_lit("Ok"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("Result")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Err")), Symbol_init(_tr_str_lit("Err"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("Result")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("_tr_exit")), Symbol_init(_tr_str_lit("_tr_exit"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("_tr_strlen")), Symbol_init(_tr_str_lit("_tr_strlen"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("read_file")), Symbol_init(_tr_str_lit("read_file"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("str")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("write_file")), Symbol_init(_tr_str_lit("write_file"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("file_exists")), Symbol_init(_tr_str_lit("file_exists"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("bool")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("_tr_system")), Symbol_init(_tr_str_lit("_tr_system"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("int")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("List")), Symbol_init(_tr_str_lit("List"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("List")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Pointer")), Symbol_init(_tr_str_lit("Pointer"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Pointer")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("StringBuilder")), Symbol_init(_tr_str_lit("StringBuilder"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("StringBuilder")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("StringObj")), Symbol_init(_tr_str_lit("StringObj"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("StringObj")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Dict")), Symbol_init(_tr_str_lit("Dict"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Dict")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Set")), Symbol_init(_tr_str_lit("Set"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Set")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Box")), Symbol_init(_tr_str_lit("Box"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Box")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Option")), Symbol_init(_tr_str_lit("Option"), SymbolKind_make_SEnum(), box_asttype(AstType_init(_tr_str_lit("Option")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Result")), Symbol_init(_tr_str_lit("Result"), SymbolKind_make_SEnum(), box_asttype(AstType_init(_tr_str_lit("Result")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Exception")), Symbol_init(_tr_str_lit("Exception"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Exception")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Error")), Symbol_init(_tr_str_lit("Error"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Error")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("ValueError")), Symbol_init(_tr_str_lit("ValueError"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("ValueError")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("TypeError")), Symbol_init(_tr_str_lit("TypeError"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("TypeError")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("IndexError")), Symbol_init(_tr_str_lit("IndexError"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("IndexError")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("IOError")), Symbol_init(_tr_str_lit("IOError"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("IOError")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("KeyError")), Symbol_init(_tr_str_lit("KeyError"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("KeyError")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Task")), Symbol_init(_tr_str_lit("Task"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Task")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Future")), Symbol_init(_tr_str_lit("Future"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Future")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Channel")), Symbol_init(_tr_str_lit("Channel"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Channel")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Chan")), Symbol_init(_tr_str_lit("Chan"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Chan")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Mutex")), Symbol_init(_tr_str_lit("Mutex"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Mutex")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("RwLock")), Symbol_init(_tr_str_lit("RwLock"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("RwLock")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("ThreadPool")), Symbol_init(_tr_str_lit("ThreadPool"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("ThreadPool")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Thread")), Symbol_init(_tr_str_lit("Thread"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Thread")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Atomic")), Symbol_init(_tr_str_lit("Atomic"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Atomic")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("ThreadLocal")), Symbol_init(_tr_str_lit("ThreadLocal"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("ThreadLocal")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("await_all")), Symbol_init(_tr_str_lit("await_all"), SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Arc")), Symbol_init(_tr_str_lit("Arc"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Arc")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Rc")), Symbol_init(_tr_str_lit("Rc"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Rc")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("WaitGroup")), Symbol_init(_tr_str_lit("WaitGroup"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("WaitGroup")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Shared")), Symbol_init(_tr_str_lit("Shared"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Shared")))));
    /* pass */
    _tr_dict_set(s->globals, _tr_strz(_tr_str_lit("Weak")), Symbol_init(_tr_str_lit("Weak"), SymbolKind_make_SClass(), box_asttype(AstType_init(_tr_str_lit("Weak")))));
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Sema_io_ty_str(Sema* self, AstType* ty) {
    /* pass */
    TrStr s = _tr_str_retain(ty->name);
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t333 = _tr_str_lit("void");
        _tr_str_release(s);
        s = _strtmp_t333;
    }
    /* pass */
    if ((ty->args->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t334 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(s);
        s = _strtmp_t334;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < ty->args->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t335 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t335;
            }
            /* pass */
            TrStr _strtmp_t336 = ({ TrStr _cr = (Sema_io_ty_str(self, (*((AstType**)List_ptr_get(ty->args, i))))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t336;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        TrStr _strtmp_t337 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(s);
        s = _strtmp_t337;
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Sema_io_doc_of(Sema* self, Block* body) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < body->stmts->len)) {
        /* pass */
        __auto_type _t338 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t338.tag == Stmt_SLine) {
            __auto_type _ = _t338.data.SLine.n;
            /* pass */
        } else if (_t338.tag == Stmt_SExpr) {
            __auto_type e = _t338.data.SExpr.expr;
            /* pass */
            if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t339 = (*e);
                if (_t339.tag == Expr_ELitStr) {
                    __auto_type s = _t339.data.ELitStr.val;
                    return _tr_str_retain(s);
                } else if (1) {
                    __auto_type _ = _t339;
                    /* pass */
                }
            }
            /* pass */
            return _tr_str_lit("");
        } else if (1) {
            __auto_type _ = _t338;
            return _tr_str_lit("");
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr Sema_io_func_sig(Sema* self, FunctionDef* f) {
    /* pass */
    TrStr s = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("def ")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t340 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t340;
        }
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, i));
        /* pass */
        TrStr _strtmp_t341 = _tr_strx_concat(_tr_strz(s), _tr_strz(p->name));
        _tr_str_release(s);
        s = _strtmp_t341;
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr _strtmp_t342 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (Sema_io_ty_str(self, (*p->ty))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t342;
        }
        /* pass */
        if (p->is_variadic) {
            /* pass */
            TrStr _strtmp_t343 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("...")));
            _tr_str_release(s);
            s = _strtmp_t343;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    TrStr _strtmp_t344 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(")")));
    _tr_str_release(s);
    s = _strtmp_t344;
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        AstType* rt = (*f->ret_ty);
        /* pass */
        if ((((strcmp(_tr_strz(rt->name), _tr_strz(_tr_str_lit(""))) != 0) && (strcmp(_tr_strz(rt->name), _tr_strz(_tr_str_lit("void"))) != 0)) && (strcmp(_tr_strz(rt->name), _tr_strz(_tr_str_lit("None"))) != 0))) {
            /* pass */
            TrStr _strtmp_t345 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (Sema_io_ty_str(self, rt)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t345;
        }
    }
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        if ((strcmp(_tr_strz((*f->throws_ty)->name), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t346 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" throws ")))); TrStr _cr = (Sema_io_ty_str(self, (*f->throws_ty))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t346;
        }
    }
    /* pass */
    TrStr doc = Sema_io_doc_of(self, f->body);
    /* pass */
    if ((strcmp(_tr_strz(doc), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t347 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("\n        \"\"\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(doc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\"\""))); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t347;
    }
    /* pass */
    _tr_str_release(doc);
    return s;
}

__attribute__((hot)) TrStr Sema_build_inspect_str(Sema* self, TrStr name) {
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        ClassDef* c = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
        /* pass */
        TrStr s = _tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(c->name));
        /* pass */
        if ((c->base_classes->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t348 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("(extends ")));
            _tr_str_release(s);
            s = _strtmp_t348;
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < c->base_classes->len)) {
                /* pass */
                if ((bi > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t349 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                    _tr_str_release(s);
                    s = _strtmp_t349;
                }
                /* pass */
                TrStr _strtmp_t350 = ({ TrStr _cr = (List_TrStr_get(c->base_classes, bi)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t350;
                /* pass */
                bi = (bi + 1LL);
            }
            /* pass */
            TrStr _strtmp_t351 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(")")));
            _tr_str_release(s);
            s = _strtmp_t351;
        }
        /* pass */
        TrStr _strtmp_t352 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":\n")));
        _tr_str_release(s);
        s = _strtmp_t352;
        /* pass */
        if ((strcmp(_tr_strz(c->docstring), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t353 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  doc: ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->docstring)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t353;
        }
        /* pass */
        if ((c->fields->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t354 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  fields:\n")));
            _tr_str_release(s);
            s = _strtmp_t354;
            /* pass */
            long long fi = 0LL;
            /* pass */
            while ((fi < c->fields->len)) {
                /* pass */
                FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
                /* pass */
                TrStr _strtmp_t355 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("    ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fld->name)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t355;
                /* pass */
                if ((((unsigned long long)(fld->ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    TrStr _strtmp_t356 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (Sema_io_ty_str(self, (*fld->ty))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(s);
                    s = _strtmp_t356;
                }
                /* pass */
                TrStr _strtmp_t357 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("\n")));
                _tr_str_release(s);
                s = _strtmp_t357;
                /* pass */
                fi = (fi + 1LL);
            }
        }
        /* pass */
        if ((c->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t358 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  methods:\n")));
            _tr_str_release(s);
            s = _strtmp_t358;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < c->methods->len)) {
                /* pass */
                TrStr _strtmp_t359 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(c->methods, mi)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t359;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        EnumDef* e = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(name)));
        /* pass */
        TrStr s = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("enum ")), _tr_strz(e->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((e->variants->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t360 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  variants:\n")));
            _tr_str_release(s);
            s = _strtmp_t360;
            /* pass */
            long long vi = 0LL;
            /* pass */
            while ((vi < e->variants->len)) {
                /* pass */
                VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
                /* pass */
                TrStr _strtmp_t361 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("    ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->name)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t361;
                /* pass */
                if ((v->fields->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t362 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("(")));
                    _tr_str_release(s);
                    s = _strtmp_t362;
                    /* pass */
                    long long vfi = 0LL;
                    /* pass */
                    while ((vfi < v->fields->len)) {
                        /* pass */
                        if ((vfi > 0LL)) {
                            /* pass */
                            TrStr _strtmp_t363 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                            _tr_str_release(s);
                            s = _strtmp_t363;
                        }
                        /* pass */
                        Param* vf = ((Param*)List_ptr_get(v->fields, vfi));
                        /* pass */
                        if ((((unsigned long long)(vf->ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t364 = ({ TrStr _cr = (Sema_io_ty_str(self, (*vf->ty))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
                            _tr_str_release(s);
                            s = _strtmp_t364;
                        }
                        /* pass */
                        vfi = (vfi + 1LL);
                    }
                    /* pass */
                    TrStr _strtmp_t365 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(")")));
                    _tr_str_release(s);
                    s = _strtmp_t365;
                }
                /* pass */
                TrStr _strtmp_t366 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("\n")));
                _tr_str_release(s);
                s = _strtmp_t366;
                /* pass */
                vi = (vi + 1LL);
            }
        }
        /* pass */
        if ((e->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t367 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  methods:\n")));
            _tr_str_release(s);
            s = _strtmp_t367;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < e->methods->len)) {
                /* pass */
                TrStr _strtmp_t368 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(e->methods, mi)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t368;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
        /* pass */
        InterfaceDef* iface = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(name)));
        /* pass */
        TrStr s = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("interface ")), _tr_strz(iface->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((iface->methods->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t369 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("  methods:\n")));
            _tr_str_release(s);
            s = _strtmp_t369;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < iface->methods->len)) {
                /* pass */
                TrStr _strtmp_t370 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("    ")))); TrStr _cr = (Sema_io_func_sig(self, ((FunctionDef*)List_ptr_get(iface->methods, mi)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t370;
                /* pass */
                mi = (mi + 1LL);
            }
        }
        /* pass */
        return s;
    } else if (_tr_dict_contains(self->fn_defs, _tr_strz(name))) {
        /* pass */
        return Sema_io_func_sig(self, ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(name))));
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("int"))) == 0)) {
        /* pass */
        return _tr_str_lit("int: 64-bit signed integer (C long long).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("float"))) == 0)) {
        /* pass */
        return _tr_str_lit("float: 64-bit floating point number (C double).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("bool: true/false (C _Bool).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return _tr_str_lit("str: immutable byte string (C char*).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("char"))) == 0)) {
        /* pass */
        return _tr_str_lit("char: single byte character (C char).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("List"))) == 0)) {
        /* pass */
        return _tr_str_lit("List[T]: growable, ordered, indexable sequence of T.");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Dict"))) == 0)) {
        /* pass */
        return _tr_str_lit("Dict[K, V]: hash map from K to V.");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Set"))) == 0)) {
        /* pass */
        return _tr_str_lit("Set[T]: unordered collection of unique T values.");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Option"))) == 0)) {
        /* pass */
        return _tr_str_lit("Option[T]: either Some(T) or None.");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Result"))) == 0)) {
        /* pass */
        return _tr_str_lit("Result[T, E]: either Ok(T) or Err(E).");
    } else if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return _tr_str_lit("Pointer[T]: raw pointer to a T (C T*); use unsafe: for arithmetic.");
    } else {
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("No info available for '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; });
    }
}

__attribute__((hot)) void Sema_error(Sema* self, TrStr msg) {
    /* pass */
    TrStr loc = _tr_str_lit("");
    /* pass */
    if ((_tr_strlen(_tr_strz(self->current_file)) > 0LL)) {
        /* pass */
        TrStr _strtmp_t371 = _tr_strx_concat(_tr_strz(self->current_file), _tr_strz(_tr_str_lit(":")));
        _tr_str_release(loc);
        loc = _strtmp_t371;
    }
    /* pass */
    if ((self->current_line > 0LL)) {
        /* pass */
        ({ TrStr _at_t372 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->current_line)))); TrStr _cres = _tr_strx_concat(_tr_strz(loc), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(msg)); _tr_str_release(_cl); _cres; })); List_TrStr_append(self->errors, _at_t372); _tr_str_release(_at_t372); });
    } else {
        /* pass */
        List_TrStr_append(self->errors, msg);
    }
    _tr_str_release(loc);
}

__attribute__((hot)) bool Sema_is_sendable_type(Sema* self, TrStr ty_name) {
    /* pass */
    if ((((((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("str"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit(""))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("auto"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("usize"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("isize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Atomic"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("RwLock"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Chan"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Channel"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("ThreadPool"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("ThreadLocal"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Weak"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(ty_name), _tr_strz(_tr_str_lit("Map"))) == 0))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(ty_name))) {
        /* pass */
        ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ty_name)));
        /* pass */
        if ((!_is_invalid_ptr(((unsigned long long)(cls->iface_names))))) {
            /* pass */
            long long ii = 0LL;
            /* pass */
            while ((ii < cls->iface_names->len)) {
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(cls->iface_names, ii)), _tr_strz(_tr_str_lit("Sendable"))) == 0)) {
                    /* pass */
                    return true;
                }
                /* pass */
                ii = (ii + 1LL);
            }
        }
        /* pass */
        return false;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_sendable_ty(Sema* self, AstType* ty) {
    /* pass */
    TrStr n = _tr_str_retain(ty->name);
    /* pass */
    if (((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Channel"))) == 0))) {
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema_is_sendable_ty(self, (*((AstType**)List_ptr_get(ty->args, 0LL))));
        }
        /* pass */
        _tr_str_release(n);
        return true;
    }
    /* pass */
    return Sema_is_sendable_type(self, n);
}

__attribute__((hot)) bool Sema_class_method_exists(Sema* self, TrStr cls_name, TrStr method) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((FunctionDef*)List_ptr_get(cls->methods, mi))->name), _tr_strz(method)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < cls->base_classes->len)) {
        /* pass */
        if (({ TrStr _at_t373 = (List_TrStr_get(cls->base_classes, bi)); __auto_type _wr = (Sema_class_method_exists(self, _at_t373, method)); _tr_str_release(_at_t373); _wr; })) {
            /* pass */
            return true;
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_universal_method(Sema* self, TrStr method) {
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("new"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("free"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_str"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_string"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("as_str"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("length"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__len__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("copy"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__getitem__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_index"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__setitem__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__eq__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__ne__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__lt__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__gt__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__le__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__ge__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__add__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__sub__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__mul__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__div__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__mod__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__hash__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__iter__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__next__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__contains__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__str__"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__repr__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__enter__"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__exit__"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_expr_is_borrow(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (hir_expr_type(e)->is_borrow) {
        /* pass */
        return true;
    }
    /* pass */
    __auto_type _t374 = (*e);
    if (_t374.tag == HirExpr_EIdent) {
        __auto_type nm = _t374.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, nm);
        /* pass */
        if (((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0) && (((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL))))) {
            /* pass */
            if ((*sym->ty)->is_borrow) {
                /* pass */
                return true;
            }
        }
    } else if (1) {
        __auto_type _ = _t374;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__expr_is_shared(Sema* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t375 = (*e);
    if (_t375.tag == HirExpr_EIdent) {
        __auto_type nm = _t375.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, nm);
        /* pass */
        return ((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0) && sym->is_shared);
    } else if (_t375.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t375.data.EMethodCall.obj;
__auto_type meth = _t375.data.EMethodCall.method;
        /* pass */
        if ((strcmp(_tr_strz(meth), _tr_strz(_tr_str_lit("clone"))) == 0)) {
            /* pass */
            return Sema__expr_is_shared(self, _tr_v_recv);
        }
    } else if (1) {
        __auto_type _ = _t375;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema_check_spawn_sendable(Sema* self, HirExpr* e) {
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t376 = (*e);
    if (_t376.tag == HirExpr_ECall) {
        __auto_type cs_args = _t376.data.ECall.args;
        /* pass */
        long long csi = 0LL;
        /* pass */
        while ((csi < cs_args->len)) {
            /* pass */
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_args, csi)))) {
                /* pass */
                Sema_error(self, _tr_str_lit("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow."));
            }
            /* pass */
            AstType* arg_ty = hir_expr_type(((HirExpr*)List_ptr_get(cs_args, csi)));
            /* pass */
            if ((((strcmp(_tr_strz(arg_ty->name), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(arg_ty->name), _tr_strz(_tr_str_lit("Weak"))) == 0)) && (arg_ty->args->len > 0LL))) {
                /* pass */
                TrStr inner_nm = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty->args, 0LL)))->name);
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm))) {
                    /* pass */
                    ({ TrStr _at_t377 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] 'Shared[")), _tr_strz(inner_nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' cannot safely cross thread boundaries because '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inner_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable.\n      FIX: Add 'implements Sendable' to '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inner_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' and ensure all mutable fields use Atomic[T] or Mutex[T]."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t377); _tr_str_release(_at_t377); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_args, csi))))) && (!Sema__is_unsafe_sendable(self, arg_ty->name)))) {
                /* pass */
                ({ TrStr _at_t378 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-7] '")), _tr_strz(arg_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary (this is exactly why Rust's 'Rc' is '!Send').\n      FIX: share it as 'Shared["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (atomic refcount, like Rust's 'Arc'), or 'Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' for exclusive access."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t378); _tr_str_release(_at_t378); });
            } else if ((!Sema_is_sendable_type(self, arg_ty->name))) {
                /* pass */
                ({ TrStr _at_t379 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] Type '")), _tr_strz(arg_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] for exclusive access, or Atomic[T] for counters/flags.\n      Or add 'implements Sendable' to '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' to confirm it is thread-safe."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t379); _tr_str_release(_at_t379); });
            }
            /* pass */
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_args, csi)));
            /* pass */
            csi = (csi + 1LL);
        }
    } else if (_t376.tag == HirExpr_EMethodCall) {
        __auto_type cs_margs = _t376.data.EMethodCall.args;
        /* pass */
        long long csmi = 0LL;
        /* pass */
        while ((csmi < cs_margs->len)) {
            /* pass */
            if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)))) {
                /* pass */
                Sema_error(self, _tr_str_lit("[T-6] a borrow (`ref`/`mut ref`) cannot cross a thread boundary: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow."));
            }
            /* pass */
            AstType* arg_ty2 = hir_expr_type(((HirExpr*)List_ptr_get(cs_margs, csmi)));
            /* pass */
            if ((((strcmp(_tr_strz(arg_ty2->name), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(arg_ty2->name), _tr_strz(_tr_str_lit("Weak"))) == 0)) && (arg_ty2->args->len > 0LL))) {
                /* pass */
                TrStr inner_nm2 = _tr_str_retain((*((AstType**)List_ptr_get(arg_ty2->args, 0LL)))->name);
                /* pass */
                if ((!Sema_is_sendable_type(self, inner_nm2))) {
                    /* pass */
                    ({ TrStr _at_t380 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] 'Shared[")), _tr_strz(inner_nm2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' cannot safely cross thread boundaries because '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inner_nm2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable.\n      FIX: Add 'implements Sendable' to '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(inner_nm2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' and protect mutable fields with Atomic[T] or Mutex[T]."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t380); _tr_str_release(_at_t380); });
                }
            } else if ((((self->strict_mode && Sema__is_rc_class(self, arg_ty2->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(cs_margs, csmi))))) && (!Sema__is_unsafe_sendable(self, arg_ty2->name)))) {
                /* pass */
                ({ TrStr _at_t381 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-7] '")), _tr_strz(arg_ty2->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot cross a thread boundary.\n      FIX: share it as 'Shared["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (atomic refcount) or 'Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' for exclusive access."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t381); _tr_str_release(_at_t381); });
            } else if ((!Sema_is_sendable_type(self, arg_ty2->name))) {
                /* pass */
                ({ TrStr _at_t382 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] Type '")), _tr_strz(arg_ty2->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable and cannot be safely shared across threads.\n      FIX: Wrap in Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arg_ty2->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t382); _tr_str_release(_at_t382); });
            }
            /* pass */
            Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(cs_margs, csmi)));
            /* pass */
            csmi = (csmi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t376;
        /* pass */
    }
}

__attribute__((hot)) void Sema_check_class_sendable_fields(Sema* self, ClassDef* c) {
    /* pass */
    bool has_unsafe = false;
    /* pass */
    long long ui = 0LL;
    /* pass */
    while ((ui < c->iface_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(c->iface_names, ui)), _tr_strz(_tr_str_lit("UnsafeSendable"))) == 0)) {
            /* pass */
            has_unsafe = true;
        }
        /* pass */
        ui = (ui + 1LL);
    }
    /* pass */
    long long cfi = 0LL;
    /* pass */
    while ((cfi < c->fields->len)) {
        /* pass */
        FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, cfi));
        /* pass */
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* fty = (*fd->ty);
            /* pass */
            if ((strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
                /* pass */
                if ((!has_unsafe)) {
                    /* pass */
                    ({ TrStr _at_t383 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-2] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' declares 'implements Sendable' but holds a raw 'Pointer' field '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', which the compiler cannot prove thread-safe.\n      FIX: If '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' wraps an internally-synchronized handle, assert it explicitly: 'implements Sendable, UnsafeSendable'.\n      Otherwise wrap the data in Mutex[T]/Atomic[T], or drop 'implements Sendable'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t383); _tr_str_release(_at_t383); });
                }
            } else if ((!Sema_is_sendable_ty(self, fty))) {
                /* pass */
                TrStr _t2_inner = _tr_str_retain(fty->name);
                /* pass */
                if ((((((strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("Channel"))) == 0)) && (fty->args->len > 0LL))) {
                    /* pass */
                    TrStr _strtmp_t384 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fty->name), _tr_strz(_tr_str_lit("[")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz((*((AstType**)List_ptr_get(fty->args, 0LL)))->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(_t2_inner);
                    _t2_inner = _strtmp_t384;
                }
                /* pass */
                ({ TrStr _at_t385 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-2] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' declares 'implements Sendable' but field '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_t2_inner)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable (non-thread-safe data is reachable across threads).\n      FIX: make the inner type Sendable, wrap it in Mutex[T]/RwLock[T] for guarded access or Atomic[T] for numerics, or drop 'implements Sendable'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t385); _tr_str_release(_at_t385); });
                _tr_str_release(_t2_inner);
            } else if ((((strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("bool"))) == 0))) {
                /* pass */
                ({ TrStr _at_t386 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-3] Sendable class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' has primitive field '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fd->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' that may cause data races if mutated from multiple threads.\n      FIX: Use 'Atomic["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' for safe concurrent mutation, or ensure this field is written only before the object is shared across threads."))); _tr_str_release(_cl); _cres; })); List_TrStr_append(self->warnings, _at_t386); _tr_str_release(_at_t386); });
            }
        }
        /* pass */
        cfi = (cfi + 1LL);
    }
}

__attribute__((hot)) HirExpr* Sema__patch_empty_dict_hint(Sema* self, HirExpr* hv, AstType* hint_ty) {
    /* pass */
    if ((((unsigned long long)(hv)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return hv;
    }
    /* pass */
    if (((strcmp(_tr_strz(hint_ty->name), _tr_strz(_tr_str_lit("Dict"))) != 0) && (strcmp(_tr_strz(hint_ty->name), _tr_strz(_tr_str_lit("Map"))) != 0))) {
        /* pass */
        return hv;
    }
    /* pass */
    if ((hint_ty->args->len < 2LL)) {
        /* pass */
        return hv;
    }
    /* pass */
    __auto_type _t387 = (*hv);
    if (_t387.tag == HirExpr_EDict) {
        __auto_type hk = _t387.data.EDict.keys;
__auto_type hvv = _t387.data.EDict.vals;
        /* pass */
        if ((hk->len == 0LL)) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_EDict(hk, hvv, hint_ty));
        }
        /* pass */
        return hv;
    } else if (1) {
        __auto_type _ = _t387;
        return hv;
    }
}

__attribute__((hot)) void Sema_mark_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            if ((!sym->is_moved)) {
                /* pass */
                sym->is_moved = true;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
                /* pass */
                if (self->capturing_moves) {
                    /* pass */
                    List_TrStr_append(self->branch_moved_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((!sym->is_moved)) {
            /* pass */
            sym->is_moved = true;
            /* pass */
            _tr_dict_set(self->globals, _tr_strz(name), sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_TrStr_append(self->branch_moved_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_mark_freed(Sema* self, TrStr name) {
    /* pass */
    long long mf_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mf_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name)));
            /* pass */
            mf_sym->is_freed = true;
            /* pass */
            mf_sym->is_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mf_i))->variables, _tr_strz(name), mf_sym);
            /* pass */
            if (self->capturing_moves) {
                /* pass */
                List_TrStr_append(self->branch_moved_buf, name);
            }
            /* pass */
            return;
        }
        /* pass */
        mf_i = (mf_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mf_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mf_sym->is_freed = true;
        /* pass */
        mf_sym->is_moved = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mf_sym);
        /* pass */
        if (self->capturing_moves) {
            /* pass */
            List_TrStr_append(self->branch_moved_buf, name);
        }
    }
}

__attribute__((hot)) void Sema_check_not_moved(Sema* self, TrStr name, TrStr ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0) && sym->is_moved)) {
        /* pass */
        ({ TrStr _at_t388 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-1] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t388); _tr_str_release(_at_t388); });
    }
}

__attribute__((hot)) void Sema_mark_borrow(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->active_borrows = (sym->active_borrows + 1LL);
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->active_borrows = (sym->active_borrows + 1LL);
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_borrow(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            if ((sym->active_borrows > 0LL)) {
                /* pass */
                sym->active_borrows = (sym->active_borrows - 1LL);
            }
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            sym->active_borrows = (sym->active_borrows - 1LL);
        }
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_check_no_active_borrows(Sema* self, TrStr name, TrStr ty_name) {
    /* pass */
    if (Sema_is_primitive_name(self, ty_name)) {
        /* pass */
        return;
    }
    /* pass */
    Symbol* sym = Sema_resolve(self, name);
    /* pass */
    if (((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0) && (sym->active_borrows > 0LL))) {
        /* pass */
        ({ TrStr _at_t389 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-2] Cannot move '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' while it is borrowed.\n      FIX: The borrow must end before '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' can be moved."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t389); _tr_str_release(_at_t389); });
    }
}

__attribute__((hot)) void Sema_mark_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            if ((!mi_sym->is_init)) {
                /* pass */
                mi_sym->is_init = true;
                /* pass */
                mi_sym->is_maybe_init = false;
                /* pass */
                _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
                /* pass */
                if (self->capturing_inits) {
                    /* pass */
                    List_TrStr_append(self->branch_init_buf, name);
                }
            }
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        if ((!mi_sym->is_init)) {
            /* pass */
            mi_sym->is_init = true;
            /* pass */
            mi_sym->is_maybe_init = false;
            /* pass */
            _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
            /* pass */
            if (self->capturing_inits) {
                /* pass */
                List_TrStr_append(self->branch_init_buf, name);
            }
        }
    }
}

__attribute__((hot)) void Sema_clear_container_borrow(Sema* self, TrStr var_name) {
    /* pass */
    if (_tr_dict_contains(self->container_borrows, _tr_strz(var_name))) {
        /* pass */
        _tr_dict_remove(self->container_borrows, _tr_strz(var_name));
    }
}

__attribute__((hot)) void Sema_unmark_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->is_moved = false;
            /* pass */
            sym->is_freed = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->is_moved = false;
        /* pass */
        sym->is_freed = false;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_moved(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->is_maybe_moved = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        sym->is_maybe_moved = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) void Sema_unmark_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            mi_sym->is_init = false;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mi_sym->is_init = false;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) void Sema_mark_maybe_init(Sema* self, TrStr name) {
    /* pass */
    long long mi_i = (self->scopes->len - 1LL);
    /* pass */
    while ((mi_i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name)));
            /* pass */
            mi_sym->is_maybe_init = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, mi_i))->variables, _tr_strz(name), mi_sym);
            /* pass */
            return;
        }
        /* pass */
        mi_i = (mi_i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        Symbol* mi_sym = ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
        /* pass */
        mi_sym->is_maybe_init = true;
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), mi_sym);
    }
}

__attribute__((hot)) bool Sema_vec_str_contains(Sema* self, List_TrStr* v, TrStr s) {
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < v->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(v, vi)), _tr_strz(s)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_has_copy_decorator(Sema* self, List_ptr* decs) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < decs->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((Decorator*)List_ptr_get(decs, i))->name), _tr_strz(_tr_str_lit("copy"))) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_copy_class(Sema* self, TrStr name) {
    /* pass */
    if (Sema_is_primitive_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->copy_classes, _tr_strz(name))) {
        /* pass */
        return ((bool)(uintptr_t)_tr_dict_get(self->copy_classes, _tr_strz(name)));
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(name)))) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_dict_set(self->copy_classes, _tr_strz(name), false);
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(name)));
    /* pass */
    if ((cls->fields->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    bool all_copy = true;
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < cls->fields->len)) {
        /* pass */
        FieldDef* fd = ((FieldDef*)List_ptr_get(cls->fields, fi));
        /* pass */
        if ((((unsigned long long)(fd->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* fty = (*fd->ty);
            /* pass */
            if ((!fty->is_borrow)) {
                /* pass */
                if ((!Sema_is_copy_class(self, fty->name))) {
                    /* pass */
                    all_copy = false;
                }
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    _tr_dict_set(self->copy_classes, _tr_strz(name), all_copy);
    /* pass */
    return all_copy;
}

__attribute__((hot)) void Sema_enter_scope(Sema* self) {
    /* pass */
    Scope* s_obj = Scope_init();
    /* pass */
    List_ptr_append(self->scopes, _tr_obj_retain(s_obj));
    /* pass */
    self->current_scope_depth = (self->current_scope_depth + 1LL);
    /* pass */
    List_i64_append(self->block_depth_stack, self->block_depth);
    /* pass */
    self->block_depth = 0LL;
    /* pass */
    List_i64_append(self->block_stack_base, self->block_stack->len);
    _tr_obj_release(s_obj, _trdrop_Scope);
}

__attribute__((hot)) void Sema_exit_scope(Sema* self) {
    /* pass */
    ((Scope*)List_ptr_pop(self->scopes));
    /* pass */
    self->current_scope_depth = (self->current_scope_depth - 1LL);
    /* pass */
    self->block_depth = List_i64_get(self->block_depth_stack, (self->block_depth_stack->len - 1LL));
    /* pass */
    List_i64_pop(self->block_depth_stack);
    /* pass */
    long long base = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    /* pass */
    while ((self->block_stack->len > base)) {
        /* pass */
        List_i64_pop(self->block_stack);
    }
    /* pass */
    List_i64_pop(self->block_stack_base);
}

__attribute__((hot)) bool Sema__coll_elem_droppable(Sema* self, TrStr n) {
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("char"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i64"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("usize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f32"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
        /* pass */
        if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((((((((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Result"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringObj"))) == 0))) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__scan_ptr_aliased_ty(Sema* self, AstType* t) {
    /* pass */
    if (((strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (t->args->len > 0LL))) {
        /* pass */
        AstType* inner = (*((AstType**)List_ptr_get(t->args, 0LL)));
        /* pass */
        _tr_dict_set(self->ptr_aliased, _tr_strz(inner->name), true);
        /* pass */
        Sema__scan_ptr_aliased_ty(self, inner);
        /* pass */
        return;
    }
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < t->args->len)) {
        /* pass */
        Sema__scan_ptr_aliased_ty(self, (*((AstType**)List_ptr_get(t->args, ai))));
        /* pass */
        ai = (ai + 1LL);
    }
}

__attribute__((hot)) bool Sema__is_rc_class(Sema* self, TrStr n) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)))->is_class)) {
        /* pass */
        return false;
    }
    /* pass */
    if (((((((((((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Box"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Result"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringObj"))) == 0))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_tr_dict_contains(self->ptr_aliased, _tr_strz(n))) {
        /* pass */
        return false;
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool Sema__ty_reaches_plain_rc(Sema* self, AstType* ft, long long depth) {
    /* pass */
    if ((depth > 24LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr n = _tr_str_retain(ft->name);
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Pointer"))) == 0))) {
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if (((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Mutex"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Box"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Option"))) == 0))) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), (depth + 1LL));
        }
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0))) {
        /* pass */
        if ((ft->args->len > 1LL)) {
            /* pass */
            _tr_str_release(n);
            return Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), (depth + 1LL));
        }
        /* pass */
        _tr_str_release(n);
        return false;
    }
    /* pass */
    if (Sema__is_rc_class(self, n)) {
        /* pass */
        _tr_str_release(n);
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(n))) {
        /* pass */
        return Sema__sendable_reaches_plain_rc(self, n, (depth + 1LL));
    }
    /* pass */
    _tr_str_release(n);
    return false;
}

__attribute__((hot)) bool Sema__sendable_reaches_plain_rc(Sema* self, TrStr tn, long long depth) {
    /* pass */
    if ((depth > 24LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cls->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(cls->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (Sema__ty_reaches_plain_rc(self, (*f->ty), (depth + 1LL))) {
                /* pass */
                return true;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__is_unsafe_sendable(Sema* self, TrStr n) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(n)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(n)));
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(cls->iface_names)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cls->iface_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(cls->iface_names, i)), _tr_strz(_tr_str_lit("UnsafeSendable"))) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__check_spawn_nested_rc(Sema* self, HirExpr* arg_expr) {
    /* pass */
    if ((!self->strict_mode)) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(arg_expr)))) {
        /* pass */
        return;
    }
    /* pass */
    AstType* at = hir_expr_type(arg_expr);
    /* pass */
    TrStr n = _tr_str_retain(at->name);
    /* pass */
    TrStr _cross_cls = _tr_str_retain(n);
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0)) && (at->args->len > 0LL))) {
        /* pass */
        TrStr _strtmp_t390 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
        _tr_str_release(_cross_cls);
        _cross_cls = _strtmp_t390;
    }
    /* pass */
    if (Sema__is_unsafe_sendable(self, _cross_cls)) {
        /* pass */
        _tr_str_release(n);
        _tr_str_release(_cross_cls);
        return;
    }
    /* pass */
    bool reaches = false;
    /* pass */
    if (Sema__expr_is_shared(self, arg_expr)) {
        /* pass */
        TrStr inner = _tr_str_retain(n);
        /* pass */
        if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0)) && (at->args->len > 0LL))) {
            /* pass */
            TrStr _strtmp_t391 = _tr_str_retain((*((AstType**)List_ptr_get(at->args, 0LL)))->name);
            _tr_str_release(inner);
            inner = _strtmp_t391;
        }
        /* pass */
        reaches = Sema__sendable_reaches_plain_rc(self, inner, 0LL);
        _tr_str_release(inner);
    } else if ((((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Mutex"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Box"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Chan"))) == 0)) && (at->args->len > 0LL))) {
        /* pass */
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 0LL))), 0LL);
    } else if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) && (at->args->len > 1LL))) {
        /* pass */
        reaches = Sema__ty_reaches_plain_rc(self, (*((AstType**)List_ptr_get(at->args, 1LL))), 0LL);
    }
    /* pass */
    if (reaches) {
        /* pass */
        ({ TrStr _at_t392 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-7] a value crossing the thread boundary transitively holds a plain (non-atomic) reference-counted class inside '")), _tr_strz(n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' (e.g. via a Mutex/Vec/field). Even wrapped in Shared/Sendable, that nested class's refcount is thread-local and would race across threads.\n      FIX: make the nested state 'Shared[T]' (atomic, like Rust's 'Arc'), or restructure so only one thread ever owns it."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t392); _tr_str_release(_at_t392); });
    }
    _tr_str_release(n);
    _tr_str_release(_cross_cls);
}

__attribute__((hot)) void Sema__collect_strong_edge(Sema* self, AstType* ft, List_TrStr* out) {
    /* pass */
    TrStr n = _tr_str_retain(ft->name);
    /* pass */
    if (ft->is_borrow) {
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Pointer"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Weak"))) == 0))) {
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Set"))) == 0))) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 0LL))), out);
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0))) {
        /* pass */
        if ((ft->args->len > 1LL)) {
            /* pass */
            Sema__collect_strong_edge(self, (*((AstType**)List_ptr_get(ft->args, 1LL))), out);
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Shared"))) == 0)) {
        /* pass */
        if ((ft->args->len > 0LL)) {
            /* pass */
            TrStr si = _tr_str_retain((*((AstType**)List_ptr_get(ft->args, 0LL)))->name);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(si))) {
                /* pass */
                List_TrStr_append(out, si);
            }
        }
        /* pass */
        _tr_str_release(n);
        return;
    }
    /* pass */
    if (Sema__is_rc_class(self, n)) {
        /* pass */
        List_TrStr_append(out, n);
    }
    _tr_str_release(n);
}

__attribute__((hot)) List_TrStr* Sema__strong_owned(Sema* self, ClassDef* cd) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < cd->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(cd->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__collect_strong_edge(self, (*f->ty), out);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return out;
}

__attribute__((hot)) void Sema__cycle_dfs(Sema* self, TrStr cur, TrStr start, TrMap* on_path, TrMap* reported, long long depth) {
    /* pass */
    if ((depth > 200LL)) {
        /* pass */
        return;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cur)))) {
        /* pass */
        return;
    }
    /* pass */
    List_TrStr* edges = Sema__strong_owned(self, ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cur))));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < edges->len)) {
        /* pass */
        TrStr nxt = List_TrStr_get(edges, i);
        /* pass */
        if ((strcmp(_tr_strz(nxt), _tr_strz(start)) == 0)) {
            /* pass */
            if ((!_tr_dict_contains(reported, _tr_strz(start)))) {
                /* pass */
                _tr_dict_set(reported, _tr_strz(start), true);
                /* pass */
                ({ TrStr _at_t393 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[S-2] class '")), _tr_strz(start))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is part of a strong reference CYCLE (via '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' -> '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nxt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'), which reference counting cannot reclaim — it would leak.\n      FIX: make one back-edge non-owning with 'Weak["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(start)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (a weak reference) or 'Pointer["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(start)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (raw, manual)."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t393); _tr_str_release(_at_t393); });
            }
        } else if ((!_tr_dict_contains(on_path, _tr_strz(nxt)))) {
            /* pass */
            _tr_dict_set(on_path, _tr_strz(nxt), true);
            /* pass */
            Sema__cycle_dfs(self, nxt, start, on_path, reported, (depth + 1LL));
            /* pass */
            _tr_dict_set(on_path, _tr_strz(nxt), false);
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(nxt);
    }
    List_TrStr_free(edges);
}

__attribute__((hot)) void Sema_check_ownership_cycles(Sema* self, Program* prog) {
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < prog->decls->len)) {
        /* pass */
        __auto_type _t394 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t394.tag == Decl_DClass) {
            __auto_type c = _t394.data.DClass.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t394.tag == Decl_DActor) {
            __auto_type c = _t394.data.DActor.cls;
            Sema__scan_class_ptrs(self, c);
        } else if (_t394.tag == Decl_DExtend) {
            __auto_type ms = _t394.data.DExtend.methods;
            /* pass */
            long long mi = 0LL;
            /* pass */
            while ((mi < ms->len)) {
                /* pass */
                Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(ms, mi)));
                /* pass */
                mi = (mi + 1LL);
            }
        } else if (_t394.tag == Decl_DFunction) {
            __auto_type f = _t394.data.DFunction.func;
            Sema__scan_fn_ptrs(self, f);
        } else if (1) {
            __auto_type _ = _t394;
            /* pass */
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    TrMap* reported = _tr_dict_new(16LL);
    /* pass */
    di = 0LL;
    /* pass */
    while ((di < prog->decls->len)) {
        /* pass */
        __auto_type _t395 = (*((Decl*)List_ptr_get(prog->decls, di)));
        if (_t395.tag == Decl_DClass) {
            __auto_type c = _t395.data.DClass.cls;
            /* pass */
            if (Sema__is_rc_class(self, c->name)) {
                /* pass */
                TrMap* on_path = _tr_dict_new(16LL);
                /* pass */
                _tr_dict_set(on_path, _tr_strz(c->name), true);
                /* pass */
                Sema__cycle_dfs(self, c->name, c->name, on_path, reported, 0LL);
            }
        } else if (1) {
            __auto_type _ = _t395;
            /* pass */
        }
        /* pass */
        di = (di + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_class_ptrs(Sema* self, ClassDef* c) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__scan_ptr_aliased_ty(self, (*f->ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        Sema__scan_fn_ptrs(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__scan_fn_ptrs(Sema* self, FunctionDef* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->params->len)) {
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, i));
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema__scan_ptr_aliased_ty(self, (*p->ty));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        Sema__scan_ptr_aliased_ty(self, (*f->ret_ty));
    }
}

__attribute__((hot)) bool Sema_is_droppable_sym(Sema* self, Symbol* sym) {
    /* pass */
    if ((sym->kind.tag != SymbolKind_make_SVariable().tag)) {
        /* pass */
        return false;
    }
    /* pass */
    if (sym->is_param) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((unsigned long long)(sym->ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (sym->is_shared) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp(_tr_strz((*sym->ty)->name), _tr_strz(_tr_str_lit("Weak"))) == 0)) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            return false;
        }
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp(_tr_strz(sym->borrows_region), _tr_strz(_tr_str_lit("@borrowed"))) == 0)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr tn = _tr_str_retain((*sym->ty)->name);
    /* pass */
    if ((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        if (sym->str_escaped) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        _tr_str_release(tn);
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
        /* pass */
        if (sym->coll_escaped) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        List_ptr* targs = (*sym->ty)->args;
        /* pass */
        if (((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Map"))) == 0))) {
            /* pass */
            if ((targs->len < 2LL)) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            TrStr kt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
            /* pass */
            TrStr vt = _tr_str_retain((*((AstType**)List_ptr_get(targs, 1LL)))->name);
            /* pass */
            if ((!Sema__coll_elem_droppable(self, kt))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
            /* pass */
            if ((!Sema__coll_elem_droppable(self, vt))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(kt);
                _tr_str_release(vt);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(kt);
            _tr_str_release(vt);
            return true;
        } else {
            /* pass */
            if ((targs->len < 1LL)) {
                /* pass */
                _tr_str_release(tn);
                return false;
            }
            /* pass */
            TrStr et = _tr_str_retain((*((AstType**)List_ptr_get(targs, 0LL)))->name);
            /* pass */
            if ((!Sema__coll_elem_droppable(self, et))) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(et);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(et);
            return true;
        }
    }
    /* pass */
    if ((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Mutex"))) == 0)) {
        /* pass */
        if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if ((sym->active_borrows > 0LL)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        if (((!sym->is_init) || sym->is_maybe_init)) {
            /* pass */
            _tr_str_release(tn);
            return false;
        }
        /* pass */
        _tr_str_release(tn);
        return true;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(tn)))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if ((((((((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Result"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Chan"))) == 0))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("StringBuilder"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("StringObj"))) == 0))) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((sym->is_moved || sym->is_maybe_moved) || sym->is_freed)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if ((sym->active_borrows > 0LL)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    if (((!sym->is_init) || sym->is_maybe_init)) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(tn)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    bool has_dispose = false;
    /* pass */
    while ((mi < cls->methods->len)) {
        /* pass */
        TrStr mnm = _tr_str_retain(((FunctionDef*)List_ptr_get(cls->methods, mi))->name);
        /* pass */
        if ((strcmp(_tr_strz(mnm), _tr_strz(_tr_str_lit("free"))) == 0)) {
            /* pass */
            if (sym->coll_escaped) {
                /* pass */
                _tr_str_release(tn);
                _tr_str_release(mnm);
                return false;
            }
            /* pass */
            _tr_str_release(tn);
            _tr_str_release(mnm);
            return true;
        }
        /* pass */
        if ((strcmp(_tr_strz(mnm), _tr_strz(_tr_str_lit("dispose"))) == 0)) {
            /* pass */
            has_dispose = true;
        }
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(mnm);
    }
    /* pass */
    if (has_dispose) {
        /* pass */
        _tr_str_release(tn);
        return false;
    }
    /* pass */
    _tr_str_release(tn);
    return true;
}

__attribute__((hot)) void Sema_open_block(Sema* self) {
    /* pass */
    self->next_block_id = (self->next_block_id + 1LL);
    /* pass */
    List_i64_append(self->block_stack, self->next_block_id);
}

__attribute__((hot)) void Sema_close_block(Sema* self) {
    /* pass */
    List_i64_pop(self->block_stack);
}

__attribute__((hot)) bool Sema_block_stack_contains(Sema* self, long long id) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->block_stack->len)) {
        /* pass */
        if ((List_i64_get(self->block_stack, i) == id)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) List_ptr* Sema_compute_scope_drops(Sema* self, long long scope_idx, bool is_innermost) {
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    if (((scope_idx < 0LL) || (scope_idx >= self->scopes->len))) {
        /* pass */
        return out;
    }
    /* pass */
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, scope_idx));
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < scope->decl_order->len)) {
        /* pass */
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
        /* pass */
        if (_tr_dict_contains(scope->variables, _tr_strz(nm))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
            /* pass */
            if ((((unsigned long long)(sym->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                bool visible = (sym->decl_block_id == 0LL);
                /* pass */
                if (((!visible) && is_innermost)) {
                    /* pass */
                    visible = Sema_block_stack_contains(self, sym->decl_block_id);
                }
                /* pass */
                if (visible) {
                    /* pass */
                    if (Sema_is_droppable_sym(self, sym)) {
                        /* pass */
                        List_ptr_append(out, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
                    }
                }
            }
        }
        /* pass */
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
    /* pass */
    return out;
}

__attribute__((hot)) void Sema_append_drops_from(Sema* self, HirBlock* hb, long long from_idx) {
    /* pass */
    Sema_append_drops_from_excl(self, hb, from_idx, _tr_str_lit(""));
}

__attribute__((hot)) void Sema_append_drops_from_excl(Sema* self, HirBlock* hb, long long from_idx, TrStr exclude) {
    /* pass */
    List_TrStr* excl_list = (void*)List_TrStr_new();
    /* pass */
    if ((strcmp(_tr_strz(exclude), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        List_TrStr_append(excl_list, exclude);
    }
    /* pass */
    Sema_append_drops_from_excl_multi(self, hb, from_idx, excl_list);
}

__attribute__((hot)) void Sema_append_drops_from_excl_multi(Sema* self, HirBlock* hb, long long from_idx, List_TrStr* excludes) {
    /* pass */
    long long idx = (self->scopes->len - 1LL);
    /* pass */
    while ((idx >= from_idx)) {
        /* pass */
        bool is_innermost = (idx == (self->scopes->len - 1LL));
        /* pass */
        List_ptr* drops = Sema_compute_scope_drops(self, idx, is_innermost);
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < drops->len)) {
            /* pass */
            HirStmt* d = ((HirStmt*)List_ptr_get(drops, di));
            /* pass */
            bool skip = false;
            /* pass */
            __auto_type _t396 = (*d);
            if (_t396.tag == HirStmt_SAutoDrop) {
                __auto_type dn = _t396.data.SAutoDrop.name;
                /* pass */
                long long ei = 0LL;
                /* pass */
                while ((ei < excludes->len)) {
                    /* pass */
                    if ((strcmp(_tr_strz(List_TrStr_get(excludes, ei)), _tr_strz(dn)) == 0)) {
                        /* pass */
                        skip = true;
                    }
                    /* pass */
                    ei = (ei + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t396;
                /* pass */
            }
            /* pass */
            if ((!skip)) {
                /* pass */
                HirBlock_push(hb, d);
            }
            /* pass */
            di = (di + 1LL);
        }
        /* pass */
        idx = (idx - 1LL);
    }
}

__attribute__((hot)) void Sema_collect_idents(Sema* self, HirExpr* e, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t397 = (*e);
    if (_t397.tag == HirExpr_EIdent) {
        __auto_type nm = _t397.data.EIdent.name;
        List_TrStr_append(out, nm);
    } else if (_t397.tag == HirExpr_EBinOp) {
        __auto_type l = _t397.data.EBinOp.left;
__auto_type r = _t397.data.EBinOp.right;
        /* pass */
        Sema_collect_idents(self, l, out);
        /* pass */
        Sema_collect_idents(self, r, out);
    } else if (_t397.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t397.data.EUnaryOp.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t397.tag == HirExpr_ECall) {
        __auto_type callee = _t397.data.ECall.callee;
__auto_type args = _t397.data.ECall.args;
        /* pass */
        Sema_collect_idents(self, callee, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t397.data.EMethodCall.obj;
__auto_type args = _t397.data.EMethodCall.args;
        /* pass */
        Sema_collect_idents(self, obj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(args, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t397.data.EPropAccess.obj;
        Sema_collect_idents(self, obj, out);
    } else if (_t397.tag == HirExpr_EIndex) {
        __auto_type obj = _t397.data.EIndex.obj;
__auto_type idx = _t397.data.EIndex._tr_v_index;
        /* pass */
        Sema_collect_idents(self, obj, out);
        /* pass */
        Sema_collect_idents(self, idx, out);
    } else if (_t397.tag == HirExpr_ECast) {
        __auto_type inner = _t397.data.ECast.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t397.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t397.data.ETryExpr.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t397.tag == HirExpr_EIfElse) {
        __auto_type c = _t397.data.EIfElse.cond;
__auto_type t = _t397.data.EIfElse.then_e;
__auto_type f = _t397.data.EIfElse.else_e;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_idents(self, t, out);
        /* pass */
        Sema_collect_idents(self, f, out);
    } else if (_t397.tag == HirExpr_EList) {
        __auto_type items = _t397.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_ESet) {
        __auto_type items = _t397.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_ETuple) {
        __auto_type items = _t397.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(items, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_EDict) {
        __auto_type keys = _t397.data.EDict.keys;
__auto_type vals = _t397.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(keys, i)), out);
            /* pass */
            Sema_collect_idents(self, ((HirExpr*)List_ptr_get(vals, i)), out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t397.tag == HirExpr_ERange) {
        __auto_type s = _t397.data.ERange.start;
__auto_type en = _t397.data.ERange.end;
        /* pass */
        Sema_collect_idents(self, s, out);
        /* pass */
        Sema_collect_idents(self, en, out);
    } else if (_t397.tag == HirExpr_EAwait) {
        __auto_type inner = _t397.data.EAwait.expr;
        Sema_collect_idents(self, inner, out);
    } else if (_t397.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t397.data.EAwaitTimeout.expr;
__auto_type to = _t397.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_collect_idents(self, inner, out);
        /* pass */
        Sema_collect_idents(self, to, out);
    } else if (_t397.tag == HirExpr_EYield) {
        __auto_type inner = _t397.data.EYield.expr;
        Sema_collect_idents(self, inner, out);
    } else if (1) {
        __auto_type _ = _t397;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_local_var(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema_mark_str_escaped(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->str_escaped = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_set_borrows_region(Sema* self, TrStr name, TrStr region) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->borrows_region = _tr_str_retain(region);
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) TrStr Sema_compute_region(Sema* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t398 = (*e);
    if (_t398.tag == Expr_EBinOp) {
        __auto_type cr_op = _t398.data.EBinOp.op;
        /* pass */
        if ((strcmp(_tr_strz(cr_op), _tr_strz(_tr_str_lit("+"))) == 0)) {
            /* pass */
            return _tr_str_lit("@owned");
        }
        /* pass */
        return _tr_str_lit("");
    } else if (_t398.tag == Expr_EIdent) {
        __auto_type cr_nm = _t398.data.EIdent.name;
        /* pass */
        Symbol* cr_sym = Sema_resolve(self, cr_nm);
        /* pass */
        if ((strcmp(_tr_strz(cr_sym->name), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        if (cr_sym->is_param) {
            /* pass */
            return _tr_str_retain(cr_nm);
        }
        /* pass */
        return _tr_str_retain(cr_sym->borrows_region);
    } else if (_t398.tag == Expr_ECall) {
        __auto_type cr_callee = _t398.data.ECall.callee;
__auto_type cr_args = _t398.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(cr_callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t399 = (*cr_callee);
            if (_t399.tag == Expr_EIdent) {
                __auto_type cr_fn = _t399.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->classes, _tr_strz(cr_fn))) {
                    /* pass */
                    return _tr_str_lit("@owned");
                }
                /* pass */
                Symbol* cr_fsym = Sema_resolve(self, cr_fn);
                /* pass */
                if (((strcmp(_tr_strz(cr_fsym->name), _tr_strz(_tr_str_lit(""))) != 0) && (((unsigned long long)(cr_fsym->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    AstType* cr_ret = (*cr_fsym->ty);
                    /* pass */
                    if (cr_ret->is_borrow) {
                        /* pass */
                        if (((cr_ret->from_index >= 0LL) && (cr_ret->from_index < cr_args->len))) {
                            /* pass */
                            return Sema_compute_region(self, ((Expr*)List_ptr_get(cr_args, cr_ret->from_index)));
                        }
                        /* pass */
                        return _tr_str_lit("");
                    }
                    /* pass */
                    return _tr_str_lit("@owned");
                }
            } else if (1) {
                __auto_type _ = _t399;
                /* pass */
            }
        }
        /* pass */
        return _tr_str_lit("");
    } else if (1) {
        __auto_type _ = _t398;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool Sema_region_outlives(Sema* self, TrStr longer, TrStr shorter) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->current_func_outlives_a->len)) {
        /* pass */
        if (((strcmp(_tr_strz(List_TrStr_get(self->current_func_outlives_a, i)), _tr_strz(longer)) == 0) && (strcmp(_tr_strz(List_TrStr_get(self->current_func_outlives_b, i)), _tr_strz(shorter)) == 0))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < self->current_func_outlives_a->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->current_func_outlives_a, j)), _tr_strz(longer)) == 0)) {
            /* pass */
            TrStr mid = List_TrStr_get(self->current_func_outlives_b, j);
            /* pass */
            long long kk = 0LL;
            /* pass */
            while ((kk < self->current_func_outlives_a->len)) {
                /* pass */
                if (((strcmp(_tr_strz(List_TrStr_get(self->current_func_outlives_a, kk)), _tr_strz(mid)) == 0) && (strcmp(_tr_strz(List_TrStr_get(self->current_func_outlives_b, kk)), _tr_strz(shorter)) == 0))) {
                    /* pass */
                    _tr_str_release(mid);
                    return true;
                }
                /* pass */
                kk = (kk + 1LL);
            }
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_field_is_borrow(Sema* self, Expr* obj, TrStr field) {
    /* pass */
    if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr cls_name = _tr_str_lit("");
    /* pass */
    __auto_type _t400 = (*obj);
    if (_t400.tag == Expr_EIdent) {
        __auto_type fb_obj = _t400.data.EIdent.name;
        /* pass */
        if ((strcmp(_tr_strz(fb_obj), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            TrStr _strtmp_t401 = _tr_str_retain(self->current_class_name);
            _tr_str_release(cls_name);
            cls_name = _strtmp_t401;
        } else {
            /* pass */
            Symbol* fb_sym = Sema_resolve(self, fb_obj);
            /* pass */
            if (((strcmp(_tr_strz(fb_sym->name), _tr_strz(_tr_str_lit(""))) != 0) && (((unsigned long long)(fb_sym->ty)) != ((unsigned long long)(0LL))))) {
                /* pass */
                TrStr _strtmp_t402 = _tr_str_retain((*fb_sym->ty)->name);
                _tr_str_release(cls_name);
                cls_name = _strtmp_t402;
            }
        }
    } else if (1) {
        __auto_type _ = _t400;
        /* pass */
    }
    /* pass */
    if ((strcmp(_tr_strz(cls_name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        _tr_str_release(cls_name);
        return false;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        _tr_str_release(cls_name);
        return false;
    }
    /* pass */
    ClassDef* fb_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    long long fb_i = 0LL;
    /* pass */
    while ((fb_i < fb_cls->fields->len)) {
        /* pass */
        FieldDef* fb_fld = ((FieldDef*)List_ptr_get(fb_cls->fields, fb_i));
        /* pass */
        if ((strcmp(_tr_strz(fb_fld->name), _tr_strz(field)) == 0)) {
            /* pass */
            if ((((unsigned long long)(fb_fld->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _tr_str_release(cls_name);
                return (*fb_fld->ty)->is_borrow;
            }
        }
        /* pass */
        fb_i = (fb_i + 1LL);
    }
    /* pass */
    _tr_str_release(cls_name);
    return false;
}

__attribute__((hot)) void Sema_mark_str_arg(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t403 = (*e);
    if (_t403.tag == HirExpr_EIdent) {
        __auto_type nm = _t403.data.EIdent.name;
        /* pass */
        if ((strcmp(_tr_strz(hir_expr_type(e)->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            Sema_mark_str_escaped(self, nm);
        }
    } else if (1) {
        __auto_type _ = _t403;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_str_args(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t404 = (*e);
    if (_t404.tag == HirExpr_EBinOp) {
        __auto_type l = _t404.data.EBinOp.left;
__auto_type r = _t404.data.EBinOp.right;
        /* pass */
        Sema_mark_escaped_str_args(self, l);
        /* pass */
        Sema_mark_escaped_str_args(self, r);
    } else if (_t404.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t404.data.EUnaryOp.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t404.tag == HirExpr_ECall) {
        __auto_type callee = _t404.data.ECall.callee;
__auto_type args = _t404.data.ECall.args;
        /* pass */
        Sema_mark_escaped_str_args(self, callee);
        /* pass */
        bool _call_is_c_free = false;
        /* pass */
        __auto_type _t405 = (*callee);
        if (_t405.tag == HirExpr_EIdent) {
            __auto_type _cnm = _t405.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(_cnm), _tr_strz(_tr_str_lit("_tr_c_free"))) == 0)) {
                /* pass */
                _call_is_c_free = true;
            }
        } else if (1) {
            __auto_type _ = _t405;
            /* pass */
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            if (_call_is_c_free) {
                /* pass */
                __auto_type _t406 = (*((HirExpr*)List_ptr_get(args, i)));
                if (_t406.tag == HirExpr_ECast) {
                    __auto_type _cf_inner = _t406.data.ECast.expr;
                    /* pass */
                    Sema_mark_str_arg(self, _cf_inner);
                } else if (1) {
                    __auto_type _ = _t406;
                    /* pass */
                }
            }
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t404.data.EMethodCall.obj;
__auto_type mname = _t404.data.EMethodCall.method;
__auto_type args = _t404.data.EMethodCall.args;
        /* pass */
        Sema_mark_escaped_str_args(self, obj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t404.data.EPropAccess.obj;
        Sema_mark_escaped_str_args(self, obj);
    } else if (_t404.tag == HirExpr_EIndex) {
        __auto_type obj = _t404.data.EIndex.obj;
__auto_type idx = _t404.data.EIndex._tr_v_index;
        /* pass */
        Sema_mark_escaped_str_args(self, obj);
        /* pass */
        Sema_mark_escaped_str_args(self, idx);
    } else if (_t404.tag == HirExpr_ECast) {
        __auto_type inner = _t404.data.ECast.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t404.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t404.data.ETryExpr.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t404.tag == HirExpr_EIfElse) {
        __auto_type c = _t404.data.EIfElse.cond;
__auto_type t = _t404.data.EIfElse.then_e;
__auto_type f = _t404.data.EIfElse.else_e;
        /* pass */
        Sema_mark_escaped_str_args(self, c);
        /* pass */
        Sema_mark_escaped_str_args(self, t);
        /* pass */
        Sema_mark_escaped_str_args(self, f);
    } else if (_t404.tag == HirExpr_EList) {
        __auto_type items = _t404.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ESet) {
        __auto_type items = _t404.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ETuple) {
        __auto_type items = _t404.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_EDict) {
        __auto_type keys = _t404.data.EDict.keys;
__auto_type vals = _t404.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ERange) {
        __auto_type s = _t404.data.ERange.start;
__auto_type en = _t404.data.ERange.end;
        /* pass */
        Sema_mark_escaped_str_args(self, s);
        /* pass */
        Sema_mark_escaped_str_args(self, en);
    } else if (_t404.tag == HirExpr_EAwait) {
        __auto_type inner = _t404.data.EAwait.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t404.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t404.data.EAwaitTimeout.expr;
__auto_type to = _t404.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_mark_escaped_str_args(self, inner);
        /* pass */
        Sema_mark_escaped_str_args(self, to);
    } else if (_t404.tag == HirExpr_EYield) {
        __auto_type inner = _t404.data.EYield.expr;
        Sema_mark_escaped_str_args(self, inner);
    } else if (_t404.tag == HirExpr_EFString) {
        __auto_type parts = _t404.data.EFString.parts;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                Sema_mark_escaped_str_args(self, part->expr);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_EClosure) {
        __auto_type captures = _t404.data.EClosure.captures;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < captures->len)) {
            /* pass */
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
            /* pass */
            if ((strcmp(_tr_strz(cap->ty->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                Sema_mark_str_escaped(self, cap->name);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t404.data.ESuperMethodCall.args;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_str_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t404.tag == HirExpr_EListComp) {
        __auto_type element = _t404.data.EListComp.element;
__auto_type generators = _t404.data.EListComp.generators;
        /* pass */
        Sema_mark_str_arg(self, element);
        /* pass */
        Sema_mark_escaped_str_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t404.data.EGeneratorExpr.element;
__auto_type generators = _t404.data.EGeneratorExpr.generators;
        /* pass */
        Sema_mark_str_arg(self, element);
        /* pass */
        Sema_mark_escaped_str_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_str_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_str_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t404.tag == HirExpr_ESlice) {
        __auto_type start = _t404.data.ESlice.start;
__auto_type stop = _t404.data.ESlice.stop;
__auto_type step = _t404.data.ESlice.step;
        /* pass */
        Sema_mark_escaped_str_args(self, start);
        /* pass */
        Sema_mark_escaped_str_args(self, stop);
        /* pass */
        Sema_mark_escaped_str_args(self, step);
    } else if (_t404.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t404.tag == HirExpr_EDo) {
        __auto_type _do_b = _t404.data.EDo.body;
        /* pass */
    } else if (_t404.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t404.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t404.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t404.tag == HirExpr_ELitNone) {
        __auto_type _ = _t404.data.ELitNone.ty;
        /* pass */
    } else if (_t404.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t404.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_coll_escaped(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name)));
            /* pass */
            sym->coll_escaped = true;
            /* pass */
            _tr_dict_set(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name), sym);
            /* pass */
            return;
        }
        /* pass */
        i = (i - 1LL);
    }
}

__attribute__((hot)) void Sema_mark_coll_arg(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t407 = (*e);
    if (_t407.tag == HirExpr_EIdent) {
        __auto_type nm = _t407.data.EIdent.name;
        /* pass */
        TrStr tn = _tr_str_retain(hir_expr_type(e)->name);
        /* pass */
        if ((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
            /* pass */
            Sema_mark_coll_escaped(self, nm);
        } else if (_tr_dict_contains(self->classes, _tr_strz(tn))) {
            /* pass */
            Sema_mark_coll_escaped(self, nm);
        }
        _tr_str_release(tn);
    } else if (1) {
        __auto_type _ = _t407;
        /* pass */
    }
}

__attribute__((hot)) void Sema_mark_escaped_coll_args(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t408 = (*e);
    if (_t408.tag == HirExpr_EBinOp) {
        __auto_type l = _t408.data.EBinOp.left;
__auto_type r = _t408.data.EBinOp.right;
        /* pass */
        Sema_mark_coll_arg(self, l);
        /* pass */
        Sema_mark_escaped_coll_args(self, l);
        /* pass */
        Sema_mark_coll_arg(self, r);
        /* pass */
        Sema_mark_escaped_coll_args(self, r);
    } else if (_t408.tag == HirExpr_EUnaryOp) {
        __auto_type inner = _t408.data.EUnaryOp.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t408.tag == HirExpr_ECall) {
        __auto_type callee = _t408.data.ECall.callee;
__auto_type args = _t408.data.ECall.args;
        /* pass */
        Sema_mark_escaped_coll_args(self, callee);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t408.data.EMethodCall.obj;
__auto_type method = _t408.data.EMethodCall.method;
__auto_type args = _t408.data.EMethodCall.args;
        /* pass */
        if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_or"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("values"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("items"))) == 0))) {
            /* pass */
            __auto_type _t409 = (*obj);
            if (_t409.tag == HirExpr_EIdent) {
                /* pass */
                AstType* obj_ty = hir_expr_type(obj);
                /* pass */
                if (((strcmp(_tr_strz(obj_ty->name), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(obj_ty->name), _tr_strz(_tr_str_lit("Map"))) == 0))) {
                    /* pass */
                    if ((obj_ty->args->len > 1LL)) {
                        /* pass */
                        TrStr obj_vt = _tr_str_retain((*((AstType**)List_ptr_get(obj_ty->args, 1LL)))->name);
                        /* pass */
                        if (((strcmp(_tr_strz(obj_vt), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(obj_vt), _tr_strz(_tr_str_lit("String"))) == 0))) {
                            /* pass */
                            Sema_mark_coll_arg(self, obj);
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t409;
                /* pass */
            }
        }
        /* pass */
        bool _coll_static_call = false;
        /* pass */
        __auto_type _t410 = (*obj);
        if (_t410.tag == HirExpr_EIdent) {
            __auto_type _ocnm2 = _t410.data.EIdent.name;
            /* pass */
            if ((_tr_dict_contains(self->classes, _tr_strz(_ocnm2)) && (!Sema_is_local_var(self, _ocnm2)))) {
                /* pass */
                _coll_static_call = true;
            }
        } else if (1) {
            __auto_type _ = _t410;
            /* pass */
        }
        /* pass */
        Sema_mark_escaped_coll_args(self, obj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            if ((!_coll_static_call)) {
                /* pass */
                Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            }
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_EPropAccess) {
        __auto_type obj = _t408.data.EPropAccess.obj;
        Sema_mark_escaped_coll_args(self, obj);
    } else if (_t408.tag == HirExpr_EIndex) {
        __auto_type obj = _t408.data.EIndex.obj;
__auto_type idx = _t408.data.EIndex._tr_v_index;
        /* pass */
        Sema_mark_escaped_coll_args(self, obj);
        /* pass */
        Sema_mark_coll_arg(self, idx);
        /* pass */
        Sema_mark_escaped_coll_args(self, idx);
    } else if (_t408.tag == HirExpr_ECast) {
        __auto_type inner = _t408.data.ECast.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t408.tag == HirExpr_ETryExpr) {
        __auto_type inner = _t408.data.ETryExpr.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t408.tag == HirExpr_EIfElse) {
        __auto_type c = _t408.data.EIfElse.cond;
__auto_type t = _t408.data.EIfElse.then_e;
__auto_type f = _t408.data.EIfElse.else_e;
        /* pass */
        Sema_mark_escaped_coll_args(self, c);
        /* pass */
        Sema_mark_coll_arg(self, t);
        /* pass */
        Sema_mark_escaped_coll_args(self, t);
        /* pass */
        Sema_mark_coll_arg(self, f);
        /* pass */
        Sema_mark_escaped_coll_args(self, f);
    } else if (_t408.tag == HirExpr_EList) {
        __auto_type items = _t408.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_ESet) {
        __auto_type items = _t408.data.ESet.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_ETuple) {
        __auto_type items = _t408.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_EDict) {
        __auto_type keys = _t408.data.EDict.keys;
__auto_type vals = _t408.data.EDict.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(keys, i)));
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(vals, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_ERange) {
        __auto_type s = _t408.data.ERange.start;
__auto_type en = _t408.data.ERange.end;
        /* pass */
        Sema_mark_escaped_coll_args(self, s);
        /* pass */
        Sema_mark_escaped_coll_args(self, en);
    } else if (_t408.tag == HirExpr_EAwait) {
        __auto_type inner = _t408.data.EAwait.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t408.tag == HirExpr_EAwaitTimeout) {
        __auto_type inner = _t408.data.EAwaitTimeout.expr;
__auto_type to = _t408.data.EAwaitTimeout.timeout_ms;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, to);
    } else if (_t408.tag == HirExpr_EYield) {
        __auto_type inner = _t408.data.EYield.expr;
        /* pass */
        Sema_mark_coll_arg(self, inner);
        /* pass */
        Sema_mark_escaped_coll_args(self, inner);
    } else if (_t408.tag == HirExpr_EFString) {
        __auto_type parts = _t408.data.EFString.parts;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            HirFStringPart* part = ((HirFStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                Sema_mark_escaped_coll_args(self, part->expr);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_EClosure) {
        __auto_type captures = _t408.data.EClosure.captures;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < captures->len)) {
            /* pass */
            HirParam* cap = ((HirParam*)List_ptr_get(captures, i));
            /* pass */
            TrStr ctn = _tr_str_retain(cap->ty->name);
            /* pass */
            if ((((((strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
                /* pass */
                Sema_mark_coll_escaped(self, cap->name);
            }
            /* pass */
            i = (i + 1LL);
            _tr_str_release(ctn);
        }
    } else if (_t408.tag == HirExpr_ESuperMethodCall) {
        __auto_type args = _t408.data.ESuperMethodCall.args;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            Sema_mark_coll_arg(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_ESuperPropAccess) {
        /* pass */
    } else if (_t408.tag == HirExpr_EListComp) {
        __auto_type element = _t408.data.EListComp.element;
__auto_type generators = _t408.data.EListComp.generators;
        /* pass */
        Sema_mark_coll_arg(self, element);
        /* pass */
        Sema_mark_escaped_coll_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_EGeneratorExpr) {
        __auto_type element = _t408.data.EGeneratorExpr.element;
__auto_type generators = _t408.data.EGeneratorExpr.generators;
        /* pass */
        Sema_mark_coll_arg(self, element);
        /* pass */
        Sema_mark_escaped_coll_args(self, element);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < generators->len)) {
            /* pass */
            HirComprehension* gen = (*((HirComprehension**)List_ptr_get(generators, i)));
            /* pass */
            Sema_mark_escaped_coll_args(self, gen->iter);
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < gen->ifs->len)) {
                /* pass */
                Sema_mark_escaped_coll_args(self, ((HirExpr*)List_ptr_get(gen->ifs, j)));
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t408.tag == HirExpr_ESlice) {
        __auto_type start = _t408.data.ESlice.start;
__auto_type stop = _t408.data.ESlice.stop;
__auto_type step = _t408.data.ESlice.step;
        /* pass */
        Sema_mark_coll_arg(self, start);
        /* pass */
        Sema_mark_escaped_coll_args(self, start);
        /* pass */
        Sema_mark_coll_arg(self, stop);
        /* pass */
        Sema_mark_escaped_coll_args(self, stop);
        /* pass */
        Sema_mark_coll_arg(self, step);
        /* pass */
        Sema_mark_escaped_coll_args(self, step);
    } else if (_t408.tag == HirExpr_ETry) {
        /* pass */
    } else if (_t408.tag == HirExpr_EDo) {
        __auto_type _do_b = _t408.data.EDo.body;
        /* pass */
    } else if (_t408.tag == HirExpr_EMatchExpr) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELoop) {
        /* pass */
    } else if (_t408.tag == HirExpr_EWhileExpr) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitInt) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitFloat) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitStr) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitBytes) {
        /* pass */
    } else if (_t408.tag == HirExpr_ERawStr) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitChar) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitBool) {
        /* pass */
    } else if (_t408.tag == HirExpr_ELitNone) {
        __auto_type _ = _t408.data.ELitNone.ty;
        /* pass */
    } else if (_t408.tag == HirExpr_ESizeOf) {
        /* pass */
    } else if (_t408.tag == HirExpr_EIdent) {
        /* pass */
    }
}

__attribute__((hot)) bool Sema_block_ends_in_jump(Sema* self, HirBlock* b) {
    /* pass */
    if ((b->stmts->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (b->stmts->len - 1LL)));
    /* pass */
    if ((((unsigned long long)(last)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t411 = (*last);
    if (_t411.tag == HirStmt_SReturn) {
        __auto_type _ = _t411.data.SReturn.val;
        return true;
    } else if (_t411.tag == HirStmt_SBreak) {
        __auto_type _ = _t411.data.SBreak.val;
        return true;
    } else if (_t411.tag == HirStmt_SContinue) {
        return true;
    } else if (1) {
        __auto_type _ = _t411;
        return false;
    }
}

__attribute__((hot)) void Sema_append_block_local_drops(Sema* self, HirBlock* hb, long long block_id) {
    /* pass */
    if (Sema_block_ends_in_jump(self, hb)) {
        /* pass */
        return;
    }
    /* pass */
    if ((self->scopes->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    Scope* scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < scope->decl_order->len)) {
        /* pass */
        TrStr nm = List_TrStr_get(scope->decl_order, ki);
        /* pass */
        Symbol* sym = ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(nm)));
        /* pass */
        if (((sym->decl_block_id == block_id) && Sema_is_droppable_sym(self, sym))) {
            /* pass */
            HirBlock_push(hb, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, (*sym->ty)->name)));
        }
        /* pass */
        ki = (ki + 1LL);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void Sema_finalize_scope_drops(Sema* self, HirBlock* hb) {
    /* pass */
    if ((!Sema_block_ends_in_jump(self, hb))) {
        /* pass */
        Sema_append_drops_from(self, hb, (self->scopes->len - 1LL));
    }
}

__attribute__((hot)) bool Sema_block_str_decl(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t412 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t412.tag == HirStmt_SLet) {
            __auto_type sn = _t412.data.SLet.name;
__auto_type sty = _t412.data.SLet.ty;
            /* pass */
            if (((strcmp(_tr_strz(sn), _tr_strz(nm)) == 0) && (strcmp(_tr_strz(sty->name), _tr_strz(_tr_str_lit("str"))) == 0))) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t412;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_block_has_autodrop(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t413 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t413.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t413.data.SAutoDrop.name;
            /* pass */
            if ((strcmp(_tr_strz(dn), _tr_strz(nm)) == 0)) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t413;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr Sema_block_coll_decl(Sema* self, HirBlock* hb, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        __auto_type _t414 = (*((HirStmt*)List_ptr_get(hb->stmts, i)));
        if (_t414.tag == HirStmt_SLet) {
            __auto_type sn = _t414.data.SLet.name;
__auto_type sty = _t414.data.SLet.ty;
            /* pass */
            if ((strcmp(_tr_strz(sn), _tr_strz(nm)) == 0)) {
                /* pass */
                TrStr tn = _tr_str_retain(sty->name);
                /* pass */
                if ((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
                    /* pass */
                    return tn;
                }
            }
        } else if (1) {
            __auto_type _ = _t414;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool Sema_coll_droppable_by_sema(Sema* self, TrStr nm) {
    /* pass */
    Symbol* sym = Sema_resolve(self, nm);
    /* pass */
    if ((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return false;
    }
    /* pass */
    return Sema_is_droppable_sym(self, sym);
}

__attribute__((hot)) void Sema_apply_mir_if_drops(Sema* self, HirFunction* hf) {
    /* pass */
    List_ptr* plan = mir_if_drop_plan(hf);
    /* pass */
    long long psi = 0LL;
    /* pass */
    while ((psi < plan->len)) {
        /* pass */
        DropSite* site = ((DropSite*)List_ptr_get(plan, psi));
        /* pass */
        if ((!Sema_block_ends_in_jump(self, site->hir_block))) {
            /* pass */
            long long ppi = 0LL;
            /* pass */
            while ((ppi < site->places->len)) {
                /* pass */
                TrStr nm = List_TrStr_get(site->places, ppi);
                /* pass */
                if ((!Sema_block_has_autodrop(self, site->hir_block, nm))) {
                    /* pass */
                    if (Sema_block_str_decl(self, site->hir_block, nm)) {
                        /* pass */
                        HirBlock_push(site->hir_block, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, _tr_str_lit("str"))));
                    } else {
                        /* pass */
                        TrStr ctn = Sema_block_coll_decl(self, site->hir_block, nm);
                        /* pass */
                        if (((strcmp(_tr_strz(ctn), _tr_strz(_tr_str_lit(""))) != 0) && Sema_coll_droppable_by_sema(self, nm))) {
                            /* pass */
                            HirBlock_push(site->hir_block, box_hirstmt(HirStmt_ctor_SAutoDrop(nm, ctn)));
                        }
                    }
                }
                /* pass */
                ppi = (ppi + 1LL);
                _tr_str_release(nm);
            }
        }
        /* pass */
        psi = (psi + 1LL);
    }
    List_ptr_free_obj(plan, _trdrop_DropSite);
}

__attribute__((hot)) void Sema_declare(Sema* self, TrStr name, SymbolKind kind, AstType** ty, bool is_mut) {
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("self"))) != 0) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_"))) != 0)) && (_tr_strlen(_tr_strz(name)) > 1LL))) {
        /* pass */
        TrStr cat = Sema_is_reserved_error(self, name);
        /* pass */
        if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            ({ TrStr _at_t415 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[N-1] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" and cannot be used as a name. Choose a different name (e.g. 'my_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("')."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t415); _tr_str_release(_at_t415); });
        } else {
            /* pass */
            bool is_toplevel_decl = ((strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) == 0) && (kind.tag != SymbolKind_make_SVariable().tag));
            /* pass */
            if (is_toplevel_decl) {
                /* pass */
                TrStr kcat = Sema_is_reserved_keyword(self, name);
                /* pass */
                if ((strcmp(_tr_strz(kcat), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ({ TrStr _at_t416 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[N-1] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(kcat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" and is reserved. Choose a different name (e.g. 'my_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("')."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t416); _tr_str_release(_at_t416); });
                }
            }
        }
    }
    /* pass */
    Symbol* sym = Symbol_init(name, kind, ty);
    /* pass */
    sym->scope_depth = self->current_scope_depth;
    /* pass */
    sym->is_mut = is_mut;
    /* pass */
    sym->decl_block_depth = self->block_depth;
    /* pass */
    long long bsbase = 0LL;
    /* pass */
    if ((self->block_stack_base->len > 0LL)) {
        /* pass */
        bsbase = List_i64_get(self->block_stack_base, (self->block_stack_base->len - 1LL));
    }
    /* pass */
    if ((self->block_stack->len > bsbase)) {
        /* pass */
        sym->decl_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
    } else {
        /* pass */
        sym->decl_block_id = 0LL;
    }
    /* pass */
    if ((self->scopes->len > 0LL)) {
        /* pass */
        Scope* decl_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
        /* pass */
        if ((!_tr_dict_contains(decl_scope->variables, _tr_strz(name)))) {
            /* pass */
            List_TrStr_append(decl_scope->decl_order, name);
        }
        /* pass */
        _tr_dict_set(decl_scope->variables, _tr_strz(name), sym);
    } else {
        /* pass */
        _tr_dict_set(self->globals, _tr_strz(name), sym);
    }
}

__attribute__((hot)) Symbol* Sema_resolve(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        Scope* scope = ((Scope*)List_ptr_get(self->scopes, i));
        /* pass */
        if (_tr_dict_contains(scope->variables, _tr_strz(name))) {
            /* pass */
            return ((Symbol*)(uintptr_t)_tr_dict_get(scope->variables, _tr_strz(name)));
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    if (_tr_dict_contains(self->globals, _tr_strz(name))) {
        /* pass */
        return ((Symbol*)(uintptr_t)_tr_dict_get(self->globals, _tr_strz(name)));
    }
    /* pass */
    return Symbol_init(_tr_str_lit(""), SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit("void"))));
}

__attribute__((hot)) bool Sema_is_known_name(Sema* self, TrStr name) {
    /* pass */
    if (Sema__is_type_param_in_scope(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->fn_sigs, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->extern_names, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->interfaces, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->type_aliases, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema_is_type_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("alloc"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("dealloc"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("realloc"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("sizeof"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("repr"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("c_callback"))) == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if (({ TrStr _wt_t417 = (_tr_str_wrap(_tr_str_slice(_tr_strz(name), 0LL, 4LL))); __auto_type _wr = (((_tr_strlen(_tr_strz(name)) >= 4LL) && (strcmp(_wt_t417.data, _tr_strz(_tr_str_lit("_tr_"))) == 0))); _tr_str_release(_wt_t417); _wr; })) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_type_name(Sema* self, TrStr nm) {
    /* pass */
    if (((_tr_dict_contains(self->classes, _tr_strz(nm)) || _tr_dict_contains(self->enums, _tr_strz(nm))) || _tr_dict_contains(self->interfaces, _tr_strz(nm)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("str"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("void"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("i8"))) == 0) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("isize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("u8"))) == 0) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("usize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("f32"))) == 0) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr Sema_type_ref_name(Sema* self, Expr* raw) {
    /* pass */
    if ((((unsigned long long)(raw)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t418 = (*raw);
    if (_t418.tag == Expr_EIdent) {
        __auto_type nm = _t418.data.EIdent.name;
        /* pass */
        if ((!Sema_is_type_name(self, nm))) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        Symbol* s = Sema_resolve(self, nm);
        /* pass */
        if (((strcmp(_tr_strz(s->name), _tr_strz(_tr_str_lit(""))) != 0) && (s->kind.tag == SymbolKind_make_SVariable().tag))) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        return _tr_str_retain(nm);
    } else if (1) {
        __auto_type _ = _t418;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool Sema_is_global_not_local(Sema* self, TrStr name) {
    /* pass */
    long long i = (self->scopes->len - 1LL);
    /* pass */
    while ((i >= 0LL)) {
        /* pass */
        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, i))->variables, _tr_strz(name))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i - 1LL);
    }
    /* pass */
    return _tr_dict_contains(self->globals, _tr_strz(name));
}

__attribute__((hot)) HirProgram* Sema_analyze(Sema* self, Program* prog) {
    /* pass */
    HirProgram* hp = HirProgram_init();
    /* pass */
    long long ppi = 0LL;
    /* pass */
    while ((ppi < prog->decls->len)) {
        /* pass */
        __auto_type _t419 = (*((Decl*)List_ptr_get(prog->decls, ppi)));
        if (_t419.tag == Decl_DClass) {
            __auto_type c = _t419.data.DClass.cls;
            /* pass */
            if ((!_tr_dict_contains(self->classes, _tr_strz(c->name)))) {
                /* pass */
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (_t419.tag == Decl_DActor) {
            __auto_type c = _t419.data.DActor.cls;
            /* pass */
            if ((!_tr_dict_contains(self->classes, _tr_strz(c->name)))) {
                /* pass */
                _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
            }
        } else if (1) {
            __auto_type _ = _t419;
            /* pass */
        }
        /* pass */
        ppi = (ppi + 1LL);
    }
    /* pass */
    long long gpi = 0LL;
    /* pass */
    while ((gpi < prog->decls->len)) {
        /* pass */
        __auto_type _t420 = (*((Decl*)List_ptr_get(prog->decls, gpi)));
        if (_t420.tag == Decl_DTopLevelStmt) {
            __auto_type gs = _t420.data.DTopLevelStmt.stmt;
            /* pass */
            __auto_type _t421 = (*gs);
            if (_t421.tag == Stmt_SLet) {
                __auto_type g_name = _t421.data.SLet.name;
__auto_type g_is_mut = _t421.data.SLet.is_mut;
__auto_type g_ty_ptr = _t421.data.SLet.ty;
__auto_type g_val_ptr = _t421.data.SLet.val;
                /* pass */
                AstType** g_ty = ((AstType**)(0LL));
                /* pass */
                if ((((unsigned long long)(g_ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    g_ty = g_ty_ptr;
                } else if ((((unsigned long long)(g_val_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t422 = (*g_val_ptr);
                    if (_t422.tag == Expr_ELitInt) {
                        __auto_type _ = _t422.data.ELitInt.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit("int")));
                    } else if (_t422.tag == Expr_ELitFloat) {
                        __auto_type _ = _t422.data.ELitFloat.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit("float")));
                    } else if (_t422.tag == Expr_ELitStr) {
                        __auto_type _ = _t422.data.ELitStr.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit("str")));
                    } else if (_t422.tag == Expr_ELitBool) {
                        __auto_type _ = _t422.data.ELitBool.val;
                        g_ty = box_asttype(AstType_init(_tr_str_lit("bool")));
                    } else if (1) {
                        __auto_type _ = _t422;
                        /* pass */
                    }
                }
                /* pass */
                if ((((unsigned long long)(g_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    Sema_declare(self, g_name, SymbolKind_make_SVariable(), g_ty, g_is_mut);
                }
            } else if (1) {
                __auto_type _ = _t421;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t420;
            /* pass */
        }
        /* pass */
        gpi = (gpi + 1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Sema_register_decl(self, ((Decl*)List_ptr_get(prog->decls, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (self->strict_mode) {
        /* pass */
        Sema_check_ownership_cycles(self, prog);
    }
    /* pass */
    if (self->strict_mode) {
        /* pass */
        i = 0LL;
        /* pass */
        while ((i < prog->decls->len)) {
            /* pass */
            __auto_type _t423 = (*((Decl*)List_ptr_get(prog->decls, i)));
            if (_t423.tag == Decl_DClass) {
                __auto_type mc = _t423.data.DClass.cls;
                /* pass */
                long long mmi = 0LL;
                /* pass */
                while ((mmi < mc->methods->len)) {
                    /* pass */
                    FunctionDef* mm = ((FunctionDef*)List_ptr_get(mc->methods, mmi));
                    /* pass */
                    if (_block_mutates_self(mm->body)) {
                        /* pass */
                        ({ TrStr _dkt_t424 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(mc->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(mm->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t424), true); _tr_str_release(_dkt_t424); });
                    }
                    /* pass */
                    mmi = (mmi + 1LL);
                }
            } else if (_t423.tag == Decl_DActor) {
                __auto_type ac = _t423.data.DActor.cls;
                /* pass */
                long long ami = 0LL;
                /* pass */
                while ((ami < ac->methods->len)) {
                    /* pass */
                    FunctionDef* am = ((FunctionDef*)List_ptr_get(ac->methods, ami));
                    /* pass */
                    if (_block_mutates_self(am->body)) {
                        /* pass */
                        ({ TrStr _dkt_t425 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ac->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(am->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t425), true); _tr_str_release(_dkt_t425); });
                    }
                    /* pass */
                    ami = (ami + 1LL);
                }
            } else if (_t423.tag == Decl_DExtend) {
                __auto_type mtarget = _t423.data.DExtend.target;
__auto_type mmethods = _t423.data.DExtend.methods;
                /* pass */
                long long emi = 0LL;
                /* pass */
                while ((emi < mmethods->len)) {
                    /* pass */
                    FunctionDef* em = ((FunctionDef*)List_ptr_get(mmethods, emi));
                    /* pass */
                    if (_block_mutates_self(em->body)) {
                        /* pass */
                        ({ TrStr _dkt_t426 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(mtarget), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(em->name)); _tr_str_release(_cl); _cres; })); _tr_dict_set(self->mutating_methods, _tr_strz(_dkt_t426), true); _tr_str_release(_dkt_t426); });
                    }
                    /* pass */
                    emi = (emi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t423;
                /* pass */
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* d = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        __auto_type _t427 = (*d);
        if (_t427.tag == Decl_DFunction) {
            __auto_type f = _t427.data.DFunction.func;
            /* pass */
            List_ptr_append(hp->functions, Sema_lower_func(self, f));
        } else if (_t427.tag == Decl_DClass) {
            __auto_type c = _t427.data.DClass.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t427.tag == Decl_DActor) {
            __auto_type c = _t427.data.DActor.cls;
            /* pass */
            List_ptr_append(hp->classes, Sema_lower_class(self, c));
        } else if (_t427.tag == Decl_DEnum) {
            __auto_type e = _t427.data.DEnum.enm;
            /* pass */
            List_ptr_append(hp->enums, Sema_lower_enum(self, e));
        } else if (_t427.tag == Decl_DInterface) {
            __auto_type i_def = _t427.data.DInterface.iface;
            /* pass */
            List_ptr_append(hp->interfaces, Sema_lower_interface(self, i_def));
        } else if (_t427.tag == Decl_DExtend) {
            __auto_type target = _t427.data.DExtend.target;
__auto_type methods = _t427.data.DExtend.methods;
            /* pass */
            self->current_class_name = _tr_str_retain(target);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
                /* pass */
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->region_params;
            }
            /* pass */
            long long hi = 0LL;
            /* pass */
            while ((hi < methods->len)) {
                /* pass */
                FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
                /* pass */
                List_ptr_append(hp->functions, Sema_lower_func(self, f));
                /* pass */
                hi = (hi + 1LL);
            }
            /* pass */
            self->current_class_name = _tr_str_lit("");
            /* pass */
            self->current_region_params = (void*)List_TrStr_new();
        } else if (_t427.tag == Decl_DTopLevelStmt) {
            __auto_type s = _t427.data.DTopLevelStmt.stmt;
            /* pass */
            List_ptr_append(hp->top_level_stmts, Sema_lower_stmt(self, s));
        } else if (_t427.tag == Decl_DExtern) {
            __auto_type functions = _t427.data.DExtern.functions;
            /* pass */
            long long ei = 0LL;
            /* pass */
            while ((ei < functions->len)) {
                /* pass */
                FunctionDef* ef = ((FunctionDef*)List_ptr_get(functions, ei));
                /* pass */
                HirFunction* hef = Sema_lower_func(self, ef);
                /* pass */
                hef->is_extern = true;
                /* pass */
                List_ptr_append(hp->extern_funcs, _tr_obj_retain(hef));
                /* pass */
                ei = (ei + 1LL);
                _tr_obj_release(hef, _trdrop_HirFunction);
            }
        } else if (_t427.tag == Decl_DDecoratorDef) {
            __auto_type f = _t427.data.DDecoratorDef.func;
            /* pass */
            HirFunction* hdf = Sema_lower_func(self, f);
            /* pass */
            hdf->is_decorator = true;
            /* pass */
            List_ptr_append(hp->decorator_defs, _tr_obj_retain(hdf));
            _tr_obj_release(hdf, _trdrop_HirFunction);
        } else if (_t427.tag == Decl_DTypeAlias) {
            __auto_type alias_name = _t427.data.DTypeAlias.name;
__auto_type target_ty = _t427.data.DTypeAlias.target;
            /* pass */
            List_TrStr_append(hp->type_alias_names, alias_name);
            /* pass */
            AstType** ta_ty_ptr = box_asttype(AstType_init(_tr_str_lit("void")));
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                ta_ty_ptr = target_ty;
            }
            /* pass */
            List_ptr_append(hp->type_alias_types, ta_ty_ptr);
        } else if (1) {
            __auto_type _ = _t427;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long nci = 0LL;
    /* pass */
    while ((nci < self->nested_classes->len)) {
        /* pass */
        List_ptr_append(hp->classes, _tr_obj_retain(((HirClass*)List_ptr_get(self->nested_classes, nci))));
        /* pass */
        nci = (nci + 1LL);
    }
    /* pass */
    long long nfi = 0LL;
    /* pass */
    while ((nfi < self->nested_functions->len)) {
        /* pass */
        List_ptr_append(hp->functions, _tr_obj_retain(((HirFunction*)List_ptr_get(self->nested_functions, nfi))));
        /* pass */
        nfi = (nfi + 1LL);
    }
    /* pass */
    long long nei = 0LL;
    /* pass */
    while ((nei < self->nested_enums->len)) {
        /* pass */
        List_ptr_append(hp->enums, _tr_obj_retain(((HirEnum*)List_ptr_get(self->nested_enums, nei))));
        /* pass */
        nei = (nei + 1LL);
    }
    /* pass */
    long long nii = 0LL;
    /* pass */
    while ((nii < self->nested_interfaces->len)) {
        /* pass */
        List_ptr_append(hp->interfaces, _tr_obj_retain(((HirInterface*)List_ptr_get(self->nested_interfaces, nii))));
        /* pass */
        nii = (nii + 1LL);
    }
    /* pass */
    Sema_compute_return_ownership(self, hp);
    /* pass */
    Sema_compute_param_ownership(self, hp);
    /* pass */
    Sema_apply_borrow_drops(self, hp);
    /* pass */
    return hp;
}

__attribute__((hot)) void Sema_compute_return_ownership(Sema* self, HirProgram* hp) {
    /* pass */
    List_TrStr* keys = (void*)List_TrStr_new();
    /* pass */
    List_ptr* fns = (void*)List_ptr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(hp->functions, fi));
        /* pass */
        if (Sema__fn_ret_is_heap_class(self, f)) {
            /* pass */
            List_TrStr_append(keys, f->name);
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(f));
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            HirFunction* m = ((HirFunction*)List_ptr_get(c->methods, mi));
            /* pass */
            if (Sema__fn_ret_is_heap_class(self, m)) {
                /* pass */
                ({ TrStr _at_t428 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(c->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t428); _tr_str_release(_at_t428); });
                /* pass */
                List_ptr_append(fns, _tr_obj_retain(m));
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long ei = 0LL;
    /* pass */
    while ((ei < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, ei));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            HirFunction* em = ((HirFunction*)List_ptr_get(en->methods, emi));
            /* pass */
            if (Sema__fn_ret_is_heap_class(self, em)) {
                /* pass */
                ({ TrStr _at_t429 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(en->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(em->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t429); _tr_str_release(_at_t429); });
                /* pass */
                List_ptr_append(fns, _tr_obj_retain(em));
            }
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        ei = (ei + 1LL);
    }
    /* pass */
    long long ki = 0LL;
    /* pass */
    while ((ki < keys->len)) {
        /* pass */
        HirFunction* kf = ((HirFunction*)List_ptr_get(fns, ki));
        /* pass */
        bool kf_owned = true;
        /* pass */
        if ((((unsigned long long)(kf->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (((kf->ret_ty->is_borrow || (strcmp(_tr_strz(kf->ret_ty->from_param), _tr_strz(_tr_str_lit(""))) != 0)) || (kf->ret_ty->from_regions->len > 0LL))) {
                /* pass */
                kf_owned = false;
            }
        }
        /* pass */
        ({ TrStr _dkt_t430 = (List_TrStr_get(keys, ki)); _tr_dict_set(self->fn_ret_owned, _tr_strz(_dkt_t430), kf_owned); _tr_str_release(_dkt_t430); });
        /* pass */
        ki = (ki + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long xi = 0LL;
        /* pass */
        while ((xi < fns->len)) {
            /* pass */
            TrStr k = List_TrStr_get(keys, xi);
            /* pass */
            if (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(k)))) {
                /* pass */
                List_ptr* rets = (void*)List_ptr_new();
                /* pass */
                Sema__collect_returns(self, ((HirFunction*)List_ptr_get(fns, xi))->body, rets);
                /* pass */
                bool all_owned = true;
                /* pass */
                long long ri = 0LL;
                /* pass */
                while ((ri < rets->len)) {
                    /* pass */
                    if ((!Sema__ret_yields_owned(self, ((HirExpr*)List_ptr_get(rets, ri))))) {
                        /* pass */
                        all_owned = false;
                    }
                    /* pass */
                    ri = (ri + 1LL);
                }
                /* pass */
                if ((!all_owned)) {
                    /* pass */
                    _tr_dict_set(self->fn_ret_owned, _tr_strz(k), false);
                    /* pass */
                    changed = true;
                }
            }
            /* pass */
            xi = (xi + 1LL);
            _tr_str_release(k);
        }
    }
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < fns->len)) {
        /* pass */
        HirFunction* hf2 = ((HirFunction*)List_ptr_get(fns, wi));
        /* pass */
        hf2->returns_owned = ({ TrStr _dkt_t431 = (List_TrStr_get(keys, wi)); __auto_type _wr = (((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(_dkt_t431)))); _tr_str_release(_dkt_t431); _wr; });
        /* pass */
        wi = (wi + 1LL);
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) void Sema_compute_param_ownership(Sema* self, HirProgram* hp) {
    /* pass */
    List_TrStr* keys = (void*)List_TrStr_new();
    /* pass */
    List_ptr* fns = (void*)List_ptr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        List_TrStr_append(keys, ((HirFunction*)List_ptr_get(hp->functions, fi))->name);
        /* pass */
        List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(hp->functions, fi))));
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            ({ TrStr _at_t432 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(c->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(((HirFunction*)List_ptr_get(c->methods, mi))->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t432); _tr_str_release(_at_t432); });
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(c->methods, mi))));
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long eni = 0LL;
    /* pass */
    while ((eni < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            ({ TrStr _at_t433 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(en->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(((HirFunction*)List_ptr_get(en->methods, emi))->name)); _tr_str_release(_cl); _cres; })); List_TrStr_append(keys, _at_t433); _tr_str_release(_at_t433); });
            /* pass */
            List_ptr_append(fns, _tr_obj_retain(((HirFunction*)List_ptr_get(en->methods, emi))));
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        eni = (eni + 1LL);
    }
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < fns->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(fns, ii));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < f->params->len)) {
            /* pass */
            if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(f->params, pi))->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                /* pass */
                ({ TrStr _dkt_t434 = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(keys, ii)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("#"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); _tr_dict_set(self->fn_param_consumes, _tr_strz(_dkt_t434), false); _tr_str_release(_dkt_t434); });
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    bool changed = true;
    /* pass */
    while (changed) {
        /* pass */
        changed = false;
        /* pass */
        long long xi = 0LL;
        /* pass */
        while ((xi < fns->len)) {
            /* pass */
            HirFunction* f = ((HirFunction*)List_ptr_get(fns, xi));
            /* pass */
            long long pi = 0LL;
            /* pass */
            while ((pi < f->params->len)) {
                /* pass */
                TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pi))->name);
                /* pass */
                TrStr pk = ({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(keys, xi)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("#"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                if (((strcmp(_tr_strz(pn), _tr_strz(_tr_str_lit("self"))) != 0) && _tr_dict_contains(self->fn_param_consumes, _tr_strz(pk)))) {
                    /* pass */
                    if ((!((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(pk))))) {
                        /* pass */
                        if (Sema__param_consumed_in_block(self, pn, f->body)) {
                            /* pass */
                            _tr_dict_set(self->fn_param_consumes, _tr_strz(pk), true);
                            /* pass */
                            changed = true;
                        }
                    }
                }
                /* pass */
                pi = (pi + 1LL);
                _tr_str_release(pn);
                _tr_str_release(pk);
            }
            /* pass */
            xi = (xi + 1LL);
        }
    }
    List_TrStr_free(keys);
    List_ptr_free_obj(fns, _trdrop_HirFunction);
}

__attribute__((hot)) bool Sema__is_pure_scalar(Sema* self, TrStr tn) {
    /* pass */
    if ((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("void"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("i8"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("i64"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("u8"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("usize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("f32"))) == 0) || (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__pc_owns_return(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(e)))) {
        /* pass */
        return true;
    }
    /* pass */
    return (!Sema__is_pure_scalar(self, hir_expr_type(e)->name));
}

__attribute__((hot)) void Sema_apply_borrow_drops(Sema* self, HirProgram* hp) {
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < hp->functions->len)) {
        /* pass */
        Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(hp->functions, fi))->body);
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < hp->classes->len)) {
        /* pass */
        HirClass* c = ((HirClass*)List_ptr_get(hp->classes, ci));
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < c->methods->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(c->methods, mi))->body);
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    long long eni = 0LL;
    /* pass */
    while ((eni < hp->enums->len)) {
        /* pass */
        HirEnum* en = ((HirEnum*)List_ptr_get(hp->enums, eni));
        /* pass */
        long long emi = 0LL;
        /* pass */
        while ((emi < en->methods->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirFunction*)List_ptr_get(en->methods, emi))->body);
            /* pass */
            emi = (emi + 1LL);
        }
        /* pass */
        eni = (eni + 1LL);
    }
}

__attribute__((hot)) void Sema__borrow_drops_block(Sema* self, HirBlock* b) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema__borrow_drops_recurse_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr* add_names = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* add_cls = (void*)List_TrStr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        __auto_type _t435 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t435.tag == HirStmt_SLet) {
            __auto_type nm = _t435.data.SLet.name;
__auto_type _own = _t435.data.SLet.ownership;
__auto_type _mut2 = _t435.data.SLet.is_mut;
__auto_type _cst = _t435.data.SLet.is_const;
__auto_type _shr = _t435.data.SLet.is_shared;
__auto_type _ty = _t435.data.SLet.ty;
__auto_type val = _t435.data.SLet.val;
            /* pass */
            TrStr cn = _tr_str_retain(_ty->name);
            /* pass */
            if ((Sema__is_owned_droppable_class(self, cn) && Sema__is_fresh_owned_producer(self, val, cn))) {
                /* pass */
                if ((!Sema__block_has_drop_or_reassign(self, b, nm))) {
                    /* pass */
                    if ((!Sema__param_consumed_in_block(self, nm, b))) {
                        /* pass */
                        List_TrStr_append(add_names, nm);
                        /* pass */
                        List_TrStr_append(add_cls, cn);
                    }
                }
            }
            _tr_str_release(cn);
        } else if (1) {
            __auto_type _ = _t435;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < add_names->len)) {
        /* pass */
        ({ TrStr _at_t436 = (List_TrStr_get(add_names, ai)); TrStr _at_t437 = (List_TrStr_get(add_cls, ai)); Sema__insert_drop_before_terminator(self, b, _at_t436, _at_t437); _tr_str_release(_at_t436); _tr_str_release(_at_t437); });
        /* pass */
        ai = (ai + 1LL);
    }
    List_TrStr_free(add_names);
    List_TrStr_free(add_cls);
}

__attribute__((hot)) void Sema__borrow_drops_recurse_stmt(Sema* self, HirStmt* sp) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t438 = (*sp);
    if (_t438.tag == HirStmt_SIf) {
        __auto_type tb = _t438.data.SIf.then_b;
__auto_type eb = _t438.data.SIf.else_b;
        /* pass */
        Sema__borrow_drops_block(self, tb);
        /* pass */
        Sema__borrow_drops_block(self, eb);
    } else if (_t438.tag == HirStmt_SWhile) {
        __auto_type wb = _t438.data.SWhile.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t438.tag == HirStmt_SFor) {
        __auto_type fb = _t438.data.SFor.body;
        Sema__borrow_drops_block(self, fb);
    } else if (_t438.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t438.data.SForUnpack.body;
        Sema__borrow_drops_block(self, fub);
    } else if (_t438.tag == HirStmt_SMatch) {
        __auto_type arms = _t438.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            Sema__borrow_drops_block(self, ((HirMatchArm*)List_ptr_get(arms, ai))->body);
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t438.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t438.data.SUnsafe.body;
        Sema__borrow_drops_block(self, ub);
    } else if (_t438.tag == HirStmt_SWith) {
        __auto_type wb = _t438.data.SWith.body;
        Sema__borrow_drops_block(self, wb);
    } else if (_t438.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t438.data.STaskGroup.body;
        Sema__borrow_drops_block(self, tb);
    } else if (_t438.tag == HirStmt_STry) {
        __auto_type tb = _t438.data.STry.try_body;
__auto_type catches = _t438.data.STry.catches;
__auto_type fb = _t438.data.STry.finally_b;
        /* pass */
        Sema__borrow_drops_block(self, tb);
        /* pass */
        Sema__borrow_drops_block(self, fb);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            Sema__borrow_drops_block(self, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body);
            /* pass */
            ci = (ci + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t438;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__is_fresh_owned_producer(Sema* self, HirExpr* val, TrStr cn) {
    /* pass */
    if ((((unsigned long long)(val)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    if (_is_invalid_ptr(((unsigned long long)(val)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t439 = (*val);
    if (_t439.tag == HirExpr_ECall) {
        __auto_type callee = _t439.data.ECall.callee;
        /* pass */
        __auto_type _t440 = (*callee);
        if (_t440.tag == HirExpr_EIdent) {
            __auto_type fnm = _t440.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(fnm), _tr_strz(cn)) == 0)) {
                /* pass */
                return true;
            }
            /* pass */
            if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(fnm)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(fnm))))) {
                /* pass */
                return true;
            }
        } else if (1) {
            __auto_type _ = _t440;
            /* pass */
        }
    } else if (_t439.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t439.data.EMethodCall.obj;
__auto_type m = _t439.data.EMethodCall.method;
        /* pass */
        if (((strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("new"))) == 0))) {
            /* pass */
            return true;
        }
        /* pass */
        TrStr mkey = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(hir_expr_type(obj)->name), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m)); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((_tr_dict_contains(self->fn_ret_owned, _tr_strz(mkey)) && ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(mkey))))) {
            /* pass */
            _tr_str_release(mkey);
            return true;
        }
        _tr_str_release(mkey);
    } else if (1) {
        __auto_type _ = _t439;
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__is_owned_droppable_class(Sema* self, TrStr cn) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cn)))) {
        /* pass */
        return false;
    }
    /* pass */
    if ((((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cn)))->generics->len > 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    if (Sema_class_method_exists(self, cn, _tr_str_lit("free"))) {
        /* pass */
        return true;
    }
    /* pass */
    return Sema__is_rc_class(self, cn);
}

__attribute__((hot)) bool Sema__block_has_drop_or_reassign(Sema* self, HirBlock* b, TrStr nm) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        __auto_type _t441 = (*((HirStmt*)List_ptr_get(b->stmts, i)));
        if (_t441.tag == HirStmt_SAutoDrop) {
            __auto_type dn = _t441.data.SAutoDrop.name;
            /* pass */
            if ((strcmp(_tr_strz(dn), _tr_strz(nm)) == 0)) {
                /* pass */
                return true;
            }
        } else if (_t441.tag == HirStmt_SFree) {
            __auto_type dn = _t441.data.SFree.name;
            /* pass */
            if ((strcmp(_tr_strz(dn), _tr_strz(nm)) == 0)) {
                /* pass */
                return true;
            }
        } else if (_t441.tag == HirStmt_SAssign) {
            __auto_type tgt = _t441.data.SAssign.target;
            /* pass */
            __auto_type _t442 = (*tgt);
            if (_t442.tag == HirExpr_EIdent) {
                __auto_type tn2 = _t442.data.EIdent.name;
                /* pass */
                if ((strcmp(_tr_strz(tn2), _tr_strz(nm)) == 0)) {
                    /* pass */
                    return true;
                }
            } else if (1) {
                __auto_type _ = _t442;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t441;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void Sema__insert_drop_before_terminator(Sema* self, HirBlock* b, TrStr nm, TrStr cn) {
    /* pass */
    HirStmt* drop = box_hirstmt(HirStmt_ctor_SAutoDrop(nm, cn));
    /* pass */
    long long n = b->stmts->len;
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        List_ptr_append(b->stmts, drop);
        /* pass */
        return;
    }
    /* pass */
    HirStmt* last = ((HirStmt*)List_ptr_get(b->stmts, (n - 1LL)));
    /* pass */
    bool is_term = false;
    /* pass */
    __auto_type _t443 = (*last);
    if (_t443.tag == HirStmt_SReturn) {
        __auto_type _ = _t443.data.SReturn.val;
        is_term = true;
    } else if (_t443.tag == HirStmt_SRaise) {
        __auto_type _ = _t443.data.SRaise.val;
        is_term = true;
    } else if (_t443.tag == HirStmt_SBreak) {
        __auto_type _ = _t443.data.SBreak.val;
        is_term = true;
    } else if (_t443.tag == HirStmt_SContinue) {
        is_term = true;
    } else if (1) {
        __auto_type _ = _t443;
        /* pass */
    }
    /* pass */
    if (is_term) {
        /* pass */
        List_ptr_remove(b->stmts, (n - 1LL));
        /* pass */
        List_ptr_append(b->stmts, drop);
        /* pass */
        List_ptr_append(b->stmts, last);
    } else {
        /* pass */
        List_ptr_append(b->stmts, drop);
    }
}

__attribute__((hot)) bool Sema__pc_is_ident(Sema* self, HirExpr* ep, TrStr name) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t444 = (*ep);
    if (_t444.tag == HirExpr_EIdent) {
        __auto_type n = _t444.data.EIdent.name;
        return (strcmp(_tr_strz(n), _tr_strz(name)) == 0);
    } else if (1) {
        __auto_type _ = _t444;
        return false;
    }
}

__attribute__((hot)) TrStr Sema__pc_callee_name(Sema* self, HirExpr* callee) {
    /* pass */
    if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t445 = (*callee);
    if (_t445.tag == HirExpr_EIdent) {
        __auto_type n = _t445.data.EIdent.name;
        return _tr_str_retain(n);
    } else if (1) {
        __auto_type _ = _t445;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool Sema__pc_stores_method(Sema* self, TrStr m) {
    /* pass */
    return (((((strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("push"))) == 0) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("append"))) == 0)) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("insert"))) == 0)) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("set"))) == 0)) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("add"))) == 0));
}

__attribute__((hot)) bool Sema__pc_is_store_target(Sema* self, HirExpr* tgt) {
    /* pass */
    if ((((unsigned long long)(tgt)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t446 = (*tgt);
    if (_t446.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t446.tag == HirExpr_EIndex) {
        return true;
    } else if (1) {
        __auto_type _ = _t446;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_refs(Sema* self, HirExpr* ep, TrStr name) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t447 = (*ep);
    if (_t447.tag == HirExpr_EIdent) {
        __auto_type n = _t447.data.EIdent.name;
        return (strcmp(_tr_strz(n), _tr_strz(name)) == 0);
    } else if (_t447.tag == HirExpr_EBinOp) {
        __auto_type l = _t447.data.EBinOp.left;
__auto_type r = _t447.data.EBinOp.right;
        return (Sema__pc_refs(self, l, name) || Sema__pc_refs(self, r, name));
    } else if (_t447.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t447.data.EUnaryOp.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t447.tag == HirExpr_ECast) {
        __auto_type e = _t447.data.ECast.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t447.tag == HirExpr_EPropAccess) {
        __auto_type o = _t447.data.EPropAccess.obj;
        return Sema__pc_refs(self, o, name);
    } else if (_t447.tag == HirExpr_EIndex) {
        __auto_type o = _t447.data.EIndex.obj;
__auto_type ix = _t447.data.EIndex._tr_v_index;
        return (Sema__pc_refs(self, o, name) || Sema__pc_refs(self, ix, name));
    } else if (_t447.tag == HirExpr_ECall) {
        __auto_type cl = _t447.data.ECall.callee;
__auto_type args = _t447.data.ECall.args;
        return (Sema__pc_refs(self, cl, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t447.tag == HirExpr_EMethodCall) {
        __auto_type o = _t447.data.EMethodCall.obj;
__auto_type args = _t447.data.EMethodCall.args;
        return (Sema__pc_refs(self, o, name) || Sema__pc_anyrefs(self, args, name));
    } else if (_t447.tag == HirExpr_EList) {
        __auto_type items = _t447.data.EList.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t447.tag == HirExpr_ESet) {
        __auto_type items = _t447.data.ESet.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t447.tag == HirExpr_ETuple) {
        __auto_type items = _t447.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, name);
    } else if (_t447.tag == HirExpr_EDict) {
        __auto_type dks = _t447.data.EDict.keys;
__auto_type dvs = _t447.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, name) || Sema__pc_anyrefs(self, dvs, name));
    } else if (_t447.tag == HirExpr_ETryExpr) {
        __auto_type e = _t447.data.ETryExpr.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t447.tag == HirExpr_EAwait) {
        __auto_type e = _t447.data.EAwait.expr;
        return Sema__pc_refs(self, e, name);
    } else if (_t447.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t447.data.EIfElse.cond;
__auto_type te = _t447.data.EIfElse.then_e;
__auto_type ee = _t447.data.EIfElse.else_e;
        return ((Sema__pc_refs(self, cnd, name) || Sema__pc_refs(self, te, name)) || Sema__pc_refs(self, ee, name));
    } else if (_t447.tag == HirExpr_ESlice) {
        __auto_type sa = _t447.data.ESlice.start;
__auto_type sb = _t447.data.ESlice.stop;
__auto_type sc = _t447.data.ESlice.step;
        return ((Sema__pc_refs(self, sa, name) || Sema__pc_refs(self, sb, name)) || Sema__pc_refs(self, sc, name));
    } else if (_t447.tag == HirExpr_ERange) {
        __auto_type ra = _t447.data.ERange.start;
__auto_type rb = _t447.data.ERange.end;
        return (Sema__pc_refs(self, ra, name) || Sema__pc_refs(self, rb, name));
    } else if (1) {
        __auto_type _ = _t447;
        return false;
    }
}

__attribute__((hot)) bool Sema__pc_anyrefs(Sema* self, List_ptr* args, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if (Sema__pc_refs(self, ((HirExpr*)List_ptr_get(args, i)), name)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_block(Sema* self, TrStr pname, HirBlock* b) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if (Sema__param_consumed_in_stmt(self, pname, ((HirStmt*)List_ptr_get(b->stmts, i)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__param_consumed_in_stmt(Sema* self, TrStr pname, HirStmt* sp) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t448 = (*sp);
    if (_t448.tag == HirStmt_SReturn) {
        __auto_type e = _t448.data.SReturn.val;
        /* pass */
        if ((!Sema__pc_owns_return(self, e))) {
            /* pass */
            return false;
        }
        /* pass */
        return Sema__pc_refs(self, e, pname);
    } else if (_t448.tag == HirStmt_SRaise) {
        __auto_type e = _t448.data.SRaise.val;
        /* pass */
        if ((!Sema__pc_owns_return(self, e))) {
            /* pass */
            return false;
        }
        /* pass */
        return Sema__pc_refs(self, e, pname);
    } else if (_t448.tag == HirStmt_SFree) {
        __auto_type nm = _t448.data.SFree.name;
        return (strcmp(_tr_strz(nm), _tr_strz(pname)) == 0);
    } else if (_t448.tag == HirStmt_SSpawn) {
        __auto_type e = _t448.data.SSpawn.expr;
        return Sema__pc_refs(self, e, pname);
    } else if (_t448.tag == HirStmt_SAssign) {
        __auto_type tgt = _t448.data.SAssign.target;
__auto_type val = _t448.data.SAssign.val;
        /* pass */
        if ((Sema__pc_is_store_target(self, tgt) && Sema__pc_refs(self, val, pname))) {
            /* pass */
            return true;
        }
        /* pass */
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t448.tag == HirStmt_SLet) {
        __auto_type val = _t448.data.SLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t448.tag == HirStmt_SMultiLet) {
        __auto_type val = _t448.data.SMultiLet.val;
        return Sema__param_consumed_in_expr(self, pname, val);
    } else if (_t448.tag == HirStmt_SExpr) {
        __auto_type e = _t448.data.SExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t448.tag == HirStmt_SIf) {
        __auto_type tb = _t448.data.SIf.then_b;
__auto_type eb = _t448.data.SIf.else_b;
        return (Sema__param_consumed_in_block(self, pname, tb) || Sema__param_consumed_in_block(self, pname, eb));
    } else if (_t448.tag == HirStmt_SWhile) {
        __auto_type wb = _t448.data.SWhile.body;
        return Sema__param_consumed_in_block(self, pname, wb);
    } else if (_t448.tag == HirStmt_SFor) {
        __auto_type fb = _t448.data.SFor.body;
        return Sema__param_consumed_in_block(self, pname, fb);
    } else if (_t448.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t448.data.SForUnpack.body;
        return Sema__param_consumed_in_block(self, pname, fub);
    } else if (_t448.tag == HirStmt_SMatch) {
        __auto_type arms = _t448.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            if (Sema__param_consumed_in_block(self, pname, ((HirMatchArm*)List_ptr_get(arms, ai))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        return false;
    } else if (_t448.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t448.data.SUnsafe.body;
        return Sema__param_consumed_in_block(self, pname, ub);
    } else if (_t448.tag == HirStmt_SWith) {
        __auto_type wb = _t448.data.SWith.body;
        return Sema__param_consumed_in_block(self, pname, wb);
    } else if (_t448.tag == HirStmt_STaskGroup) {
        __auto_type tb = _t448.data.STaskGroup.body;
        return Sema__param_consumed_in_block(self, pname, tb);
    } else if (_t448.tag == HirStmt_SDefer) {
        __auto_type ds = _t448.data.SDefer.stmt;
        return Sema__param_consumed_in_stmt(self, pname, ds);
    } else if (_t448.tag == HirStmt_STry) {
        __auto_type tb = _t448.data.STry.try_body;
__auto_type catches = _t448.data.STry.catches;
__auto_type fb = _t448.data.STry.finally_b;
        /* pass */
        if (Sema__param_consumed_in_block(self, pname, tb)) {
            /* pass */
            return true;
        }
        /* pass */
        if (Sema__param_consumed_in_block(self, pname, fb)) {
            /* pass */
            return true;
        }
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            if (Sema__param_consumed_in_block(self, pname, (*((HirCatchClause**)List_ptr_get(catches, ci)))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        return false;
    } else if (1) {
        __auto_type _ = _t448;
        return false;
    }
}

__attribute__((hot)) bool Sema__param_consumed_in_expr(Sema* self, TrStr pname, HirExpr* ep) {
    /* pass */
    if ((((unsigned long long)(ep)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t449 = (*ep);
    if (_t449.tag == HirExpr_ECall) {
        __auto_type callee = _t449.data.ECall.callee;
__auto_type args = _t449.data.ECall.args;
        /* pass */
        TrStr gname = Sema__pc_callee_name(self, callee);
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < args->len)) {
            /* pass */
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j)), pname)) {
                /* pass */
                if ((strcmp(_tr_strz(gname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    _tr_str_release(gname);
                    return true;
                }
                /* pass */
                TrStr ck = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(gname), _tr_strz(_tr_str_lit("#")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(j)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                /* pass */
                if ((!_tr_dict_contains(self->fn_param_consumes, _tr_strz(ck)))) {
                    /* pass */
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
                /* pass */
                if (((bool)(uintptr_t)_tr_dict_get(self->fn_param_consumes, _tr_strz(ck)))) {
                    /* pass */
                    _tr_str_release(gname);
                    _tr_str_release(ck);
                    return true;
                }
            } else {
                /* pass */
                if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j)))) {
                    /* pass */
                    _tr_str_release(gname);
                    return true;
                }
            }
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        _tr_str_release(gname);
        return Sema__param_consumed_in_expr(self, pname, callee);
    } else if (_t449.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t449.data.EMethodCall.obj;
__auto_type method = _t449.data.EMethodCall.method;
__auto_type args = _t449.data.EMethodCall.args;
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("free"))) == 0) && Sema__pc_is_ident(self, obj, pname))) {
            /* pass */
            return true;
        }
        /* pass */
        long long j2 = 0LL;
        /* pass */
        while ((j2 < args->len)) {
            /* pass */
            if (Sema__pc_is_ident(self, ((HirExpr*)List_ptr_get(args, j2)), pname)) {
                /* pass */
                return true;
            }
            /* pass */
            if (Sema__param_consumed_in_expr(self, pname, ((HirExpr*)List_ptr_get(args, j2)))) {
                /* pass */
                return true;
            }
            /* pass */
            j2 = (j2 + 1LL);
        }
        /* pass */
        return Sema__param_consumed_in_expr(self, pname, obj);
    } else if (_t449.tag == HirExpr_EList) {
        __auto_type items = _t449.data.EList.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t449.tag == HirExpr_ESet) {
        __auto_type items = _t449.data.ESet.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t449.tag == HirExpr_ETuple) {
        __auto_type items = _t449.data.ETuple.items;
        return Sema__pc_anyrefs(self, items, pname);
    } else if (_t449.tag == HirExpr_EDict) {
        __auto_type dks = _t449.data.EDict.keys;
__auto_type dvs = _t449.data.EDict.vals;
        return (Sema__pc_anyrefs(self, dks, pname) || Sema__pc_anyrefs(self, dvs, pname));
    } else if (_t449.tag == HirExpr_EClosure) {
        __auto_type caps = _t449.data.EClosure.captures;
        /* pass */
        long long cj = 0LL;
        /* pass */
        while ((cj < caps->len)) {
            /* pass */
            if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(caps, cj))->name), _tr_strz(pname)) == 0)) {
                /* pass */
                return true;
            }
            /* pass */
            cj = (cj + 1LL);
        }
        /* pass */
        return false;
    } else if (_t449.tag == HirExpr_EBinOp) {
        __auto_type l = _t449.data.EBinOp.left;
__auto_type r = _t449.data.EBinOp.right;
        return (Sema__param_consumed_in_expr(self, pname, l) || Sema__param_consumed_in_expr(self, pname, r));
    } else if (_t449.tag == HirExpr_EUnaryOp) {
        __auto_type e = _t449.data.EUnaryOp.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t449.tag == HirExpr_ECast) {
        __auto_type e = _t449.data.ECast.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t449.tag == HirExpr_EPropAccess) {
        __auto_type o = _t449.data.EPropAccess.obj;
        return Sema__param_consumed_in_expr(self, pname, o);
    } else if (_t449.tag == HirExpr_EIndex) {
        __auto_type o = _t449.data.EIndex.obj;
__auto_type ix = _t449.data.EIndex._tr_v_index;
        return (Sema__param_consumed_in_expr(self, pname, o) || Sema__param_consumed_in_expr(self, pname, ix));
    } else if (_t449.tag == HirExpr_EIfElse) {
        __auto_type cnd = _t449.data.EIfElse.cond;
__auto_type te = _t449.data.EIfElse.then_e;
__auto_type ee = _t449.data.EIfElse.else_e;
        return ((Sema__param_consumed_in_expr(self, pname, cnd) || Sema__param_consumed_in_expr(self, pname, te)) || Sema__param_consumed_in_expr(self, pname, ee));
    } else if (_t449.tag == HirExpr_EAwait) {
        __auto_type e = _t449.data.EAwait.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (_t449.tag == HirExpr_ETryExpr) {
        __auto_type e = _t449.data.ETryExpr.expr;
        return Sema__param_consumed_in_expr(self, pname, e);
    } else if (1) {
        __auto_type _ = _t449;
        return false;
    }
}

__attribute__((hot)) bool Sema__fn_ret_is_heap_class(Sema* self, HirFunction* f) {
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rn = _tr_str_retain(f->ret_ty->name);
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(rn)))) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    if ((((((((((((((((strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Box"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Result"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(rn), _tr_strz(_tr_str_lit("StringObj"))) == 0))) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    ClassDef* cinfo = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(rn)));
    /* pass */
    if ((!cinfo->is_class)) {
        /* pass */
        _tr_str_release(rn);
        return false;
    }
    /* pass */
    _tr_str_release(rn);
    return true;
}

__attribute__((hot)) void Sema__collect_returns(Sema* self, HirBlock* b, List_ptr* out) {
    /* pass */
    if ((((unsigned long long)(b)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema__collect_returns_stmt(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema__collect_returns_stmt(Sema* self, HirStmt* sp, List_ptr* out) {
    /* pass */
    if ((((unsigned long long)(sp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t450 = (*sp);
    if (_t450.tag == HirStmt_SReturn) {
        __auto_type re = _t450.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(re)) != ((unsigned long long)(0LL)))) {
            /* pass */
            List_ptr_append(out, re);
        }
    } else if (_t450.tag == HirStmt_SIf) {
        __auto_type tb = _t450.data.SIf.then_b;
__auto_type eb = _t450.data.SIf.else_b;
        /* pass */
        Sema__collect_returns(self, tb, out);
        /* pass */
        Sema__collect_returns(self, eb, out);
    } else if (_t450.tag == HirStmt_SWhile) {
        __auto_type wb = _t450.data.SWhile.body;
        Sema__collect_returns(self, wb, out);
    } else if (_t450.tag == HirStmt_SFor) {
        __auto_type fb = _t450.data.SFor.body;
        Sema__collect_returns(self, fb, out);
    } else if (_t450.tag == HirStmt_SForUnpack) {
        __auto_type fub = _t450.data.SForUnpack.body;
        Sema__collect_returns(self, fub, out);
    } else if (_t450.tag == HirStmt_SMatch) {
        __auto_type marms = _t450.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < marms->len)) {
            /* pass */
            Sema__collect_returns(self, ((HirMatchArm*)List_ptr_get(marms, ai))->body, out);
            /* pass */
            ai = (ai + 1LL);
        }
    } else if (_t450.tag == HirStmt_SUnsafe) {
        __auto_type ub = _t450.data.SUnsafe.body;
        Sema__collect_returns(self, ub, out);
    } else if (_t450.tag == HirStmt_SWith) {
        __auto_type wb2 = _t450.data.SWith.body;
        Sema__collect_returns(self, wb2, out);
    } else if (_t450.tag == HirStmt_STaskGroup) {
        __auto_type tgb = _t450.data.STaskGroup.body;
        Sema__collect_returns(self, tgb, out);
    } else if (_t450.tag == HirStmt_SGpuBlock) {
        __auto_type gb = _t450.data.SGpuBlock.body;
        Sema__collect_returns(self, gb, out);
    } else if (_t450.tag == HirStmt_SDefer) {
        __auto_type ds = _t450.data.SDefer.stmt;
        Sema__collect_returns_stmt(self, ds, out);
    } else if (_t450.tag == HirStmt_STry) {
        __auto_type tb2 = _t450.data.STry.try_body;
__auto_type catches = _t450.data.STry.catches;
__auto_type fb2 = _t450.data.STry.finally_b;
        /* pass */
        Sema__collect_returns(self, tb2, out);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < catches->len)) {
            /* pass */
            HirCatchClause** cc = ((HirCatchClause**)List_ptr_get(catches, ci));
            /* pass */
            if ((((unsigned long long)(cc)) != ((unsigned long long)(0LL)))) {
                /* pass */
                Sema__collect_returns(self, (*cc)->body, out);
            }
            /* pass */
            ci = (ci + 1LL);
        }
        /* pass */
        Sema__collect_returns(self, fb2, out);
    } else if (1) {
        __auto_type _ = _t450;
        /* pass */
    }
}

__attribute__((hot)) bool Sema__owned_of(Sema* self, TrStr key) {
    /* pass */
    if (_tr_dict_contains(self->fn_ret_owned, _tr_strz(key))) {
        /* pass */
        return ((bool)(uintptr_t)_tr_dict_get(self->fn_ret_owned, _tr_strz(key)));
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__ret_yields_owned(Sema* self, HirExpr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t451 = (*e);
    if (_t451.tag == HirExpr_EIdent) {
        return true;
    } else if (_t451.tag == HirExpr_EPropAccess) {
        return true;
    } else if (_t451.tag == HirExpr_ECall) {
        __auto_type callee = _t451.data.ECall.callee;
        /* pass */
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t452 = (*callee);
        if (_t452.tag == HirExpr_EIdent) {
            __auto_type cn = _t452.data.EIdent.name;
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(cn))) {
                /* pass */
                return true;
            }
            /* pass */
            return Sema__owned_of(self, cn);
        } else if (1) {
            __auto_type _ = _t452;
            return false;
        }
    } else if (_t451.tag == HirExpr_EMethodCall) {
        __auto_type _tr_v_recv = _t451.data.EMethodCall.obj;
__auto_type m = _t451.data.EMethodCall.method;
        /* pass */
        TrStr rt = _tr_str_retain(hir_expr_type(_tr_v_recv)->name);
        /* pass */
        if (((((((strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("Set"))) == 0)) && (((strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("first"))) == 0)) || (strcmp(_tr_strz(m), _tr_strz(_tr_str_lit("last"))) == 0)))) {
            /* pass */
            _tr_str_release(rt);
            return false;
        }
        /* pass */
        __auto_type _t453 = (*_tr_v_recv);
        if (_t453.tag == HirExpr_EIdent) {
            __auto_type rn = _t453.data.EIdent.name;
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(rn))) {
                /* pass */
                TrStr _strtmp_t454 = _tr_str_retain(rn);
                _tr_str_release(rt);
                rt = _strtmp_t454;
            }
        } else if (1) {
            __auto_type _ = _t453;
            /* pass */
        }
        /* pass */
        return ({ TrStr _at_t455 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(rt), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (Sema__owned_of(self, _at_t455)); _tr_str_release(_at_t455); _wr; });
    } else if (_t451.tag == HirExpr_EIndex) {
        return false;
    } else if (_t451.tag == HirExpr_EIfElse) {
        __auto_type a = _t451.data.EIfElse.then_e;
__auto_type b2 = _t451.data.EIfElse.else_e;
        /* pass */
        return (Sema__ret_yields_owned(self, a) && Sema__ret_yields_owned(self, b2));
    } else if (1) {
        __auto_type _ = _t451;
        return false;
    }
}

__attribute__((hot)) bool Sema__is_type_param_in_scope(Sema* self, TrStr name) {
    /* pass */
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
        /* pass */
        return true;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->current_func_generics->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->current_func_generics, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0) && _tr_dict_contains(self->classes, _tr_strz(self->current_class_name)))) {
        /* pass */
        ClassDef* ccls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < ccls->generics->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(ccls->generics, ci)), _tr_strz(name)) == 0)) {
                /* pass */
                return true;
            }
            /* pass */
            ci = (ci + 1LL);
        }
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema__type_satisfies_bound(Sema* self, TrStr type_name, TrStr iface_name) {
    /* pass */
    if ((!_tr_dict_contains(self->interfaces, _tr_strz(iface_name)))) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema__is_type_param_in_scope(self, type_name)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(type_name)))) {
        /* pass */
        return false;
    }
    /* pass */
    ClassDef* cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(type_name)));
    /* pass */
    long long ii = 0LL;
    /* pass */
    while ((ii < cls->iface_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(cls->iface_names, ii)), _tr_strz(iface_name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        ii = (ii + 1LL);
    }
    /* pass */
    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(iface_name)));
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < idef->methods->len)) {
        /* pass */
        TrStr imeth_nm = _tr_str_retain(((FunctionDef*)List_ptr_get(idef->methods, mi))->name);
        /* pass */
        bool found = false;
        /* pass */
        long long cmi = 0LL;
        /* pass */
        while ((cmi < cls->methods->len)) {
            /* pass */
            if ((strcmp(_tr_strz(((FunctionDef*)List_ptr_get(cls->methods, cmi))->name), _tr_strz(imeth_nm)) == 0)) {
                /* pass */
                found = true;
            }
            /* pass */
            cmi = (cmi + 1LL);
        }
        /* pass */
        if ((!found)) {
            /* pass */
            _tr_str_release(imeth_nm);
            return false;
        }
        /* pass */
        mi = (mi + 1LL);
        _tr_str_release(imeth_nm);
    }
    /* pass */
    return true;
}

__attribute__((hot)) AstType** Sema__resolve_generic_bound_method_ret(Sema* self, TrStr pname, TrStr method) {
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < self->current_func_constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(self->current_func_constraints, ci));
        /* pass */
        if ((strcmp(_tr_strz(gc->target), _tr_strz(pname)) == 0)) {
            /* pass */
            long long bi = 0LL;
            /* pass */
            while ((bi < gc->bounds->len)) {
                /* pass */
                AstType* bound_ty = (*((AstType**)List_ptr_get(gc->bounds, bi)));
                /* pass */
                if (_tr_dict_contains(self->interfaces, _tr_strz(bound_ty->name))) {
                    /* pass */
                    InterfaceDef* idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(bound_ty->name)));
                    /* pass */
                    long long mi = 0LL;
                    /* pass */
                    while ((mi < idef->methods->len)) {
                        /* pass */
                        FunctionDef* imdef = ((FunctionDef*)List_ptr_get(idef->methods, mi));
                        /* pass */
                        if (((strcmp(_tr_strz(imdef->name), _tr_strz(method)) == 0) && (((unsigned long long)(imdef->ret_ty)) != ((unsigned long long)(0LL))))) {
                            /* pass */
                            AstType* found_ty = (*imdef->ret_ty);
                            /* pass */
                            if (((idef->generics->len > 0LL) && (bound_ty->args->len > 0LL))) {
                                /* pass */
                                long long gi = 0LL;
                                /* pass */
                                while ((gi < idef->generics->len)) {
                                    /* pass */
                                    if (((strcmp(_tr_strz(List_TrStr_get(idef->generics, gi)), _tr_strz(found_ty->name)) == 0) && (gi < bound_ty->args->len))) {
                                        /* pass */
                                        found_ty = (*((AstType**)List_ptr_get(bound_ty->args, gi)));
                                    }
                                    /* pass */
                                    gi = (gi + 1LL);
                                }
                            }
                            /* pass */
                            return box_asttype(found_ty);
                        }
                        /* pass */
                        mi = (mi + 1LL);
                    }
                }
                /* pass */
                bi = (bi + 1LL);
            }
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    return ((AstType**)(0LL));
}

__attribute__((hot)) void Sema_check_call_bounds(Sema* self, TrStr fname, List_ptr* hargs) {
    /* pass */
    if ((!_tr_dict_contains(self->fn_defs, _tr_strz(fname)))) {
        /* pass */
        return;
    }
    /* pass */
    FunctionDef* fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(fname)));
    /* pass */
    if ((fd->constraints->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    long long sk = 0LL;
    /* pass */
    if (((fd->params->len > 0LL) && (strcmp(_tr_strz(((Param*)List_ptr_get(fd->params, 0LL))->name), _tr_strz(_tr_str_lit("self"))) == 0))) {
        /* pass */
        sk = 1LL;
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < fd->constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(fd->constraints, ci));
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < fd->params->len)) {
            /* pass */
            AstType** _pty = ((Param*)List_ptr_get(fd->params, pi))->ty;
            /* pass */
            if (((((unsigned long long)(_pty)) != ((unsigned long long)(0LL))) && (strcmp(_tr_strz((*_pty)->name), _tr_strz(gc->target)) == 0))) {
                /* pass */
                long long aidx = (pi - sk);
                /* pass */
                if (((aidx >= 0LL) && (aidx < hargs->len))) {
                    /* pass */
                    TrStr concrete_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(hargs, aidx)))->name);
                    /* pass */
                    long long bi = 0LL;
                    /* pass */
                    while ((bi < gc->bounds->len)) {
                        /* pass */
                        TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
                        /* pass */
                        if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
                            /* pass */
                            ({ TrStr _at_t456 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[G-1] Type '")), _tr_strz(concrete_n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' does not satisfy the bound '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' required by type parameter '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gc->target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' of '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: implement all methods of interface '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' on '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(concrete_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', or add 'implements "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t456); _tr_str_release(_at_t456); });
                        }
                        /* pass */
                        bi = (bi + 1LL);
                        _tr_str_release(iface_n);
                    }
                }
            }
            /* pass */
            pi = (pi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) AstType* Sema__subst_ret_generics(Sema* self, AstType* ty, List_TrStr* generics, List_ptr* concrete) {
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < generics->len)) {
        /* pass */
        if (((strcmp(_tr_strz(List_TrStr_get(generics, gi)), _tr_strz(ty->name)) == 0) && (gi < concrete->len))) {
            /* pass */
            return (*((AstType**)List_ptr_get(concrete, gi)));
        }
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    if ((ty->args->len == 0LL)) {
        /* pass */
        return ty;
    }
    /* pass */
    AstType* nt = AstType_init(ty->name);
    /* pass */
    nt->array_size = ty->array_size;
    /* pass */
    nt->args = (void*)List_ptr_new();
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < ty->args->len)) {
        /* pass */
        List_ptr_append(nt->args, box_asttype(Sema__subst_ret_generics(self, (*((AstType**)List_ptr_get(ty->args, ai))), generics, concrete)));
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    return nt;
}

__attribute__((hot)) void Sema_check_class_bounds(Sema* self, TrStr cls_name, List_ptr* arg_tys) {
    /* pass */
    if ((!_tr_dict_contains(self->classes, _tr_strz(cls_name)))) {
        /* pass */
        return;
    }
    /* pass */
    ClassDef* cd = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(cls_name)));
    /* pass */
    if ((cd->constraints->len == 0LL)) {
        /* pass */
        return;
    }
    /* pass */
    long long ci = 0LL;
    /* pass */
    while ((ci < cd->constraints->len)) {
        /* pass */
        GenericConstraint* gc = ((GenericConstraint*)List_ptr_get(cd->constraints, ci));
        /* pass */
        long long gpi = 0LL;
        /* pass */
        while ((gpi < cd->generics->len)) {
            /* pass */
            if (((strcmp(_tr_strz(List_TrStr_get(cd->generics, gpi)), _tr_strz(gc->target)) == 0) && (gpi < arg_tys->len))) {
                /* pass */
                TrStr concrete_n = _tr_str_retain((*((AstType**)List_ptr_get(arg_tys, gpi)))->name);
                /* pass */
                long long bi = 0LL;
                /* pass */
                while ((bi < gc->bounds->len)) {
                    /* pass */
                    TrStr iface_n = _tr_str_retain((*((AstType**)List_ptr_get(gc->bounds, bi)))->name);
                    /* pass */
                    if ((!Sema__type_satisfies_bound(self, concrete_n, iface_n))) {
                        /* pass */
                        ({ TrStr _at_t457 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[G-1] Type '")), _tr_strz(concrete_n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' does not satisfy the bound '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' required by type parameter '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gc->target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' of class '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cls_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: implement all methods of interface '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' on '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(concrete_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', or add 'implements "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(iface_n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t457); _tr_str_release(_at_t457); });
                    }
                    /* pass */
                    bi = (bi + 1LL);
                    _tr_str_release(iface_n);
                }
            }
            /* pass */
            gpi = (gpi + 1LL);
        }
        /* pass */
        ci = (ci + 1LL);
    }
}

__attribute__((hot)) void Sema_register_decl(Sema* self, Decl* d) {
    /* pass */
    __auto_type _t458 = (*d);
    if (_t458.tag == Decl_DFunction) {
        __auto_type f = _t458.data.DFunction.func;
        /* pass */
        AstType** _f_ret = box_asttype(AstType_init(_tr_str_lit("void")));
        /* pass */
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            _f_ret = f->ret_ty;
        }
        /* pass */
        if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* _frt = (*f->ret_ty);
            /* pass */
            if ((_frt->from_regions->len == 1LL)) {
                /* pass */
                long long _fi = 0LL;
                /* pass */
                while ((_fi < f->params->len)) {
                    /* pass */
                    if ((strcmp(_tr_strz(((Param*)List_ptr_get(f->params, _fi))->name), _tr_strz(List_TrStr_get(_frt->from_regions, 0LL))) == 0)) {
                        /* pass */
                        _frt->from_index = _fi;
                    }
                    /* pass */
                    _fi = (_fi + 1LL);
                }
                /* pass */
                _f_ret = box_asttype(_frt);
            }
        }
        /* pass */
        AstType** _decl_ret = _f_ret;
        /* pass */
        if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if ((strcmp(_tr_strz((*f->throws_ty)->name), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                AstType* _result_ty = AstType_init(_tr_str_lit("Result"));
                /* pass */
                List_ptr_append(_result_ty->args, _f_ret);
                /* pass */
                List_ptr_append(_result_ty->args, f->throws_ty);
                /* pass */
                _decl_ret = box_asttype(_result_ty);
            }
        }
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), _decl_ret, false);
        /* pass */
        AstType* _fnty = AstType_init(_tr_str_lit("def"));
        /* pass */
        long long _pi = 0LL;
        /* pass */
        while ((_pi < f->params->len)) {
            /* pass */
            List_ptr_append(_fnty->args, ((Param*)List_ptr_get(f->params, _pi))->ty);
            /* pass */
            _pi = (_pi + 1LL);
        }
        /* pass */
        List_ptr_append(_fnty->args, _f_ret);
        /* pass */
        _tr_dict_set(self->fn_sigs, _tr_strz(f->name), _fnty);
        /* pass */
        _tr_dict_set(self->fn_defs, _tr_strz(f->name), _tr_obj_retain(f));
        /* pass */
        if ((f->params->len > 0LL)) {
            /* pass */
            Param* _vp = ((Param*)List_ptr_get(f->params, (f->params->len - 1LL)));
            /* pass */
            if (_vp->is_variadic) {
                /* pass */
                AstType* _velem_ty = AstType_init(_tr_str_lit("int"));
                /* pass */
                if ((((unsigned long long)(_vp->ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    _velem_ty = (*_vp->ty);
                }
                /* pass */
                ({ TrStr _dvt_t459 = (_tr_str_wrap(_tr_int_to_str((long long)((f->params->len - 1LL))))); _tr_dict_set(self->variadic_fns, _tr_strz(f->name), _tr_str_box(_tr_str_retain(_dvt_t459))); _tr_str_release(_dvt_t459); });
                /* pass */
                _tr_dict_set(self->variadic_elem_ty, _tr_strz(f->name), box_asttype(_velem_ty));
            }
        }
    } else if (_t458.tag == Decl_DClass) {
        __auto_type c = _t458.data.DClass.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t458.tag == Decl_DActor) {
        __auto_type c = _t458.data.DActor.cls;
        /* pass */
        Sema_declare(self, c->name, SymbolKind_make_SClass(), box_asttype(AstType_init(c->name)), false);
        /* pass */
        _tr_dict_set(self->classes, _tr_strz(c->name), _tr_obj_retain(c));
        /* pass */
        if (Sema_has_copy_decorator(self, c->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(c->name), true);
        }
    } else if (_t458.tag == Decl_DEnum) {
        __auto_type e = _t458.data.DEnum.enm;
        /* pass */
        Sema_declare(self, e->name, SymbolKind_make_SEnum(), box_asttype(AstType_init(e->name)), false);
        /* pass */
        _tr_dict_set(self->enums, _tr_strz(e->name), _tr_obj_retain(e));
        /* pass */
        if (Sema_has_copy_decorator(self, e->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(e->name), true);
        }
    } else if (_t458.tag == Decl_DInterface) {
        __auto_type i = _t458.data.DInterface.iface;
        /* pass */
        Sema_declare(self, i->name, SymbolKind_make_SInterface(), box_asttype(AstType_init(i->name)), false);
        /* pass */
        _tr_dict_set(self->interfaces, _tr_strz(i->name), _tr_obj_retain(i));
        /* pass */
        if (Sema_has_copy_decorator(self, i->decorators)) {
            /* pass */
            _tr_dict_set(self->copy_classes, _tr_strz(i->name), true);
        }
    } else if (_t458.tag == Decl_DExtend) {
        __auto_type target = _t458.data.DExtend.target;
__auto_type methods = _t458.data.DExtend.methods;
        /* pass */
        long long hi = 0LL;
        /* pass */
        while ((hi < methods->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(methods, hi));
            /* pass */
            AstType** _m_ret = box_asttype(AstType_init(_tr_str_lit("void")));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _m_ret = f->ret_ty;
            }
            /* pass */
            TrStr _decl_key = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(target), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; });
            /* pass */
            if (_tr_dict_contains(self->globals, _tr_strz(_decl_key))) {
                /* pass */
                long long _pc = 0LL;
                /* pass */
                long long _pci = 0LL;
                /* pass */
                while ((_pci < f->params->len)) {
                    /* pass */
                    if ((strcmp(_tr_strz(((Param*)List_ptr_get(f->params, _pci))->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                        /* pass */
                        _pc = (_pc + 1LL);
                    }
                    /* pass */
                    _pci = (_pci + 1LL);
                }
                /* pass */
                TrStr _strtmp_t460 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_decl_key), _tr_strz(_tr_str_lit("_")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_pc)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("arg"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(_decl_key);
                _decl_key = _strtmp_t460;
            }
            /* pass */
            Sema_declare(self, _decl_key, SymbolKind_make_SFunction(), _m_ret, false);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(target))) {
                /* pass */
                List_ptr_append(((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(target)))->methods, _tr_obj_retain(f));
            }
            /* pass */
            hi = (hi + 1LL);
            _tr_str_release(_decl_key);
        }
    } else if (_t458.tag == Decl_DExtern) {
        __auto_type abi = _t458.data.DExtern.abi;
__auto_type functions = _t458.data.DExtern.functions;
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < functions->len)) {
            /* pass */
            FunctionDef* f = ((FunctionDef*)List_ptr_get(functions, ei));
            /* pass */
            AstType** _e_ret = box_asttype(AstType_init(_tr_str_lit("void")));
            /* pass */
            if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                _e_ret = f->ret_ty;
            }
            /* pass */
            Sema_declare(self, f->name, SymbolKind_make_SFunction(), _e_ret, false);
            /* pass */
            _tr_dict_set(self->extern_names, _tr_strz(f->name), true);
            /* pass */
            ei = (ei + 1LL);
        }
    } else if (_t458.tag == Decl_DDecoratorDef) {
        __auto_type f = _t458.data.DDecoratorDef.func;
        /* pass */
        _tr_dict_set(self->decorator_names, _tr_strz(f->name), true);
        /* pass */
        Sema_declare(self, f->name, SymbolKind_make_SFunction(), box_asttype(AstType_init(_tr_str_lit("void"))), false);
    } else if (_t458.tag == Decl_DTypeAlias) {
        __auto_type alias_name = _t458.data.DTypeAlias.name;
__auto_type target_ty = _t458.data.DTypeAlias.target;
        /* pass */
        AstType** alias_ty = box_asttype(AstType_init(_tr_str_lit("void")));
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            alias_ty = target_ty;
        }
        /* pass */
        Sema_declare(self, alias_name, SymbolKind_make_SClass(), alias_ty, false);
        /* pass */
        TrStr resolved_name = _tr_str_lit("");
        /* pass */
        if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr _strtmp_t461 = _tr_str_retain((*target_ty)->name);
            _tr_str_release(resolved_name);
            resolved_name = _strtmp_t461;
        }
        /* pass */
        if ((_tr_strlen(_tr_strz(resolved_name)) > 0LL)) {
            /* pass */
            _tr_dict_set(self->type_aliases, _tr_strz(alias_name), _tr_str_box(_tr_str_retain(resolved_name)));
            /* pass */
            if ((((unsigned long long)(target_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                AstType* ta_t = (*target_ty);
                /* pass */
                if ((ta_t->args->len > 0LL)) {
                    /* pass */
                    _tr_dict_set(self->type_alias_elem, _tr_strz(alias_name), _tr_str_box(_tr_str_retain((*((AstType**)List_ptr_get(ta_t->args, 0LL)))->name)));
                }
            }
        }
        _tr_str_release(resolved_name);
    } else if (1) {
        __auto_type _ = _t458;
        /* pass */
    }
}

__attribute__((hot)) HirFunction* Sema_lower_func(Sema* self, FunctionDef* f) {
    /* pass */
    self->current_line = f->line;
    /* pass */
    bool _saved_fn_is_lib = self->cur_fn_is_lib;
    /* pass */
    self->cur_fn_is_lib = f->is_lib;
    /* pass */
    if ((((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) != 0) && (strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit(""))) != 0)) && (_tr_strlen(_tr_strz(f->name)) > 1LL))) {
        /* pass */
        TrStr fn_cat = Sema_is_reserved_error(self, f->name);
        /* pass */
        if ((strcmp(_tr_strz(fn_cat), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            ({ TrStr _at_t462 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[N-1] '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fn_cat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" and cannot be used as a function name."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t462); _tr_str_release(_at_t462); });
        } else if ((strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr fn_kcat = Sema_is_reserved_keyword(self, f->name);
            /* pass */
            if ((strcmp(_tr_strz(fn_kcat), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                ({ TrStr _at_t463 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[N-1] '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fn_kcat)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" and is reserved. Choose a different function name."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t463); _tr_str_release(_at_t463); });
            }
        }
    }
    /* pass */
    bool saved_async = self->in_async_fn;
    /* pass */
    self->in_async_fn = f->is_async;
    /* pass */
    self->current_func_name = _tr_str_retain(f->name);
    /* pass */
    List_TrStr* saved_func_generics = self->current_func_generics;
    /* pass */
    self->current_func_generics = f->generics;
    /* pass */
    List_ptr* saved_func_constraints = self->current_func_constraints;
    /* pass */
    self->current_func_constraints = f->constraints;
    /* pass */
    self->container_borrows = _tr_dict_new(16LL);
    /* pass */
    TrStr saved_ret_from = _tr_str_retain(self->current_func_ret_from);
    /* pass */
    bool saved_ret_borrow_str = self->current_func_ret_borrow_str;
    /* pass */
    List_TrStr* saved_ret_regions = self->current_func_ret_regions;
    /* pass */
    List_TrStr* saved_outlives_a = self->current_func_outlives_a;
    /* pass */
    List_TrStr* saved_outlives_b = self->current_func_outlives_b;
    /* pass */
    self->current_func_ret_from = _tr_str_lit("");
    /* pass */
    self->current_func_ret_borrow_str = false;
    /* pass */
    self->current_func_ret_regions = (void*)List_TrStr_new();
    /* pass */
    self->current_func_outlives_a = f->outlives_a;
    /* pass */
    self->current_func_outlives_b = f->outlives_b;
    /* pass */
    if ((self->strict_mode && (f->outlives_a->len > 0LL))) {
        /* pass */
        long long wbi = 0LL;
        /* pass */
        while ((wbi < f->outlives_a->len)) {
            /* pass */
            bool w_ok_a = false;
            /* pass */
            bool w_ok_b = false;
            /* pass */
            long long wpi = 0LL;
            /* pass */
            while ((wpi < f->params->len)) {
                /* pass */
                if ((strcmp(_tr_strz(((Param*)List_ptr_get(f->params, wpi))->name), _tr_strz(List_TrStr_get(f->outlives_a, wbi))) == 0)) {
                    /* pass */
                    w_ok_a = true;
                }
                /* pass */
                if ((strcmp(_tr_strz(((Param*)List_ptr_get(f->params, wpi))->name), _tr_strz(List_TrStr_get(f->outlives_b, wbi))) == 0)) {
                    /* pass */
                    w_ok_b = true;
                }
                /* pass */
                wpi = (wpi + 1LL);
            }
            /* pass */
            if ((!w_ok_a)) {
                /* pass */
                ({ TrStr _at_t464 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(f->outlives_a, wbi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[L-2] region '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in the `where ... outlives ...` clause is not a parameter of this function."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t464); _tr_str_release(_at_t464); });
            }
            /* pass */
            if ((!w_ok_b)) {
                /* pass */
                ({ TrStr _at_t465 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(f->outlives_b, wbi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[L-2] region '")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in the `where ... outlives ...` clause is not a parameter of this function."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t465); _tr_str_release(_at_t465); });
            }
            /* pass */
            wbi = (wbi + 1LL);
        }
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        self->current_func_ret_from = _tr_str_retain((*f->ret_ty)->from_param);
        /* pass */
        self->current_func_ret_regions = (*f->ret_ty)->from_regions;
        /* pass */
        if (((*f->ret_ty)->is_borrow && (strcmp(_tr_strz((*f->ret_ty)->name), _tr_strz(_tr_str_lit("str"))) == 0))) {
            /* pass */
            self->current_func_ret_borrow_str = true;
        }
    }
    /* pass */
    if (((strcmp(_tr_strz(self->current_func_ret_from), _tr_strz(_tr_str_lit(""))) == 0) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        TrStr infer_ret_nm = _tr_str_retain((*f->ret_ty)->name);
        /* pass */
        bool infer_is_borrow = (*f->ret_ty)->is_borrow;
        /* pass */
        if (((((strcmp(_tr_strz(infer_ret_nm), _tr_strz(_tr_str_lit("Pointer"))) == 0) || (strcmp(_tr_strz(infer_ret_nm), _tr_strz(_tr_str_lit("ref"))) == 0)) || (strcmp(_tr_strz(infer_ret_nm), _tr_strz(_tr_str_lit("mut_ref"))) == 0)) || infer_is_borrow)) {
            /* pass */
            TrStr infer_from = _tr_str_lit("");
            /* pass */
            long long infer_count = 0LL;
            /* pass */
            long long infer_i = 0LL;
            /* pass */
            while ((infer_i < f->params->len)) {
                /* pass */
                Param* infer_p = ((Param*)List_ptr_get(f->params, infer_i));
                /* pass */
                if (((strcmp(_tr_strz(infer_p->name), _tr_strz(_tr_str_lit("self"))) != 0) && (((unsigned long long)(infer_p->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    AstType* infer_pty = (*infer_p->ty);
                    /* pass */
                    if ((!Sema_is_primitive_name(self, infer_pty->name))) {
                        /* pass */
                        TrStr _strtmp_t466 = _tr_str_retain(infer_p->name);
                        _tr_str_release(infer_from);
                        infer_from = _strtmp_t466;
                        /* pass */
                        infer_count = (infer_count + 1LL);
                    }
                }
                /* pass */
                infer_i = (infer_i + 1LL);
            }
            /* pass */
            if ((infer_count == 1LL)) {
                /* pass */
                self->current_func_ret_from = _tr_str_retain(infer_from);
            }
        }
        _tr_str_release(infer_ret_nm);
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        List_TrStr* rregs = (*f->ret_ty)->from_regions;
        /* pass */
        long long rri = 0LL;
        /* pass */
        while ((rri < rregs->len)) {
            /* pass */
            TrStr rnm = List_TrStr_get(rregs, rri);
            /* pass */
            bool rok = false;
            /* pass */
            long long rpi = 0LL;
            /* pass */
            while ((rpi < f->params->len)) {
                /* pass */
                if ((strcmp(_tr_strz(((Param*)List_ptr_get(f->params, rpi))->name), _tr_strz(rnm)) == 0)) {
                    /* pass */
                    rok = true;
                }
                /* pass */
                rpi = (rpi + 1LL);
            }
            /* pass */
            if ((!rok)) {
                /* pass */
                long long rci = 0LL;
                /* pass */
                while ((rci < self->current_region_params->len)) {
                    /* pass */
                    if ((strcmp(_tr_strz(List_TrStr_get(self->current_region_params, rci)), _tr_strz(rnm)) == 0)) {
                        /* pass */
                        rok = true;
                    }
                    /* pass */
                    rci = (rci + 1LL);
                }
            }
            /* pass */
            if ((!rok)) {
                /* pass */
                ({ TrStr _at_t467 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-2] region source '")), _tr_strz(rnm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' named in 'from' is not a parameter or a region parameter of the enclosing type.\n      FIX: name a parameter the borrow comes from, or declare 'class/enum/interface <T> from "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rnm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t467); _tr_str_release(_at_t467); });
            }
            /* pass */
            rri = (rri + 1LL);
            _tr_str_release(rnm);
        }
    }
    /* pass */
    Sema_enter_scope(self);
    /* pass */
    List_ptr* hparams = (void*)List_ptr_new();
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < f->params->len)) {
        /* pass */
        Param* p = ((Param*)List_ptr_get(f->params, j));
        /* pass */
        AstType* p_ty = AstType_init(_tr_str_lit("int"));
        /* pass */
        if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            p_ty = (*p->ty);
        } else if (((strcmp(_tr_strz(p->name), _tr_strz(_tr_str_lit("self"))) == 0) && (strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            p_ty = AstType_init(self->current_class_name);
        }
        /* pass */
        if (p->is_variadic) {
            /* pass */
            AstType* _vp_ty = AstType_init(_tr_str_lit("List"));
            /* pass */
            List_ptr_append(_vp_ty->args, box_asttype(p_ty));
            /* pass */
            p_ty = _vp_ty;
        }
        /* pass */
        Sema_declare(self, p->name, SymbolKind_make_SVariable(), box_asttype(p_ty), false);
        /* pass */
        if ((self->scopes->len > 0LL)) {
            /* pass */
            Scope* pb_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
            /* pass */
            if (_tr_dict_contains(pb_scope->variables, _tr_strz(p->name))) {
                /* pass */
                Symbol* pb_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pb_scope->variables, _tr_strz(p->name)));
                /* pass */
                pb_sym->is_param = true;
                /* pass */
                pb_sym->ptr_region = 2LL;
                /* pass */
                _tr_dict_set(pb_scope->variables, _tr_strz(p->name), pb_sym);
            }
        }
        /* pass */
        HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
        /* pass */
        hp->name = _tr_str_retain(p->name);
        /* pass */
        hp->ty = p_ty;
        /* pass */
        List_ptr_append(hparams, _tr_obj_retain(hp));
        /* pass */
        j = (j + 1LL);
        _tr_obj_release(hp, _trdrop_HirParam);
    }
    /* pass */
    HirFunction* hf = ((HirFunction*)_tr_obj_alloc(sizeof(HirFunction)));
    /* pass */
    hf->name = _tr_str_retain(f->name);
    /* pass */
    hf->class_name = _tr_str_retain(self->current_class_name);
    /* pass */
    hf->generics = f->generics;
    /* pass */
    hf->constraints = f->constraints;
    /* pass */
    hf->params = hparams;
    /* pass */
    hf->ret_ty = AstType_init(_tr_str_lit("None"));
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->ret_ty = (*f->ret_ty);
    }
    /* pass */
    hf->throws_ty = AstType_init(_tr_str_lit(""));
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        hf->throws_ty = (*f->throws_ty);
    }
    /* pass */
    self->cur_func_borrowers = (void*)List_TrStr_new();
    /* pass */
    self->cur_func_sources = (void*)List_TrStr_new();
    /* pass */
    HirBlock* _cltmp_t468 = Sema_lower_block(self, f->body);
    _tr_obj_release(hf->body, _trdrop_HirBlock);
    hf->body = _cltmp_t468;
    /* pass */
    hf->borrow_borrowers = self->cur_func_borrowers;
    /* pass */
    hf->borrow_sources = self->cur_func_sources;
    /* pass */
    hf->proven_borrows = mir_proven_borrows(hf);
    /* pass */
    if (self->strict_mode) {
        /* pass */
        List_TrStr* _bconf = mir_borrow_conflicts(hf, self->mutating_methods);
        /* pass */
        long long _bci = 0LL;
        /* pass */
        while ((_bci < _bconf->len)) {
            /* pass */
            ({ TrStr _at_t469 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(_bconf, _bci)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[B-1] ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".\n      A place may have MANY shared `ref` borrows, OR exactly ONE exclusive `mut` borrow — never both. End one borrow's last use before the other begins."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t469); _tr_str_release(_at_t469); });
            /* pass */
            _bci = (_bci + 1LL);
        }
        /* pass */
        List_TrStr* _b3 = mir_shared_ref_param_violations(hf, self->mutating_methods);
        /* pass */
        long long _b3i = 0LL;
        /* pass */
        while ((_b3i < _b3->len)) {
            /* pass */
            ({ TrStr _at_t470 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(_b3, _b3i)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[B-3] ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".\n      A shared `ref T` parameter cannot be mutated — declare it `mut ref T` for an exclusive (mutable) borrow."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t470); _tr_str_release(_at_t470); });
            /* pass */
            _b3i = (_b3i + 1LL);
        }
        List_TrStr_free(_bconf);
    }
    /* pass */
    hf->decorators = f->decorators;
    /* pass */
    hf->is_async = f->is_async;
    /* pass */
    hf->is_extern = f->is_extern;
    /* pass */
    hf->is_public = f->is_public;
    /* pass */
    hf->is_export = f->is_export;
    /* pass */
    bool _has_self = false;
    /* pass */
    long long _si = 0LL;
    /* pass */
    while ((_si < hparams->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((HirParam*)List_ptr_get(hparams, _si))->name), _tr_strz(_tr_str_lit("self"))) == 0)) {
            /* pass */
            _has_self = true;
        }
        /* pass */
        _si = (_si + 1LL);
    }
    /* pass */
    hf->is_static = ((strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0) && (!_has_self));
    /* pass */
    hf->is_variadic = f->is_variadic;
    /* pass */
    hf->is_decorator = false;
    /* pass */
    bool _is_entry_main = ((strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("main"))) == 0) && (strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) == 0));
    /* pass */
    if ((((!f->is_extern) && (f->body->stmts->len > 0LL)) && (((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL))))) {
        /* pass */
        TrStr ret_nm = _tr_str_retain((*f->ret_ty)->name);
        /* pass */
        if (((((((strcmp(_tr_strz(ret_nm), _tr_strz(_tr_str_lit("void"))) != 0) && (strcmp(_tr_strz(ret_nm), _tr_strz(_tr_str_lit("None"))) != 0)) && (strcmp(_tr_strz(ret_nm), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("init"))) != 0)) && (strcmp(_tr_strz(f->name), _tr_strz(_tr_str_lit("new"))) != 0)) && (!_is_entry_main))) {
            /* pass */
            if ((!Sema_block_returns(self, f->body))) {
                /* pass */
                self->current_line = f->line;
                /* pass */
                ({ TrStr _at_t471 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[F-3] Function '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' returns '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ret_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' but is missing a return statement on at least one code path. FIX: Add a return at the end, or ensure all if/elif/else branches return."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t471); _tr_str_release(_at_t471); });
            }
        }
    }
    /* pass */
    Sema_finalize_scope_drops(self, hf->body);
    /* pass */
    Sema_apply_mir_if_drops(self, hf);
    /* pass */
    Sema_exit_scope(self);
    /* pass */
    self->in_async_fn = saved_async;
    /* pass */
    self->current_func_name = _tr_str_lit("");
    /* pass */
    self->current_func_generics = saved_func_generics;
    /* pass */
    self->current_func_constraints = saved_func_constraints;
    /* pass */
    self->current_func_ret_from = _tr_str_retain(saved_ret_from);
    /* pass */
    self->current_func_ret_borrow_str = saved_ret_borrow_str;
    /* pass */
    self->current_func_ret_regions = saved_ret_regions;
    /* pass */
    self->current_func_outlives_a = saved_outlives_a;
    /* pass */
    self->current_func_outlives_b = saved_outlives_b;
    /* pass */
    self->cur_fn_is_lib = _saved_fn_is_lib;
    /* pass */
    _tr_str_release(saved_ret_from);
    return hf;
}

__attribute__((hot)) HirClass* Sema_lower_class(Sema* self, ClassDef* c) {
    /* pass */
    self->current_line = c->line;
    /* pass */
    self->current_class_name = _tr_str_retain(c->name);
    /* pass */
    self->current_region_params = c->region_params;
    /* pass */
    List_ptr* hfields = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->fields->len)) {
        /* pass */
        FieldDef* f = ((FieldDef*)List_ptr_get(c->fields, i));
        /* pass */
        HirField* hf = ((HirField*)_tr_obj_alloc(sizeof(HirField)));
        /* pass */
        hf->name = _tr_str_retain(f->name);
        /* pass */
        AstType* f_ty = AstType_init(_tr_str_lit("int"));
        /* pass */
        if ((((unsigned long long)(f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            f_ty = (*f->ty);
        }
        /* pass */
        hf->ty = f_ty;
        /* pass */
        List_ptr_append(hfields, _tr_obj_retain(hf));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hf, _trdrop_HirField);
    }
    /* pass */
    long long pf_i = 0LL;
    /* pass */
    while ((pf_i < c->fields->len)) {
        /* pass */
        FieldDef* pf_f = ((FieldDef*)List_ptr_get(c->fields, pf_i));
        /* pass */
        if ((((unsigned long long)(pf_f->ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            AstType* pf_ty = (*pf_f->ty);
            /* pass */
            if (((strcmp(_tr_strz(pf_ty->name), _tr_strz(_tr_str_lit("Shared"))) == 0) && (pf_ty->args->len > 0LL))) {
                /* pass */
                AstType* pf_inner = (*((AstType**)List_ptr_get(pf_ty->args, 0LL)));
                /* pass */
                if ((strcmp(_tr_strz(pf_inner->name), _tr_strz(c->name)) == 0)) {
                    /* pass */
                    ({ TrStr _at_t472 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[S-1] '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' has a 'Shared["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' field '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pf_f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' - this creates a reference cycle that leaks memory.\n      FIX: Use 'Weak["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' for back-references to break the cycle."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t472); _tr_str_release(_at_t472); });
                }
            }
        }
        /* pass */
        pf_i = (pf_i + 1LL);
    }
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(c->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    long long _ifc_i = 0LL;
    /* pass */
    while ((_ifc_i < c->iface_names->len)) {
        /* pass */
        TrStr _ifc_nm = List_TrStr_get(c->iface_names, _ifc_i);
        /* pass */
        if ((strcmp(_tr_strz(_ifc_nm), _tr_strz(_tr_str_lit("Sendable"))) == 0)) {
            /* pass */
            if ((c->generics->len == 0LL)) {
                /* pass */
                Sema_check_class_sendable_fields(self, c);
            }
        } else if ((strcmp(_tr_strz(_ifc_nm), _tr_strz(_tr_str_lit("UnsafeSendable"))) == 0)) {
            /* pass */
            /* pass */
        } else if ((!_tr_dict_contains(self->interfaces, _tr_strz(_ifc_nm)))) {
            /* pass */
            ({ TrStr _at_t473 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-1] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' declares 'implements "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' but interface '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not defined.\n      FIX: Define 'interface "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":' before this class, or check for typos."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t473); _tr_str_release(_at_t473); });
        } else {
            /* pass */
            InterfaceDef* _idef = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(_ifc_nm)));
            /* pass */
            long long _im_i = 0LL;
            /* pass */
            while ((_im_i < _idef->methods->len)) {
                /* pass */
                FunctionDef* _imeth = ((FunctionDef*)List_ptr_get(_idef->methods, _im_i));
                /* pass */
                bool _found = false;
                /* pass */
                long long _cm_i = 0LL;
                /* pass */
                while ((_cm_i < c->methods->len)) {
                    /* pass */
                    FunctionDef* _cmeth = ((FunctionDef*)List_ptr_get(c->methods, _cm_i));
                    /* pass */
                    if ((strcmp(_tr_strz(_cmeth->name), _tr_strz(_imeth->name)) == 0)) {
                        /* pass */
                        _found = true;
                        /* pass */
                        TrStr _iret = _tr_str_lit("void");
                        /* pass */
                        if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t474 = _tr_str_retain((*_imeth->ret_ty)->name);
                            _tr_str_release(_iret);
                            _iret = _strtmp_t474;
                        }
                        /* pass */
                        TrStr _cret = _tr_str_lit("void");
                        /* pass */
                        if ((((unsigned long long)(_cmeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            TrStr _strtmp_t475 = _tr_str_retain((*_cmeth->ret_ty)->name);
                            _tr_str_release(_cret);
                            _cret = _strtmp_t475;
                        }
                        /* pass */
                        bool _iret_is_generic = ((((_ifc_i >= 0LL) && (_idef->generics->len > 0LL)) && (strcmp(_tr_strz(_iret), _tr_strz(_tr_str_lit("void"))) != 0)) && (strcmp(_tr_strz(_iret), _tr_strz(_tr_str_lit(""))) != 0));
                        /* pass */
                        long long _gi = 0LL;
                        /* pass */
                        while ((_gi < _idef->generics->len)) {
                            /* pass */
                            if ((strcmp(_tr_strz(List_TrStr_get(_idef->generics, _gi)), _tr_strz(_iret)) == 0)) {
                                /* pass */
                                _iret_is_generic = true;
                            }
                            /* pass */
                            _gi = (_gi + 1LL);
                        }
                        /* pass */
                        if (((((!_iret_is_generic) && (strcmp(_tr_strz(_iret), _tr_strz(_tr_str_lit("void"))) != 0)) && (strcmp(_tr_strz(_iret), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(_cret), _tr_strz(_iret)) != 0))) {
                            /* pass */
                            ({ TrStr _at_t476 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-3] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': method '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_cmeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' returns '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_cret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' but interface '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' declares '-> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_iret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: Change return type to '-> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_iret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t476); _tr_str_release(_at_t476); });
                        }
                        /* pass */
                        long long _ip_cnt = 0LL;
                        /* pass */
                        long long _ip_i = 0LL;
                        /* pass */
                        while ((_ip_i < _imeth->params->len)) {
                            /* pass */
                            if ((strcmp(_tr_strz(((Param*)List_ptr_get(_imeth->params, _ip_i))->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                                /* pass */
                                _ip_cnt = (_ip_cnt + 1LL);
                            }
                            /* pass */
                            _ip_i = (_ip_i + 1LL);
                        }
                        /* pass */
                        long long _cp_cnt = 0LL;
                        /* pass */
                        long long _cp_i = 0LL;
                        /* pass */
                        while ((_cp_i < _cmeth->params->len)) {
                            /* pass */
                            if ((strcmp(_tr_strz(((Param*)List_ptr_get(_cmeth->params, _cp_i))->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                                /* pass */
                                _cp_cnt = (_cp_cnt + 1LL);
                            }
                            /* pass */
                            _cp_i = (_cp_i + 1LL);
                        }
                        /* pass */
                        if ((_ip_cnt != _cp_cnt)) {
                            /* pass */
                            ({ TrStr _at_t477 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-3] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': method '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_cmeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' has "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_cp_cnt)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" parameter(s) but interface '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' requires "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(_ip_cnt)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".\n      FIX: Match the parameter list in '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' exactly."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t477); _tr_str_release(_at_t477); });
                        }
                    }
                    /* pass */
                    _cm_i = (_cm_i + 1LL);
                }
                /* pass */
                if ((!_found)) {
                    /* pass */
                    TrStr _sig = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("pub def ")), _tr_strz(_imeth->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(self"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    long long _pi = 0LL;
                    /* pass */
                    while ((_pi < _imeth->params->len)) {
                        /* pass */
                        Param* _p = ((Param*)List_ptr_get(_imeth->params, _pi));
                        /* pass */
                        if ((strcmp(_tr_strz(_p->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                            /* pass */
                            TrStr _pty = _tr_str_lit("int");
                            /* pass */
                            if ((((unsigned long long)(_p->ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                TrStr _strtmp_t478 = _tr_str_retain((*_p->ty)->name);
                                _tr_str_release(_pty);
                                _pty = _strtmp_t478;
                            }
                            /* pass */
                            TrStr _strtmp_t479 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_sig), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_p->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_pty)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(_sig);
                            _sig = _strtmp_t479;
                        }
                        /* pass */
                        _pi = (_pi + 1LL);
                    }
                    /* pass */
                    TrStr _iret2 = _tr_str_lit("void");
                    /* pass */
                    if ((((unsigned long long)(_imeth->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        TrStr _strtmp_t480 = _tr_str_retain((*_imeth->ret_ty)->name);
                        _tr_str_release(_iret2);
                        _iret2 = _strtmp_t480;
                    }
                    /* pass */
                    TrStr _strtmp_t481 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_sig), _tr_strz(_tr_str_lit(") -> ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_iret2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(_sig);
                    _sig = _strtmp_t481;
                    /* pass */
                    ({ TrStr _at_t482 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-2] Class '")), _tr_strz(c->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' implements '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_ifc_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' but is missing method '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_imeth->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: Add to 'extend "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":'  "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_sig)); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t482); _tr_str_release(_at_t482); });
                }
                /* pass */
                _im_i = (_im_i + 1LL);
            }
        }
        /* pass */
        _ifc_i = (_ifc_i + 1LL);
        _tr_str_release(_ifc_nm);
    }
    /* pass */
    HirClass* hc = ((HirClass*)_tr_obj_alloc(sizeof(HirClass)));
    /* pass */
    hc->name = _tr_str_retain(c->name);
    /* pass */
    hc->generics = c->generics;
    /* pass */
    hc->fields = hfields;
    /* pass */
    hc->methods = hmethods;
    /* pass */
    hc->base_classes = c->base_classes;
    /* pass */
    hc->iface_names = c->iface_names;
    /* pass */
    hc->iface_targs = c->iface_targs;
    /* pass */
    hc->decorators = c->decorators;
    /* pass */
    hc->is_public = c->is_public;
    /* pass */
    hc->is_class = c->is_class;
    /* pass */
    self->current_class_name = _tr_str_lit("");
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return hc;
}

__attribute__((hot)) HirEnum* Sema_lower_enum(Sema* self, EnumDef* e) {
    /* pass */
    self->current_class_name = _tr_str_retain(e->name);
    /* pass */
    self->current_region_params = e->region_params;
    /* pass */
    List_ptr* hvariants = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < e->variants->len)) {
        /* pass */
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, i));
        /* pass */
        List_ptr* hfields = (void*)List_ptr_new();
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < v->fields->len)) {
            /* pass */
            Param* p = ((Param*)List_ptr_get(v->fields, j));
            /* pass */
            if ((self->strict_mode && (((unsigned long long)(p->ty)) != ((unsigned long long)(0LL))))) {
                /* pass */
                AstType* _vty = (*p->ty);
                /* pass */
                long long _vri = 0LL;
                /* pass */
                while ((_vri < _vty->from_regions->len)) {
                    /* pass */
                    TrStr _vrn = List_TrStr_get(_vty->from_regions, _vri);
                    /* pass */
                    bool _vrok = false;
                    /* pass */
                    long long _vrk = 0LL;
                    /* pass */
                    while ((_vrk < e->region_params->len)) {
                        /* pass */
                        if ((strcmp(_tr_strz(List_TrStr_get(e->region_params, _vrk)), _tr_strz(_vrn)) == 0)) {
                            /* pass */
                            _vrok = true;
                        }
                        /* pass */
                        _vrk = (_vrk + 1LL);
                    }
                    /* pass */
                    if ((!_vrok)) {
                        /* pass */
                        self->current_line = e->line;
                        /* pass */
                        ({ TrStr _at_t483 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-2] region source '")), _tr_strz(_vrn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in variant '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' of enum '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not a region parameter.\n      FIX: declare 'enum "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" from "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_vrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t483); _tr_str_release(_at_t483); });
                    }
                    /* pass */
                    _vri = (_vri + 1LL);
                    _tr_str_release(_vrn);
                }
            }
            /* pass */
            HirParam* hp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
            /* pass */
            hp->name = _tr_str_retain(p->name);
            /* pass */
            AstType* p_ty = AstType_init(p->name);
            /* pass */
            if ((((unsigned long long)(p->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                p_ty = (*p->ty);
            }
            /* pass */
            hp->ty = p_ty;
            /* pass */
            List_ptr_append(hfields, _tr_obj_retain(hp));
            /* pass */
            j = (j + 1LL);
            _tr_obj_release(hp, _trdrop_HirParam);
        }
        /* pass */
        HirVariant* hv = ((HirVariant*)_tr_obj_alloc(sizeof(HirVariant)));
        /* pass */
        hv->name = _tr_str_retain(v->name);
        /* pass */
        hv->fields = hfields;
        /* pass */
        List_ptr_append(hvariants, _tr_obj_retain(hv));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hv, _trdrop_HirVariant);
    }
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < e->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(e->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    HirEnum* he = ((HirEnum*)_tr_obj_alloc(sizeof(HirEnum)));
    /* pass */
    he->name = _tr_str_retain(e->name);
    /* pass */
    he->generics = e->generics;
    /* pass */
    he->variants = hvariants;
    /* pass */
    he->methods = hmethods;
    /* pass */
    he->iface_names = e->iface_names;
    /* pass */
    he->decorators = e->decorators;
    /* pass */
    he->is_public = e->is_public;
    /* pass */
    self->current_class_name = _tr_str_lit("");
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return he;
}

__attribute__((hot)) HirInterface* Sema_lower_interface(Sema* self, InterfaceDef* i_def) {
    /* pass */
    self->current_region_params = i_def->region_params;
    /* pass */
    List_ptr* hmethods = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < i_def->methods->len)) {
        /* pass */
        HirFunction* hm = Sema_lower_func(self, ((FunctionDef*)List_ptr_get(i_def->methods, i)));
        /* pass */
        List_ptr_append(hmethods, _tr_obj_retain(hm));
        /* pass */
        i = (i + 1LL);
        _tr_obj_release(hm, _trdrop_HirFunction);
    }
    /* pass */
    HirInterface* hi = ((HirInterface*)_tr_obj_alloc(sizeof(HirInterface)));
    /* pass */
    hi->name = _tr_str_retain(i_def->name);
    /* pass */
    hi->generics = i_def->generics;
    /* pass */
    hi->methods = hmethods;
    /* pass */
    self->current_region_params = (void*)List_TrStr_new();
    /* pass */
    return hi;
}

__attribute__((hot)) void Sema_apply_escape_marks(Sema* self, HirStmt* _hs) {
    /* pass */
    __auto_type _t484 = (*_hs);
    if (_t484.tag == HirStmt_SExpr) {
        __auto_type _es_e = _t484.data.SExpr.expr;
        /* pass */
        Sema_mark_escaped_str_args(self, _es_e);
        /* pass */
        Sema_mark_escaped_coll_args(self, _es_e);
    } else if (_t484.tag == HirStmt_SLet) {
        __auto_type _sl_v = _t484.data.SLet.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sl_v);
        /* pass */
        Sema_mark_coll_arg(self, _sl_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sl_v);
    } else if (_t484.tag == HirStmt_SAssign) {
        __auto_type _sa_t = _t484.data.SAssign.target;
__auto_type _sa_v = _t484.data.SAssign.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sa_t);
        /* pass */
        Sema_mark_escaped_str_args(self, _sa_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sa_t);
        /* pass */
        Sema_mark_coll_arg(self, _sa_v);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sa_v);
    } else if (_t484.tag == HirStmt_SReturn) {
        __auto_type _sr_e = _t484.data.SReturn.val;
        /* pass */
        Sema_mark_escaped_str_args(self, _sr_e);
        /* pass */
        Sema_mark_coll_arg(self, _sr_e);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sr_e);
    } else if (_t484.tag == HirStmt_SIf) {
        __auto_type _si_c = _t484.data.SIf.cond;
        /* pass */
        Sema_mark_escaped_str_args(self, _si_c);
        /* pass */
        Sema_mark_escaped_coll_args(self, _si_c);
    } else if (_t484.tag == HirStmt_SWhile) {
        __auto_type _sw_c = _t484.data.SWhile.cond;
        /* pass */
        Sema_mark_escaped_str_args(self, _sw_c);
        /* pass */
        Sema_mark_escaped_coll_args(self, _sw_c);
    } else if (_t484.tag == HirStmt_SFor) {
        __auto_type _sf_iter = _t484.data.SFor.iter;
        /* pass */
        Sema_mark_escaped_coll_args(self, _sf_iter);
    } else if (_t484.tag == HirStmt_SForUnpack) {
        __auto_type _sfu_iter = _t484.data.SForUnpack.iter;
        /* pass */
        Sema_mark_escaped_coll_args(self, _sfu_iter);
    } else if (1) {
        __auto_type _ = _t484;
        /* pass */
    }
}

__attribute__((hot)) HirBlock* Sema_lower_block(Sema* self, Block* b) {
    /* pass */
    HirBlock* hb = HirBlock_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Stmt* orig_ptr = ((Stmt*)List_ptr_get(b->stmts, i));
        /* pass */
        HirStmt* _hs = Sema_lower_stmt(self, orig_ptr);
        /* pass */
        Sema_apply_escape_marks(self, _hs);
        /* pass */
        if ((((unsigned long long)(orig_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t485 = (*orig_ptr);
            if (_t485.tag == Stmt_SReturn) {
                __auto_type _ = _t485.data.SReturn.val;
                /* pass */
                List_TrStr* ret_excl = (void*)List_TrStr_new();
                /* pass */
                __auto_type _t486 = (*_hs);
                if (_t486.tag == HirStmt_SReturn) {
                    __auto_type lowered_ret = _t486.data.SReturn.val;
                    /* pass */
                    Sema_collect_idents(self, lowered_ret, ret_excl);
                } else if (1) {
                    __auto_type _ = _t486;
                    /* pass */
                }
                /* pass */
                long long ret_from = 0LL;
                /* pass */
                if ((self->fn_scope_base->len > 0LL)) {
                    /* pass */
                    ret_from = List_i64_get(self->fn_scope_base, (self->fn_scope_base->len - 1LL));
                }
                /* pass */
                Sema_append_drops_from_excl_multi(self, hb, ret_from, ret_excl);
            } else if (_t485.tag == Stmt_SBreak) {
                __auto_type _ = _t485.data.SBreak.val;
                /* pass */
                if ((self->loop_scope_base->len > 0LL)) {
                    /* pass */
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
                }
            } else if (_t485.tag == Stmt_SContinue) {
                /* pass */
                if ((self->loop_scope_base->len > 0LL)) {
                    /* pass */
                    Sema_append_drops_from(self, hb, List_i64_get(self->loop_scope_base, (self->loop_scope_base->len - 1LL)));
                }
            } else if (1) {
                __auto_type _ = _t485;
                /* pass */
            }
        }
        /* pass */
        HirBlock_push(hb, _hs);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return hb;
}

__attribute__((hot)) HirStmt* Sema_lower_stmt(Sema* self, Stmt* s_ptr) {
    /* pass */
    if ((((unsigned long long)(s_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (HirStmt*)(0LL);
    }
    /* pass */
    __auto_type s = (*s_ptr);
    /* pass */
    __auto_type _t487 = s;
    if (_t487.tag == Stmt_SExpr) {
        __auto_type e = _t487.data.SExpr.expr;
        /* pass */
        HirStmt* h_s_expr = box_hirstmt(HirStmt_ctor_SExpr(Sema_lower_expr(self, e)));
        /* pass */
        if ((((unsigned long long)(e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t488 = (*e);
            if (_t488.tag == Expr_ECall) {
                __auto_type callee = _t488.data.ECall.callee;
__auto_type args = _t488.data.ECall.args;
                /* pass */
                if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t489 = (*callee);
                    if (_t489.tag == Expr_EIdent) {
                        __auto_type fn_name = _t489.data.EIdent.name;
                        /* pass */
                        Symbol* fn_sym = Sema_resolve(self, fn_name);
                        /* pass */
                        if (((fn_sym->kind.tag == SymbolKind_make_SFunction().tag) && (strcmp(_tr_strz((*fn_sym->ty)->name), _tr_strz(_tr_str_lit("Result"))) == 0))) {
                            /* pass */
                            ({ TrStr _at_t490 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-4] '")), _tr_strz(fn_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()' returns a Result and its error must be handled. FIX: Assign the result and match on it, use '?' to propagate, or '_ = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fn_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(...)' to explicitly discard."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t490); _tr_str_release(_at_t490); });
                        }
                    } else if (1) {
                        __auto_type _ = _t489;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t488;
                /* pass */
            }
        }
        /* pass */
        return h_s_expr;
    } else if (_t487.tag == Stmt_SReturn) {
        __auto_type e = _t487.data.SReturn.val;
        /* pass */
        if (((self->strict_mode && self->current_func_ret_borrow_str) && (((unsigned long long)(e)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t491 = (*e);
            if (_t491.tag == Expr_EBinOp) {
                __auto_type l3_op = _t491.data.EBinOp.op;
                /* pass */
                if ((strcmp(_tr_strz(l3_op), _tr_strz(_tr_str_lit("+"))) == 0)) {
                    /* pass */
                    ({ TrStr _at_t492 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-3] borrow-returning function returns a freshly-built string (owned), not a borrow of region '")), _tr_strz(self->current_func_ret_from))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t492); _tr_str_release(_at_t492); });
                }
            } else if (_t491.tag == Expr_EIdent) {
                __auto_type l3_nm = _t491.data.EIdent.name;
                /* pass */
                Symbol* l3_sym = Sema_resolve(self, l3_nm);
                /* pass */
                if ((strcmp(_tr_strz(l3_sym->name), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr l3_eff = _tr_str_lit("");
                    /* pass */
                    if (l3_sym->is_param) {
                        /* pass */
                        TrStr _strtmp_t493 = _tr_str_retain(l3_nm);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t493;
                    } else {
                        /* pass */
                        TrStr _strtmp_t494 = _tr_str_retain(l3_sym->borrows_region);
                        _tr_str_release(l3_eff);
                        l3_eff = _strtmp_t494;
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(l3_eff), _tr_strz(_tr_str_lit("@owned"))) == 0)) {
                        /* pass */
                        ({ TrStr _at_t495 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-3] borrow-returning function returns '")), _tr_strz(l3_nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', a freshly-built string (owned), not a borrow of region '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: return a slice/view of the region source, or change the return type to a plain owned 'str'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t495); _tr_str_release(_at_t495); });
                    } else if (((strcmp(_tr_strz(l3_eff), _tr_strz(_tr_str_lit(""))) != 0) && (self->current_func_ret_regions->len > 0LL))) {
                        /* pass */
                        bool l3_in = false;
                        /* pass */
                        long long l3ri = 0LL;
                        /* pass */
                        while ((l3ri < self->current_func_ret_regions->len)) {
                            /* pass */
                            if ((strcmp(_tr_strz(List_TrStr_get(self->current_func_ret_regions, l3ri)), _tr_strz(l3_eff)) == 0)) {
                                /* pass */
                                l3_in = true;
                            }
                            /* pass */
                            l3ri = (l3ri + 1LL);
                        }
                        /* pass */
                        if ((!l3_in)) {
                            /* pass */
                            long long l3di = 0LL;
                            /* pass */
                            while ((l3di < self->current_func_ret_regions->len)) {
                                /* pass */
                                if (({ TrStr _at_t496 = (List_TrStr_get(self->current_func_ret_regions, l3di)); __auto_type _wr = (Sema_region_outlives(self, l3_eff, _at_t496)); _tr_str_release(_at_t496); _wr; })) {
                                    /* pass */
                                    l3_in = true;
                                }
                                /* pass */
                                l3di = (l3di + 1LL);
                            }
                        }
                        /* pass */
                        if ((!l3_in)) {
                            /* pass */
                            ({ TrStr _at_t497 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-4] borrow-returning function returns a borrow of region '")), _tr_strz(l3_eff))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', but its return is declared 'from "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: add '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(l3_eff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' to the 'from' list, declare 'where "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(l3_eff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" outlives "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_ret_from)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', or return a borrow of a declared region."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t497); _tr_str_release(_at_t497); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t491;
                /* pass */
            }
        }
        /* pass */
        if ((((((unsigned long long)(e)) != ((unsigned long long)(0LL))) && (!self->in_unsafe)) && (strcmp(_tr_strz(self->current_func_ret_from), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            __auto_type _t498 = (*e);
            if (_t498.tag == Expr_EIdent) {
                __auto_type ret_name = _t498.data.EIdent.name;
                /* pass */
                Symbol* ret_sym = Sema_resolve(self, ret_name);
                /* pass */
                if (((strcmp(_tr_strz(ret_sym->name), _tr_strz(_tr_str_lit(""))) != 0) && (ret_sym->ptr_region == 0LL))) {
                    /* pass */
                    if ((((unsigned long long)(ret_sym->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        if ((strcmp(_tr_strz((*ret_sym->ty)->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
                            /* pass */
                            ({ TrStr _at_t499 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-1] '")), _tr_strz(ret_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a local Pointer that may not outlive this function call. Returning it is unsafe.\n      FIX: Annotate the return type with 'from <param>' if the pointer borrows from a parameter, or wrap the allocation in 'unsafe:' if it is heap-allocated."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t499); _tr_str_release(_at_t499); });
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t498;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SReturn(Sema_lower_expr(self, e)));
    } else if (_t487.tag == Stmt_SLet) {
        __auto_type name = _t487.data.SLet.name;
__auto_type ownership = _t487.data.SLet.ownership;
__auto_type is_mut = _t487.data.SLet.is_mut;
__auto_type is_const = _t487.data.SLet.is_const;
__auto_type is_shared = _t487.data.SLet.is_shared;
__auto_type ty_ptr = _t487.data.SLet.ty;
__auto_type val_ptr = _t487.data.SLet.val;
        /* pass */
        if (((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL))) && (((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t500 = (*val_ptr);
            if (_t500.tag == Expr_ELitNone) {
                /* pass */
                TrStr m7_ty_name = _tr_str_retain((*ty_ptr)->name);
                /* pass */
                if ((((((strcmp(_tr_strz(m7_ty_name), _tr_strz(_tr_str_lit("Option"))) != 0) && (strcmp(_tr_strz(m7_ty_name), _tr_strz(_tr_str_lit("None"))) != 0)) && (strcmp(_tr_strz(m7_ty_name), _tr_strz(_tr_str_lit("void"))) != 0)) && (strcmp(_tr_strz(m7_ty_name), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(m7_ty_name), _tr_strz(_tr_str_lit("Pointer"))) != 0))) {
                    /* pass */
                    ({ TrStr _at_t501 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-7] Cannot assign 'none' to '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' which has type '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m7_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'. Only Option[T] can hold 'none'. FIX: Use 'Option["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m7_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' as the type, or give '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' a real initial value."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t501); _tr_str_release(_at_t501); });
                }
                _tr_str_release(m7_ty_name);
            } else if (1) {
                __auto_type _ = _t500;
                /* pass */
            }
        }
        /* pass */
        AstType* ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ty = (*ty_ptr);
        }
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        if (((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("void"))) != 0) && (strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("None"))) != 0))) {
            /* pass */
            hval = Sema__patch_empty_dict_hint(self, hval, ty);
        }
        /* pass */
        if (((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("None"))) == 0))) {
            /* pass */
            ty = hir_expr_type(hval);
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (!is_shared))) {
            /* pass */
            __auto_type _t502 = (*val_ptr);
            if (_t502.tag == Expr_EIdent) {
                __auto_type m1_src = _t502.data.EIdent.name;
                /* pass */
                Symbol* m1_sym = Sema_resolve(self, m1_src);
                /* pass */
                AstType* m1_ty = (*m1_sym->ty);
                /* pass */
                if (((((!Sema_is_primitive(self, m1_ty)) && (!Sema_is_copy_class(self, m1_ty->name))) && (strcmp(_tr_strz(m1_sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (!m1_sym->is_shared))) {
                    /* pass */
                    Sema_check_not_moved(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_check_no_active_borrows(self, m1_src, m1_ty->name);
                    /* pass */
                    Sema_mark_moved(self, m1_src);
                }
            } else if (1) {
                __auto_type _ = _t502;
                /* pass */
            }
        }
        /* pass */
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(ty), is_mut);
        /* pass */
        if ((is_shared && (self->scopes->len > 0LL))) {
            /* pass */
            Scope* _sh_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
            /* pass */
            if (_tr_dict_contains(_sh_scope->variables, _tr_strz(name))) {
                /* pass */
                Symbol* _sh_sym = ((Symbol*)(uintptr_t)_tr_dict_get(_sh_scope->variables, _tr_strz(name)));
                /* pass */
                _sh_sym->is_shared = true;
                /* pass */
                _tr_dict_set(_sh_scope->variables, _tr_strz(name), _sh_sym);
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ({ TrStr _at_t503 = (Sema_compute_region(self, val_ptr)); Sema_set_borrows_region(self, name, _at_t503); _tr_str_release(_at_t503); });
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t504 = (*val_ptr);
            if (_t504.tag == Expr_ECall) {
                __auto_type ce_callee = _t504.data.ECall.callee;
__auto_type ce_args = _t504.data.ECall.args;
                /* pass */
                if ((((unsigned long long)(ce_callee)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t505 = (*ce_callee);
                    if (_t505.tag == Expr_EIdent) {
                        __auto_type ce_fn = _t505.data.EIdent.name;
                        /* pass */
                        Symbol* ce_sym = Sema_resolve(self, ce_fn);
                        /* pass */
                        if (((strcmp(_tr_strz(ce_sym->name), _tr_strz(_tr_str_lit(""))) != 0) && (((unsigned long long)(ce_sym->ty)) != ((unsigned long long)(0LL))))) {
                            /* pass */
                            AstType* ce_ret = (*ce_sym->ty);
                            /* pass */
                            if ((((ce_ret->is_borrow || (ce_ret->from_regions->len > 0LL)) && (ce_ret->from_index >= 0LL)) && (ce_ret->from_index < ce_args->len))) {
                                /* pass */
                                __auto_type _t506 = (*((Expr*)List_ptr_get(ce_args, ce_ret->from_index)));
                                if (_t506.tag == Expr_EIdent) {
                                    __auto_type ce_src = _t506.data.EIdent.name;
                                    /* pass */
                                    List_TrStr_append(self->cur_func_borrowers, name);
                                    /* pass */
                                    List_TrStr_append(self->cur_func_sources, ce_src);
                                } else if (1) {
                                    __auto_type _ = _t506;
                                    /* pass */
                                }
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t505;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t504;
                /* pass */
            }
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL))) && (strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0))) {
            /* pass */
            bool heap_rhs = false;
            /* pass */
            __auto_type _t507 = (*val_ptr);
            if (_t507.tag == Expr_ECall) {
                heap_rhs = true;
            } else if (_t507.tag == Expr_EMethodCall) {
                heap_rhs = true;
            } else if (1) {
                __auto_type _ = _t507;
                /* pass */
            }
            /* pass */
            if (heap_rhs) {
                /* pass */
                if ((self->scopes->len > 0LL)) {
                    /* pass */
                    Scope* alloc_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                    /* pass */
                    if (_tr_dict_contains(alloc_scope->variables, _tr_strz(name))) {
                        /* pass */
                        Symbol* alloc_sym = ((Symbol*)(uintptr_t)_tr_dict_get(alloc_scope->variables, _tr_strz(name)));
                        /* pass */
                        alloc_sym->ptr_region = 1LL;
                        /* pass */
                        _tr_dict_set(alloc_scope->variables, _tr_strz(name), alloc_sym);
                    }
                }
            }
        }
        /* pass */
        bool _decl_fixed_arr = false;
        /* pass */
        if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if (((strcmp(_tr_strz((*ty_ptr)->name), _tr_strz(_tr_str_lit("Array"))) == 0) && ((*ty_ptr)->array_size > 0LL))) {
                /* pass */
                _decl_fixed_arr = true;
            }
        }
        /* pass */
        if (((((unsigned long long)(val_ptr)) == ((unsigned long long)(0LL))) && (!_decl_fixed_arr))) {
            /* pass */
            if ((self->scopes->len > 0LL)) {
                /* pass */
                Scope* pd_scope = ((Scope*)List_ptr_get(self->scopes, (self->scopes->len - 1LL)));
                /* pass */
                if (_tr_dict_contains(pd_scope->variables, _tr_strz(name))) {
                    /* pass */
                    Symbol* pd_sym = ((Symbol*)(uintptr_t)_tr_dict_get(pd_scope->variables, _tr_strz(name)));
                    /* pass */
                    pd_sym->is_init = false;
                    /* pass */
                    _tr_dict_set(pd_scope->variables, _tr_strz(name), pd_sym);
                }
            }
        }
        /* pass */
        if ((((unsigned long long)(val_ptr)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr pc_cont_nm = _tr_str_lit("");
            /* pass */
            TrStr pc_meth = _tr_str_lit("");
            /* pass */
            __auto_type _t508 = (*val_ptr);
            if (_t508.tag == Expr_EMethodCall) {
                __auto_type pc_obj = _t508.data.EMethodCall.obj;
__auto_type pc_m = _t508.data.EMethodCall.method;
                /* pass */
                TrStr _strtmp_t509 = _tr_str_retain(pc_m);
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t509;
                /* pass */
                __auto_type _t510 = (*pc_obj);
                if (_t510.tag == Expr_EIdent) {
                    __auto_type pc_src = _t510.data.EIdent.name;
                    TrStr _strtmp_t511 = _tr_str_retain(pc_src);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t511;
                } else if (1) {
                    __auto_type _ = _t510;
                    /* pass */
                }
            } else if (_t508.tag == Expr_EIndex) {
                __auto_type pc_iobj = _t508.data.EIndex.obj;
                /* pass */
                TrStr _strtmp_t512 = _tr_str_lit("get");
                _tr_str_release(pc_meth);
                pc_meth = _strtmp_t512;
                /* pass */
                __auto_type _t513 = (*pc_iobj);
                if (_t513.tag == Expr_EIdent) {
                    __auto_type pc_isrc = _t513.data.EIdent.name;
                    TrStr _strtmp_t514 = _tr_str_retain(pc_isrc);
                    _tr_str_release(pc_cont_nm);
                    pc_cont_nm = _strtmp_t514;
                } else if (1) {
                    __auto_type _ = _t513;
                    /* pass */
                }
            } else if (1) {
                __auto_type _ = _t508;
                /* pass */
            }
            /* pass */
            if (((strcmp(_tr_strz(pc_cont_nm), _tr_strz(_tr_str_lit(""))) != 0) && (((strcmp(_tr_strz(pc_meth), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(pc_meth), _tr_strz(_tr_str_lit("first"))) == 0)) || (strcmp(_tr_strz(pc_meth), _tr_strz(_tr_str_lit("last"))) == 0)))) {
                /* pass */
                Symbol* pc_cont_sym = Sema_resolve(self, pc_cont_nm);
                /* pass */
                AstType* pc_cont_ty = (*pc_cont_sym->ty);
                /* pass */
                TrStr pc_ctn = _tr_str_retain(pc_cont_ty->name);
                /* pass */
                if (((strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("Mutex"))) == 0) || (strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("RwLock"))) == 0))) {
                    /* pass */
                    Sema_set_borrows_region(self, name, _tr_str_lit("@borrowed"));
                } else if ((((((strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(pc_ctn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
                    /* pass */
                    TrStr pc_elem_ty = _tr_str_retain(hir_expr_type(hval)->name);
                    /* pass */
                    bool pc_is_vt = (_tr_dict_contains(self->classes, _tr_strz(pc_elem_ty)) && (!((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(pc_elem_ty)))->is_class));
                    /* pass */
                    if (((!Sema_is_primitive(self, hir_expr_type(hval))) && (!pc_is_vt))) {
                        /* pass */
                        _tr_dict_set(self->container_borrows, _tr_strz(pc_cont_nm), _tr_str_box(_tr_str_retain(name)));
                        /* pass */
                        if (((((((strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("str"))) != 0) && (strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("Vec"))) != 0)) && (strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("List"))) != 0)) && (strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("Dict"))) != 0)) && (strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("Map"))) != 0)) && (strcmp(_tr_strz(pc_elem_ty), _tr_strz(_tr_str_lit("Set"))) != 0))) {
                            /* pass */
                            Sema_set_borrows_region(self, name, _tr_str_lit("@borrowed"));
                        }
                    }
                }
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SLet(name, ownership, is_mut, is_const, is_shared, ty, hval));
    } else if (_t487.tag == Stmt_SAssign) {
        __auto_type target = _t487.data.SAssign.target;
__auto_type val = _t487.data.SAssign.val;
        /* pass */
        self->in_assign_target = true;
        /* pass */
        HirExpr* htgt = Sema_lower_expr(self, target);
        /* pass */
        self->in_assign_target = false;
        /* pass */
        HirExpr* hv = Sema_lower_expr(self, val);
        /* pass */
        hv = Sema__patch_empty_dict_hint(self, hv, hir_expr_type(htgt));
        /* pass */
        if ((self->strict_mode && (((unsigned long long)(target)) != ((unsigned long long)(0LL))))) {
            /* pass */
            __auto_type _t515 = (*target);
            if (_t515.tag == Expr_EPropAccess) {
                __auto_type l5_obj = _t515.data.EPropAccess.obj;
__auto_type l5_field = _t515.data.EPropAccess.prop;
                /* pass */
                if (((strcmp(_tr_strz(Sema_compute_region(self, val)), _tr_strz(_tr_str_lit("@owned"))) == 0) && Sema_field_is_borrow(self, l5_obj, l5_field))) {
                    /* pass */
                    ({ TrStr _at_t516 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[L-5] storing a freshly-built (owned) string into the borrow field '")), _tr_strz(l5_field))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' (declared 'ref').\n      FIX: store a borrow of the field's region, or make the field a plain owned 'str'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t516); _tr_str_release(_at_t516); });
                }
            } else if (1) {
                __auto_type _ = _t515;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t517 = (*target);
            if (_t517.tag == Expr_EIdent) {
                __auto_type sa_decl_name = _t517.data.EIdent.name;
                /* pass */
                ({ TrStr _at_t518 = (Sema_compute_region(self, val)); Sema_set_borrows_region(self, sa_decl_name, _at_t518); _tr_str_release(_at_t518); });
                /* pass */
                Symbol* sa_existing = Sema_resolve(self, sa_decl_name);
                /* pass */
                if ((strcmp(_tr_strz(sa_existing->name), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    Sema_declare(self, sa_decl_name, SymbolKind_make_SVariable(), box_asttype(hir_expr_type(hv)), false);
                } else if ((((self->scopes->len > 0LL) && Sema_is_global_not_local(self, sa_decl_name)) && (!sa_existing->is_mut))) {
                    /* pass */
                    Sema_declare(self, sa_decl_name, SymbolKind_make_SVariable(), box_asttype(hir_expr_type(hv)), true);
                    /* pass */
                    Sema_mark_init(self, sa_decl_name);
                    /* pass */
                    return box_hirstmt(HirStmt_ctor_SLet(sa_decl_name, Ownership_make_Own(), true, false, false, hir_expr_type(hv), hv));
                } else if (((sa_existing->kind.tag == SymbolKind_make_SVariable().tag) && (!sa_existing->is_mut))) {
                    /* pass */
                    ({ TrStr _at_t519 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-8] Cannot assign to '")), _tr_strz(sa_decl_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' a second time because it is immutable.\n      FIX: Declare it as 'mut "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sa_decl_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = ...' if it needs to change."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t519); _tr_str_release(_at_t519); });
                }
            } else if (1) {
                __auto_type _ = _t517;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t520 = (*val);
            if (_t520.tag == Expr_EIdent) {
                __auto_type sa_src = _t520.data.EIdent.name;
                /* pass */
                Symbol* sa_sym = Sema_resolve(self, sa_src);
                /* pass */
                AstType* sa_ty = (*sa_sym->ty);
                /* pass */
                bool sa_is_known = (_tr_dict_contains(self->classes, _tr_strz(sa_ty->name)) || _tr_dict_contains(self->enums, _tr_strz(sa_ty->name)));
                /* pass */
                if (((((sa_is_known && (!Sema_is_primitive(self, sa_ty))) && (!Sema_is_copy_class(self, sa_ty->name))) && (strcmp(_tr_strz(sa_sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (!sa_sym->is_shared))) {
                    /* pass */
                    Sema_check_no_active_borrows(self, sa_src, sa_ty->name);
                    /* pass */
                    Sema_mark_moved(self, sa_src);
                }
            } else if (1) {
                __auto_type _ = _t520;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t521 = (*target);
            if (_t521.tag == Expr_EIdent) {
                __auto_type pd_tgt = _t521.data.EIdent.name;
                Sema_mark_init(self, pd_tgt);
            } else if (1) {
                __auto_type _ = _t521;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(target)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t522 = (*target);
            if (_t522.tag == Expr_EIdent) {
                __auto_type pc_tgt = _t522.data.EIdent.name;
                Sema_clear_container_borrow(self, pc_tgt);
            } else if (1) {
                __auto_type _ = _t522;
                /* pass */
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAssign(htgt, hv));
    } else if (_t487.tag == Stmt_SIf) {
        __auto_type cond = _t487.data.SIf.cond;
__auto_type then_b = _t487.data.SIf.then_b;
__auto_type elifs = _t487.data.SIf.elifs;
__auto_type else_b = _t487.data.SIf.else_b;
        /* pass */
        if ((((unsigned long long)(cond)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t523 = (*cond);
            if (_t523.tag == Expr_EIdent) {
                __auto_type t5_name = _t523.data.EIdent.name;
                /* pass */
                Symbol* t5_sym = Sema_resolve(self, t5_name);
                /* pass */
                TrStr t5_ty = _tr_str_retain((*t5_sym->ty)->name);
                /* pass */
                if ((((((strcmp(_tr_strz(t5_ty), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(t5_ty), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(t5_ty), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(t5_ty), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(t5_ty), _tr_strz(_tr_str_lit("f64"))) == 0))) {
                    /* pass */
                    ({ TrStr _at_t524 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-5] '")), _tr_strz(t5_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a number ("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(t5_ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") and cannot be used as an 'if' condition. FIX: Write 'if "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(t5_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" != 0:' to explicitly check for non-zero."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t524); _tr_str_release(_at_t524); });
                }
                _tr_str_release(t5_ty);
            } else if (1) {
                __auto_type _ = _t523;
                /* pass */
            }
        }
        /* pass */
        bool si_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* si_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool si_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* si_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        HirExpr* hcond = Sema_lower_expr(self, cond);
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* hthen = Sema_lower_block(self, then_b);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_TrStr* si_then_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* si_then_inited = self->branch_init_buf;
        /* pass */
        long long si_uti = 0LL;
        /* pass */
        while ((si_uti < si_then_moved->len)) {
            /* pass */
            ({ TrStr _at_t525 = (List_TrStr_get(si_then_moved, si_uti)); Sema_unmark_moved(self, _at_t525); _tr_str_release(_at_t525); });
            /* pass */
            si_uti = (si_uti + 1LL);
        }
        /* pass */
        long long si_uii = 0LL;
        /* pass */
        while ((si_uii < si_then_inited->len)) {
            /* pass */
            ({ TrStr _at_t526 = (List_TrStr_get(si_then_inited, si_uii)); Sema_unmark_init(self, _at_t526); _tr_str_release(_at_t526); });
            /* pass */
            si_uii = (si_uii + 1LL);
        }
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        HirBlock* helse = HirBlock_init();
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        if ((elifs->len > 0LL)) {
            /* pass */
            Sema_open_block(self);
            /* pass */
            HirBlock* base_else = Sema_lower_block(self, else_b);
            /* pass */
            Sema_close_block(self);
            /* pass */
            HirBlock* chain = _tr_obj_retain(base_else);
            /* pass */
            long long k = (elifs->len - 1LL);
            /* pass */
            while ((k >= 0LL)) {
                /* pass */
                ElifClause* elif_c = ((ElifClause*)List_ptr_get(elifs, k));
                /* pass */
                HirExpr* elif_cond = Sema_lower_expr(self, elif_c->cond);
                /* pass */
                Sema_open_block(self);
                /* pass */
                HirBlock* elif_body = Sema_lower_block(self, (*elif_c->body));
                /* pass */
                Sema_close_block(self);
                /* pass */
                HirBlock* nested = HirBlock_init();
                /* pass */
                HirBlock_push(nested, box_hirstmt(HirStmt_ctor_SIf(elif_cond, elif_body, chain)));
                /* pass */
                HirBlock* _cltmp_t527 = _tr_obj_retain(nested);
                _tr_obj_release(chain, _trdrop_HirBlock);
                chain = _cltmp_t527;
                /* pass */
                k = (k - 1LL);
                _tr_obj_release(elif_body, _trdrop_HirBlock);
                _tr_obj_release(nested, _trdrop_HirBlock);
            }
            /* pass */
            HirBlock* _cltmp_t528 = _tr_obj_retain(chain);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t528;
            _tr_obj_release(base_else, _trdrop_HirBlock);
        } else {
            /* pass */
            HirBlock* _cltmp_t529 = Sema_lower_block(self, else_b);
            _tr_obj_release(helse, _trdrop_HirBlock);
            helse = _cltmp_t529;
        }
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_TrStr* si_else_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* si_else_inited = self->branch_init_buf;
        /* pass */
        long long si_uei = 0LL;
        /* pass */
        while ((si_uei < si_else_moved->len)) {
            /* pass */
            ({ TrStr _at_t530 = (List_TrStr_get(si_else_moved, si_uei)); Sema_unmark_moved(self, _at_t530); _tr_str_release(_at_t530); });
            /* pass */
            si_uei = (si_uei + 1LL);
        }
        /* pass */
        long long si_uei2 = 0LL;
        /* pass */
        while ((si_uei2 < si_else_inited->len)) {
            /* pass */
            ({ TrStr _at_t531 = (List_TrStr_get(si_else_inited, si_uei2)); Sema_unmark_init(self, _at_t531); _tr_str_release(_at_t531); });
            /* pass */
            si_uei2 = (si_uei2 + 1LL);
        }
        /* pass */
        self->capturing_moves = si_outer_cap_m;
        /* pass */
        self->branch_moved_buf = si_outer_buf_m;
        /* pass */
        self->capturing_inits = si_outer_cap_i;
        /* pass */
        self->branch_init_buf = si_outer_buf_i;
        /* pass */
        bool si_then_jumps = Sema_block_ends_in_jump(self, hthen);
        /* pass */
        bool si_else_jumps = Sema_block_ends_in_jump(self, helse);
        /* pass */
        if ((si_then_jumps && (!si_else_jumps))) {
            /* pass */
            long long si_ei0 = 0LL;
            /* pass */
            while ((si_ei0 < si_else_moved->len)) {
                /* pass */
                ({ TrStr _at_t532 = (List_TrStr_get(si_else_moved, si_ei0)); Sema_mark_moved(self, _at_t532); _tr_str_release(_at_t532); });
                /* pass */
                si_ei0 = (si_ei0 + 1LL);
            }
            /* pass */
            long long si_ini0 = 0LL;
            /* pass */
            while ((si_ini0 < si_else_inited->len)) {
                /* pass */
                ({ TrStr _at_t533 = (List_TrStr_get(si_else_inited, si_ini0)); Sema_mark_init(self, _at_t533); _tr_str_release(_at_t533); });
                /* pass */
                si_ini0 = (si_ini0 + 1LL);
            }
        } else if ((si_else_jumps && (!si_then_jumps))) {
            /* pass */
            long long si_mi0 = 0LL;
            /* pass */
            while ((si_mi0 < si_then_moved->len)) {
                /* pass */
                ({ TrStr _at_t534 = (List_TrStr_get(si_then_moved, si_mi0)); Sema_mark_moved(self, _at_t534); _tr_str_release(_at_t534); });
                /* pass */
                si_mi0 = (si_mi0 + 1LL);
            }
            /* pass */
            long long si_ti0 = 0LL;
            /* pass */
            while ((si_ti0 < si_then_inited->len)) {
                /* pass */
                ({ TrStr _at_t535 = (List_TrStr_get(si_then_inited, si_ti0)); Sema_mark_init(self, _at_t535); _tr_str_release(_at_t535); });
                /* pass */
                si_ti0 = (si_ti0 + 1LL);
            }
        } else if ((si_then_jumps && si_else_jumps)) {
            /* pass */
            /* pass */
        } else {
            /* pass */
            long long si_mi = 0LL;
            /* pass */
            while ((si_mi < si_then_moved->len)) {
                /* pass */
                TrStr si_mn = List_TrStr_get(si_then_moved, si_mi);
                /* pass */
                if (Sema_vec_str_contains(self, si_else_moved, si_mn)) {
                    /* pass */
                    Sema_mark_moved(self, si_mn);
                } else {
                    /* pass */
                    Sema_mark_maybe_moved(self, si_mn);
                }
                /* pass */
                si_mi = (si_mi + 1LL);
                _tr_str_release(si_mn);
            }
            /* pass */
            long long si_ei = 0LL;
            /* pass */
            while ((si_ei < si_else_moved->len)) {
                /* pass */
                TrStr si_en = List_TrStr_get(si_else_moved, si_ei);
                /* pass */
                if ((!Sema_vec_str_contains(self, si_then_moved, si_en))) {
                    /* pass */
                    Sema_mark_maybe_moved(self, si_en);
                }
                /* pass */
                si_ei = (si_ei + 1LL);
                _tr_str_release(si_en);
            }
            /* pass */
            long long si_ini = 0LL;
            /* pass */
            while ((si_ini < si_then_inited->len)) {
                /* pass */
                TrStr si_inn = List_TrStr_get(si_then_inited, si_ini);
                /* pass */
                if (Sema_vec_str_contains(self, si_else_inited, si_inn)) {
                    /* pass */
                    Sema_mark_init(self, si_inn);
                } else {
                    /* pass */
                    Sema_mark_maybe_init(self, si_inn);
                }
                /* pass */
                si_ini = (si_ini + 1LL);
                _tr_str_release(si_inn);
            }
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SIf(hcond, hthen, helse));
    } else if (_t487.tag == Stmt_SWhile) {
        __auto_type cond = _t487.data.SWhile.cond;
__auto_type body = _t487.data.SWhile.body;
__auto_type decorators = _t487.data.SWhile.decorators;
        /* pass */
        HirExpr* sw_cond = Sema_lower_expr(self, cond);
        /* pass */
        bool sw_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* sw_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sw_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* sw_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        List_i64_append(self->loop_scope_base, self->scopes->len);
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        long long sw_block_id = List_i64_get(self->block_stack, (self->block_stack->len - 1LL));
        /* pass */
        HirBlock* sw_body = Sema_lower_block(self, body);
        /* pass */
        Sema_append_block_local_drops(self, sw_body, sw_block_id);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        List_TrStr* sw_loop_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* sw_loop_inited = self->branch_init_buf;
        /* pass */
        self->capturing_moves = sw_outer_cap_m;
        /* pass */
        self->branch_moved_buf = sw_outer_buf_m;
        /* pass */
        self->capturing_inits = sw_outer_cap_i;
        /* pass */
        self->branch_init_buf = sw_outer_buf_i;
        /* pass */
        long long sw_mi = 0LL;
        /* pass */
        while ((sw_mi < sw_loop_moved->len)) {
            /* pass */
            ({ TrStr _at_t536 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_unmark_moved(self, _at_t536); _tr_str_release(_at_t536); });
            /* pass */
            ({ TrStr _at_t537 = (List_TrStr_get(sw_loop_moved, sw_mi)); Sema_mark_maybe_moved(self, _at_t537); _tr_str_release(_at_t537); });
            /* pass */
            sw_mi = (sw_mi + 1LL);
        }
        /* pass */
        long long sw_ii = 0LL;
        /* pass */
        while ((sw_ii < sw_loop_inited->len)) {
            /* pass */
            ({ TrStr _at_t538 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_unmark_init(self, _at_t538); _tr_str_release(_at_t538); });
            /* pass */
            ({ TrStr _at_t539 = (List_TrStr_get(sw_loop_inited, sw_ii)); Sema_mark_maybe_init(self, _at_t539); _tr_str_release(_at_t539); });
            /* pass */
            sw_ii = (sw_ii + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWhile(sw_cond, sw_body));
    } else if (_t487.tag == Stmt_SFor) {
        __auto_type var = _t487.data.SFor.var;
__auto_type iter = _t487.data.SFor.iter;
__auto_type body = _t487.data.SFor.body;
__auto_type decorators = _t487.data.SFor.decorators;
__auto_type for_is_ref = _t487.data.SFor.is_ref;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        HirExpr* h_iter_for = Sema_lower_expr(self, iter);
        /* pass */
        AstType* var_ty_for = AstType_init(_tr_str_lit("int"));
        /* pass */
        TrStr iter_hn = _tr_str_retain(hir_expr_type(h_iter_for)->name);
        /* pass */
        long long iter_hal = hir_expr_type(h_iter_for)->args->len;
        /* pass */
        if ((((strcmp(_tr_strz(iter_hn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(iter_hn), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if (((strcmp(_tr_strz(iter_hn), _tr_strz(_tr_str_lit("Chan"))) == 0) && (iter_hal > 0LL))) {
            /* pass */
            var_ty_for = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_for)->args, 0LL)));
        } else if ((strcmp(_tr_strz(iter_hn), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            var_ty_for = AstType_init(_tr_str_lit("char"));
        }
        /* pass */
        Sema_declare(self, var, SymbolKind_make_SVariable(), box_asttype(var_ty_for), false);
        /* pass */
        if (for_is_ref) {
            /* pass */
            __auto_type _t540 = (*iter);
            if (_t540.tag == Expr_EIdent) {
                __auto_type for_coll = _t540.data.EIdent.name;
                /* pass */
                List_TrStr_append(self->cur_func_borrowers, var);
                /* pass */
                List_TrStr_append(self->cur_func_sources, for_coll);
            } else if (1) {
                __auto_type _ = _t540;
                /* pass */
            }
        }
        /* pass */
        bool sf_outer_cap_m = self->capturing_moves;
        /* pass */
        List_TrStr* sf_outer_buf_m = self->branch_moved_buf;
        /* pass */
        bool sf_outer_cap_i = self->capturing_inits;
        /* pass */
        List_TrStr* sf_outer_buf_i = self->branch_init_buf;
        /* pass */
        self->capturing_moves = true;
        /* pass */
        self->capturing_inits = true;
        /* pass */
        self->branch_moved_buf = (void*)List_TrStr_new();
        /* pass */
        self->branch_init_buf = (void*)List_TrStr_new();
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* sf_body = Sema_lower_block(self, body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        List_TrStr* sf_loop_moved = self->branch_moved_buf;
        /* pass */
        List_TrStr* sf_loop_inited = self->branch_init_buf;
        /* pass */
        self->capturing_moves = sf_outer_cap_m;
        /* pass */
        self->branch_moved_buf = sf_outer_buf_m;
        /* pass */
        self->capturing_inits = sf_outer_cap_i;
        /* pass */
        self->branch_init_buf = sf_outer_buf_i;
        /* pass */
        long long sf_mi = 0LL;
        /* pass */
        while ((sf_mi < sf_loop_moved->len)) {
            /* pass */
            ({ TrStr _at_t541 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_unmark_moved(self, _at_t541); _tr_str_release(_at_t541); });
            /* pass */
            ({ TrStr _at_t542 = (List_TrStr_get(sf_loop_moved, sf_mi)); Sema_mark_maybe_moved(self, _at_t542); _tr_str_release(_at_t542); });
            /* pass */
            sf_mi = (sf_mi + 1LL);
        }
        /* pass */
        long long sf_ii = 0LL;
        /* pass */
        while ((sf_ii < sf_loop_inited->len)) {
            /* pass */
            ({ TrStr _at_t543 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_unmark_init(self, _at_t543); _tr_str_release(_at_t543); });
            /* pass */
            ({ TrStr _at_t544 = (List_TrStr_get(sf_loop_inited, sf_ii)); Sema_mark_maybe_init(self, _at_t544); _tr_str_release(_at_t544); });
            /* pass */
            sf_ii = (sf_ii + 1LL);
        }
        /* pass */
        Sema_finalize_scope_drops(self, sf_body);
        /* pass */
        HirStmt* hstmt = box_hirstmt(HirStmt_ctor_SFor(var, h_iter_for, sf_body));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        _tr_str_release(iter_hn);
        _tr_obj_release(sf_body, _trdrop_HirBlock);
        return hstmt;
    } else if (_t487.tag == Stmt_SForUnpack) {
        __auto_type vars = _t487.data.SForUnpack.vars;
__auto_type iter = _t487.data.SForUnpack.iter;
__auto_type body = _t487.data.SForUnpack.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        HirExpr* h_iter_fu = Sema_lower_expr(self, iter);
        /* pass */
        List_ptr* fu_tys = (void*)List_ptr_new();
        /* pass */
        long long fu_ti = 0LL;
        /* pass */
        while ((fu_ti < vars->len)) {
            /* pass */
            List_ptr_append(fu_tys, AstType_init(_tr_str_lit("int")));
            /* pass */
            fu_ti = (fu_ti + 1LL);
        }
        /* pass */
        __auto_type _t545 = (*h_iter_fu);
        if (_t545.tag == HirExpr_ECall) {
            __auto_type fu_callee = _t545.data.ECall.callee;
__auto_type fu_args = _t545.data.ECall.args;
            /* pass */
            __auto_type _t546 = (*fu_callee);
            if (_t546.tag == HirExpr_EIdent) {
                __auto_type fu_fn = _t546.data.EIdent.name;
                /* pass */
                if ((((strcmp(_tr_strz(fu_fn), _tr_strz(_tr_str_lit("enumerate"))) == 0) && (fu_args->len == 1LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    TrStr fu_col_ty_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
                    /* pass */
                    long long fu_col_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    if ((((strcmp(_tr_strz(fu_col_ty_n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(fu_col_ty_n), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (fu_col_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                } else if ((((strcmp(_tr_strz(fu_fn), _tr_strz(_tr_str_lit("zip"))) == 0) && (fu_args->len == 2LL)) && (vars->len >= 2LL))) {
                    /* pass */
                    TrStr fu_a_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->name);
                    /* pass */
                    long long fu_a_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args->len;
                    /* pass */
                    TrStr fu_b_n = _tr_str_retain(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->name);
                    /* pass */
                    long long fu_b_al = hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args->len;
                    /* pass */
                    if ((((strcmp(_tr_strz(fu_a_n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(fu_a_n), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (fu_a_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 0LL)))->args, 0LL))));
                    }
                    /* pass */
                    if ((((strcmp(_tr_strz(fu_b_n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(fu_b_n), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (fu_b_al > 0LL))) {
                        /* pass */
                        List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(((HirExpr*)List_ptr_get(fu_args, 1LL)))->args, 0LL))));
                    }
                }
            } else if (1) {
                __auto_type _ = _t546;
                /* pass */
            }
        } else if (_t545.tag == HirExpr_EMethodCall) {
            __auto_type fu_obj = _t545.data.EMethodCall.obj;
__auto_type fu_meth = _t545.data.EMethodCall.method;
            /* pass */
            if (((strcmp(_tr_strz(fu_meth), _tr_strz(_tr_str_lit("items"))) == 0) && (vars->len >= 2LL))) {
                /* pass */
                TrStr fu_dty_n = _tr_str_retain(hir_expr_type(fu_obj)->name);
                /* pass */
                long long fu_dty_al = hir_expr_type(fu_obj)->args->len;
                /* pass */
                if ((((strcmp(_tr_strz(fu_dty_n), _tr_strz(_tr_str_lit("Dict"))) == 0) || (strcmp(_tr_strz(fu_dty_n), _tr_strz(_tr_str_lit("Map"))) == 0)) && (fu_dty_al >= 2LL))) {
                    /* pass */
                    List_ptr_set(fu_tys, 0LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 0LL))));
                    /* pass */
                    List_ptr_set(fu_tys, 1LL, (*((AstType**)List_ptr_get(hir_expr_type(fu_obj)->args, 1LL))));
                }
            }
        } else if (1) {
            __auto_type _ = _t545;
            /* pass */
        }
        /* pass */
        long long vi_fu = 0LL;
        /* pass */
        while ((vi_fu < vars->len)) {
            /* pass */
            ({ TrStr _at_t547 = (List_TrStr_get(vars, vi_fu)); Sema_declare(self, _at_t547, SymbolKind_make_SVariable(), box_asttype(((AstType*)List_ptr_get(fu_tys, vi_fu))), false); _tr_str_release(_at_t547); });
            /* pass */
            vi_fu = (vi_fu + 1LL);
        }
        /* pass */
        HirBlock* hblk_fu = Sema_lower_block(self, body);
        /* pass */
        Sema_finalize_scope_drops(self, hblk_fu);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        List_ptr_free(fu_tys);
        return box_hirstmt(HirStmt_ctor_SForUnpack(vars, h_iter_fu, hblk_fu));
    } else if (_t487.tag == Stmt_SMatch) {
        __auto_type subj = _t487.data.SMatch.expr;
__auto_type arms = _t487.data.SMatch.arms;
        /* pass */
        HirExpr* hsubj = Sema_lower_expr(self, subj);
        /* pass */
        TrStr ex_ty_name = _tr_str_retain(hir_expr_type(hsubj)->name);
        /* pass */
        List_ptr* h_arms = (void*)List_ptr_new();
        /* pass */
        bool ex_has_wild = false;
        /* pass */
        List_TrStr* ex_covered = (void*)List_TrStr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, k));
            /* pass */
            __auto_type _t548 = arm->pat;
            if (_t548.tag == Pattern_PWild) {
                ex_has_wild = true;
            } else if (_t548.tag == Pattern_PBind) {
                __auto_type _ = _t548.data.PBind.name;
                ex_has_wild = true;
            } else if (_t548.tag == Pattern_PVariant) {
                __auto_type ex_vn = _t548.data.PVariant.variant;
                List_TrStr_append(ex_covered, ex_vn);
            } else if (_t548.tag == Pattern_PVariantBind) {
                __auto_type ex_vn2 = _t548.data.PVariantBind.variant;
                List_TrStr_append(ex_covered, ex_vn2);
            } else if (_t548.tag == Pattern_PVariantBindMany) {
                __auto_type ex_vn3 = _t548.data.PVariantBindMany.variant;
                List_TrStr_append(ex_covered, ex_vn3);
            } else if (_t548.tag == Pattern_POr) {
                __auto_type ex_orpats = _t548.data.POr.patterns;
                /* pass */
                long long ex_oi = 0LL;
                /* pass */
                while ((ex_oi < ex_orpats->len)) {
                    /* pass */
                    __auto_type _t549 = List_Pattern_get(ex_orpats, ex_oi);
                    if (_t549.tag == Pattern_PVariant) {
                        __auto_type ex_ovn = _t549.data.PVariant.variant;
                        List_TrStr_append(ex_covered, ex_ovn);
                    } else if (_t549.tag == Pattern_PVariantBind) {
                        __auto_type ex_bvn = _t549.data.PVariantBind.variant;
                        /* pass */
                        List_TrStr_append(ex_covered, ex_bvn);
                        /* pass */
                        ({ TrStr _at_t550 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[E-3] Payload binding '")), _tr_strz(ex_ty_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_bvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_bvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" | ...:'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t550); _tr_str_release(_at_t550); });
                    } else if (_t549.tag == Pattern_PVariantBindMany) {
                        __auto_type ex_mvn = _t549.data.PVariantBindMany.variant;
                        /* pass */
                        List_TrStr_append(ex_covered, ex_mvn);
                        /* pass */
                        ({ TrStr _at_t551 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[E-3] Payload binding '")), _tr_strz(ex_ty_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_mvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(..)' is not allowed inside an or-pattern ('|').\n      FIX: Split the alternatives into separate 'case' arms, or match the variant without binding: 'case "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_mvn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" | ...:'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t551); _tr_str_release(_at_t551); });
                    } else if (1) {
                        __auto_type _ = _t549;
                        /* pass */
                    }
                    /* pass */
                    ex_oi = (ex_oi + 1LL);
                }
            } else if (1) {
                __auto_type _ = _t548;
                /* pass */
            }
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            AstType* _subj_ty = hir_expr_type(hsubj);
            /* pass */
            Sema_declare_pattern_binds_typed(self, arm->pat, _subj_ty);
            /* pass */
            HirMatchArm* h_arm = ({ HirBlock* _aot_t552 = (Sema_lower_block(self, (*arm->body))); __auto_type _wr = (HirMatchArm_init(arm->pat, _aot_t552)); _tr_obj_release(_aot_t552, _trdrop_HirBlock); _wr; });
            /* pass */
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                h_arm->guard = Sema_lower_expr(self, arm->guard);
            }
            /* pass */
            Sema_finalize_scope_drops(self, h_arm->body);
            /* pass */
            List_ptr_append(h_arms, _tr_obj_retain(h_arm));
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            k = (k + 1LL);
            _tr_obj_release(h_arm, _trdrop_HirMatchArm);
        }
        /* pass */
        if (((!ex_has_wild) && _tr_dict_contains(self->enums, _tr_strz(ex_ty_name)))) {
            /* pass */
            EnumDef* ex_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(ex_ty_name)));
            /* pass */
            List_TrStr* ex_missing = (void*)List_TrStr_new();
            /* pass */
            long long ex_vi = 0LL;
            /* pass */
            while ((ex_vi < ex_edef->variants->len)) {
                /* pass */
                TrStr ex_vname = _tr_str_retain(((VariantDef*)List_ptr_get(ex_edef->variants, ex_vi))->name);
                /* pass */
                if ((!List_TrStr_contains(ex_covered, ex_vname))) {
                    /* pass */
                    List_TrStr_append(ex_missing, ex_vname);
                }
                /* pass */
                ex_vi = (ex_vi + 1LL);
                _tr_str_release(ex_vname);
            }
            /* pass */
            if ((ex_missing->len > 0LL)) {
                /* pass */
                TrStr ex_msg = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[E-1] Non-exhaustive match on '")), _tr_strz(ex_ty_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': missing variant"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((ex_missing->len > 1LL)) {
                    /* pass */
                    TrStr _strtmp_t553 = _tr_strx_concat(_tr_strz(ex_msg), _tr_strz(_tr_str_lit("s")));
                    _tr_str_release(ex_msg);
                    ex_msg = _strtmp_t553;
                }
                /* pass */
                TrStr _strtmp_t554 = _tr_strx_concat(_tr_strz(ex_msg), _tr_strz(_tr_str_lit(": ")));
                _tr_str_release(ex_msg);
                ex_msg = _strtmp_t554;
                /* pass */
                long long ex_mi = 0LL;
                /* pass */
                while ((ex_mi < ex_missing->len)) {
                    /* pass */
                    if ((ex_mi > 0LL)) {
                        /* pass */
                        TrStr _strtmp_t555 = _tr_strx_concat(_tr_strz(ex_msg), _tr_strz(_tr_str_lit(", ")));
                        _tr_str_release(ex_msg);
                        ex_msg = _strtmp_t555;
                    }
                    /* pass */
                    TrStr _strtmp_t556 = ({ TrStr _cr = (List_TrStr_get(ex_missing, ex_mi)); TrStr _cres = _tr_strx_concat(_tr_strz(ex_msg), _cr.data); _tr_str_release(_cr); _cres; });
                    _tr_str_release(ex_msg);
                    ex_msg = _strtmp_t556;
                    /* pass */
                    ex_mi = (ex_mi + 1LL);
                }
                /* pass */
                TrStr _strtmp_t557 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ex_msg), _tr_strz(_tr_str_lit(".\n      FIX: Add a 'case ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ex_ty_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".VariantName:' arm for each missing variant, or add 'case _:' to handle all remaining cases."))); _tr_str_release(_cl); _cres; });
                _tr_str_release(ex_msg);
                ex_msg = _strtmp_t557;
                /* pass */
                Sema_error(self, ex_msg);
            }
        }
        /* pass */
        _tr_str_release(ex_ty_name);
        List_TrStr_free(ex_covered);
        return box_hirstmt(HirStmt_ctor_SMatch(hsubj, h_arms));
    } else if (_t487.tag == Stmt_STry) {
        __auto_type try_body = _t487.data.STry.try_body;
__auto_type catches = _t487.data.STry.catches;
__auto_type finally_b = _t487.data.STry.finally_b;
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_try_body = Sema_lower_block(self, try_body);
        /* pass */
        List_ptr* h_catches = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < catches->len)) {
            /* pass */
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, k)));
            /* pass */
            HirCatchClause* hcc_val = ((HirCatchClause*)_tr_obj_alloc(sizeof(HirCatchClause)));
            /* pass */
            hcc_val->err_name = _tr_str_retain(cc->err_name);
            /* pass */
            hcc_val->err_type = AstType_init(_tr_str_lit("str"));
            /* pass */
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
                /* pass */
                hcc_val->err_type = (*cc->err_type);
            }
            /* pass */
            if ((strcmp(_tr_strz(cc->err_name), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                Sema_declare(self, cc->err_name, SymbolKind_make_SVariable(), box_asttype(hcc_val->err_type), true);
            }
            /* pass */
            HirBlock* _cltmp_t558 = Sema_lower_block(self, (*cc->body));
            _tr_obj_release(hcc_val->body, _trdrop_HirBlock);
            hcc_val->body = _cltmp_t558;
            /* pass */
            /* unsafe block */
            /* pass */
            HirCatchClause** hcc = ((HirCatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(HirCatchClause*)));
            /* pass */
            (*hcc = hcc_val);
            /* pass */
            List_ptr_append(h_catches, hcc);
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        HirBlock* h_finally_b = Sema_lower_block(self, finally_b);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_STry(h_try_body, h_catches, h_finally_b));
    } else if (_t487.tag == Stmt_SRaise) {
        __auto_type e = _t487.data.SRaise.val;
        return box_hirstmt(HirStmt_ctor_SRaise(Sema_lower_expr(self, e)));
    } else if (_t487.tag == Stmt_SAssert) {
        __auto_type cond = _t487.data.SAssert.cond;
__auto_type msg = _t487.data.SAssert.msg;
        return box_hirstmt(HirStmt_ctor_SAssert(Sema_lower_expr(self, cond), Sema_lower_expr(self, msg)));
    } else if (_t487.tag == Stmt_SDefer) {
        __auto_type inner = _t487.data.SDefer.stmt;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SDefer(Sema_lower_stmt(self, inner)));
    } else if (_t487.tag == Stmt_SWith) {
        __auto_type items = _t487.data.SWith.items;
__auto_type aliases = _t487.data.SWith.aliases;
__auto_type body = _t487.data.SWith.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* h_items = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < items->len)) {
            /* pass */
            HirExpr* h_wi = Sema_lower_expr(self, ((Expr*)List_ptr_get(items, k)));
            /* pass */
            List_ptr_append(h_items, h_wi);
            /* pass */
            if (((k < aliases->len) && (strcmp(_tr_strz(List_TrStr_get(aliases, k)), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                AstType* wi_ty = hir_expr_type(h_wi);
                /* pass */
                ({ TrStr _at_t559 = (List_TrStr_get(aliases, k)); Sema_declare(self, _at_t559, SymbolKind_make_SVariable(), box_asttype(wi_ty), true); _tr_str_release(_at_t559); });
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        HirBlock* h_with_body = Sema_lower_block(self, body);
        /* pass */
        Sema_finalize_scope_drops(self, h_with_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SWith(h_items, aliases, h_with_body));
    } else if (_t487.tag == Stmt_SAsm) {
        __auto_type code = _t487.data.SAsm.code;
__auto_type outputs = _t487.data.SAsm.outputs;
__auto_type inputs = _t487.data.SAsm.inputs;
__auto_type clobbers = _t487.data.SAsm.clobbers;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SAsm(code, outputs, inputs, clobbers));
    } else if (_t487.tag == Stmt_SSpawn) {
        __auto_type e = _t487.data.SSpawn.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t560 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[C-5] 'spawn:' used outside an async function. FIX: Declare '")), _tr_strz(self->current_func_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' as 'async def "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(...)' to use spawn inside it."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t560); _tr_str_release(_at_t560); });
        }
        /* pass */
        HirExpr* spawn_lowered = Sema_lower_expr(self, e);
        /* pass */
        Sema_check_spawn_sendable(self, spawn_lowered);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SSpawn(spawn_lowered));
    } else if (_t487.tag == Stmt_STaskGroup) {
        __auto_type body = _t487.data.STaskGroup.body;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t561 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[C-6] 'taskgroup:' used outside an async function. FIX: Declare '")), _tr_strz(self->current_func_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' as 'async def "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(...)' to use taskgroup inside it."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t561); _tr_str_release(_at_t561); });
        }
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_tg_body = Sema_lower_block(self, body);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_STaskGroup(h_tg_body));
    } else if (_t487.tag == Stmt_SChanSelect) {
        __auto_type cs_cases = _t487.data.SChanSelect.cases;
        /* pass */
        List_ptr* hcs_cases = (void*)List_ptr_new();
        /* pass */
        long long csi2 = 0LL;
        /* pass */
        while ((csi2 < cs_cases->len)) {
            /* pass */
            ChanSelectArm* arm = (*((ChanSelectArm**)List_ptr_get(cs_cases, csi2)));
            /* pass */
            HirChanSelectArm** harm = (HirChanSelectArm**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            harm = ((HirChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(HirChanSelectArm*)));
            /* pass */
            HirChanSelectArm* harm_v = ((HirChanSelectArm*)_tr_obj_alloc(sizeof(HirChanSelectArm)));
            /* pass */
            harm_v->kind = arm->kind;
            /* pass */
            harm_v->var_name = _tr_str_retain(arm->var_name);
            /* pass */
            if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->chan_expr = Sema_lower_expr(self, arm->chan_expr);
            } else {
                /* pass */
                harm_v->chan_expr = (HirExpr*)(0LL);
            }
            /* pass */
            if ((((unsigned long long)(arm->val_expr)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->val_expr = Sema_lower_expr(self, arm->val_expr);
            } else {
                /* pass */
                harm_v->val_expr = (HirExpr*)(0LL);
            }
            /* pass */
            if ((((unsigned long long)(arm->timeout_ms)) != ((unsigned long long)(0LL)))) {
                /* pass */
                harm_v->timeout_ms = Sema_lower_expr(self, arm->timeout_ms);
            } else {
                /* pass */
                harm_v->timeout_ms = (HirExpr*)(0LL);
            }
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            if (((arm->kind == 0LL) && (strcmp(_tr_strz(arm->var_name), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                AstType* recv_ty = AstType_init(_tr_str_lit("int"));
                /* pass */
                if ((((unsigned long long)(arm->chan_expr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    HirExpr* chan_hir = harm_v->chan_expr;
                    /* pass */
                    AstType* chan_ty = hir_expr_type(chan_hir);
                    /* pass */
                    if (((!_is_invalid_ptr(((unsigned long long)(chan_ty->args)))) && (chan_ty->args->len > 0LL))) {
                        /* pass */
                        recv_ty = (*((AstType**)List_ptr_get(chan_ty->args, 0LL)));
                    }
                }
                /* pass */
                Sema_declare(self, arm->var_name, SymbolKind_make_SVariable(), box_asttype(recv_ty), true);
            }
            /* pass */
            HirBlock* _cltmp_t562 = Sema_lower_block(self, arm->body);
            _tr_obj_release(harm_v->body, _trdrop_HirBlock);
            harm_v->body = _cltmp_t562;
            /* pass */
            Sema_finalize_scope_drops(self, harm_v->body);
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            /* unsafe block */
            /* pass */
            (*harm = harm_v);
            /* pass */
            List_ptr_append(hcs_cases, harm);
            /* pass */
            csi2 = (csi2 + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SChanSelect(hcs_cases));
    } else if (_t487.tag == Stmt_SGpuBlock) {
        __auto_type body = _t487.data.SGpuBlock.body;
        /* pass */
        self->block_depth = (self->block_depth + 1LL);
        /* pass */
        Sema_open_block(self);
        /* pass */
        HirBlock* h_gpu_body = Sema_lower_block(self, body);
        /* pass */
        self->block_depth = (self->block_depth - 1LL);
        /* pass */
        Sema_close_block(self);
        /* pass */
        return box_hirstmt(HirStmt_ctor_SGpuBlock(h_gpu_body));
    } else if (_t487.tag == Stmt_SBreak) {
        __auto_type bv = _t487.data.SBreak.val;
        /* pass */
        HirExpr* hbv = (HirExpr*)(0LL);
        /* pass */
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
            /* pass */
            hbv = Sema_lower_expr(self, bv);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SBreak(hbv));
    } else if (_t487.tag == Stmt_SContinue) {
        return box_hirstmt(HirStmt_make_SContinue());
    } else if (_t487.tag == Stmt_SPass) {
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t487.tag == Stmt_SLocalDecl) {
        __auto_type ldecl = _t487.data.SLocalDecl.decl;
        /* pass */
        if ((strcmp(_tr_strz(self->current_func_name), _tr_strz(_tr_str_lit("main"))) != 0)) {
            /* pass */
            Sema_error(self, _tr_str_lit("[E-2] Nested class/def/enum/interface declarations are only supported inside main().\n      FIX: Move this declaration to module (top-level) scope, or declare it inside main()."));
            /* pass */
            return box_hirstmt(HirStmt_make_SPass());
        }
        /* pass */
        Sema_register_decl(self, ldecl);
        /* pass */
        TrStr saved_ld_func_name = _tr_str_retain(self->current_func_name);
        /* pass */
        TrStr saved_ld_class_name = _tr_str_retain(self->current_class_name);
        /* pass */
        __auto_type _t563 = (*ldecl);
        if (_t563.tag == Decl_DFunction) {
            __auto_type ld_f = _t563.data.DFunction.func;
            /* pass */
            List_ptr_append(self->nested_functions, Sema_lower_func(self, ld_f));
        } else if (_t563.tag == Decl_DClass) {
            __auto_type ld_c = _t563.data.DClass.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t563.tag == Decl_DActor) {
            __auto_type ld_c = _t563.data.DActor.cls;
            /* pass */
            List_ptr_append(self->nested_classes, Sema_lower_class(self, ld_c));
        } else if (_t563.tag == Decl_DEnum) {
            __auto_type ld_e = _t563.data.DEnum.enm;
            /* pass */
            List_ptr_append(self->nested_enums, Sema_lower_enum(self, ld_e));
        } else if (_t563.tag == Decl_DInterface) {
            __auto_type ld_i = _t563.data.DInterface.iface;
            /* pass */
            List_ptr_append(self->nested_interfaces, Sema_lower_interface(self, ld_i));
        } else if (_t563.tag == Decl_DExtend) {
            __auto_type ld_target = _t563.data.DExtend.target;
__auto_type ld_methods = _t563.data.DExtend.methods;
            /* pass */
            self->current_class_name = _tr_str_retain(ld_target);
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(ld_target))) {
                /* pass */
                self->current_region_params = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(ld_target)))->region_params;
            }
            /* pass */
            long long ld_ci = 0LL;
            /* pass */
            bool ld_found = false;
            /* pass */
            while ((ld_ci < self->nested_classes->len)) {
                /* pass */
                HirClass* ld_nc = ((HirClass*)List_ptr_get(self->nested_classes, ld_ci));
                /* pass */
                if ((strcmp(_tr_strz(ld_nc->name), _tr_strz(ld_target)) == 0)) {
                    /* pass */
                    long long ld_hi = 0LL;
                    /* pass */
                    while ((ld_hi < ld_methods->len)) {
                        /* pass */
                        List_ptr_append(ld_nc->methods, Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi))));
                        /* pass */
                        ld_hi = (ld_hi + 1LL);
                    }
                    /* pass */
                    List_ptr_set(self->nested_classes, ld_ci, ld_nc);
                    /* pass */
                    ld_found = true;
                }
                /* pass */
                ld_ci = (ld_ci + 1LL);
            }
            /* pass */
            if ((!ld_found)) {
                /* pass */
                long long ld_hi2 = 0LL;
                /* pass */
                while ((ld_hi2 < ld_methods->len)) {
                    /* pass */
                    List_ptr_append(self->nested_functions, Sema_lower_func(self, ((FunctionDef*)List_ptr_get(ld_methods, ld_hi2))));
                    /* pass */
                    ld_hi2 = (ld_hi2 + 1LL);
                }
            }
        } else if (1) {
            __auto_type _ = _t563;
            /* pass */
        }
        /* pass */
        self->current_func_name = _tr_str_retain(saved_ld_func_name);
        /* pass */
        self->current_class_name = _tr_str_retain(saved_ld_class_name);
        /* pass */
        self->current_region_params = (void*)List_TrStr_new();
        /* pass */
        _tr_str_release(saved_ld_func_name);
        _tr_str_release(saved_ld_class_name);
        return box_hirstmt(HirStmt_make_SPass());
    } else if (_t487.tag == Stmt_SUnsafe) {
        __auto_type body = _t487.data.SUnsafe.body;
        /* pass */
        bool saved_unsafe = self->in_unsafe;
        /* pass */
        self->in_unsafe = true;
        /* pass */
        HirBlock* unsafe_hir = Sema_lower_block(self, body);
        /* pass */
        self->in_unsafe = saved_unsafe;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SUnsafe(unsafe_hir));
    } else if (_t487.tag == Stmt_SMultiLet) {
        __auto_type names = _t487.data.SMultiLet.names;
__auto_type is_mut = _t487.data.SMultiLet.is_mut;
__auto_type val_ptr = _t487.data.SMultiLet.val;
        /* pass */
        HirExpr* hval = Sema_lower_expr(self, val_ptr);
        /* pass */
        AstType* val_ty = hir_expr_type(hval);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < names->len)) {
            /* pass */
            AstType* nty = AstType_init(_tr_str_lit("int"));
            /* pass */
            if ((val_ty->args->len > i)) {
                /* pass */
                nty = (*((AstType**)List_ptr_get(val_ty->args, i)));
            }
            /* pass */
            ({ TrStr _at_t564 = (List_TrStr_get(names, i)); Sema_declare(self, _at_t564, SymbolKind_make_SVariable(), box_asttype(nty), is_mut); _tr_str_release(_at_t564); });
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return box_hirstmt(HirStmt_ctor_SMultiLet(names, is_mut, hval));
    } else if (_t487.tag == Stmt_SLine) {
        __auto_type n = _t487.data.SLine.n;
        /* pass */
        self->current_line = n;
        /* pass */
        return box_hirstmt(HirStmt_ctor_SLineMarker(n));
    } else if (1) {
        __auto_type _ = _t487;
        return box_hirstmt(HirStmt_make_SPass());
    }
}

__attribute__((hot)) AstType* Sema_variant_field_ty(Sema* self, TrStr type_name, TrStr variant_name, long long field_idx) {
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
        /* pass */
        EnumDef* enm = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(type_name)));
        /* pass */
        long long vi = 0LL;
        /* pass */
        while ((vi < enm->variants->len)) {
            /* pass */
            VariantDef* v = ((VariantDef*)List_ptr_get(enm->variants, vi));
            /* pass */
            if ((strcmp(_tr_strz(v->name), _tr_strz(variant_name)) == 0)) {
                /* pass */
                if ((field_idx < v->fields->len)) {
                    /* pass */
                    Param* fp = ((Param*)List_ptr_get(v->fields, field_idx));
                    /* pass */
                    if ((((unsigned long long)(fp->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        return (*fp->ty);
                    }
                }
                /* pass */
                return AstType_init(_tr_str_lit("void"));
            }
            /* pass */
            vi = (vi + 1LL);
        }
    }
    /* pass */
    return AstType_init(_tr_str_lit("void"));
}

__attribute__((hot)) void Sema_declare_pattern_binds(Sema* self, Pattern pat) {
    /* pass */
    Sema_declare_pattern_binds_typed(self, pat, AstType_init(_tr_str_lit("int")));
}

__attribute__((hot)) void Sema_declare_pattern_binds_typed(Sema* self, Pattern pat, AstType* subj_ty) {
    /* pass */
    __auto_type _t565 = pat;
    if (_t565.tag == Pattern_PBind) {
        __auto_type name = _t565.data.PBind.name;
        Sema_declare(self, name, SymbolKind_make_SVariable(), box_asttype(subj_ty), false);
    } else if (_t565.tag == Pattern_PVariantBind) {
        __auto_type type_name = _t565.data.PVariantBind.type_name;
__auto_type variant_name = _t565.data.PVariantBind.variant;
__auto_type field = _t565.data.PVariantBind.field;
        /* pass */
        AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, 0LL);
        /* pass */
        if ((((strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Option"))) == 0) && (strcmp(_tr_strz(variant_name), _tr_strz(_tr_str_lit("Some"))) == 0)) && (subj_ty->args->len > 0LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if ((((strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Result"))) == 0) && (strcmp(_tr_strz(variant_name), _tr_strz(_tr_str_lit("Ok"))) == 0)) && (subj_ty->args->len > 0LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 0LL)));
        } else if ((((strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Result"))) == 0) && (strcmp(_tr_strz(variant_name), _tr_strz(_tr_str_lit("Err"))) == 0)) && (subj_ty->args->len > 1LL))) {
            /* pass */
            fty = (*((AstType**)List_ptr_get(subj_ty->args, 1LL)));
        }
        /* pass */
        if ((strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("void"))) == 0)) {
            /* pass */
            fty = AstType_init(_tr_str_lit("int"));
        }
        /* pass */
        Sema_declare(self, field, SymbolKind_make_SVariable(), box_asttype(fty), false);
    } else if (_t565.tag == Pattern_PVariantBindMany) {
        __auto_type type_name = _t565.data.PVariantBindMany.type_name;
__auto_type variant_name = _t565.data.PVariantBindMany.variant;
__auto_type fields = _t565.data.PVariantBindMany.fields;
        /* pass */
        long long _pi = 0LL;
        /* pass */
        while ((_pi < fields->len)) {
            /* pass */
            TrStr _pf = List_TrStr_get(fields, _pi);
            /* pass */
            if ((strcmp(_tr_strz(_pf), _tr_strz(_tr_str_lit("_"))) != 0)) {
                /* pass */
                AstType* fty = Sema_variant_field_ty(self, type_name, variant_name, _pi);
                /* pass */
                if ((strcmp(_tr_strz(fty->name), _tr_strz(_tr_str_lit("void"))) == 0)) {
                    /* pass */
                    fty = AstType_init(_tr_str_lit("int"));
                }
                /* pass */
                Sema_declare(self, _pf, SymbolKind_make_SVariable(), box_asttype(fty), false);
            }
            /* pass */
            _pi = (_pi + 1LL);
            _tr_str_release(_pf);
        }
    } else if (_t565.tag == Pattern_PTuple) {
        __auto_type first = _t565.data.PTuple.first;
__auto_type second = _t565.data.PTuple.second;
        /* pass */
        Sema_declare(self, first, SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit("int"))), false);
        /* pass */
        Sema_declare(self, second, SymbolKind_make_SVariable(), box_asttype(AstType_init(_tr_str_lit("int"))), false);
    } else if (_t565.tag == Pattern_POr) {
        __auto_type pats = _t565.data.POr.patterns;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < pats->len)) {
            /* pass */
            Sema_declare_pattern_binds_typed(self, List_Pattern_get(pats, i), subj_ty);
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t565;
        /* pass */
    }
}

__attribute__((hot)) AstType* Sema_str_method_ret_ty(Sema* self, TrStr method) {
    /* pass */
    if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("split"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("split_to_vec"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("split_once"))) == 0))) {
        /* pass */
        return AstType_init_generic(_tr_str_lit("Vec"), box_asttype(AstType_init(_tr_str_lit("str"))));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim_left"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("trim_right"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_upper"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_lower"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("upper"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lower"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lstrip"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("rstrip"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("center"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pad_left"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pad_right"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("zfill"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("format"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("chars"))) == 0)) {
        /* pass */
        return AstType_init_generic(_tr_str_lit("Vec"), box_asttype(AstType_init(_tr_str_lit("str"))));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("capitalize"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("title"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reverse"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("repeat"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("replace"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("replace_first"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("slice"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip_prefix"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("strip_suffix"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove_char"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("join"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("str"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("index_of"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("last_index_of"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("count"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("char_at"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_int"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_int"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_i64"))) == 0))) {
        /* pass */
        return AstType_init(_tr_str_lit("int"));
    }
    /* pass */
    if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_float"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_f64"))) == 0))) {
        /* pass */
        return AstType_init(_tr_str_lit("float"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("starts_with"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ends_with"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_bool"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains_char"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("eq"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_digit"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_alpha"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_alnum"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_space"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_upper"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_lower"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_bool"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("bool"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("parse_float"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("float"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lines"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("Vec"));
    }
    /* pass */
    if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("words"))) == 0)) {
        /* pass */
        return AstType_init(_tr_str_lit("Vec"));
    }
    /* pass */
    return AstType_init(_tr_str_lit("void"));
}

__attribute__((hot)) void Sema_collect_block_refs(Sema* self, HirBlock* b, List_TrStr* out) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Sema_collect_stmt_refs(self, ((HirStmt*)List_ptr_get(b->stmts, i)), out);
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Sema_collect_stmt_refs(Sema* self, HirStmt* s, List_TrStr* out) {
    /* pass */
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t566 = (*s);
    if (_t566.tag == HirStmt_SExpr) {
        __auto_type e = _t566.data.SExpr.expr;
        Sema_collect_idents(self, e, out);
    } else if (_t566.tag == HirStmt_SLet) {
        __auto_type v = _t566.data.SLet.val;
        Sema_collect_idents(self, v, out);
    } else if (_t566.tag == HirStmt_SAssign) {
        __auto_type t = _t566.data.SAssign.target;
__auto_type v = _t566.data.SAssign.val;
        /* pass */
        Sema_collect_idents(self, t, out);
        /* pass */
        Sema_collect_idents(self, v, out);
    } else if (_t566.tag == HirStmt_SReturn) {
        __auto_type v = _t566.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema_collect_idents(self, v, out);
        }
    } else if (_t566.tag == HirStmt_SBreak) {
        __auto_type v = _t566.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(v)) != ((unsigned long long)(0LL)))) {
            /* pass */
            Sema_collect_idents(self, v, out);
        }
    } else if (_t566.tag == HirStmt_SRaise) {
        __auto_type v = _t566.data.SRaise.val;
        Sema_collect_idents(self, v, out);
    } else if (_t566.tag == HirStmt_SIf) {
        __auto_type c = _t566.data.SIf.cond;
__auto_type tb = _t566.data.SIf.then_b;
__auto_type eb = _t566.data.SIf.else_b;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_block_refs(self, tb, out);
        /* pass */
        Sema_collect_block_refs(self, eb, out);
    } else if (_t566.tag == HirStmt_SWhile) {
        __auto_type c = _t566.data.SWhile.cond;
__auto_type b2 = _t566.data.SWhile.body;
        /* pass */
        Sema_collect_idents(self, c, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t566.tag == HirStmt_SFor) {
        __auto_type it = _t566.data.SFor.iter;
__auto_type b2 = _t566.data.SFor.body;
        /* pass */
        Sema_collect_idents(self, it, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t566.tag == HirStmt_SForUnpack) {
        __auto_type it = _t566.data.SForUnpack.iter;
__auto_type b2 = _t566.data.SForUnpack.body;
        /* pass */
        Sema_collect_idents(self, it, out);
        /* pass */
        Sema_collect_block_refs(self, b2, out);
    } else if (_t566.tag == HirStmt_SMatch) {
        __auto_type subj = _t566.data.SMatch.expr;
__auto_type arms = _t566.data.SMatch.arms;
        /* pass */
        Sema_collect_idents(self, subj, out);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            Sema_collect_block_refs(self, ((HirMatchArm*)List_ptr_get(arms, i))->body, out);
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t566.tag == HirStmt_SUnsafe) {
        __auto_type b2 = _t566.data.SUnsafe.body;
        Sema_collect_block_refs(self, b2, out);
    } else if (1) {
        __auto_type _ = _t566;
        /* pass */
    }
}

__attribute__((hot)) HirExpr* Sema_lower_do_value(Sema* self, Block* do_body) {
    /* pass */
    Sema_enter_scope(self);
    /* pass */
    HirBlock* do_hb = HirBlock_init();
    /* pass */
    long long do_n = do_body->stmts->len;
    /* pass */
    long long do_i = 0LL;
    /* pass */
    HirExpr* do_val = (HirExpr*)(0LL);
    /* pass */
    AstType* do_ty = AstType_init(_tr_str_lit("void"));
    /* pass */
    while ((do_i < do_n)) {
        /* pass */
        Stmt* do_s = ((Stmt*)List_ptr_get(do_body->stmts, do_i));
        /* pass */
        if ((do_i == (do_n - 1LL))) {
            /* pass */
            __auto_type _t567 = (*do_s);
            if (_t567.tag == Stmt_SExpr) {
                __auto_type de = _t567.data.SExpr.expr;
                /* pass */
                do_val = Sema_lower_expr(self, de);
                /* pass */
                do_ty = hir_expr_type(do_val);
            } else if (1) {
                __auto_type _ = _t567;
                /* pass */
                HirStmt* do_hs = Sema_lower_stmt(self, do_s);
                /* pass */
                if ((((unsigned long long)(do_hs)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    Sema_apply_escape_marks(self, do_hs);
                    /* pass */
                    HirBlock_push(do_hb, do_hs);
                }
            }
        } else {
            /* pass */
            HirStmt* do_hs2 = Sema_lower_stmt(self, do_s);
            /* pass */
            if ((((unsigned long long)(do_hs2)) != ((unsigned long long)(0LL)))) {
                /* pass */
                Sema_apply_escape_marks(self, do_hs2);
                /* pass */
                HirBlock_push(do_hb, do_hs2);
            }
        }
        /* pass */
        do_i = (do_i + 1LL);
    }
    /* pass */
    List_TrStr* do_excl = (void*)List_TrStr_new();
    /* pass */
    if (((((unsigned long long)(do_val)) != ((unsigned long long)(0LL))) && (strcmp(_tr_strz(do_ty->name), _tr_strz(_tr_str_lit("str"))) != 0))) {
        /* pass */
        Sema_collect_idents(self, do_val, do_excl);
    }
    /* pass */
    Sema_append_drops_from_excl_multi(self, do_hb, (self->scopes->len - 1LL), do_excl);
    /* pass */
    if ((((unsigned long long)(do_val)) != ((unsigned long long)(0LL)))) {
        /* pass */
        HirBlock_push(do_hb, box_hirstmt(HirStmt_ctor_SExpr(do_val)));
    }
    /* pass */
    Sema_exit_scope(self);
    /* pass */
    return box_hirexpr(HirExpr_ctor_EDo(do_hb, do_ty));
}

__attribute__((hot)) AstType* Sema_infer_break_type(Sema* self, HirBlock* hb) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hb->stmts->len)) {
        /* pass */
        AstType* t = Sema_infer_break_type_stmt(self, ((HirStmt*)List_ptr_get(hb->stmts, i)));
        /* pass */
        if ((strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
            /* pass */
            return t;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return AstType_init(_tr_str_lit("void"));
}

__attribute__((hot)) AstType* Sema_infer_break_type_stmt(Sema* self, HirStmt* s) {
    /* pass */
    __auto_type _t568 = (*s);
    if (_t568.tag == HirStmt_SBreak) {
        __auto_type bv = _t568.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(bv)) != ((unsigned long long)(0LL)))) {
            /* pass */
            return hir_expr_type(bv);
        }
        /* pass */
        return AstType_init(_tr_str_lit("void"));
    } else if (_t568.tag == HirStmt_SIf) {
        __auto_type then_b = _t568.data.SIf.then_b;
__auto_type else_b = _t568.data.SIf.else_b;
        /* pass */
        AstType* t = Sema_infer_break_type(self, then_b);
        /* pass */
        if ((strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
            /* pass */
            return t;
        }
        /* pass */
        return Sema_infer_break_type(self, else_b);
    } else if (_t568.tag == HirStmt_SMatch) {
        __auto_type m_arms = _t568.data.SMatch.arms;
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < m_arms->len)) {
            /* pass */
            AstType* t2 = Sema_infer_break_type(self, ((HirMatchArm*)List_ptr_get(m_arms, ai))->body);
            /* pass */
            if ((strcmp(_tr_strz(t2->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
                /* pass */
                return t2;
            }
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        return AstType_init(_tr_str_lit("void"));
    } else if (_t568.tag == HirStmt_STry) {
        __auto_type tb = _t568.data.STry.try_body;
__auto_type fb = _t568.data.STry.finally_b;
        /* pass */
        AstType* t3 = Sema_infer_break_type(self, tb);
        /* pass */
        if ((strcmp(_tr_strz(t3->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
            /* pass */
            return t3;
        }
        /* pass */
        return Sema_infer_break_type(self, fb);
    } else if (1) {
        __auto_type _ = _t568;
        return AstType_init(_tr_str_lit("void"));
    }
}

__attribute__((hot)) void Sema_check_no_heap(Sema* self, TrStr what, TrStr fix) {
    /* pass */
    if (self->no_heap) {
        /* pass */
        ({ TrStr _at_t569 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[H-1] ")), _tr_strz(what))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" allocates on the heap, which --no-heap forbids (bare-metal zero-heap build).\n      FIX: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fix)); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t569); _tr_str_release(_at_t569); });
    }
}

__attribute__((hot)) HirExpr* Sema_lower_expr(Sema* self, Expr* e_ptr) {
    /* pass */
    if ((((unsigned long long)(e_ptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (HirExpr*)(0LL);
    }
    /* pass */
    __auto_type e = (*e_ptr);
    /* pass */
    __auto_type _t570 = e;
    if (_t570.tag == Expr_ELitInt) {
        __auto_type v = _t570.data.ELitInt.val;
        return box_hirexpr(HirExpr_ctor_ELitInt(v, AstType_init(_tr_str_lit("int"))));
    } else if (_t570.tag == Expr_ELitFloat) {
        __auto_type v = _t570.data.ELitFloat.val;
        return box_hirexpr(HirExpr_ctor_ELitFloat(v, AstType_init(_tr_str_lit("float"))));
    } else if (_t570.tag == Expr_ELitStr) {
        __auto_type v = _t570.data.ELitStr.val;
        return box_hirexpr(HirExpr_ctor_ELitStr(v, AstType_init(_tr_str_lit("str"))));
    } else if (_t570.tag == Expr_ERawStr) {
        __auto_type v = _t570.data.ERawStr.val;
        return box_hirexpr(HirExpr_ctor_ERawStr(v, AstType_init(_tr_str_lit("str"))));
    } else if (_t570.tag == Expr_ELitBytes) {
        __auto_type v = _t570.data.ELitBytes.val;
        return box_hirexpr(HirExpr_ctor_ELitBytes(v, AstType_init(_tr_str_lit("Bytes"))));
    } else if (_t570.tag == Expr_ELitBool) {
        __auto_type v = _t570.data.ELitBool.val;
        return box_hirexpr(HirExpr_ctor_ELitBool(v, AstType_init(_tr_str_lit("bool"))));
    } else if (_t570.tag == Expr_ELitChar) {
        __auto_type v = _t570.data.ELitChar.val;
        return box_hirexpr(HirExpr_ctor_ELitChar(v, AstType_init(_tr_str_lit("char"))));
    } else if (_t570.tag == Expr_ELitNone) {
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit("None"))));
    } else if (_t570.tag == Expr_EIdent) {
        __auto_type name = _t570.data.EIdent.name;
        /* pass */
        Symbol* sym = Sema_resolve(self, name);
        /* pass */
        AstType* ty = (*sym->ty);
        /* pass */
        if ((((((strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) != 0)) && (!self->in_assign_target)) && (!self->in_recv_pos)) && (!Sema_is_known_name(self, name)))) {
            /* pass */
            ({ TrStr _at_t571 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[N-3] name '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not defined.\n      FIX: check the spelling, declare it with 'mut "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = ...', or import it before use."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t571); _tr_str_release(_at_t571); });
        }
        /* pass */
        if ((_tr_dict_contains(self->fn_sigs, _tr_strz(name)) && (sym->kind.tag == SymbolKind_make_SFunction().tag))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_EIdent(name, ((AstType*)(uintptr_t)_tr_dict_get(self->fn_sigs, _tr_strz(name))), false));
        }
        /* pass */
        if ((sym->is_freed && (strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            ({ TrStr _at_t572 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-6] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' was freed by 'dealloc()' and can no longer be used.\n      FIX: Remove all uses of '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' after 'dealloc()', or restructure so the pointer is freed only when no longer needed."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t572); _tr_str_release(_at_t572); });
        } else if (((((sym->is_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (!sym->is_shared))) {
            /* pass */
            ({ TrStr _at_t573 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-1] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' was moved and cannot be used again.\n      FIX: Use the variable that now owns it, or call .clone() to copy before moving."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t573); _tr_str_release(_at_t573); });
        } else if (((((sym->is_maybe_moved && (!Sema_is_primitive(self, ty))) && (!Sema_is_copy_class(self, ty->name))) && (strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (!sym->is_shared))) {
            /* pass */
            ({ TrStr _at_t574 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-5] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' may have been moved on some code paths, making this use unsafe.\n      FIX: Ensure '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not moved before this point on any branch, or restructure so the use is inside the branch where it's still valid."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t574); _tr_str_release(_at_t574); });
        }
        /* pass */
        if ((((((!sym->is_init) && (!sym->is_maybe_init)) && (strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            ({ TrStr _at_t575 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-1] Variable '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is used before being assigned a value.\n      FIX: Assign a value before use, e.g. 'mut "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = <default>'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t575); _tr_str_release(_at_t575); });
        } else if (((((sym->is_maybe_init && (!sym->is_init)) && (strcmp(_tr_strz(sym->name), _tr_strz(_tr_str_lit(""))) != 0)) && (sym->kind.tag == SymbolKind_make_SVariable().tag)) && (!self->in_assign_target))) {
            /* pass */
            ({ TrStr _at_t576 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[I-2] '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not initialized on all code paths before this use.\n      FIX: Initialize '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' before the if/loop, or ensure every branch assigns a value."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t576); _tr_str_release(_at_t576); });
        }
        /* pass */
        bool is_move = false;
        /* pass */
        if ((_tr_dict_contains(self->assign_froms, _tr_strz(name)) && (!Sema_is_primitive(self, ty)))) {
            /* pass */
            is_move = true;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIdent(name, ty, is_move));
    } else if (_t570.tag == Expr_EBinOp) {
        __auto_type op = _t570.data.EBinOp.op;
__auto_type left = _t570.data.EBinOp.left;
__auto_type right = _t570.data.EBinOp.right;
        /* pass */
        HirExpr* hleft = Sema_lower_expr(self, left);
        /* pass */
        HirExpr* hright = Sema_lower_expr(self, right);
        /* pass */
        if ((self->no_heap && ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)))) {
            /* pass */
            TrStr _nh_l = _tr_str_retain(hir_expr_type(hleft)->name);
            /* pass */
            TrStr _nh_r = _tr_str_retain(hir_expr_type(hright)->name);
            /* pass */
            if (((((strcmp(_tr_strz(_nh_l), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(_nh_l), _tr_strz(_tr_str_lit("String"))) == 0)) || (strcmp(_tr_strz(_nh_r), _tr_strz(_tr_str_lit("str"))) == 0)) || (strcmp(_tr_strz(_nh_r), _tr_strz(_tr_str_lit("String"))) == 0))) {
                /* pass */
                TrStr _nh_what = _tr_str_lit("string concatenation (+)");
                /* pass */
                if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t577 = _tr_str_lit("string repetition (*)");
                    _tr_str_release(_nh_what);
                    _nh_what = _strtmp_t577;
                }
                /* pass */
                Sema_check_no_heap(self, _nh_what, _tr_str_lit("keep string literals (they live in rodata) or use a StrView / fixed byte buffer; do not build new strings"));
            }
        }
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0))) {
            /* pass */
            TrStr lref = Sema_type_ref_name(self, left);
            /* pass */
            TrStr rref = Sema_type_ref_name(self, right);
            /* pass */
            if (((strcmp(_tr_strz(lref), _tr_strz(_tr_str_lit(""))) != 0) || (strcmp(_tr_strz(rref), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                TrStr lname = _tr_str_retain(lref);
                /* pass */
                if ((strcmp(_tr_strz(lname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t578 = _tr_str_retain(hir_expr_type(hleft)->name);
                    _tr_str_release(lname);
                    lname = _strtmp_t578;
                }
                /* pass */
                TrStr rname = _tr_str_retain(rref);
                /* pass */
                if ((strcmp(_tr_strz(rname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t579 = _tr_str_retain(hir_expr_type(hright)->name);
                    _tr_str_release(rname);
                    rname = _strtmp_t579;
                }
                /* pass */
                _tr_str_release(lref);
                _tr_str_release(rref);
                return box_hirexpr(HirExpr_ctor_EBinOp(op, box_hirexpr(HirExpr_ctor_ELitStr(lname, AstType_init(_tr_str_lit("str")))), box_hirexpr(HirExpr_ctor_ELitStr(rname, AstType_init(_tr_str_lit("str")))), AstType_init(_tr_str_lit("bool"))));
            }
        }
        /* pass */
        AstType* bin_ty = hir_expr_type(hleft);
        /* pass */
        if ((strcmp(_tr_strz(bin_ty->name), _tr_strz(_tr_str_lit("void"))) == 0)) {
            /* pass */
            bin_ty = hir_expr_type(hright);
        }
        /* pass */
        if (((((((((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("or"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&&"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("||"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("not in"))) == 0))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit("bool"));
        } else if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0) && ((((strcmp(_tr_strz(hir_expr_type(hleft)->name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(hir_expr_type(hleft)->name), _tr_strz(_tr_str_lit("String"))) == 0)) || (strcmp(_tr_strz(hir_expr_type(hright)->name), _tr_strz(_tr_str_lit("str"))) == 0)) || (strcmp(_tr_strz(hir_expr_type(hright)->name), _tr_strz(_tr_str_lit("String"))) == 0)))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit("str"));
        } else if ((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) && (_binop_is_float_name(hir_expr_type(hleft)->name) || _binop_is_float_name(hir_expr_type(hright)->name)))) {
            /* pass */
            bin_ty = AstType_init(_tr_str_lit("float"));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EBinOp(op, hleft, hright, bin_ty));
    } else if (_t570.tag == Expr_EUnaryOp) {
        __auto_type op = _t570.data.EUnaryOp.op;
__auto_type expr = _t570.data.EUnaryOp.expr;
        /* pass */
        HirExpr* hexpr_inner = Sema_lower_expr(self, expr);
        /* pass */
        AstType* inner_ty = hir_expr_type(hexpr_inner);
        /* pass */
        AstType* un_ty = inner_ty;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            AstType* addr_ty = AstType_init(_tr_str_lit("Pointer"));
            /* pass */
            List_ptr_append(addr_ty->args, box_asttype(inner_ty));
            /* pass */
            un_ty = addr_ty;
        } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(inner_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (inner_ty->args->len > 0LL))) {
                /* pass */
                un_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EUnaryOp(op, hexpr_inner, un_ty));
    } else if (_t570.tag == Expr_ECall) {
        __auto_type callee = _t570.data.ECall.callee;
__auto_type args = _t570.data.ECall.args;
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t580 = (*callee);
            if (_t580.tag == Expr_EIdent) {
                __auto_type ato_n = _t580.data.EIdent.name;
                /* pass */
                if ((self->no_heap && Sema__is_rc_class(self, ato_n))) {
                    /* pass */
                    ({ TrStr _at_t581 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("constructing heap class '")), _tr_strz(ato_n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; })); Sema_check_no_heap(self, _at_t581, _tr_str_lit("mark the class '@value_type' (stack; all-scalar fields), or place the instance in caller-owned memory via a Pointer[T]")); _tr_str_release(_at_t581); });
                }
                /* pass */
                if ((strcmp(_tr_strz(ato_n), _tr_strz(_tr_str_lit("await_timeout"))) == 0)) {
                    /* pass */
                    if ((!self->in_async_fn)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit("[C-4] 'await_timeout' used outside an async function."));
                    }
                    /* pass */
                    if ((args->len < 2LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit("await_timeout requires 2 arguments: await_timeout(expr, timeout_ms)"));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitInt(0LL, AstType_init(_tr_str_lit("int"))));
                    }
                    /* pass */
                    HirExpr* ato_inner = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    HirExpr* ato_ms = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 1LL)));
                    /* pass */
                    AstType* ato_ty = hir_expr_type(ato_inner);
                    /* pass */
                    return box_hirexpr(HirExpr_ctor_EAwaitTimeout(ato_inner, ato_ms, ato_ty));
                }
                /* pass */
                if ((strcmp(_tr_strz(ato_n), _tr_strz(_tr_str_lit("instanceOf"))) == 0)) {
                    /* pass */
                    if ((args->len < 2LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit("instanceOf requires 2 arguments: instanceOf(obj, T)"));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitBool(false, AstType_init(_tr_str_lit("bool"))));
                    }
                    /* pass */
                    HirExpr* io_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                    /* pass */
                    TrStr io_obj_ty_n = _tr_str_retain(hir_expr_type(io_obj)->name);
                    /* pass */
                    TrStr io_target_n = _tr_str_lit("");
                    /* pass */
                    __auto_type _t582 = (*((Expr*)List_ptr_get(args, 1LL)));
                    if (_t582.tag == Expr_EIdent) {
                        __auto_type io_tn = _t582.data.EIdent.name;
                        TrStr _strtmp_t583 = _tr_str_retain(io_tn);
                        _tr_str_release(io_target_n);
                        io_target_n = _strtmp_t583;
                    } else if (_t582.tag == Expr_EIndex) {
                        __auto_type io_base = _t582.data.EIndex.obj;
                        /* pass */
                        __auto_type _t584 = (*io_base);
                        if (_t584.tag == Expr_EIdent) {
                            __auto_type io_tn2 = _t584.data.EIdent.name;
                            TrStr _strtmp_t585 = _tr_str_retain(io_tn2);
                            _tr_str_release(io_target_n);
                            io_target_n = _strtmp_t585;
                        } else if (1) {
                            __auto_type _ = _t584;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t582;
                        /* pass */
                    }
                    /* pass */
                    return box_hirexpr(HirExpr_ctor_ELitBool((strcmp(_tr_strz(io_obj_ty_n), _tr_strz(io_target_n)) == 0), AstType_init(_tr_str_lit("bool"))));
                }
                /* pass */
                if ((strcmp(_tr_strz(ato_n), _tr_strz(_tr_str_lit("inspect"))) == 0)) {
                    /* pass */
                    if ((args->len < 1LL)) {
                        /* pass */
                        Sema_error(self, _tr_str_lit("inspect requires 1 argument: inspect(T)"));
                        /* pass */
                        return box_hirexpr(HirExpr_ctor_ELitStr(_tr_str_lit(""), AstType_init(_tr_str_lit("str"))));
                    }
                    /* pass */
                    TrStr isp_target_n = _tr_str_lit("");
                    /* pass */
                    __auto_type _t586 = (*((Expr*)List_ptr_get(args, 0LL)));
                    if (_t586.tag == Expr_EIdent) {
                        __auto_type isp_tn = _t586.data.EIdent.name;
                        TrStr _strtmp_t587 = _tr_str_retain(isp_tn);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t587;
                    } else if (_t586.tag == Expr_EIndex) {
                        __auto_type isp_base = _t586.data.EIndex.obj;
                        /* pass */
                        __auto_type _t588 = (*isp_base);
                        if (_t588.tag == Expr_EIdent) {
                            __auto_type isp_tn2 = _t588.data.EIdent.name;
                            TrStr _strtmp_t589 = _tr_str_retain(isp_tn2);
                            _tr_str_release(isp_target_n);
                            isp_target_n = _strtmp_t589;
                        } else if (1) {
                            __auto_type _ = _t588;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t586;
                        /* pass */
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(isp_target_n), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        HirExpr* isp_obj = Sema_lower_expr(self, ((Expr*)List_ptr_get(args, 0LL)));
                        /* pass */
                        TrStr _strtmp_t590 = _tr_str_retain(hir_expr_type(isp_obj)->name);
                        _tr_str_release(isp_target_n);
                        isp_target_n = _strtmp_t590;
                    }
                    /* pass */
                    return ({ TrStr _at_t591 = (Sema_build_inspect_str(self, isp_target_n)); __auto_type _wr = (box_hirexpr(HirExpr_ctor_ELitStr(_at_t591, AstType_init(_tr_str_lit("str"))))); _tr_str_release(_at_t591); _wr; });
                }
            } else if (1) {
                __auto_type _ = _t580;
                /* pass */
            }
        }
        /* pass */
        List_TrStr* p23_borrow_names = (void*)List_TrStr_new();
        /* pass */
        TrMap* p23_seen = _tr_dict_new(4LL);
        /* pass */
        long long p23_k = 0LL;
        /* pass */
        while ((p23_k < args->len)) {
            /* pass */
            Expr* p23_arg = ((Expr*)List_ptr_get(args, p23_k));
            /* pass */
            TrStr p23_nm = _tr_str_lit("");
            /* pass */
            if ((((unsigned long long)(p23_arg)) != ((unsigned long long)(0LL)))) {
                /* pass */
                __auto_type _t592 = (*p23_arg);
                if (_t592.tag == Expr_EIdent) {
                    __auto_type p23_n = _t592.data.EIdent.name;
                    TrStr _strtmp_t593 = _tr_str_retain(p23_n);
                    _tr_str_release(p23_nm);
                    p23_nm = _strtmp_t593;
                } else if (1) {
                    __auto_type _ = _t592;
                    /* pass */
                }
            }
            /* pass */
            if ((strcmp(_tr_strz(p23_nm), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                Symbol* p23_sym = Sema_resolve(self, p23_nm);
                /* pass */
                if (((!Sema_is_primitive(self, (*p23_sym->ty))) && (strcmp(_tr_strz(p23_sym->name), _tr_strz(_tr_str_lit(""))) != 0))) {
                    /* pass */
                    Sema_mark_borrow(self, p23_nm);
                    /* pass */
                    List_TrStr_append(p23_borrow_names, p23_nm);
                    /* pass */
                    if (_tr_dict_contains(p23_seen, _tr_strz(p23_nm))) {
                        /* pass */
                        ({ TrStr _at_t594 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-3] '")), _tr_strz(p23_nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' appears twice in the same call, creating aliased mutable access.\n      FIX: Clone one of the arguments: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(p23_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".clone()"))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t594); _tr_str_release(_at_t594); });
                    }
                    /* pass */
                    _tr_dict_set(p23_seen, _tr_strz(p23_nm), true);
                }
            }
            /* pass */
            p23_k = (p23_k + 1LL);
            _tr_str_release(p23_nm);
        }
        /* pass */
        List_ptr* hl = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < args->len)) {
            /* pass */
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        long long p23_ui = 0LL;
        /* pass */
        while ((p23_ui < p23_borrow_names->len)) {
            /* pass */
            ({ TrStr _at_t595 = (List_TrStr_get(p23_borrow_names, p23_ui)); Sema_unmark_borrow(self, _at_t595); _tr_str_release(_at_t595); });
            /* pass */
            p23_ui = (p23_ui + 1LL);
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t596 = (*callee);
            if (_t596.tag == Expr_EIdent) {
                __auto_type vfn_n = _t596.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->variadic_fns, _tr_strz(vfn_n))) {
                    /* pass */
                    long long vfixed = _tr_str_to_int(_tr_strz(_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->variadic_fns, _tr_strz(vfn_n))))));
                    /* pass */
                    AstType* velem_ty = AstType_init(_tr_str_lit("int"));
                    /* pass */
                    if (_tr_dict_contains(self->variadic_elem_ty, _tr_strz(vfn_n))) {
                        /* pass */
                        velem_ty = (*((AstType**)(uintptr_t)_tr_dict_get(self->variadic_elem_ty, _tr_strz(vfn_n))));
                    }
                    /* pass */
                    if ((hl->len >= vfixed)) {
                        /* pass */
                        List_ptr* vargs = (void*)List_ptr_new();
                        /* pass */
                        long long vi = vfixed;
                        /* pass */
                        while ((vi < hl->len)) {
                            /* pass */
                            List_ptr_append(vargs, ((HirExpr*)List_ptr_get(hl, vi)));
                            /* pass */
                            vi = (vi + 1LL);
                        }
                        /* pass */
                        AstType* vlist_ty = AstType_init(_tr_str_lit("List"));
                        /* pass */
                        List_ptr_append(vlist_ty->args, box_asttype(velem_ty));
                        /* pass */
                        List_ptr* vnew_hl = (void*)List_ptr_new();
                        /* pass */
                        long long vk = 0LL;
                        /* pass */
                        while ((vk < vfixed)) {
                            /* pass */
                            List_ptr_append(vnew_hl, ((HirExpr*)List_ptr_get(hl, vk)));
                            /* pass */
                            vk = (vk + 1LL);
                        }
                        /* pass */
                        List_ptr_append(vnew_hl, box_hirexpr(HirExpr_ctor_EList(vargs, vlist_ty)));
                        /* pass */
                        hl = vnew_hl;
                    }
                }
            } else if (1) {
                __auto_type _ = _t596;
                /* pass */
            }
        }
        /* pass */
        HirExpr* hcallee = Sema_lower_expr(self, callee);
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if ((((unsigned long long)(callee)) == ((unsigned long long)(0LL)))) {
            /* pass */
            List_TrStr_free(p23_borrow_names);
            Dict_free(p23_seen);
            return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
        }
        /* pass */
        __auto_type _t597 = (*callee);
        if (_t597.tag == Expr_EIdent) {
            __auto_type n = _t597.data.EIdent.name;
            /* pass */
            Sema_check_call_bounds(self, n, hl);
            /* pass */
            if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("main"))) == 0)) {
                /* pass */
                Sema_error(self, _tr_str_lit("[E-1] Explicit call to 'main()' is forbidden. The compiler automatically invokes main() as the program entry point. Remove the 'main()' call from your source."));
                /* pass */
                List_TrStr_free(p23_borrow_names);
                Dict_free(p23_seen);
                return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
            }
            /* pass */
            if ((((((((_tr_dict_contains(self->classes, _tr_strz(n)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringObj"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Pointer"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (_tr_dict_contains(self->enums, _tr_strz(n))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (((strcmp(_tr_strz(Sema_resolve(self, n)->name), _tr_strz(_tr_str_lit(""))) == 0) && Sema__is_type_param_in_scope(self, n))) {
                /* pass */
                ret_ty = AstType_init(n);
            } else if (((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("abs"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("min"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("max"))) == 0)) && _hl_has_float(hl))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("float"));
            } else if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("c_callback"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Pointer"));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
            } else if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("alloc"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("dealloc"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Pointer"));
                /* pass */
                if ((!self->in_unsafe)) {
                    /* pass */
                    ({ TrStr _at_t598 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[U-1] '")), _tr_strz(n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(n)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("[T](n)"))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t598); _tr_str_release(_at_t598); });
                }
            } else {
                /* pass */
                Symbol* _fsym = Sema_resolve(self, n);
                /* pass */
                ret_ty = (*_fsym->ty);
                /* pass */
                if (((strcmp(_tr_strz(ret_ty->name), _tr_strz(_tr_str_lit("def"))) == 0) && (ret_ty->args->len > 0LL))) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(ret_ty->args, (ret_ty->args->len - 1LL))));
                }
                /* pass */
                if (_tr_dict_contains(self->fn_defs, _tr_strz(n))) {
                    /* pass */
                    FunctionDef* _gfd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(n)));
                    /* pass */
                    if (((_gfd->generics->len > 0LL) && List_TrStr_contains(_gfd->generics, ret_ty->name))) {
                        /* pass */
                        long long _gsk = 0LL;
                        /* pass */
                        if (((_gfd->params->len > 0LL) && (strcmp(_tr_strz(((Param*)List_ptr_get(_gfd->params, 0LL))->name), _tr_strz(_tr_str_lit("self"))) == 0))) {
                            /* pass */
                            _gsk = 1LL;
                        }
                        /* pass */
                        long long _gpi = 0LL;
                        /* pass */
                        while ((_gpi < _gfd->params->len)) {
                            /* pass */
                            AstType** _gpty = ((Param*)List_ptr_get(_gfd->params, _gpi))->ty;
                            /* pass */
                            if (((((unsigned long long)(_gpty)) != ((unsigned long long)(0LL))) && (strcmp(_tr_strz((*_gpty)->name), _tr_strz(ret_ty->name)) == 0))) {
                                /* pass */
                                long long _gaidx = (_gpi - _gsk);
                                /* pass */
                                if (((_gaidx >= 0LL) && (_gaidx < hl->len))) {
                                    /* pass */
                                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _gaidx)));
                                    /* pass */
                                    break;
                                }
                            }
                            /* pass */
                            _gpi = (_gpi + 1LL);
                        }
                    }
                }
            }
        } else if (_t597.tag == Expr_EIndex) {
            __auto_type base = _t597.data.EIndex.obj;
__auto_type idx = _t597.data.EIndex._tr_v_index;
            /* pass */
            __auto_type _t599 = (*base);
            if (_t599.tag == Expr_EIdent) {
                __auto_type gn = _t599.data.EIdent.name;
                /* pass */
                if (((strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("alloc"))) == 0) || (strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("dealloc"))) == 0))) {
                    /* pass */
                    __auto_type _t600 = (*idx);
                    if (_t600.tag == Expr_EIdent) {
                        __auto_type tn = _t600.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(_tr_str_lit("Pointer"));
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t600;
                        /* pass */
                        ret_ty = AstType_init(_tr_str_lit("Pointer"));
                    }
                    /* pass */
                    if ((!self->in_unsafe)) {
                        /* pass */
                        ({ TrStr _at_t601 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[U-1] 'alloc'/'dealloc' used outside an 'unsafe:' block.\n      FIX: Wrap raw memory operations in 'unsafe:' to signal manual memory management, e.g.\n          unsafe:\n              p = ")), _tr_strz(gn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("[T](n)"))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t601); _tr_str_release(_at_t601); });
                    }
                } else if ((((((strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("Pointer"))) == 0) || (strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(gn), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
                    /* pass */
                    __auto_type _t602 = (*idx);
                    if (_t602.tag == Expr_EIdent) {
                        __auto_type tn = _t602.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                    } else if (1) {
                        __auto_type _ = _t602;
                        /* pass */
                        ret_ty = AstType_init(gn);
                    }
                } else if (_tr_dict_contains(self->classes, _tr_strz(gn))) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                    /* pass */
                    __auto_type _t603 = (*idx);
                    if (_t603.tag == Expr_EIdent) {
                        __auto_type targ_ca = _t603.data.EIdent.name;
                        List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_ca)));
                    } else if (1) {
                        __auto_type _ = _t603;
                        /* pass */
                    }
                } else if (_tr_dict_contains(self->enums, _tr_strz(gn))) {
                    /* pass */
                    ret_ty = AstType_init(gn);
                } else {
                    /* pass */
                    __auto_type _t604 = (*idx);
                    if (_t604.tag == Expr_EIdent) {
                        __auto_type farg_c = _t604.data.EIdent.name;
                        /* pass */
                        AstType* fret = AstType_init(farg_c);
                        /* pass */
                        ret_ty = fret;
                        /* pass */
                        hcallee = ({ TrStr _at_t605 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(gn), _tr_strz(_tr_str_lit("__MONO_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(farg_c)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t605, fret, false))); _tr_str_release(_at_t605); _wr; });
                    } else if (_t604.tag == Expr_ETuple) {
                        __auto_type ftargs = _t604.data.ETuple.items;
                        /* pass */
                        List_TrStr* _mf_names = (void*)List_TrStr_new();
                        /* pass */
                        long long _mfi = 0LL;
                        /* pass */
                        while ((_mfi < ftargs->len)) {
                            /* pass */
                            TrStr _mf_nm = _tr_str_lit("");
                            /* pass */
                            __auto_type _t606 = (*((Expr*)List_ptr_get(ftargs, _mfi)));
                            if (_t606.tag == Expr_EIdent) {
                                __auto_type _mfn = _t606.data.EIdent.name;
                                TrStr _strtmp_t607 = _tr_str_retain(_mfn);
                                _tr_str_release(_mf_nm);
                                _mf_nm = _strtmp_t607;
                            } else if (1) {
                                __auto_type _ = _t606;
                                /* pass */
                            }
                            /* pass */
                            List_TrStr_append(_mf_names, _mf_nm);
                            /* pass */
                            _mfi = (_mfi + 1LL);
                            _tr_str_release(_mf_nm);
                        }
                        /* pass */
                        TrStr _mf_mangled = _tr_str_retain(gn);
                        /* pass */
                        long long _mf_ji = 0LL;
                        /* pass */
                        while ((_mf_ji < _mf_names->len)) {
                            /* pass */
                            if ((_mf_ji == 0LL)) {
                                /* pass */
                                TrStr _strtmp_t608 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_mf_mangled), _tr_strz(_tr_str_lit("__MONO_")))); TrStr _cr = (List_TrStr_get(_mf_names, _mf_ji)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                                _tr_str_release(_mf_mangled);
                                _mf_mangled = _strtmp_t608;
                            } else {
                                /* pass */
                                TrStr _strtmp_t609 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_mf_mangled), _tr_strz(_tr_str_lit("_")))); TrStr _cr = (List_TrStr_get(_mf_names, _mf_ji)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                                _tr_str_release(_mf_mangled);
                                _mf_mangled = _strtmp_t609;
                            }
                            /* pass */
                            _mf_ji = (_mf_ji + 1LL);
                        }
                        /* pass */
                        AstType* fret2 = AstType_init(_tr_str_lit("void"));
                        /* pass */
                        if (_tr_dict_contains(self->fn_defs, _tr_strz(gn))) {
                            /* pass */
                            FunctionDef* _mf_fd = ((FunctionDef*)(uintptr_t)_tr_dict_get(self->fn_defs, _tr_strz(gn)));
                            /* pass */
                            if ((((unsigned long long)(_mf_fd->ret_ty)) != ((unsigned long long)(0LL)))) {
                                /* pass */
                                fret2 = (*_mf_fd->ret_ty);
                            }
                            /* pass */
                            long long _mf_gi = 0LL;
                            /* pass */
                            while ((_mf_gi < _mf_fd->generics->len)) {
                                /* pass */
                                if (((strcmp(_tr_strz(List_TrStr_get(_mf_fd->generics, _mf_gi)), _tr_strz(fret2->name)) == 0) && (_mf_gi < _mf_names->len))) {
                                    /* pass */
                                    fret2 = ({ TrStr _at_t610 = (List_TrStr_get(_mf_names, _mf_gi)); __auto_type _wr = (AstType_init(_at_t610)); _tr_str_release(_at_t610); _wr; });
                                }
                                /* pass */
                                _mf_gi = (_mf_gi + 1LL);
                            }
                        }
                        /* pass */
                        ret_ty = fret2;
                        /* pass */
                        hcallee = box_hirexpr(HirExpr_ctor_EIdent(_mf_mangled, fret2, false));
                        List_TrStr_free(_mf_names);
                        _tr_str_release(_mf_mangled);
                    } else if (1) {
                        __auto_type _ = _t604;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t599;
                /* pass */
            }
        } else if (_t597.tag == Expr_EPropAccess) {
            __auto_type obj = _t597.data.EPropAccess.obj;
__auto_type variant = _t597.data.EPropAccess.prop;
            /* pass */
            __auto_type _t611 = (*obj);
            if (_t611.tag == Expr_EIdent) {
                __auto_type type_name = _t611.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->enums, _tr_strz(type_name))) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if ((((((strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("ThreadPool"))) == 0)) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("ThreadLocal"))) == 0)) && (((strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("spawn"))) == 0) || (strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("new"))) == 0)) || (strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("init"))) == 0)))) {
                    /* pass */
                    ret_ty = AstType_init(type_name);
                } else if (((strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("new"))) == 0))) {
                    /* pass */
                    if ((((((strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Pointer"))) == 0) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(type_name), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    } else if (_tr_dict_contains(self->classes, _tr_strz(type_name))) {
                        /* pass */
                        ret_ty = AstType_init(type_name);
                    }
                }
            } else if (_t611.tag == Expr_EIndex) {
                __auto_type base2 = _t611.data.EIndex.obj;
__auto_type idx2 = _t611.data.EIndex._tr_v_index;
                /* pass */
                if (((strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(variant), _tr_strz(_tr_str_lit("new"))) == 0))) {
                    /* pass */
                    __auto_type _t612 = (*base2);
                    if (_t612.tag == Expr_EIdent) {
                        __auto_type gn2 = _t612.data.EIdent.name;
                        /* pass */
                        if ((((((strcmp(_tr_strz(gn2), _tr_strz(_tr_str_lit("Pointer"))) == 0) || (strcmp(_tr_strz(gn2), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(gn2), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(gn2), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(gn2), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
                            /* pass */
                            __auto_type _t613 = (*idx2);
                            if (_t613.tag == Expr_EIdent) {
                                __auto_type tn2 = _t613.data.EIdent.name;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                                /* pass */
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn2)));
                            } else if (1) {
                                __auto_type _ = _t613;
                                /* pass */
                                ret_ty = AstType_init(gn2);
                            }
                        } else if (_tr_dict_contains(self->classes, _tr_strz(gn2))) {
                            /* pass */
                            ret_ty = AstType_init(gn2);
                            /* pass */
                            __auto_type _t614 = (*idx2);
                            if (_t614.tag == Expr_EIdent) {
                                __auto_type targ_cb = _t614.data.EIdent.name;
                                List_ptr_append(ret_ty->args, box_asttype(AstType_init(targ_cb)));
                            } else if (1) {
                                __auto_type _ = _t614;
                                /* pass */
                            }
                        }
                    } else if (1) {
                        __auto_type _ = _t612;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t611;
                /* pass */
            }
        } else if (1) {
            __auto_type _ = _t597;
            /* pass */
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t615 = (*callee);
            if (_t615.tag == Expr_EIdent) {
                __auto_type aa_nm = _t615.data.EIdent.name;
                /* pass */
                if ((strcmp(_tr_strz(aa_nm), _tr_strz(_tr_str_lit("await_all"))) == 0)) {
                    /* pass */
                    long long aa_i = 0LL;
                    /* pass */
                    while ((aa_i < hl->len)) {
                        /* pass */
                        Sema_check_spawn_sendable(self, ((HirExpr*)List_ptr_get(hl, aa_i)));
                        /* pass */
                        aa_i = (aa_i + 1LL);
                    }
                }
            } else if (1) {
                __auto_type _ = _t615;
                /* pass */
            }
        }
        /* pass */
        if ((((unsigned long long)(callee)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t616 = (*callee);
            if (_t616.tag == Expr_EIdent) {
                __auto_type df_nm = _t616.data.EIdent.name;
                /* pass */
                if (((strcmp(_tr_strz(df_nm), _tr_strz(_tr_str_lit("dealloc"))) == 0) && (args->len > 0LL))) {
                    /* pass */
                    Expr* df_arg0 = ((Expr*)List_ptr_get(args, 0LL));
                    /* pass */
                    if ((((unsigned long long)(df_arg0)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        __auto_type _t617 = (*df_arg0);
                        if (_t617.tag == Expr_EIdent) {
                            __auto_type df_ptr = _t617.data.EIdent.name;
                            /* pass */
                            Symbol* df_sym = Sema_resolve(self, df_ptr);
                            /* pass */
                            if ((strcmp(_tr_strz(df_sym->name), _tr_strz(_tr_str_lit(""))) != 0)) {
                                /* pass */
                                Sema_mark_freed(self, df_ptr);
                            }
                        } else if (1) {
                            __auto_type _ = _t617;
                            /* pass */
                        }
                    }
                }
            } else if (1) {
                __auto_type _ = _t616;
                /* pass */
            }
        }
        /* pass */
        List_TrStr_free(p23_borrow_names);
        Dict_free(p23_seen);
        return box_hirexpr(HirExpr_ctor_ECall(hcallee, hl, ret_ty));
    } else if (_t570.tag == Expr_EMethodCall) {
        __auto_type obj = _t570.data.EMethodCall.obj;
__auto_type method = _t570.data.EMethodCall.method;
__auto_type args = _t570.data.EMethodCall.args;
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__index__"))) == 0) && (args->len > 0LL))) {
            /* pass */
            return Sema_lower_expr(self, box_expr(Expr_ctor_EIndex(obj, ((Expr*)List_ptr_get(args, 0LL)))));
        }
        /* pass */
        if (((self->no_heap && (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("new"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("with_capacity"))) == 0))) && (((unsigned long long)(obj)) != ((unsigned long long)(0LL))))) {
            /* pass */
            TrStr _nh_rn = _tr_str_lit("");
            /* pass */
            __auto_type _t618 = (*obj);
            if (_t618.tag == Expr_EIdent) {
                __auto_type _nhn = _t618.data.EIdent.name;
                TrStr _strtmp_t619 = _tr_str_retain(_nhn);
                _tr_str_release(_nh_rn);
                _nh_rn = _strtmp_t619;
            } else if (_t618.tag == Expr_EIndex) {
                __auto_type _nhbase = _t618.data.EIndex.obj;
                /* pass */
                if ((((unsigned long long)(_nhbase)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    __auto_type _t620 = (*_nhbase);
                    if (_t620.tag == Expr_EIdent) {
                        __auto_type _nhn2 = _t620.data.EIdent.name;
                        TrStr _strtmp_t621 = _tr_str_retain(_nhn2);
                        _tr_str_release(_nh_rn);
                        _nh_rn = _strtmp_t621;
                    } else if (1) {
                        __auto_type _ = _t620;
                        /* pass */
                    }
                }
            } else if (1) {
                __auto_type _ = _t618;
                /* pass */
            }
            /* pass */
            if ((((((strcmp(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit("Set"))) == 0))) {
                /* pass */
                ({ TrStr _at_t622 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_nh_rn), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("() (a growable heap collection)"))); _tr_str_release(_cl); _cres; })); Sema_check_no_heap(self, _at_t622, _tr_str_lit("use a fixed-size array [T; N] (stack), or parallel fixed arrays for a map")); _tr_str_release(_at_t622); });
            }
        }
        /* pass */
        List_ptr* hl = (void*)List_ptr_new();
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < args->len)) {
            /* pass */
            List_ptr_append(hl, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k))));
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        bool _saved_recv_mc = self->in_recv_pos;
        /* pass */
        self->in_recv_pos = true;
        /* pass */
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        self->in_recv_pos = _saved_recv_mc;
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("free"))) == 0)) {
            /* pass */
            __auto_type _t623 = (*obj);
            if (_t623.tag == Expr_EIdent) {
                __auto_type free_nm = _t623.data.EIdent.name;
                /* pass */
                Symbol* free_sym = Sema_resolve(self, free_nm);
                /* pass */
                if ((strcmp(_tr_strz(free_sym->name), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    Sema_mark_freed(self, free_nm);
                }
            } else if (1) {
                __auto_type _ = _t623;
                /* pass */
            }
        }
        /* pass */
        AstType* hobj_ty = hir_expr_type(hobj);
        /* pass */
        if (_tr_dict_contains(self->type_aliases, _tr_strz(hobj_ty->name))) {
            /* pass */
            TrStr _alias_base = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_aliases, _tr_strz(hobj_ty->name))));
            /* pass */
            AstType* _alias_ty = AstType_init(_alias_base);
            /* pass */
            if (_tr_dict_contains(self->type_alias_elem, _tr_strz(hobj_ty->name))) {
                /* pass */
                TrStr _elem_name = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->type_alias_elem, _tr_strz(hobj_ty->name))));
                /* pass */
                _alias_ty = AstType_init_generic(_alias_base, box_asttype(AstType_init(_elem_name)));
            }
            /* pass */
            hobj_ty = _alias_ty;
        }
        /* pass */
        TrStr _recv_name = _tr_str_lit("");
        /* pass */
        bool _recv_is_shared = false;
        /* pass */
        __auto_type _t624 = (*obj);
        if (_t624.tag == Expr_EIdent) {
            __auto_type _rn = _t624.data.EIdent.name;
            /* pass */
            TrStr _strtmp_t625 = _tr_str_retain(_rn);
            _tr_str_release(_recv_name);
            _recv_name = _strtmp_t625;
            /* pass */
            Symbol* _rsym = Sema_resolve(self, _rn);
            /* pass */
            if (((strcmp(_tr_strz(_rsym->name), _tr_strz(_tr_str_lit(""))) != 0) && _rsym->is_shared)) {
                /* pass */
                _recv_is_shared = true;
            }
        } else if (1) {
            __auto_type _ = _t624;
            /* pass */
        }
        /* pass */
        if ((((strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit(""))) != 0) && (!Sema_is_local_var(self, _recv_name))) && (!Sema__is_known_type_name(self, _recv_name)))) {
            /* pass */
            if ((strcmp(_tr_strz(Sema_resolve(self, _recv_name)->name), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                ({ TrStr _at_t626 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[E-1] '")), _tr_strz(_recv_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not a defined type or value, so '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_recv_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(...)' cannot be resolved.\n      FIX: check the spelling, import the type, or define it — a call on an unknown name would otherwise emit invalid C."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t626); _tr_str_release(_at_t626); });
            }
        }
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
            /* pass */
            ClassDef* _cf_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
            /* pass */
            long long _cf_i = 0LL;
            /* pass */
            AstType* _cf_ty = AstType_init(_tr_str_lit("void"));
            /* pass */
            bool _cf_found = false;
            /* pass */
            while ((_cf_i < _cf_cls->fields->len)) {
                /* pass */
                FieldDef* _cf_fld = ((FieldDef*)List_ptr_get(_cf_cls->fields, _cf_i));
                /* pass */
                if (((strcmp(_tr_strz(_cf_fld->name), _tr_strz(method)) == 0) && (((unsigned long long)(_cf_fld->ty)) != ((unsigned long long)(0LL))))) {
                    /* pass */
                    _cf_ty = (*_cf_fld->ty);
                    /* pass */
                    _cf_found = true;
                }
                /* pass */
                _cf_i = (_cf_i + 1LL);
            }
            /* pass */
            if ((_cf_found && (strcmp(_tr_strz(_cf_ty->name), _tr_strz(_tr_str_lit("def"))) == 0))) {
                /* pass */
                AstType* _cf_ret = AstType_init(_tr_str_lit("void"));
                /* pass */
                if ((_cf_ty->args->len > 0LL)) {
                    /* pass */
                    _cf_ret = (*((AstType**)List_ptr_get(_cf_ty->args, (_cf_ty->args->len - 1LL))));
                }
                /* pass */
                if (((_cf_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                    /* pass */
                    _cf_ret = Sema__subst_ret_generics(self, _cf_ret, _cf_cls->generics, hobj_ty->args);
                }
                /* pass */
                HirExpr* _cf_prop = box_hirexpr(HirExpr_ctor_EPropAccess(hobj, method, _cf_ty));
                /* pass */
                _tr_str_release(_recv_name);
                return box_hirexpr(HirExpr_ctor_ECall(_cf_prop, hl, _cf_ret));
            }
        }
        /* pass */
        if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("push"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pop"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("insert"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove"))) == 0))) {
            /* pass */
            TrStr pc_obj_nm = _tr_str_lit("");
            /* pass */
            __auto_type _t627 = (*obj);
            if (_t627.tag == Expr_EIdent) {
                __auto_type pc_src = _t627.data.EIdent.name;
                TrStr _strtmp_t628 = _tr_str_retain(pc_src);
                _tr_str_release(pc_obj_nm);
                pc_obj_nm = _strtmp_t628;
            } else if (1) {
                __auto_type _ = _t627;
                /* pass */
            }
            /* pass */
            if (((strcmp(_tr_strz(pc_obj_nm), _tr_strz(_tr_str_lit(""))) != 0) && _tr_dict_contains(self->container_borrows, _tr_strz(pc_obj_nm)))) {
                /* pass */
                TrStr pc_borrow_var = _tr_str_retain(_tr_str_unbox(_tr_dict_get(self->container_borrows, _tr_strz(pc_obj_nm))));
                /* pass */
                ({ TrStr _at_t629 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[M-4] Cannot mutate '")), _tr_strz(pc_obj_nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' while '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' holds a reference into it.\n      FIX: Finish using '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' before modifying '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pc_obj_nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("', or copy it first: 'mut copy = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pc_borrow_var)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t629); _tr_str_release(_at_t629); });
            }
        }
        /* pass */
        TrStr _bm_obj_nm = _tr_str_lit("");
        /* pass */
        __auto_type _t630 = (*obj);
        if (_t630.tag == Expr_EIdent) {
            __auto_type _bm_n = _t630.data.EIdent.name;
            TrStr _strtmp_t631 = _tr_str_retain(_bm_n);
            _tr_str_release(_bm_obj_nm);
            _bm_obj_nm = _strtmp_t631;
        } else if (1) {
            __auto_type _ = _t630;
            /* pass */
        }
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        AstType** _gpb_ret = ((AstType**)(0LL));
        /* pass */
        if (((self->current_func_constraints->len > 0LL) && Sema__is_type_param_in_scope(self, hobj_ty->name))) {
            /* pass */
            _gpb_ret = Sema__resolve_generic_bound_method_ret(self, hobj_ty->name, method);
        }
        /* pass */
        if ((((unsigned long long)(_gpb_ret)) != ((unsigned long long)(0LL)))) {
            /* pass */
            ret_ty = (*_gpb_ret);
        } else if (((strcmp(_tr_strz(_bm_obj_nm), _tr_strz(_tr_str_lit("OS"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("OS"))) == 0))) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cwd"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("platform"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("str"));
            } else if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_windows"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_linux"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_darwin"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_macos"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            }
        } else if (((strcmp(_tr_strz(_bm_obj_nm), _tr_strz(_tr_str_lit("Process"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Process"))) == 0))) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("system"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("exit"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("shell_output"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("str"));
            }
        } else if (((strcmp(_tr_strz(_bm_obj_nm), _tr_strz(_tr_str_lit("Env"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Env"))) == 0))) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_var"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cwd"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("str"));
            }
        } else if (((strcmp(_tr_strz(_bm_obj_nm), _tr_strz(_tr_str_lit("Hash"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Hash"))) == 0))) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sha256"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("md5"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("str"));
            }
        }
        /* pass */
        if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("write"))) == 0)) && (!self->in_unsafe))) {
            /* pass */
            Sema_error(self, _tr_str_lit("[P-1] '.write()' on a Pointer mutates raw memory and must be inside an 'unsafe:' block.\n      FIX: Wrap this call in 'unsafe:', e.g.\n          unsafe:\n              ... .write(...) ..."));
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("init"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("new"))) == 0))) {
            /* pass */
            if (((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit(""))) == 0))) {
                /* pass */
                __auto_type _t632 = (*hobj);
                if (_t632.tag == HirExpr_EIdent) {
                    __auto_type recv_nm = _t632.data.EIdent.name;
                    ret_ty = AstType_init(recv_nm);
                } else if (_t632.tag == HirExpr_EIndex) {
                    __auto_type idx_base = _t632.data.EIndex.obj;
__auto_type idx_arg = _t632.data.EIndex._tr_v_index;
                    /* pass */
                    __auto_type _t633 = (*idx_base);
                    if (_t633.tag == HirExpr_EIdent) {
                        __auto_type gn = _t633.data.EIdent.name;
                        /* pass */
                        ret_ty = AstType_init(gn);
                        /* pass */
                        __auto_type _t634 = (*idx_arg);
                        if (_t634.tag == HirExpr_EIdent) {
                            __auto_type tn = _t634.data.EIdent.name;
                            List_ptr_append(ret_ty->args, box_asttype(AstType_init(tn)));
                        } else if (1) {
                            __auto_type _ = _t634;
                            /* pass */
                        }
                    } else if (1) {
                        __auto_type _ = _t633;
                        ret_ty = hobj_ty;
                    }
                } else if (1) {
                    __auto_type _ = _t632;
                    ret_ty = hobj_ty;
                }
            } else {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("offset"))) == 0)) {
            /* pass */
            if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (!self->cur_fn_is_lib)) && (!self->in_unsafe))) {
                /* pass */
                Sema_error(self, _tr_str_lit("[P-2] '.offset()' does raw pointer arithmetic (can create an out-of-bounds pointer) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or use a safe slice / Vec[T] instead of pointer math."));
            }
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sum"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("min"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("min_val"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max_val"))) == 0)))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            }
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("any"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("all"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("bool"));
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("count"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("index_of"))) == 0)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("int"));
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("first"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("last"))) == 0)))) {
            /* pass */
            if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("void"));
            }
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reversed"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reversed_copy"))) == 0)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("copy"))) == 0)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("reverse"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("void"));
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("index_of"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("last_index_of"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("count"))) == 0)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("int"));
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("join"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("str"));
        } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("read"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                if (((!self->cur_fn_is_lib) && (!self->in_unsafe))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit("[P-2] '.read()' dereferences a raw pointer (may read freed or invalid memory) and must be inside an 'unsafe:' block.\n      FIX: wrap it in 'unsafe:', or hold the value in a safe Box[T] / Vec[T] / class reference instead of a raw pointer."));
                }
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            }
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("as_str"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_str"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("str"));
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("__len__"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("int"));
        } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("checked_add"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("checked_sub"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("checked_mul"))) == 0))) {
            /* pass */
            ret_ty = AstType_init_generic(_tr_str_lit("Option"), box_asttype(AstType_init(_tr_str_lit("int"))));
        } else if ((((((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("abs"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("min"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("max"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pow"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sign"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clamp"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("gcd"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lcm"))) == 0)) && (((((((((((((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i8"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("isize"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u8"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("usize"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("f32"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("f64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("char"))) == 0)))) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("f64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("f32"))) == 0))) {
            /* pass */
            if ((((((((((((((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("floor"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ceil"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("round"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sqrt"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("fabs"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log2"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("log10"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("exp"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sin"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cos"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("tan"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("asin"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("acos"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("atan"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("atan2"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pow"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("float"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_nan"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_inf"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            ret_ty = Sema_str_method_ret_ty(self, method);
        } else if ((((((((((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("i8"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u64"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("u8"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("usize"))) == 0)) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("char"))) == 0))) {
            /* pass */
            if ((((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_hex"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_HEX"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_hex_upper"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_octal"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_oct"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_binary"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_bin"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("str"));
            }
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_float"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_f64"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("float"));
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_int"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_i64"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("int"));
        } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_str"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_string"))) == 0)) && (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("StringBuilder"))) != 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("str"));
        } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("pop"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get_or"))) == 0))) {
            /* pass */
            if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Map"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Dict"))) == 0)) && (hobj_ty->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
            } else if ((hobj_ty->args->len > 0LL)) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
            } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
                /* pass */
                ClassDef* _gcls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
                /* pass */
                long long _gmi = 0LL;
                /* pass */
                while ((_gmi < _gcls->methods->len)) {
                    /* pass */
                    FunctionDef* _gmdef = ((FunctionDef*)List_ptr_get(_gcls->methods, _gmi));
                    /* pass */
                    if ((strcmp(_tr_strz(_gmdef->name), _tr_strz(method)) == 0)) {
                        /* pass */
                        if ((((unsigned long long)(_gmdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                            /* pass */
                            ret_ty = (*_gmdef->ret_ty);
                        }
                    }
                    /* pass */
                    _gmi = (_gmi + 1LL);
                }
            }
        } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("alloc"))) == 0)) {
            /* pass */
            ret_ty = hobj_ty;
        } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0)) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("bool"));
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Array"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("as_ptr"))) == 0)) {
                /* pass */
                AstType* _arr_elem = AstType_init(_tr_str_lit("u8"));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _arr_elem = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit("Pointer"), box_asttype(_arr_elem));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("length"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Set"))) == 0)) {
            /* pass */
            if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("contains"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("has"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_empty"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_subset"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("length"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("remove"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clear"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("void"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_list"))) == 0)) {
                /* pass */
                AstType* _set_elem_ty = AstType_init(_tr_str_lit("str"));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _set_elem_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit("List"), box_asttype(_set_elem_ty));
            } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("union"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("intersection"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("difference"))) == 0))) {
                /* pass */
                ret_ty = hobj_ty;
            }
        } else if (((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Map"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("keys"))) == 0)) {
                /* pass */
                AstType* _dict_key_ty = AstType_init(_tr_str_lit("str"));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    _dict_key_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit("List"), box_asttype(_dict_key_ty));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("values"))) == 0)) {
                /* pass */
                AstType* _dict_val_ty = AstType_init(_tr_str_lit("ptr"));
                /* pass */
                if ((hobj_ty->args->len > 1LL)) {
                    /* pass */
                    _dict_val_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                }
                /* pass */
                ret_ty = AstType_init_generic(_tr_str_lit("List"), box_asttype(_dict_val_ty));
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Option"))) == 0)) {
            /* pass */
            if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Some"))) == 0) || ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("some"))) == 0) && (strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit("Option"))) == 0))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Option"));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("None"))) == 0) || ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("none"))) == 0) && (strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit("Option"))) == 0)))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Option"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_some"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_none"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("unwrap"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("expect"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("unwrap_or"))) == 0)) {
                /* pass */
                if ((hl->len > 0LL)) {
                    /* pass */
                    ret_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)));
                } else if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Result"))) == 0)) {
            /* pass */
            if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Ok"))) == 0) || ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ok"))) == 0) && (strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit("Result"))) == 0))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Result"));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
            } else if ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Err"))) == 0) || ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("err"))) == 0) && (strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit("Result"))) == 0))) && (hl->len == 1LL))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Result"));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_ok"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_err"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("unwrap"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("ok"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("unwrap_err"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("err"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 1LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 1LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("str"));
                }
            }
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("spawn"))) == 0) && ((strcmp(_tr_strz(_recv_name), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Thread"))) == 0)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("Thread"));
            /* pass */
            long long _tsi = 1LL;
            /* pass */
            while ((_tsi < hl->len)) {
                /* pass */
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _tsi)))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to Thread.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow."));
                }
                /* pass */
                AstType* _tsa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _tsi)));
                /* pass */
                if ((!Sema_is_sendable_ty(self, _tsa_ty))) {
                    /* pass */
                    ({ TrStr _at_t635 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] Type '")), _tr_strz(_tsa_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable and cannot be passed to Thread.spawn.\n      FIX: Wrap in Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] for exclusive access, or add 'implements Sendable' to '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' to confirm it is thread-safe."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t635); _tr_str_release(_at_t635); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _tsa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _tsi))))) && (!Sema__is_unsafe_sendable(self, _tsa_ty->name)))) {
                    /* pass */
                    ({ TrStr _at_t636 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-7] '")), _tr_strz(_tsa_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it is not 'Send' and cannot be passed to Thread.spawn (exactly why Rust's 'Rc' is '!Send'): retaining/releasing it from another thread races the count.\n      FIX: share it as 'Shared["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tsa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (atomic refcount, like Rust's 'Arc'). A 'Sendable' class must itself hold only Sendable + atomically-shared state."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t636); _tr_str_release(_at_t636); });
                }
                /* pass */
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _tsi)));
                /* pass */
                _tsi = (_tsi + 1LL);
            }
        } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("spawn"))) == 0) && (strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("ThreadPool"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("void"));
            /* pass */
            long long _psi = 1LL;
            /* pass */
            while ((_psi < hl->len)) {
                /* pass */
                if (Sema_expr_is_borrow(self, ((HirExpr*)List_ptr_get(hl, _psi)))) {
                    /* pass */
                    Sema_error(self, _tr_str_lit("[T-6] a borrow (`ref`/`mut ref`) cannot be passed to ThreadPool.spawn: the borrowed value may be mutated or freed by another thread, or outlive its source.\n      FIX: pass an owned value, a `Shared[T]`, or a `Mutex[T]`/`Atomic[T]` handle instead of a borrow."));
                }
                /* pass */
                AstType* _psa_ty = hir_expr_type(((HirExpr*)List_ptr_get(hl, _psi)));
                /* pass */
                if ((!Sema_is_sendable_ty(self, _psa_ty))) {
                    /* pass */
                    ({ TrStr _at_t637 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-1] Type '")), _tr_strz(_psa_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is not Sendable and cannot be passed to ThreadPool.spawn.\n      FIX: Wrap in Mutex["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("] for exclusive access, or add 'implements Sendable' to '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' to confirm it is thread-safe."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t637); _tr_str_release(_at_t637); });
                } else if ((((self->strict_mode && Sema__is_rc_class(self, _psa_ty->name)) && (!Sema__expr_is_shared(self, ((HirExpr*)List_ptr_get(hl, _psi))))) && (!Sema__is_unsafe_sendable(self, _psa_ty->name)))) {
                    /* pass */
                    ({ TrStr _at_t638 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[T-7] '")), _tr_strz(_psa_ty->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' is a plain reference-counted class — its refcount is thread-local (non-atomic), so it cannot be passed to ThreadPool.spawn (like Rust's 'Rc' being '!Send').\n      FIX: share it as 'Shared["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_psa_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]' (atomic refcount, like Rust's 'Arc')."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t638); _tr_str_release(_at_t638); });
                }
                /* pass */
                Sema__check_spawn_nested_rc(self, ((HirExpr*)List_ptr_get(hl, _psi)));
                /* pass */
                _psi = (_psi + 1LL);
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Atomic"))) == 0)) {
            /* pass */
            if (((((((((((((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("load"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sub"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("swap"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("exchange"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("fetch_add"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("fetch_sub"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("load_relaxed"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("load_acquire"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("load_seqcst"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add_relaxed"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add_release"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("add_acqrel"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sub_relaxed"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("sub_release"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            } else if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cas"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("compare_exchange"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cas_weak"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cas_acqrel"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("store"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("set"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("store_relaxed"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("store_release"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("store_seqcst"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("void"));
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Chan"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("recv"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("try_recv"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("cap"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            } else if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_closed"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("try_send"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("send_timeout"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("recv_timeout"))) == 0)) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Mutex"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("lock"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("get"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("RwLock"))) == 0)) {
            /* pass */
            if (((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("read"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("read_lock"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("write"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("write_lock"))) == 0))) {
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, 0LL)));
                } else {
                    /* pass */
                    ret_ty = AstType_init(_tr_str_lit("int"));
                }
            }
        } else if (((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("StringBuilder"))) == 0) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_string"))) == 0))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("StringObj"));
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Shared"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0)) {
                /* pass */
                ret_ty = hobj_ty;
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_null"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("downgrade"))) == 0)) {
                /* pass */
                AstType* weak_ty = AstType_init(_tr_str_lit("Weak"));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    weak_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(weak_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = weak_ty;
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("drop"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("void"));
            }
        } else if ((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Weak"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("upgrade"))) == 0)) {
                /* pass */
                AstType* opt_ty = AstType_init(_tr_str_lit("Option"));
                /* pass */
                if ((hobj_ty->args->len > 0LL)) {
                    /* pass */
                    opt_ty->args = (void*)List_ptr_new();
                    /* pass */
                    List_ptr_append(opt_ty->args, box_asttype((*((AstType**)List_ptr_get(hobj_ty->args, 0LL)))));
                }
                /* pass */
                ret_ty = opt_ty;
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_alive"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            }
        } else if ((_recv_is_shared && ((((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("downgrade"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("drop"))) == 0)) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_null"))) == 0)))) {
            /* pass */
            if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("clone"))) == 0)) {
                /* pass */
                ret_ty = hobj_ty;
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("is_null"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("drop"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("void"));
            } else if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("downgrade"))) == 0)) {
                /* pass */
                AstType* _wk_ty = AstType_init(_tr_str_lit("Weak"));
                /* pass */
                _wk_ty->args = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(_wk_ty->args, box_asttype(hobj_ty));
                /* pass */
                ret_ty = _wk_ty;
            }
        } else if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(hobj_ty->name)))) {
            /* pass */
            InterfaceDef* _iface_def = ((InterfaceDef*)(uintptr_t)_tr_dict_get(self->interfaces, _tr_strz(hobj_ty->name)));
            /* pass */
            long long _imi = 0LL;
            /* pass */
            while ((_imi < _iface_def->methods->len)) {
                /* pass */
                FunctionDef* _imdef = ((FunctionDef*)List_ptr_get(_iface_def->methods, _imi));
                /* pass */
                if ((strcmp(_tr_strz(_imdef->name), _tr_strz(method)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_imdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_imdef->ret_ty);
                    }
                }
                /* pass */
                _imi = (_imi + 1LL);
            }
            /* pass */
            if (((_iface_def->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                long long _gi5 = 0LL;
                /* pass */
                while ((_gi5 < _iface_def->generics->len)) {
                    /* pass */
                    if ((strcmp(_tr_strz(List_TrStr_get(_iface_def->generics, _gi5)), _tr_strz(ret_ty->name)) == 0)) {
                        /* pass */
                        if ((_gi5 < hobj_ty->args->len)) {
                            /* pass */
                            ret_ty = (*((AstType**)List_ptr_get(hobj_ty->args, _gi5)));
                        }
                    }
                    /* pass */
                    _gi5 = (_gi5 + 1LL);
                }
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty->name))) {
            /* pass */
            ret_ty = AstType_init(hobj_ty->name);
            /* pass */
            if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Option"))) == 0) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Some"))) == 0)) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Result"))) == 0) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Ok"))) == 0)) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
            } else if ((((strcmp(_tr_strz(hobj_ty->name), _tr_strz(_tr_str_lit("Result"))) == 0) && (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("Err"))) == 0)) && (hl->len == 1LL))) {
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
                /* pass */
                List_ptr_append(ret_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hl, 0LL)))));
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty->name))) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty->name)));
            /* pass */
            TrStr _hty_n = _tr_str_retain(hobj_ty->name);
            /* pass */
            bool _is_builtin_dispatch = ((((((((((((((((strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("ThreadLocal"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("ThreadPool"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Mutex"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Channel"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("OS"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Process"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Env"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("Hash"))) == 0)) || (strcmp(_tr_strz(_hty_n), _tr_strz(_tr_str_lit("File"))) == 0));
            /* pass */
            if ((((!_is_builtin_dispatch) && (!Sema_class_method_exists(self, hobj_ty->name, method))) && (!Sema_is_universal_method(self, method)))) {
                /* pass */
                ({ TrStr _at_t639 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[E-1] No method '")), _tr_strz(method))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' found on type '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'.\n      FIX: Define 'pub def "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(self, ...)' in '"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' or its base class via 'extend "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(hobj_ty->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":'."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t639); _tr_str_release(_at_t639); });
            }
            /* pass */
            long long _mi = 0LL;
            /* pass */
            while ((_mi < _cls->methods->len)) {
                /* pass */
                FunctionDef* _mdef = ((FunctionDef*)List_ptr_get(_cls->methods, _mi));
                /* pass */
                if ((strcmp(_tr_strz(_mdef->name), _tr_strz(method)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_mdef->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        long long _mpc = 0LL;
                        /* pass */
                        long long _mpi = 0LL;
                        /* pass */
                        while ((_mpi < _mdef->params->len)) {
                            /* pass */
                            if ((strcmp(_tr_strz(((Param*)List_ptr_get(_mdef->params, _mpi))->name), _tr_strz(_tr_str_lit("self"))) != 0)) {
                                /* pass */
                                _mpc = (_mpc + 1LL);
                            }
                            /* pass */
                            _mpi = (_mpi + 1LL);
                        }
                        /* pass */
                        if ((_mpc == hl->len)) {
                            /* pass */
                            ret_ty = (*_mdef->ret_ty);
                        } else if ((strcmp(_tr_strz(ret_ty->name), _tr_strz(_tr_str_lit("void"))) == 0)) {
                            /* pass */
                            ret_ty = (*_mdef->ret_ty);
                        }
                    }
                }
                /* pass */
                _mi = (_mi + 1LL);
            }
            /* pass */
            if (((_cls->generics->len > 0LL) && (hobj_ty->args->len > 0LL))) {
                /* pass */
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty->args);
            }
        }
        /* pass */
        _tr_str_release(_recv_name);
        _tr_str_release(_bm_obj_nm);
        return box_hirexpr(HirExpr_ctor_EMethodCall(hobj, method, hl, ret_ty));
    } else if (_t570.tag == Expr_EPropAccess) {
        __auto_type obj = _t570.data.EPropAccess.obj;
__auto_type prop = _t570.data.EPropAccess.prop;
        /* pass */
        __auto_type _t640 = (*obj);
        if (_t640.tag == Expr_EIdent) {
            __auto_type _pmv_cn = _t640.data.EIdent.name;
            /* pass */
            Symbol* _pmv_sym = Sema_resolve(self, _pmv_cn);
            /* pass */
            if (((((strcmp(_tr_strz(_pmv_sym->name), _tr_strz(_tr_str_lit(""))) == 0) || (_pmv_sym->kind.tag == SymbolKind_make_SClass().tag)) && _tr_dict_contains(self->classes, _tr_strz(_pmv_cn))) && Sema_class_method_exists(self, _pmv_cn, prop))) {
                /* pass */
                ClassDef* _pmv_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(_pmv_cn)));
                /* pass */
                FunctionDef* _pmv_m = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, 0LL));
                /* pass */
                bool _pmv_found = false;
                /* pass */
                long long _pmv_mi = 0LL;
                /* pass */
                while ((_pmv_mi < _pmv_cls->methods->len)) {
                    /* pass */
                    FunctionDef* _pmv_mm = ((FunctionDef*)List_ptr_get(_pmv_cls->methods, _pmv_mi));
                    /* pass */
                    if ((strcmp(_tr_strz(_pmv_mm->name), _tr_strz(prop)) == 0)) {
                        /* pass */
                        bool _pmv_is_static = true;
                        /* pass */
                        if (((_pmv_mm->params->len > 0LL) && (strcmp(_tr_strz(((Param*)List_ptr_get(_pmv_mm->params, 0LL))->name), _tr_strz(_tr_str_lit("self"))) == 0))) {
                            /* pass */
                            _pmv_is_static = false;
                        }
                        /* pass */
                        if (_pmv_is_static) {
                            /* pass */
                            _pmv_m = _tr_obj_retain(_pmv_mm);
                            /* pass */
                            _pmv_found = true;
                        }
                    }
                    /* pass */
                    _pmv_mi = (_pmv_mi + 1LL);
                }
                /* pass */
                if (_pmv_found) {
                    /* pass */
                    AstType* _pmv_fnty = AstType_init(_tr_str_lit("def"));
                    /* pass */
                    long long _pmv_pi = 0LL;
                    /* pass */
                    while ((_pmv_pi < _pmv_m->params->len)) {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, ((Param*)List_ptr_get(_pmv_m->params, _pmv_pi))->ty);
                        /* pass */
                        _pmv_pi = (_pmv_pi + 1LL);
                    }
                    /* pass */
                    if ((((unsigned long long)(_pmv_m->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, _pmv_m->ret_ty);
                    } else {
                        /* pass */
                        List_ptr_append(_pmv_fnty->args, box_asttype(AstType_init(_tr_str_lit("void"))));
                    }
                    /* pass */
                    return ({ TrStr _at_t641 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_pmv_cn), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(prop)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (box_hirexpr(HirExpr_ctor_EIdent(_at_t641, _pmv_fnty, false))); _tr_str_release(_at_t641); _wr; });
                }
            }
        } else if (1) {
            __auto_type _ = _t640;
            /* pass */
        }
        /* pass */
        bool _saved_recv_pa = self->in_recv_pos;
        /* pass */
        self->in_recv_pos = true;
        /* pass */
        HirExpr* hobj = Sema_lower_expr(self, obj);
        /* pass */
        self->in_recv_pos = _saved_recv_pa;
        /* pass */
        TrStr hobj_ty_n = _tr_str_retain(hir_expr_type(hobj)->name);
        /* pass */
        AstType* hobj_ty_full = hir_expr_type(hobj);
        /* pass */
        AstType* ret_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if ((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Result"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("is_err"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("is_ok"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("ok"))) == 0) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            } else if (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("err"))) == 0) && (hobj_ty_full->args->len > 1LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 1LL)));
            }
        } else if ((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Option"))) == 0)) {
            /* pass */
            if (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("is_some"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("is_none"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("bool"));
            } else if ((((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("value"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("val"))) == 0)) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = (*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)));
            }
        } else if ((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("StringObj"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("data"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Pointer"));
            } else if (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("length"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("capacity"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            }
        } else if ((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("buf"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("StringObj"));
            }
        } else if (((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("List"))) == 0))) {
            /* pass */
            if ((((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("length"))) == 0)) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("capacity"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            } else if ((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("data"))) == 0)) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("Pointer"));
            }
        } else if ((((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Map"))) == 0) || (strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Set"))) == 0))) {
            /* pass */
            if ((((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("length"))) == 0)) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("capacity"))) == 0))) {
                /* pass */
                ret_ty = AstType_init(_tr_str_lit("int"));
            }
        } else if ((((strcmp(_tr_strz(hobj_ty_n), _tr_strz(_tr_str_lit("Shared"))) == 0) && (hobj_ty_full->args->len > 0LL)) && _tr_dict_contains(self->classes, _tr_strz((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name)))) {
            /* pass */
            ClassDef* _scls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz((*((AstType**)List_ptr_get(hobj_ty_full->args, 0LL)))->name)));
            /* pass */
            long long _sfi = 0LL;
            /* pass */
            while ((_sfi < _scls->fields->len)) {
                /* pass */
                FieldDef* _sfld = ((FieldDef*)List_ptr_get(_scls->fields, _sfi));
                /* pass */
                if ((strcmp(_tr_strz(_sfld->name), _tr_strz(prop)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_sfld->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_sfld->ty);
                    }
                }
                /* pass */
                _sfi = (_sfi + 1LL);
            }
        } else if (_tr_dict_contains(self->classes, _tr_strz(hobj_ty_n))) {
            /* pass */
            ClassDef* _cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(hobj_ty_n)));
            /* pass */
            long long _fi = 0LL;
            /* pass */
            while ((_fi < _cls->fields->len)) {
                /* pass */
                FieldDef* _fld = ((FieldDef*)List_ptr_get(_cls->fields, _fi));
                /* pass */
                if ((strcmp(_tr_strz(_fld->name), _tr_strz(prop)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(_fld->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        ret_ty = (*_fld->ty);
                    }
                }
                /* pass */
                _fi = (_fi + 1LL);
            }
            /* pass */
            if (((_cls->generics->len > 0LL) && (hobj_ty_full->args->len > 0LL))) {
                /* pass */
                ret_ty = Sema__subst_ret_generics(self, ret_ty, _cls->generics, hobj_ty_full->args);
            }
        } else if (_tr_dict_contains(self->enums, _tr_strz(hobj_ty_n))) {
            /* pass */
            ret_ty = AstType_init(hobj_ty_n);
        }
        /* pass */
        if (((strcmp(_tr_strz(ret_ty->name), _tr_strz(_tr_str_lit("void"))) == 0) && (((strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("len"))) == 0) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("length"))) == 0)) || (strcmp(_tr_strz(prop), _tr_strz(_tr_str_lit("capacity"))) == 0)))) {
            /* pass */
            ret_ty = AstType_init(_tr_str_lit("int"));
        }
        /* pass */
        _tr_str_release(hobj_ty_n);
        return box_hirexpr(HirExpr_ctor_EPropAccess(hobj, prop, ret_ty));
    } else if (_t570.tag == Expr_EIndex) {
        __auto_type obj = _t570.data.EIndex.obj;
__auto_type idx_inner = _t570.data.EIndex._tr_v_index;
        /* pass */
        HirExpr* hexpr_obj = Sema_lower_expr(self, obj);
        /* pass */
        TrStr obj_ty_n = _tr_str_retain(hir_expr_type(hexpr_obj)->name);
        /* pass */
        TrStr obj_name = _tr_str_lit("");
        /* pass */
        __auto_type _t642 = (*obj);
        if (_t642.tag == Expr_EIdent) {
            __auto_type n = _t642.data.EIdent.name;
            TrStr _strtmp_t643 = _tr_str_retain(n);
            _tr_str_release(obj_name);
            obj_name = _strtmp_t643;
        } else if (1) {
            __auto_type _ = _t642;
            /* pass */
        }
        /* pass */
        bool is_generic = false;
        /* pass */
        TrStr generic_arg_n = _tr_str_lit("");
        /* pass */
        AstType** generic_arg_ty = ((AstType**)(0LL));
        /* pass */
        List_ptr* generic_args = (void*)List_ptr_new();
        /* pass */
        if ((((unsigned long long)(idx_inner)) == ((unsigned long long)(0LL)))) {
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return hexpr_obj;
        }
        /* pass */
        __auto_type _t644 = (*idx_inner);
        if (_t644.tag == Expr_ETuple) {
            __auto_type _tup_targs = _t644.data.ETuple.items;
            /* pass */
            is_generic = true;
            /* pass */
            long long _tti = 0LL;
            /* pass */
            while ((_tti < _tup_targs->len)) {
                /* pass */
                List_ptr_append(generic_args, Sema__targ_of(self, ((Expr*)List_ptr_get(_tup_targs, _tti))));
                /* pass */
                _tti = (_tti + 1LL);
            }
            /* pass */
            if ((generic_args->len > 0LL)) {
                /* pass */
                generic_arg_ty = ((AstType**)List_ptr_get(generic_args, 0LL));
                /* pass */
                TrStr _strtmp_t645 = _tr_str_retain((*((AstType**)List_ptr_get(generic_args, 0LL)))->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t645;
            }
        } else if (_t644.tag == Expr_ETypeArg) {
            __auto_type targ_ty = _t644.data.ETypeArg.ty;
            /* pass */
            is_generic = true;
            /* pass */
            generic_arg_ty = targ_ty;
            /* pass */
            TrStr _strtmp_t646 = _tr_str_retain((*targ_ty)->name);
            _tr_str_release(generic_arg_n);
            generic_arg_n = _strtmp_t646;
        } else if (_t644.tag == Expr_EIdent) {
            __auto_type iname = _t644.data.EIdent.name;
            /* pass */
            bool is_param = false;
            /* pass */
            if ((strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                    /* pass */
                    ClassDef* cc = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                    /* pass */
                    long long ci = 0LL;
                    /* pass */
                    while ((ci < cc->generics->len)) {
                        /* pass */
                        if ((strcmp(_tr_strz(List_TrStr_get(cc->generics, ci)), _tr_strz(iname)) == 0)) {
                            /* pass */
                            is_param = true;
                        }
                        /* pass */
                        ci = (ci + 1LL);
                    }
                }
            }
            /* pass */
            long long fi = 0LL;
            /* pass */
            while ((fi < self->current_func_generics->len)) {
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(self->current_func_generics, fi)), _tr_strz(iname)) == 0)) {
                    /* pass */
                    is_param = true;
                }
                /* pass */
                fi = (fi + 1LL);
            }
            /* pass */
            if ((((((((((((((Sema_is_primitive_name(self, iname) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("str"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("Str"))) == 0)) || is_param) || (_tr_strlen(_tr_strz(iname)) == 1LL)) || _tr_dict_contains(self->classes, _tr_strz(iname))) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("StringObj"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || _tr_dict_contains(self->enums, _tr_strz(iname))) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("Vec"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("Tuple"))) == 0)) || (strcmp(_tr_strz(iname), _tr_strz(_tr_str_lit("tuple"))) == 0))) {
                /* pass */
                is_generic = true;
                /* pass */
                TrStr _strtmp_t647 = _tr_str_retain(iname);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t647;
            }
        } else if (_t644.tag == Expr_EIndex) {
            /* pass */
            is_generic = true;
            /* pass */
            AstType** nested_ty = Sema_build_ast_type(self, idx_inner);
            /* pass */
            if ((((unsigned long long)(nested_ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                TrStr _strtmp_t648 = _tr_str_retain((*nested_ty)->name);
                _tr_str_release(generic_arg_n);
                generic_arg_n = _strtmp_t648;
                /* pass */
                generic_arg_ty = nested_ty;
            }
        } else if (1) {
            __auto_type _ = _t644;
            /* pass */
        }
        /* pass */
        bool obj_is_type = ((((((((((((strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("alloc"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("dealloc"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("resize"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("copy"))) == 0)) || _tr_dict_contains(self->classes, _tr_strz(obj_name))) || _tr_dict_contains(self->enums, _tr_strz(obj_name)));
        /* pass */
        if ((is_generic && obj_is_type)) {
            /* pass */
            TrStr eff_ty_n = _tr_str_retain(obj_ty_n);
            /* pass */
            if ((((strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit(""))) == 0)) && ((((((strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("Pointer"))) == 0)))) {
                /* pass */
                TrStr _strtmp_t649 = _tr_str_retain(obj_name);
                _tr_str_release(eff_ty_n);
                eff_ty_n = _strtmp_t649;
            }
            /* pass */
            if ((((((((strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(eff_ty_n), _tr_strz(_tr_str_lit("Pointer"))) == 0)) && (strcmp(_tr_strz(generic_arg_n), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                AstType* container_ty = AstType_init(eff_ty_n);
                /* pass */
                container_ty->args = (void*)List_ptr_new();
                /* pass */
                if ((generic_args->len > 0LL)) {
                    /* pass */
                    long long _gci = 0LL;
                    /* pass */
                    while ((_gci < generic_args->len)) {
                        /* pass */
                        List_ptr_append(container_ty->args, ((AstType**)List_ptr_get(generic_args, _gci)));
                        /* pass */
                        _gci = (_gci + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    List_ptr_append(container_ty->args, generic_arg_ty);
                } else {
                    /* pass */
                    List_ptr_append(container_ty->args, box_asttype(AstType_init(generic_arg_n)));
                }
                /* pass */
                if (((((strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("alloc"))) == 0) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("dealloc"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("resize"))) == 0)) || (strcmp(_tr_strz(obj_name), _tr_strz(_tr_str_lit("copy"))) == 0))) {
                    /* pass */
                    _tr_str_release(obj_ty_n);
                    _tr_str_release(obj_name);
                    _tr_str_release(generic_arg_n);
                    _tr_str_release(eff_ty_n);
                    return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), container_ty));
                }
                /* pass */
                _tr_str_release(obj_ty_n);
                _tr_str_release(obj_name);
                _tr_str_release(generic_arg_n);
                return box_hirexpr(HirExpr_ctor_EIdent(eff_ty_n, container_ty, false));
            }
            /* pass */
            if (((_tr_dict_contains(self->classes, _tr_strz(obj_name)) || _tr_dict_contains(self->enums, _tr_strz(obj_name))) && (strcmp(_tr_strz(generic_arg_n), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                AstType* cls_ty = AstType_init(obj_name);
                /* pass */
                cls_ty->args = (void*)List_ptr_new();
                /* pass */
                if ((generic_args->len > 0LL)) {
                    /* pass */
                    long long _cgi = 0LL;
                    /* pass */
                    while ((_cgi < generic_args->len)) {
                        /* pass */
                        List_ptr_append(cls_ty->args, ((AstType**)List_ptr_get(generic_args, _cgi)));
                        /* pass */
                        _cgi = (_cgi + 1LL);
                    }
                } else if ((((unsigned long long)(generic_arg_ty)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    List_ptr_append(cls_ty->args, generic_arg_ty);
                } else {
                    /* pass */
                    List_ptr_append(cls_ty->args, box_asttype(AstType_init(generic_arg_n)));
                }
                /* pass */
                Sema_check_class_bounds(self, obj_name, cls_ty->args);
                /* pass */
                _tr_str_release(obj_ty_n);
                _tr_str_release(generic_arg_n);
                _tr_str_release(eff_ty_n);
                return box_hirexpr(HirExpr_ctor_EIdent(obj_name, cls_ty, false));
            }
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            _tr_str_release(eff_ty_n);
            return hexpr_obj;
        }
        /* pass */
        if (((strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Map"))) == 0) || (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
            /* pass */
            AstType* dval_ty = AstType_init(_tr_str_lit("void"));
            /* pass */
            AstType* dobj_full = hir_expr_type(hexpr_obj);
            /* pass */
            if ((dobj_full->args->len > 1LL)) {
                /* pass */
                dval_ty = (*((AstType**)List_ptr_get(dobj_full->args, 1LL)));
            }
            /* pass */
            List_ptr* didx_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(didx_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit("get_index"), didx_args, dval_ty));
        }
        /* pass */
        if ((strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Tuple"))) == 0)) {
            /* pass */
            AstType* tup_full = hir_expr_type(hexpr_obj);
            /* pass */
            AstType* telem_ty = AstType_init(_tr_str_lit("void"));
            /* pass */
            __auto_type _t650 = (*idx_inner);
            if (_t650.tag == Expr_ELitInt) {
                __auto_type tup_ci = _t650.data.ELitInt.val;
                /* pass */
                if (((tup_ci >= 0LL) && (tup_ci < tup_full->args->len))) {
                    /* pass */
                    telem_ty = (*((AstType**)List_ptr_get(tup_full->args, tup_ci)));
                }
            } else if (1) {
                __auto_type _ = _t650;
                /* pass */
            }
            /* pass */
            List_ptr* tup_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(tup_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit("get_index"), tup_args, telem_ty));
        }
        /* pass */
        if ((((((!Sema_is_primitive_name(self, obj_ty_n)) && (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("str"))) != 0)) && (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Pointer"))) != 0)) && (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("List"))) != 0)) && (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Vec"))) != 0))) {
            /* pass */
            List_ptr* call_args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(call_args, Sema_lower_expr(self, idx_inner));
            /* pass */
            _tr_str_release(obj_ty_n);
            _tr_str_release(obj_name);
            _tr_str_release(generic_arg_n);
            return box_hirexpr(HirExpr_ctor_EMethodCall(hexpr_obj, _tr_str_lit("get_index"), call_args, AstType_init(_tr_str_lit("void"))));
        }
        /* pass */
        AstType* elem_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if (((strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Vec"))) == 0))) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if ((strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
            /* pass */
            if ((hir_expr_type(hexpr_obj)->args->len > 0LL)) {
                /* pass */
                elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(hexpr_obj)->args, 0LL)));
            }
        } else if ((strcmp(_tr_strz(obj_ty_n), _tr_strz(_tr_str_lit("str"))) == 0)) {
            /* pass */
            elem_ty = AstType_init(_tr_str_lit("char"));
        }
        /* pass */
        _tr_str_release(obj_ty_n);
        _tr_str_release(obj_name);
        _tr_str_release(generic_arg_n);
        return box_hirexpr(HirExpr_ctor_EIndex(hexpr_obj, Sema_lower_expr(self, idx_inner), elem_ty));
    } else if (_t570.tag == Expr_ESizeOf) {
        __auto_type ty = _t570.data.ESizeOf.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return box_hirexpr(HirExpr_ctor_ESizeOf(AstType_init(_tr_str_lit("void")), AstType_init(_tr_str_lit("int"))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESizeOf((*ty), AstType_init(_tr_str_lit("int"))));
    } else if (_t570.tag == Expr_ECast) {
        __auto_type expr = _t570.data.ECast.expr;
__auto_type ty = _t570.data.ECast.ty;
        /* pass */
        if ((((unsigned long long)(ty)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return Sema_lower_expr(self, expr);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ECast(Sema_lower_expr(self, expr), (*ty)));
    } else if (_t570.tag == Expr_EFString) {
        __auto_type parts = _t570.data.EFString.parts;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit("an f-string"), _tr_str_lit("build output in a fixed byte buffer, or print the parts separately; plain string literals are fine (they live in rodata)"));
        /* pass */
        List_ptr* hparts = (void*)List_ptr_new();
        /* pass */
        long long m = 0LL;
        /* pass */
        while ((m < parts->len)) {
            /* pass */
            FStringPart* p_ast = ((FStringPart*)List_ptr_get(parts, m));
            /* pass */
            HirFStringPart* hp = HirFStringPart_init();
            /* pass */
            hp->is_expr = p_ast->is_expr;
            /* pass */
            hp->text = _tr_str_retain(p_ast->text);
            /* pass */
            hp->fmt_spec = _tr_str_retain(p_ast->fmt_spec);
            /* pass */
            hp->expr = Sema_lower_expr(self, p_ast->expr);
            /* pass */
            List_ptr_append(hparts, _tr_obj_retain(hp));
            /* pass */
            m = (m + 1LL);
            _tr_obj_release(hp, _trdrop_HirFStringPart);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EFString(hparts, AstType_init(_tr_str_lit("str"))));
    } else if (_t570.tag == Expr_ETuple) {
        __auto_type items = _t570.data.ETuple.items;
        /* pass */
        if ((self->no_heap && self->heap_boxes_tuples)) {
            /* pass */
            Sema_check_no_heap(self, _tr_str_lit("a tuple literal (heap-boxed on the LLVM/native backend)"), _tr_str_lit("use the C backend (stack tuples) for --no-heap, or return values via out-params / a @value_type struct"));
        }
        /* pass */
        List_ptr* hitems = (void*)List_ptr_new();
        /* pass */
        long long n = 0LL;
        /* pass */
        while ((n < items->len)) {
            /* pass */
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
            /* pass */
            n = (n + 1LL);
        }
        /* pass */
        AstType* tup_ty = AstType_init(_tr_str_lit("Tuple"));
        /* pass */
        long long m = 0LL;
        /* pass */
        while ((m < hitems->len)) {
            /* pass */
            List_ptr_append(tup_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, m)))));
            /* pass */
            m = (m + 1LL);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ETuple(hitems, tup_ty));
    } else if (_t570.tag == Expr_EList) {
        __auto_type items = _t570.data.EList.items;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit("a list literal []"), _tr_str_lit("use a fixed-size array [T; N] (stack, zero-heap) instead"));
        /* pass */
        List_ptr* hitems = (void*)List_ptr_new();
        /* pass */
        long long n = 0LL;
        /* pass */
        while ((n < items->len)) {
            /* pass */
            List_ptr_append(hitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, n))));
            /* pass */
            n = (n + 1LL);
        }
        /* pass */
        AstType* list_ty = AstType_init(_tr_str_lit("List"));
        /* pass */
        if ((hitems->len > 0LL)) {
            /* pass */
            list_ty->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(list_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(hitems, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EList(hitems, list_ty));
    } else if (_t570.tag == Expr_ESet) {
        __auto_type items = _t570.data.ESet.items;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit("a set literal"), _tr_str_lit("use a fixed-size array [T; N] + linear scan, or a bitset over a fixed word"));
        /* pass */
        List_ptr* sitems = (void*)List_ptr_new();
        /* pass */
        long long sn = 0LL;
        /* pass */
        while ((sn < items->len)) {
            /* pass */
            List_ptr_append(sitems, Sema_lower_expr(self, ((Expr*)List_ptr_get(items, sn))));
            /* pass */
            sn = (sn + 1LL);
        }
        /* pass */
        AstType* set_ty = AstType_init(_tr_str_lit("Set"));
        /* pass */
        if ((sitems->len > 0LL)) {
            /* pass */
            set_ty->args = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(set_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(sitems, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESet(sitems, set_ty));
    } else if (_t570.tag == Expr_EClosure) {
        __auto_type params = _t570.data.EClosure.params;
__auto_type ret_ty = _t570.data.EClosure.ret_ty;
__auto_type body = _t570.data.EClosure.body;
__auto_type is_async = _t570.data.EClosure.is_async;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        long long cap_idx = (self->scopes->len - 1LL);
        /* pass */
        List_ptr* hparams = (void*)List_ptr_new();
        /* pass */
        long long l = 0LL;
        /* pass */
        while ((l < params->len)) {
            /* pass */
            Param* pa = ((Param*)List_ptr_get(params, l));
            /* pass */
            AstType* pa_ty = AstType_init(_tr_str_lit("int"));
            /* pass */
            if ((((unsigned long long)(pa->ty)) != ((unsigned long long)(0LL)))) {
                /* pass */
                pa_ty = (*pa->ty);
            }
            /* pass */
            Sema_declare(self, pa->name, SymbolKind_make_SVariable(), box_asttype(pa_ty), false);
            /* pass */
            HirParam* hpa = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
            /* pass */
            hpa->name = _tr_str_retain(pa->name);
            /* pass */
            hpa->ty = pa_ty;
            /* pass */
            List_ptr_append(hparams, _tr_obj_retain(hpa));
            /* pass */
            l = (l + 1LL);
            _tr_obj_release(hpa, _trdrop_HirParam);
        }
        /* pass */
        AstType* r_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
            /* pass */
            r_ty = (*ret_ty);
        }
        /* pass */
        List_i64_append(self->fn_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* clo_body = Sema_lower_block(self, body);
        /* pass */
        List_i64_pop(self->fn_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, clo_body);
        /* pass */
        List_TrStr* clo_refs = (void*)List_TrStr_new();
        /* pass */
        Sema_collect_block_refs(self, clo_body, clo_refs);
        /* pass */
        List_ptr* clo_caps = (void*)List_ptr_new();
        /* pass */
        TrMap* clo_seen = _tr_dict_new(8LL);
        /* pass */
        long long cri = 0LL;
        /* pass */
        while ((cri < clo_refs->len)) {
            /* pass */
            TrStr rn = List_TrStr_get(clo_refs, cri);
            /* pass */
            if ((!_tr_dict_contains(clo_seen, _tr_strz(rn)))) {
                /* pass */
                _tr_dict_set(clo_seen, _tr_strz(rn), true);
                /* pass */
                if ((!_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, cap_idx))->variables, _tr_strz(rn)))) {
                    /* pass */
                    long long si = (cap_idx - 1LL);
                    /* pass */
                    AstType** cty_p = (AstType**)(0LL);
                    /* pass */
                    while ((si >= 0LL)) {
                        /* pass */
                        if (_tr_dict_contains(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn))) {
                            /* pass */
                            cty_p = ((Symbol*)(uintptr_t)_tr_dict_get(((Scope*)List_ptr_get(self->scopes, si))->variables, _tr_strz(rn)))->ty;
                            /* pass */
                            si = (0LL - 1LL);
                        } else {
                            /* pass */
                            si = (si - 1LL);
                        }
                    }
                    /* pass */
                    if ((((unsigned long long)(cty_p)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        HirParam* ccp = ((HirParam*)_tr_obj_alloc(sizeof(HirParam)));
                        /* pass */
                        ccp->name = _tr_str_retain(rn);
                        /* pass */
                        ccp->ty = (*cty_p);
                        /* pass */
                        List_ptr_append(clo_caps, _tr_obj_retain(ccp));
                    }
                }
            }
            /* pass */
            cri = (cri + 1LL);
            _tr_str_release(rn);
        }
        /* pass */
        HirExpr hexpr = HirExpr_ctor_EClosure(hparams, r_ty, clo_body, is_async, clo_caps);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        _tr_obj_release(clo_body, _trdrop_HirBlock);
        Dict_free(clo_seen);
        return box_hirexpr(hexpr);
    } else if (_t570.tag == Expr_EIfElse) {
        __auto_type cond = _t570.data.EIfElse.cond;
__auto_type then_e = _t570.data.EIfElse.then_expr;
__auto_type else_e = _t570.data.EIfElse.else_expr;
        /* pass */
        HirExpr* hcond = Sema_lower_expr(self, cond);
        /* pass */
        HirExpr* hthen = Sema_lower_expr(self, then_e);
        /* pass */
        HirExpr* helse = Sema_lower_expr(self, else_e);
        /* pass */
        AstType* ite_ty = hir_expr_type(hthen);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EIfElse(hcond, hthen, helse, ite_ty));
    } else if (_t570.tag == Expr_EDo) {
        __auto_type do_body = _t570.data.EDo.body;
        /* pass */
        return Sema_lower_do_value(self, do_body);
    } else if (_t570.tag == Expr_EMatch) {
        __auto_type m_subj = _t570.data.EMatch.subj;
__auto_type m_arms = _t570.data.EMatch.arms;
        /* pass */
        HirExpr* hm_subj = Sema_lower_expr(self, m_subj);
        /* pass */
        AstType* hm_subj_ty = hir_expr_type(hm_subj);
        /* pass */
        List_ptr* hm_arms = (void*)List_ptr_new();
        /* pass */
        AstType* hm_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        long long hm_k = 0LL;
        /* pass */
        while ((hm_k < m_arms->len)) {
            /* pass */
            MatchArm* hm_arm = ((MatchArm*)List_ptr_get(m_arms, hm_k));
            /* pass */
            Sema_enter_scope(self);
            /* pass */
            Sema_declare_pattern_binds_typed(self, hm_arm->pat, hm_subj_ty);
            /* pass */
            HirExpr* hm_edo = Sema_lower_do_value(self, (*hm_arm->body));
            /* pass */
            if ((strcmp(_tr_strz(hir_expr_type(hm_edo)->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
                /* pass */
                hm_ty = hir_expr_type(hm_edo);
            }
            /* pass */
            HirBlock* hm_body = HirBlock_init();
            /* pass */
            HirBlock_push(hm_body, box_hirstmt(HirStmt_ctor_SExpr(hm_edo)));
            /* pass */
            HirMatchArm* h_marm = HirMatchArm_init(hm_arm->pat, hm_body);
            /* pass */
            if ((((unsigned long long)(hm_arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                h_marm->guard = Sema_lower_expr(self, hm_arm->guard);
            }
            /* pass */
            List_ptr_append(hm_arms, _tr_obj_retain(h_marm));
            /* pass */
            Sema_exit_scope(self);
            /* pass */
            hm_k = (hm_k + 1LL);
            _tr_obj_release(hm_body, _trdrop_HirBlock);
            _tr_obj_release(h_marm, _trdrop_HirMatchArm);
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EMatchExpr(hm_subj, hm_arms, hm_ty));
    } else if (_t570.tag == Expr_ELoop) {
        __auto_type loop_body = _t570.data.ELoop.body;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* h_loop_body = Sema_lower_block(self, loop_body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, h_loop_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        AstType* loop_ty = Sema_infer_break_type(self, h_loop_body);
        /* pass */
        return box_hirexpr(HirExpr_ctor_ELoop(h_loop_body, loop_ty));
    } else if (_t570.tag == Expr_EWhileExpr) {
        __auto_type we_cond = _t570.data.EWhileExpr.cond;
__auto_type we_body = _t570.data.EWhileExpr.body;
__auto_type we_else = _t570.data.EWhileExpr.else_body;
        /* pass */
        HirExpr* h_we_cond = Sema_lower_expr(self, we_cond);
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_i64_append(self->loop_scope_base, (self->scopes->len - 1LL));
        /* pass */
        HirBlock* h_we_body = Sema_lower_block(self, we_body);
        /* pass */
        List_i64_pop(self->loop_scope_base);
        /* pass */
        Sema_finalize_scope_drops(self, h_we_body);
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        HirExpr* h_we_else = Sema_lower_do_value(self, we_else);
        /* pass */
        AstType* we_ty = Sema_infer_break_type(self, h_we_body);
        /* pass */
        if ((strcmp(_tr_strz(we_ty->name), _tr_strz(_tr_str_lit("void"))) == 0)) {
            /* pass */
            we_ty = hir_expr_type(h_we_else);
        }
        /* pass */
        HirBlock* h_we_else_b = HirBlock_init();
        /* pass */
        HirBlock_push(h_we_else_b, box_hirstmt(HirStmt_ctor_SExpr(h_we_else)));
        /* pass */
        return box_hirexpr(HirExpr_ctor_EWhileExpr(h_we_cond, h_we_body, h_we_else_b, we_ty));
    } else if (_t570.tag == Expr_ETryExpr) {
        __auto_type inner = _t570.data.ETryExpr.expr;
        /* pass */
        HirExpr* hinner = Sema_lower_expr(self, inner);
        /* pass */
        AstType* inner_ty = hir_expr_type(hinner);
        /* pass */
        AstType* ok_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if (((strcmp(_tr_strz(inner_ty->name), _tr_strz(_tr_str_lit("Result"))) == 0) && (inner_ty->args->len > 0LL))) {
            /* pass */
            ok_ty = (*((AstType**)List_ptr_get(inner_ty->args, 0LL)));
        } else if ((strcmp(_tr_strz(inner_ty->name), _tr_strz(_tr_str_lit("void"))) != 0)) {
            /* pass */
            ok_ty = inner_ty;
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ETryExpr(hinner, ok_ty));
    } else if (_t570.tag == Expr_EAwait) {
        __auto_type inner_await = _t570.data.EAwait.expr;
        /* pass */
        if ((!self->in_async_fn)) {
            /* pass */
            ({ TrStr _at_t651 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("[C-4] 'await' used outside an async function. FIX: Declare '")), _tr_strz(self->current_func_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' as 'async def "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->current_func_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(...)' to use await inside it."))); _tr_str_release(_cl); _cres; })); Sema_error(self, _at_t651); _tr_str_release(_at_t651); });
        }
        /* pass */
        HirExpr* hinner_await = Sema_lower_expr(self, inner_await);
        /* pass */
        AstType* await_ty = hir_expr_type(hinner_await);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EAwait(hinner_await, await_ty));
    } else if (_t570.tag == Expr_EDict) {
        __auto_type keys = _t570.data.EDict.keys;
__auto_type vals = _t570.data.EDict.vals;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit("a dict/map literal"), _tr_str_lit("use parallel fixed-size arrays [K; N]/[V; N], or a static lookup table"));
        /* pass */
        List_ptr* h_keys = (void*)List_ptr_new();
        /* pass */
        List_ptr* h_vals = (void*)List_ptr_new();
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < keys->len)) {
            /* pass */
            List_ptr_append(h_keys, Sema_lower_expr(self, ((Expr*)List_ptr_get(keys, di))));
            /* pass */
            List_ptr_append(h_vals, Sema_lower_expr(self, ((Expr*)List_ptr_get(vals, di))));
            /* pass */
            di = (di + 1LL);
        }
        /* pass */
        AstType* dict_ty = AstType_init(_tr_str_lit("Dict"));
        /* pass */
        if ((h_keys->len > 0LL)) {
            /* pass */
            List_ptr_append(dict_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_keys, 0LL)))));
            /* pass */
            List_ptr_append(dict_ty->args, box_asttype(hir_expr_type(((HirExpr*)List_ptr_get(h_vals, 0LL)))));
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_EDict(h_keys, h_vals, dict_ty));
    } else if (_t570.tag == Expr_EListComp) {
        __auto_type element = _t570.data.EListComp.element;
__auto_type generators = _t570.data.EListComp.generators;
        /* pass */
        Sema_check_no_heap(self, _tr_str_lit("a list comprehension"), _tr_str_lit("loop over a fixed-size array [T; N] and write into another"));
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* hgens = (void*)List_ptr_new();
        /* pass */
        long long gc = 0LL;
        /* pass */
        while ((gc < generators->len)) {
            /* pass */
            Comprehension* gen_ast = (*((Comprehension**)List_ptr_get(generators, gc)));
            /* pass */
            HirComprehension** hgen_ptr = (HirComprehension**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            hgen_ptr = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val->target = _tr_str_retain(gen_ast->target);
            /* pass */
            HirExpr* h_iter_lc = Sema_lower_expr(self, gen_ast->iter);
            /* pass */
            hgen_val->iter = h_iter_lc;
            /* pass */
            TrStr lc_itn = _tr_str_retain(hir_expr_type(h_iter_lc)->name);
            /* pass */
            long long lc_ial = hir_expr_type(h_iter_lc)->args->len;
            /* pass */
            AstType* lc_elem_ty = AstType_init(_tr_str_lit("int"));
            /* pass */
            if ((((strcmp(_tr_strz(lc_itn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(lc_itn), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (lc_ial > 0LL))) {
                /* pass */
                lc_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_lc)->args, 0LL)));
            } else if ((strcmp(_tr_strz(lc_itn), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                lc_elem_ty = AstType_init(_tr_str_lit("char"));
            }
            /* pass */
            Sema_declare(self, gen_ast->target, SymbolKind_make_SVariable(), box_asttype(lc_elem_ty), false);
            /* pass */
            List_ptr* hifs = (void*)List_ptr_new();
            /* pass */
            long long lc_fi = 0LL;
            /* pass */
            while ((lc_fi < gen_ast->ifs->len)) {
                /* pass */
                List_ptr_append(hifs, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast->ifs, lc_fi))));
                /* pass */
                lc_fi = (lc_fi + 1LL);
            }
            /* pass */
            hgen_val->ifs = hifs;
            /* pass */
            hgen_val->is_async = gen_ast->is_async;
            /* pass */
            /* unsafe block */
            /* pass */
            (*hgen_ptr = hgen_val);
            /* pass */
            List_ptr_append(hgens, hgen_ptr);
            /* pass */
            gc = (gc + 1LL);
            _tr_str_release(lc_itn);
        }
        /* pass */
        HirExpr* h_lc_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* lc_elem_hty = hir_expr_type(h_lc_elem);
        /* pass */
        AstType* comp_ty = AstType_init(_tr_str_lit("List"));
        /* pass */
        comp_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(comp_ty->args, box_asttype(lc_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EListComp(h_lc_elem, hgens, comp_ty));
    } else if (_t570.tag == Expr_EGeneratorExpr) {
        __auto_type element = _t570.data.EGeneratorExpr.element;
__auto_type generators = _t570.data.EGeneratorExpr.generators;
        /* pass */
        Sema_enter_scope(self);
        /* pass */
        List_ptr* hgens2 = (void*)List_ptr_new();
        /* pass */
        long long gc2 = 0LL;
        /* pass */
        while ((gc2 < generators->len)) {
            /* pass */
            Comprehension* gen_ast2 = (*((Comprehension**)List_ptr_get(generators, gc2)));
            /* pass */
            HirComprehension** hgen_ptr2 = (HirComprehension**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            hgen_ptr2 = ((HirComprehension**)_tr_c_calloc((size_t)(1LL), sizeof(HirComprehension*)));
            /* pass */
            HirComprehension* hgen_val2 = ((HirComprehension*)_tr_obj_alloc(sizeof(HirComprehension)));
            /* pass */
            hgen_val2->target = _tr_str_retain(gen_ast2->target);
            /* pass */
            HirExpr* h_iter_ge = Sema_lower_expr(self, gen_ast2->iter);
            /* pass */
            hgen_val2->iter = h_iter_ge;
            /* pass */
            TrStr ge_itn = _tr_str_retain(hir_expr_type(h_iter_ge)->name);
            /* pass */
            long long ge_ial = hir_expr_type(h_iter_ge)->args->len;
            /* pass */
            AstType* ge_elem_ty = AstType_init(_tr_str_lit("int"));
            /* pass */
            if ((((strcmp(_tr_strz(ge_itn), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(ge_itn), _tr_strz(_tr_str_lit("Vec"))) == 0)) && (ge_ial > 0LL))) {
                /* pass */
                ge_elem_ty = (*((AstType**)List_ptr_get(hir_expr_type(h_iter_ge)->args, 0LL)));
            } else if ((strcmp(_tr_strz(ge_itn), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                ge_elem_ty = AstType_init(_tr_str_lit("char"));
            }
            /* pass */
            Sema_declare(self, gen_ast2->target, SymbolKind_make_SVariable(), box_asttype(ge_elem_ty), false);
            /* pass */
            List_ptr* hifs2 = (void*)List_ptr_new();
            /* pass */
            long long ge_fi = 0LL;
            /* pass */
            while ((ge_fi < gen_ast2->ifs->len)) {
                /* pass */
                List_ptr_append(hifs2, Sema_lower_expr(self, ((Expr*)List_ptr_get(gen_ast2->ifs, ge_fi))));
                /* pass */
                ge_fi = (ge_fi + 1LL);
            }
            /* pass */
            hgen_val2->ifs = hifs2;
            /* pass */
            hgen_val2->is_async = gen_ast2->is_async;
            /* pass */
            /* unsafe block */
            /* pass */
            (*hgen_ptr2 = hgen_val2);
            /* pass */
            List_ptr_append(hgens2, hgen_ptr2);
            /* pass */
            gc2 = (gc2 + 1LL);
            _tr_str_release(ge_itn);
        }
        /* pass */
        HirExpr* h_ge_elem = Sema_lower_expr(self, element);
        /* pass */
        AstType* ge_elem_hty = hir_expr_type(h_ge_elem);
        /* pass */
        AstType* gen_ty = AstType_init(_tr_str_lit("List"));
        /* pass */
        gen_ty->args = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(gen_ty->args, box_asttype(ge_elem_hty));
        /* pass */
        Sema_exit_scope(self);
        /* pass */
        return box_hirexpr(HirExpr_ctor_EGeneratorExpr(h_ge_elem, hgens2, gen_ty));
    } else if (_t570.tag == Expr_ESuperMethodCall) {
        __auto_type base_class = _t570.data.ESuperMethodCall.base_class;
__auto_type method = _t570.data.ESuperMethodCall.method;
__auto_type args = _t570.data.ESuperMethodCall.args;
        /* pass */
        List_ptr* h_super_args = (void*)List_ptr_new();
        /* pass */
        long long k_smc = 0LL;
        /* pass */
        while ((k_smc < args->len)) {
            /* pass */
            List_ptr_append(h_super_args, Sema_lower_expr(self, ((Expr*)List_ptr_get(args, k_smc))));
            /* pass */
            k_smc = (k_smc + 1LL);
        }
        /* pass */
        TrStr resolved_base = _tr_str_retain(base_class);
        /* pass */
        if (((strcmp(_tr_strz(resolved_base), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                /* pass */
                ClassDef* cur_cls = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                /* pass */
                if ((cur_cls->base_classes->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t652 = List_TrStr_get(cur_cls->base_classes, 0LL);
                    _tr_str_release(resolved_base);
                    resolved_base = _strtmp_t652;
                }
            }
        }
        /* pass */
        AstType* super_ret_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base))) {
            /* pass */
            ClassDef* bc_def = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base)));
            /* pass */
            long long k_smc2 = 0LL;
            /* pass */
            while ((k_smc2 < bc_def->methods->len)) {
                /* pass */
                if ((strcmp(_tr_strz(((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->name), _tr_strz(method)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        super_ret_ty = (*((FunctionDef*)List_ptr_get(bc_def->methods, k_smc2))->ret_ty);
                    }
                }
                /* pass */
                k_smc2 = (k_smc2 + 1LL);
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESuperMethodCall(resolved_base, method, h_super_args, super_ret_ty));
    } else if (_t570.tag == Expr_ESuperPropAccess) {
        __auto_type base_class = _t570.data.ESuperPropAccess.base_class;
__auto_type prop = _t570.data.ESuperPropAccess.prop;
        /* pass */
        TrStr resolved_base2 = _tr_str_retain(base_class);
        /* pass */
        if (((strcmp(_tr_strz(resolved_base2), _tr_strz(_tr_str_lit(""))) == 0) && (strcmp(_tr_strz(self->current_class_name), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            if (_tr_dict_contains(self->classes, _tr_strz(self->current_class_name))) {
                /* pass */
                ClassDef* cur_cls2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(self->current_class_name)));
                /* pass */
                if ((cur_cls2->base_classes->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t653 = List_TrStr_get(cur_cls2->base_classes, 0LL);
                    _tr_str_release(resolved_base2);
                    resolved_base2 = _strtmp_t653;
                }
            }
        }
        /* pass */
        AstType* super_field_ty = AstType_init(_tr_str_lit("void"));
        /* pass */
        if (_tr_dict_contains(self->classes, _tr_strz(resolved_base2))) {
            /* pass */
            ClassDef* bc_def2 = ((ClassDef*)(uintptr_t)_tr_dict_get(self->classes, _tr_strz(resolved_base2)));
            /* pass */
            long long k_spa = 0LL;
            /* pass */
            while ((k_spa < bc_def2->fields->len)) {
                /* pass */
                if ((strcmp(_tr_strz(((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->name), _tr_strz(prop)) == 0)) {
                    /* pass */
                    if ((((unsigned long long)(((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty)) != ((unsigned long long)(0LL)))) {
                        /* pass */
                        super_field_ty = (*((FieldDef*)List_ptr_get(bc_def2->fields, k_spa))->ty);
                    }
                }
                /* pass */
                k_spa = (k_spa + 1LL);
            }
        }
        /* pass */
        return box_hirexpr(HirExpr_ctor_ESuperPropAccess(resolved_base2, prop, super_field_ty));
    } else if (1) {
        __auto_type _ = _t570;
        return box_hirexpr(HirExpr_ctor_ELitNone(AstType_init(_tr_str_lit("None"))));
    }
}

__attribute__((hot)) TrStr Sema_is_reserved_error(Sema* self, TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("true"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("True"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in boolean constant 'true'");
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("false"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("False"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in boolean constant 'false'");
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("none"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in null constant 'none'");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Some"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in Option constructor 'Some'");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Ok"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in Result constructor 'Ok'");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Err"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in Result constructor 'Err'");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr Sema_is_reserved_keyword(Sema* self, TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("print"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("input"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in I/O function");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("range"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in range function");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("len"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in len function");
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("type"))) == 0)) {
        /* pass */
        return _tr_str_lit("built-in type function");
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("abs"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("max"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("min"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("sum"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in math function");
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("int"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("bool"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in primitive type");
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("List"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Dict"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in container type");
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Option"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Result"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in enum type");
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Exception"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Error"))) == 0))) {
        /* pass */
        return _tr_str_lit("built-in exception type");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool Sema_block_returns(Sema* self, Block* b) {
    /* pass */
    if ((b->stmts->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long last_idx = (b->stmts->len - 1LL);
    /* pass */
    bool sl_going = true;
    /* pass */
    while ((last_idx > 0LL)) {
        /* pass */
        if ((!sl_going)) {
            /* pass */
            break;
        }
        /* pass */
        Stmt* sl_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
        /* pass */
        if ((((unsigned long long)(sl_s)) == ((unsigned long long)(0LL)))) {
            /* pass */
            last_idx = (last_idx - 1LL);
        } else {
            /* pass */
            bool is_sline = false;
            /* pass */
            __auto_type _t654 = (*sl_s);
            if (_t654.tag == Stmt_SLine) {
                __auto_type _ = _t654.data.SLine.n;
                is_sline = true;
            } else if (1) {
                __auto_type _ = _t654;
                /* pass */
            }
            /* pass */
            if (is_sline) {
                /* pass */
                last_idx = (last_idx - 1LL);
            } else {
                /* pass */
                sl_going = false;
            }
        }
    }
    /* pass */
    Stmt* last_s = ((Stmt*)List_ptr_get(b->stmts, last_idx));
    /* pass */
    if ((((unsigned long long)(last_s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t655 = (*last_s);
    if (_t655.tag == Stmt_SReturn) {
        __auto_type _ = _t655.data.SReturn.val;
        return true;
    } else if (_t655.tag == Stmt_SRaise) {
        __auto_type _ = _t655.data.SRaise.val;
        return true;
    } else if (_t655.tag == Stmt_SUnsafe) {
        __auto_type body = _t655.data.SUnsafe.body;
        return Sema_block_returns(self, body);
    } else if (_t655.tag == Stmt_SIf) {
        __auto_type cond = _t655.data.SIf.cond;
__auto_type then_b = _t655.data.SIf.then_b;
__auto_type elifs = _t655.data.SIf.elifs;
__auto_type else_b = _t655.data.SIf.else_b;
        /* pass */
        if ((else_b->stmts->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        return (Sema_block_returns(self, then_b) && Sema_block_returns(self, else_b));
    } else if (_t655.tag == Stmt_SMatch) {
        __auto_type subj = _t655.data.SMatch.expr;
__auto_type arms = _t655.data.SMatch.arms;
        /* pass */
        bool has_wild = false;
        /* pass */
        bool all_ret = true;
        /* pass */
        List_TrStr* _fr_tnames = (void*)List_TrStr_new();
        /* pass */
        List_TrStr* _fr_vnames = (void*)List_TrStr_new();
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, mi));
            /* pass */
            __auto_type _t656 = arm->pat;
            if (_t656.tag == Pattern_PWild) {
                has_wild = true;
            } else if (_t656.tag == Pattern_PBind) {
                __auto_type _ = _t656.data.PBind.name;
                has_wild = true;
            } else if (1) {
                __auto_type _ = _t656;
                Sema_pattern_covers(self, arm->pat, _fr_tnames, _fr_vnames);
            }
            /* pass */
            if ((((unsigned long long)(arm->body)) != ((unsigned long long)(0LL)))) {
                /* pass */
                if ((!Sema_block_returns(self, (*arm->body)))) {
                    /* pass */
                    all_ret = false;
                }
            }
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        if (has_wild) {
            /* pass */
            return all_ret;
        }
        /* pass */
        if ((!all_ret)) {
            /* pass */
            return false;
        }
        /* pass */
        if ((_fr_vnames->len == 0LL)) {
            /* pass */
            return false;
        }
        /* pass */
        TrStr _fr_ety = List_TrStr_get(_fr_tnames, 0LL);
        /* pass */
        bool _fr_same_ty = true;
        /* pass */
        long long _fr_ti = 1LL;
        /* pass */
        while ((_fr_ti < _fr_tnames->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(_fr_tnames, _fr_ti)), _tr_strz(_fr_ety)) != 0)) {
                /* pass */
                _fr_same_ty = false;
            }
            /* pass */
            _fr_ti = (_fr_ti + 1LL);
        }
        /* pass */
        if ((!_fr_same_ty)) {
            /* pass */
            _tr_str_release(_fr_ety);
            return false;
        }
        /* pass */
        if ((!_tr_dict_contains(self->enums, _tr_strz(_fr_ety)))) {
            /* pass */
            _tr_str_release(_fr_ety);
            return false;
        }
        /* pass */
        EnumDef* _fr_edef = ((EnumDef*)(uintptr_t)_tr_dict_get(self->enums, _tr_strz(_fr_ety)));
        /* pass */
        long long _fr_ei = 0LL;
        /* pass */
        while ((_fr_ei < _fr_edef->variants->len)) {
            /* pass */
            TrStr _fr_vn = _tr_str_retain(((VariantDef*)List_ptr_get(_fr_edef->variants, _fr_ei))->name);
            /* pass */
            bool _fr_found = false;
            /* pass */
            long long _fr_vi = 0LL;
            /* pass */
            while ((_fr_vi < _fr_vnames->len)) {
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(_fr_vnames, _fr_vi)), _tr_strz(_fr_vn)) == 0)) {
                    /* pass */
                    _fr_found = true;
                }
                /* pass */
                _fr_vi = (_fr_vi + 1LL);
            }
            /* pass */
            if ((!_fr_found)) {
                /* pass */
                _tr_str_release(_fr_ety);
                _tr_str_release(_fr_vn);
                return false;
            }
            /* pass */
            _fr_ei = (_fr_ei + 1LL);
            _tr_str_release(_fr_vn);
        }
        /* pass */
        _tr_str_release(_fr_ety);
        return true;
    } else if (1) {
        __auto_type _ = _t655;
        return false;
    }
}

__attribute__((hot)) void Sema_pattern_covers(Sema* self, Pattern p, List_TrStr* tnames, List_TrStr* vnames) {
    /* pass */
    __auto_type _t657 = p;
    if (_t657.tag == Pattern_PVariant) {
        __auto_type tn = _t657.data.PVariant.type_name;
__auto_type vn = _t657.data.PVariant.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t657.tag == Pattern_PVariantBind) {
        __auto_type tn = _t657.data.PVariantBind.type_name;
__auto_type vn = _t657.data.PVariantBind.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t657.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t657.data.PVariantBindMany.type_name;
__auto_type vn = _t657.data.PVariantBindMany.variant;
        /* pass */
        List_TrStr_append(tnames, tn);
        /* pass */
        List_TrStr_append(vnames, vn);
    } else if (_t657.tag == Pattern_POr) {
        __auto_type pats = _t657.data.POr.patterns;
        /* pass */
        long long oi = 0LL;
        /* pass */
        while ((oi < pats->len)) {
            /* pass */
            Sema_pattern_covers(self, List_Pattern_get(pats, oi), tnames, vnames);
            /* pass */
            oi = (oi + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t657;
        /* pass */
    }
}

__attribute__((hot)) bool Sema_is_primitive(Sema* self, AstType* ty) {
    /* pass */
    return Sema_is_primitive_name(self, ty->name);
}

__attribute__((hot)) bool Sema__is_known_type_name(Sema* self, TrStr name) {
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if (Sema_is_primitive_name(self, name)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Str"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("StringObj"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("StringBuilder"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ptr"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("object"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(name)) == 1LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->classes, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (_tr_dict_contains(self->enums, _tr_strz(name))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((unsigned long long)(self->interfaces)) != ((unsigned long long)(0LL))) && _tr_dict_contains(self->interfaces, _tr_strz(name)))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Vec"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("List"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Map"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Dict"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Set"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Box"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Tuple"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("tuple"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Pointer"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Mutex"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Atomic"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Shared"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Weak"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Option"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Result"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Chan"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Channel"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ThreadPool"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ThreadLocal"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Coro"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("OS"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Process"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Env"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Hash"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("File"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Math"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Clock"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Time"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Random"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Hmac"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Json"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("JsonDoc"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("JsonWriter"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("JsonReader"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool Sema_is_primitive_name(Sema* self, TrStr name) {
    /* pass */
    if (((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("void"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("i64"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("i32"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("i16"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("i8"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("u64"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("u16"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("u8"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("usize"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("f64"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("f32"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("lambda"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((_tr_str_starts_with(_tr_strz(name), _tr_strz(_tr_str_lit("c_"))) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("c_void_ptr"))) != 0)) && (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("c_FILE"))) != 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("str"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Str"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("StringObj"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Bytes"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Chan"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Channel"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Mutex"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("RwLock"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Atomic"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Thread"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ThreadPool"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ThreadLocal"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ref"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("mut_ref"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _expr_is_self_field(Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t658 = (*e);
    if (_t658.tag == Expr_EPropAccess) {
        __auto_type obj = _t658.data.EPropAccess.obj;
        /* pass */
        if ((((unsigned long long)(obj)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t659 = (*obj);
        if (_t659.tag == Expr_EIdent) {
            __auto_type nm = _t659.data.EIdent.name;
            return (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("self"))) == 0);
        } else if (1) {
            __auto_type _ = _t659;
            return false;
        }
    } else if (1) {
        __auto_type _ = _t658;
        return false;
    }
}

__attribute__((hot)) bool _binop_is_float_name(TrStr n) {
    /* pass */
    return (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f32"))) == 0));
}

__attribute__((hot)) bool _hl_has_float(List_ptr* hl) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < hl->len)) {
        /* pass */
        if (_binop_is_float_name(hir_expr_type(((HirExpr*)List_ptr_get(hl, i)))->name)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _block_mutates_self(Block* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if (_stmt_mutates_self(((Stmt*)List_ptr_get(b->stmts, i)))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _pblock_mutates_self(Block** pb) {
    /* pass */
    if ((((unsigned long long)(pb)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    return _block_mutates_self((*pb));
}

__attribute__((hot)) bool _stmt_mutates_self(Stmt* s) {
    /* pass */
    if ((((unsigned long long)(s)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t660 = (*s);
    if (_t660.tag == Stmt_SAssign) {
        __auto_type tgt = _t660.data.SAssign.target;
        return _expr_is_self_field(tgt);
    } else if (_t660.tag == Stmt_SIf) {
        __auto_type then_b = _t660.data.SIf.then_b;
__auto_type elifs = _t660.data.SIf.elifs;
__auto_type else_b = _t660.data.SIf.else_b;
        /* pass */
        if (_block_mutates_self(then_b)) {
            /* pass */
            return true;
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < elifs->len)) {
            /* pass */
            if (_pblock_mutates_self(((ElifClause*)List_ptr_get(elifs, i))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _block_mutates_self(else_b);
    } else if (_t660.tag == Stmt_SWhile) {
        __auto_type body = _t660.data.SWhile.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SFor) {
        __auto_type body = _t660.data.SFor.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SForUnpack) {
        __auto_type body = _t660.data.SForUnpack.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SUnsafe) {
        __auto_type body = _t660.data.SUnsafe.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SWith) {
        __auto_type body = _t660.data.SWith.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_STaskGroup) {
        __auto_type body = _t660.data.STaskGroup.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SGpuBlock) {
        __auto_type body = _t660.data.SGpuBlock.body;
        return _block_mutates_self(body);
    } else if (_t660.tag == Stmt_SMatch) {
        __auto_type arms = _t660.data.SMatch.arms;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            if (_pblock_mutates_self(((MatchArm*)List_ptr_get(arms, i))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return false;
    } else if (_t660.tag == Stmt_STry) {
        __auto_type try_body = _t660.data.STry.try_body;
__auto_type catches = _t660.data.STry.catches;
__auto_type finally_b = _t660.data.STry.finally_b;
        /* pass */
        if (_block_mutates_self(try_body)) {
            /* pass */
            return true;
        }
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < catches->len)) {
            /* pass */
            if (_pblock_mutates_self((*((CatchClause**)List_ptr_get(catches, i)))->body)) {
                /* pass */
                return true;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _block_mutates_self(finally_b);
    } else if (_t660.tag == Stmt_SDefer) {
        __auto_type inner = _t660.data.SDefer.stmt;
        return _stmt_mutates_self(inner);
    } else if (1) {
        __auto_type _ = _t660;
        return false;
    }
}

__attribute__((hot)) Symbol** box_symbol(Symbol* s) {
    /* pass */
    /* unsafe block */
    /* pass */
    Symbol** p = ((Symbol**)_tr_c_calloc((size_t)(1LL), sizeof(Symbol*)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

