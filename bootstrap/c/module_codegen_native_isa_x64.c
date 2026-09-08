#include "tauraro_types.h"

long long _argreg_modrm(long long idx);
long long _argreg_rex(long long idx);
long long _round16(long long n);
long long _vreg_disp(long long id);
long long _var_disp(LFunc* lf, TrStr name);
void _st_rax(ByteBuf* c, long long disp);
void _ld_rax(ByteBuf* c, long long disp);
void _ld_rcx(ByteBuf* c, long long disp);
void _ld_argreg(ByteBuf* c, long long idx, long long disp);
void _st_argreg(ByteBuf* c, long long idx, long long disp);
void _mov_rax_imm64(ByteBuf* c, long long v);
void _ld_xmm0(ByteBuf* c, long long disp);
void _ld_xmm1(ByteBuf* c, long long disp);
void _st_xmm0(ByteBuf* c, long long disp);
void _emit_farith(ByteBuf* c, TrStr op);
long long _fsetcc(TrStr op);
bool _is_cmp(TrStr op);
long long _setcc(TrStr op);
void _emit_arith(ByteBuf* c, TrStr op);
void _emit_return(ByteBuf* c, long long rv);

__attribute__((malloc,returns_nonnull,hot)) EncodedFunc* EncodedFunc_init(TrStr name) {
    /* pass */
    EncodedFunc* e = ((EncodedFunc*)_tr_obj_alloc(sizeof(EncodedFunc)));
    /* pass */
    e->name = _tr_str_retain(name);
    /* pass */
    e->is_main = false;
    /* pass */
    ByteBuf* _cltmp_t3332 = ByteBuf_init();
    _tr_obj_release(e->code, _trdrop_ByteBuf);
    e->code = _cltmp_t3332;
    /* pass */
    e->relocs = (void*)List_ptr_new();
    /* pass */
    return e;
}

__attribute__((hot)) long long _argreg_modrm(long long idx) {
    /* pass */
    if ((idx == 0LL)) {
        /* pass */
        return 189LL;
    }
    /* pass */
    if ((idx == 1LL)) {
        /* pass */
        return 181LL;
    }
    /* pass */
    if ((idx == 2LL)) {
        /* pass */
        return 149LL;
    }
    /* pass */
    if ((idx == 3LL)) {
        /* pass */
        return 141LL;
    }
    /* pass */
    if ((idx == 4LL)) {
        /* pass */
        return 133LL;
    }
    /* pass */
    return 141LL;
}

__attribute__((hot)) long long _argreg_rex(long long idx) {
    /* pass */
    if ((idx >= 4LL)) {
        /* pass */
        return 76LL;
    }
    /* pass */
    return 72LL;
}

__attribute__((hot)) long long _round16(long long n) {
    /* pass */
    long long r = n;
    /* pass */
    while (((r % 16LL) != 0LL)) {
        /* pass */
        r = (r + 1LL);
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long _vreg_disp(long long id) {
    /* pass */
    return (0LL - (8LL * (id + 1LL)));
}

__attribute__((hot)) long long _var_disp(LFunc* lf, TrStr name) {
    /* pass */
    long long idx = LFunc_var_index(lf, name);
    /* pass */
    if ((idx < 0LL)) {
        /* pass */
        idx = 0LL;
    }
    /* pass */
    return (0LL - (8LL * ((lf->n_vregs + idx) + 1LL)));
}

__attribute__((hot)) void _st_rax(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rax(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_rcx(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, 141LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_argreg(ByteBuf* c, long long idx, long long disp) {
    /* pass */
    ByteBuf_u8(c, _argreg_rex(idx));
    /* pass */
    ByteBuf_u8(c, 139LL);
    /* pass */
    ByteBuf_u8(c, _argreg_modrm(idx));
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _st_argreg(ByteBuf* c, long long idx, long long disp) {
    /* pass */
    ByteBuf_u8(c, _argreg_rex(idx));
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, _argreg_modrm(idx));
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _mov_rax_imm64(ByteBuf* c, long long v) {
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 184LL);
    /* pass */
    ByteBuf_u64(c, v);
}

__attribute__((hot)) void _ld_xmm0(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 242LL);
    /* pass */
    ByteBuf_u8(c, 15LL);
    /* pass */
    ByteBuf_u8(c, 16LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _ld_xmm1(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 242LL);
    /* pass */
    ByteBuf_u8(c, 15LL);
    /* pass */
    ByteBuf_u8(c, 16LL);
    /* pass */
    ByteBuf_u8(c, 141LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _st_xmm0(ByteBuf* c, long long disp) {
    /* pass */
    ByteBuf_u8(c, 242LL);
    /* pass */
    ByteBuf_u8(c, 15LL);
    /* pass */
    ByteBuf_u8(c, 17LL);
    /* pass */
    ByteBuf_u8(c, 133LL);
    /* pass */
    ByteBuf_u32(c, disp);
}

__attribute__((hot)) void _emit_farith(ByteBuf* c, TrStr op) {
    /* pass */
    ByteBuf_u8(c, 242LL);
    /* pass */
    ByteBuf_u8(c, 15LL);
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 88LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 92LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 89LL);
    } else {
        /* pass */
        ByteBuf_u8(c, 94LL);
    }
    /* pass */
    ByteBuf_u8(c, 193LL);
}

__attribute__((hot)) long long _fsetcc(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        return 146LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        return 151LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return 148LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return 149LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        return 150LL;
    }
    /* pass */
    return 147LL;
}

__attribute__((hot)) bool _is_cmp(TrStr op) {
    /* pass */
    return ((((((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0));
}

__attribute__((hot)) long long _setcc(TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        return 156LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        return 159LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return 148LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return 149LL;
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        return 158LL;
    }
    /* pass */
    return 157LL;
}

__attribute__((hot)) void _emit_arith(ByteBuf* c, TrStr op) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 1LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 41LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 15LL);
        /* pass */
        ByteBuf_u8(c, 175LL);
        /* pass */
        ByteBuf_u8(c, 193LL);
    } else if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("/"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("//"))) == 0))) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 153LL);
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 247LL);
        /* pass */
        ByteBuf_u8(c, 249LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 153LL);
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 247LL);
        /* pass */
        ByteBuf_u8(c, 249LL);
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 137LL);
        /* pass */
        ByteBuf_u8(c, 208LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("&"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 33LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("|"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 9LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("^"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 49LL);
        /* pass */
        ByteBuf_u8(c, 200LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<<"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 211LL);
        /* pass */
        ByteBuf_u8(c, 224LL);
    } else if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">>"))) == 0)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 211LL);
        /* pass */
        ByteBuf_u8(c, 248LL);
    }
}

__attribute__((hot)) void _emit_return(ByteBuf* c, long long rv) {
    /* pass */
    if ((rv == 0LL)) {
        /* pass */
        ByteBuf_u8(c, 49LL);
        /* pass */
        ByteBuf_u8(c, 192LL);
    } else {
        /* pass */
        ByteBuf_u8(c, 184LL);
        /* pass */
        ByteBuf_u32(c, rv);
    }
    /* pass */
    ByteBuf_u8(c, 201LL);
    /* pass */
    ByteBuf_u8(c, 195LL);
}

__attribute__((hot)) EncodedFunc* encode_func(LFunc* lf) {
    /* pass */
    EncodedFunc* e = EncodedFunc_init(lf->name);
    /* pass */
    e->is_main = lf->is_main;
    /* pass */
    ByteBuf* c = _tr_obj_retain(e->code);
    /* pass */
    long long framesize = _round16(((lf->n_vregs + lf->vars->len) * 8LL));
    /* pass */
    ByteBuf_u8(c, 85LL);
    /* pass */
    ByteBuf_u8(c, 72LL);
    /* pass */
    ByteBuf_u8(c, 137LL);
    /* pass */
    ByteBuf_u8(c, 229LL);
    /* pass */
    if ((framesize > 0LL)) {
        /* pass */
        ByteBuf_u8(c, 72LL);
        /* pass */
        ByteBuf_u8(c, 129LL);
        /* pass */
        ByteBuf_u8(c, 236LL);
        /* pass */
        ByteBuf_u32(c, framesize);
    }
    /* pass */
    long long ppi = 0LL;
    /* pass */
    while ((ppi < lf->params->len)) {
        /* pass */
        ({ TrStr _at_t3333 = (List_TrStr_get(lf->params, ppi)); _st_argreg(c, ppi, _var_disp(lf, _at_t3333)); _tr_str_release(_at_t3333); });
        /* pass */
        ppi = (ppi + 1LL);
    }
    /* pass */
    List_i64* block_start = (void*)List_i64_new();
    /* pass */
    List_ptr* jumps = (void*)List_ptr_new();
    /* pass */
    long long bi = 0LL;
    /* pass */
    while ((bi < lf->blocks->len)) {
        /* pass */
        LBlock* blk = ((LBlock*)List_ptr_get(lf->blocks, bi));
        /* pass */
        List_i64_append(block_start, c->len);
        /* pass */
        long long ii = 0LL;
        /* pass */
        while ((ii < blk->insts->len)) {
            /* pass */
            __auto_type _t3334 = (*((LInst*)List_ptr_get(blk->insts, ii)));
            if (_t3334.tag == LInst_IConst) {
                __auto_type dst = _t3334.data.IConst.dst;
__auto_type v = _t3334.data.IConst.v;
                /* pass */
                _mov_rax_imm64(c, v);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IStr) {
                __auto_type dst = _t3334.data.IStr.dst;
__auto_type sidx = _t3334.data.IStr.str_idx;
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 141LL);
                /* pass */
                ByteBuf_u8(c, 5LL);
                /* pass */
                Reloc* sr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                sr->offset = c->len;
                /* pass */
                sr->symbol = _tr_str_lit("");
                /* pass */
                sr->kind = 1LL;
                /* pass */
                sr->str_idx = sidx;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(sr));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(sr, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_ILoadVar) {
                __auto_type dst = _t3334.data.ILoadVar.dst;
__auto_type name = _t3334.data.ILoadVar.name;
                /* pass */
                _ld_rax(c, _var_disp(lf, name));
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IStoreVar) {
                __auto_type name = _t3334.data.IStoreVar.name;
__auto_type src = _t3334.data.IStoreVar.src;
                /* pass */
                _ld_rax(c, _vreg_disp(src));
                /* pass */
                _st_rax(c, _var_disp(lf, name));
            } else if (_t3334.tag == LInst_ILoadGlobal) {
                __auto_type dst = _t3334.data.ILoadGlobal.dst;
__auto_type gidx = _t3334.data.ILoadGlobal.gidx;
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 139LL);
                /* pass */
                ByteBuf_u8(c, 5LL);
                /* pass */
                Reloc* glr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                glr->offset = c->len;
                /* pass */
                glr->symbol = _tr_str_lit("");
                /* pass */
                glr->kind = 3LL;
                /* pass */
                glr->str_idx = gidx;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(glr));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(glr, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IStoreGlobal) {
                __auto_type gidx = _t3334.data.IStoreGlobal.gidx;
__auto_type src = _t3334.data.IStoreGlobal.src;
                /* pass */
                _ld_rax(c, _vreg_disp(src));
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 137LL);
                /* pass */
                ByteBuf_u8(c, 5LL);
                /* pass */
                Reloc* gsr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                gsr->offset = c->len;
                /* pass */
                gsr->symbol = _tr_str_lit("");
                /* pass */
                gsr->kind = 3LL;
                /* pass */
                gsr->str_idx = gidx;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(gsr));
                /* pass */
                ByteBuf_u32(c, 0LL);
                _tr_obj_release(gsr, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IBinOp) {
                __auto_type dst = _t3334.data.IBinOp.dst;
__auto_type op = _t3334.data.IBinOp.op;
__auto_type a = _t3334.data.IBinOp.a;
__auto_type b = _t3334.data.IBinOp.b;
                /* pass */
                _ld_rax(c, _vreg_disp(a));
                /* pass */
                _ld_rcx(c, _vreg_disp(b));
                /* pass */
                if (_is_cmp(op)) {
                    /* pass */
                    ByteBuf_u8(c, 72LL);
                    /* pass */
                    ByteBuf_u8(c, 57LL);
                    /* pass */
                    ByteBuf_u8(c, 200LL);
                    /* pass */
                    ByteBuf_u8(c, 15LL);
                    /* pass */
                    ByteBuf_u8(c, _setcc(op));
                    /* pass */
                    ByteBuf_u8(c, 192LL);
                    /* pass */
                    ByteBuf_u8(c, 72LL);
                    /* pass */
                    ByteBuf_u8(c, 15LL);
                    /* pass */
                    ByteBuf_u8(c, 182LL);
                    /* pass */
                    ByteBuf_u8(c, 192LL);
                } else {
                    /* pass */
                    _emit_arith(c, op);
                }
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_ICall) {
                __auto_type dst = _t3334.data.ICall.dst;
__auto_type callee = _t3334.data.ICall.callee;
__auto_type args = _t3334.data.ICall.args;
                /* pass */
                long long ai = 0LL;
                /* pass */
                while ((ai < args->len)) {
                    /* pass */
                    _ld_argreg(c, ai, _vreg_disp(List_i64_get(args, ai)));
                    /* pass */
                    ai = (ai + 1LL);
                }
                /* pass */
                ByteBuf_u8(c, 232LL);
                /* pass */
                Reloc* r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                r->offset = c->len;
                /* pass */
                r->symbol = _tr_str_retain(callee);
                /* pass */
                r->kind = 0LL;
                /* pass */
                r->str_idx = 0LL;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(r));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                if ((dst >= 0LL)) {
                    /* pass */
                    _st_rax(c, _vreg_disp(dst));
                }
                _tr_obj_release(r, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IFBinOp) {
                __auto_type dst = _t3334.data.IFBinOp.dst;
__auto_type op = _t3334.data.IFBinOp.op;
__auto_type a = _t3334.data.IFBinOp.a;
__auto_type b = _t3334.data.IFBinOp.b;
                /* pass */
                _ld_xmm0(c, _vreg_disp(a));
                /* pass */
                _ld_xmm1(c, _vreg_disp(b));
                /* pass */
                if (_is_cmp(op)) {
                    /* pass */
                    ByteBuf_u8(c, 102LL);
                    /* pass */
                    ByteBuf_u8(c, 15LL);
                    /* pass */
                    ByteBuf_u8(c, 47LL);
                    /* pass */
                    ByteBuf_u8(c, 193LL);
                    /* pass */
                    ByteBuf_u8(c, 15LL);
                    /* pass */
                    ByteBuf_u8(c, _fsetcc(op));
                    /* pass */
                    ByteBuf_u8(c, 192LL);
                    /* pass */
                    ByteBuf_u8(c, 72LL);
                    /* pass */
                    ByteBuf_u8(c, 15LL);
                    /* pass */
                    ByteBuf_u8(c, 182LL);
                    /* pass */
                    ByteBuf_u8(c, 192LL);
                    /* pass */
                    _st_rax(c, _vreg_disp(dst));
                } else {
                    /* pass */
                    _emit_farith(c, op);
                    /* pass */
                    _st_xmm0(c, _vreg_disp(dst));
                }
            } else if (_t3334.tag == LInst_IIToF) {
                __auto_type dst = _t3334.data.IIToF.dst;
__auto_type src = _t3334.data.IIToF.src;
                /* pass */
                _ld_rax(c, _vreg_disp(src));
                /* pass */
                ByteBuf_u8(c, 242LL);
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 15LL);
                /* pass */
                ByteBuf_u8(c, 42LL);
                /* pass */
                ByteBuf_u8(c, 192LL);
                /* pass */
                _st_xmm0(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IFToI) {
                __auto_type dst = _t3334.data.IFToI.dst;
__auto_type src = _t3334.data.IFToI.src;
                /* pass */
                _ld_xmm0(c, _vreg_disp(src));
                /* pass */
                ByteBuf_u8(c, 242LL);
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 15LL);
                /* pass */
                ByteBuf_u8(c, 44LL);
                /* pass */
                ByteBuf_u8(c, 192LL);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IFCall1) {
                __auto_type dst = _t3334.data.IFCall1.dst;
__auto_type callee = _t3334.data.IFCall1.callee;
__auto_type arg = _t3334.data.IFCall1.arg;
                /* pass */
                _ld_xmm0(c, _vreg_disp(arg));
                /* pass */
                ByteBuf_u8(c, 232LL);
                /* pass */
                Reloc* fr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                fr->offset = c->len;
                /* pass */
                fr->symbol = _tr_str_retain(callee);
                /* pass */
                fr->kind = 0LL;
                /* pass */
                fr->str_idx = 0LL;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(fr));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                if ((dst >= 0LL)) {
                    /* pass */
                    _st_rax(c, _vreg_disp(dst));
                }
                _tr_obj_release(fr, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IFCallF) {
                __auto_type dst = _t3334.data.IFCallF.dst;
__auto_type callee = _t3334.data.IFCallF.callee;
__auto_type arg = _t3334.data.IFCallF.arg;
                /* pass */
                _ld_xmm0(c, _vreg_disp(arg));
                /* pass */
                ByteBuf_u8(c, 232LL);
                /* pass */
                Reloc* ffr = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                ffr->offset = c->len;
                /* pass */
                ffr->symbol = _tr_str_retain(callee);
                /* pass */
                ffr->kind = 0LL;
                /* pass */
                ffr->str_idx = 0LL;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(ffr));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                _st_xmm0(c, _vreg_disp(dst));
                _tr_obj_release(ffr, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IFCall2F) {
                __auto_type dst = _t3334.data.IFCall2F.dst;
__auto_type callee = _t3334.data.IFCall2F.callee;
__auto_type a = _t3334.data.IFCall2F.a;
__auto_type b = _t3334.data.IFCall2F.b;
                /* pass */
                _ld_xmm0(c, _vreg_disp(a));
                /* pass */
                _ld_xmm1(c, _vreg_disp(b));
                /* pass */
                ByteBuf_u8(c, 232LL);
                /* pass */
                Reloc* f2r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                f2r->offset = c->len;
                /* pass */
                f2r->symbol = _tr_str_retain(callee);
                /* pass */
                f2r->kind = 0LL;
                /* pass */
                f2r->str_idx = 0LL;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(f2r));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                _st_xmm0(c, _vreg_disp(dst));
                _tr_obj_release(f2r, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_IBitsF) {
                __auto_type dst = _t3334.data.IBitsF.dst;
__auto_type src = _t3334.data.IBitsF.src;
                /* pass */
                _ld_rax(c, _vreg_disp(src));
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IFBits) {
                __auto_type dst = _t3334.data.IFBits.dst;
__auto_type src = _t3334.data.IFBits.src;
                /* pass */
                _ld_rax(c, _vreg_disp(src));
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IAddrVar) {
                __auto_type dst = _t3334.data.IAddrVar.dst;
__auto_type name = _t3334.data.IAddrVar.name;
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 141LL);
                /* pass */
                ByteBuf_u8(c, 133LL);
                /* pass */
                ByteBuf_u32(c, _var_disp(lf, name));
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IAddrGlobal) {
                /* pass */
                _emit_return(c, 0LL);
            } else if (_t3334.tag == LInst_IFuncAddr) {
                __auto_type dst = _t3334.data.IFuncAddr.dst;
__auto_type fname = _t3334.data.IFuncAddr.fname;
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 141LL);
                /* pass */
                ByteBuf_u8(c, 5LL);
                /* pass */
                Reloc* fnaddr_r = ((Reloc*)_tr_obj_alloc(sizeof(Reloc)));
                /* pass */
                fnaddr_r->offset = c->len;
                /* pass */
                fnaddr_r->symbol = _tr_str_retain(fname);
                /* pass */
                fnaddr_r->kind = 4LL;
                /* pass */
                fnaddr_r->str_idx = 0LL;
                /* pass */
                List_ptr_append(e->relocs, _tr_obj_retain(fnaddr_r));
                /* pass */
                ByteBuf_u32(c, 0LL);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
                _tr_obj_release(fnaddr_r, _trdrop_Reloc);
            } else if (_t3334.tag == LInst_ICallInd) {
                __auto_type dst = _t3334.data.ICallInd.dst;
__auto_type fnreg = _t3334.data.ICallInd.fnreg;
__auto_type iargs = _t3334.data.ICallInd.args;
                /* pass */
                long long ci = 0LL;
                /* pass */
                while ((ci < iargs->len)) {
                    /* pass */
                    _ld_argreg(c, ci, _vreg_disp(List_i64_get(iargs, ci)));
                    /* pass */
                    ci = (ci + 1LL);
                }
                /* pass */
                _ld_rax(c, _vreg_disp(fnreg));
                /* pass */
                ByteBuf_u8(c, 255LL);
                /* pass */
                ByteBuf_u8(c, 208LL);
                /* pass */
                if ((dst >= 0LL)) {
                    /* pass */
                    _st_rax(c, _vreg_disp(dst));
                }
            } else if (_t3334.tag == LInst_IAsm) {
                __auto_type acode = _t3334.data.IAsm.code;
__auto_type acons = _t3334.data.IAsm.cons;
                /* pass */
                /* pass */
            } else if (_t3334.tag == LInst_ILoad) {
                __auto_type dst = _t3334.data.ILoad.dst;
__auto_type base = _t3334.data.ILoad.base;
__auto_type off = _t3334.data.ILoad.off;
                /* pass */
                _ld_rax(c, _vreg_disp(base));
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 139LL);
                /* pass */
                ByteBuf_u8(c, 128LL);
                /* pass */
                ByteBuf_u32(c, off);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IStore) {
                __auto_type base = _t3334.data.IStore.base;
__auto_type off = _t3334.data.IStore.off;
__auto_type src = _t3334.data.IStore.src;
                /* pass */
                _ld_rax(c, _vreg_disp(base));
                /* pass */
                _ld_rcx(c, _vreg_disp(src));
                /* pass */
                ByteBuf_u8(c, 72LL);
                /* pass */
                ByteBuf_u8(c, 137LL);
                /* pass */
                ByteBuf_u8(c, 136LL);
                /* pass */
                ByteBuf_u32(c, off);
            } else if (_t3334.tag == LInst_ILoadB) {
                __auto_type dst = _t3334.data.ILoadB.dst;
__auto_type base = _t3334.data.ILoadB.base;
__auto_type off = _t3334.data.ILoadB.off;
                /* pass */
                _ld_rax(c, _vreg_disp(base));
                /* pass */
                ByteBuf_u8(c, 15LL);
                /* pass */
                ByteBuf_u8(c, 182LL);
                /* pass */
                ByteBuf_u8(c, 128LL);
                /* pass */
                ByteBuf_u32(c, off);
                /* pass */
                _st_rax(c, _vreg_disp(dst));
            } else if (_t3334.tag == LInst_IStoreB) {
                __auto_type base = _t3334.data.IStoreB.base;
__auto_type off = _t3334.data.IStoreB.off;
__auto_type src = _t3334.data.IStoreB.src;
                /* pass */
                _ld_rax(c, _vreg_disp(base));
                /* pass */
                _ld_rcx(c, _vreg_disp(src));
                /* pass */
                ByteBuf_u8(c, 136LL);
                /* pass */
                ByteBuf_u8(c, 136LL);
                /* pass */
                ByteBuf_u32(c, off);
            }
            /* pass */
            ii = (ii + 1LL);
        }
        /* pass */
        __auto_type _t3335 = blk->term;
        if (_t3335.tag == LTerm_TRetInt) {
            __auto_type rv = _t3335.data.TRetInt.v;
            /* pass */
            _emit_return(c, rv);
        } else if (_t3335.tag == LTerm_TRetVal) {
            __auto_type rvreg = _t3335.data.TRetVal.v;
            /* pass */
            _ld_rax(c, _vreg_disp(rvreg));
            /* pass */
            ByteBuf_u8(c, 201LL);
            /* pass */
            ByteBuf_u8(c, 195LL);
        } else if (_t3335.tag == LTerm_TRetVoid) {
            /* pass */
            _emit_return(c, 0LL);
        } else if (_t3335.tag == LTerm_TBr) {
            __auto_type target = _t3335.data.TBr.target;
            /* pass */
            ByteBuf_u8(c, 233LL);
            /* pass */
            Jump* jp = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
            /* pass */
            jp->patch_off = c->len;
            /* pass */
            jp->target = target;
            /* pass */
            List_ptr_append(jumps, _tr_obj_retain(jp));
            /* pass */
            ByteBuf_u32(c, 0LL);
            _tr_obj_release(jp, _trdrop_Jump);
        } else if (_t3335.tag == LTerm_TCondBr) {
            __auto_type cond = _t3335.data.TCondBr.cond;
__auto_type tb = _t3335.data.TCondBr.then_b;
__auto_type eb = _t3335.data.TCondBr.else_b;
            /* pass */
            _ld_rax(c, _vreg_disp(cond));
            /* pass */
            ByteBuf_u8(c, 72LL);
            /* pass */
            ByteBuf_u8(c, 133LL);
            /* pass */
            ByteBuf_u8(c, 192LL);
            /* pass */
            ByteBuf_u8(c, 15LL);
            /* pass */
            ByteBuf_u8(c, 133LL);
            /* pass */
            Jump* jp2 = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
            /* pass */
            jp2->patch_off = c->len;
            /* pass */
            jp2->target = tb;
            /* pass */
            List_ptr_append(jumps, _tr_obj_retain(jp2));
            /* pass */
            ByteBuf_u32(c, 0LL);
            /* pass */
            ByteBuf_u8(c, 233LL);
            /* pass */
            Jump* jp3 = ((Jump*)_tr_obj_alloc(sizeof(Jump)));
            /* pass */
            jp3->patch_off = c->len;
            /* pass */
            jp3->target = eb;
            /* pass */
            List_ptr_append(jumps, _tr_obj_retain(jp3));
            /* pass */
            ByteBuf_u32(c, 0LL);
            _tr_obj_release(jp2, _trdrop_Jump);
            _tr_obj_release(jp3, _trdrop_Jump);
        } else if (1) {
            __auto_type _ = _t3335;
            /* pass */
            _emit_return(c, 0LL);
        }
        /* pass */
        bi = (bi + 1LL);
    }
    /* pass */
    long long ji = 0LL;
    /* pass */
    while ((ji < jumps->len)) {
        /* pass */
        Jump* j = ((Jump*)List_ptr_get(jumps, ji));
        /* pass */
        long long rel = (List_i64_get(block_start, j->target) - (j->patch_off + 4LL));
        /* pass */
        ByteBuf_patch_u32(c, j->patch_off, rel);
        /* pass */
        ji = (ji + 1LL);
    }
    /* pass */
    _tr_obj_release(c, _trdrop_ByteBuf);
    List_i64_free(block_start);
    List_ptr_free_obj(jumps, _trdrop_Jump);
    return e;
}

