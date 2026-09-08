#include "tauraro_types.h"


__attribute__((hot)) GVal* GVal_make(TrStr ty, TrStr val) {
    /* pass */
    GVal* v = ((GVal*)_tr_obj_alloc(sizeof(GVal)));
    /* pass */
    v->ty = _tr_str_retain(ty);
    /* pass */
    v->val = _tr_str_retain(val);
    /* pass */
    return v;
}

__attribute__((malloc,returns_nonnull,hot)) GpuEmitter* GpuEmitter_init(TrStr target) {
    /* pass */
    GpuEmitter* e = ((GpuEmitter*)_tr_obj_alloc(sizeof(GpuEmitter)));
    /* pass */
    e->sb = StringBuilder_init(1024LL);
    /* pass */
    e->target = _tr_str_retain(target);
    /* pass */
    e->tmp = 0LL;
    /* pass */
    e->lbl = 0LL;
    /* pass */
    e->ok = true;
    /* pass */
    e->fail_note = _tr_str_lit("");
    /* pass */
    e->var_names = (void*)List_TrStr_new();
    /* pass */
    e->var_ll = (void*)List_TrStr_new();
    /* pass */
    e->var_elem = (void*)List_TrStr_new();
    /* pass */
    e->ret_llty = _tr_str_lit("void");
    /* pass */
    e->dev_fns = (void*)List_ptr_new();
    /* pass */
    return e;
}

__attribute__((hot)) TrStr GpuEmitter__gpu_ret_ty(GpuEmitter* self, AstType* t) {
    /* pass */
    if ((((strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("None"))) == 0)) || (strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit(""))) == 0))) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((strcmp(_tr_strz(t->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return _tr_str_lit("ptr addrspace(1)");
    }
    /* pass */
    return _gpu_scalar_ty(t->name);
}

__attribute__((hot)) void GpuEmitter_w(GpuEmitter* self, TrStr s) {
    /* pass */
    StringBuilder_append(self->sb, s);
}

__attribute__((hot)) TrStr GpuEmitter_fresh(GpuEmitter* self) {
    /* pass */
    TrStr r = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("%t")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    self->tmp = (self->tmp + 1LL);
    /* pass */
    return r;
}

__attribute__((hot)) TrStr GpuEmitter_newlbl(GpuEmitter* self) {
    /* pass */
    TrStr r = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->lbl)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("L")), _cr.data); _tr_str_release(_cr); _cres; });
    /* pass */
    self->lbl = (self->lbl + 1LL);
    /* pass */
    return r;
}

__attribute__((hot)) void GpuEmitter_fail(GpuEmitter* self, TrStr why) {
    /* pass */
    self->ok = false;
    /* pass */
    if ((strcmp(_tr_strz(self->fail_note), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        self->fail_note = _tr_str_retain(why);
    }
}

__attribute__((hot)) void GpuEmitter_add_var(GpuEmitter* self, TrStr name, AstType* ty) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->var_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if (((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (ty->args->len > 0LL))) {
        /* pass */
        TrStr et = _gpu_scalar_ty((*((AstType**)List_ptr_get(ty->args, 0LL)))->name);
        /* pass */
        List_TrStr_append(self->var_names, name);
        /* pass */
        List_TrStr_append(self->var_ll, _tr_str_lit("ptr addrspace(1)"));
        /* pass */
        List_TrStr_append(self->var_elem, et);
        _tr_str_release(et);
    } else {
        /* pass */
        List_TrStr_append(self->var_names, name);
        /* pass */
        ({ TrStr _at_t3398 = (_gpu_scalar_ty(ty->name)); List_TrStr_append(self->var_ll, _at_t3398); _tr_str_release(_at_t3398); });
        /* pass */
        List_TrStr_append(self->var_elem, _tr_str_lit(""));
    }
}

__attribute__((hot)) TrStr GpuEmitter_var_slot_ty(GpuEmitter* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->var_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return List_TrStr_get(self->var_ll, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("i64");
}

__attribute__((hot)) TrStr GpuEmitter_var_elem_ty(GpuEmitter* self, TrStr name) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->var_names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->var_names, i)), _tr_strz(name)) == 0)) {
            /* pass */
            return List_TrStr_get(self->var_elem, i);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) void GpuEmitter_scan_vars_block(GpuEmitter* self, HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        GpuEmitter_scan_vars_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_scan_vars_stmt(GpuEmitter* self, HirStmt s) {
    /* pass */
    __auto_type _t3399 = s;
    if (_t3399.tag == HirStmt_SLet) {
        __auto_type name = _t3399.data.SLet.name;
__auto_type ownership = _t3399.data.SLet.ownership;
__auto_type is_mut = _t3399.data.SLet.is_mut;
__auto_type is_const = _t3399.data.SLet.is_const;
__auto_type is_shared = _t3399.data.SLet.is_shared;
__auto_type ty = _t3399.data.SLet.ty;
__auto_type val = _t3399.data.SLet.val;
        /* pass */
        AstType* vt = ty;
        /* pass */
        if ((strcmp(_tr_strz(vt->name), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            vt = hir_expr_type(val);
        }
        /* pass */
        GpuEmitter_add_var(self, name, vt);
    } else if (_t3399.tag == HirStmt_SIf) {
        __auto_type cond = _t3399.data.SIf.cond;
__auto_type then_b = _t3399.data.SIf.then_b;
__auto_type else_b = _t3399.data.SIf.else_b;
        /* pass */
        GpuEmitter_scan_vars_block(self, then_b);
        /* pass */
        GpuEmitter_scan_vars_block(self, else_b);
    } else if (_t3399.tag == HirStmt_SWhile) {
        __auto_type cond = _t3399.data.SWhile.cond;
__auto_type body = _t3399.data.SWhile.body;
        /* pass */
        GpuEmitter_scan_vars_block(self, body);
    } else if (_t3399.tag == HirStmt_SUnsafe) {
        __auto_type body = _t3399.data.SUnsafe.body;
        /* pass */
        GpuEmitter_scan_vars_block(self, body);
    } else if (1) {
        __auto_type _ = _t3399;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) TrStr GpuEmitter_emit_kernel(GpuEmitter* self, HirFunction* f) {
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        GpuEmitter_add_var(self, p->name, p->ty);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    GpuEmitter_scan_vars_block(self, f->body);
    /* pass */
    TrStr cc = _tr_str_lit("spir_kernel");
    /* pass */
    if ((strcmp(_tr_strz(self->target), _tr_strz(_tr_str_lit("nvptx"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3400 = _tr_str_lit("ptx_kernel");
        _tr_str_release(cc);
        cc = _strtmp_t3400;
    }
    /* pass */
    ({ TrStr _at_t3401 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("define ")), _tr_strz(cc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" void @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3401); _tr_str_release(_at_t3401); });
    /* pass */
    long long pj = 0LL;
    /* pass */
    while ((pj < f->params->len)) {
        /* pass */
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
        /* pass */
        if ((pj > 0LL)) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit(", "));
        }
        /* pass */
        ({ TrStr _at_t3402 = (({ TrStr _cl = (({ TrStr _cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" %arg_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pp->name)); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3402); _tr_str_release(_at_t3402); });
        /* pass */
        pj = (pj + 1LL);
    }
    /* pass */
    GpuEmitter_w(self, _tr_str_lit(") {\nentry:\n"));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < self->var_names->len)) {
        /* pass */
        ({ TrStr _at_t3403 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(self->var_names, vi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  %var_")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = alloca "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(self->var_ll, vi)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3403); _tr_str_release(_at_t3403); });
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    long long pk = 0LL;
    /* pass */
    while ((pk < f->params->len)) {
        /* pass */
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
        /* pass */
        ({ TrStr _at_t3404 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" %arg_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3404); _tr_str_release(_at_t3404); });
        /* pass */
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    GpuEmitter_emit_block(self, f->body);
    /* pass */
    GpuEmitter_w(self, _tr_str_lit("  ret void\n}\n\n"));
    /* pass */
    if ((!self->ok)) {
        /* pass */
        _tr_str_release(cc);
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("; kernel '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' NOT emitted: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->fail_note)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n\n"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    _tr_str_release(cc);
    return StringObj_as_str(StringBuilder_to_string(self->sb));
}

__attribute__((hot)) TrStr GpuEmitter_emit_device_fn(GpuEmitter* self, HirFunction* f) {
    /* pass */
    self->ret_llty = GpuEmitter__gpu_ret_ty(self, f->ret_ty);
    /* pass */
    long long pi = 0LL;
    /* pass */
    while ((pi < f->params->len)) {
        /* pass */
        HirParam* p = ((HirParam*)List_ptr_get(f->params, pi));
        /* pass */
        GpuEmitter_add_var(self, p->name, p->ty);
        /* pass */
        pi = (pi + 1LL);
    }
    /* pass */
    GpuEmitter_scan_vars_block(self, f->body);
    /* pass */
    ({ TrStr _at_t3405 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("define ")), _tr_strz(self->ret_llty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3405); _tr_str_release(_at_t3405); });
    /* pass */
    long long pj = 0LL;
    /* pass */
    while ((pj < f->params->len)) {
        /* pass */
        HirParam* pp = ((HirParam*)List_ptr_get(f->params, pj));
        /* pass */
        if ((pj > 0LL)) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit(", "));
        }
        /* pass */
        ({ TrStr _at_t3406 = (({ TrStr _cl = (({ TrStr _cl = (GpuEmitter_var_slot_ty(self, pp->name)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" %arg_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pp->name)); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3406); _tr_str_release(_at_t3406); });
        /* pass */
        pj = (pj + 1LL);
    }
    /* pass */
    GpuEmitter_w(self, _tr_str_lit(") {\nentry:\n"));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < self->var_names->len)) {
        /* pass */
        ({ TrStr _at_t3407 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(self->var_names, vi)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  %var_")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = alloca "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(self->var_ll, vi)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3407); _tr_str_release(_at_t3407); });
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    long long pk = 0LL;
    /* pass */
    while ((pk < f->params->len)) {
        /* pass */
        TrStr pn = _tr_str_retain(((HirParam*)List_ptr_get(f->params, pk))->name);
        /* pass */
        ({ TrStr _at_t3408 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (GpuEmitter_var_slot_ty(self, pn)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" %arg_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3408); _tr_str_release(_at_t3408); });
        /* pass */
        pk = (pk + 1LL);
        _tr_str_release(pn);
    }
    /* pass */
    GpuEmitter_emit_block(self, f->body);
    /* pass */
    if ((strcmp(_tr_strz(self->ret_llty), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        GpuEmitter_w(self, _tr_str_lit("  ret void\n}\n\n"));
    } else if (_gpu_is_float(self->ret_llty)) {
        /* pass */
        ({ TrStr _at_t3409 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ")), _tr_strz(self->ret_llty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 0.0\n}\n\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3409); _tr_str_release(_at_t3409); });
    } else if ((strcmp(_tr_strz(self->ret_llty), _tr_strz(_tr_str_lit("ptr addrspace(1)"))) == 0)) {
        /* pass */
        ({ TrStr _at_t3410 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ")), _tr_strz(self->ret_llty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" null\n}\n\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3410); _tr_str_release(_at_t3410); });
    } else {
        /* pass */
        ({ TrStr _at_t3411 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ")), _tr_strz(self->ret_llty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 0\n}\n\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3411); _tr_str_release(_at_t3411); });
    }
    /* pass */
    if ((!self->ok)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("; device fn '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' NOT emitted: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(self->fail_note)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n\n"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->sb));
}

__attribute__((hot)) void GpuEmitter_emit_block(GpuEmitter* self, HirBlock* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        GpuEmitter_emit_stmt(self, (*((HirStmt*)List_ptr_get(b->stmts, i))));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt(GpuEmitter* self, HirStmt s) {
    /* pass */
    __auto_type _t3412 = s;
    if (_t3412.tag == HirStmt_SLet) {
        __auto_type name = _t3412.data.SLet.name;
__auto_type ownership = _t3412.data.SLet.ownership;
__auto_type is_mut = _t3412.data.SLet.is_mut;
__auto_type is_const = _t3412.data.SLet.is_const;
__auto_type is_shared = _t3412.data.SLet.is_shared;
__auto_type ty = _t3412.data.SLet.ty;
__auto_type val = _t3412.data.SLet.val;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, val);
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, slot);
        /* pass */
        ({ TrStr _at_t3413 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(slot))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3413); _tr_str_release(_at_t3413); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (_t3412.tag == HirStmt_SAssign) {
        __auto_type target = _t3412.data.SAssign.target;
__auto_type val = _t3412.data.SAssign.val;
        /* pass */
        GpuEmitter_emit_assign(self, (*target), val);
    } else if (_t3412.tag == HirStmt_SExpr) {
        __auto_type expr = _t3412.data.SExpr.expr;
        /* pass */
        GpuEmitter_emit_stmt_expr(self, (*expr));
    } else if (_t3412.tag == HirStmt_SIf) {
        __auto_type cond = _t3412.data.SIf.cond;
__auto_type then_b = _t3412.data.SIf.then_b;
__auto_type else_b = _t3412.data.SIf.else_b;
        /* pass */
        GVal* c = GpuEmitter_emit_expr(self, cond);
        /* pass */
        TrStr cc = GpuEmitter_coerce_bool(self, c);
        /* pass */
        TrStr lt = GpuEmitter_newlbl(self);
        /* pass */
        TrStr le = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lend = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3414 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br i1 ")), _tr_strz(cc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(le)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3414); _tr_str_release(_at_t3414); });
        /* pass */
        ({ TrStr _at_t3415 = (_tr_strx_concat(_tr_strz(lt), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3415); _tr_str_release(_at_t3415); });
        /* pass */
        GpuEmitter_emit_block(self, then_b);
        /* pass */
        ({ TrStr _at_t3416 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br label %")), _tr_strz(lend))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3416); _tr_str_release(_at_t3416); });
        /* pass */
        ({ TrStr _at_t3417 = (_tr_strx_concat(_tr_strz(le), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3417); _tr_str_release(_at_t3417); });
        /* pass */
        GpuEmitter_emit_block(self, else_b);
        /* pass */
        ({ TrStr _at_t3418 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br label %")), _tr_strz(lend))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3418); _tr_str_release(_at_t3418); });
        /* pass */
        ({ TrStr _at_t3419 = (_tr_strx_concat(_tr_strz(lend), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3419); _tr_str_release(_at_t3419); });
        _tr_obj_release(c, _trdrop_GVal);
        _tr_str_release(cc);
        _tr_str_release(lt);
        _tr_str_release(le);
        _tr_str_release(lend);
    } else if (_t3412.tag == HirStmt_SWhile) {
        __auto_type cond = _t3412.data.SWhile.cond;
__auto_type body = _t3412.data.SWhile.body;
        /* pass */
        TrStr lc = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lb = GpuEmitter_newlbl(self);
        /* pass */
        TrStr lend2 = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3420 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br label %")), _tr_strz(lc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3420); _tr_str_release(_at_t3420); });
        /* pass */
        ({ TrStr _at_t3421 = (_tr_strx_concat(_tr_strz(lc), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3421); _tr_str_release(_at_t3421); });
        /* pass */
        GVal* cnd = GpuEmitter_emit_expr(self, cond);
        /* pass */
        TrStr cb = GpuEmitter_coerce_bool(self, cnd);
        /* pass */
        ({ TrStr _at_t3422 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br i1 ")), _tr_strz(cb))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lend2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3422); _tr_str_release(_at_t3422); });
        /* pass */
        ({ TrStr _at_t3423 = (_tr_strx_concat(_tr_strz(lb), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3423); _tr_str_release(_at_t3423); });
        /* pass */
        GpuEmitter_emit_block(self, body);
        /* pass */
        ({ TrStr _at_t3424 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br label %")), _tr_strz(lc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3424); _tr_str_release(_at_t3424); });
        /* pass */
        ({ TrStr _at_t3425 = (_tr_strx_concat(_tr_strz(lend2), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3425); _tr_str_release(_at_t3425); });
        _tr_str_release(lc);
        _tr_str_release(lb);
        _tr_str_release(lend2);
        _tr_obj_release(cnd, _trdrop_GVal);
        _tr_str_release(cb);
    } else if (_t3412.tag == HirStmt_SUnsafe) {
        __auto_type body = _t3412.data.SUnsafe.body;
        /* pass */
        GpuEmitter_emit_block(self, body);
    } else if (_t3412.tag == HirStmt_SReturn) {
        __auto_type val = _t3412.data.SReturn.val;
        /* pass */
        if (((strcmp(_tr_strz(self->ret_llty), _tr_strz(_tr_str_lit("void"))) == 0) || (((unsigned long long)(val)) == ((unsigned long long)(0LL))))) {
            /* pass */
            GpuEmitter_w(self, _tr_str_lit("  ret void\n"));
        } else {
            /* pass */
            GVal* rv = GpuEmitter_emit_expr(self, val);
            /* pass */
            if ((strcmp(_tr_strz(self->ret_llty), _tr_strz(_tr_str_lit("ptr addrspace(1)"))) == 0)) {
                /* pass */
                ({ TrStr _at_t3426 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ptr addrspace(1) ")), _tr_strz(rv->val))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3426); _tr_str_release(_at_t3426); });
            } else {
                /* pass */
                TrStr cv = GpuEmitter_coerce(self, rv, self->ret_llty);
                /* pass */
                ({ TrStr _at_t3427 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ")), _tr_strz(self->ret_llty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3427); _tr_str_release(_at_t3427); });
            }
        }
        /* pass */
        TrStr lu = GpuEmitter_newlbl(self);
        /* pass */
        ({ TrStr _at_t3428 = (_tr_strx_concat(_tr_strz(lu), _tr_strz(_tr_str_lit(":\n")))); GpuEmitter_w(self, _at_t3428); _tr_str_release(_at_t3428); });
        _tr_str_release(lu);
    } else if (_t3412.tag == HirStmt_SPass) {
        /* pass */
        /* pass */
    } else if (_t3412.tag == HirStmt_SLineMarker) {
        __auto_type n = _t3412.data.SLineMarker.n;
        /* pass */
        /* pass */
    } else if (1) {
        __auto_type _ = _t3412;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("unsupported statement in kernel"));
    }
}

__attribute__((hot)) void GpuEmitter_emit_assign(GpuEmitter* self, HirExpr target, HirExpr* val) {
    /* pass */
    __auto_type _t3429 = target;
    if (_t3429.tag == HirExpr_EIdent) {
        __auto_type name = _t3429.data.EIdent.name;
__auto_type ty = _t3429.data.EIdent.ty;
__auto_type is_move = _t3429.data.EIdent.is_move;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, val);
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, slot);
        /* pass */
        ({ TrStr _at_t3430 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(slot))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3430); _tr_str_release(_at_t3430); });
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(slot);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t3429;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("unsupported assignment target in kernel"));
    }
}

__attribute__((hot)) void GpuEmitter_emit_stmt_expr(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3431 = e;
    if (_t3431.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t3431.data.EMethodCall.obj;
__auto_type method = _t3431.data.EMethodCall.method;
__auto_type args = _t3431.data.EMethodCall.args;
__auto_type ty = _t3431.data.EMethodCall.ty;
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("write"))) == 0) && (args->len == 1LL))) {
            /* pass */
            GpuEmitter_emit_store(self, (*obj), ((HirExpr*)List_ptr_get(args, 0LL)));
            /* pass */
            return;
        }
        /* pass */
        GVal* _v = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v, _trdrop_GVal);
    } else if (_t3431.tag == HirExpr_ECall) {
        __auto_type callee = _t3431.data.ECall.callee;
__auto_type args = _t3431.data.ECall.args;
__auto_type ty = _t3431.data.ECall.ty;
        /* pass */
        if ((strcmp(_tr_strz(GpuEmitter_callee_name(self, (*callee))), _tr_strz(_tr_str_lit("gpu_barrier"))) == 0)) {
            /* pass */
            if ((strcmp(_tr_strz(self->target), _tr_strz(_tr_str_lit("nvptx"))) == 0)) {
                /* pass */
                GpuEmitter_w(self, _tr_str_lit("  call void @llvm.nvvm.barrier0()\n"));
            } else {
                /* pass */
                GpuEmitter_w(self, _tr_str_lit("  call spir_func void @_Z7barrierj(i32 1)\n"));
            }
            /* pass */
            return;
        }
        /* pass */
        GVal* _v2 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v2, _trdrop_GVal);
    } else if (1) {
        __auto_type _ = _t3431;
        /* pass */
        GVal* _v3 = GpuEmitter_emit_expr_hir(self, e);
        _tr_obj_release(_v3, _trdrop_GVal);
    }
}

__attribute__((hot)) void GpuEmitter_emit_store(GpuEmitter* self, HirExpr chain, HirExpr* valp) {
    /* pass */
    __auto_type _t3432 = chain;
    if (_t3432.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t3432.data.EMethodCall.obj;
__auto_type pmeth = _t3432.data.EMethodCall.method;
__auto_type pargs = _t3432.data.EMethodCall.args;
__auto_type pty = _t3432.data.EMethodCall.ty;
        /* pass */
        if (((strcmp(_tr_strz(pmeth), _tr_strz(_tr_str_lit("offset"))) != 0) || (pargs->len != 1LL))) {
            /* pass */
            GpuEmitter_fail(self, _tr_str_lit("store target must be p.offset(i).write(v)"));
            /* pass */
            return;
        }
        /* pass */
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
        /* pass */
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
        /* pass */
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit("i64"));
        /* pass */
        TrStr gep = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3433 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(gep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr addrspace(1) "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pr->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(i64idx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3433); _tr_str_release(_at_t3433); });
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, valp);
        /* pass */
        TrStr cv = GpuEmitter_coerce(self, v, pr->ty);
        /* pass */
        ({ TrStr _at_t3434 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(pr->ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr addrspace(1) "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3434); _tr_str_release(_at_t3434); });
        _tr_obj_release(pr, _trdrop_GVal);
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        _tr_obj_release(v, _trdrop_GVal);
        _tr_str_release(cv);
    } else if (1) {
        __auto_type _ = _t3432;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("store target must be p.offset(i).write(v)"));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_ptr(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3435 = e;
    if (_t3435.tag == HirExpr_EIdent) {
        __auto_type name = _t3435.data.EIdent.name;
__auto_type ty = _t3435.data.EIdent.ty;
__auto_type is_move = _t3435.data.EIdent.is_move;
        /* pass */
        TrStr et = GpuEmitter_var_elem_ty(self, name);
        /* pass */
        if ((strcmp(_tr_strz(et), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t3436 = _tr_str_lit("float");
            _tr_str_release(et);
            et = _strtmp_t3436;
        }
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3437 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load ptr addrspace(1), ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3437); _tr_str_release(_at_t3437); });
        /* pass */
        return GVal_make(et, r);
    } else if (_t3435.tag == HirExpr_ECast) {
        __auto_type inner = _t3435.data.ECast.expr;
__auto_type target_ty = _t3435.data.ECast.target_ty;
        /* pass */
        GVal* g = GpuEmitter_emit_ptr(self, (*inner));
        /* pass */
        if (((strcmp(_tr_strz(target_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0) && (target_ty->args->len > 0LL))) {
            /* pass */
            g->ty = _gpu_scalar_ty((*((AstType**)List_ptr_get(target_ty->args, 0LL)))->name);
        }
        /* pass */
        return g;
    } else if (1) {
        __auto_type _ = _t3435;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("unsupported pointer expression in kernel"));
        /* pass */
        return GVal_make(_tr_str_lit("float"), _tr_str_lit("null"));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr(GpuEmitter* self, HirExpr* ep) {
    /* pass */
    return GpuEmitter_emit_expr_hir(self, (*ep));
}

__attribute__((hot)) GVal* GpuEmitter_emit_expr_hir(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3438 = e;
    if (_t3438.tag == HirExpr_ELitInt) {
        __auto_type val = _t3438.data.ELitInt.val;
__auto_type ty = _t3438.data.ELitInt.ty;
        /* pass */
        TrStr t = _gpu_scalar_ty(ty->name);
        /* pass */
        if (_gpu_is_float(t)) {
            /* pass */
            TrStr _strtmp_t3439 = _tr_str_lit("i64");
            _tr_str_release(t);
            t = _strtmp_t3439;
        }
        /* pass */
        return ({ TrStr _at_t3440 = (_tr_str_wrap(_tr_int_to_str((long long)(val)))); __auto_type _wr = (GVal_make(t, _at_t3440)); _tr_str_release(_at_t3440); _wr; });
    } else if (_t3438.tag == HirExpr_ELitFloat) {
        __auto_type val = _t3438.data.ELitFloat.val;
__auto_type ty = _t3438.data.ELitFloat.ty;
        /* pass */
        TrStr ft = _gpu_scalar_ty(ty->name);
        /* pass */
        if ((!_gpu_is_float(ft))) {
            /* pass */
            TrStr _strtmp_t3441 = _tr_str_lit("double");
            _tr_str_release(ft);
            ft = _strtmp_t3441;
        }
        /* pass */
        return ({ TrStr _at_t3442 = (GpuEmitter_float_lit(self, val)); __auto_type _wr = (GVal_make(ft, _at_t3442)); _tr_str_release(_at_t3442); _wr; });
    } else if (_t3438.tag == HirExpr_ELitBool) {
        __auto_type val = _t3438.data.ELitBool.val;
__auto_type ty = _t3438.data.ELitBool.ty;
        /* pass */
        if (val) {
            /* pass */
            return GVal_make(_tr_str_lit("i1"), _tr_str_lit("1"));
        }
        /* pass */
        return GVal_make(_tr_str_lit("i1"), _tr_str_lit("0"));
    } else if (_t3438.tag == HirExpr_EIdent) {
        __auto_type name = _t3438.data.EIdent.name;
__auto_type ty = _t3438.data.EIdent.ty;
__auto_type is_move = _t3438.data.EIdent.is_move;
        /* pass */
        TrStr slot = GpuEmitter_var_slot_ty(self, name);
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3443 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(slot)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3443); _tr_str_release(_at_t3443); });
        /* pass */
        return GVal_make(slot, r);
    } else if (_t3438.tag == HirExpr_EBinOp) {
        __auto_type op = _t3438.data.EBinOp.op;
__auto_type left = _t3438.data.EBinOp.left;
__auto_type right = _t3438.data.EBinOp.right;
__auto_type ty = _t3438.data.EBinOp.ty;
        /* pass */
        return GpuEmitter_emit_binop(self, op, left, right);
    } else if (_t3438.tag == HirExpr_EUnaryOp) {
        __auto_type op = _t3438.data.EUnaryOp.op;
__auto_type expr = _t3438.data.EUnaryOp.expr;
__auto_type ty = _t3438.data.EUnaryOp.ty;
        /* pass */
        GVal* v = GpuEmitter_emit_expr(self, expr);
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            if (_gpu_is_float(v->ty)) {
                /* pass */
                ({ TrStr _at_t3444 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fneg "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3444); _tr_str_release(_at_t3444); });
            } else {
                /* pass */
                ({ TrStr _at_t3445 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sub "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 0, "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3445); _tr_str_release(_at_t3445); });
            }
            /* pass */
            return GVal_make(v->ty, r);
        }
        /* pass */
        return v;
    } else if (_t3438.tag == HirExpr_ECast) {
        __auto_type inner = _t3438.data.ECast.expr;
__auto_type target_ty = _t3438.data.ECast.target_ty;
        /* pass */
        GVal* iv = GpuEmitter_emit_expr(self, inner);
        /* pass */
        TrStr tt = _gpu_scalar_ty(target_ty->name);
        /* pass */
        if ((strcmp(_tr_strz(target_ty->name), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
            /* pass */
            _tr_str_release(tt);
            return iv;
        }
        /* pass */
        return ({ TrStr _at_t3446 = (GpuEmitter_coerce(self, iv, tt)); __auto_type _wr = (GVal_make(tt, _at_t3446)); _tr_str_release(_at_t3446); _wr; });
    } else if (_t3438.tag == HirExpr_EMethodCall) {
        __auto_type obj = _t3438.data.EMethodCall.obj;
__auto_type method = _t3438.data.EMethodCall.method;
__auto_type args = _t3438.data.EMethodCall.args;
__auto_type ty = _t3438.data.EMethodCall.ty;
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("read"))) == 0) && (args->len == 0LL))) {
            /* pass */
            return GpuEmitter_emit_load(self, (*obj));
        }
        /* pass */
        ({ TrStr _at_t3447 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unsupported method '")), _tr_strz(method))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in kernel"))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3447); _tr_str_release(_at_t3447); });
        /* pass */
        return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
    } else if (_t3438.tag == HirExpr_ECall) {
        __auto_type callee = _t3438.data.ECall.callee;
__auto_type args = _t3438.data.ECall.args;
__auto_type ty = _t3438.data.ECall.ty;
        /* pass */
        return GpuEmitter_emit_call(self, (*callee), args);
    } else if (1) {
        __auto_type _ = _t3438;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("unsupported expression in kernel"));
        /* pass */
        return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_load(GpuEmitter* self, HirExpr chain) {
    /* pass */
    __auto_type _t3448 = chain;
    if (_t3448.tag == HirExpr_EMethodCall) {
        __auto_type pobj = _t3448.data.EMethodCall.obj;
__auto_type pmeth = _t3448.data.EMethodCall.method;
__auto_type pargs = _t3448.data.EMethodCall.args;
__auto_type pty = _t3448.data.EMethodCall.ty;
        /* pass */
        if (((strcmp(_tr_strz(pmeth), _tr_strz(_tr_str_lit("offset"))) != 0) || (pargs->len != 1LL))) {
            /* pass */
            GpuEmitter_fail(self, _tr_str_lit("load must be p.offset(i).read()"));
            /* pass */
            return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
        }
        /* pass */
        GVal* pr = GpuEmitter_emit_ptr(self, (*pobj));
        /* pass */
        GVal* idx = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(pargs, 0LL)));
        /* pass */
        TrStr i64idx = GpuEmitter_coerce(self, idx, _tr_str_lit("i64"));
        /* pass */
        TrStr gep = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3449 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(gep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr addrspace(1) "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pr->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(i64idx)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3449); _tr_str_release(_at_t3449); });
        /* pass */
        TrStr r = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3450 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pr->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr addrspace(1) "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3450); _tr_str_release(_at_t3450); });
        /* pass */
        _tr_obj_release(idx, _trdrop_GVal);
        _tr_str_release(i64idx);
        _tr_str_release(gep);
        return GVal_make(pr->ty, r);
    } else if (1) {
        __auto_type _ = _t3448;
        /* pass */
        GpuEmitter_fail(self, _tr_str_lit("load must be p.offset(i).read()"));
        /* pass */
        return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
    }
}

__attribute__((hot)) TrStr GpuEmitter_callee_name(GpuEmitter* self, HirExpr c) {
    /* pass */
    __auto_type _t3451 = c;
    if (_t3451.tag == HirExpr_EIdent) {
        __auto_type name = _t3451.data.EIdent.name;
__auto_type ty = _t3451.data.EIdent.ty;
__auto_type is_move = _t3451.data.EIdent.is_move;
        return _tr_str_retain(name);
    } else if (1) {
        __auto_type _ = _t3451;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) GVal* GpuEmitter_emit_call(GpuEmitter* self, HirExpr callee, List_ptr* args) {
    /* pass */
    TrStr nm = GpuEmitter_callee_name(self, callee);
    /* pass */
    if (((((((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_global_id"))) == 0) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_id"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_group_id"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_size"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_global_size"))) == 0)) || (strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_num_groups"))) == 0))) {
        /* pass */
        long long dim = 0LL;
        /* pass */
        if ((args->len == 1LL)) {
            /* pass */
            dim = GpuEmitter_literal_int(self, (*((HirExpr*)List_ptr_get(args, 0LL))));
        }
        /* pass */
        return GpuEmitter_emit_gpu_builtin(self, nm, dim);
    }
    /* pass */
    long long di = 0LL;
    /* pass */
    while ((di < self->dev_fns->len)) {
        /* pass */
        HirFunction* df = ((HirFunction*)List_ptr_get(self->dev_fns, di));
        /* pass */
        if ((strcmp(_tr_strz(df->name), _tr_strz(nm)) == 0)) {
            /* pass */
            TrStr rty = GpuEmitter__gpu_ret_ty(self, df->ret_ty);
            /* pass */
            TrStr argstr = _tr_str_lit("");
            /* pass */
            long long ai = 0LL;
            /* pass */
            while ((ai < args->len)) {
                /* pass */
                if ((ai > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t3452 = _tr_strx_concat(_tr_strz(argstr), _tr_strz(_tr_str_lit(", ")));
                    _tr_str_release(argstr);
                    argstr = _strtmp_t3452;
                }
                /* pass */
                GVal* av = GpuEmitter_emit_expr(self, ((HirExpr*)List_ptr_get(args, ai)));
                /* pass */
                TrStr aty = _tr_str_retain(av->ty);
                /* pass */
                TrStr aval = _tr_str_retain(av->val);
                /* pass */
                if ((ai < df->params->len)) {
                    /* pass */
                    TrStr ptt = GpuEmitter__gpu_ret_ty(self, ((HirParam*)List_ptr_get(df->params, ai))->ty);
                    /* pass */
                    if (((strcmp(_tr_strz(ptt), _tr_strz(_tr_str_lit("ptr addrspace(1)"))) != 0) && (strcmp(_tr_strz(aty), _tr_strz(ptt)) != 0))) {
                        /* pass */
                        TrStr _strtmp_t3453 = GpuEmitter_coerce(self, av, ptt);
                        _tr_str_release(aval);
                        aval = _strtmp_t3453;
                        /* pass */
                        TrStr _strtmp_t3454 = _tr_str_retain(ptt);
                        _tr_str_release(aty);
                        aty = _strtmp_t3454;
                    }
                }
                /* pass */
                TrStr _strtmp_t3455 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(argstr), _tr_strz(aty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(aval)); _tr_str_release(_cl); _cres; });
                _tr_str_release(argstr);
                argstr = _strtmp_t3455;
                /* pass */
                ai = (ai + 1LL);
                _tr_obj_release(av, _trdrop_GVal);
                _tr_str_release(aty);
                _tr_str_release(aval);
            }
            /* pass */
            if ((strcmp(_tr_strz(rty), _tr_strz(_tr_str_lit("void"))) == 0)) {
                /* pass */
                ({ TrStr _at_t3456 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  call void @")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(argstr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3456); _tr_str_release(_at_t3456); });
                /* pass */
                _tr_str_release(nm);
                _tr_str_release(rty);
                _tr_str_release(argstr);
                return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
            }
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3457 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(argstr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3457); _tr_str_release(_at_t3457); });
            /* pass */
            _tr_str_release(nm);
            _tr_str_release(argstr);
            return GVal_make(rty, r);
        }
        /* pass */
        di = (di + 1LL);
    }
    /* pass */
    ({ TrStr _at_t3458 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("call to '")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' not allowed in kernel (mark it @device, or use a GPU builtin)"))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3458); _tr_str_release(_at_t3458); });
    /* pass */
    _tr_str_release(nm);
    return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
}

__attribute__((hot)) GVal* GpuEmitter_emit_gpu_builtin(GpuEmitter* self, TrStr nm, long long dim) {
    /* pass */
    if ((strcmp(_tr_strz(self->target), _tr_strz(_tr_str_lit("nvptx"))) == 0)) {
        /* pass */
        TrStr axis = _tr_str_lit("x");
        /* pass */
        if ((dim == 1LL)) {
            /* pass */
            TrStr _strtmp_t3459 = _tr_str_lit("y");
            _tr_str_release(axis);
            axis = _strtmp_t3459;
        }
        /* pass */
        if ((dim == 2LL)) {
            /* pass */
            TrStr _strtmp_t3460 = _tr_str_lit("z");
            _tr_str_release(axis);
            axis = _strtmp_t3460;
        }
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_id"))) == 0)) {
            /* pass */
            TrStr r = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3461 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.tid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3461); _tr_str_release(_at_t3461); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r);
        }
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_group_id"))) == 0)) {
            /* pass */
            TrStr r2 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3462 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3462); _tr_str_release(_at_t3462); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r2);
        }
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_size"))) == 0)) {
            /* pass */
            TrStr r3 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3463 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r3))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3463); _tr_str_release(_at_t3463); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r3);
        }
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_num_groups"))) == 0)) {
            /* pass */
            TrStr r4 = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3464 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r4))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3464); _tr_str_release(_at_t3464); });
            /* pass */
            _tr_str_release(axis);
            return GpuEmitter_i32_to_i64(self, r4);
        }
        /* pass */
        TrStr ct = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3465 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(ct))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.ctaid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3465); _tr_str_release(_at_t3465); });
        /* pass */
        TrStr nt = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3466 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(nt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.ntid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3466); _tr_str_release(_at_t3466); });
        /* pass */
        TrStr m = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3467 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(m))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = mul i32 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3467); _tr_str_release(_at_t3467); });
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_global_size"))) == 0)) {
            /* pass */
            TrStr nc = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3468 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(nc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.nctaid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3468); _tr_str_release(_at_t3468); });
            /* pass */
            TrStr gs = GpuEmitter_fresh(self);
            /* pass */
            ({ TrStr _at_t3469 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(gs))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = mul i32 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3469); _tr_str_release(_at_t3469); });
            /* pass */
            _tr_str_release(axis);
            _tr_str_release(ct);
            _tr_str_release(nt);
            _tr_str_release(m);
            _tr_str_release(nc);
            return GpuEmitter_i32_to_i64(self, gs);
        }
        /* pass */
        TrStr tid = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3470 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(tid))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call i32 @llvm.nvvm.read.ptx.sreg.tid."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(axis)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3470); _tr_str_release(_at_t3470); });
        /* pass */
        TrStr g = GpuEmitter_fresh(self);
        /* pass */
        ({ TrStr _at_t3471 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(g))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = add i32 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tid)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3471); _tr_str_release(_at_t3471); });
        /* pass */
        _tr_str_release(axis);
        _tr_str_release(ct);
        _tr_str_release(nt);
        _tr_str_release(m);
        _tr_str_release(tid);
        return GpuEmitter_i32_to_i64(self, g);
    }
    /* pass */
    TrStr mangled = _tr_str_lit("@_Z13get_global_idj");
    /* pass */
    if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_id"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3472 = _tr_str_lit("@_Z12get_local_idj");
        _tr_str_release(mangled);
        mangled = _strtmp_t3472;
    }
    /* pass */
    if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_group_id"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3473 = _tr_str_lit("@_Z12get_group_idj");
        _tr_str_release(mangled);
        mangled = _strtmp_t3473;
    }
    /* pass */
    if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_local_size"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3474 = _tr_str_lit("@_Z14get_local_sizej");
        _tr_str_release(mangled);
        mangled = _strtmp_t3474;
    }
    /* pass */
    if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_global_size"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3475 = _tr_str_lit("@_Z15get_global_sizej");
        _tr_str_release(mangled);
        mangled = _strtmp_t3475;
    }
    /* pass */
    if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit("gpu_num_groups"))) == 0)) {
        /* pass */
        TrStr _strtmp_t3476 = _tr_str_lit("@_Z14get_num_groupsj");
        _tr_str_release(mangled);
        mangled = _strtmp_t3476;
    }
    /* pass */
    TrStr sr = GpuEmitter_fresh(self);
    /* pass */
    ({ TrStr _at_t3477 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(sr))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call spir_func i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(mangled)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(i32 "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(dim)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3477); _tr_str_release(_at_t3477); });
    /* pass */
    _tr_str_release(mangled);
    return GVal_make(_tr_str_lit("i64"), sr);
}

__attribute__((hot)) GVal* GpuEmitter_i32_to_i64(GpuEmitter* self, TrStr v) {
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    ({ TrStr _at_t3478 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sext i32 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3478); _tr_str_release(_at_t3478); });
    /* pass */
    return GVal_make(_tr_str_lit("i64"), r);
}

__attribute__((hot)) GVal* GpuEmitter_emit_binop(GpuEmitter* self, TrStr op, HirExpr* lp, HirExpr* rp) {
    /* pass */
    GVal* l = GpuEmitter_emit_expr(self, lp);
    /* pass */
    GVal* r = GpuEmitter_emit_expr(self, rp);
    /* pass */
    TrStr ct = GpuEmitter_common_ty(self, l->ty, r->ty);
    /* pass */
    TrStr la = GpuEmitter_coerce(self, l, ct);
    /* pass */
    TrStr ra = GpuEmitter_coerce(self, r, ct);
    /* pass */
    bool isf = _gpu_is_float(ct);
    /* pass */
    TrStr res = GpuEmitter_fresh(self);
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3479 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fadd "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3479); _tr_str_release(_at_t3479); });
        } else {
            /* pass */
            ({ TrStr _at_t3480 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = add "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3480); _tr_str_release(_at_t3480); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3481 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fsub "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3481); _tr_str_release(_at_t3481); });
        } else {
            /* pass */
            ({ TrStr _at_t3482 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sub "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3482); _tr_str_release(_at_t3482); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3483 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fmul "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3483); _tr_str_release(_at_t3483); });
        } else {
            /* pass */
            ({ TrStr _at_t3484 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = mul "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3484); _tr_str_release(_at_t3484); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3485 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fdiv "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3485); _tr_str_release(_at_t3485); });
        } else {
            /* pass */
            ({ TrStr _at_t3486 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sdiv "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3486); _tr_str_release(_at_t3486); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            ({ TrStr _at_t3487 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = frem "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3487); _tr_str_release(_at_t3487); });
        } else {
            /* pass */
            ({ TrStr _at_t3488 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = srem "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3488); _tr_str_release(_at_t3488); });
        }
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(la);
        _tr_str_release(ra);
        return GVal_make(ct, res);
    }
    /* pass */
    TrStr pred = GpuEmitter_cmp_pred(self, op, isf);
    /* pass */
    if ((strcmp(_tr_strz(pred), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        ({ TrStr _at_t3489 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unsupported operator '")), _tr_strz(op))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in kernel"))); _tr_str_release(_cl); _cres; })); GpuEmitter_fail(self, _at_t3489); _tr_str_release(_at_t3489); });
        /* pass */
        _tr_obj_release(l, _trdrop_GVal);
        _tr_obj_release(r, _trdrop_GVal);
        _tr_str_release(ct);
        _tr_str_release(la);
        _tr_str_release(ra);
        _tr_str_release(res);
        _tr_str_release(pred);
        return GVal_make(_tr_str_lit("i64"), _tr_str_lit("0"));
    }
    /* pass */
    if (isf) {
        /* pass */
        ({ TrStr _at_t3490 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fcmp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3490); _tr_str_release(_at_t3490); });
    } else {
        /* pass */
        ({ TrStr _at_t3491 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(res))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = icmp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ct)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3491); _tr_str_release(_at_t3491); });
    }
    /* pass */
    _tr_obj_release(l, _trdrop_GVal);
    _tr_obj_release(r, _trdrop_GVal);
    _tr_str_release(ct);
    _tr_str_release(la);
    _tr_str_release(ra);
    _tr_str_release(pred);
    return GVal_make(_tr_str_lit("i1"), res);
}

__attribute__((hot)) TrStr GpuEmitter_cmp_pred(GpuEmitter* self, TrStr op, bool isf) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("oeq");
        }
        /* pass */
        return _tr_str_lit("eq");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("one");
        }
        /* pass */
        return _tr_str_lit("ne");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("olt");
        }
        /* pass */
        return _tr_str_lit("slt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("ole");
        }
        /* pass */
        return _tr_str_lit("sle");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("ogt");
        }
        /* pass */
        return _tr_str_lit("sgt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        if (isf) {
            /* pass */
            return _tr_str_lit("oge");
        }
        /* pass */
        return _tr_str_lit("sge");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr GpuEmitter_common_ty(GpuEmitter* self, TrStr a, TrStr b) {
    /* pass */
    if ((strcmp(_tr_strz(a), _tr_strz(b)) == 0)) {
        /* pass */
        return _tr_str_retain(a);
    }
    /* pass */
    bool af = _gpu_is_float(a);
    /* pass */
    bool bf = _gpu_is_float(b);
    /* pass */
    if ((af || bf)) {
        /* pass */
        if (((strcmp(_tr_strz(a), _tr_strz(_tr_str_lit("double"))) == 0) || (strcmp(_tr_strz(b), _tr_strz(_tr_str_lit("double"))) == 0))) {
            /* pass */
            return _tr_str_lit("double");
        }
        /* pass */
        return _tr_str_lit("float");
    }
    /* pass */
    if ((_gpu_iwidth(a) >= _gpu_iwidth(b))) {
        /* pass */
        return _tr_str_retain(a);
    }
    /* pass */
    return _tr_str_retain(b);
}

__attribute__((hot)) TrStr GpuEmitter_coerce(GpuEmitter* self, GVal* v, TrStr target) {
    /* pass */
    if ((strcmp(_tr_strz(v->ty), _tr_strz(target)) == 0)) {
        /* pass */
        return _tr_str_retain(v->val);
    }
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    bool sf = _gpu_is_float(v->ty);
    /* pass */
    bool tf = _gpu_is_float(target);
    /* pass */
    if ((sf && tf)) {
        /* pass */
        if ((strcmp(_tr_strz(target), _tr_strz(_tr_str_lit("double"))) == 0)) {
            /* pass */
            ({ TrStr _at_t3492 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fpext float "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to double\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3492); _tr_str_release(_at_t3492); });
        } else {
            /* pass */
            ({ TrStr _at_t3493 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fptrunc double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to float\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3493); _tr_str_release(_at_t3493); });
        }
        /* pass */
        return r;
    }
    /* pass */
    if ((tf && (!sf))) {
        /* pass */
        ({ TrStr _at_t3494 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sitofp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3494); _tr_str_release(_at_t3494); });
        /* pass */
        return r;
    }
    /* pass */
    if ((sf && (!tf))) {
        /* pass */
        ({ TrStr _at_t3495 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fptosi "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3495); _tr_str_release(_at_t3495); });
        /* pass */
        return r;
    }
    /* pass */
    long long sw = _gpu_iwidth(v->ty);
    /* pass */
    long long tw = _gpu_iwidth(target);
    /* pass */
    if ((tw > sw)) {
        /* pass */
        ({ TrStr _at_t3496 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sext "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3496); _tr_str_release(_at_t3496); });
        /* pass */
        return r;
    }
    /* pass */
    if ((tw < sw)) {
        /* pass */
        ({ TrStr _at_t3497 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = trunc "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3497); _tr_str_release(_at_t3497); });
        /* pass */
        return r;
    }
    /* pass */
    _tr_str_release(r);
    return _tr_str_retain(v->val);
}

__attribute__((hot)) TrStr GpuEmitter_coerce_bool(GpuEmitter* self, GVal* v) {
    /* pass */
    if ((strcmp(_tr_strz(v->ty), _tr_strz(_tr_str_lit("i1"))) == 0)) {
        /* pass */
        return _tr_str_retain(v->val);
    }
    /* pass */
    TrStr r = GpuEmitter_fresh(self);
    /* pass */
    if (_gpu_is_float(v->ty)) {
        /* pass */
        ({ TrStr _at_t3498 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fcmp one "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", 0.0\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3498); _tr_str_release(_at_t3498); });
    } else {
        /* pass */
        ({ TrStr _at_t3499 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = icmp ne "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(v->val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", 0\n"))); _tr_str_release(_cl); _cres; })); GpuEmitter_w(self, _at_t3499); _tr_str_release(_at_t3499); });
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long GpuEmitter_literal_int(GpuEmitter* self, HirExpr e) {
    /* pass */
    __auto_type _t3500 = e;
    if (_t3500.tag == HirExpr_ELitInt) {
        __auto_type val = _t3500.data.ELitInt.val;
__auto_type ty = _t3500.data.ELitInt.ty;
        return val;
    } else if (1) {
        __auto_type _ = _t3500;
        return 0LL;
    }
}

__attribute__((hot)) TrStr GpuEmitter_float_lit(GpuEmitter* self, double v) {
    /* pass */
    return ({ TrStr _cr = (_hex16(_gpu_f64_bits(v))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("0x")), _cr.data); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) bool fn_is_kernel(HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((Decorator*)List_ptr_get(f->decorators, i))->name), _tr_strz(_tr_str_lit("kernel"))) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool fn_is_device(HirFunction* f) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < f->decorators->len)) {
        /* pass */
        if ((strcmp(_tr_strz(((Decorator*)List_ptr_get(f->decorators, i))->name), _tr_strz(_tr_str_lit("device"))) == 0)) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _gpu_is_float(TrStr t) {
    /* pass */
    return ((strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("double"))) == 0));
}

__attribute__((hot)) long long _gpu_iwidth(TrStr t) {
    /* pass */
    if ((strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("i1"))) == 0)) {
        /* pass */
        return 1LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("i8"))) == 0)) {
        /* pass */
        return 8LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("i16"))) == 0)) {
        /* pass */
        return 16LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(t), _tr_strz(_tr_str_lit("i32"))) == 0)) {
        /* pass */
        return 32LL;
    }
    /* pass */
    return 64LL;
}

__attribute__((hot)) TrStr _gpu_scalar_ty(TrStr n) {
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f32"))) == 0)) {
        /* pass */
        return _tr_str_lit("float");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("f64"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("double"))) == 0))) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("i1");
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i8"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u8"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("c_char"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("c_schar"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("c_uchar"))) == 0))) {
        /* pass */
        return _tr_str_lit("i8");
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i16"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u16"))) == 0))) {
        /* pass */
        return _tr_str_lit("i16");
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("i32"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("u32"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("c_int"))) == 0))) {
        /* pass */
        return _tr_str_lit("i32");
    }
    /* pass */
    return _tr_str_lit("i64");
}

__attribute__((hot)) long long _gpu_f64_bits(double v) {
    /* pass */
    /* unsafe block */
    /* pass */
    double* p = ((double*)_tr_c_calloc((size_t)(1LL), sizeof(double)));
    /* pass */
    (*p = v);
    /* pass */
    long long* ip = ((long long*)(p));
    /* pass */
    return (*ip);
}

__attribute__((hot)) long long _hexdig(long long n) {
    /* pass */
    if ((n < 10LL)) {
        /* pass */
        return (48LL + n);
    }
    /* pass */
    return (65LL + (n - 10LL));
}

__attribute__((hot)) TrStr _hex16(long long v) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(16LL);
    /* pass */
    long long k = 15LL;
    /* pass */
    while ((k >= 0LL)) {
        /* pass */
        long long nib = ((v >> (4LL * k)) & 15LL);
        /* pass */
        StringBuilder_append_char(sb, _hexdig(nib));
        /* pass */
        k = (k - 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

