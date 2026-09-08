#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) LBlock* LBlock_init(long long id) {
    /* pass */
    LBlock* b = ((LBlock*)_tr_obj_alloc(sizeof(LBlock)));
    /* pass */
    b->id = id;
    /* pass */
    b->insts = (void*)List_ptr_new();
    /* pass */
    b->term = LTerm_make_TUnset();
    /* pass */
    return b;
}

__attribute__((malloc,returns_nonnull,hot)) LFunc* LFunc_init(TrStr name) {
    /* pass */
    LFunc* f = ((LFunc*)_tr_obj_alloc(sizeof(LFunc)));
    /* pass */
    f->name = _tr_str_retain(name);
    /* pass */
    f->is_main = false;
    /* pass */
    f->blocks = (void*)List_ptr_new();
    /* pass */
    f->cur = (-1LL);
    /* pass */
    f->n_vregs = 0LL;
    /* pass */
    f->vreg_types = (void*)List_i64_new();
    /* pass */
    f->vars = (void*)List_TrStr_new();
    /* pass */
    f->var_types = (void*)List_i64_new();
    /* pass */
    f->var_cls = (void*)List_TrStr_new();
    /* pass */
    f->var_arr = (void*)List_i64_new();
    /* pass */
    f->params = (void*)List_TrStr_new();
    /* pass */
    f->tmp_ctr = 0LL;
    /* pass */
    f->loop_cont = (void*)List_i64_new();
    /* pass */
    f->loop_brk = (void*)List_i64_new();
    /* pass */
    f->fresh_strs = (void*)List_i64_new();
    /* pass */
    f->fresh_objs = (void*)List_i64_new();
    /* pass */
    f->captures = (void*)List_TrStr_new();
    /* pass */
    f->cap_tags = (void*)List_i64_new();
    /* pass */
    f->var_xret = (void*)List_i64_new();
    /* pass */
    f->vreg_xret = (void*)List_i64_new();
    /* pass */
    f->try_blks = (void*)List_i64_new();
    /* pass */
    f->try_msgs = (void*)List_TrStr_new();
    /* pass */
    f->defers = (void*)List_ptr_new();
    /* pass */
    f->mutex_unlocks = (void*)List_TrStr_new();
    /* pass */
    f->blk_depth = 0LL;
    /* pass */
    f->in_defer = false;
    /* pass */
    f->is_throws = false;
    /* pass */
    f->throws_ok_tag = 0LL;
    /* pass */
    f->throws_err_tag = 0LL;
    /* pass */
    f->in_taskgroup = 0LL;
    /* pass */
    return f;
}

__attribute__((hot)) long long LFunc_fresh_id(LFunc* self) {
    /* pass */
    long long id = self->tmp_ctr;
    /* pass */
    self->tmp_ctr = (self->tmp_ctr + 1LL);
    /* pass */
    return id;
}

__attribute__((hot)) long long LFunc_new_block(LFunc* self) {
    /* pass */
    long long id = self->blocks->len;
    /* pass */
    List_ptr_append(self->blocks, LBlock_init(id));
    /* pass */
    return id;
}

__attribute__((hot)) void LFunc_set_cur(LFunc* self, long long id) {
    /* pass */
    self->cur = id;
}

__attribute__((hot)) void LFunc_emit(LFunc* self, LInst i) {
    /* pass */
    List_ptr_append(((LBlock*)List_ptr_get(self->blocks, self->cur))->insts, box_linst(i));
}

__attribute__((hot)) void LFunc_set_term(LFunc* self, LTerm t) {
    /* pass */
    LBlock* b = ((LBlock*)List_ptr_get(self->blocks, self->cur));
    /* pass */
    __auto_type _t3047 = b->term;
    if (_t3047.tag == LTerm_TUnset) {
        b->term = t;
    } else if (1) {
        __auto_type _ = _t3047;
        /* pass */
    }
}

__attribute__((hot)) bool LFunc_cur_terminated(LFunc* self) {
    /* pass */
    __auto_type _t3048 = ((LBlock*)List_ptr_get(self->blocks, self->cur))->term;
    if (_t3048.tag == LTerm_TUnset) {
        return false;
    } else if (1) {
        __auto_type _ = _t3048;
        return true;
    }
}

__attribute__((hot)) long long LFunc_new_vreg(LFunc* self) {
    /* pass */
    long long id = self->n_vregs;
    /* pass */
    self->n_vregs = (self->n_vregs + 1LL);
    /* pass */
    List_i64_append(self->vreg_types, 0LL);
    /* pass */
    List_i64_append(self->vreg_xret, (-1LL));
    /* pass */
    return id;
}

__attribute__((hot)) void LFunc_set_vreg_type(LFunc* self, long long id, long long t) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
        /* pass */
        List_i64_set(self->vreg_types, id, t);
    }
}

__attribute__((hot)) long long LFunc_vreg_type(LFunc* self, long long id) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_types->len))) {
        /* pass */
        return List_i64_get(self->vreg_types, id);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LFunc_add_var(LFunc* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->vars->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->vars, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->vars, name);
    /* pass */
    List_i64_append(self->var_types, 0LL);
    /* pass */
    List_TrStr_append(self->var_cls, _tr_str_lit(""));
    /* pass */
    List_i64_append(self->var_arr, 0LL);
    /* pass */
    List_i64_append(self->var_xret, (-1LL));
}

__attribute__((hot)) long long LFunc_var_index(LFunc* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->vars->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->vars, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) void LFunc_set_var_type(LFunc* self, TrStr name, long long t) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_i64_set(self->var_types, idx, t);
    }
}

__attribute__((hot)) long long LFunc_var_type(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->var_types, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LFunc_set_var_cls(LFunc* self, TrStr name, TrStr cls) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_TrStr_set(self->var_cls, idx, cls);
    }
}

__attribute__((hot)) TrStr LFunc_var_cls_of(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_TrStr_get(self->var_cls, idx);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) void LFunc_set_var_arr(LFunc* self, TrStr name, long long n) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_i64_set(self->var_arr, idx, n);
    }
}

__attribute__((hot)) long long LFunc_var_arr_of(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->var_arr, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LFunc_set_vreg_xret(LFunc* self, long long id, long long t) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_xret->len))) {
        /* pass */
        List_i64_set(self->vreg_xret, id, t);
    }
}

__attribute__((hot)) long long LFunc_vreg_xret_of(LFunc* self, long long id) {
    /* pass */
    if (((id >= 0LL) && (id < self->vreg_xret->len))) {
        /* pass */
        return List_i64_get(self->vreg_xret, id);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) void LFunc_set_var_xret(LFunc* self, TrStr name, long long t) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        List_i64_set(self->var_xret, idx, t);
    }
}

__attribute__((hot)) long long LFunc_var_xret_of(LFunc* self, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->var_xret, idx);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) long long LFunc_capture_index(LFunc* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->captures->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->captures, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) ClassLayout* ClassLayout_init(TrStr name) {
    /* pass */
    ClassLayout* c = ((ClassLayout*)_tr_obj_alloc(sizeof(ClassLayout)));
    /* pass */
    c->name = _tr_str_retain(name);
    /* pass */
    c->base = _tr_str_lit("");
    /* pass */
    c->fields = (void*)List_TrStr_new();
    /* pass */
    c->ftags = (void*)List_i64_new();
    /* pass */
    c->fcls = (void*)List_TrStr_new();
    /* pass */
    c->is_value = false;
    /* pass */
    return c;
}

__attribute__((hot)) long long ClassLayout_field_index(ClassLayout* self, TrStr fname) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fields->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fields, i)), _tr_strz(fname)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) VariantLayout* VariantLayout_init(TrStr name) {
    /* pass */
    VariantLayout* v = ((VariantLayout*)_tr_obj_alloc(sizeof(VariantLayout)));
    /* pass */
    v->name = _tr_str_retain(name);
    /* pass */
    v->fields = (void*)List_TrStr_new();
    /* pass */
    v->ftags = (void*)List_i64_new();
    /* pass */
    v->fcls = (void*)List_TrStr_new();
    /* pass */
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) EnumLayout* EnumLayout_init(TrStr name) {
    /* pass */
    EnumLayout* e = ((EnumLayout*)_tr_obj_alloc(sizeof(EnumLayout)));
    /* pass */
    e->name = _tr_str_retain(name);
    /* pass */
    e->variants = (void*)List_ptr_new();
    /* pass */
    return e;
}

__attribute__((hot)) long long EnumLayout_variant_index(EnumLayout* self, TrStr vname) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->variants->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((VariantLayout*)List_ptr_get(self->variants, i))->name), _tr_strz(vname)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((malloc,returns_nonnull,hot)) LModule* LModule_init() {
    /* pass */
    LModule* m = ((LModule*)_tr_obj_alloc(sizeof(LModule)));
    /* pass */
    m->funcs = (void*)List_ptr_new();
    /* pass */
    m->classes = (void*)List_ptr_new();
    /* pass */
    m->enums = (void*)List_ptr_new();
    /* pass */
    m->ifaces = (void*)List_TrStr_new();
    /* pass */
    m->externs = (void*)List_TrStr_new();
    /* pass */
    m->fn_names = (void*)List_TrStr_new();
    /* pass */
    m->fn_ret = (void*)List_i64_new();
    /* pass */
    m->strings = (void*)List_TrStr_new();
    /* pass */
    m->globals = (void*)List_TrStr_new();
    /* pass */
    m->global_types = (void*)List_i64_new();
    /* pass */
    m->global_arr = (void*)List_i64_new();
    /* pass */
    m->global_inits = (void*)List_ptr_new();
    /* pass */
    m->ok = true;
    /* pass */
    m->fail_note = _tr_str_lit("");
    /* pass */
    m->unavail_names = (void*)List_TrStr_new();
    /* pass */
    m->unavail_notes = (void*)List_TrStr_new();
    /* pass */
    m->subst_names = (void*)List_TrStr_new();
    /* pass */
    m->subst_tys = (void*)List_ptr_new();
    /* pass */
    m->extfn_names = (void*)List_TrStr_new();
    /* pass */
    m->extfn_ret = (void*)List_i64_new();
    /* pass */
    m->fn_owned_names = (void*)List_TrStr_new();
    /* pass */
    m->overloaded_sigs = (void*)List_TrStr_new();
    /* pass */
    m->target_llvm = false;
    /* pass */
    m->no_heap = false;
    /* pass */
    return m;
}

__attribute__((hot)) void LModule_mark_overloaded(LModule* self, TrStr cls, TrStr method) {
    /* pass */
    TrStr key = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cls), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; });
    /* pass */
    if (LModule_is_overloaded(self, cls, method)) {
        /* pass */
        _tr_str_release(key);
        return;
    }
    /* pass */
    List_TrStr_append(self->overloaded_sigs, key);
    _tr_str_release(key);
}

__attribute__((hot)) bool LModule_is_overloaded(LModule* self, TrStr cls, TrStr method) {
    /* pass */
    TrStr key = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cls), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; });
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->overloaded_sigs->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->overloaded_sigs, i)), _tr_strz(key)) == 0)) {
            /* pass */
            _tr_str_release(key);
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    _tr_str_release(key);
    return false;
}

__attribute__((hot)) TrStr LModule_resolve_method_ov(LModule* self, TrStr cls, TrStr method, long long argc) {
    /* pass */
    TrStr cur = _tr_str_retain(cls);
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((depth < 32LL)) {
        /* pass */
        long long ci = LModule_class_index(self, cur);
        /* pass */
        if ((ci < 0LL)) {
            /* pass */
            _tr_str_release(cur);
            return _tr_str_lit("");
        }
        /* pass */
        if (LModule_is_overloaded(self, cur, method)) {
            /* pass */
            TrStr mm = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argc)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("arg"))); _tr_str_release(_cl); _cres; });
            /* pass */
            if (LModule_is_user_fn(self, mm)) {
                /* pass */
                _tr_str_release(cur);
                return mm;
            }
        }
        /* pass */
        TrStr mangled = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; });
        /* pass */
        if (LModule_is_user_fn(self, mangled)) {
            /* pass */
            _tr_str_release(cur);
            return mangled;
        }
        /* pass */
        TrStr _strtmp_t3049 = _tr_str_retain(((ClassLayout*)List_ptr_get(self->classes, ci))->base);
        _tr_str_release(cur);
        cur = _strtmp_t3049;
        /* pass */
        if (((((unsigned long long)(((char*)(_tr_strz(cur))))) == ((unsigned long long)(0LL))) || (strcmp(_tr_strz(cur), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _tr_str_lit("");
        }
        /* pass */
        depth = (depth + 1LL);
        _tr_str_release(mangled);
    }
    /* pass */
    _tr_str_release(cur);
    return _tr_str_lit("");
}

__attribute__((hot)) long long LModule_add_global(LModule* self, TrStr name, long long tag) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->globals->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->globals, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->globals, name);
    /* pass */
    List_i64_append(self->global_types, tag);
    /* pass */
    List_i64_append(self->global_arr, 0LL);
    /* pass */
    return (self->globals->len - 1LL);
}

__attribute__((hot)) void LModule_set_global_arr(LModule* self, long long idx, long long n) {
    /* pass */
    if (((idx >= 0LL) && (idx < self->global_arr->len))) {
        /* pass */
        List_i64_set(self->global_arr, idx, n);
    }
}

__attribute__((hot)) long long LModule_global_arr_of(LModule* self, TrStr name) {
    /* pass */
    long long idx = LModule_global_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->global_arr, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long LModule_global_index(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->globals->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->globals, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_global(LModule* self, TrStr name) {
    /* pass */
    return (LModule_global_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_global_type(LModule* self, TrStr name) {
    /* pass */
    long long idx = LModule_global_index(self, name);
    /* pass */
    if ((idx >= 0LL)) {
        /* pass */
        return List_i64_get(self->global_types, idx);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long LModule_fn_ret_tag(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return List_i64_get(self->fn_ret, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long LModule_add_string(LModule* self, TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->strings->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->strings, i)), _tr_strz(s)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->strings, s);
    /* pass */
    return (self->strings->len - 1LL);
}

__attribute__((hot)) void LModule_add_extern(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->externs->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->externs, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->externs, name);
}

__attribute__((hot)) bool LModule_is_user_fn(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool LModule_is_extern_fn(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->extfn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->extfn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long LModule_extern_ret_tag(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->extfn_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->extfn_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return List_i64_get(self->extfn_ret, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void LModule_mark_fn_owned(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_owned_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_owned_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->fn_owned_names, name);
}

__attribute__((hot)) bool LModule_fn_ret_owned(LModule* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->fn_owned_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->fn_owned_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long LModule_unavail_index(LModule* self, TrStr name) {
    /* pass */
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->unavail_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->unavail_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) void LModule_add_class(LModule* self, ClassLayout* cl) {
    /* pass */
    List_ptr_append(self->classes, _tr_obj_retain(cl));
}

__attribute__((hot)) long long LModule_class_index(LModule* self, TrStr name) {
    /* pass */
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->classes->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((ClassLayout*)List_ptr_get(self->classes, i))->name), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_class(LModule* self, TrStr name) {
    /* pass */
    return (LModule_class_index(self, name) >= 0LL);
}

__attribute__((hot)) bool LModule_is_value_class(LModule* self, TrStr name) {
    /* pass */
    long long ci = LModule_class_index(self, name);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return ((ClassLayout*)List_ptr_get(self->classes, ci))->is_value;
}

__attribute__((hot)) long long LModule_class_size(LModule* self, TrStr name) {
    /* pass */
    long long ci = LModule_class_index(self, name);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    long long n = ((ClassLayout*)List_ptr_get(self->classes, ci))->fields->len;
    /* pass */
    if ((n < 1LL)) {
        /* pass */
        n = 1LL;
    }
    /* pass */
    return (n * 8LL);
}

__attribute__((hot)) long long LModule_field_offset(LModule* self, TrStr cls, TrStr fld) {
    /* pass */
    long long ci = LModule_class_index(self, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
    /* pass */
    if ((fi < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    return (fi * 8LL);
}

__attribute__((hot)) long long LModule_field_tag(LModule* self, TrStr cls, TrStr fld) {
    /* pass */
    long long ci = LModule_class_index(self, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
    /* pass */
    if ((fi < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    return List_i64_get(((ClassLayout*)List_ptr_get(self->classes, ci))->ftags, fi);
}

__attribute__((hot)) TrStr LModule_resolve_method(LModule* self, TrStr cls, TrStr method) {
    /* pass */
    TrStr cur = _tr_str_retain(cls);
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((depth < 32LL)) {
        /* pass */
        long long ci = LModule_class_index(self, cur);
        /* pass */
        if ((ci < 0LL)) {
            /* pass */
            _tr_str_release(cur);
            return _tr_str_lit("");
        }
        /* pass */
        TrStr mangled = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(method)); _tr_str_release(_cl); _cres; });
        /* pass */
        if (LModule_is_user_fn(self, mangled)) {
            /* pass */
            _tr_str_release(cur);
            return mangled;
        }
        /* pass */
        TrStr _strtmp_t3050 = _tr_str_retain(((ClassLayout*)List_ptr_get(self->classes, ci))->base);
        _tr_str_release(cur);
        cur = _strtmp_t3050;
        /* pass */
        if (((((unsigned long long)(((char*)(_tr_strz(cur))))) == ((unsigned long long)(0LL))) || (strcmp(_tr_strz(cur), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            _tr_str_release(cur);
            _tr_str_release(mangled);
            return _tr_str_lit("");
        }
        /* pass */
        depth = (depth + 1LL);
        _tr_str_release(mangled);
    }
    /* pass */
    _tr_str_release(cur);
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr LModule_field_cls(LModule* self, TrStr cls, TrStr fld) {
    /* pass */
    long long ci = LModule_class_index(self, cls);
    /* pass */
    if ((ci < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    long long fi = ClassLayout_field_index(((ClassLayout*)List_ptr_get(self->classes, ci)), fld);
    /* pass */
    if ((fi < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return List_TrStr_get(((ClassLayout*)List_ptr_get(self->classes, ci))->fcls, fi);
}

__attribute__((hot)) void LModule_add_enum(LModule* self, EnumLayout* el) {
    /* pass */
    List_ptr_append(self->enums, _tr_obj_retain(el));
}

__attribute__((hot)) long long LModule_enum_index(LModule* self, TrStr name) {
    /* pass */
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->enums->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((EnumLayout*)List_ptr_get(self->enums, i))->name), _tr_strz(name)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) bool LModule_is_enum(LModule* self, TrStr name) {
    /* pass */
    return (LModule_enum_index(self, name) >= 0LL);
}

__attribute__((hot)) long long LModule_enum_size(LModule* self, TrStr name) {
    /* pass */
    long long ei = LModule_enum_index(self, name);
    /* pass */
    if ((ei < 0LL)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    long long mx = 0LL;
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < ((EnumLayout*)List_ptr_get(self->enums, ei))->variants->len)) {
        /* pass */
        long long n = ((VariantLayout*)List_ptr_get(((EnumLayout*)List_ptr_get(self->enums, ei))->variants, vi))->fields->len;
        /* pass */
        if ((n > mx)) {
            /* pass */
            mx = n;
        }
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    return ((1LL + mx) * 8LL);
}

__attribute__((hot)) long long LModule_enum_variant_index(LModule* self, TrStr ename, TrStr vname) {
    /* pass */
    long long ei = LModule_enum_index(self, ename);
    /* pass */
    if ((ei < 0LL)) {
        /* pass */
        return (-1LL);
    }
    /* pass */
    return EnumLayout_variant_index(((EnumLayout*)List_ptr_get(self->enums, ei)), vname);
}

__attribute__((hot)) void LModule_add_iface(LModule* self, TrStr name) {
    /* pass */
    List_TrStr_append(self->ifaces, name);
}

__attribute__((hot)) bool LModule_is_iface(LModule* self, TrStr name) {
    /* pass */
    if ((((unsigned long long)(((char*)(_tr_strz(name))))) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->ifaces->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->ifaces, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) LInst* box_linst(LInst i) {
    /* pass */
    /* unsafe block */
    /* pass */
    LInst* p = ((LInst*)_tr_c_calloc((size_t)(1LL), sizeof(LInst)));
    /* pass */
    (*p = i);
    /* pass */
    return p;
}

