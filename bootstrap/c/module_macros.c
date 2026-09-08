#include "tauraro_types.h"

MacroVal* str_list(List_TrStr* items);
MacroVal* param_rec(Param* p);
MacroVal* params_list(List_ptr* ps);
MacroVal* fn_rec(FunctionDef* f);
MacroVal* method_list(List_ptr* ms);
void parse_into(TrStr src, List_ptr* gen);
List_ptr* decl_decorators(Decl* declptr);
TrStr render_arg(Expr* e);
Expr* parse_expr_src(TrStr src);
TrStr kind_of(Expr* e);
MacroVal* arg_rec(Expr* e);
MacroCtx* run_fn_macro(FunctionDef* mdef, List_ptr* args, long long gbase);

__attribute__((malloc,returns_nonnull,hot)) MacroCtx* MacroCtx_init() {
    /* pass */
    MacroCtx* c = ((MacroCtx*)_tr_obj_alloc(sizeof(MacroCtx)));
    /* pass */
    c->env = _tr_dict_new(16LL);
    /* pass */
    c->returned = false;
    /* pass */
    c->result = _tr_str_lit("");
    /* pass */
    c->has_error = false;
    /* pass */
    c->error_msg = _tr_str_lit("");
    /* pass */
    c->gensym_ctr = 0LL;
    /* pass */
    return c;
}

__attribute__((hot)) void MacroCtx_fail(MacroCtx* self, TrStr msg) {
    /* pass */
    if ((!self->has_error)) {
        /* pass */
        self->has_error = true;
        /* pass */
        self->error_msg = _tr_str_retain(msg);
    }
}

__attribute__((hot)) MacroVal* MacroCtx_eval_binop(MacroCtx* self, TrStr op, MacroVal* lv, MacroVal* rv) {
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        __auto_type _t3506 = (*lv);
        if (_t3506.tag == MacroVal_MInt) {
            __auto_type a = _t3506.data.MInt.n;
            /* pass */
            __auto_type _t3507 = (*rv);
            if (_t3507.tag == MacroVal_MInt) {
                __auto_type b = _t3507.data.MInt.n;
                return box_mv(MacroVal_ctor_MInt((a + b)));
            } else if (1) {
                __auto_type _ = _t3507;
                return ({ TrStr _at_t3508 = (({ TrStr _cl = (mv_to_str(lv)); TrStr _cr = (mv_to_str(rv)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t3508))); _tr_str_release(_at_t3508); _wr; });
            }
        } else if (1) {
            __auto_type _ = _t3506;
            return ({ TrStr _at_t3509 = (({ TrStr _cl = (mv_to_str(lv)); TrStr _cr = (mv_to_str(rv)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t3509))); _tr_str_release(_at_t3509); _wr; });
        }
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool(mv_eq(lv, rv)));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((!mv_eq(lv, rv))));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("and"))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) && mv_truthy(rv))));
    }
    /* pass */
    if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("or"))) == 0)) {
        /* pass */
        return box_mv(MacroVal_ctor_MBool((mv_truthy(lv) || mv_truthy(rv))));
    }
    /* pass */
    __auto_type _t3510 = (*lv);
    if (_t3510.tag == MacroVal_MInt) {
        __auto_type a = _t3510.data.MInt.n;
        /* pass */
        __auto_type _t3511 = (*rv);
        if (_t3511.tag == MacroVal_MInt) {
            __auto_type b = _t3511.data.MInt.n;
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a < b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a > b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("<="))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a <= b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(">="))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MBool((a >= b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MInt((a - b)));
            }
            /* pass */
            if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("*"))) == 0)) {
                /* pass */
                return box_mv(MacroVal_ctor_MInt((a * b)));
            }
        } else if (1) {
            __auto_type _ = _t3511;
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t3510;
        /* pass */
    }
    /* pass */
    ({ TrStr _at_t3512 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unsupported operator '")), _tr_strz(op))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in macro body"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t3512); _tr_str_release(_at_t3512); });
    /* pass */
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) MacroVal* MacroCtx_eval_mexpr(MacroCtx* self, Expr* eptr) {
    /* pass */
    if ((((unsigned long long)(eptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t3513 = (*eptr);
    if (_t3513.tag == Expr_ELitStr) {
        __auto_type s = _t3513.data.ELitStr.val;
        return box_mv(MacroVal_ctor_MStr(s));
    } else if (_t3513.tag == Expr_ERawStr) {
        __auto_type s = _t3513.data.ERawStr.val;
        return box_mv(MacroVal_ctor_MStr(s));
    } else if (_t3513.tag == Expr_ELitInt) {
        __auto_type n = _t3513.data.ELitInt.val;
        return box_mv(MacroVal_ctor_MInt(n));
    } else if (_t3513.tag == Expr_ELitBool) {
        __auto_type b = _t3513.data.ELitBool.val;
        return box_mv(MacroVal_ctor_MBool(b));
    } else if (_t3513.tag == Expr_EIdent) {
        __auto_type name = _t3513.data.EIdent.name;
        /* pass */
        if (_tr_dict_contains(self->env, _tr_strz(name))) {
            /* pass */
            return ((MacroVal*)(uintptr_t)_tr_dict_get(self->env, _tr_strz(name)));
        }
        /* pass */
        ({ TrStr _at_t3514 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unknown name '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t3514); _tr_str_release(_at_t3514); });
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t3513.tag == Expr_EBinOp) {
        __auto_type op = _t3513.data.EBinOp.op;
__auto_type l = _t3513.data.EBinOp.left;
__auto_type r = _t3513.data.EBinOp.right;
        /* pass */
        return MacroCtx_eval_binop(self, op, MacroCtx_eval_mexpr(self, l), MacroCtx_eval_mexpr(self, r));
    } else if (_t3513.tag == Expr_EUnaryOp) {
        __auto_type op = _t3513.data.EUnaryOp.op;
__auto_type x = _t3513.data.EUnaryOp.expr;
        /* pass */
        MacroVal* xv = MacroCtx_eval_mexpr(self, x);
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("not"))) == 0)) {
            /* pass */
            return box_mv(MacroVal_ctor_MBool((!mv_truthy(xv))));
        }
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("-"))) == 0)) {
            /* pass */
            __auto_type _t3515 = (*xv);
            if (_t3515.tag == MacroVal_MInt) {
                __auto_type n = _t3515.data.MInt.n;
                return box_mv(MacroVal_ctor_MInt((0LL - n)));
            } else if (1) {
                __auto_type _ = _t3515;
                /* pass */
            }
        }
        /* pass */
        ({ TrStr _at_t3516 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unsupported unary operator '")), _tr_strz(op))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in macro body"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t3516); _tr_str_release(_at_t3516); });
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t3513.tag == Expr_EFString) {
        __auto_type parts = _t3513.data.EFString.parts;
        /* pass */
        TrStr out = _tr_str_lit("");
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            FStringPart* part = ((FStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (part->is_expr) {
                /* pass */
                TrStr _strtmp_t3517 = ({ TrStr _cr = (mv_to_str(MacroCtx_eval_mexpr(self, part->expr))); TrStr _cres = _tr_strx_concat(_tr_strz(out), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(out);
                out = _strtmp_t3517;
            } else {
                /* pass */
                TrStr _strtmp_t3518 = _tr_strx_concat(_tr_strz(out), _tr_strz(part->text));
                _tr_str_release(out);
                out = _strtmp_t3518;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return box_mv(MacroVal_ctor_MStr(out));
    } else if (_t3513.tag == Expr_EIndex) {
        __auto_type obj = _t3513.data.EIndex.obj;
__auto_type idx = _t3513.data.EIndex._tr_v_index;
        /* pass */
        MacroVal* ov = MacroCtx_eval_mexpr(self, obj);
        /* pass */
        MacroVal* iv = MacroCtx_eval_mexpr(self, idx);
        /* pass */
        __auto_type _t3519 = (*ov);
        if (_t3519.tag == MacroVal_MList) {
            __auto_type items = _t3519.data.MList.items;
            /* pass */
            __auto_type _t3520 = (*iv);
            if (_t3520.tag == MacroVal_MInt) {
                __auto_type n = _t3520.data.MInt.n;
                /* pass */
                if (((n >= 0LL) && (n < items->len))) {
                    /* pass */
                    return ((MacroVal*)List_ptr_get(items, n));
                }
                /* pass */
                MacroCtx_fail(self, _tr_str_lit("macro: list index out of range"));
            } else if (1) {
                __auto_type _ = _t3520;
                MacroCtx_fail(self, _tr_str_lit("macro: list index must be an int"));
            }
        } else if (1) {
            __auto_type _ = _t3519;
            MacroCtx_fail(self, _tr_str_lit("macro: indexing requires a list"));
        }
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t3513.tag == Expr_EPropAccess) {
        __auto_type obj = _t3513.data.EPropAccess.obj;
__auto_type prop = _t3513.data.EPropAccess.prop;
        /* pass */
        return mrec_get(MacroCtx_eval_mexpr(self, obj), prop);
    } else if (_t3513.tag == Expr_EMethodCall) {
        __auto_type obj = _t3513.data.EMethodCall.obj;
__auto_type method = _t3513.data.EMethodCall.method;
__auto_type margs = _t3513.data.EMethodCall.args;
        /* pass */
        MacroVal* ov = MacroCtx_eval_mexpr(self, obj);
        /* pass */
        if ((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("len"))) == 0)) {
            /* pass */
            __auto_type _t3521 = (*ov);
            if (_t3521.tag == MacroVal_MList) {
                __auto_type items = _t3521.data.MList.items;
                return box_mv(MacroVal_ctor_MInt(items->len));
            } else if (1) {
                __auto_type _ = _t3521;
                /* pass */
            }
            /* pass */
            return box_mv(MacroVal_ctor_MInt(0LL));
        }
        /* pass */
        if (((strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_str"))) == 0) || (strcmp(_tr_strz(method), _tr_strz(_tr_str_lit("to_string"))) == 0))) {
            /* pass */
            return ({ TrStr _at_t3522 = (mv_to_str(ov)); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t3522))); _tr_str_release(_at_t3522); _wr; });
        }
        /* pass */
        ({ TrStr _at_t3523 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("macro: unsupported method '")), _tr_strz(method))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("' in macro body"))); _tr_str_release(_cl); _cres; })); MacroCtx_fail(self, _at_t3523); _tr_str_release(_at_t3523); });
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t3513.tag == Expr_ECall) {
        __auto_type callee = _t3513.data.ECall.callee;
__auto_type cargs = _t3513.data.ECall.args;
        /* pass */
        __auto_type _t3524 = (*callee);
        if (_t3524.tag == Expr_EIdent) {
            __auto_type cn = _t3524.data.EIdent.name;
            /* pass */
            if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("macro_error"))) == 0)) {
                /* pass */
                TrStr msg = _tr_str_lit("macro_error");
                /* pass */
                if ((cargs->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t3525 = mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))));
                    _tr_str_release(msg);
                    msg = _strtmp_t3525;
                }
                /* pass */
                MacroCtx_fail(self, msg);
                /* pass */
                _tr_str_release(msg);
                return box_mv(MacroVal_make_MNil());
            }
            /* pass */
            if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("gensym"))) == 0)) {
                /* pass */
                self->gensym_ctr = (self->gensym_ctr + 1LL);
                /* pass */
                TrStr pfx = _tr_str_lit("g");
                /* pass */
                if ((cargs->len > 0LL)) {
                    /* pass */
                    TrStr _strtmp_t3526 = mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))));
                    _tr_str_release(pfx);
                    pfx = _strtmp_t3526;
                }
                /* pass */
                return ({ TrStr _at_t3527 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("__hyg_")), _tr_strz(pfx))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(self->gensym_ctr)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t3527))); _tr_str_release(_at_t3527); _wr; });
            }
            /* pass */
            if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("str"))) == 0)) {
                /* pass */
                if ((cargs->len > 0LL)) {
                    /* pass */
                    return ({ TrStr _at_t3528 = (mv_to_str(MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(cargs, 0LL))))); __auto_type _wr = (box_mv(MacroVal_ctor_MStr(_at_t3528))); _tr_str_release(_at_t3528); _wr; });
                }
                /* pass */
                return box_mv(MacroVal_ctor_MStr(_tr_str_lit("")));
            }
        } else if (1) {
            __auto_type _ = _t3524;
            /* pass */
        }
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported call in macro body"));
        /* pass */
        return box_mv(MacroVal_make_MNil());
    } else if (_t3513.tag == Expr_EList) {
        __auto_type items = _t3513.data.EList.items;
        /* pass */
        List_ptr* l = (void*)List_ptr_new();
        /* pass */
        long long i2 = 0LL;
        /* pass */
        while ((i2 < items->len)) {
            /* pass */
            List_ptr_append(l, MacroCtx_eval_mexpr(self, ((Expr*)List_ptr_get(items, i2))));
            /* pass */
            i2 = (i2 + 1LL);
        }
        /* pass */
        return box_mv(MacroVal_ctor_MList(l));
    } else if (1) {
        __auto_type _ = _t3513;
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported expression in macro body"));
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void MacroCtx_eval_mblock(MacroCtx* self, Block* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        if ((self->returned || self->has_error)) {
            /* pass */
            return;
        }
        /* pass */
        MacroCtx_eval_mstmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void MacroCtx_eval_mstmt(MacroCtx* self, Stmt* sptr) {
    /* pass */
    if ((((unsigned long long)(sptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t3529 = (*sptr);
    if (_t3529.tag == Stmt_SLet) {
        __auto_type name = _t3529.data.SLet.name;
__auto_type val = _t3529.data.SLet.val;
        /* pass */
        _tr_dict_set(self->env, _tr_strz(name), MacroCtx_eval_mexpr(self, val));
    } else if (_t3529.tag == Stmt_SAssign) {
        __auto_type target = _t3529.data.SAssign.target;
__auto_type val = _t3529.data.SAssign.val;
        /* pass */
        __auto_type _t3530 = (*target);
        if (_t3530.tag == Expr_EIdent) {
            __auto_type nm = _t3530.data.EIdent.name;
            _tr_dict_set(self->env, _tr_strz(nm), MacroCtx_eval_mexpr(self, val));
        } else if (1) {
            __auto_type _ = _t3530;
            MacroCtx_fail(self, _tr_str_lit("macro: assignment target must be a simple variable"));
        }
    } else if (_t3529.tag == Stmt_SReturn) {
        __auto_type val = _t3529.data.SReturn.val;
        /* pass */
        self->result = mv_to_str(MacroCtx_eval_mexpr(self, val));
        /* pass */
        self->returned = true;
    } else if (_t3529.tag == Stmt_SExpr) {
        __auto_type e = _t3529.data.SExpr.expr;
        /* pass */
        MacroVal* _r = MacroCtx_eval_mexpr(self, e);
    } else if (_t3529.tag == Stmt_SFor) {
        __auto_type var = _t3529.data.SFor.var;
__auto_type iter = _t3529.data.SFor.iter;
__auto_type body = _t3529.data.SFor.body;
        /* pass */
        MacroVal* iv = MacroCtx_eval_mexpr(self, iter);
        /* pass */
        __auto_type _t3531 = (*iv);
        if (_t3531.tag == MacroVal_MList) {
            __auto_type items = _t3531.data.MList.items;
            /* pass */
            long long i = 0LL;
            /* pass */
            while ((i < items->len)) {
                /* pass */
                if ((self->returned || self->has_error)) {
                    /* pass */
                    return;
                }
                /* pass */
                _tr_dict_set(self->env, _tr_strz(var), ((MacroVal*)List_ptr_get(items, i)));
                /* pass */
                MacroCtx_eval_mblock(self, body);
                /* pass */
                i = (i + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3531;
            MacroCtx_fail(self, _tr_str_lit("macro: 'for' needs a list (e.g. item.fields)"));
        }
    } else if (_t3529.tag == Stmt_SIf) {
        __auto_type cond = _t3529.data.SIf.cond;
__auto_type then_b = _t3529.data.SIf.then_b;
__auto_type elifs = _t3529.data.SIf.elifs;
__auto_type else_b = _t3529.data.SIf.else_b;
        /* pass */
        if (mv_truthy(MacroCtx_eval_mexpr(self, cond))) {
            /* pass */
            MacroCtx_eval_mblock(self, then_b);
        } else {
            /* pass */
            bool done = false;
            /* pass */
            long long i = 0LL;
            /* pass */
            while (((i < elifs->len) && (!done))) {
                /* pass */
                ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, i));
                /* pass */
                if (mv_truthy(MacroCtx_eval_mexpr(self, ec->cond))) {
                    /* pass */
                    MacroCtx_eval_mblock(self, (*ec->body));
                    /* pass */
                    done = true;
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            if ((!done)) {
                /* pass */
                MacroCtx_eval_mblock(self, else_b);
            }
        }
    } else if (_t3529.tag == Stmt_SPass) {
        /* pass */
    } else if (_t3529.tag == Stmt_SLine) {
        __auto_type _ = _t3529.data.SLine.n;
        /* pass */
    } else if (1) {
        __auto_type _ = _t3529;
        /* pass */
        MacroCtx_fail(self, _tr_str_lit("macro: unsupported statement in macro body"));
    }
}

__attribute__((malloc,returns_nonnull,hot)) FnMacroExpander* FnMacroExpander_init(TrMap* m) {
    /* pass */
    FnMacroExpander* e = ((FnMacroExpander*)_tr_obj_alloc(sizeof(FnMacroExpander)));
    /* pass */
    e->macros = m;
    /* pass */
    e->errors = 0LL;
    /* pass */
    e->depth = 0LL;
    /* pass */
    e->gensym_base = 0LL;
    /* pass */
    return e;
}

__attribute__((hot)) void FnMacroExpander_visit_expr(FnMacroExpander* self, Expr* eptr) {
    /* pass */
    if ((((unsigned long long)(eptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t3532 = (*eptr);
    if (_t3532.tag == Expr_EMacroCall) {
        __auto_type name = _t3532.data.EMacroCall.name;
__auto_type args = _t3532.data.EMacroCall.args;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < args->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(args, i)));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if ((!_tr_dict_contains(self->macros, _tr_strz(name)))) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("error: [MACRO] unknown function-like macro '")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("!'"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            self->errors = (self->errors + 1LL);
            /* pass */
            return;
        }
        /* pass */
        MacroCtx* ctx = run_fn_macro(((FunctionDef*)(uintptr_t)_tr_dict_get(self->macros, _tr_strz(name))), args, self->gensym_base);
        /* pass */
        self->gensym_base = ctx->gensym_ctr;
        /* pass */
        if (ctx->has_error) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("error: [MACRO] ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("!: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ctx->error_msg)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            self->errors = (self->errors + 1LL);
            /* pass */
            _tr_obj_release(ctx, _trdrop_MacroCtx);
            return;
        }
        /* pass */
        Expr* newe = parse_expr_src(ctx->result);
        /* pass */
        if ((((unsigned long long)(newe)) != ((unsigned long long)(0LL)))) {
            /* pass */
            if ((self->depth < 64LL)) {
                /* pass */
                self->depth = (self->depth + 1LL);
                /* pass */
                FnMacroExpander_visit_expr(self, newe);
                /* pass */
                self->depth = (self->depth - 1LL);
            }
            /* pass */
            /* unsafe block */
            /* pass */
            (*eptr = (*newe));
        }
        _tr_obj_release(ctx, _trdrop_MacroCtx);
    } else if (_t3532.tag == Expr_EBinOp) {
        __auto_type l = _t3532.data.EBinOp.left;
__auto_type r = _t3532.data.EBinOp.right;
        /* pass */
        FnMacroExpander_visit_expr(self, l);
        /* pass */
        FnMacroExpander_visit_expr(self, r);
    } else if (_t3532.tag == Expr_EUnaryOp) {
        __auto_type x = _t3532.data.EUnaryOp.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t3532.tag == Expr_ECall) {
        __auto_type callee = _t3532.data.ECall.callee;
__auto_type cargs = _t3532.data.ECall.args;
        /* pass */
        FnMacroExpander_visit_expr(self, callee);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < cargs->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(cargs, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3532.tag == Expr_EMethodCall) {
        __auto_type obj = _t3532.data.EMethodCall.obj;
__auto_type margs = _t3532.data.EMethodCall.args;
        /* pass */
        FnMacroExpander_visit_expr(self, obj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < margs->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(margs, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3532.tag == Expr_EPropAccess) {
        __auto_type obj = _t3532.data.EPropAccess.obj;
        FnMacroExpander_visit_expr(self, obj);
    } else if (_t3532.tag == Expr_EIndex) {
        __auto_type obj = _t3532.data.EIndex.obj;
__auto_type idx = _t3532.data.EIndex._tr_v_index;
        /* pass */
        FnMacroExpander_visit_expr(self, obj);
        /* pass */
        FnMacroExpander_visit_expr(self, idx);
    } else if (_t3532.tag == Expr_ECast) {
        __auto_type x = _t3532.data.ECast.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t3532.tag == Expr_ETryExpr) {
        __auto_type x = _t3532.data.ETryExpr.expr;
        FnMacroExpander_visit_expr(self, x);
    } else if (_t3532.tag == Expr_EIfElse) {
        __auto_type c = _t3532.data.EIfElse.cond;
__auto_type t = _t3532.data.EIfElse.then_expr;
__auto_type e2 = _t3532.data.EIfElse.else_expr;
        /* pass */
        FnMacroExpander_visit_expr(self, c);
        /* pass */
        FnMacroExpander_visit_expr(self, t);
        /* pass */
        FnMacroExpander_visit_expr(self, e2);
    } else if (_t3532.tag == Expr_EList) {
        __auto_type items = _t3532.data.EList.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3532.tag == Expr_ETuple) {
        __auto_type items = _t3532.data.ETuple.items;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3532.tag == Expr_EFString) {
        __auto_type parts = _t3532.data.EFString.parts;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < parts->len)) {
            /* pass */
            FStringPart* p = ((FStringPart*)List_ptr_get(parts, i));
            /* pass */
            if (p->is_expr) {
                /* pass */
                FnMacroExpander_visit_expr(self, p->expr);
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3532.tag == Expr_EDo) {
        __auto_type body = _t3532.data.EDo.body;
        FnMacroExpander_visit_block(self, body);
    } else if (_t3532.tag == Expr_ELoop) {
        __auto_type body = _t3532.data.ELoop.body;
        FnMacroExpander_visit_block(self, body);
    } else if (_t3532.tag == Expr_EWhileExpr) {
        __auto_type c = _t3532.data.EWhileExpr.cond;
__auto_type body = _t3532.data.EWhileExpr.body;
__auto_type eb = _t3532.data.EWhileExpr.else_body;
        /* pass */
        FnMacroExpander_visit_expr(self, c);
        /* pass */
        FnMacroExpander_visit_block(self, body);
        /* pass */
        FnMacroExpander_visit_block(self, eb);
    } else if (_t3532.tag == Expr_EMatch) {
        __auto_type subj = _t3532.data.EMatch.subj;
__auto_type arms = _t3532.data.EMatch.arms;
        /* pass */
        FnMacroExpander_visit_expr(self, subj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            FnMacroExpander_visit_block(self, (*((MatchArm*)List_ptr_get(arms, i))->body));
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t3532;
        /* pass */
    }
}

__attribute__((hot)) void FnMacroExpander_visit_block(FnMacroExpander* self, Block* b) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        FnMacroExpander_visit_stmt(self, ((Stmt*)List_ptr_get(b->stmts, i)));
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void FnMacroExpander_visit_stmt(FnMacroExpander* self, Stmt* sptr) {
    /* pass */
    if ((((unsigned long long)(sptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t3533 = (*sptr);
    if (_t3533.tag == Stmt_SExpr) {
        __auto_type e = _t3533.data.SExpr.expr;
        FnMacroExpander_visit_expr(self, e);
    } else if (_t3533.tag == Stmt_SLet) {
        __auto_type v = _t3533.data.SLet.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SMultiLet) {
        __auto_type v = _t3533.data.SMultiLet.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SAssign) {
        __auto_type t = _t3533.data.SAssign.target;
__auto_type v = _t3533.data.SAssign.val;
        /* pass */
        FnMacroExpander_visit_expr(self, t);
        /* pass */
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SReturn) {
        __auto_type v = _t3533.data.SReturn.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SBreak) {
        __auto_type v = _t3533.data.SBreak.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SRaise) {
        __auto_type v = _t3533.data.SRaise.val;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SSpawn) {
        __auto_type v = _t3533.data.SSpawn.expr;
        FnMacroExpander_visit_expr(self, v);
    } else if (_t3533.tag == Stmt_SAssert) {
        __auto_type c = _t3533.data.SAssert.cond;
__auto_type m = _t3533.data.SAssert.msg;
        /* pass */
        FnMacroExpander_visit_expr(self, c);
        /* pass */
        FnMacroExpander_visit_expr(self, m);
    } else if (_t3533.tag == Stmt_SIf) {
        __auto_type c = _t3533.data.SIf.cond;
__auto_type tb = _t3533.data.SIf.then_b;
__auto_type elifs = _t3533.data.SIf.elifs;
__auto_type eb = _t3533.data.SIf.else_b;
        /* pass */
        FnMacroExpander_visit_expr(self, c);
        /* pass */
        FnMacroExpander_visit_block(self, tb);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < elifs->len)) {
            /* pass */
            ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, i));
            /* pass */
            FnMacroExpander_visit_expr(self, ec->cond);
            /* pass */
            FnMacroExpander_visit_block(self, (*ec->body));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        FnMacroExpander_visit_block(self, eb);
    } else if (_t3533.tag == Stmt_SWhile) {
        __auto_type c = _t3533.data.SWhile.cond;
__auto_type b = _t3533.data.SWhile.body;
        /* pass */
        FnMacroExpander_visit_expr(self, c);
        /* pass */
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_SFor) {
        __auto_type it = _t3533.data.SFor.iter;
__auto_type b = _t3533.data.SFor.body;
        /* pass */
        FnMacroExpander_visit_expr(self, it);
        /* pass */
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_SForUnpack) {
        __auto_type it = _t3533.data.SForUnpack.iter;
__auto_type b = _t3533.data.SForUnpack.body;
        /* pass */
        FnMacroExpander_visit_expr(self, it);
        /* pass */
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_SMatch) {
        __auto_type subj = _t3533.data.SMatch.expr;
__auto_type arms = _t3533.data.SMatch.arms;
        /* pass */
        FnMacroExpander_visit_expr(self, subj);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < arms->len)) {
            /* pass */
            FnMacroExpander_visit_block(self, (*((MatchArm*)List_ptr_get(arms, i))->body));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3533.tag == Stmt_SWith) {
        __auto_type items = _t3533.data.SWith.items;
__auto_type b = _t3533.data.SWith.body;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            FnMacroExpander_visit_expr(self, ((Expr*)List_ptr_get(items, i)));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_STry) {
        __auto_type tb = _t3533.data.STry.try_body;
__auto_type catches = _t3533.data.STry.catches;
__auto_type fb = _t3533.data.STry.finally_b;
        /* pass */
        FnMacroExpander_visit_block(self, tb);
        /* pass */
        FnMacroExpander_visit_block(self, fb);
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < catches->len)) {
            /* pass */
            FnMacroExpander_visit_block(self, (*(*((CatchClause**)List_ptr_get(catches, i)))->body));
            /* pass */
            i = (i + 1LL);
        }
    } else if (_t3533.tag == Stmt_SUnsafe) {
        __auto_type b = _t3533.data.SUnsafe.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_STaskGroup) {
        __auto_type b = _t3533.data.STaskGroup.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_SGpuBlock) {
        __auto_type b = _t3533.data.SGpuBlock.body;
        FnMacroExpander_visit_block(self, b);
    } else if (_t3533.tag == Stmt_SDefer) {
        __auto_type s = _t3533.data.SDefer.stmt;
        FnMacroExpander_visit_stmt(self, s);
    } else if (1) {
        __auto_type _ = _t3533;
        /* pass */
    }
}

__attribute__((hot)) void FnMacroExpander_expand_decl(FnMacroExpander* self, Decl* dptr) {
    /* pass */
    __auto_type _t3534 = (*dptr);
    if (_t3534.tag == Decl_DFunction) {
        __auto_type f = _t3534.data.DFunction.func;
        FnMacroExpander_visit_block(self, f->body);
    } else if (_t3534.tag == Decl_DClass) {
        __auto_type c = _t3534.data.DClass.cls;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < c->methods->len)) {
            /* pass */
            FnMacroExpander_visit_block(self, ((FunctionDef*)List_ptr_get(c->methods, i))->body);
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t3534;
        /* pass */
    }
}

__attribute__((hot)) MacroVal* box_mv(MacroVal v) {
    /* pass */
    /* unsafe block */
    /* pass */
    MacroVal* p = ((MacroVal*)_tr_c_calloc((size_t)(1LL), sizeof(MacroVal)));
    /* pass */
    (*p = v);
    /* pass */
    return p;
}

__attribute__((hot)) MacroVal* mrec(List_TrStr* keys, List_ptr* vals) {
    /* pass */
    return box_mv(MacroVal_ctor_MRec(keys, vals));
}

__attribute__((hot)) MacroVal* mrec_get(MacroVal* recptr, TrStr key) {
    /* pass */
    if ((((unsigned long long)(recptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t3535 = (*recptr);
    if (_t3535.tag == MacroVal_MRec) {
        __auto_type keys = _t3535.data.MRec.keys;
__auto_type vals = _t3535.data.MRec.vals;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < keys->len)) {
            /* pass */
            if ((strcmp(_tr_strz(List_TrStr_get(keys, i)), _tr_strz(key)) == 0)) {
                /* pass */
                return ((MacroVal*)List_ptr_get(vals, i));
            }
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t3535;
        /* pass */
    }
    /* pass */
    return box_mv(MacroVal_make_MNil());
}

__attribute__((hot)) TrStr mv_to_str(MacroVal* vptr) {
    /* pass */
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t3536 = (*vptr);
    if (_t3536.tag == MacroVal_MStr) {
        __auto_type s = _t3536.data.MStr.s;
        return _tr_str_retain(s);
    } else if (_t3536.tag == MacroVal_MInt) {
        __auto_type n = _t3536.data.MInt.n;
        return _tr_str_wrap(_tr_int_to_str((long long)(n)));
    } else if (_t3536.tag == MacroVal_MBool) {
        __auto_type b = _t3536.data.MBool.b;
        /* pass */
        if (b) {
            /* pass */
            return _tr_str_lit("true");
        }
        /* pass */
        return _tr_str_lit("false");
    } else if (_t3536.tag == MacroVal_MRec) {
        return mv_to_str(mrec_get(vptr, _tr_str_lit("src")));
    } else if (1) {
        __auto_type _ = _t3536;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) bool mv_truthy(MacroVal* vptr) {
    /* pass */
    if ((((unsigned long long)(vptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t3537 = (*vptr);
    if (_t3537.tag == MacroVal_MBool) {
        __auto_type b = _t3537.data.MBool.b;
        return b;
    } else if (_t3537.tag == MacroVal_MInt) {
        __auto_type n = _t3537.data.MInt.n;
        return (n != 0LL);
    } else if (_t3537.tag == MacroVal_MStr) {
        __auto_type s = _t3537.data.MStr.s;
        return (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit(""))) != 0);
    } else if (1) {
        __auto_type _ = _t3537;
        return false;
    }
}

__attribute__((hot)) bool mv_eq(MacroVal* a, MacroVal* b) {
    /* pass */
    return (strcmp(_tr_strz(mv_to_str(a)), _tr_strz(mv_to_str(b))) == 0);
}

__attribute__((hot)) TrStr render_type(AstType** typtr) {
    /* pass */
    if ((((unsigned long long)(typtr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    AstType* t = (*typtr);
    /* pass */
    TrStr base = _tr_str_retain(t->name);
    /* pass */
    if ((t->args->len > 0LL)) {
        /* pass */
        TrStr s = _tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("[")));
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < t->args->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t3538 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t3538;
            }
            /* pass */
            TrStr _strtmp_t3539 = ({ TrStr _cr = (render_type(((AstType**)List_ptr_get(t->args, i)))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t3539;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        TrStr _strtmp_t3540 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(base);
        base = _strtmp_t3540;
        _tr_str_release(s);
    }
    /* pass */
    if (t->is_mut_borrow) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("mut ref ")), _tr_strz(base));
    }
    /* pass */
    if (t->is_borrow) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("ref ")), _tr_strz(base));
    }
    /* pass */
    return base;
}

__attribute__((hot)) MacroVal* str_list(List_TrStr* items) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < items->len)) {
        /* pass */
        ({ TrStr _at_t3541 = (List_TrStr_get(items, i)); List_ptr_append(l, box_mv(MacroVal_ctor_MStr(_at_t3541))); _tr_str_release(_at_t3541); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* param_rec(Param* p) {
    /* pass */
    List_TrStr* ks = (void*)List_TrStr_new();
    /* pass */
    List_ptr* vs = (void*)List_ptr_new();
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("name"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(p->name)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("type"));
    /* pass */
    ({ TrStr _at_t3542 = (render_type(p->ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t3542))); _tr_str_release(_at_t3542); });
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_ref"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_ref)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_mut"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_mut_ref)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_variadic"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(p->is_variadic)));
    /* pass */
    return mrec(ks, vs);
}

__attribute__((hot)) MacroVal* params_list(List_ptr* ps) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ps->len)) {
        /* pass */
        List_ptr_append(l, param_rec(((Param*)List_ptr_get(ps, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* fn_rec(FunctionDef* f) {
    /* pass */
    List_TrStr* ks = (void*)List_TrStr_new();
    /* pass */
    List_ptr* vs = (void*)List_ptr_new();
    /* pass */
    TrStr arglist = _tr_str_lit("");
    /* pass */
    long long ai = 0LL;
    /* pass */
    while ((ai < f->params->len)) {
        /* pass */
        if ((ai > 0LL)) {
            /* pass */
            TrStr _strtmp_t3543 = _tr_strx_concat(_tr_strz(arglist), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(arglist);
            arglist = _strtmp_t3543;
        }
        /* pass */
        TrStr _strtmp_t3544 = _tr_strx_concat(_tr_strz(arglist), _tr_strz(((Param*)List_ptr_get(f->params, ai))->name));
        _tr_str_release(arglist);
        arglist = _strtmp_t3544;
        /* pass */
        ai = (ai + 1LL);
    }
    /* pass */
    TrStr thr = _tr_str_lit("");
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t3545 = render_type(f->throws_ty);
        _tr_str_release(thr);
        thr = _strtmp_t3545;
    }
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("kind"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit("fn"))));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("name"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(f->name)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("params"));
    /* pass */
    List_ptr_append(vs, params_list(f->params));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("ret"));
    /* pass */
    ({ TrStr _at_t3546 = (render_type(f->ret_ty)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t3546))); _tr_str_release(_at_t3546); });
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("arglist"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(arglist)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_pub"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_public)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("is_async"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(f->is_async)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("throws"));
    /* pass */
    List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(thr)));
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("generics"));
    /* pass */
    List_ptr_append(vs, str_list(f->generics));
    /* pass */
    _tr_str_release(arglist);
    _tr_str_release(thr);
    return mrec(ks, vs);
}

__attribute__((hot)) MacroVal* method_list(List_ptr* ms) {
    /* pass */
    List_ptr* l = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < ms->len)) {
        /* pass */
        List_ptr_append(l, fn_rec(((FunctionDef*)List_ptr_get(ms, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return box_mv(MacroVal_ctor_MList(l));
}

__attribute__((hot)) MacroVal* build_item(Decl* declptr) {
    /* pass */
    if ((((unsigned long long)(declptr)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return box_mv(MacroVal_make_MNil());
    }
    /* pass */
    __auto_type _t3547 = (*declptr);
    if (_t3547.tag == Decl_DClass) {
        __auto_type c = _t3547.data.DClass.cls;
        /* pass */
        List_TrStr* ks = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs = (void*)List_ptr_new();
        /* pass */
        List_ptr* fl = (void*)List_ptr_new();
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < c->fields->len)) {
            /* pass */
            FieldDef* fd = ((FieldDef*)List_ptr_get(c->fields, i));
            /* pass */
            List_TrStr* fks = (void*)List_TrStr_new();
            /* pass */
            List_ptr* fvs = (void*)List_ptr_new();
            /* pass */
            List_TrStr_append(fks, _tr_str_lit("name"));
            /* pass */
            List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(fd->name)));
            /* pass */
            List_TrStr_append(fks, _tr_str_lit("type"));
            /* pass */
            ({ TrStr _at_t3548 = (render_type(fd->ty)); List_ptr_append(fvs, box_mv(MacroVal_ctor_MStr(_at_t3548))); _tr_str_release(_at_t3548); });
            /* pass */
            List_ptr_append(fl, mrec(fks, fvs));
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_tr_str_lit("class"))));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(c->name)));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("fields"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MList(fl)));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("methods"));
        /* pass */
        List_ptr_append(vs, method_list(c->methods));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("bases"));
        /* pass */
        List_ptr_append(vs, str_list(c->base_classes));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("interfaces"));
        /* pass */
        List_ptr_append(vs, str_list(c->iface_names));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("generics"));
        /* pass */
        List_ptr_append(vs, str_list(c->generics));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("is_value_type"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool((!c->is_class))));
        /* pass */
        List_TrStr_append(ks, _tr_str_lit("is_pub"));
        /* pass */
        List_ptr_append(vs, box_mv(MacroVal_ctor_MBool(c->is_public)));
        /* pass */
        return mrec(ks, vs);
    } else if (_t3547.tag == Decl_DFunction) {
        __auto_type f = _t3547.data.DFunction.func;
        /* pass */
        return fn_rec(f);
    } else if (_t3547.tag == Decl_DEnum) {
        __auto_type e = _t3547.data.DEnum.enm;
        /* pass */
        List_TrStr* ks2 = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs2 = (void*)List_ptr_new();
        /* pass */
        List_ptr* vl = (void*)List_ptr_new();
        /* pass */
        long long i2 = 0LL;
        /* pass */
        while ((i2 < e->variants->len)) {
            /* pass */
            VariantDef* vd = ((VariantDef*)List_ptr_get(e->variants, i2));
            /* pass */
            List_TrStr* vks = (void*)List_TrStr_new();
            /* pass */
            List_ptr* vvs = (void*)List_ptr_new();
            /* pass */
            List_TrStr* tys = (void*)List_TrStr_new();
            /* pass */
            long long j = 0LL;
            /* pass */
            while ((j < vd->fields->len)) {
                /* pass */
                ({ TrStr _at_t3549 = (render_type(((Param*)List_ptr_get(vd->fields, j))->ty)); List_TrStr_append(tys, _at_t3549); _tr_str_release(_at_t3549); });
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            List_TrStr_append(vks, _tr_str_lit("name"));
            /* pass */
            List_ptr_append(vvs, box_mv(MacroVal_ctor_MStr(vd->name)));
            /* pass */
            List_TrStr_append(vks, _tr_str_lit("fields"));
            /* pass */
            List_ptr_append(vvs, str_list(tys));
            /* pass */
            List_ptr_append(vl, mrec(vks, vvs));
            /* pass */
            i2 = (i2 + 1LL);
        }
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(_tr_str_lit("enum"))));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MStr(e->name)));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("variants"));
        /* pass */
        List_ptr_append(vs2, box_mv(MacroVal_ctor_MList(vl)));
        /* pass */
        List_TrStr_append(ks2, _tr_str_lit("generics"));
        /* pass */
        List_ptr_append(vs2, str_list(e->generics));
        /* pass */
        return mrec(ks2, vs2);
    } else if (_t3547.tag == Decl_DInterface) {
        __auto_type ifc = _t3547.data.DInterface.iface;
        /* pass */
        List_TrStr* ks3 = (void*)List_TrStr_new();
        /* pass */
        List_ptr* vs3 = (void*)List_ptr_new();
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("kind"));
        /* pass */
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(_tr_str_lit("interface"))));
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("name"));
        /* pass */
        List_ptr_append(vs3, box_mv(MacroVal_ctor_MStr(ifc->name)));
        /* pass */
        List_TrStr_append(ks3, _tr_str_lit("methods"));
        /* pass */
        List_ptr_append(vs3, method_list(ifc->methods));
        /* pass */
        return mrec(ks3, vs3);
    } else if (1) {
        __auto_type _ = _t3547;
        return box_mv(MacroVal_make_MNil());
    }
}

__attribute__((hot)) void parse_into(TrStr src, List_ptr* gen) {
    /* pass */
    Lexer* lx = Lexer_init(src);
    /* pass */
    List_Token* toks = Lexer_tokenize(lx);
    /* pass */
    Parser* ps = Parser_init(toks, lx->token_lines);
    /* pass */
    Program* gp = Parser_parse_program(ps);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < gp->decls->len)) {
        /* pass */
        List_ptr_append(gen, ((Decl*)List_ptr_get(gp->decls, i)));
        /* pass */
        i = (i + 1LL);
    }
    _tr_obj_release(lx, _trdrop_Lexer);
    _tr_obj_release(ps, _trdrop_Parser);
    _tr_obj_release(gp, _trdrop_Program);
}

__attribute__((hot)) List_ptr* decl_decorators(Decl* declptr) {
    /* pass */
    __auto_type _t3550 = (*declptr);
    if (_t3550.tag == Decl_DClass) {
        __auto_type c = _t3550.data.DClass.cls;
        return c->decorators;
    } else if (_t3550.tag == Decl_DFunction) {
        __auto_type f = _t3550.data.DFunction.func;
        return f->decorators;
    } else if (_t3550.tag == Decl_DEnum) {
        __auto_type e = _t3550.data.DEnum.enm;
        return e->decorators;
    } else if (_t3550.tag == Decl_DInterface) {
        __auto_type ifc = _t3550.data.DInterface.iface;
        return ifc->decorators;
    } else if (1) {
        __auto_type _ = _t3550;
        return (void*)List_ptr_new();
    }
}

__attribute__((hot)) TrStr render_arg(Expr* e) {
    /* pass */
    Formatter* fm = Formatter_init((void*)List_i64_new(), (void*)List_TrStr_new(), (void*)List_bool_new());
    /* pass */
    return Formatter_expr_str(fm, e);
}

__attribute__((hot)) Expr* parse_expr_src(TrStr src) {
    /* pass */
    Lexer* lx = Lexer_init(src);
    /* pass */
    List_Token* toks = Lexer_tokenize(lx);
    /* pass */
    Parser* ps = Parser_init(toks, lx->token_lines);
    /* pass */
    _tr_obj_release(lx, _trdrop_Lexer);
    return Parser_parse_expr(ps);
}

__attribute__((hot)) TrStr kind_of(Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("nil");
    }
    /* pass */
    __auto_type _t3551 = (*e);
    if (_t3551.tag == Expr_ELitInt) {
        __auto_type _ = _t3551.data.ELitInt.val;
        return _tr_str_lit("int");
    } else if (_t3551.tag == Expr_ELitFloat) {
        __auto_type _ = _t3551.data.ELitFloat.val;
        return _tr_str_lit("float");
    } else if (_t3551.tag == Expr_ELitStr) {
        __auto_type _ = _t3551.data.ELitStr.val;
        return _tr_str_lit("str");
    } else if (_t3551.tag == Expr_ERawStr) {
        __auto_type _ = _t3551.data.ERawStr.val;
        return _tr_str_lit("str");
    } else if (_t3551.tag == Expr_ELitBool) {
        __auto_type _ = _t3551.data.ELitBool.val;
        return _tr_str_lit("bool");
    } else if (_t3551.tag == Expr_ELitChar) {
        __auto_type _ = _t3551.data.ELitChar.val;
        return _tr_str_lit("char");
    } else if (_t3551.tag == Expr_EIdent) {
        __auto_type _ = _t3551.data.EIdent.name;
        return _tr_str_lit("ident");
    } else if (_t3551.tag == Expr_ECall) {
        return _tr_str_lit("call");
    } else if (_t3551.tag == Expr_EMethodCall) {
        return _tr_str_lit("call");
    } else if (_t3551.tag == Expr_EBinOp) {
        return _tr_str_lit("binop");
    } else if (_t3551.tag == Expr_EUnaryOp) {
        return _tr_str_lit("unop");
    } else if (_t3551.tag == Expr_EList) {
        __auto_type _ = _t3551.data.EList.items;
        return _tr_str_lit("list");
    } else if (_t3551.tag == Expr_ETuple) {
        __auto_type _ = _t3551.data.ETuple.items;
        return _tr_str_lit("tuple");
    } else if (_t3551.tag == Expr_EIndex) {
        return _tr_str_lit("index");
    } else if (_t3551.tag == Expr_EPropAccess) {
        return _tr_str_lit("field");
    } else if (_t3551.tag == Expr_EFString) {
        __auto_type _ = _t3551.data.EFString.parts;
        return _tr_str_lit("fstring");
    } else if (1) {
        __auto_type _ = _t3551;
        return _tr_str_lit("expr");
    }
}

__attribute__((hot)) MacroVal* arg_rec(Expr* e) {
    /* pass */
    List_TrStr* ks = (void*)List_TrStr_new();
    /* pass */
    List_ptr* vs = (void*)List_ptr_new();
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("src"));
    /* pass */
    ({ TrStr _at_t3552 = (render_arg(e)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t3552))); _tr_str_release(_at_t3552); });
    /* pass */
    List_TrStr_append(ks, _tr_str_lit("kind"));
    /* pass */
    ({ TrStr _at_t3553 = (kind_of(e)); List_ptr_append(vs, box_mv(MacroVal_ctor_MStr(_at_t3553))); _tr_str_release(_at_t3553); });
    /* pass */
    return mrec(ks, vs);
}

__attribute__((hot)) MacroCtx* run_fn_macro(FunctionDef* mdef, List_ptr* args, long long gbase) {
    /* pass */
    MacroCtx* ctx = MacroCtx_init();
    /* pass */
    ctx->gensym_ctr = gbase;
    /* pass */
    List_ptr* argvals = (void*)List_ptr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        List_ptr_append(argvals, arg_rec(((Expr*)List_ptr_get(args, i))));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((mdef->params->len > 0LL)) {
        /* pass */
        _tr_dict_set(ctx->env, _tr_strz(((Param*)List_ptr_get(mdef->params, 0LL))->name), box_mv(MacroVal_ctor_MList(argvals)));
    }
    /* pass */
    MacroCtx_eval_mblock(ctx, mdef->body);
    /* pass */
    return ctx;
}

__attribute__((hot)) long long expand_macros(Program* prog) {
    /* pass */
    TrMap* macros = _tr_dict_new(16LL);
    /* pass */
    long long nmac = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        __auto_type _t3554 = (*((Decl*)List_ptr_get(prog->decls, i)));
        if (_t3554.tag == Decl_DDecoratorDef) {
            __auto_type f = _t3554.data.DDecoratorDef.func;
            /* pass */
            if (f->is_macro) {
                /* pass */
                _tr_dict_set(macros, _tr_strz(f->name), _tr_obj_retain(f));
                /* pass */
                nmac = (nmac + 1LL);
            }
        } else if (1) {
            __auto_type _ = _t3554;
            /* pass */
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((nmac == 0LL)) {
        /* pass */
        Dict_free_objval(macros, _trdrop_FunctionDef);
        return 0LL;
    }
    /* pass */
    long long errors = 0LL;
    /* pass */
    long long gbase = 0LL;
    /* pass */
    List_ptr* out = (void*)List_ptr_new();
    /* pass */
    List_ptr* gen = (void*)List_ptr_new();
    /* pass */
    i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* dptr = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        bool is_macro_def = false;
        /* pass */
        __auto_type _t3555 = (*dptr);
        if (_t3555.tag == Decl_DDecoratorDef) {
            __auto_type f = _t3555.data.DDecoratorDef.func;
            /* pass */
            if (f->is_macro) {
                /* pass */
                is_macro_def = true;
            }
        } else if (1) {
            __auto_type _ = _t3555;
            /* pass */
        }
        /* pass */
        if ((!is_macro_def)) {
            /* pass */
            List_ptr* decs = decl_decorators(dptr);
            /* pass */
            long long di = 0LL;
            /* pass */
            while ((di < decs->len)) {
                /* pass */
                TrStr dname = _tr_str_retain(((Decorator*)List_ptr_get(decs, di))->name);
                /* pass */
                if (_tr_dict_contains(macros, _tr_strz(dname))) {
                    /* pass */
                    MacroCtx* ctx = MacroCtx_init();
                    /* pass */
                    ctx->gensym_ctr = gbase;
                    /* pass */
                    FunctionDef* mdef = ((FunctionDef*)(uintptr_t)_tr_dict_get(macros, _tr_strz(dname)));
                    /* pass */
                    if ((mdef->params->len > 0LL)) {
                        /* pass */
                        _tr_dict_set(ctx->env, _tr_strz(((Param*)List_ptr_get(mdef->params, 0LL))->name), build_item(dptr));
                    }
                    /* pass */
                    MacroCtx_eval_mblock(ctx, mdef->body);
                    /* pass */
                    gbase = ctx->gensym_ctr;
                    /* pass */
                    if (ctx->has_error) {
                        /* pass */
                        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("error: [MACRO] @")), _tr_strz(dname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ctx->error_msg)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
                        /* pass */
                        errors = (errors + 1LL);
                    } else {
                        /* pass */
                        parse_into(ctx->result, gen);
                    }
                    _tr_obj_release(ctx, _trdrop_MacroCtx);
                }
                /* pass */
                di = (di + 1LL);
                _tr_str_release(dname);
            }
            /* pass */
            List_ptr_append(out, dptr);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    long long gi = 0LL;
    /* pass */
    while ((gi < gen->len)) {
        /* pass */
        List_ptr_append(out, ((Decl*)List_ptr_get(gen, gi)));
        /* pass */
        gi = (gi + 1LL);
    }
    /* pass */
    FnMacroExpander* fexp = FnMacroExpander_init(macros);
    /* pass */
    fexp->gensym_base = gbase;
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < out->len)) {
        /* pass */
        FnMacroExpander_expand_decl(fexp, ((Decl*)List_ptr_get(out, fi)));
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    errors = (errors + fexp->errors);
    /* pass */
    prog->decls = out;
    /* pass */
    Dict_free_objval(macros, _trdrop_FunctionDef);
    _tr_obj_release(fexp, _trdrop_FnMacroExpander);
    return errors;
}

