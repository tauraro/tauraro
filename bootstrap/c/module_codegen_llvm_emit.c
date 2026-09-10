#include "tauraro_types.h"

TrStr _tbaa_suffix(long long aclass);
bool _is_fresh_alloc_ret(TrStr name);
TrStr _ll_int_instr(TrStr op);
TrStr _ll_icmp_pred(TrStr op);
TrStr _ll_float_instr(TrStr op);
TrStr _ll_fcmp_pred(TrStr op);

__attribute__((malloc,returns_nonnull,hot)) LlvmEmitter* LlvmEmitter_init(LModule* m) {
    /* pass */
    LlvmEmitter* e = ((LlvmEmitter*)_tr_obj_alloc(sizeof(LlvmEmitter)));
    /* pass */
    e->out = StringBuilder_init(4096LL);
    /* pass */
    LModule* _cltmp_t3338 = _tr_obj_retain(m);
    _tr_obj_release(e->m, _trdrop_LModule);
    e->m = _cltmp_t3338;
    /* pass */
    e->tmp = 0LL;
    /* pass */
    e->cur_ret = _tr_str_lit("i64");
    /* pass */
    e->cur_main = false;
    /* pass */
    return e;
}

__attribute__((hot)) void LlvmEmitter_w(LlvmEmitter* self, TrStr s) {
    /* pass */
    StringBuilder_append(self->out, s);
}

__attribute__((hot)) TrStr LlvmEmitter_newtmp(LlvmEmitter* self) {
    /* pass */
    self->tmp = (self->tmp + 1LL);
    /* pass */
    return ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->tmp)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("%t")), _cr.data); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) TrStr LlvmEmitter_vty(LlvmEmitter* self, long long v) {
    /* pass */
    return _ll_ty(LFunc_vreg_type(self->cur, v));
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg(LlvmEmitter* self, long long v) {
    /* pass */
    TrStr t = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3339 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(t))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (LlvmEmitter_vty(self, v)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %v"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3339); _tr_str_release(_at_t3339); });
    /* pass */
    return t;
}

__attribute__((hot)) TrStr LlvmEmitter_load_vreg_as(LlvmEmitter* self, long long v, TrStr ty) {
    /* pass */
    TrStr t = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3340 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(t))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %v"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3340); _tr_str_release(_at_t3340); });
    /* pass */
    return t;
}

__attribute__((hot)) void LlvmEmitter_store_vreg(LlvmEmitter* self, long long v, TrStr ty, TrStr val) {
    /* pass */
    ({ TrStr _at_t3341 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %v"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3341); _tr_str_release(_at_t3341); });
}

__attribute__((hot)) TrStr LlvmEmitter_addr_of_base(LlvmEmitter* self, long long v) {
    /* pass */
    if ((strcmp(_tr_strz(LlvmEmitter_vty(self, v)), _tr_strz(_tr_str_lit("ptr"))) == 0)) {
        /* pass */
        return LlvmEmitter_load_vreg_as(self, v, _tr_str_lit("ptr"));
    }
    /* pass */
    TrStr bi = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit("i64"));
    /* pass */
    TrStr bp = LlvmEmitter_newtmp(self);
    /* pass */
    ({ TrStr _at_t3342 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = inttoptr i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to ptr\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3342); _tr_str_release(_at_t3342); });
    /* pass */
    _tr_str_release(bi);
    return bp;
}

__attribute__((hot)) TrStr LlvmEmitter_user_ret_ty(LlvmEmitter* self, TrStr name) {
    /* pass */
    return _ll_ty(LModule_fn_ret_tag(self->m, name));
}

__attribute__((hot)) void LlvmEmitter_emit_inst(LlvmEmitter* self, LInst inst) {
    /* pass */
    __auto_type _t3343 = inst;
    if (_t3343.tag == LInst_IConst) {
        __auto_type dst = _t3343.data.IConst.dst;
__auto_type v = _t3343.data.IConst.v;
        /* pass */
        if ((strcmp(_tr_strz(LlvmEmitter_vty(self, dst)), _tr_strz(_tr_str_lit("double"))) == 0)) {
            /* pass */
            ({ TrStr _at_t3344 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_ll_hexpad16(v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  store double 0x")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %v"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(dst)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3344); _tr_str_release(_at_t3344); });
        } else {
            /* pass */
            ({ TrStr _at_t3345 = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), _at_t3345); _tr_str_release(_at_t3345); });
        }
    } else if (_t3343.tag == LInst_IStr) {
        __auto_type dst = _t3343.data.IStr.dst;
__auto_type str_idx = _t3343.data.IStr.str_idx;
        /* pass */
        ({ TrStr _at_t3346 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(str_idx)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@.str.")), _cr.data); _tr_str_release(_cr); _cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit("ptr"), _at_t3346); _tr_str_release(_at_t3346); });
    } else if (_t3343.tag == LInst_IBinOp) {
        __auto_type dst = _t3343.data.IBinOp.dst;
__auto_type op = _t3343.data.IBinOp.op;
__auto_type a = _t3343.data.IBinOp.a;
__auto_type b = _t3343.data.IBinOp.b;
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0) && (strcmp(_tr_strz(LlvmEmitter_vty(self, dst)), _tr_strz(_tr_str_lit("ptr"))) == 0))) {
            /* pass */
            TrStr pbase = LlvmEmitter_addr_of_base(self, a);
            /* pass */
            TrStr poff = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit("i64"));
            /* pass */
            TrStr pg = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3347 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(pg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pbase)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(poff)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3347); _tr_str_release(_at_t3347); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit("ptr"), pg);
            _tr_str_release(pbase);
            _tr_str_release(poff);
            _tr_str_release(pg);
        } else {
            /* pass */
            TrStr la = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit("i64"));
            /* pass */
            TrStr lb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit("i64"));
            /* pass */
            TrStr pred = _ll_icmp_pred(op);
            /* pass */
            if ((strcmp(_tr_strz(pred), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr c = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3348 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(c))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = icmp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3348); _tr_str_release(_at_t3348); });
                /* pass */
                TrStr z = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3349 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(z))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = zext i1 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3349); _tr_str_release(_at_t3349); });
                /* pass */
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), z);
                _tr_str_release(c);
                _tr_str_release(z);
            } else {
                /* pass */
                TrStr r = LlvmEmitter_newtmp(self);
                /* pass */
                ({ TrStr _at_t3350 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_int_instr(op)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(la)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3350); _tr_str_release(_at_t3350); });
                /* pass */
                LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), r);
                _tr_str_release(r);
            }
        }
    } else if (_t3343.tag == LInst_IFBinOp) {
        __auto_type dst = _t3343.data.IFBinOp.dst;
__auto_type op = _t3343.data.IFBinOp.op;
__auto_type a = _t3343.data.IFBinOp.a;
__auto_type b = _t3343.data.IFBinOp.b;
        /* pass */
        TrStr fa = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit("double"));
        /* pass */
        TrStr fb = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit("double"));
        /* pass */
        TrStr fpred = _ll_fcmp_pred(op);
        /* pass */
        if ((strcmp(_tr_strz(fpred), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr c2 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3351 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(c2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fcmp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fpred)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fa)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3351); _tr_str_release(_at_t3351); });
            /* pass */
            TrStr z2 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3352 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(z2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = zext i1 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3352); _tr_str_release(_at_t3352); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), z2);
            _tr_str_release(c2);
            _tr_str_release(z2);
        } else {
            /* pass */
            TrStr fr = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3353 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(fr))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_float_instr(op)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fa)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3353); _tr_str_release(_at_t3353); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit("double"), fr);
            _tr_str_release(fr);
        }
        _tr_str_release(fa);
        _tr_str_release(fb);
        _tr_str_release(fpred);
    } else if (_t3343.tag == LInst_IIToF) {
        __auto_type dst = _t3343.data.IIToF.dst;
__auto_type src = _t3343.data.IIToF.src;
        /* pass */
        TrStr si = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit("i64"));
        /* pass */
        TrStr rf = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3354 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(rf))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = sitofp i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(si)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to double\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3354); _tr_str_release(_at_t3354); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit("double"), rf);
        _tr_str_release(si);
        _tr_str_release(rf);
    } else if (_t3343.tag == LInst_IFToI) {
        __auto_type dst = _t3343.data.IFToI.dst;
__auto_type src = _t3343.data.IFToI.src;
        /* pass */
        TrStr sf = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit("double"));
        /* pass */
        TrStr ri = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3355 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(ri))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = fptosi double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sf)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3355); _tr_str_release(_at_t3355); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), ri);
        _tr_str_release(sf);
        _tr_str_release(ri);
    } else if (_t3343.tag == LInst_ILoadVar) {
        __auto_type dst = _t3343.data.ILoadVar.dst;
__auto_type name = _t3343.data.ILoadVar.name;
        /* pass */
        TrStr vt = _ll_ty(LFunc_var_type(self->cur, name));
        /* pass */
        TrStr t = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3356 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(t))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3356); _tr_str_release(_at_t3356); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, vt, t);
        _tr_str_release(vt);
        _tr_str_release(t);
    } else if (_t3343.tag == LInst_IStoreVar) {
        __auto_type name = _t3343.data.IStoreVar.name;
__auto_type src = _t3343.data.IStoreVar.src;
        /* pass */
        TrStr st = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr ls = LlvmEmitter_load_vreg_as(self, src, st);
        /* pass */
        ({ TrStr _at_t3357 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(st))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3357); _tr_str_release(_at_t3357); });
        _tr_str_release(st);
        _tr_str_release(ls);
    } else if (_t3343.tag == LInst_ILoadGlobal) {
        __auto_type dst = _t3343.data.ILoadGlobal.dst;
__auto_type gidx = _t3343.data.ILoadGlobal.gidx;
        /* pass */
        TrStr gt = _ll_ty(List_i64_get(self->m->global_types, gidx));
        /* pass */
        TrStr tg = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3358 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(tg))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gt)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr @g."))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3358); _tr_str_release(_at_t3358); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, gt, tg);
        _tr_str_release(gt);
        _tr_str_release(tg);
    } else if (_t3343.tag == LInst_IStoreGlobal) {
        __auto_type gidx = _t3343.data.IStoreGlobal.gidx;
__auto_type src = _t3343.data.IStoreGlobal.src;
        /* pass */
        TrStr gt2 = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr lg = LlvmEmitter_load_vreg_as(self, src, gt2);
        /* pass */
        ({ TrStr _at_t3359 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(gt2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lg)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr @g."))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3359); _tr_str_release(_at_t3359); });
        _tr_str_release(gt2);
        _tr_str_release(lg);
    } else if (_t3343.tag == LInst_ICall) {
        __auto_type dst = _t3343.data.ICall.dst;
__auto_type callee = _t3343.data.ICall.callee;
__auto_type args = _t3343.data.ICall.args;
        /* pass */
        LlvmEmitter_emit_call(self, dst, callee, args);
    } else if (_t3343.tag == LInst_IFCall1) {
        __auto_type dst = _t3343.data.IFCall1.dst;
__auto_type callee = _t3343.data.IFCall1.callee;
__auto_type arg = _t3343.data.IFCall1.arg;
        /* pass */
        TrStr a1 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit("double"));
        /* pass */
        TrStr rt1 = _tr_str_lit("void");
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3360 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t3360;
        }
        /* pass */
        if ((strcmp(_tr_strz(rt1), _tr_strz(_tr_str_lit("void"))) == 0)) {
            /* pass */
            ({ TrStr _at_t3361 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  call void @")), _tr_strz(callee))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(a1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3361); _tr_str_release(_at_t3361); });
        } else {
            /* pass */
            TrStr r1 = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3362 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r1))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rt1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(a1)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3362); _tr_str_release(_at_t3362); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, rt1, r1);
            _tr_str_release(r1);
        }
        _tr_str_release(a1);
        _tr_str_release(rt1);
    } else if (_t3343.tag == LInst_IFCallF) {
        __auto_type dst = _t3343.data.IFCallF.dst;
__auto_type callee = _t3343.data.IFCallF.callee;
__auto_type arg = _t3343.data.IFCallF.arg;
        /* pass */
        TrStr a2 = LlvmEmitter_load_vreg_as(self, arg, _tr_str_lit("double"));
        /* pass */
        TrStr r2 = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3363 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call double @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(a2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3363); _tr_str_release(_at_t3363); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit("double"), r2);
        }
        _tr_str_release(a2);
        _tr_str_release(r2);
    } else if (_t3343.tag == LInst_IFCall2F) {
        __auto_type dst = _t3343.data.IFCall2F.dst;
__auto_type callee = _t3343.data.IFCall2F.callee;
__auto_type a = _t3343.data.IFCall2F.a;
__auto_type b = _t3343.data.IFCall2F.b;
        /* pass */
        TrStr fa2 = LlvmEmitter_load_vreg_as(self, a, _tr_str_lit("double"));
        /* pass */
        TrStr fb2 = LlvmEmitter_load_vreg_as(self, b, _tr_str_lit("double"));
        /* pass */
        TrStr r3 = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3364 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r3))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call double @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fa2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fb2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3364); _tr_str_release(_at_t3364); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, _tr_str_lit("double"), r3);
        }
        _tr_str_release(fa2);
        _tr_str_release(fb2);
        _tr_str_release(r3);
    } else if (_t3343.tag == LInst_IBitsF) {
        __auto_type dst = _t3343.data.IBitsF.dst;
__auto_type src = _t3343.data.IBitsF.src;
        /* pass */
        TrStr bi = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit("i64"));
        /* pass */
        TrStr bf = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3365 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bf))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = bitcast i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bi)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to double\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3365); _tr_str_release(_at_t3365); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit("double"), bf);
        _tr_str_release(bi);
        _tr_str_release(bf);
    } else if (_t3343.tag == LInst_IFBits) {
        __auto_type dst = _t3343.data.IFBits.dst;
__auto_type src = _t3343.data.IFBits.src;
        /* pass */
        TrStr bd = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit("double"));
        /* pass */
        TrStr bx = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3366 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = bitcast double "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bd)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3366); _tr_str_release(_at_t3366); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), bx);
        _tr_str_release(bd);
        _tr_str_release(bx);
    } else if (_t3343.tag == LInst_IAddrVar) {
        __auto_type dst = _t3343.data.IAddrVar.dst;
__auto_type name = _t3343.data.IAddrVar.name;
        /* pass */
        ({ TrStr _at_t3367 = (_tr_strx_concat(_tr_strz(_tr_str_lit("%var_")), _tr_strz(name))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit("ptr"), _at_t3367); _tr_str_release(_at_t3367); });
    } else if (_t3343.tag == LInst_IAddrGlobal) {
        __auto_type dst = _t3343.data.IAddrGlobal.dst;
__auto_type gidx = _t3343.data.IAddrGlobal.gidx;
        /* pass */
        ({ TrStr _at_t3368 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gidx)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@g.")), _cr.data); _tr_str_release(_cr); _cres; })); LlvmEmitter_store_vreg(self, dst, _tr_str_lit("ptr"), _at_t3368); _tr_str_release(_at_t3368); });
    } else if (_t3343.tag == LInst_IFuncAddr) {
        __auto_type dst = _t3343.data.IFuncAddr.dst;
__auto_type fname = _t3343.data.IFuncAddr.fname;
        /* pass */
        ({ TrStr _at_t3369 = (_tr_strx_concat(_tr_strz(_tr_str_lit("@")), _tr_strz(fname))); LlvmEmitter_store_vreg(self, dst, _tr_str_lit("ptr"), _at_t3369); _tr_str_release(_at_t3369); });
    } else if (_t3343.tag == LInst_ICallInd) {
        __auto_type dst = _t3343.data.ICallInd.dst;
__auto_type fnreg = _t3343.data.ICallInd.fnreg;
__auto_type iargs = _t3343.data.ICallInd.args;
        /* pass */
        TrStr ind_args = _tr_str_lit("");
        /* pass */
        long long ii2 = 0LL;
        /* pass */
        while ((ii2 < iargs->len)) {
            /* pass */
            long long iav = List_i64_get(iargs, ii2);
            /* pass */
            TrStr iaty = LlvmEmitter_vty(self, iav);
            /* pass */
            TrStr ial = LlvmEmitter_load_vreg_as(self, iav, iaty);
            /* pass */
            if ((ii2 > 0LL)) {
                /* pass */
                TrStr _strtmp_t3370 = _tr_strx_concat(_tr_strz(ind_args), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(ind_args);
                ind_args = _strtmp_t3370;
            }
            /* pass */
            TrStr _strtmp_t3371 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ind_args), _tr_strz(iaty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ial)); _tr_str_release(_cl); _cres; });
            _tr_str_release(ind_args);
            ind_args = _strtmp_t3371;
            /* pass */
            ii2 = (ii2 + 1LL);
            _tr_str_release(iaty);
            _tr_str_release(ial);
        }
        /* pass */
        TrStr fp = LlvmEmitter_load_vreg_as(self, fnreg, _tr_str_lit("ptr"));
        /* pass */
        TrStr ind_ret = _tr_str_lit("void");
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3372 = LlvmEmitter_vty(self, dst);
            _tr_str_release(ind_ret);
            ind_ret = _strtmp_t3372;
        }
        /* pass */
        if ((strcmp(_tr_strz(ind_ret), _tr_strz(_tr_str_lit("void"))) == 0)) {
            /* pass */
            ({ TrStr _at_t3373 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  call void ")), _tr_strz(fp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ind_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3373); _tr_str_release(_at_t3373); });
        } else {
            /* pass */
            TrStr ir = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3374 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(ir))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ind_ret)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ind_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3374); _tr_str_release(_at_t3374); });
            /* pass */
            LlvmEmitter_store_vreg(self, dst, ind_ret, ir);
            _tr_str_release(ir);
        }
        _tr_str_release(ind_args);
        _tr_str_release(fp);
        _tr_str_release(ind_ret);
    } else if (_t3343.tag == LInst_IAsm) {
        __auto_type code = _t3343.data.IAsm.code;
__auto_type cons = _t3343.data.IAsm.cons;
        /* pass */
        ({ TrStr _at_t3375 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_ll_str_escape(code)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  call void asm sideeffect \"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\", \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cons)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"()\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3375); _tr_str_release(_at_t3375); });
    } else if (_t3343.tag == LInst_ILoad) {
        __auto_type dst = _t3343.data.ILoad.dst;
__auto_type base = _t3343.data.ILoad.base;
__auto_type off = _t3343.data.ILoad.off;
__auto_type aclass = _t3343.data.ILoad.aclass;
        /* pass */
        TrStr lbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr lep = _tr_str_retain(lbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3376 = LlvmEmitter_newtmp(self);
            _tr_str_release(lep);
            lep = _strtmp_t3376;
            /* pass */
            ({ TrStr _at_t3377 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(lep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3377); _tr_str_release(_at_t3377); });
        }
        /* pass */
        TrStr ldty = LlvmEmitter_vty(self, dst);
        /* pass */
        TrStr lv = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3378 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(lv))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ldty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lep)); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tbaa_suffix(aclass)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3378); _tr_str_release(_at_t3378); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, ldty, lv);
        _tr_str_release(lbp);
        _tr_str_release(lep);
        _tr_str_release(ldty);
        _tr_str_release(lv);
    } else if (_t3343.tag == LInst_IStore) {
        __auto_type base = _t3343.data.IStore.base;
__auto_type off = _t3343.data.IStore.off;
__auto_type src = _t3343.data.IStore.src;
__auto_type aclass = _t3343.data.IStore.aclass;
        /* pass */
        TrStr sbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr sep = _tr_str_retain(sbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3379 = LlvmEmitter_newtmp(self);
            _tr_str_release(sep);
            sep = _strtmp_t3379;
            /* pass */
            ({ TrStr _at_t3380 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(sep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3380); _tr_str_release(_at_t3380); });
        }
        /* pass */
        TrStr ssty = LlvmEmitter_vty(self, src);
        /* pass */
        TrStr ssv = LlvmEmitter_load_vreg(self, src);
        /* pass */
        ({ TrStr _at_t3381 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(ssty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ssv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sep)); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tbaa_suffix(aclass)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3381); _tr_str_release(_at_t3381); });
        _tr_str_release(sbp);
        _tr_str_release(sep);
        _tr_str_release(ssty);
        _tr_str_release(ssv);
    } else if (_t3343.tag == LInst_ILoadB) {
        __auto_type dst = _t3343.data.ILoadB.dst;
__auto_type base = _t3343.data.ILoadB.base;
__auto_type off = _t3343.data.ILoadB.off;
        /* pass */
        TrStr bbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr bep = _tr_str_retain(bbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3382 = LlvmEmitter_newtmp(self);
            _tr_str_release(bep);
            bep = _strtmp_t3382;
            /* pass */
            ({ TrStr _at_t3383 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3383); _tr_str_release(_at_t3383); });
        }
        /* pass */
        TrStr bb = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3384 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bb))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = load i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", !tbaa !5\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3384); _tr_str_release(_at_t3384); });
        /* pass */
        TrStr bz = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3385 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(bz))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = zext i8 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bb)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i64\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3385); _tr_str_release(_at_t3385); });
        /* pass */
        LlvmEmitter_store_vreg(self, dst, _tr_str_lit("i64"), bz);
        _tr_str_release(bbp);
        _tr_str_release(bep);
        _tr_str_release(bb);
        _tr_str_release(bz);
    } else if (_t3343.tag == LInst_IStoreB) {
        __auto_type base = _t3343.data.IStoreB.base;
__auto_type off = _t3343.data.IStoreB.off;
__auto_type src = _t3343.data.IStoreB.src;
        /* pass */
        TrStr tbp = LlvmEmitter_addr_of_base(self, base);
        /* pass */
        TrStr tep = _tr_str_retain(tbp);
        /* pass */
        if ((off != 0LL)) {
            /* pass */
            TrStr _strtmp_t3386 = LlvmEmitter_newtmp(self);
            _tr_str_release(tep);
            tep = _strtmp_t3386;
            /* pass */
            ({ TrStr _at_t3387 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(tep))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = getelementptr i8, ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tbp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(off)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3387); _tr_str_release(_at_t3387); });
        }
        /* pass */
        TrStr tsv = LlvmEmitter_load_vreg_as(self, src, _tr_str_lit("i64"));
        /* pass */
        TrStr tt = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3388 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(tt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = trunc i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tsv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i8\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3388); _tr_str_release(_at_t3388); });
        /* pass */
        ({ TrStr _at_t3389 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store i8 ")), _tr_strz(tt))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tep)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", !tbaa !5\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3389); _tr_str_release(_at_t3389); });
        _tr_str_release(tbp);
        _tr_str_release(tep);
        _tr_str_release(tsv);
        _tr_str_release(tt);
    }
}

__attribute__((hot)) void LlvmEmitter_emit_call(LlvmEmitter* self, long long dst, TrStr callee, List_i64* args) {
    /* pass */
    TrStr arglist = _tr_str_lit("");
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < args->len)) {
        /* pass */
        long long av = List_i64_get(args, ai);
        /* pass */
        TrStr aty = LlvmEmitter_vty(self, av);
        /* pass */
        TrStr al = LlvmEmitter_load_vreg_as(self, av, aty);
        /* pass */
        if ((ai > 0LL)) {
            /* pass */
            TrStr _strtmp_t3390 = _tr_strx_concat(_tr_strz(arglist), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(arglist);
            arglist = _strtmp_t3390;
        }
        /* pass */
        TrStr _strtmp_t3391 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(arglist), _tr_strz(aty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(al)); _tr_str_release(_cl); _cres; });
        _tr_str_release(arglist);
        arglist = _strtmp_t3391;
        /* pass */
        ai = (ai + 1LL);
        _tr_str_release(aty);
        _tr_str_release(al);
    }
    /* pass */
    TrStr retty = _tr_str_lit("void");
    /* pass */
    if (LModule_is_user_fn(self->m, callee)) {
        /* pass */
        TrStr _strtmp_t3392 = LlvmEmitter_user_ret_ty(self, callee);
        _tr_str_release(retty);
        retty = _strtmp_t3392;
    } else if ((dst >= 0LL)) {
        /* pass */
        TrStr _strtmp_t3393 = LlvmEmitter_vty(self, dst);
        _tr_str_release(retty);
        retty = _strtmp_t3393;
    }
    /* pass */
    if ((strcmp(_tr_strz(retty), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        ({ TrStr _at_t3394 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  call void @")), _tr_strz(callee))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arglist)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3394); _tr_str_release(_at_t3394); });
    } else {
        /* pass */
        TrStr r = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3395 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(r))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = call "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(retty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arglist)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3395); _tr_str_release(_at_t3395); });
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            LlvmEmitter_store_vreg(self, dst, retty, r);
        }
    }
    _tr_str_release(arglist);
    _tr_str_release(retty);
}

__attribute__((hot)) void LlvmEmitter_emit_term(LlvmEmitter* self, LTerm t) {
    /* pass */
    __auto_type _t3396 = t;
    if (_t3396.tag == LTerm_TRetInt) {
        __auto_type v = _t3396.data.TRetInt.v;
        /* pass */
        if (self->cur_main) {
            /* pass */
            ({ TrStr _at_t3397 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ret i32 ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3397); _tr_str_release(_at_t3397); });
        } else if ((strcmp(_tr_strz(self->cur_ret), _tr_strz(_tr_str_lit("double"))) == 0)) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret double 0.0\n"));
        } else if ((strcmp(_tr_strz(self->cur_ret), _tr_strz(_tr_str_lit("ptr"))) == 0)) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret ptr null\n"));
        } else {
            /* pass */
            ({ TrStr _at_t3398 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(v)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ret i64 ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3398); _tr_str_release(_at_t3398); });
        }
    } else if (_t3396.tag == LTerm_TRetVal) {
        __auto_type v = _t3396.data.TRetVal.v;
        /* pass */
        if (self->cur_main) {
            /* pass */
            TrStr lv = LlvmEmitter_load_vreg_as(self, v, _tr_str_lit("i64"));
            /* pass */
            TrStr tr = LlvmEmitter_newtmp(self);
            /* pass */
            ({ TrStr _at_t3399 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(tr))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = trunc i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lv)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" to i32\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3399); _tr_str_release(_at_t3399); });
            /* pass */
            ({ TrStr _at_t3400 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret i32 ")), _tr_strz(tr))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3400); _tr_str_release(_at_t3400); });
            _tr_str_release(lv);
            _tr_str_release(tr);
        } else {
            /* pass */
            TrStr lv2 = LlvmEmitter_load_vreg_as(self, v, self->cur_ret);
            /* pass */
            ({ TrStr _at_t3401 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ret ")), _tr_strz(self->cur_ret))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lv2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3401); _tr_str_release(_at_t3401); });
            _tr_str_release(lv2);
        }
    } else if (_t3396.tag == LTerm_TRetVoid) {
        /* pass */
        if (self->cur_main) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret i32 0\n"));
        } else if ((strcmp(_tr_strz(self->cur_ret), _tr_strz(_tr_str_lit("double"))) == 0)) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret double 0.0\n"));
        } else if ((strcmp(_tr_strz(self->cur_ret), _tr_strz(_tr_str_lit("ptr"))) == 0)) {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret ptr null\n"));
        } else {
            /* pass */
            LlvmEmitter_w(self, _tr_str_lit("  ret i64 0\n"));
        }
    } else if (_t3396.tag == LTerm_TBr) {
        __auto_type target = _t3396.data.TBr.target;
        /* pass */
        ({ TrStr _at_t3402 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(target)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  br label %bb")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3402); _tr_str_release(_at_t3402); });
    } else if (_t3396.tag == LTerm_TCondBr) {
        __auto_type cond = _t3396.data.TCondBr.cond;
__auto_type then_b = _t3396.data.TCondBr.then_b;
__auto_type else_b = _t3396.data.TCondBr.else_b;
        /* pass */
        TrStr lc = LlvmEmitter_load_vreg_as(self, cond, _tr_str_lit("i64"));
        /* pass */
        TrStr c = LlvmEmitter_newtmp(self);
        /* pass */
        ({ TrStr _at_t3403 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _tr_strz(c))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = icmp ne i64 "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(lc)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", 0\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3403); _tr_str_release(_at_t3403); });
        /* pass */
        ({ TrStr _at_t3404 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  br i1 ")), _tr_strz(c))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %bb"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(then_b)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", label %bb"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(else_b)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3404); _tr_str_release(_at_t3404); });
        _tr_str_release(lc);
        _tr_str_release(c);
    } else if (_t3396.tag == LTerm_TUnset) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit("  unreachable\n"));
    }
}

__attribute__((hot)) void LlvmEmitter_emit_function(LlvmEmitter* self, LFunc* lf) {
    /* pass */
    self->tmp = 0LL;
    /* pass */
    LFunc* _cltmp_t3405 = _tr_obj_retain(lf);
    _tr_obj_release(self->cur, _trdrop_LFunc);
    self->cur = _cltmp_t3405;
    /* pass */
    self->cur_main = lf->is_main;
    /* pass */
    if (lf->is_main) {
        /* pass */
        self->cur_ret = _tr_str_lit("i32");
    } else {
        /* pass */
        self->cur_ret = _ll_ty(LModule_fn_ret_tag(self->m, lf->name));
    }
    /* pass */
    TrStr fname = _tr_str_retain(lf->name);
    /* pass */
    bool main_args = (lf->is_main && (lf->params->len > 0LL));
    /* pass */
    ({ TrStr _at_t3406 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("define ")), _tr_strz(self->cur_ret))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3406); _tr_str_release(_at_t3406); });
    /* pass */
    if (main_args) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit("i32 %argc, ptr %argv"));
    } else {
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < lf->params->len)) {
            /* pass */
            TrStr pn = List_TrStr_get(lf->params, pi);
            /* pass */
            TrStr pty = _ll_ty(LFunc_var_type(lf, pn));
            /* pass */
            if ((pi > 0LL)) {
                /* pass */
                LlvmEmitter_w(self, _tr_str_lit(", "));
            }
            /* pass */
            ({ TrStr _at_t3407 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pty), _tr_strz(_tr_str_lit(" %arg_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn)); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3407); _tr_str_release(_at_t3407); });
            /* pass */
            pi = (pi + 1LL);
            _tr_str_release(pn);
            _tr_str_release(pty);
        }
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit(") {\nentry:\n"));
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < lf->vars->len)) {
        /* pass */
        TrStr vn = List_TrStr_get(lf->vars, vi);
        /* pass */
        long long van = List_i64_get(lf->var_arr, vi);
        /* pass */
        if ((van > 0LL)) {
            /* pass */
            ({ TrStr _at_t3408 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  %var_")), _tr_strz(vn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = alloca ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(van)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" x i64]\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3408); _tr_str_release(_at_t3408); });
        } else {
            /* pass */
            ({ TrStr _at_t3409 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  %var_")), _tr_strz(vn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = alloca "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_ty(LFunc_var_type(lf, vn))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3409); _tr_str_release(_at_t3409); });
        }
        /* pass */
        vi = (vi + 1LL);
        _tr_str_release(vn);
    }
    /* pass */
    long long ri = 0LL;
    /* pass */
    while ((ri < lf->n_vregs)) {
        /* pass */
        ({ TrStr _at_t3410 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ri)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  %v")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = alloca "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_ty(LFunc_vreg_type(lf, ri))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3410); _tr_str_release(_at_t3410); });
        /* pass */
        ri = (ri + 1LL);
    }
    /* pass */
    if (main_args) {
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit("  %argc64 = sext i32 %argc to i64\n"));
        /* pass */
        LlvmEmitter_w(self, _tr_str_lit("  %argl = call ptr @_tr_rt_argv_list(i64 %argc64, ptr %argv)\n"));
        /* pass */
        ({ TrStr _at_t3411 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(lf->params, 0LL)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  store ptr %argl, ptr %var_")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3411); _tr_str_release(_at_t3411); });
    } else {
        /* pass */
        long long pi = 0LL;
        /* pass */
        while ((pi < lf->params->len)) {
            /* pass */
            TrStr pn2 = List_TrStr_get(lf->params, pi);
            /* pass */
            TrStr pty2 = _ll_ty(LFunc_var_type(lf, pn2));
            /* pass */
            ({ TrStr _at_t3412 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("  store ")), _tr_strz(pty2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" %arg_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", ptr %var_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(pn2)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3412); _tr_str_release(_at_t3412); });
            /* pass */
            pi = (pi + 1LL);
            _tr_str_release(pn2);
            _tr_str_release(pty2);
        }
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("  br label %bb0\n"));
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < lf->blocks->len)) {
        /* pass */
        LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
        /* pass */
        ({ TrStr _at_t3413 = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blk->id)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("bb")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3413); _tr_str_release(_at_t3413); });
        /* pass */
        long long ii = 0LL;
        /* pass */
        while ((ii < blk->insts->len)) {
            /* pass */
            LlvmEmitter_emit_inst(self, (*((LInst*)List_ptr_get(blk->insts, ii))));
            /* pass */
            ii = (ii + 1LL);
        }
        /* pass */
        LlvmEmitter_emit_term(self, blk->term);
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("}\n\n"));
    _tr_str_release(fname);
}

__attribute__((hot)) void LlvmEmitter_emit_extern_decls(LlvmEmitter* self) {
    /* pass */
    TrMap* seen = _tr_dict_new(64LL);
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < self->m->funcs->len)) {
        /* pass */
        LFunc* lf = ((LFunc*)List_ptr_get(self->m->funcs, fi));
        /* pass */
        LFunc* _cltmp_t3414 = _tr_obj_retain(lf);
        _tr_obj_release(self->cur, _trdrop_LFunc);
        self->cur = _cltmp_t3414;
        /* pass */
        long long bi = 0LL;
        /* pass */
        while ((bi < lf->blocks->len)) {
            /* pass */
            LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
            /* pass */
            long long ii = 0LL;
            /* pass */
            while ((ii < blk->insts->len)) {
                /* pass */
                LlvmEmitter_scan_call_decl(self, (*((LInst*)List_ptr_get(blk->insts, ii))), seen);
                /* pass */
                ii = (ii + 1LL);
            }
            /* pass */
            bi = (bi + 1LL);
        }
        /* pass */
        fi = (fi + 1LL);
    }
}

__attribute__((hot)) void LlvmEmitter_scan_call_decl(LlvmEmitter* self, LInst inst, TrMap* seen) {
    /* pass */
    __auto_type _t3415 = inst;
    if (_t3415.tag == LInst_ICall) {
        __auto_type dst = _t3415.data.ICall.dst;
__auto_type callee = _t3415.data.ICall.callee;
__auto_type args = _t3415.data.ICall.args;
        /* pass */
        if (LModule_is_user_fn(self->m, callee)) {
            /* pass */
            return;
        }
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        TrStr retty = _tr_str_lit("void");
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3416 = LlvmEmitter_vty(self, dst);
            _tr_str_release(retty);
            retty = _strtmp_t3416;
        }
        /* pass */
        TrStr params = _tr_str_lit("");
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < args->len)) {
            /* pass */
            if ((ai > 0LL)) {
                /* pass */
                TrStr _strtmp_t3417 = _tr_strx_concat(_tr_strz(params), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(params);
                params = _strtmp_t3417;
            }
            /* pass */
            TrStr _strtmp_t3418 = ({ TrStr _cr = (LlvmEmitter_vty(self, List_i64_get(args, ai))); TrStr _cres = _tr_strx_concat(_tr_strz(params), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(params);
            params = _strtmp_t3418;
            /* pass */
            ai = (ai + 1LL);
        }
        /* pass */
        TrStr _na = _tr_str_lit("");
        /* pass */
        if (((strcmp(_tr_strz(retty), _tr_strz(_tr_str_lit("ptr"))) == 0) && _is_fresh_alloc_ret(callee))) {
            /* pass */
            TrStr _strtmp_t3419 = _tr_str_lit("noalias ");
            _tr_str_release(_na);
            _na = _strtmp_t3419;
        }
        /* pass */
        ({ TrStr _at_t3420 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("declare ")), _tr_strz(_na))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(retty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(params)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3420); _tr_str_release(_at_t3420); });
        _tr_str_release(retty);
        _tr_str_release(params);
        _tr_str_release(_na);
    } else if (_t3415.tag == LInst_IFCall1) {
        __auto_type dst = _t3415.data.IFCall1.dst;
__auto_type callee = _t3415.data.IFCall1.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        TrStr rt1 = _tr_str_lit("void");
        /* pass */
        if ((dst >= 0LL)) {
            /* pass */
            TrStr _strtmp_t3421 = LlvmEmitter_vty(self, dst);
            _tr_str_release(rt1);
            rt1 = _strtmp_t3421;
        }
        /* pass */
        ({ TrStr _at_t3422 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("declare ")), _tr_strz(rt1))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(callee)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double)\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3422); _tr_str_release(_at_t3422); });
        _tr_str_release(rt1);
    } else if (_t3415.tag == LInst_IFCallF) {
        __auto_type callee = _t3415.data.IFCallF.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        ({ TrStr _at_t3423 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("declare double @")), _tr_strz(callee))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double)\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3423); _tr_str_release(_at_t3423); });
    } else if (_t3415.tag == LInst_IFCall2F) {
        __auto_type callee = _t3415.data.IFCall2F.callee;
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(callee))) {
            /* pass */
            return;
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(callee), true);
        /* pass */
        ({ TrStr _at_t3424 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("declare double @")), _tr_strz(callee))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(double, double)\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3424); _tr_str_release(_at_t3424); });
    } else if (1) {
        __auto_type _ = _t3415;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) TrStr LlvmEmitter_emit_module(LlvmEmitter* self) {
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("; Tauraro LLVM IR — backend A (taumir LIR -> LLVM IR -> llc)\n\n"));
    /* pass */
    long long si = 0LL;
    /* pass */
    while ((si < self->m->strings->len)) {
        /* pass */
        TrStr s = List_TrStr_get(self->m->strings, si);
        /* pass */
        long long blen = (_ll_str_bytelen(s) + 1LL);
        /* pass */
        ({ TrStr _at_t3425 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(si)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@.str.")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = private unnamed_addr constant ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(blen)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" x i8] c\""))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_ll_str_escape(s)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\\00\"\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3425); _tr_str_release(_at_t3425); });
        /* pass */
        si = (si + 1LL);
        _tr_str_release(s);
    }
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < self->m->globals->len)) {
        /* pass */
        long long gan = List_i64_get(self->m->global_arr, gi);
        /* pass */
        if ((gan > 0LL)) {
            /* pass */
            ({ TrStr _at_t3426 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@g.")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = internal global ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gan)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" x i64] zeroinitializer\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3426); _tr_str_release(_at_t3426); });
        } else {
            /* pass */
            TrStr gty = _ll_ty(List_i64_get(self->m->global_types, gi));
            /* pass */
            TrStr init = _tr_str_lit("0");
            /* pass */
            if ((strcmp(_tr_strz(gty), _tr_strz(_tr_str_lit("double"))) == 0)) {
                /* pass */
                TrStr _strtmp_t3427 = _tr_str_lit("0.0");
                _tr_str_release(init);
                init = _strtmp_t3427;
            }
            /* pass */
            if ((strcmp(_tr_strz(gty), _tr_strz(_tr_str_lit("ptr"))) == 0)) {
                /* pass */
                TrStr _strtmp_t3428 = _tr_str_lit("null");
                _tr_str_release(init);
                init = _strtmp_t3428;
            }
            /* pass */
            ({ TrStr _at_t3429 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(gi)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("@g.")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = internal global "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(init)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); LlvmEmitter_w(self, _at_t3429); _tr_str_release(_at_t3429); });
            _tr_str_release(gty);
            _tr_str_release(init);
        }
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("\n"));
    /* pass */
    LlvmEmitter_emit_extern_decls(self);
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("declare ptr @_tr_rt_argv_list(i64, ptr)\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("\n"));
    /* pass */
    List_TrStr* emitted = (void*)List_TrStr_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < self->m->funcs->len)) {
        /* pass */
        LFunc* efn = ((LFunc*)List_ptr_get(self->m->funcs, fi));
        /* pass */
        bool dup = false;
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < emitted->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(emitted, ei)), _tr_strz(efn->name)) == 0)) {
                /* pass */
                dup = true;
                /* pass */
                ei = emitted->len;
            } else {
                /* pass */
                ei = (ei + 1LL);
            }
        }
        /* pass */
        if ((!dup)) {
            /* pass */
            List_TrStr_append(emitted, efn->name);
            /* pass */
            LlvmEmitter_emit_function(self, efn);
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!0 = !{!\"tauraro_tbaa_root\"}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!1 = !{!\"listhdr\", !0, i64 0}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!2 = !{!\"listelem\", !0, i64 0}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!3 = !{!\"objfield\", !0, i64 0}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!4 = !{!1, !1, i64 0}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!5 = !{!2, !2, i64 0}\n"));
    /* pass */
    LlvmEmitter_w(self, _tr_str_lit("!6 = !{!3, !3, i64 0}\n"));
    /* pass */
    List_TrStr_free(emitted);
    return StringObj_as_str(StringBuilder_to_string(self->out));
}

__attribute__((hot)) TrStr _tbaa_suffix(long long aclass) {
    /* pass */
    if ((aclass == 1LL)) {
        /* pass */
        return _tr_str_lit(", !tbaa !4");
    }
    /* pass */
    if ((aclass == 2LL)) {
        /* pass */
        return _tr_str_lit(", !tbaa !5");
    }
    /* pass */
    if ((aclass == 3LL)) {
        /* pass */
        return _tr_str_lit(", !tbaa !6");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool _is_fresh_alloc_ret(TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("malloc"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("calloc"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_c_malloc"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_c_calloc"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_checked_alloc"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_raw_alloc"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_list_new"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_argv_list"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_blist_new"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_idict_new"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_sdict_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_dict_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_idict_new"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_set_new"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_set_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_iset_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_pset_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_sset_new"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_obj_alloc"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_obj_alloc"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_str_new"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_str_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_rt_str_alloc"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_char_to_str_alloc"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_bytes_new"))) == 0) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_range_new"))) == 0)) || (strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("_tr_shared_new"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _ll_int_instr(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        return _tr_str_lit("add");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        return _tr_str_lit("sub");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        return _tr_str_lit("mul");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        return _tr_str_lit("sdiv");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("//"))) == 0)) {
        /* pass */
        return _tr_str_lit("sdiv");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        return _tr_str_lit("srem");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0)) {
        /* pass */
        return _tr_str_lit("and");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("|"))) == 0)) {
        /* pass */
        return _tr_str_lit("or");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("^"))) == 0)) {
        /* pass */
        return _tr_str_lit("xor");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<<"))) == 0)) {
        /* pass */
        return _tr_str_lit("shl");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">>"))) == 0)) {
        /* pass */
        return _tr_str_lit("ashr");
    }
    /* pass */
    return _tr_str_lit("add");
}

__attribute__((hot)) TrStr _ll_icmp_pred(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return _tr_str_lit("eq");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return _tr_str_lit("ne");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        return _tr_str_lit("slt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        return _tr_str_lit("sle");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        return _tr_str_lit("sgt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        return _tr_str_lit("sge");
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr _ll_float_instr(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        return _tr_str_lit("fadd");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        return _tr_str_lit("fsub");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        return _tr_str_lit("fmul");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        return _tr_str_lit("fdiv");
    }
    /* pass */
    return _tr_str_lit("fadd");
}

__attribute__((hot)) TrStr _ll_fcmp_pred(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return _tr_str_lit("oeq");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return _tr_str_lit("one");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        return _tr_str_lit("olt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        return _tr_str_lit("ole");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        return _tr_str_lit("ogt");
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        return _tr_str_lit("oge");
    }
    /* pass */
    return _tr_str_lit("");
}

