#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Formatter* Formatter_init(List_i64* c_lines, List_TrStr* c_texts, List_bool* c_trailing) {
    /* pass */
    Formatter* f = ((Formatter*)_tr_obj_alloc(sizeof(Formatter)));
    /* pass */
    f->out = StringBuilder_init(4096LL);
    /* pass */
    f->indent = 0LL;
    /* pass */
    f->c_lines = c_lines;
    /* pass */
    f->c_texts = c_texts;
    /* pass */
    f->c_trailing = c_trailing;
    /* pass */
    f->ci = 0LL;
    /* pass */
    f->unsupported = false;
    /* pass */
    f->in_fstring = false;
    /* pass */
    return f;
}

__attribute__((hot)) TrStr Formatter_pad(Formatter* self) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(((self->indent * 4LL) + 1LL));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (self->indent * 4LL))) {
        /* pass */
        StringBuilder_append_char(sb, 32LL);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void Formatter_line(Formatter* self, TrStr text) {
    /* pass */
    ({ TrStr _sbt_t661 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t661); _tr_str_release(_sbt_t661); });
    /* pass */
    StringBuilder_append(self->out, text);
    /* pass */
    StringBuilder_append(self->out, _tr_str_lit("\n"));
}

__attribute__((hot)) void Formatter_flush_standalone_before(Formatter* self, long long line_no) {
    /* pass */
    while ((self->ci < self->c_lines->len)) {
        /* pass */
        if ((List_i64_get(self->c_lines, self->ci) >= line_no)) {
            /* pass */
            break;
        }
        /* pass */
        if (List_bool_get(self->c_trailing, self->ci)) {
            /* pass */
            ({ TrStr _sbt_t662 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t662); _tr_str_release(_sbt_t662); });
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("#"));
            /* pass */
            ({ TrStr _sbt_t663 = (List_TrStr_get(self->c_texts, self->ci)); StringBuilder_append(self->out, _sbt_t663); _tr_str_release(_sbt_t663); });
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("\n"));
        } else {
            /* pass */
            TrStr txt = List_TrStr_get(self->c_texts, self->ci);
            /* pass */
            if ((_tr_strlen(_tr_strz(txt)) == 0LL)) {
                /* pass */
                StringBuilder_append(self->out, _tr_str_lit("#\n"));
            } else {
                /* pass */
                ({ TrStr _sbt_t664 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t664); _tr_str_release(_sbt_t664); });
                /* pass */
                StringBuilder_append(self->out, _tr_str_lit("#"));
                /* pass */
                StringBuilder_append(self->out, txt);
                /* pass */
                StringBuilder_append(self->out, _tr_str_lit("\n"));
            }
        }
        /* pass */
        self->ci = (self->ci + 1LL);
    }
}

__attribute__((hot)) TrStr Formatter_trailing_for(Formatter* self, long long line_no) {
    /* pass */
    if ((self->ci < self->c_lines->len)) {
        /* pass */
        if ((List_bool_get(self->c_trailing, self->ci) && (List_i64_get(self->c_lines, self->ci) == line_no))) {
            /* pass */
            TrStr txt = List_TrStr_get(self->c_texts, self->ci);
            /* pass */
            self->ci = (self->ci + 1LL);
            /* pass */
            return _tr_strx_concat(_tr_strz(_tr_str_lit("  #")), _tr_strz(txt));
        }
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) void Formatter_flush_remaining(Formatter* self) {
    /* pass */
    while ((self->ci < self->c_lines->len)) {
        /* pass */
        TrStr txt = List_TrStr_get(self->c_texts, self->ci);
        /* pass */
        if ((_tr_strlen(_tr_strz(txt)) == 0LL)) {
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("#\n"));
        } else {
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("#"));
            /* pass */
            StringBuilder_append(self->out, txt);
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("\n"));
        }
        /* pass */
        self->ci = (self->ci + 1LL);
        _tr_str_release(txt);
    }
}

__attribute__((hot)) TrStr Formatter_escape_str(Formatter* self, TrStr s) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((_tr_strlen(_tr_strz(s)) + 2LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = 0LL;
        /* pass */
        /* unsafe block */
        /* pass */
        c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\\\"));
        } else if ((c == 34LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\\""));
        } else if ((c == 10LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\n"));
        } else if ((c == 9LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\t"));
        } else if ((c == 13LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\r"));
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_escape_char(Formatter* self, long long v) {
    /* pass */
    if ((v == 0LL)) {
        /* pass */
        return _tr_str_lit("\\0");
    }
    /* pass */
    if ((v == 92LL)) {
        /* pass */
        return _tr_str_lit("\\\\");
    }
    /* pass */
    if ((v == 39LL)) {
        /* pass */
        return _tr_str_lit("\\'");
    }
    /* pass */
    if ((v == 10LL)) {
        /* pass */
        return _tr_str_lit("\\n");
    }
    /* pass */
    if ((v == 9LL)) {
        /* pass */
        return _tr_str_lit("\\t");
    }
    /* pass */
    if ((v == 13LL)) {
        /* pass */
        return _tr_str_lit("\\r");
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(2LL);
    /* pass */
    StringBuilder_append_char(sb, v);
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_escape_str_sq(Formatter* self, TrStr s) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((_tr_strlen(_tr_strz(s)) + 2LL));
    /* pass */
    char* p = ((char*)(_tr_strz(s)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = 0LL;
        /* pass */
        /* unsafe block */
        /* pass */
        c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\\\"));
        } else if ((c == 39LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\'"));
        } else if ((c == 10LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\n"));
        } else if ((c == 9LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\t"));
        } else if ((c == 13LL)) {
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\\r"));
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr Formatter_type_str(Formatter* self, AstType** t) {
    /* pass */
    if ((((unsigned long long)(t)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    AstType* ty = (*t);
    /* pass */
    if (((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("ref"))) == 0) && (ty->args->len == 1LL))) {
        /* pass */
        return ({ TrStr _cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("ref ")), _cr.data); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    if (((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("mut_ref"))) == 0) && (ty->args->len == 1LL))) {
        /* pass */
        return ({ TrStr _cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, 0LL)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("mut ref ")), _cr.data); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    if ((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("Tuple"))) == 0)) {
        /* pass */
        TrStr ts = _tr_str_lit("(");
        /* pass */
        long long ti = 0LL;
        /* pass */
        while ((ti < ty->args->len)) {
            /* pass */
            if ((ti > 0LL)) {
                /* pass */
                TrStr _strtmp_t665 = _tr_strx_concat(_tr_strz(ts), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(ts);
                ts = _strtmp_t665;
            }
            /* pass */
            TrStr _strtmp_t666 = ({ TrStr _cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, ti)))); TrStr _cres = _tr_strx_concat(_tr_strz(ts), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(ts);
            ts = _strtmp_t666;
            /* pass */
            ti = (ti + 1LL);
        }
        /* pass */
        return _tr_strx_concat(_tr_strz(ts), _tr_strz(_tr_str_lit(")")));
    }
    /* pass */
    if ((strcmp(_tr_strz(ty->name), _tr_strz(_tr_str_lit("def"))) == 0)) {
        /* pass */
        TrStr ds = _tr_str_lit("def(");
        /* pass */
        long long di = 0LL;
        /* pass */
        while ((di < (ty->args->len - 1LL))) {
            /* pass */
            if ((di > 0LL)) {
                /* pass */
                TrStr _strtmp_t667 = _tr_strx_concat(_tr_strz(ds), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(ds);
                ds = _strtmp_t667;
            }
            /* pass */
            TrStr _strtmp_t668 = ({ TrStr _cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, di)))); TrStr _cres = _tr_strx_concat(_tr_strz(ds), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(ds);
            ds = _strtmp_t668;
            /* pass */
            di = (di + 1LL);
        }
        /* pass */
        TrStr _strtmp_t669 = _tr_strx_concat(_tr_strz(ds), _tr_strz(_tr_str_lit(")")));
        _tr_str_release(ds);
        ds = _strtmp_t669;
        /* pass */
        if ((ty->args->len > 0LL)) {
            /* pass */
            TrStr rt = Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, (ty->args->len - 1LL))));
            /* pass */
            if (((_tr_strlen(_tr_strz(rt)) > 0LL) && (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("void"))) != 0))) {
                /* pass */
                TrStr _strtmp_t670 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ds), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rt)); _tr_str_release(_cl); _cres; });
                _tr_str_release(ds);
                ds = _strtmp_t670;
            }
        }
        /* pass */
        return ds;
    }
    /* pass */
    TrStr s = _tr_str_retain(ty->name);
    /* pass */
    if ((ty->args->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t671 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(s);
        s = _strtmp_t671;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < ty->args->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t672 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t672;
            }
            /* pass */
            TrStr _strtmp_t673 = ({ TrStr _cr = (Formatter_type_str(self, ((AstType**)List_ptr_get(ty->args, i)))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t673;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        TrStr _strtmp_t674 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(s);
        s = _strtmp_t674;
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(ty->from_param)) > 0LL)) {
        /* pass */
        TrStr _strtmp_t675 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" from ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ty->from_param)); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t675;
    }
    /* pass */
    return s;
}

__attribute__((hot)) bool Formatter_needs_parens(Formatter* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t676 = (*e);
    if (_t676.tag == Expr_EBinOp) {
        return true;
    } else if (_t676.tag == Expr_EUnaryOp) {
        return true;
    } else if (_t676.tag == Expr_EIfElse) {
        return true;
    } else if (_t676.tag == Expr_ETuple) {
        __auto_type _ = _t676.data.ETuple.items;
        return true;
    } else if (_t676.tag == Expr_ERange) {
        return true;
    } else if (_t676.tag == Expr_ECast) {
        return true;
    } else if (1) {
        __auto_type _ = _t676;
        return false;
    }
}

__attribute__((hot)) TrStr Formatter_operand_str(Formatter* self, Expr* e) {
    /* pass */
    if (Formatter_needs_parens(self, e)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, e)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("(")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return Formatter_expr_str(self, e);
}

__attribute__((hot)) TrStr Formatter_args_str(Formatter* self, List_ptr* args) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < args->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t677 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t677;
        }
        /* pass */
        TrStr _strtmp_t678 = ({ TrStr _cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(args, i)))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t678;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Formatter_expr_str(Formatter* self, Expr* e) {
    /* pass */
    if ((((unsigned long long)(e)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type _t679 = (*e);
    if (_t679.tag == Expr_ELitInt) {
        __auto_type v = _t679.data.ELitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t679.tag == Expr_ELitFloat) {
        __auto_type v = _t679.data.ELitFloat.val;
        return _tr_str_wrap(_tr_float_to_str((double)(v)));
    } else if (_t679.tag == Expr_ELitStr) {
        __auto_type v = _t679.data.ELitStr.val;
        /* pass */
        if (self->in_fstring) {
            /* pass */
            return ({ TrStr _cl = (({ TrStr _cr = (Formatter_escape_str_sq(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("'")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; });
        }
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_escape_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ELitBool) {
        __auto_type b = _t679.data.ELitBool.val;
        /* pass */
        if (b) {
            /* pass */
            return _tr_str_lit("True");
        }
        /* pass */
        return _tr_str_lit("False");
    } else if (_t679.tag == Expr_ELitChar) {
        __auto_type v = _t679.data.ELitChar.val;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_escape_char(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("'")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ELitBytes) {
        __auto_type v = _t679.data.ELitBytes.val;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_escape_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("b\"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ERawStr) {
        __auto_type v = _t679.data.ERawStr.val;
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("r\"")), _tr_strz(v))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ELitNone) {
        return _tr_str_lit("None");
    } else if (_t679.tag == Expr_EIdent) {
        __auto_type nm = _t679.data.EIdent.name;
        return _tr_str_retain(nm);
    } else if (_t679.tag == Expr_EBinOp) {
        __auto_type op = _t679.data.EBinOp.op;
__auto_type l = _t679.data.EBinOp.left;
__auto_type r = _t679.data.EBinOp.right;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, l)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(op)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_operand_str(self, r)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_EUnaryOp) {
        __auto_type op = _t679.data.EUnaryOp.op;
__auto_type x = _t679.data.EUnaryOp.expr;
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("not"))) == 0)) {
            /* pass */
            return ({ TrStr _cr = (Formatter_operand_str(self, x)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("not ")), _cr.data); _tr_str_release(_cr); _cres; });
        }
        /* pass */
        return ({ TrStr _cr = (Formatter_operand_str(self, x)); TrStr _cres = _tr_strx_concat(_tr_strz(op), _cr.data); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_ECall) {
        __auto_type callee = _t679.data.ECall.callee;
__auto_type args = _t679.data.ECall.args;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (Formatter_expr_str(self, callee)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_args_str(self, args)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EMethodCall) {
        __auto_type obj = _t679.data.EMethodCall.obj;
__auto_type m = _t679.data.EMethodCall.method;
__auto_type args = _t679.data.EMethodCall.args;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, obj)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_args_str(self, args)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EPropAccess) {
        __auto_type obj = _t679.data.EPropAccess.obj;
__auto_type p = _t679.data.EPropAccess.prop;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, obj)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("."))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(p)); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EIndex) {
        __auto_type obj = _t679.data.EIndex.obj;
__auto_type idx = _t679.data.EIndex._tr_v_index;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, obj)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, idx)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ECast) {
        __auto_type x = _t679.data.ECast.expr;
__auto_type ty = _t679.data.ECast.ty;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, x)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" as "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_type_str(self, ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_EFString) {
        __auto_type parts = _t679.data.EFString.parts;
        return Formatter_fstring_str(self, parts);
    } else if (_t679.tag == Expr_ETryExpr) {
        __auto_type x = _t679.data.ETryExpr.expr;
        return ({ TrStr _cl = (Formatter_operand_str(self, x)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("?"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EClosure) {
        __auto_type params = _t679.data.EClosure.params;
__auto_type ret_ty = _t679.data.EClosure.ret_ty;
__auto_type body = _t679.data.EClosure.body;
__auto_type is_async = _t679.data.EClosure.is_async;
        /* pass */
        return Formatter_closure_str(self, params, ret_ty, body, is_async);
    } else if (_t679.tag == Expr_ESuperMethodCall) {
        __auto_type base = _t679.data.ESuperMethodCall.base_class;
__auto_type m = _t679.data.ESuperMethodCall.method;
__auto_type args = _t679.data.ESuperMethodCall.args;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("super.")), _tr_strz(m))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_args_str(self, args)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ESuperPropAccess) {
        __auto_type base = _t679.data.ESuperPropAccess.base_class;
__auto_type p = _t679.data.ESuperPropAccess.prop;
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("super.")), _tr_strz(p));
    } else if (_t679.tag == Expr_EList) {
        __auto_type items = _t679.data.EList.items;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_args_str(self, items)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ESet) {
        __auto_type items = _t679.data.ESet.items;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_args_str(self, items)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("{")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("}"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EDict) {
        __auto_type keys = _t679.data.EDict.keys;
__auto_type vals = _t679.data.EDict.vals;
        return Formatter_dict_str(self, keys, vals);
    } else if (_t679.tag == Expr_ETuple) {
        __auto_type items = _t679.data.ETuple.items;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_args_str(self, items)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("(")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EListComp) {
        __auto_type elem = _t679.data.EListComp.element;
__auto_type gens = _t679.data.EListComp.generators;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_comp_str(self, elem, gens)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EGeneratorExpr) {
        __auto_type elem = _t679.data.EGeneratorExpr.element;
__auto_type gens = _t679.data.EGeneratorExpr.generators;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_comp_str(self, elem, gens)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("(")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_ESlice) {
        __auto_type a = _t679.data.ESlice.start;
__auto_type b = _t679.data.ESlice.stop;
__auto_type c = _t679.data.ESlice.step;
        return Formatter_slice_str(self, a, b, c);
    } else if (_t679.tag == Expr_EAwait) {
        __auto_type x = _t679.data.EAwait.expr;
        return ({ TrStr _cr = (Formatter_operand_str(self, x)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("await ")), _cr.data); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_EYield) {
        __auto_type x = _t679.data.EYield.expr;
        /* pass */
        if ((((unsigned long long)(x)) == ((unsigned long long)(0LL)))) {
            /* pass */
            return _tr_str_lit("yield");
        }
        /* pass */
        return ({ TrStr _cr = (Formatter_expr_str(self, x)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("yield ")), _cr.data); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_ERange) {
        __auto_type a = _t679.data.ERange.start;
__auto_type b = _t679.data.ERange.end;
__auto_type inclusive = _t679.data.ERange.inclusive;
        /* pass */
        TrStr sep = _tr_str_lit("..");
        /* pass */
        if (inclusive) {
            /* pass */
            TrStr _strtmp_t680 = _tr_str_lit("..=");
            _tr_str_release(sep);
            sep = _strtmp_t680;
        }
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, a)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sep)); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_operand_str(self, b)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_ESizeOf) {
        __auto_type ty = _t679.data.ESizeOf.ty;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_type_str(self, ty)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("sizeof(")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t679.tag == Expr_EIfElse) {
        __auto_type c = _t679.data.EIfElse.cond;
__auto_type t = _t679.data.EIfElse.then_expr;
__auto_type f = _t679.data.EIfElse.else_expr;
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (Formatter_operand_str(self, t)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" if "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_operand_str(self, c)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" else "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_operand_str(self, f)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    } else if (_t679.tag == Expr_ETypeArg) {
        __auto_type ty = _t679.data.ETypeArg.ty;
        return Formatter_type_str(self, ty);
    } else if (1) {
        __auto_type _ = _t679;
        /* pass */
        self->unsupported = true;
        /* pass */
        return _tr_str_lit("");
    }
}

__attribute__((hot)) TrStr Formatter_fstring_str(Formatter* self, List_ptr* parts) {
    /* pass */
    TrStr s = _tr_str_lit("f\"");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        FStringPart* part = ((FStringPart*)List_ptr_get(parts, i));
        /* pass */
        if (part->is_expr) {
            /* pass */
            bool saved = self->in_fstring;
            /* pass */
            self->in_fstring = true;
            /* pass */
            TrStr es = Formatter_expr_str(self, part->expr);
            /* pass */
            self->in_fstring = saved;
            /* pass */
            TrStr _strtmp_t681 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("{")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(es)); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t681;
            /* pass */
            if ((_tr_strlen(_tr_strz(part->fmt_spec)) > 0LL)) {
                /* pass */
                TrStr _strtmp_t682 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(part->fmt_spec)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t682;
            }
            /* pass */
            TrStr _strtmp_t683 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}")));
            _tr_str_release(s);
            s = _strtmp_t683;
            _tr_str_release(es);
        } else {
            /* pass */
            TrStr _strtmp_t684 = ({ TrStr _cr = (Formatter_escape_str(self, part->text)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t684;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("\"")));
}

__attribute__((hot)) TrStr Formatter_dict_str(Formatter* self, List_ptr* keys, List_ptr* vals) {
    /* pass */
    if ((keys->len == 0LL)) {
        /* pass */
        return _tr_str_lit("{}");
    }
    /* pass */
    TrStr s = _tr_str_lit("{");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < keys->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t685 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t685;
        }
        /* pass */
        TrStr _strtmp_t686 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(keys, i)))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(vals, i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t686;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("}")));
}

__attribute__((hot)) TrStr Formatter_comp_str(Formatter* self, Expr* elem, List_ptr* gens) {
    /* pass */
    TrStr s = Formatter_expr_str(self, elem);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < gens->len)) {
        /* pass */
        Comprehension* g = (*((Comprehension**)List_ptr_get(gens, i)));
        /* pass */
        TrStr _strtmp_t687 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" for ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(g->target)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" in "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, g->iter)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t687;
        /* pass */
        long long j = 0LL;
        /* pass */
        while ((j < g->ifs->len)) {
            /* pass */
            TrStr _strtmp_t688 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" if ")))); TrStr _cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(g->ifs, j)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t688;
            /* pass */
            j = (j + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Formatter_slice_str(Formatter* self, Expr* a, Expr* b, Expr* c) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    if ((((unsigned long long)(a)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t689 = ({ TrStr _cr = (Formatter_expr_str(self, a)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t689;
    }
    /* pass */
    TrStr _strtmp_t690 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":")));
    _tr_str_release(s);
    s = _strtmp_t690;
    /* pass */
    if ((((unsigned long long)(b)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t691 = ({ TrStr _cr = (Formatter_expr_str(self, b)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t691;
    }
    /* pass */
    if ((((unsigned long long)(c)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t692 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":")))); TrStr _cr = (Formatter_expr_str(self, c)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t692;
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Formatter_closure_str(Formatter* self, List_ptr* params, AstType** ret_ty, Block* body, bool is_async) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    if (is_async) {
        /* pass */
        TrStr _strtmp_t693 = _tr_str_lit("async ");
        _tr_str_release(s);
        s = _strtmp_t693;
    }
    /* pass */
    TrStr _strtmp_t694 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("def (")))); TrStr _cr = (Formatter_params_str(self, params)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    _tr_str_release(s);
    s = _strtmp_t694;
    /* pass */
    if ((((unsigned long long)(ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr rt = Formatter_type_str(self, ret_ty);
        /* pass */
        if (((_tr_strlen(_tr_strz(rt)) > 0LL) && (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("void"))) != 0))) {
            /* pass */
            TrStr _strtmp_t695 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rt)); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t695;
        }
    }
    /* pass */
    TrStr _strtmp_t696 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":")));
    _tr_str_release(s);
    s = _strtmp_t696;
    /* pass */
    Stmt* real = (Stmt*)(0LL);
    /* pass */
    long long _tr_v_count = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < body->stmts->len)) {
        /* pass */
        __auto_type _t697 = (*((Stmt*)List_ptr_get(body->stmts, i)));
        if (_t697.tag == Stmt_SLine) {
            __auto_type _ = _t697.data.SLine.n;
            /* pass */
        } else if (1) {
            __auto_type _ = _t697;
            /* pass */
            _tr_v_count = (_tr_v_count + 1LL);
            /* pass */
            real = ((Stmt*)List_ptr_get(body->stmts, i));
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((_tr_v_count == 1LL)) {
        /* pass */
        __auto_type _t698 = (*real);
        if (_t698.tag == Stmt_SReturn) {
            __auto_type v = _t698.data.SReturn.val;
            /* pass */
            if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
                /* pass */
                return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" return")));
            }
            /* pass */
            return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" return ")))); TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        } else if (_t698.tag == Stmt_SExpr) {
            __auto_type e = _t698.data.SExpr.expr;
            return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (Formatter_expr_str(self, e)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        } else if (1) {
            __auto_type _ = _t698;
            /* pass */
            self->unsupported = true;
            /* pass */
            return s;
        }
    }
    /* pass */
    self->unsupported = true;
    /* pass */
    return s;
}

__attribute__((hot)) TrStr Formatter_params_str(Formatter* self, List_ptr* params) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < params->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t699 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t699;
        }
        /* pass */
        Param* p = ((Param*)List_ptr_get(params, i));
        /* pass */
        TrStr _strtmp_t700 = _tr_strx_concat(_tr_strz(s), _tr_strz(p->name));
        _tr_str_release(s);
        s = _strtmp_t700;
        /* pass */
        TrStr tn = Formatter_type_str(self, p->ty);
        /* pass */
        if (((_tr_strlen(_tr_strz(tn)) > 0LL) && (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("void"))) != 0))) {
            /* pass */
            TrStr _strtmp_t701 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(": ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tn)); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t701;
        }
        /* pass */
        if (p->is_variadic) {
            /* pass */
            TrStr _strtmp_t702 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("...")));
            _tr_str_release(s);
            s = _strtmp_t702;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(tn);
    }
    /* pass */
    return s;
}

__attribute__((hot)) void Formatter_emit_decorators(Formatter* self, List_ptr* decs) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < decs->len)) {
        /* pass */
        Decorator* d = ((Decorator*)List_ptr_get(decs, i));
        /* pass */
        if ((d->args->len > 0LL)) {
            /* pass */
            ({ TrStr _at_t703 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@")), _tr_strz(d->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_args_str(self, d->args)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t703); _tr_str_release(_at_t703); });
        } else {
            /* pass */
            ({ TrStr _at_t704 = (_tr_strx_concat(_tr_strz(_tr_str_lit("@")), _tr_strz(d->name))); Formatter_line(self, _at_t704); _tr_str_release(_at_t704); });
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Formatter_emit_block(Formatter* self, Block* b) {
    /* pass */
    long long cur_line = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < b->stmts->len)) {
        /* pass */
        Stmt* sp = ((Stmt*)List_ptr_get(b->stmts, i));
        /* pass */
        __auto_type _t705 = (*sp);
        if (_t705.tag == Stmt_SLine) {
            __auto_type n = _t705.data.SLine.n;
            /* pass */
            Formatter_flush_standalone_before(self, n);
            /* pass */
            cur_line = n;
        } else if (1) {
            __auto_type _ = _t705;
            /* pass */
            Formatter_emit_stmt(self, sp, cur_line);
        }
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void Formatter_emit_simple(Formatter* self, TrStr text, long long src_line) {
    /* pass */
    ({ TrStr _sbt_t706 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t706); _tr_str_release(_sbt_t706); });
    /* pass */
    StringBuilder_append(self->out, text);
    /* pass */
    ({ TrStr _sbt_t707 = (Formatter_trailing_for(self, src_line)); StringBuilder_append(self->out, _sbt_t707); _tr_str_release(_sbt_t707); });
    /* pass */
    StringBuilder_append(self->out, _tr_str_lit("\n"));
}

__attribute__((hot)) void Formatter_emit_stmt(Formatter* self, Stmt* sp, long long src_line) {
    /* pass */
    __auto_type _t708 = (*sp);
    if (_t708.tag == Stmt_SExpr) {
        __auto_type e = _t708.data.SExpr.expr;
        ({ TrStr _at_t709 = (Formatter_expr_str(self, e)); Formatter_emit_simple(self, _at_t709, src_line); _tr_str_release(_at_t709); });
    } else if (_t708.tag == Stmt_SLet) {
        __auto_type name = _t708.data.SLet.name;
__auto_type own = _t708.data.SLet.ownership;
__auto_type is_mut = _t708.data.SLet.is_mut;
__auto_type is_const = _t708.data.SLet.is_const;
__auto_type is_shared = _t708.data.SLet.is_shared;
__auto_type ty = _t708.data.SLet.ty;
__auto_type val = _t708.data.SLet.val;
        /* pass */
        ({ TrStr _at_t710 = (Formatter_let_str(self, name, own, is_mut, is_const, is_shared, ty, val)); Formatter_emit_simple(self, _at_t710, src_line); _tr_str_release(_at_t710); });
    } else if (_t708.tag == Stmt_SMultiLet) {
        __auto_type names = _t708.data.SMultiLet.names;
__auto_type is_mut = _t708.data.SMultiLet.is_mut;
__auto_type val = _t708.data.SMultiLet.val;
        /* pass */
        TrStr pfx = _tr_str_lit("");
        /* pass */
        if (is_mut) {
            /* pass */
            TrStr _strtmp_t711 = _tr_str_lit("mut ");
            _tr_str_release(pfx);
            pfx = _strtmp_t711;
        }
        /* pass */
        TrStr ns = _tr_str_lit("");
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < names->len)) {
            /* pass */
            if ((k > 0LL)) {
                /* pass */
                TrStr _strtmp_t712 = _tr_strx_concat(_tr_strz(ns), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(ns);
                ns = _strtmp_t712;
            }
            /* pass */
            TrStr _strtmp_t713 = ({ TrStr _cr = (List_TrStr_get(names, k)); TrStr _cres = _tr_strx_concat(_tr_strz(ns), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(ns);
            ns = _strtmp_t713;
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        ({ TrStr _at_t714 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pfx), _tr_strz(ns))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, val)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t714, src_line); _tr_str_release(_at_t714); });
        _tr_str_release(pfx);
        _tr_str_release(ns);
    } else if (_t708.tag == Stmt_SAssign) {
        __auto_type t = _t708.data.SAssign.target;
__auto_type v = _t708.data.SAssign.val;
        /* pass */
        ({ TrStr _at_t715 = (({ TrStr _cl = (({ TrStr _cl = (Formatter_expr_str(self, t)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t715, src_line); _tr_str_release(_at_t715); });
    } else if (_t708.tag == Stmt_SReturn) {
        __auto_type v = _t708.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
            /* pass */
            Formatter_emit_simple(self, _tr_str_lit("return"), src_line);
        } else {
            /* pass */
            ({ TrStr _at_t716 = (({ TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t716, src_line); _tr_str_release(_at_t716); });
        }
    } else if (_t708.tag == Stmt_SBreak) {
        __auto_type bv = _t708.data.SBreak.val;
        /* pass */
        if ((((unsigned long long)(bv)) == ((unsigned long long)(0LL)))) {
            /* pass */
            Formatter_emit_simple(self, _tr_str_lit("break"), src_line);
        } else {
            /* pass */
            ({ TrStr _at_t717 = (({ TrStr _cr = (Formatter_expr_str(self, bv)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("break ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t717, src_line); _tr_str_release(_at_t717); });
        }
    } else if (_t708.tag == Stmt_SContinue) {
        Formatter_emit_simple(self, _tr_str_lit("continue"), src_line);
    } else if (_t708.tag == Stmt_SPass) {
        Formatter_emit_simple(self, _tr_str_lit("pass"), src_line);
    } else if (_t708.tag == Stmt_SRaise) {
        __auto_type v = _t708.data.SRaise.val;
        ({ TrStr _at_t718 = (({ TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("raise ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t718, src_line); _tr_str_release(_at_t718); });
    } else if (_t708.tag == Stmt_SAssert) {
        __auto_type c = _t708.data.SAssert.cond;
__auto_type m = _t708.data.SAssert.msg;
        /* pass */
        if ((((unsigned long long)(m)) == ((unsigned long long)(0LL)))) {
            /* pass */
            ({ TrStr _at_t719 = (({ TrStr _cr = (Formatter_expr_str(self, c)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("assert ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t719, src_line); _tr_str_release(_at_t719); });
        } else {
            /* pass */
            ({ TrStr _at_t720 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, c)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("assert ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, m)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t720, src_line); _tr_str_release(_at_t720); });
        }
    } else if (_t708.tag == Stmt_SUnsafe) {
        __auto_type body = _t708.data.SUnsafe.body;
        /* pass */
        Formatter_emit_simple(self, _tr_str_lit("unsafe:"), src_line);
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_SIf) {
        __auto_type cond = _t708.data.SIf.cond;
__auto_type then_b = _t708.data.SIf.then_b;
__auto_type elifs = _t708.data.SIf.elifs;
__auto_type else_b = _t708.data.SIf.else_b;
        /* pass */
        ({ TrStr _at_t721 = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, cond)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("if ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_emit_simple(self, _at_t721, src_line); _tr_str_release(_at_t721); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, then_b);
        /* pass */
        self->indent = (self->indent - 1LL);
        /* pass */
        long long ei = 0LL;
        /* pass */
        while ((ei < elifs->len)) {
            /* pass */
            ElifClause* ec = ((ElifClause*)List_ptr_get(elifs, ei));
            /* pass */
            ({ TrStr _at_t722 = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, ec->cond)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("elif ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t722); _tr_str_release(_at_t722); });
            /* pass */
            self->indent = (self->indent + 1LL);
            /* pass */
            Formatter_emit_block(self, (*ec->body));
            /* pass */
            self->indent = (self->indent - 1LL);
            /* pass */
            ei = (ei + 1LL);
        }
        /* pass */
        if ((else_b->stmts->len > 0LL)) {
            /* pass */
            Formatter_line(self, _tr_str_lit("else:"));
            /* pass */
            self->indent = (self->indent + 1LL);
            /* pass */
            Formatter_emit_block(self, else_b);
            /* pass */
            self->indent = (self->indent - 1LL);
        }
    } else if (_t708.tag == Stmt_SWhile) {
        __auto_type cond = _t708.data.SWhile.cond;
__auto_type body = _t708.data.SWhile.body;
__auto_type decs = _t708.data.SWhile.decorators;
        /* pass */
        ({ TrStr _at_t723 = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, cond)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("while ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_emit_simple(self, _at_t723, src_line); _tr_str_release(_at_t723); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_SFor) {
        __auto_type var = _t708.data.SFor.var;
__auto_type it = _t708.data.SFor.iter;
__auto_type body = _t708.data.SFor.body;
__auto_type decs = _t708.data.SFor.decorators;
        /* pass */
        ({ TrStr _at_t724 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("for ")), _tr_strz(var))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" in "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, it)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_emit_simple(self, _at_t724, src_line); _tr_str_release(_at_t724); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_SForUnpack) {
        __auto_type vars = _t708.data.SForUnpack.vars;
__auto_type it = _t708.data.SForUnpack.iter;
__auto_type body = _t708.data.SForUnpack.body;
        /* pass */
        TrStr vs = _tr_str_lit("");
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < vars->len)) {
            /* pass */
            if ((k > 0LL)) {
                /* pass */
                TrStr _strtmp_t725 = _tr_strx_concat(_tr_strz(vs), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(vs);
                vs = _strtmp_t725;
            }
            /* pass */
            TrStr _strtmp_t726 = ({ TrStr _cr = (List_TrStr_get(vars, k)); TrStr _cres = _tr_strx_concat(_tr_strz(vs), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(vs);
            vs = _strtmp_t726;
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        ({ TrStr _at_t727 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("for ")), _tr_strz(vs))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" in "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, it)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_emit_simple(self, _at_t727, src_line); _tr_str_release(_at_t727); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
        _tr_str_release(vs);
    } else if (_t708.tag == Stmt_SMatch) {
        __auto_type e = _t708.data.SMatch.expr;
__auto_type arms = _t708.data.SMatch.arms;
        /* pass */
        ({ TrStr _at_t728 = (({ TrStr _cl = (({ TrStr _cr = (Formatter_expr_str(self, e)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("match ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_emit_simple(self, _at_t728, src_line); _tr_str_release(_at_t728); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        long long ai = 0LL;
        /* pass */
        while ((ai < arms->len)) {
            /* pass */
            MatchArm* arm = ((MatchArm*)List_ptr_get(arms, ai));
            /* pass */
            TrStr head = ({ TrStr _cr = (Formatter_pattern_str(self, arm->pat)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("case ")), _cr.data); _tr_str_release(_cr); _cres; });
            /* pass */
            if ((((unsigned long long)(arm->guard)) != ((unsigned long long)(0LL)))) {
                /* pass */
                TrStr _strtmp_t729 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(" if ")))); TrStr _cr = (Formatter_expr_str(self, arm->guard)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(head);
                head = _strtmp_t729;
            }
            /* pass */
            ({ TrStr _at_t730 = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); Formatter_line(self, _at_t730); _tr_str_release(_at_t730); });
            /* pass */
            self->indent = (self->indent + 1LL);
            /* pass */
            Formatter_emit_block(self, (*arm->body));
            /* pass */
            self->indent = (self->indent - 1LL);
            /* pass */
            ai = (ai + 1LL);
            _tr_str_release(head);
        }
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_STry) {
        __auto_type try_b = _t708.data.STry.try_body;
__auto_type catches = _t708.data.STry.catches;
__auto_type finally_b = _t708.data.STry.finally_b;
        /* pass */
        Formatter_emit_simple(self, _tr_str_lit("try:"), src_line);
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, try_b);
        /* pass */
        self->indent = (self->indent - 1LL);
        /* pass */
        long long ci2 = 0LL;
        /* pass */
        while ((ci2 < catches->len)) {
            /* pass */
            CatchClause* cc = (*((CatchClause**)List_ptr_get(catches, ci2)));
            /* pass */
            TrStr head = _tr_str_lit("except");
            /* pass */
            if ((((unsigned long long)(cc->err_type)) != ((unsigned long long)(0LL)))) {
                /* pass */
                TrStr et = Formatter_type_str(self, cc->err_type);
                /* pass */
                if (((_tr_strlen(_tr_strz(et)) > 0LL) && (strcmp(_tr_strz(et), _tr_strz(_tr_str_lit("void"))) != 0))) {
                    /* pass */
                    TrStr _strtmp_t731 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(et)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(head);
                    head = _strtmp_t731;
                }
            }
            /* pass */
            if ((_tr_strlen(_tr_strz(cc->err_name)) > 0LL)) {
                /* pass */
                TrStr _strtmp_t732 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(" as ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cc->err_name)); _tr_str_release(_cl); _cres; });
                _tr_str_release(head);
                head = _strtmp_t732;
            }
            /* pass */
            ({ TrStr _at_t733 = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); Formatter_line(self, _at_t733); _tr_str_release(_at_t733); });
            /* pass */
            self->indent = (self->indent + 1LL);
            /* pass */
            Formatter_emit_block(self, (*cc->body));
            /* pass */
            self->indent = (self->indent - 1LL);
            /* pass */
            ci2 = (ci2 + 1LL);
            _tr_str_release(head);
        }
        /* pass */
        if ((finally_b->stmts->len > 0LL)) {
            /* pass */
            Formatter_line(self, _tr_str_lit("finally:"));
            /* pass */
            self->indent = (self->indent + 1LL);
            /* pass */
            Formatter_emit_block(self, finally_b);
            /* pass */
            self->indent = (self->indent - 1LL);
        }
    } else if (_t708.tag == Stmt_SWith) {
        __auto_type items = _t708.data.SWith.items;
__auto_type aliases = _t708.data.SWith.aliases;
__auto_type body = _t708.data.SWith.body;
        /* pass */
        TrStr hs = _tr_str_lit("with ");
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < items->len)) {
            /* pass */
            if ((k > 0LL)) {
                /* pass */
                TrStr _strtmp_t734 = _tr_strx_concat(_tr_strz(hs), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(hs);
                hs = _strtmp_t734;
            }
            /* pass */
            TrStr _strtmp_t735 = ({ TrStr _cr = (Formatter_expr_str(self, ((Expr*)List_ptr_get(items, k)))); TrStr _cres = _tr_strx_concat(_tr_strz(hs), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(hs);
            hs = _strtmp_t735;
            /* pass */
            if (((k < aliases->len) && (_tr_strlen(_tr_strz(List_TrStr_get(aliases, k))) > 0LL))) {
                /* pass */
                TrStr _strtmp_t736 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(hs), _tr_strz(_tr_str_lit(" as ")))); TrStr _cr = (List_TrStr_get(aliases, k)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(hs);
                hs = _strtmp_t736;
            }
            /* pass */
            k = (k + 1LL);
        }
        /* pass */
        ({ TrStr _at_t737 = (_tr_strx_concat(_tr_strz(hs), _tr_strz(_tr_str_lit(":")))); Formatter_emit_simple(self, _at_t737, src_line); _tr_str_release(_at_t737); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
        _tr_str_release(hs);
    } else if (_t708.tag == Stmt_SSpawn) {
        __auto_type e = _t708.data.SSpawn.expr;
        ({ TrStr _at_t738 = (({ TrStr _cr = (Formatter_expr_str(self, e)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("spawn ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_emit_simple(self, _at_t738, src_line); _tr_str_release(_at_t738); });
    } else if (_t708.tag == Stmt_STaskGroup) {
        __auto_type body = _t708.data.STaskGroup.body;
        /* pass */
        Formatter_emit_simple(self, _tr_str_lit("task_group:"), src_line);
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_SDefer) {
        __auto_type inner = _t708.data.SDefer.stmt;
        /* pass */
        ({ TrStr _sbt_t739 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t739); _tr_str_release(_sbt_t739); });
        /* pass */
        StringBuilder_append(self->out, _tr_str_lit("defer "));
        /* pass */
        Formatter_emit_inline_stmt(self, inner);
        /* pass */
        StringBuilder_append(self->out, _tr_str_lit("\n"));
    } else if (_t708.tag == Stmt_SGpuBlock) {
        __auto_type body = _t708.data.SGpuBlock.body;
        /* pass */
        Formatter_emit_simple(self, _tr_str_lit("gpu:"), src_line);
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        Formatter_emit_block(self, body);
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t708.tag == Stmt_SLocalDecl) {
        __auto_type d = _t708.data.SLocalDecl.decl;
        Formatter_emit_decl(self, d);
    } else if (1) {
        __auto_type _ = _t708;
        /* pass */
        self->unsupported = true;
    }
}

__attribute__((hot)) void Formatter_emit_inline_stmt(Formatter* self, Stmt* sp) {
    /* pass */
    __auto_type _t740 = (*sp);
    if (_t740.tag == Stmt_SExpr) {
        __auto_type e = _t740.data.SExpr.expr;
        ({ TrStr _sbt_t741 = (Formatter_expr_str(self, e)); StringBuilder_append(self->out, _sbt_t741); _tr_str_release(_sbt_t741); });
    } else if (_t740.tag == Stmt_SReturn) {
        __auto_type v = _t740.data.SReturn.val;
        /* pass */
        if ((((unsigned long long)(v)) == ((unsigned long long)(0LL)))) {
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("return"));
        } else {
            /* pass */
            ({ TrStr _sbt_t742 = (({ TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _cr.data); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->out, _sbt_t742); _tr_str_release(_sbt_t742); });
        }
    } else if (_t740.tag == Stmt_SAssign) {
        __auto_type t = _t740.data.SAssign.target;
__auto_type v = _t740.data.SAssign.val;
        ({ TrStr _sbt_t743 = (({ TrStr _cl = (({ TrStr _cl = (Formatter_expr_str(self, t)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_expr_str(self, v)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->out, _sbt_t743); _tr_str_release(_sbt_t743); });
    } else if (1) {
        __auto_type _ = _t740;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_let_str(Formatter* self, TrStr name, Ownership own, bool is_mut, bool is_const, bool is_shared, AstType** ty, Expr* val) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    if (is_const) {
        /* pass */
        TrStr _strtmp_t744 = _tr_str_lit("const ");
        _tr_str_release(s);
        s = _strtmp_t744;
    } else if (is_shared) {
        /* pass */
        TrStr _strtmp_t745 = _tr_str_lit("shared ");
        _tr_str_release(s);
        s = _strtmp_t745;
    } else if (is_mut) {
        /* pass */
        TrStr _strtmp_t746 = _tr_str_lit("mut ");
        _tr_str_release(s);
        s = _strtmp_t746;
    }
    /* pass */
    TrStr _strtmp_t747 = _tr_strx_concat(_tr_strz(s), _tr_strz(name));
    _tr_str_release(s);
    s = _strtmp_t747;
    /* pass */
    TrStr tn = Formatter_type_str(self, ty);
    /* pass */
    if (((_tr_strlen(_tr_strz(tn)) > 0LL) && (strcmp(_tr_strz(tn), _tr_strz(_tr_str_lit("void"))) != 0))) {
        /* pass */
        TrStr _strtmp_t748 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(": ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tn)); _tr_str_release(_cl); _cres; });
        _tr_str_release(s);
        s = _strtmp_t748;
    }
    /* pass */
    if ((((unsigned long long)(val)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr _strtmp_t749 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" = ")))); TrStr _cr = (Formatter_expr_str(self, val)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t749;
    }
    /* pass */
    _tr_str_release(tn);
    return s;
}

__attribute__((hot)) TrStr Formatter_pattern_str(Formatter* self, Pattern p) {
    /* pass */
    __auto_type _t750 = p;
    if (_t750.tag == Pattern_PWild) {
        return _tr_str_lit("_");
    } else if (_t750.tag == Pattern_PBind) {
        __auto_type n = _t750.data.PBind.name;
        return _tr_str_retain(n);
    } else if (_t750.tag == Pattern_PLitInt) {
        __auto_type v = _t750.data.PLitInt.val;
        return _tr_str_wrap(_tr_int_to_str((long long)(v)));
    } else if (_t750.tag == Pattern_PLitStr) {
        __auto_type v = _t750.data.PLitStr.val;
        return ({ TrStr _cl = (({ TrStr _cr = (Formatter_escape_str(self, v)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("\"")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
    } else if (_t750.tag == Pattern_PLitBool) {
        __auto_type b = _t750.data.PLitBool.val;
        /* pass */
        if (b) {
            /* pass */
            return _tr_str_lit("True");
        }
        /* pass */
        return _tr_str_lit("False");
    } else if (_t750.tag == Pattern_PVariant) {
        __auto_type tn = _t750.data.PVariant.type_name;
__auto_type vn = _t750.data.PVariant.variant;
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(tn), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vn)); _tr_str_release(_cl); _cres; });
    } else if (_t750.tag == Pattern_PVariantBind) {
        __auto_type tn = _t750.data.PVariantBind.type_name;
__auto_type vn = _t750.data.PVariantBind.variant;
__auto_type f = _t750.data.PVariantBind.field;
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(tn), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t750.tag == Pattern_PVariantBindMany) {
        __auto_type tn = _t750.data.PVariantBindMany.type_name;
__auto_type vn = _t750.data.PVariantBindMany.variant;
__auto_type fs = _t750.data.PVariantBindMany.fields;
        /* pass */
        TrStr s = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(tn), _tr_strz(_tr_str_lit(".")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; });
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < fs->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t751 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t751;
            }
            /* pass */
            TrStr _strtmp_t752 = ({ TrStr _cr = (List_TrStr_get(fs, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t752;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(")")));
    } else if (_t750.tag == Pattern_PTuple) {
        __auto_type a = _t750.data.PTuple.first;
__auto_type b = _t750.data.PTuple.second;
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("(")), _tr_strz(a))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(b)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    } else if (_t750.tag == Pattern_POr) {
        __auto_type pats = _t750.data.POr.patterns;
        /* pass */
        TrStr s = _tr_str_lit("");
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < pats->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t753 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" | ")));
                _tr_str_release(s);
                s = _strtmp_t753;
            }
            /* pass */
            TrStr _strtmp_t754 = ({ TrStr _cr = (Formatter_pattern_str(self, List_Pattern_get(pats, i))); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t754;
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        return s;
    } else if (1) {
        __auto_type _ = _t750;
        return _tr_str_lit("_");
    }
}

__attribute__((hot)) TrStr Formatter_fn_header(Formatter* self, FunctionDef* f) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    if (f->is_public) {
        /* pass */
        TrStr _strtmp_t755 = _tr_str_lit("pub ");
        _tr_str_release(s);
        s = _strtmp_t755;
    }
    /* pass */
    if (f->is_async) {
        /* pass */
        TrStr _strtmp_t756 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("async ")));
        _tr_str_release(s);
        s = _strtmp_t756;
    }
    /* pass */
    TrStr _strtmp_t757 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("def ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f->name)); _tr_str_release(_cl); _cres; });
    _tr_str_release(s);
    s = _strtmp_t757;
    /* pass */
    if ((f->generics->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t758 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(s);
        s = _strtmp_t758;
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < f->generics->len)) {
            /* pass */
            if ((gi > 0LL)) {
                /* pass */
                TrStr _strtmp_t759 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t759;
            }
            /* pass */
            TrStr _strtmp_t760 = ({ TrStr _cr = (List_TrStr_get(f->generics, gi)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t760;
            /* pass */
            gi = (gi + 1LL);
        }
        /* pass */
        TrStr _strtmp_t761 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(s);
        s = _strtmp_t761;
    }
    /* pass */
    TrStr _strtmp_t762 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("(")))); TrStr _cr = (Formatter_params_str(self, f->params)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
    _tr_str_release(s);
    s = _strtmp_t762;
    /* pass */
    if ((((unsigned long long)(f->throws_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr tt = Formatter_type_str(self, f->throws_ty);
        /* pass */
        if ((_tr_strlen(_tr_strz(tt)) > 0LL)) {
            /* pass */
            TrStr _strtmp_t763 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" throws ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tt)); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t763;
        }
    }
    /* pass */
    if ((((unsigned long long)(f->ret_ty)) != ((unsigned long long)(0LL)))) {
        /* pass */
        TrStr rt = Formatter_type_str(self, f->ret_ty);
        /* pass */
        if (((_tr_strlen(_tr_strz(rt)) > 0LL) && (strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("void"))) != 0))) {
            /* pass */
            TrStr _strtmp_t764 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rt)); _tr_str_release(_cl); _cres; });
            _tr_str_release(s);
            s = _strtmp_t764;
        }
    }
    /* pass */
    return _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(":")));
}

__attribute__((hot)) void Formatter_emit_function(Formatter* self, FunctionDef* f) {
    /* pass */
    Formatter_flush_standalone_before(self, f->line);
    /* pass */
    Formatter_emit_decorators(self, f->decorators);
    /* pass */
    ({ TrStr _sbt_t765 = (Formatter_pad(self)); StringBuilder_append(self->out, _sbt_t765); _tr_str_release(_sbt_t765); });
    /* pass */
    ({ TrStr _sbt_t766 = (Formatter_fn_header(self, f)); StringBuilder_append(self->out, _sbt_t766); _tr_str_release(_sbt_t766); });
    /* pass */
    ({ TrStr _sbt_t767 = (Formatter_trailing_for(self, f->line)); StringBuilder_append(self->out, _sbt_t767); _tr_str_release(_sbt_t767); });
    /* pass */
    StringBuilder_append(self->out, _tr_str_lit("\n"));
    /* pass */
    self->indent = (self->indent + 1LL);
    /* pass */
    if ((f->body->stmts->len == 0LL)) {
        /* pass */
        Formatter_line(self, _tr_str_lit("pass"));
    } else {
        /* pass */
        Formatter_emit_block(self, f->body);
    }
    /* pass */
    self->indent = (self->indent - 1LL);
}

__attribute__((hot)) void Formatter_emit_class(Formatter* self, ClassDef* c, TrStr kw) {
    /* pass */
    Formatter_flush_standalone_before(self, c->line);
    /* pass */
    Formatter_emit_decorators(self, c->decorators);
    /* pass */
    TrStr head = _tr_str_lit("");
    /* pass */
    if (c->is_public) {
        /* pass */
        TrStr _strtmp_t768 = _tr_str_lit("pub ");
        _tr_str_release(head);
        head = _strtmp_t768;
    }
    /* pass */
    TrStr _strtmp_t769 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(kw))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(c->name)); _tr_str_release(_cl); _cres; });
    _tr_str_release(head);
    head = _strtmp_t769;
    /* pass */
    if ((c->generics->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t770 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(head);
        head = _strtmp_t770;
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < c->generics->len)) {
            /* pass */
            if ((gi > 0LL)) {
                /* pass */
                TrStr _strtmp_t771 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(head);
                head = _strtmp_t771;
            }
            /* pass */
            TrStr _strtmp_t772 = ({ TrStr _cr = (List_TrStr_get(c->generics, gi)); TrStr _cres = _tr_strx_concat(_tr_strz(head), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(head);
            head = _strtmp_t772;
            /* pass */
            gi = (gi + 1LL);
        }
        /* pass */
        TrStr _strtmp_t773 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(head);
        head = _strtmp_t773;
    }
    /* pass */
    if ((c->base_classes->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t774 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(" extends ")))); TrStr _cr = (List_TrStr_get(c->base_classes, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(head);
        head = _strtmp_t774;
        /* pass */
        long long bi = 1LL;
        /* pass */
        while ((bi < c->base_classes->len)) {
            /* pass */
            TrStr _strtmp_t775 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(", ")))); TrStr _cr = (List_TrStr_get(c->base_classes, bi)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(head);
            head = _strtmp_t775;
            /* pass */
            bi = (bi + 1LL);
        }
    }
    /* pass */
    if ((c->iface_names->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t776 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(" implements ")));
        _tr_str_release(head);
        head = _strtmp_t776;
        /* pass */
        long long ii = 0LL;
        /* pass */
        while ((ii < c->iface_names->len)) {
            /* pass */
            if ((ii > 0LL)) {
                /* pass */
                TrStr _strtmp_t777 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(head);
                head = _strtmp_t777;
            }
            /* pass */
            TrStr _strtmp_t778 = ({ TrStr _cr = (List_TrStr_get(c->iface_names, ii)); TrStr _cres = _tr_strx_concat(_tr_strz(head), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(head);
            head = _strtmp_t778;
            /* pass */
            ii = (ii + 1LL);
        }
    }
    /* pass */
    ({ TrStr _at_t779 = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); Formatter_line(self, _at_t779); _tr_str_release(_at_t779); });
    /* pass */
    self->indent = (self->indent + 1LL);
    /* pass */
    bool had_body = false;
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < c->fields->len)) {
        /* pass */
        FieldDef* fld = ((FieldDef*)List_ptr_get(c->fields, fi));
        /* pass */
        TrStr fs = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("pub ")), _tr_strz(fld->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_type_str(self, fld->ty)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        /* pass */
        if ((((unsigned long long)(fld->default_val)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr _strtmp_t780 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(fs), _tr_strz(_tr_str_lit(" = ")))); TrStr _cr = (Formatter_expr_str(self, fld->default_val)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(fs);
            fs = _strtmp_t780;
        }
        /* pass */
        Formatter_line(self, fs);
        /* pass */
        had_body = true;
        /* pass */
        fi = (fi + 1LL);
        _tr_str_release(fs);
    }
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < c->methods->len)) {
        /* pass */
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(c->methods, mi)));
        /* pass */
        had_body = true;
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    if ((!had_body)) {
        /* pass */
        Formatter_line(self, _tr_str_lit("pass"));
    }
    /* pass */
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_enum(Formatter* self, EnumDef* e) {
    /* pass */
    Formatter_flush_standalone_before(self, e->line);
    /* pass */
    Formatter_emit_decorators(self, e->decorators);
    /* pass */
    TrStr head = _tr_str_lit("");
    /* pass */
    if (e->is_public) {
        /* pass */
        TrStr _strtmp_t781 = _tr_str_lit("pub ");
        _tr_str_release(head);
        head = _strtmp_t781;
    }
    /* pass */
    TrStr _strtmp_t782 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("enum ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e->name)); _tr_str_release(_cl); _cres; });
    _tr_str_release(head);
    head = _strtmp_t782;
    /* pass */
    if ((e->generics->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t783 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(head);
        head = _strtmp_t783;
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < e->generics->len)) {
            /* pass */
            if ((gi > 0LL)) {
                /* pass */
                TrStr _strtmp_t784 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(head);
                head = _strtmp_t784;
            }
            /* pass */
            TrStr _strtmp_t785 = ({ TrStr _cr = (List_TrStr_get(e->generics, gi)); TrStr _cres = _tr_strx_concat(_tr_strz(head), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(head);
            head = _strtmp_t785;
            /* pass */
            gi = (gi + 1LL);
        }
        /* pass */
        TrStr _strtmp_t786 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(head);
        head = _strtmp_t786;
    }
    /* pass */
    ({ TrStr _at_t787 = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); Formatter_line(self, _at_t787); _tr_str_release(_at_t787); });
    /* pass */
    self->indent = (self->indent + 1LL);
    /* pass */
    long long vi = 0LL;
    /* pass */
    while ((vi < e->variants->len)) {
        /* pass */
        VariantDef* v = ((VariantDef*)List_ptr_get(e->variants, vi));
        /* pass */
        if ((v->fields->len > 0LL)) {
            /* pass */
            ({ TrStr _at_t788 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(v->name), _tr_strz(_tr_str_lit("(")))); TrStr _cr = (Formatter_params_str(self, v->fields)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t788); _tr_str_release(_at_t788); });
        } else {
            /* pass */
            Formatter_line(self, v->name);
        }
        /* pass */
        vi = (vi + 1LL);
    }
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < e->methods->len)) {
        /* pass */
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(e->methods, mi)));
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_interface(Formatter* self, InterfaceDef* it) {
    /* pass */
    Formatter_flush_standalone_before(self, it->line);
    /* pass */
    TrStr head = _tr_str_lit("");
    /* pass */
    if (it->is_public) {
        /* pass */
        TrStr _strtmp_t789 = _tr_str_lit("pub ");
        _tr_str_release(head);
        head = _strtmp_t789;
    }
    /* pass */
    TrStr _strtmp_t790 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("interface ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(it->name)); _tr_str_release(_cl); _cres; });
    _tr_str_release(head);
    head = _strtmp_t790;
    /* pass */
    if ((it->generics->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t791 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("[")));
        _tr_str_release(head);
        head = _strtmp_t791;
        /* pass */
        long long gi = 0LL;
        /* pass */
        while ((gi < it->generics->len)) {
            /* pass */
            if ((gi > 0LL)) {
                /* pass */
                TrStr _strtmp_t792 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(head);
                head = _strtmp_t792;
            }
            /* pass */
            TrStr _strtmp_t793 = ({ TrStr _cr = (List_TrStr_get(it->generics, gi)); TrStr _cres = _tr_strx_concat(_tr_strz(head), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(head);
            head = _strtmp_t793;
            /* pass */
            gi = (gi + 1LL);
        }
        /* pass */
        TrStr _strtmp_t794 = _tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit("]")));
        _tr_str_release(head);
        head = _strtmp_t794;
    }
    /* pass */
    ({ TrStr _at_t795 = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); Formatter_line(self, _at_t795); _tr_str_release(_at_t795); });
    /* pass */
    self->indent = (self->indent + 1LL);
    /* pass */
    long long mi = 0LL;
    /* pass */
    while ((mi < it->methods->len)) {
        /* pass */
        Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(it->methods, mi)));
        /* pass */
        mi = (mi + 1LL);
    }
    /* pass */
    if ((it->methods->len == 0LL)) {
        /* pass */
        Formatter_line(self, _tr_str_lit("pass"));
    }
    /* pass */
    self->indent = (self->indent - 1LL);
    _tr_str_release(head);
}

__attribute__((hot)) void Formatter_emit_decl(Formatter* self, Decl* dp) {
    /* pass */
    __auto_type _t796 = (*dp);
    if (_t796.tag == Decl_DFunction) {
        __auto_type f = _t796.data.DFunction.func;
        Formatter_emit_function(self, f);
    } else if (_t796.tag == Decl_DDecoratorDef) {
        __auto_type f = _t796.data.DDecoratorDef.func;
        /* pass */
        Formatter_flush_standalone_before(self, f->line);
        /* pass */
        ({ TrStr _at_t797 = (({ TrStr _cr = (Formatter_fn_header(self, f)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("decorator ")), _cr.data); _tr_str_release(_cr); _cres; })); Formatter_line(self, _at_t797); _tr_str_release(_at_t797); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        if ((f->body->stmts->len == 0LL)) {
            /* pass */
            Formatter_line(self, _tr_str_lit("pass"));
        } else {
            /* pass */
            Formatter_emit_block(self, f->body);
        }
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t796.tag == Decl_DClass) {
        __auto_type c = _t796.data.DClass.cls;
        Formatter_emit_class(self, c, _tr_str_lit("class"));
    } else if (_t796.tag == Decl_DActor) {
        __auto_type c = _t796.data.DActor.cls;
        Formatter_emit_class(self, c, _tr_str_lit("actor"));
    } else if (_t796.tag == Decl_DEnum) {
        __auto_type e = _t796.data.DEnum.enm;
        Formatter_emit_enum(self, e);
    } else if (_t796.tag == Decl_DInterface) {
        __auto_type it = _t796.data.DInterface.iface;
        Formatter_emit_interface(self, it);
    } else if (_t796.tag == Decl_DExtend) {
        __auto_type target = _t796.data.DExtend.target;
__auto_type methods = _t796.data.DExtend.methods;
        /* pass */
        ({ TrStr _at_t798 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("extend ")), _tr_strz(target))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":"))); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t798); _tr_str_release(_at_t798); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        long long mi = 0LL;
        /* pass */
        while ((mi < methods->len)) {
            /* pass */
            Formatter_emit_function(self, ((FunctionDef*)List_ptr_get(methods, mi)));
            /* pass */
            mi = (mi + 1LL);
        }
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t796.tag == Decl_DImport) {
        __auto_type path = _t796.data.DImport.path;
__auto_type alias = _t796.data.DImport.alias;
        /* pass */
        if ((_tr_strlen(_tr_strz(alias)) > 0LL)) {
            /* pass */
            ({ TrStr _at_t799 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("import ")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" as "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(alias)); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t799); _tr_str_release(_at_t799); });
        } else {
            /* pass */
            ({ TrStr _at_t800 = (_tr_strx_concat(_tr_strz(_tr_str_lit("import ")), _tr_strz(path))); Formatter_line(self, _at_t800); _tr_str_release(_at_t800); });
        }
    } else if (_t796.tag == Decl_DFromImport) {
        __auto_type path = _t796.data.DFromImport.path;
__auto_type items = _t796.data.DFromImport.items;
        /* pass */
        TrStr s = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("from ")), _tr_strz(path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" import "))); _tr_str_release(_cl); _cres; });
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < items->len)) {
            /* pass */
            if ((i > 0LL)) {
                /* pass */
                TrStr _strtmp_t801 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                _tr_str_release(s);
                s = _strtmp_t801;
            }
            /* pass */
            ImportItem* it = ((ImportItem*)List_ptr_get(items, i));
            /* pass */
            TrStr _strtmp_t802 = _tr_strx_concat(_tr_strz(s), _tr_strz(it->name));
            _tr_str_release(s);
            s = _strtmp_t802;
            /* pass */
            if ((_tr_strlen(_tr_strz(it->alias)) > 0LL)) {
                /* pass */
                TrStr _strtmp_t803 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" as ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(it->alias)); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t803;
            }
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        Formatter_line(self, s);
        _tr_str_release(s);
    } else if (_t796.tag == Decl_DExtern) {
        __auto_type abi = _t796.data.DExtern.abi;
__auto_type functions = _t796.data.DExtern.functions;
        /* pass */
        ({ TrStr _at_t804 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("extern \"")), _tr_strz(abi))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\":"))); _tr_str_release(_cl); _cres; })); Formatter_line(self, _at_t804); _tr_str_release(_at_t804); });
        /* pass */
        self->indent = (self->indent + 1LL);
        /* pass */
        long long fi = 0LL;
        /* pass */
        while ((fi < functions->len)) {
            /* pass */
            ({ TrStr _at_t805 = (Formatter_fn_header_no_colon(self, ((FunctionDef*)List_ptr_get(functions, fi)))); Formatter_line(self, _at_t805); _tr_str_release(_at_t805); });
            /* pass */
            fi = (fi + 1LL);
        }
        /* pass */
        self->indent = (self->indent - 1LL);
    } else if (_t796.tag == Decl_DTypeAlias) {
        __auto_type name = _t796.data.DTypeAlias.name;
__auto_type target = _t796.data.DTypeAlias.target;
        /* pass */
        ({ TrStr _at_t806 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (Formatter_type_str(self, target)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); Formatter_line(self, _at_t806); _tr_str_release(_at_t806); });
    } else if (_t796.tag == Decl_DTopLevelStmt) {
        __auto_type s = _t796.data.DTopLevelStmt.stmt;
        /* pass */
        Formatter_emit_stmt(self, s, 0LL);
    } else if (1) {
        __auto_type _ = _t796;
        /* pass */
    }
}

__attribute__((hot)) TrStr Formatter_fn_header_no_colon(Formatter* self, FunctionDef* f) {
    /* pass */
    TrStr h = Formatter_fn_header(self, f);
    /* pass */
    if (({ TrStr _wt_t807 = (_tr_str_wrap(_tr_str_slice(_tr_strz(h), (_tr_strlen(_tr_strz(h)) - 1LL), _tr_strlen(_tr_strz(h))))); __auto_type _wr = (((_tr_strlen(_tr_strz(h)) > 0LL) && (strcmp(_wt_t807.data, _tr_strz(_tr_str_lit(":"))) == 0))); _tr_str_release(_wt_t807); _wr; })) {
        /* pass */
        return _tr_str_wrap(_tr_str_slice(_tr_strz(h), 0LL, (_tr_strlen(_tr_strz(h)) - 1LL)));
    }
    /* pass */
    return h;
}

__attribute__((hot)) bool Formatter_is_block_decl(Formatter* self, Decl* dp) {
    /* pass */
    __auto_type _t808 = (*dp);
    if (_t808.tag == Decl_DFunction) {
        __auto_type _ = _t808.data.DFunction.func;
        return true;
    } else if (_t808.tag == Decl_DDecoratorDef) {
        __auto_type _ = _t808.data.DDecoratorDef.func;
        return true;
    } else if (_t808.tag == Decl_DClass) {
        __auto_type _ = _t808.data.DClass.cls;
        return true;
    } else if (_t808.tag == Decl_DActor) {
        __auto_type _ = _t808.data.DActor.cls;
        return true;
    } else if (_t808.tag == Decl_DEnum) {
        __auto_type _ = _t808.data.DEnum.enm;
        return true;
    } else if (_t808.tag == Decl_DInterface) {
        __auto_type _ = _t808.data.DInterface.iface;
        return true;
    } else if (_t808.tag == Decl_DExtend) {
        return true;
    } else if (_t808.tag == Decl_DExtern) {
        return true;
    } else if (1) {
        __auto_type _ = _t808;
        return false;
    }
}

__attribute__((hot)) TrStr Formatter_format_program(Formatter* self, Program* prog) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < prog->decls->len)) {
        /* pass */
        Decl* dp = ((Decl*)List_ptr_get(prog->decls, i));
        /* pass */
        if ((((i > 0LL) && Formatter_is_block_decl(self, dp)) && (self->out->buf->len > 0LL))) {
            /* pass */
            StringBuilder_append(self->out, _tr_str_lit("\n"));
        }
        /* pass */
        Formatter_emit_decl(self, dp);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    Formatter_flush_remaining(self);
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(self->out));
}

