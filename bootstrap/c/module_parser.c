#include "tauraro_types.h"

TrStr Parser__peek_cmp_op(Parser* self);

__attribute__((malloc,returns_nonnull,hot)) Parser* Parser_init(List_Token* tokens, List_i64* lines) {
    /* pass */
    Parser* p = ((Parser*)_tr_obj_alloc(sizeof(Parser)));
    /* pass */
    p->tokens = tokens;
    /* pass */
    p->lines = lines;
    /* pass */
    p->cols = (void*)List_i64_new();
    /* pass */
    p->src_text = _tr_str_lit("");
    /* pass */
    p->pos = 0LL;
    /* pass */
    p->error_count = 0LL;
    /* pass */
    p->current_file = _tr_str_lit("");
    /* pass */
    p->import_aliases = _tr_dict_new(8LL);
    /* pass */
    return p;
}

__attribute__((hot)) Token Parser_peek(Parser* self) {
    /* pass */
    if ((self->pos < self->tokens->len)) {
        /* pass */
        return List_Token_get(self->tokens, self->pos);
    }
    /* pass */
    return Token_make_Eof();
}

__attribute__((hot)) Token Parser_advance(Parser* self) {
    /* pass */
    Token tok = Parser_peek(self);
    /* pass */
    if ((self->pos < self->tokens->len)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return tok;
}

__attribute__((hot)) void Parser_skip_newlines(Parser* self) {
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        __auto_type _t22 = Parser_peek(self);
        if (_t22.tag == Token_Newline) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            going = true;
        } else if (1) {
            __auto_type _ = _t22;
            /* pass */
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_skip_newlines_and_indent(Parser* self) {
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        __auto_type _t23 = Parser_peek(self);
        if ((_t23.tag == Token_Newline || _t23.tag == Token_Indent || _t23.tag == Token_Dedent)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            going = true;
        } else if (1) {
            __auto_type _ = _t23;
            /* pass */
            /* pass */
        }
    }
}

__attribute__((hot)) void Parser_expect_newline(Parser* self) {
    /* pass */
    __auto_type _t24 = Parser_peek(self);
    if ((_t24.tag == Token_Newline || _t24.tag == Token_Semicolon)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t24;
        /* pass */
        /* pass */
    }
}

__attribute__((hot)) bool Parser_at_end(Parser* self) {
    /* pass */
    __auto_type _t25 = Parser_peek(self);
    if (_t25.tag == Token_Eof) {
        /* pass */
        return true;
    } else if (1) {
        __auto_type _ = _t25;
        /* pass */
        /* pass */
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long Parser_cur_line(Parser* self) {
    /* pass */
    if ((self->pos < self->lines->len)) {
        /* pass */
        return List_i64_get(self->lines, self->pos);
    }
    /* pass */
    if ((self->lines->len > 0LL)) {
        /* pass */
        return List_i64_get(self->lines, (self->lines->len - 1LL));
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) long long Parser_cur_col(Parser* self) {
    /* pass */
    if ((self->pos < self->cols->len)) {
        /* pass */
        return List_i64_get(self->cols, self->pos);
    }
    /* pass */
    if ((self->cols->len > 0LL)) {
        /* pass */
        return List_i64_get(self->cols, (self->cols->len - 1LL));
    }
    /* pass */
    return 0LL;
}

__attribute__((hot)) void Parser_emit_diag(Parser* self, TrStr msg, TrStr hint) {
    /* pass */
    long long ln = Parser_cur_line(self);
    /* pass */
    long long col = Parser_cur_col(self);
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (c_bold(msg)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; }))); printf("\n"); });
    /* pass */
    TrStr loc = _tr_str_lit("");
    /* pass */
    if ((_tr_strlen(_tr_strz(self->current_file)) > 0LL)) {
        /* pass */
        TrStr _strtmp_t26 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t26;
    }
    /* pass */
    TrStr head = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(loc), _tr_strz(_tr_str_lit(":")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if ((col > 0LL)) {
        /* pass */
        TrStr _strtmp_t27 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(head);
        head = _strtmp_t27;
    }
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_cyan(_tr_str_lit("-->"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(head)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    if (((_tr_strlen(_tr_strz(self->src_text)) > 0LL) && (ln > 0LL))) {
        /* pass */
        TrStr srcline = _nth_source_line(self->src_text, ln);
        /* pass */
        if ((_tr_strlen(_tr_strz(srcline)) > 0LL)) {
            /* pass */
            TrStr gnum = ({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("   ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" | "))); _tr_str_release(_cl); _cres; });
            /* pass */
            TrStr gbar = ({ TrStr _cl = (_spaces((_tr_strlen(_tr_strz(gnum)) - 2LL))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("| "))); _tr_str_release(_cl); _cres; });
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_dim(gnum)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(srcline)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            if ((col > 0LL)) {
                /* pass */
                long long width = 1LL;
                /* pass */
                if ((((self->pos + 1LL) < self->cols->len) && ((self->pos + 1LL) < self->lines->len))) {
                    /* pass */
                    if ((List_i64_get(self->lines, (self->pos + 1LL)) == ln)) {
                        /* pass */
                        long long d = (List_i64_get(self->cols, (self->pos + 1LL)) - col);
                        /* pass */
                        if (((d > 0LL) && (d < 200LL))) {
                            /* pass */
                            width = d;
                        }
                    }
                }
                /* pass */
                ({ TrStr _at_t28 = (repeat_char(_tr_str_lit("^"), width)); ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (c_dim(gbar)); TrStr _cr = (_spaces((col - 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cr = (c_red(_at_t28)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; }))); printf("\n"); }); _tr_str_release(_at_t28); });
            }
            _tr_str_release(gnum);
        }
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(hint)) > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_green(_tr_str_lit("= help"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(hint)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) TrStr Parser_consume_ident(Parser* self) {
    /* pass */
    __auto_type _t29 = Parser_peek(self);
    if (_t29.tag == Token_Ident) {
        __auto_type name = _t29.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_retain(name);
    } else if (_t29.tag == Token_KwInt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("int");
    } else if (_t29.tag == Token_KwFloat) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("float");
    } else if (_t29.tag == Token_KwBool) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("bool");
    } else if ((_t29.tag == Token_KwStr || _t29.tag == Token_KwString)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("str");
    } else if (_t29.tag == Token_KwChar) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("char");
    } else if (_t29.tag == Token_KwVoid) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("void");
    } else if (_t29.tag == Token_KwExtend) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("extend");
    } else if (_t29.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("None");
    } else if (_t29.tag == Token_KwLambda) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("lambda");
    } else if (_t29.tag == Token_KwSpawn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("spawn");
    } else if (_t29.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("async");
    } else if (1) {
        __auto_type _ = _t29;
        /* pass */
        /* pass */
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr Parser_consume_module_ident(Parser* self) {
    /* pass */
    __auto_type _t30 = Parser_peek(self);
    if (_t30.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("async");
    } else if (_t30.tag == Token_KwMatch) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("match");
    } else if (_t30.tag == Token_KwFor) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("for");
    } else if (_t30.tag == Token_KwIn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("in");
    } else if (_t30.tag == Token_KwIs) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return _tr_str_lit("is");
    } else if (1) {
        __auto_type _ = _t30;
        /* pass */
        return Parser_consume_ident(self);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) AstType* Parser_parse_type(Parser* self) {
    /* pass */
    TrStr name = _tr_str_lit("");
    /* pass */
    __auto_type _t31 = Parser_peek(self);
    if (_t31.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        _tr_str_release(name);
        return AstType_init(_tr_str_lit("void"));
    } else if (_t31.tag == Token_LParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* tup_t = AstType_init(_tr_str_lit("Tuple"));
        /* pass */
        while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
            /* pass */
            AstType* elem_t = Parser_parse_type(self);
            /* pass */
            List_ptr_append(tup_t->args, box_asttype(elem_t));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        _tr_str_release(name);
        return tup_t;
    } else if (_t31.tag == Token_LBracket) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* arr_t = AstType_init(_tr_str_lit("Array"));
        /* pass */
        AstType* arr_elem = Parser_parse_type(self);
        /* pass */
        List_ptr_append(arr_t->args, box_asttype(arr_elem));
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Semicolon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type _t32 = Parser_peek(self);
            if (_t32.tag == Token_IntLit) {
                __auto_type arr_sz = _t32.data.IntLit.val;
                /* pass */
                arr_t->array_size = arr_sz;
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t32;
                /* pass */
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        _tr_str_release(name);
        return arr_t;
    } else if (_t31.tag == Token_KwDef) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* dfn_t = AstType_init(_tr_str_lit("def"));
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                AstType* dpt = Parser_parse_type(self);
                /* pass */
                List_ptr_append(dfn_t->args, box_asttype(dpt));
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
        /* pass */
        AstType* dret = AstType_init(_tr_str_lit("void"));
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            dret = Parser_parse_type(self);
        }
        /* pass */
        List_ptr_append(dfn_t->args, box_asttype(dret));
        /* pass */
        _tr_str_release(name);
        return dfn_t;
    } else if (_t31.tag == Token_KwMut) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr mr_name = Parser_consume_ident(self);
        /* pass */
        if ((strcmp(_tr_strz(mr_name), _tr_strz(_tr_str_lit("ref"))) == 0)) {
            /* pass */
            AstType* mr_inner = Parser_parse_type(self);
            /* pass */
            mr_inner->is_borrow = true;
            /* pass */
            mr_inner->is_mut_borrow = true;
            /* pass */
            _tr_str_release(mr_name);
            _tr_str_release(name);
            return mr_inner;
        }
        /* pass */
        _tr_str_release(mr_name);
        _tr_str_release(name);
        return AstType_init(_tr_str_lit("void"));
    } else if (1) {
        __auto_type _ = _t31;
        /* pass */
        TrStr _strtmp_t33 = Parser_consume_ident(self);
        _tr_str_release(name);
        name = _strtmp_t33;
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        _tr_str_release(name);
        return AstType_init(_tr_str_lit("void"));
    }
    /* pass */
    if ((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit("ref"))) == 0)) {
        /* pass */
        AstType* ref_inner = Parser_parse_type(self);
        /* pass */
        ref_inner->is_borrow = true;
        /* pass */
        _tr_str_release(name);
        return ref_inner;
    }
    /* pass */
    __auto_type _t34 = Parser_peek(self);
    if (_t34.tag == Token_LBracket) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* t = AstType_init(name);
        /* pass */
        bool going = true;
        /* pass */
        while (going) {
            /* pass */
            __auto_type _t35 = Parser_peek(self);
            if ((_t35.tag == Token_RBracket || _t35.tag == Token_Eof)) {
                /* pass */
                going = false;
            } else if (1) {
                __auto_type _ = _t35;
                /* pass */
                /* pass */
            }
            /* pass */
            if (going) {
                /* pass */
                __auto_type arg_t = Parser_parse_type(self);
                /* pass */
                List_ptr_append(t->args, box_asttype(arg_t));
                /* pass */
                __auto_type _t36 = Parser_peek(self);
                if (_t36.tag == Token_Comma) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if (_t36.tag == Token_Semicolon) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    __auto_type _t37 = Parser_peek(self);
                    if (_t37.tag == Token_IntLit) {
                        __auto_type gsz = _t37.data.IntLit.val;
                        /* pass */
                        t->array_size = gsz;
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t37;
                        /* pass */
                    }
                    /* pass */
                    going = false;
                } else if (1) {
                    __auto_type _ = _t36;
                    /* pass */
                    going = false;
                }
            }
        }
        /* pass */
        __auto_type _t38 = Parser_peek(self);
        if (_t38.tag == Token_RBracket) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t38;
            /* pass */
            /* pass */
        }
        /* pass */
        _tr_str_release(name);
        return t;
    } else if (1) {
        __auto_type _ = _t34;
        /* pass */
        /* pass */
    }
    /* pass */
    return AstType_init(name);
}

__attribute__((hot)) List_ptr* Parser_parse_param_list(Parser* self) {
    /* pass */
    List_ptr* pl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type _t39 = Parser_peek(self);
        if ((_t39.tag == Token_RParen || _t39.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t39;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            TrStr item_name = Parser_consume_ident(self);
            /* pass */
            if ((strcmp(_tr_strz(item_name), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                AstType** ty_ptr = (AstType**)(0LL);
                /* pass */
                __auto_type _t40 = Parser_peek(self);
                if (_t40.tag == Token_Colon) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    __auto_type ty = Parser_parse_type(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                        /* pass */
                        ty->from_param = Parser_consume_ident(self);
                        /* pass */
                        List_TrStr_append(ty->from_regions, ty->from_param);
                    }
                    /* pass */
                    ty_ptr = box_asttype(ty);
                } else if (1) {
                    __auto_type _ = _t40;
                    /* pass */
                    if ((strcmp(_tr_strz(item_name), _tr_strz(_tr_str_lit("ref"))) == 0)) {
                        /* pass */
                        AstType* bt = Parser_parse_type(self);
                        /* pass */
                        bt->is_borrow = true;
                        /* pass */
                        if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
                            /* pass */
                            self->pos = (self->pos + 1LL);
                            /* pass */
                            bt->from_param = Parser_consume_ident(self);
                            /* pass */
                            List_TrStr_append(bt->from_regions, bt->from_param);
                        }
                        /* pass */
                        ty_ptr = box_asttype(bt);
                        /* pass */
                        TrStr _strtmp_t41 = _tr_str_retain(bt->name);
                        _tr_str_release(item_name);
                        item_name = _strtmp_t41;
                    }
                }
                /* pass */
                Param* p = Param_init(item_name, ty_ptr);
                /* pass */
                if ((((unsigned long long)(ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    if ((*ty_ptr)->is_borrow) {
                        /* pass */
                        p->is_ref = true;
                    }
                    /* pass */
                    if ((*ty_ptr)->is_mut_borrow) {
                        /* pass */
                        p->is_mut_ref = true;
                    }
                }
                /* pass */
                if ((Parser_peek(self).tag == Token_make_DotDotDot().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    p->is_variadic = true;
                }
                /* pass */
                List_ptr_append(pl, _tr_obj_retain(p));
                /* pass */
                __auto_type _t42 = Parser_peek(self);
                if (_t42.tag == Token_Comma) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    Parser_skip_newlines_and_indent(self);
                } else if (1) {
                    __auto_type _ = _t42;
                    /* pass */
                    going = false;
                }
            } else {
                /* pass */
                going = false;
            }
        }
    }
    /* pass */
    return pl;
}

__attribute__((hot)) Block* Parser_parse_block(Parser* self) {
    /* pass */
    Block* b = Block_init();
    /* pass */
    bool is_inline = false;
    /* pass */
    __auto_type _t43 = Parser_peek(self);
    if (_t43.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (_t43.tag == Token_Newline) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
    } else if (1) {
        __auto_type _ = _t43;
        /* pass */
        is_inline = true;
    }
    /* pass */
    if (is_inline) {
        /* pass */
        Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
        /* pass */
        Block_push(b, Parser_parse_stmt(self));
        /* pass */
        bool semi_going = true;
        /* pass */
        while (semi_going) {
            /* pass */
            semi_going = false;
            /* pass */
            __auto_type _t44 = Parser_peek(self);
            if (_t44.tag == Token_Semicolon) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                __auto_type _t45 = Parser_peek(self);
                if ((_t45.tag == Token_Newline || _t45.tag == Token_Eof || _t45.tag == Token_Dedent)) {
                    /* pass */
                    /* pass */
                } else if (1) {
                    __auto_type _ = _t45;
                    /* pass */
                    Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
                    /* pass */
                    Block_push(b, Parser_parse_stmt(self));
                    /* pass */
                    semi_going = true;
                }
            } else if (1) {
                __auto_type _ = _t44;
                /* pass */
                if ((self->pos > 0LL)) {
                    /* pass */
                    __auto_type _t46 = List_Token_get(self->tokens, (self->pos - 1LL));
                    if (_t46.tag == Token_Semicolon) {
                        /* pass */
                        __auto_type _t47 = Parser_peek(self);
                        if ((_t47.tag == Token_Newline || _t47.tag == Token_Eof || _t47.tag == Token_Dedent)) {
                            /* pass */
                            /* pass */
                        } else if (1) {
                            __auto_type _ = _t47;
                            /* pass */
                            Block_push(b, box_stmt(Stmt_ctor_SLine(Parser_cur_line(self))));
                            /* pass */
                            Block_push(b, Parser_parse_stmt(self));
                            /* pass */
                            semi_going = true;
                        }
                    } else if (1) {
                        __auto_type _ = _t46;
                        /* pass */
                        /* pass */
                    }
                }
            }
        }
        /* pass */
        return b;
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        __auto_type _t48 = Parser_peek(self);
        if ((_t48.tag == Token_Dedent || _t48.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t48;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t49 = Parser_peek(self);
            if ((_t49.tag == Token_Dedent || _t49.tag == Token_Eof)) {
                /* pass */
                going = false;
            } else if (1) {
                __auto_type _ = _t49;
                /* pass */
                /* pass */
            }
            /* pass */
            if (going) {
                /* pass */
                long long _sline = Parser_cur_line(self);
                /* pass */
                __auto_type s = Parser_parse_stmt(self);
                /* pass */
                Block_push(b, box_stmt(Stmt_ctor_SLine(_sline)));
                /* pass */
                Block_push(b, s);
            }
        }
    }
    /* pass */
    __auto_type _t50 = Parser_peek(self);
    if (_t50.tag == Token_Dedent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t50;
        /* pass */
        /* pass */
    }
    /* pass */
    return b;
}

__attribute__((hot)) Stmt* Parser_parse_stmt(Parser* self) {
    /* pass */
    __auto_type _t51 = Parser_peek(self);
    if (_t51.tag == Token_KwReturn) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t52 = Parser_peek(self);
        if ((_t52.tag == Token_Newline || _t52.tag == Token_Dedent || _t52.tag == Token_Eof)) {
            /* pass */
            return box_stmt(Stmt_ctor_SReturn((Expr*)(0LL)));
        } else if (1) {
            __auto_type _ = _t52;
            /* pass */
            /* pass */
        }
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SReturn(e));
    } else if (_t51.tag == Token_KwPass) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (_t51.tag == Token_KwBreak) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* brk_val = (Expr*)(0LL);
        /* pass */
        __auto_type _t53 = Parser_peek(self);
        if (_t53.tag == Token_Newline) {
            /* pass */
        } else if (_t53.tag == Token_Dedent) {
            /* pass */
        } else if (_t53.tag == Token_Eof) {
            /* pass */
        } else if (_t53.tag == Token_Semicolon) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t53;
            /* pass */
            brk_val = Parser_parse_expr(self);
        }
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SBreak(brk_val));
    } else if (_t51.tag == Token_KwContinue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_make_SContinue());
    } else if (_t51.tag == Token_KwRaise) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SRaise(e));
    } else if (_t51.tag == Token_KwAsm) {
        /* pass */
        return Parser_parse_asm_stmt(self);
    } else if (_t51.tag == Token_KwSpawn) {
        /* pass */
        return Parser_parse_spawn_stmt(self);
    } else if (_t51.tag == Token_KwTaskGroup) {
        /* pass */
        return Parser_parse_taskgroup_stmt(self);
    } else if (_t51.tag == Token_KwGpu) {
        /* pass */
        return Parser_parse_gpu_stmt(self);
    } else if (_t51.tag == Token_KwIf) {
        /* pass */
        return Parser_parse_if_stmt(self);
    } else if (_t51.tag == Token_KwWhile) {
        /* pass */
        return Parser_parse_while_stmt(self);
    } else if (_t51.tag == Token_KwLoop) {
        /* pass */
        return Parser_parse_loop_stmt(self);
    } else if (_t51.tag == Token_KwFor) {
        /* pass */
        return Parser_parse_for_stmt(self);
    } else if (_t51.tag == Token_KwMatch) {
        /* pass */
        return Parser_parse_match_stmt(self);
    } else if (_t51.tag == Token_KwMut) {
        /* pass */
        return Parser_parse_let_stmt(self, true);
    } else if (_t51.tag == Token_KwShared) {
        /* pass */
        return Parser_parse_shared_let_stmt(self);
    } else if (_t51.tag == Token_KwConst) {
        /* pass */
        return Parser_parse_const_let_stmt(self);
    } else if (_t51.tag == Token_KwUnsafe) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t54 = Parser_peek(self);
        if (_t54.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t54;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type body = Parser_parse_block(self);
        /* pass */
        return box_stmt(Stmt_ctor_SUnsafe(body));
    } else if (_t51.tag == Token_KwDefer) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Stmt* deferred = Parser_parse_stmt(self);
        /* pass */
        return box_stmt(Stmt_ctor_SDefer(deferred));
    } else if (_t51.tag == Token_KwWith) {
        /* pass */
        return Parser_parse_with_stmt(self);
    } else if (_t51.tag == Token_KwTry) {
        /* pass */
        return Parser_parse_try_stmt(self);
    } else if (_t51.tag == Token_KwAssert) {
        /* pass */
        return Parser_parse_assert_stmt(self);
    } else if (_t51.tag == Token_KwFrom) {
        /* pass */
        Parser_parse_from_import(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (_t51.tag == Token_KwImport) {
        /* pass */
        Parser_parse_import(self);
        /* pass */
        return box_stmt(Stmt_make_SPass());
    } else if (_t51.tag == Token_KwClass) {
        /* pass */
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t51.tag == Token_KwEnum) {
        /* pass */
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t51.tag == Token_KwInterface) {
        /* pass */
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t51.tag == Token_KwExtend) {
        /* pass */
        return box_stmt(Stmt_ctor_SLocalDecl(Parser_parse_decl(self)));
    } else if (_t51.tag == Token_KwDef) {
        /* pass */
        long long _ldline = Parser_cur_line(self);
        /* pass */
        FunctionDef* _ldf = Parser_parse_function_def(self, false);
        /* pass */
        _ldf->line = _ldline;
        /* pass */
        return box_stmt(Stmt_ctor_SLocalDecl(box_decl(Decl_ctor_DFunction(_ldf))));
    } else if (1) {
        __auto_type _ = _t51;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type _t55 = Parser_peek(self);
    if (_t55.tag == Token_Ident) {
        __auto_type cs_nm = _t55.data.Ident.name;
        /* pass */
        if ((strcmp(_tr_strz(cs_nm), _tr_strz(_tr_str_lit("type"))) == 0)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr _ta_name = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType* _ta_ty = Parser_parse_type(self);
                /* pass */
                Parser_expect_newline(self);
                /* pass */
                _tr_str_release(_ta_name);
                return box_stmt(Stmt_make_SPass());
            }
        }
        /* pass */
        if ((strcmp(_tr_strz(cs_nm), _tr_strz(_tr_str_lit("chan_select"))) == 0)) {
            /* pass */
            return Parser_parse_chan_select_stmt(self);
        }
        /* pass */
        if ((strcmp(_tr_strz(cs_nm), _tr_strz(_tr_str_lit("var"))) == 0)) {
            /* pass */
            long long _var_save = self->pos;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type _t56 = Parser_peek(self);
            if (_t56.tag == Token_Ident) {
                __auto_type _ = _t56.data.Ident.name;
                /* pass */
                return Parser_parse_assign_or_expr_stmt(self);
            } else if (1) {
                __auto_type _ = _t56;
                /* pass */
                self->pos = _var_save;
            }
        }
    } else if (1) {
        __auto_type _ = _t55;
        /* pass */
    }
    /* pass */
    return Parser_parse_assign_or_expr_stmt(self);
}

__attribute__((hot)) Stmt* Parser_parse_try_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t57 = Parser_peek(self);
    if (_t57.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t57;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type try_body = Parser_parse_block(self);
    /* pass */
    List_ptr* catches = (void*)List_ptr_new();
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwExcept().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType** err_type = (AstType**)(0LL);
        /* pass */
        TrStr err_name = _tr_str_lit("");
        /* pass */
        if ((Parser_peek(self).tag != Token_make_Colon().tag)) {
            /* pass */
            bool is_bare_var = false;
            /* pass */
            __auto_type _t58 = Parser_peek(self);
            if (_t58.tag == Token_Ident) {
                __auto_type _ = _t58.data.Ident.name;
                /* pass */
                long long save_pos = self->pos;
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                    /* pass */
                    is_bare_var = true;
                }
                /* pass */
                self->pos = save_pos;
            } else if (1) {
                __auto_type _ = _t58;
                /* pass */
                /* pass */
            }
            /* pass */
            if (is_bare_var) {
                /* pass */
                TrStr _strtmp_t59 = Parser_consume_ident(self);
                _tr_str_release(err_name);
                err_name = _strtmp_t59;
            } else {
                /* pass */
                err_type = box_asttype(Parser_parse_type(self));
                /* pass */
                if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    TrStr _strtmp_t60 = Parser_consume_ident(self);
                    _tr_str_release(err_name);
                    err_name = _strtmp_t60;
                }
            }
        }
        /* pass */
        __auto_type _t61 = Parser_peek(self);
        if (_t61.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t61;
            /* pass */
            /* pass */
        }
        /* pass */
        Block* body = Parser_parse_block(self);
        /* pass */
        CatchClause* c = CatchClause_init(err_name, (Block**)(0LL));
        /* pass */
        /* unsafe block */
        /* pass */
        Block** body_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
        /* pass */
        (*body_ptr = body);
        /* pass */
        c->body = body_ptr;
        /* pass */
        c->err_type = err_type;
        /* pass */
        /* unsafe block */
        /* pass */
        CatchClause** c_ptr = ((CatchClause**)_tr_c_calloc((size_t)(1LL), sizeof(CatchClause*)));
        /* pass */
        (*c_ptr = c);
        /* pass */
        List_ptr_append(catches, c_ptr);
        _tr_str_release(err_name);
    }
    /* pass */
    Block* finally_b = Block_init();
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFinally().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t62 = Parser_peek(self);
        if (_t62.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t62;
            /* pass */
            /* pass */
        }
        /* pass */
        finally_b = Parser_parse_block(self);
    }
    /* pass */
    return box_stmt(Stmt_ctor_STry(try_body, catches, finally_b));
}

__attribute__((hot)) Stmt* Parser_parse_assert_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Expr* msg = (Expr*)(0LL);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        msg = Parser_parse_expr(self);
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SAssert(cond, msg));
}

__attribute__((hot)) Stmt* Parser_parse_with_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    List_ptr* items = (void*)List_ptr_new();
    /* pass */
    List_TrStr* aliases = (void*)List_TrStr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Expr* item_e = Parser_parse_expr(self);
        /* pass */
        TrStr alias = _tr_str_lit("");
        /* pass */
        if ((((unsigned long long)(item_e)) != ((unsigned long long)(0LL)))) {
            /* pass */
            __auto_type _t63 = (*item_e);
            if (_t63.tag == Expr_ECast) {
                __auto_type cast_inner = _t63.data.ECast.expr;
__auto_type cast_ty_ptr = _t63.data.ECast.ty;
                /* pass */
                if ((((unsigned long long)(cast_ty_ptr)) != ((unsigned long long)(0LL)))) {
                    /* pass */
                    TrStr _strtmp_t64 = _tr_str_retain((*cast_ty_ptr)->name);
                    _tr_str_release(alias);
                    alias = _strtmp_t64;
                }
                /* pass */
                item_e = cast_inner;
            } else if (1) {
                __auto_type _ = _t63;
                /* pass */
            }
        }
        /* pass */
        List_ptr_append(items, item_e);
        /* pass */
        List_TrStr_append(aliases, alias);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else {
            /* pass */
            going = false;
        }
        _tr_str_release(alias);
    }
    /* pass */
    __auto_type _t65 = Parser_peek(self);
    if (_t65.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t65;
        /* pass */
        /* pass */
    }
    /* pass */
    Block* body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SWith(items, aliases, body));
}

__attribute__((hot)) Stmt* Parser_parse_asm_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t66 = Parser_peek(self);
    if (_t66.tag == Token_LParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t66;
        /* pass */
        /* pass */
    }
    /* pass */
    List_TrStr* parts = (void*)List_TrStr_new();
    /* pass */
    bool reading = true;
    /* pass */
    while (reading) {
        /* pass */
        __auto_type _t67 = Parser_peek(self);
        if (_t67.tag == Token_StrLit) {
            __auto_type s = _t67.data.StrLit.val;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            List_TrStr_append(parts, s);
            /* pass */
            __auto_type _t68 = Parser_peek(self);
            if (_t68.tag == Token_Comma) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t68;
                reading = false;
            }
        } else if (1) {
            __auto_type _ = _t67;
            reading = false;
        }
    }
    /* pass */
    __auto_type _t69 = Parser_peek(self);
    if (_t69.tag == Token_RParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t69;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    TrStr code = _tr_str_lit("");
    /* pass */
    TrStr outs = _tr_str_lit("");
    /* pass */
    TrStr ins = _tr_str_lit("");
    /* pass */
    TrStr clob = _tr_str_lit("");
    /* pass */
    if ((parts->len > 0LL)) {
        /* pass */
        TrStr _strtmp_t70 = List_TrStr_get(parts, 0LL);
        _tr_str_release(code);
        code = _strtmp_t70;
    }
    /* pass */
    if ((parts->len > 1LL)) {
        /* pass */
        TrStr _strtmp_t71 = List_TrStr_get(parts, 1LL);
        _tr_str_release(outs);
        outs = _strtmp_t71;
    }
    /* pass */
    if ((parts->len > 2LL)) {
        /* pass */
        TrStr _strtmp_t72 = List_TrStr_get(parts, 2LL);
        _tr_str_release(ins);
        ins = _strtmp_t72;
    }
    /* pass */
    if ((parts->len > 3LL)) {
        /* pass */
        TrStr _strtmp_t73 = List_TrStr_get(parts, 3LL);
        _tr_str_release(clob);
        clob = _strtmp_t73;
    }
    /* pass */
    List_TrStr_free(parts);
    return box_stmt(Stmt_ctor_SAsm(code, outs, ins, clob));
}

__attribute__((hot)) Stmt* Parser_parse_spawn_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type e = Parser_parse_expr(self);
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SSpawn(e));
}

__attribute__((hot)) Stmt* Parser_parse_taskgroup_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t74 = Parser_peek(self);
    if (_t74.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t74;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_STaskGroup(body));
}

__attribute__((hot)) Stmt* Parser_parse_chan_select_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Indent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    List_ptr* cases = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        __auto_type _t75 = Parser_peek(self);
        if (_t75.tag == Token_KwCase) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            long long arm_kind = (-1LL);
            /* pass */
            TrStr var_nm = _tr_str_lit("");
            /* pass */
            Expr* chan_e = (Expr*)(0LL);
            /* pass */
            Expr* val_e = (Expr*)(0LL);
            /* pass */
            Expr* timeout_e = (Expr*)(0LL);
            /* pass */
            __auto_type _t76 = Parser_peek(self);
            if (_t76.tag == Token_Ident) {
                __auto_type arm_nm = _t76.data.Ident.name;
                /* pass */
                if ((strcmp(_tr_strz(arm_nm), _tr_strz(_tr_str_lit("default"))) == 0)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    arm_kind = 3LL;
                } else if ((strcmp(_tr_strz(arm_nm), _tr_strz(_tr_str_lit("timeout"))) == 0)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    timeout_e = Parser_parse_expr(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    /* pass */
                    arm_kind = 2LL;
                } else {
                    /* pass */
                    long long saved = self->pos;
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    __auto_type _t77 = Parser_peek(self);
                    if (_t77.tag == Token_Eq) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                        /* pass */
                        chan_e = Parser_parse_expr(self);
                        /* pass */
                        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                            /* pass */
                            self->pos = (self->pos + 1LL);
                        }
                        /* pass */
                        TrStr _strtmp_t78 = _tr_str_retain(arm_nm);
                        _tr_str_release(var_nm);
                        var_nm = _strtmp_t78;
                        /* pass */
                        arm_kind = 0LL;
                    } else if (1) {
                        __auto_type _ = _t77;
                        /* pass */
                        self->pos = saved;
                        /* pass */
                        chan_e = Parser_parse_expr(self);
                        /* pass */
                        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                            /* pass */
                            self->pos = (self->pos + 1LL);
                        }
                        /* pass */
                        arm_kind = 1LL;
                    }
                }
            } else if (1) {
                __auto_type _ = _t76;
                /* pass */
                chan_e = Parser_parse_expr(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                arm_kind = 1LL;
            }
            /* pass */
            Block* arm_body = Parser_parse_block(self);
            /* pass */
            /* unsafe block */
            /* pass */
            ChanSelectArm** arm_p = ((ChanSelectArm**)_tr_c_calloc((size_t)(1LL), sizeof(ChanSelectArm*)));
            /* pass */
            if ((arm_kind == 0LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_recv(chan_e, var_nm, arm_body));
            } else if ((arm_kind == 1LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_send(chan_e, (Expr*)(0LL), arm_body));
            } else if ((arm_kind == 2LL)) {
                /* pass */
                (*arm_p = ChanSelectArm_init_timeout(timeout_e, arm_body));
            } else {
                /* pass */
                (*arm_p = ChanSelectArm_init_default(arm_body));
            }
            /* pass */
            List_ptr_append(cases, arm_p);
            _tr_str_release(var_nm);
        } else if ((_t75.tag == Token_Dedent || _t75.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t75.tag == Token_Newline) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t75;
            /* pass */
            going = false;
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_stmt(Stmt_ctor_SChanSelect(cases));
}

__attribute__((hot)) Stmt* Parser_parse_gpu_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type _t79 = Parser_peek(self);
    if (_t79.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t79;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SGpuBlock(body));
}

__attribute__((hot)) Stmt* Parser_parse_let_stmt(Parser* self, bool is_mut) {
    /* pass */
    if (is_mut) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        List_TrStr* names = (void*)List_TrStr_new();
        /* pass */
        List_TrStr_append(names, name);
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t80 = (Parser_consume_ident(self)); List_TrStr_append(names, _at_t80); _tr_str_release(_at_t80); });
        }
        /* pass */
        __auto_type _t81 = Parser_peek(self);
        if (_t81.tag == Token_Eq) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t81;
            /* pass */
            /* pass */
        }
        /* pass */
        Expr* val_ptr = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SMultiLet(names, is_mut, val_ptr));
    }
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t82 = Parser_peek(self);
    if (_t82.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t82;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t83 = Parser_peek(self);
    if (_t83.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t83;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), is_mut, false, false, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_shared_let_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t84 = Parser_peek(self);
    if (_t84.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t84;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t85 = Parser_peek(self);
    if (_t85.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t85;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Shared(), true, false, true, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_const_let_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type name = Parser_consume_ident(self);
    /* pass */
    AstType** ty_ptr = (AstType**)(0LL);
    /* pass */
    __auto_type _t86 = Parser_peek(self);
    if (_t86.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type ty = Parser_parse_type(self);
        /* pass */
        ty_ptr = box_asttype(ty);
    } else if (1) {
        __auto_type _ = _t86;
        /* pass */
        /* pass */
    }
    /* pass */
    Expr* val_ptr = (Expr*)(0LL);
    /* pass */
    __auto_type _t87 = Parser_peek(self);
    if (_t87.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        val_ptr = Parser_parse_expr(self);
    } else if (1) {
        __auto_type _ = _t87;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, true, false, ty_ptr, val_ptr));
}

__attribute__((hot)) Stmt* Parser_parse_if_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t88 = Parser_peek(self);
    if (_t88.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t88;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type then_b = Parser_parse_block(self);
    /* pass */
    List_ptr* elifs = (void*)List_ptr_new();
    /* pass */
    Block* else_b = Block_init();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        going = false;
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        __auto_type _t89 = Parser_peek(self);
        if (_t89.tag == Token_KwElif) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type ec = Parser_parse_expr(self);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t90 = Parser_peek(self);
            if (_t90.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t90;
                /* pass */
                /* pass */
            }
            /* pass */
            __auto_type eb = Parser_parse_block(self);
            /* pass */
            /* unsafe block */
            /* pass */
            Block** eb_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
            /* pass */
            (*eb_ptr = eb);
            /* pass */
            List_ptr_append(elifs, ElifClause_init(ec, eb_ptr));
            /* pass */
            going = true;
        } else if (_t89.tag == Token_KwElse) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            __auto_type _t91 = Parser_peek(self);
            if (_t91.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t91;
                /* pass */
                /* pass */
            }
            /* pass */
            else_b = Parser_parse_block(self);
        } else if (1) {
            __auto_type _ = _t89;
            /* pass */
            /* pass */
        }
    }
    /* pass */
    return box_stmt(Stmt_ctor_SIf(cond, then_b, elifs, else_b));
}

__attribute__((hot)) Stmt* Parser_parse_while_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type cond = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t92 = Parser_peek(self);
    if (_t92.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t92;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SWhile(cond, body, (void*)List_ptr_new()));
}

__attribute__((hot)) Stmt* Parser_parse_loop_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Block* body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SWhile(box_expr(Expr_ctor_ELitBool(true)), body, (void*)List_ptr_new()));
}

__attribute__((hot)) Stmt* Parser_parse_for_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    bool for_is_ref = false;
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwMut().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr _mr = Parser_consume_ident(self);
        /* pass */
        for_is_ref = true;
    }
    /* pass */
    TrStr var = Parser_consume_ident(self);
    /* pass */
    if (((!for_is_ref) && (strcmp(_tr_strz(var), _tr_strz(_tr_str_lit("ref"))) == 0))) {
        /* pass */
        __auto_type _t93 = Parser_peek(self);
        if (_t93.tag == Token_KwIn) {
            /* pass */
        } else if (_t93.tag == Token_Comma) {
            /* pass */
        } else if (1) {
            __auto_type _ = _t93;
            /* pass */
            for_is_ref = true;
            /* pass */
            TrStr _strtmp_t94 = Parser_consume_ident(self);
            _tr_str_release(var);
            var = _strtmp_t94;
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
        /* pass */
        List_TrStr* unpack_vars = (void*)List_TrStr_new();
        /* pass */
        List_TrStr_append(unpack_vars, var);
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t95 = (Parser_consume_ident(self)); List_TrStr_append(unpack_vars, _at_t95); _tr_str_release(_at_t95); });
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwIn().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Expr* fu_iter = Parser_parse_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* fu_body = Parser_parse_block(self);
        /* pass */
        _tr_str_release(var);
        return box_stmt(Stmt_ctor_SForUnpack(unpack_vars, fu_iter, fu_body));
    }
    /* pass */
    __auto_type _t96 = Parser_peek(self);
    if (_t96.tag == Token_KwIn) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t96;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type iter_e = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t97 = Parser_peek(self);
    if (_t97.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t97;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type body = Parser_parse_block(self);
    /* pass */
    return box_stmt(Stmt_ctor_SFor(var, iter_e, body, (void*)List_ptr_new(), for_is_ref));
}

__attribute__((hot)) Stmt* Parser_parse_match_stmt(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    __auto_type subj = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t98 = Parser_peek(self);
    if (_t98.tag == Token_Colon) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t98;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* arms = Parser_parse_match_arms(self);
    /* pass */
    return box_stmt(Stmt_ctor_SMatch(subj, arms));
}

__attribute__((hot)) List_ptr* Parser_parse_match_arms(Parser* self) {
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t99 = Parser_peek(self);
    if (_t99.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t99;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* arms = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        __auto_type _t100 = Parser_peek(self);
        if ((_t100.tag == Token_Dedent || _t100.tag == Token_Eof || _t100.tag == Token_RParen || _t100.tag == Token_RBracket || _t100.tag == Token_RBrace)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t100;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            Pattern pat = Parser_parse_pattern(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
                /* pass */
                List_Pattern* or_pats = (void*)List_Pattern_new();
                /* pass */
                List_Pattern_append(or_pats, pat);
                /* pass */
                while ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_Pattern_append(or_pats, Parser_parse_pattern(self));
                }
                /* pass */
                pat = Pattern_ctor_POr(or_pats);
            }
            /* pass */
            Expr* guard_expr = (Expr*)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                guard_expr = Parser_parse_expr(self);
            }
            /* pass */
            __auto_type _t101 = Parser_peek(self);
            if (_t101.tag == Token_Colon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t101;
                /* pass */
                /* pass */
            }
            /* pass */
            Block* arm_block = Block_init();
            /* pass */
            bool is_block_arm = false;
            /* pass */
            __auto_type _t102 = Parser_peek(self);
            if ((_t102.tag == Token_Newline || _t102.tag == Token_Indent)) {
                /* pass */
                arm_block = Parser_parse_block(self);
                /* pass */
                is_block_arm = true;
            } else if (1) {
                __auto_type _ = _t102;
                /* pass */
                /* pass */
            }
            /* pass */
            if ((!is_block_arm)) {
                /* pass */
                Block_push(arm_block, Parser_parse_stmt(self));
            }
            /* pass */
            Block** ab_ptr = (Block**)(0LL);
            /* pass */
            /* unsafe block */
            /* pass */
            ab_ptr = ((Block**)_tr_c_calloc((size_t)(1LL), sizeof(Block*)));
            /* pass */
            (*ab_ptr = arm_block);
            /* pass */
            MatchArm* new_arm = MatchArm_init(pat, ab_ptr);
            /* pass */
            new_arm->guard = guard_expr;
            /* pass */
            List_ptr_append(arms, _tr_obj_retain(new_arm));
        }
    }
    /* pass */
    __auto_type _t103 = Parser_peek(self);
    if (_t103.tag == Token_Dedent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t103;
        /* pass */
        /* pass */
    }
    /* pass */
    return arms;
}

__attribute__((hot)) Pattern Parser_parse_pattern(Parser* self) {
    /* pass */
    __auto_type _t104 = Parser_peek(self);
    if (_t104.tag == Token_KwCase) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t104;
        /* pass */
        /* pass */
    }
    /* pass */
    __auto_type _t105 = Parser_peek(self);
    if (_t105.tag == Token_Ident) {
        __auto_type type_name = _t105.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t106 = Parser_peek(self);
        if (_t106.tag == Token_Dot) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type variant = Parser_consume_ident(self);
            /* pass */
            __auto_type _t107 = Parser_peek(self);
            if (_t107.tag == Token_LParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                List_TrStr* fields = (void*)List_TrStr_new();
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    TrStr fname = Parser_consume_ident(self);
                    /* pass */
                    if ((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        List_TrStr_append(fields, fname);
                    }
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                    _tr_str_release(fname);
                }
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                if ((fields->len == 1LL)) {
                    /* pass */
                    return ({ TrStr _at_t108 = (List_TrStr_get(fields, 0LL)); __auto_type _wr = (Pattern_ctor_PVariantBind(type_name, variant, _at_t108)); _tr_str_release(_at_t108); _wr; });
                }
                /* pass */
                return Pattern_ctor_PVariantBindMany(type_name, variant, fields);
            } else if (1) {
                __auto_type _ = _t107;
                /* pass */
                /* pass */
            }
            /* pass */
            return Pattern_ctor_PVariant(type_name, variant);
        } else if (1) {
            __auto_type _ = _t106;
            /* pass */
            /* pass */
        }
        /* pass */
        return Pattern_ctor_PBind(type_name);
    } else if (_t105.tag == Token_IntLit) {
        __auto_type v = _t105.data.IntLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitInt(v);
    } else if (_t105.tag == Token_KwTrue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitBool(true);
    } else if (_t105.tag == Token_KwFalse) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitBool(false);
    } else if (_t105.tag == Token_StrLit) {
        __auto_type s = _t105.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Pattern_ctor_PLitStr(s);
    } else if (1) {
        __auto_type _ = _t105;
        /* pass */
        /* pass */
    }
    /* pass */
    return Pattern_make_PWild();
}

__attribute__((hot)) Stmt* Parser_parse_assign_or_expr_stmt(Parser* self) {
    /* pass */
    __auto_type lhs = Parser_parse_expr(self);
    /* pass */
    __auto_type _t109 = Parser_peek(self);
    if (_t109.tag == Token_Colon) {
        /* pass */
        __auto_type _t110 = (*lhs);
        if (_t110.tag == Expr_EIdent) {
            __auto_type name = _t110.data.EIdent.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            AstType* ty = Parser_parse_type(self);
            /* pass */
            Expr* val = (Expr*)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                val = Parser_parse_expr(self);
            }
            /* pass */
            Parser_expect_newline(self);
            /* pass */
            return box_stmt(Stmt_ctor_SLet(name, Ownership_make_Own(), false, false, false, box_asttype(ty), val));
        } else if (1) {
            __auto_type _ = _t110;
            /* pass */
        }
    } else if (_t109.tag == Token_Eq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, rhs));
    } else if (_t109.tag == Token_PlusEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("+"), lhs, rhs))));
    } else if (_t109.tag == Token_MinusEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("-"), lhs, rhs))));
    } else if (_t109.tag == Token_StarEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("*"), lhs, rhs))));
    } else if (_t109.tag == Token_SlashEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("/"), lhs, rhs))));
    } else if (_t109.tag == Token_PercentEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("%"), lhs, rhs))));
    } else if (_t109.tag == Token_FloorDivEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("//"), lhs, rhs))));
    } else if (_t109.tag == Token_StarStarEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("**"), lhs, rhs))));
    } else if (_t109.tag == Token_AmpEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("&"), lhs, rhs))));
    } else if (_t109.tag == Token_PipeEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("|"), lhs, rhs))));
    } else if (_t109.tag == Token_CaretEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("^"), lhs, rhs))));
    } else if (_t109.tag == Token_LtLtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit("<<"), lhs, rhs))));
    } else if (_t109.tag == Token_GtGtEq) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* rhs = Parser_parse_expr(self);
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        return box_stmt(Stmt_ctor_SAssign(lhs, box_expr(Expr_ctor_EBinOp(_tr_str_lit(">>"), lhs, rhs))));
    } else if (1) {
        __auto_type _ = _t109;
        /* pass */
        /* pass */
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_stmt(Stmt_ctor_SExpr(lhs));
}

__attribute__((hot)) Expr* Parser_parse_expr(Parser* self) {
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwDo().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* do_body = Parser_parse_block(self);
        /* pass */
        return box_expr(Expr_ctor_EDo(do_body));
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
        /* pass */
        return Parser_parse_if_expr(self);
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwMatch().tag)) {
        /* pass */
        return Parser_parse_match_expr(self);
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwLoop().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* loop_body = Parser_parse_block(self);
        /* pass */
        return box_expr(Expr_ctor_ELoop(loop_body));
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwWhile().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* we_cond = Parser_parse_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* we_body = Parser_parse_block(self);
        /* pass */
        Block* we_else = Block_init();
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwElse().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            /* pass */
            we_else = Parser_parse_block(self);
        }
        /* pass */
        return box_expr(Expr_ctor_EWhileExpr(we_cond, we_body, we_else));
    }
    /* pass */
    return Parser_parse_ternary(self);
}

__attribute__((hot)) Expr* Parser_parse_match_expr(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    Expr* subj = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    List_ptr* arms = Parser_parse_match_arms(self);
    /* pass */
    return box_expr(Expr_ctor_EMatch(subj, arms));
}

__attribute__((hot)) Expr* Parser_parse_if_expr(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    Expr* cond = Parser_parse_expr(self);
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Block* then_b = Parser_parse_block(self);
    /* pass */
    Expr* then_e = box_expr(Expr_ctor_EDo(then_b));
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    Expr* else_e = box_expr(Expr_ctor_EDo(Block_init()));
    /* pass */
    __auto_type _t111 = Parser_peek(self);
    if (_t111.tag == Token_KwElif) {
        /* pass */
        else_e = Parser_parse_if_expr(self);
    } else if (_t111.tag == Token_KwElse) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Block* else_b = Parser_parse_block(self);
        /* pass */
        else_e = box_expr(Expr_ctor_EDo(else_b));
    } else if (1) {
        __auto_type _ = _t111;
        /* pass */
    }
    /* pass */
    return box_expr(Expr_ctor_EIfElse(cond, then_e, else_e));
}

__attribute__((hot)) Expr* Parser_parse_ternary(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_or_expr(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Newline().tag)) {
        /* pass */
        return left;
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* cond = Parser_parse_or_expr(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwElse().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        Expr* else_e = Parser_parse_ternary(self);
        /* pass */
        return box_expr(Expr_ctor_EIfElse(cond, left, else_e));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_or_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_and_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwOr().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type right = Parser_parse_and_expr(self);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("or"), left, right));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_and_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_not_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_KwAnd().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type right = Parser_parse_not_expr(self);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("and"), left, right));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_not_expr(Parser* self) {
    /* pass */
    __auto_type _t112 = Parser_peek(self);
    if (_t112.tag == Token_KwNot) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("not"), Parser_parse_not_expr(self)));
    } else if (1) {
        __auto_type _ = _t112;
        /* pass */
        /* pass */
    }
    /* pass */
    return Parser_parse_comparison(self);
}

__attribute__((hot)) Expr* Parser_parse_comparison(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitor_expr(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwNot().tag)) {
        /* pass */
        long long saved_pos = self->pos;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwIn().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("not"), box_expr(Expr_ctor_EBinOp(_tr_str_lit("in"), left, Parser_parse_bitor_expr(self)))));
        }
        /* pass */
        self->pos = saved_pos;
    }
    /* pass */
    List_ptr* operands = (void*)List_ptr_new();
    /* pass */
    List_ptr_append(operands, left);
    /* pass */
    List_TrStr* ops = (void*)List_TrStr_new();
    /* pass */
    while (true) {
        /* pass */
        TrStr op = Parser__peek_cmp_op(self);
        /* pass */
        if ((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            break;
        }
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        List_TrStr_append(ops, op);
        /* pass */
        List_ptr_append(operands, Parser_parse_bitor_expr(self));
        /* pass */
        if (((strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("is"))) == 0) || (strcmp(_tr_strz(op), _tr_strz(_tr_str_lit("in"))) == 0))) {
            /* pass */
            break;
        }
        _tr_str_release(op);
    }
    /* pass */
    if ((ops->len == 0LL)) {
        /* pass */
        List_TrStr_free(ops);
        return left;
    }
    /* pass */
    Expr* result = ({ TrStr _at_t113 = (List_TrStr_get(ops, 0LL)); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_at_t113, ((Expr*)List_ptr_get(operands, 0LL)), ((Expr*)List_ptr_get(operands, 1LL))))); _tr_str_release(_at_t113); _wr; });
    /* pass */
    long long ci = 1LL;
    /* pass */
    while ((ci < ops->len)) {
        /* pass */
        Expr* cmp = ({ TrStr _at_t114 = (List_TrStr_get(ops, ci)); __auto_type _wr = (box_expr(Expr_ctor_EBinOp(_at_t114, ((Expr*)List_ptr_get(operands, ci)), ((Expr*)List_ptr_get(operands, (ci + 1LL)))))); _tr_str_release(_at_t114); _wr; });
        /* pass */
        result = box_expr(Expr_ctor_EBinOp(_tr_str_lit("and"), result, cmp));
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    List_TrStr_free(ops);
    return result;
}

__attribute__((hot)) TrStr Parser__peek_cmp_op(Parser* self) {
    /* pass */
    __auto_type _t115 = Parser_peek(self);
    if (_t115.tag == Token_EqEq) {
        return _tr_str_lit("==");
    } else if (_t115.tag == Token_NotEq) {
        return _tr_str_lit("!=");
    } else if (_t115.tag == Token_Lt) {
        return _tr_str_lit("<");
    } else if (_t115.tag == Token_Gt) {
        return _tr_str_lit(">");
    } else if (_t115.tag == Token_LtEq) {
        return _tr_str_lit("<=");
    } else if (_t115.tag == Token_GtEq) {
        return _tr_str_lit(">=");
    } else if (_t115.tag == Token_KwIs) {
        return _tr_str_lit("is");
    } else if (_t115.tag == Token_KwIn) {
        return _tr_str_lit("in");
    } else if (1) {
        __auto_type _ = _t115;
        return _tr_str_lit("");
    }
}

__attribute__((hot)) Expr* Parser_parse_bitor_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitxor_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Pipe().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("|"), left, Parser_parse_bitxor_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitxor_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_bitand_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Caret().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("^"), left, Parser_parse_bitand_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_bitand_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_shift_expr(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Amp().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("&"), left, Parser_parse_shift_expr(self)));
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_shift_expr(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_additive(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t116 = Parser_peek(self);
        if (_t116.tag == Token_LtLt) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("<<"), left, Parser_parse_additive(self)));
        } else if (_t116.tag == Token_GtGt) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit(">>"), left, Parser_parse_additive(self)));
        } else if (1) {
            __auto_type _ = _t116;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_additive(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_multiplicative(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t117 = Parser_peek(self);
        if (_t117.tag == Token_Plus) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines_and_indent(self);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("+"), left, Parser_parse_multiplicative(self)));
        } else if (_t117.tag == Token_Minus) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines_and_indent(self);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("-"), left, Parser_parse_multiplicative(self)));
        } else if (1) {
            __auto_type _ = _t117;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_multiplicative(Parser* self) {
    /* pass */
    Expr* left = Parser_parse_power(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t118 = Parser_peek(self);
        if (_t118.tag == Token_Star) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("*"), left, Parser_parse_power(self)));
        } else if (_t118.tag == Token_Slash) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("/"), left, Parser_parse_power(self)));
        } else if (_t118.tag == Token_Percent) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("%"), left, Parser_parse_power(self)));
        } else if (_t118.tag == Token_FloorDiv) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            left = box_expr(Expr_ctor_EBinOp(_tr_str_lit("//"), left, Parser_parse_power(self)));
        } else if (1) {
            __auto_type _ = _t118;
            break;
        }
    }
    /* pass */
    return left;
}

__attribute__((hot)) Expr* Parser_parse_power(Parser* self) {
    /* pass */
    Expr* base = Parser_parse_unary(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_StarStar().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Expr* exp = Parser_parse_power(self);
        /* pass */
        return box_expr(Expr_ctor_EBinOp(_tr_str_lit("**"), base, exp));
    }
    /* pass */
    return base;
}

__attribute__((hot)) Expr* Parser_parse_unary(Parser* self) {
    /* pass */
    __auto_type _t119 = Parser_peek(self);
    if (_t119.tag == Token_Minus) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("-"), Parser_parse_unary(self)));
    } else if (_t119.tag == Token_Tilde) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("~"), Parser_parse_unary(self)));
    } else if (_t119.tag == Token_Amp) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("&"), Parser_parse_unary(self)));
    } else if (_t119.tag == Token_Star) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EUnaryOp(_tr_str_lit("*"), Parser_parse_unary(self)));
    } else if (1) {
        __auto_type _ = _t119;
        /* pass */
        /* pass */
    }
    /* pass */
    return Parser_parse_postfix(self);
}

__attribute__((hot)) Expr* Parser_parse_postfix(Parser* self) {
    /* pass */
    Expr* e = Parser_parse_primary(self);
    /* pass */
    while (true) {
        /* pass */
        __auto_type _t120 = Parser_peek(self);
        if (_t120.tag == Token_Dot) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr attr = Parser_consume_ident(self);
            /* pass */
            __auto_type _t121 = Parser_peek(self);
            if (_t121.tag == Token_LParen) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                e = box_expr(Expr_ctor_EMethodCall(e, attr, Parser_parse_arg_list(self)));
            } else if (1) {
                __auto_type _ = _t121;
                /* pass */
                e = box_expr(Expr_ctor_EPropAccess(e, attr));
            }
            _tr_str_release(attr);
        } else if (_t120.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type _t122 = (*e);
            if (_t122.tag == Expr_EIdent) {
                __auto_type _pf_nm = _t122.data.EIdent.name;
                /* pass */
                if (_tr_dict_contains(self->import_aliases, _tr_strz(_pf_nm))) {
                    /* pass */
                    e = ({ TrStr _at_t123 = (_tr_str_retain(_tr_str_unbox(_tr_dict_get(self->import_aliases, _tr_strz(_pf_nm))))); __auto_type _wr = (box_expr(Expr_ctor_EIdent(_at_t123))); _tr_str_release(_at_t123); _wr; });
                }
            } else if (1) {
                __auto_type _ = _t122;
                /* pass */
                /* pass */
            }
            /* pass */
            e = box_expr(Expr_ctor_ECall(e, Parser_parse_arg_list(self)));
        } else if (_t120.tag == Token_LBracket) {
            /* pass */
            long long ix_oln = Parser_cur_line(self);
            /* pass */
            long long ix_ocol = Parser_cur_col(self);
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Expr* idx = ((Expr*)(0LL));
            /* pass */
            if ((Parser_peek(self).tag == Token_make_KwDef().tag)) {
                /* pass */
                idx = box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self))));
            } else {
                /* pass */
                idx = Parser_parse_expr(self);
            }
            /* pass */
            List_ptr* _ti_extra = (void*)List_ptr_new();
            /* pass */
            while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines_and_indent(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_KwDef().tag)) {
                    /* pass */
                    List_ptr_append(_ti_extra, box_expr(Expr_ctor_ETypeArg(box_asttype(Parser_parse_type(self)))));
                } else {
                    /* pass */
                    List_ptr_append(_ti_extra, Parser_parse_expr(self));
                }
            }
            /* pass */
            Parser_expect_rbracket(self, ix_oln, ix_ocol, _tr_str_lit("this index"));
            /* pass */
            if ((_ti_extra->len > 0LL)) {
                /* pass */
                List_ptr* _ti_all = (void*)List_ptr_new();
                /* pass */
                List_ptr_append(_ti_all, idx);
                /* pass */
                long long _tj = 0LL;
                /* pass */
                while ((_tj < _ti_extra->len)) {
                    /* pass */
                    List_ptr_append(_ti_all, ((Expr*)List_ptr_get(_ti_extra, _tj)));
                    /* pass */
                    _tj = (_tj + 1LL);
                }
                /* pass */
                idx = box_expr(Expr_ctor_ETuple(_ti_all));
            }
            /* pass */
            e = box_expr(Expr_ctor_EIndex(e, idx));
        } else if (_t120.tag == Token_Question) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            e = box_expr(Expr_ctor_ETryExpr(e));
        } else if (_t120.tag == Token_KwAs) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            e = box_expr(Expr_ctor_ECast(e, box_asttype(Parser_parse_type(self))));
        } else if (1) {
            __auto_type _ = _t120;
            break;
        }
    }
    /* pass */
    return e;
}

__attribute__((hot)) void Parser_emit_diag_at(Parser* self, long long ln, long long col, TrStr msg, TrStr hint) {
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (c_red(_tr_str_lit("error"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (c_bold(msg)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; }))); printf("\n"); });
    /* pass */
    TrStr loc = _tr_str_lit("");
    /* pass */
    if ((_tr_strlen(_tr_strz(self->current_file)) > 0LL)) {
        /* pass */
        TrStr _strtmp_t124 = _tr_str_retain(self->current_file);
        _tr_str_release(loc);
        loc = _strtmp_t124;
    }
    /* pass */
    TrStr head = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(loc), _tr_strz(_tr_str_lit(":")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if ((col > 0LL)) {
        /* pass */
        TrStr _strtmp_t125 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(head), _tr_strz(_tr_str_lit(":")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(col)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(head);
        head = _strtmp_t125;
    }
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_cyan(_tr_str_lit("-->"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(head)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    if (((_tr_strlen(_tr_strz(self->src_text)) > 0LL) && (ln > 0LL))) {
        /* pass */
        TrStr srcline = _nth_source_line(self->src_text, ln);
        /* pass */
        if ((_tr_strlen(_tr_strz(srcline)) > 0LL)) {
            /* pass */
            TrStr gnum = ({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ln)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("   ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" | "))); _tr_str_release(_cl); _cres; });
            /* pass */
            TrStr gbar = ({ TrStr _cl = (_spaces((_tr_strlen(_tr_strz(gnum)) - 2LL))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("| "))); _tr_str_release(_cl); _cres; });
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (c_dim(gnum)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(srcline)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
            /* pass */
            if ((col > 0LL)) {
                /* pass */
                ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (c_dim(gbar)); TrStr _cr = (_spaces((col - 1LL))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cr = (c_red(_tr_str_lit("^"))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; }))); printf("\n"); });
            }
            _tr_str_release(gnum);
        }
    }
    /* pass */
    if ((_tr_strlen(_tr_strz(hint)) > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (c_green(_tr_str_lit("= help"))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("  ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(hint)); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    self->error_count = (self->error_count + 1LL);
    _tr_str_release(loc);
    _tr_str_release(head);
}

__attribute__((hot)) void Parser_expect_rparen(Parser* self, long long oln, long long ocol, TrStr what) {
    /* pass */
    __auto_type _t126 = Parser_peek(self);
    if (_t126.tag == Token_RParen) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t126;
        /* pass */
        ({ TrStr _at_t127 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unclosed '(' — ")), _tr_strz(what))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" is never closed"))); _tr_str_release(_cl); _cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t127, _tr_str_lit("add the matching ')'.")); _tr_str_release(_at_t127); });
    }
}

__attribute__((hot)) void Parser_expect_rbracket(Parser* self, long long oln, long long ocol, TrStr what) {
    /* pass */
    __auto_type _t128 = Parser_peek(self);
    if (_t128.tag == Token_RBracket) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t128;
        /* pass */
        ({ TrStr _at_t129 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unclosed '[' — ")), _tr_strz(what))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" is never closed"))); _tr_str_release(_cl); _cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t129, _tr_str_lit("add the matching ']'.")); _tr_str_release(_at_t129); });
    }
}

__attribute__((hot)) void Parser_expect_rbrace(Parser* self, long long oln, long long ocol, TrStr what) {
    /* pass */
    __auto_type _t130 = Parser_peek(self);
    if (_t130.tag == Token_RBrace) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t130;
        /* pass */
        ({ TrStr _at_t131 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unclosed '{' — ")), _tr_strz(what))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" is never closed"))); _tr_str_release(_cl); _cres; })); Parser_emit_diag_at(self, oln, ocol, _at_t131, _tr_str_lit("add the matching '}'.")); _tr_str_release(_at_t131); });
    }
}

__attribute__((hot)) List_ptr* Parser_parse_arg_list(Parser* self) {
    /* pass */
    long long oln = 0LL;
    /* pass */
    long long ocol = 0LL;
    /* pass */
    if ((self->pos > 0LL)) {
        /* pass */
        oln = List_i64_get(self->lines, (self->pos - 1LL));
        /* pass */
        ocol = List_i64_get(self->cols, (self->pos - 1LL));
    }
    /* pass */
    List_ptr* el = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines_and_indent(self);
        /* pass */
        __auto_type _t132 = Parser_peek(self);
        if ((_t132.tag == Token_RParen || _t132.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (1) {
            __auto_type _ = _t132;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            List_ptr_append(el, Parser_parse_expr(self));
            /* pass */
            __auto_type _t133 = Parser_peek(self);
            if (_t133.tag == Token_Comma) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines_and_indent(self);
            } else if (1) {
                __auto_type _ = _t133;
                /* pass */
                going = false;
            }
        }
    }
    /* pass */
    Parser_expect_rparen(self, oln, ocol, _tr_str_lit("this call's argument list"));
    /* pass */
    return el;
}

__attribute__((hot)) Expr* Parser_parse_primary(Parser* self) {
    /* pass */
    __auto_type _t134 = Parser_peek(self);
    if (_t134.tag == Token_IntLit) {
        __auto_type v = _t134.data.IntLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitInt(v));
    } else if (_t134.tag == Token_FloatLit) {
        __auto_type v = _t134.data.FloatLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitFloat(v));
    } else if (_t134.tag == Token_StrLit) {
        __auto_type s = _t134.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitStr(s));
    } else if (_t134.tag == Token_TripleStrLit) {
        __auto_type s = _t134.data.TripleStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitStr(s));
    } else if (_t134.tag == Token_RawStrLit) {
        __auto_type s = _t134.data.RawStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ERawStr(s));
    } else if (_t134.tag == Token_ByteStrLit) {
        __auto_type s = _t134.data.ByteStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBytes(s));
    } else if (_t134.tag == Token_FStrLit) {
        __auto_type s = _t134.data.FStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Parser_parse_fstring(self, s);
    } else if (_t134.tag == Token_CharLit) {
        __auto_type v = _t134.data.CharLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitChar(v));
    } else if (_t134.tag == Token_BoolLit) {
        __auto_type v = _t134.data.BoolLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(v));
    } else if (_t134.tag == Token_KwTrue) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(true));
    } else if (_t134.tag == Token_KwFalse) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_ELitBool(false));
    } else if (_t134.tag == Token_KwNone) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_make_ELitNone());
    } else if (_t134.tag == Token_KwInt) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("int")));
    } else if (_t134.tag == Token_KwFloat) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("float")));
    } else if (_t134.tag == Token_KwBool) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("bool")));
    } else if ((_t134.tag == Token_KwStr || _t134.tag == Token_KwString)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("str")));
    } else if (_t134.tag == Token_KwChar) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("char")));
    } else if (_t134.tag == Token_KwVoid) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("void")));
    } else if (_t134.tag == Token_KwSuper) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr super_base = _tr_str_lit("");
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr _strtmp_t135 = Parser_consume_ident(self);
            _tr_str_release(super_base);
            super_base = _strtmp_t135;
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                TrStr super_method = Parser_consume_ident(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args = Parser_parse_arg_list(self);
                    /* pass */
                    return box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method, super_args));
                } else {
                    /* pass */
                    return box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method));
                }
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr super_next = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Dot().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                TrStr _strtmp_t136 = _tr_str_retain(super_next);
                _tr_str_release(super_base);
                super_base = _strtmp_t136;
                /* pass */
                TrStr super_method2 = Parser_consume_ident(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args2 = Parser_parse_arg_list(self);
                    /* pass */
                    _tr_str_release(super_next);
                    return box_expr(Expr_ctor_ESuperMethodCall(super_base, super_method2, super_args2));
                } else {
                    /* pass */
                    _tr_str_release(super_next);
                    return box_expr(Expr_ctor_ESuperPropAccess(super_base, super_method2));
                }
            } else {
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr* super_args3 = Parser_parse_arg_list(self);
                    /* pass */
                    _tr_str_release(super_base);
                    return box_expr(Expr_ctor_ESuperMethodCall(_tr_str_lit(""), super_next, super_args3));
                } else {
                    /* pass */
                    _tr_str_release(super_base);
                    return box_expr(Expr_ctor_ESuperPropAccess(_tr_str_lit(""), super_next));
                }
            }
        }
        /* pass */
        _tr_str_release(super_base);
        return box_expr(Expr_ctor_EIdent(_tr_str_lit("super")));
    } else if (_t134.tag == Token_Ident) {
        __auto_type name = _t134.data.Ident.name;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Bang().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                List_ptr* mac_args = Parser_parse_arg_list(self);
                /* pass */
                return box_expr(Expr_ctor_EMacroCall(name, mac_args));
            }
        }
        /* pass */
        return box_expr(Expr_ctor_EIdent(name));
    } else if (_t134.tag == Token_KwSizeOf) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t137 = Parser_peek(self);
        if (_t137.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t137;
            /* pass */
            /* pass */
        }
        /* pass */
        AstType** ty = box_asttype(Parser_parse_type(self));
        /* pass */
        __auto_type _t138 = Parser_peek(self);
        if (_t138.tag == Token_RParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t138;
            /* pass */
            /* pass */
        }
        /* pass */
        return box_expr(Expr_ctor_ESizeOf(ty));
    } else if (_t134.tag == Token_LParen) {
        /* pass */
        long long p_oln = Parser_cur_line(self);
        /* pass */
        long long p_ocol = Parser_cur_col(self);
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_ETuple((void*)List_ptr_new()));
        }
        /* pass */
        Expr* e = Parser_parse_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            List_ptr* items = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(items, e);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RParen().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                List_ptr_append(items, Parser_parse_expr(self));
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    Parser_skip_newlines(self);
                } else {
                    /* pass */
                    break;
                }
            }
            /* pass */
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit("this tuple"));
            /* pass */
            return box_expr(Expr_ctor_ETuple(items));
        } else {
            /* pass */
            Parser_expect_rparen(self, p_oln, p_ocol, _tr_str_lit("this parenthesized expression"));
            /* pass */
            return e;
        }
    } else if (_t134.tag == Token_LBracket) {
        /* pass */
        long long lb_oln = Parser_cur_line(self);
        /* pass */
        long long lb_ocol = Parser_cur_col(self);
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_ctor_EList((void*)List_ptr_new()));
        }
        /* pass */
        Expr* first = Parser_parse_or_expr(self);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwFor().tag)) {
            /* pass */
            List_ptr* generators = (void*)List_ptr_new();
            /* pass */
            while ((Parser_peek(self).tag == Token_make_KwFor().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                TrStr target = Parser_consume_ident(self);
                /* pass */
                __auto_type _t139 = Parser_peek(self);
                if (_t139.tag == Token_KwIn) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if (1) {
                    __auto_type _ = _t139;
                    /* pass */
                    /* pass */
                }
                /* pass */
                Expr* iter = Parser_parse_or_expr(self);
                /* pass */
                List_ptr* ifs = (void*)List_ptr_new();
                /* pass */
                while ((Parser_peek(self).tag == Token_make_KwIf().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    List_ptr_append(ifs, Parser_parse_or_expr(self));
                }
                /* pass */
                Comprehension* c = Comprehension_init(target, iter);
                /* pass */
                c->ifs = ifs;
                /* pass */
                /* unsafe block */
                /* pass */
                Comprehension** c_ptr = ((Comprehension**)_tr_c_calloc((size_t)(1LL), sizeof(Comprehension*)));
                /* pass */
                (*c_ptr = c);
                /* pass */
                List_ptr_append(generators, c_ptr);
                _tr_str_release(target);
            }
            /* pass */
            Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit("this list comprehension"));
            /* pass */
            return box_expr(Expr_ctor_EListComp(first, generators));
        }
        /* pass */
        List_ptr* items = (void*)List_ptr_new();
        /* pass */
        List_ptr_append(items, first);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                /* pass */
                List_ptr_append(items, Parser_parse_expr(self));
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    Parser_skip_newlines(self);
                } else {
                    /* pass */
                    break;
                }
            }
        }
        /* pass */
        Parser_expect_rbracket(self, lb_oln, lb_ocol, _tr_str_lit("this list"));
        /* pass */
        return box_expr(Expr_ctor_EList(items));
    } else if (_t134.tag == Token_LBrace) {
        /* pass */
        long long br_oln = Parser_cur_line(self);
        /* pass */
        long long br_ocol = Parser_cur_col(self);
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* keys = (void*)List_ptr_new();
        /* pass */
        List_ptr* vals = (void*)List_ptr_new();
        /* pass */
        List_ptr* set_items = (void*)List_ptr_new();
        /* pass */
        bool is_set = false;
        /* pass */
        if ((Parser_peek(self).tag != Token_make_RBrace().tag)) {
            /* pass */
            Expr* first = Parser_parse_expr(self);
            /* pass */
            Parser_skip_newlines(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                Expr* value = Parser_parse_expr(self);
                /* pass */
                List_ptr_append(keys, first);
                /* pass */
                List_ptr_append(vals, value);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBrace().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    Expr* key = Parser_parse_expr(self);
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    __auto_type _t140 = Parser_peek(self);
                    if (_t140.tag == Token_Colon) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else if (1) {
                        __auto_type _ = _t140;
                        /* pass */
                        /* pass */
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    Expr* v = Parser_parse_expr(self);
                    /* pass */
                    List_ptr_append(keys, key);
                    /* pass */
                    List_ptr_append(vals, v);
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        break;
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                }
            } else {
                /* pass */
                is_set = true;
                /* pass */
                List_ptr_append(set_items, first);
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                Parser_skip_newlines(self);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBrace().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    List_ptr_append(set_items, Parser_parse_expr(self));
                    /* pass */
                    Parser_skip_newlines(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else {
                        /* pass */
                        break;
                    }
                    /* pass */
                    Parser_skip_newlines(self);
                }
            }
        }
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        Parser_expect_rbrace(self, br_oln, br_ocol, _tr_str_lit("this dict/set literal"));
        /* pass */
        if (is_set) {
            /* pass */
            return box_expr(Expr_ctor_ESet(set_items));
        } else {
            /* pass */
            return box_expr(Expr_ctor_EDict(keys, vals));
        }
    } else if (_t134.tag == Token_KwTry) {
        /* pass */
        Stmt* st = Parser_parse_try_stmt(self);
        /* pass */
        __auto_type _t141 = (*st);
        if (_t141.tag == Stmt_STry) {
            __auto_type try_body = _t141.data.STry.try_body;
__auto_type catches = _t141.data.STry.catches;
__auto_type finally_b = _t141.data.STry.finally_b;
            /* pass */
            return box_expr(Expr_ctor_ETry(try_body, catches, finally_b));
        } else if (1) {
            __auto_type _ = _t141;
            /* pass */
            /* pass */
        }
    } else if (_t134.tag == Token_KwAwait) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EAwait(Parser_parse_expr(self)));
    } else if (_t134.tag == Token_KwYield) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return box_expr(Expr_ctor_EYield(Parser_parse_expr(self)));
    } else if ((_t134.tag == Token_KwDef || _t134.tag == Token_KwAsync)) {
        /* pass */
        bool is_async = false;
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwAsync().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            is_async = true;
        }
        /* pass */
        __auto_type _t142 = Parser_peek(self);
        if (_t142.tag == Token_KwDef) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t142;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t143 = Parser_peek(self);
        if (_t143.tag == Token_LParen) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t143;
            /* pass */
            /* pass */
        }
        /* pass */
        List_ptr* params = Parser_parse_param_list(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        AstType** return_type = (AstType**)(0LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return_type = box_asttype(Parser_parse_type(self));
        }
        /* pass */
        __auto_type _t144 = Parser_peek(self);
        if (_t144.tag == Token_Colon) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t144;
            /* pass */
            /* pass */
        }
        /* pass */
        Block* body = Parser_parse_block(self);
        /* pass */
        return box_expr(Expr_ctor_EClosure(params, return_type, body, is_async));
    } else if (_t134.tag == Token_FStrLit) {
        __auto_type raw = _t134.data.FStrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        return Parser_parse_fstring(self, raw);
    } else if (1) {
        __auto_type _ = _t134;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((!Parser_at_end(self))) {
        /* pass */
        TrStr tok_desc = _tr_str_lit("token");
        /* pass */
        TrStr hint = _tr_str_lit("check for a typo, a missing ':' or unbalanced parentheses/brackets near this point.");
        /* pass */
        __auto_type _t145 = Parser_peek(self);
        if (_t145.tag == Token_Newline) {
            /* pass */
            TrStr _strtmp_t146 = _tr_str_lit("end of line");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t146;
            /* pass */
            TrStr _strtmp_t147 = _tr_str_lit("an expression was expected before the end of this line - check for a missing value or trailing operator.");
            _tr_str_release(hint);
            hint = _strtmp_t147;
        } else if (_t145.tag == Token_Indent) {
            /* pass */
            TrStr _strtmp_t148 = _tr_str_lit("indentation");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t148;
            /* pass */
            TrStr _strtmp_t149 = _tr_str_lit("check that this line's indentation matches the surrounding block.");
            _tr_str_release(hint);
            hint = _strtmp_t149;
        } else if (_t145.tag == Token_Dedent) {
            /* pass */
            TrStr _strtmp_t150 = _tr_str_lit("dedent");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t150;
            /* pass */
            TrStr _strtmp_t151 = _tr_str_lit("check that this block is properly indented and closed.");
            _tr_str_release(hint);
            hint = _strtmp_t151;
        } else if (_t145.tag == Token_Ident) {
            __auto_type n = _t145.data.Ident.name;
            /* pass */
            TrStr _strtmp_t152 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("identifier '")), _tr_strz(n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t152;
            /* pass */
            TrStr _strtmp_t153 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("an operator, ':' or end of statement was expected before '")), _tr_strz(n))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("'."))); _tr_str_release(_cl); _cres; });
            _tr_str_release(hint);
            hint = _strtmp_t153;
        } else if (_t145.tag == Token_KwMut) {
            TrStr _strtmp_t154 = _tr_str_lit("keyword 'mut'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t154;
        } else if (_t145.tag == Token_KwConst) {
            TrStr _strtmp_t155 = _tr_str_lit("keyword 'const'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t155;
        } else if (_t145.tag == Token_KwPub) {
            TrStr _strtmp_t156 = _tr_str_lit("keyword 'pub'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t156;
        } else if (_t145.tag == Token_KwReturn) {
            TrStr _strtmp_t157 = _tr_str_lit("keyword 'return'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t157;
        } else if (_t145.tag == Token_KwIf) {
            TrStr _strtmp_t158 = _tr_str_lit("keyword 'if'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t158;
        } else if (_t145.tag == Token_KwWhile) {
            TrStr _strtmp_t159 = _tr_str_lit("keyword 'while'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t159;
        } else if (_t145.tag == Token_Comma) {
            /* pass */
            TrStr _strtmp_t160 = _tr_str_lit("','");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t160;
            /* pass */
            TrStr _strtmp_t161 = _tr_str_lit("remove the extra ',' or add the missing item before it.");
            _tr_str_release(hint);
            hint = _strtmp_t161;
        } else if (_t145.tag == Token_Colon) {
            /* pass */
            TrStr _strtmp_t162 = _tr_str_lit("':'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t162;
            /* pass */
            TrStr _strtmp_t163 = _tr_str_lit("remove the extra ':' or check the statement before it is complete.");
            _tr_str_release(hint);
            hint = _strtmp_t163;
        } else if (_t145.tag == Token_RParen) {
            /* pass */
            TrStr _strtmp_t164 = _tr_str_lit("')'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t164;
            /* pass */
            TrStr _strtmp_t165 = _tr_str_lit("check for an extra ')' or a missing matching '('.");
            _tr_str_release(hint);
            hint = _strtmp_t165;
        } else if (_t145.tag == Token_RBracket) {
            /* pass */
            TrStr _strtmp_t166 = _tr_str_lit("']'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t166;
            /* pass */
            TrStr _strtmp_t167 = _tr_str_lit("check for an extra ']' or a missing matching '['.");
            _tr_str_release(hint);
            hint = _strtmp_t167;
        } else if (_t145.tag == Token_RBrace) {
            /* pass */
            TrStr _strtmp_t168 = _tr_str_lit("'}'");
            _tr_str_release(tok_desc);
            tok_desc = _strtmp_t168;
            /* pass */
            TrStr _strtmp_t169 = _tr_str_lit("check for an extra '}' or a missing matching '{'.");
            _tr_str_release(hint);
            hint = _strtmp_t169;
        } else if (_t145.tag == Token_Error) {
            __auto_type em = _t145.data.Error.msg;
            /* pass */
            Parser_emit_diag(self, em, _tr_str_lit(""));
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            return box_expr(Expr_make_ELitNone());
        } else if (1) {
            __auto_type _ = _t145;
            /* pass */
        }
        /* pass */
        ({ TrStr _at_t170 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("unexpected ")), _tr_strz(tok_desc))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" in expression"))); _tr_str_release(_cl); _cres; })); Parser_emit_diag(self, _at_t170, hint); _tr_str_release(_at_t170); });
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_expr(Expr_make_ELitNone());
}

__attribute__((hot)) Expr* Parser_parse_fstring(Parser* self, TrStr raw) {
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    char* p = ((char*)(_tr_strz(raw)));
    /* pass */
    long long i = 0LL;
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (true) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if ((c == 123LL)) {
            /* pass */
            if ((sb->buf->len > 0LL)) {
                /* pass */
                ({ TrStr _at_t171 = (StringObj_as_str(StringBuilder_to_string(sb))); List_ptr_append(fl, FStringPart_init_text(_at_t171)); _tr_str_release(_at_t171); });
                /* pass */
                sb = StringBuilder_init(64LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            StringBuilder* expr_sb = StringBuilder_init(64LL);
            /* pass */
            long long depth = 1LL;
            /* pass */
            while ((depth > 0LL)) {
                /* pass */
                long long ec = ((long long)((*(p + i))));
                /* pass */
                if ((ec == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                if ((ec == 123LL)) {
                    /* pass */
                    depth = (depth + 1LL);
                }
                /* pass */
                if ((ec == 125LL)) {
                    /* pass */
                    depth = (depth - 1LL);
                }
                /* pass */
                if ((depth > 0LL)) {
                    /* pass */
                    StringBuilder_append_char(expr_sb, ec);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            TrStr expr_str = StringObj_as_str(StringBuilder_to_string(expr_sb));
            /* pass */
            TrStr fmt_spec = _tr_str_lit("");
            /* pass */
            long long colon_pos = _find_fmt_colon(expr_str);
            /* pass */
            if ((colon_pos >= 0LL)) {
                /* pass */
                TrStr _fs = _tr_str_wrap(_tr_str_slice(_tr_strz(expr_str), (colon_pos + 1LL), _tr_strlen(_tr_strz(expr_str))));
                /* pass */
                TrStr _strtmp_t172 = _tr_str_wrap(_tr_str_strip(_tr_strz(_fs)));
                _tr_str_release(fmt_spec);
                fmt_spec = _strtmp_t172;
                /* pass */
                TrStr _es = _tr_str_wrap(_tr_str_slice(_tr_strz(expr_str), 0LL, colon_pos));
                /* pass */
                TrStr _strtmp_t173 = _tr_str_wrap(_tr_str_strip(_tr_strz(_es)));
                _tr_str_release(expr_str);
                expr_str = _strtmp_t173;
                _tr_str_release(_fs);
                _tr_str_release(_es);
            }
            /* pass */
            Lexer* lexer = Lexer_init(expr_str);
            /* pass */
            List_Token* _fstr_tokens = Lexer_tokenize(lexer);
            /* pass */
            if ((_fstr_tokens->len > 0LL)) {
                /* pass */
                Parser* parser = Parser_init(_fstr_tokens, lexer->token_lines);
                /* pass */
                Expr* e = Parser_parse_expr(parser);
                /* pass */
                if ((_tr_strlen(_tr_strz(fmt_spec)) > 0LL)) {
                    /* pass */
                    List_ptr_append(fl, FStringPart_init_expr_fmt(e, fmt_spec));
                } else {
                    /* pass */
                    List_ptr_append(fl, FStringPart_init_expr(e));
                }
                _tr_obj_release(parser, _trdrop_Parser);
            }
            _tr_str_release(expr_str);
            StringBuilder__tr_fn_free(expr_sb);
            _tr_obj_release(lexer, _trdrop_Lexer);
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    if ((sb->buf->len > 0LL)) {
        /* pass */
        ({ TrStr _at_t174 = (StringObj_as_str(StringBuilder_to_string(sb))); List_ptr_append(fl, FStringPart_init_text(_at_t174)); _tr_str_release(_at_t174); });
    }
    /* pass */
    StringBuilder__tr_fn_free(sb);
    return box_expr(Expr_ctor_EFString(fl));
}

__attribute__((hot)) Program* Parser_parse_program(Parser* self) {
    /* pass */
    Program* prog = Program_init();
    /* pass */
    Parser_skip_newlines_and_indent(self);
    /* pass */
    while ((!Parser_at_end(self))) {
        /* pass */
        bool sk = true;
        /* pass */
        while (sk) {
            /* pass */
            sk = false;
            /* pass */
            __auto_type _t175 = Parser_peek(self);
            if ((_t175.tag == Token_Newline || _t175.tag == Token_Dedent)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                sk = true;
            } else if (_t175.tag == Token_Indent) {
                /* pass */
                Parser_emit_diag(self, _tr_str_lit("unexpected indentation"), _tr_str_lit("this line is indented but no block was opened above it (a statement ending in ':' and its own line) — remove the extra indentation."));
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                sk = true;
            } else if (1) {
                __auto_type _ = _t175;
                /* pass */
                /* pass */
            }
        }
        /* pass */
        if ((!Parser_at_end(self))) {
            /* pass */
            Program_push(prog, Parser_parse_decl(self));
        }
    }
    /* pass */
    return prog;
}

__attribute__((hot)) Decl* Parser_parse_decl(Parser* self) {
    /* pass */
    List_ptr* decorators = (void*)List_ptr_new();
    /* pass */
    while ((Parser_peek(self).tag == Token_make_At().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type name = Parser_consume_ident(self);
        /* pass */
        Decorator* d = Decorator_init(name);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            d->args = Parser_parse_arg_list(self);
        }
        /* pass */
        List_ptr_append(decorators, _tr_obj_retain(d));
        /* pass */
        Parser_expect_newline(self);
        _tr_obj_release(d, _trdrop_Decorator);
    }
    /* pass */
    bool is_public = false;
    /* pass */
    bool is_export = false;
    /* pass */
    bool _scan_mods = true;
    /* pass */
    while (_scan_mods) {
        /* pass */
        _scan_mods = false;
        /* pass */
        __auto_type _t176 = Parser_peek(self);
        if (_t176.tag == Token_KwPub) {
            /* pass */
            is_public = true;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            _scan_mods = true;
        } else if (_t176.tag == Token_KwExport) {
            /* pass */
            is_export = true;
            /* pass */
            is_public = true;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            _scan_mods = true;
        } else if (1) {
            __auto_type _ = _t176;
            /* pass */
            /* pass */
        }
    }
    /* pass */
    __auto_type _t177 = Parser_peek(self);
    if (_t177.tag == Token_KwFrom) {
        /* pass */
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return Parser_parse_from_import(self);
    } else if (_t177.tag == Token_KwImport) {
        /* pass */
        List_ptr_free_obj(decorators, _trdrop_Decorator);
        return Parser_parse_import(self);
    } else if (_t177.tag == Token_Ident) {
        __auto_type type_kw = _t177.data.Ident.name;
        /* pass */
        if ((strcmp(_tr_strz(type_kw), _tr_strz(_tr_str_lit("type"))) == 0)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr alias_name = Parser_consume_ident(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Eq().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType* target_ty = Parser_parse_type(self);
                /* pass */
                Parser_expect_newline(self);
                /* pass */
                List_ptr_free_obj(decorators, _trdrop_Decorator);
                return box_decl(Decl_ctor_DTypeAlias(alias_name, box_asttype(target_ty)));
            }
        }
    } else if (_t177.tag == Token_KwDef) {
        /* pass */
        long long _fline = Parser_cur_line(self);
        /* pass */
        FunctionDef* f = Parser_parse_function_def(self, false);
        /* pass */
        f->line = _fline;
        /* pass */
        f->is_public = is_public;
        /* pass */
        f->is_export = is_export;
        /* pass */
        f->decorators = decorators;
        /* pass */
        return box_decl(Decl_ctor_DFunction(f));
    } else if (_t177.tag == Token_KwClass) {
        /* pass */
        long long _cline = Parser_cur_line(self);
        /* pass */
        Decl* c_ptr = Parser_parse_class_decl(self);
        /* pass */
        __auto_type _t178 = (*c_ptr);
        if (_t178.tag == Decl_DClass) {
            __auto_type c = _t178.data.DClass.cls;
            /* pass */
            c->line = _cline;
            /* pass */
            c->is_public = is_public;
            /* pass */
            bool _is_vt = false;
            /* pass */
            long long _dvi = 0LL;
            /* pass */
            while ((_dvi < decorators->len)) {
                /* pass */
                TrStr _dvn = _tr_str_retain(((Decorator*)List_ptr_get(decorators, _dvi))->name);
                /* pass */
                if (((((strcmp(_tr_strz(_dvn), _tr_strz(_tr_str_lit("value_type"))) == 0) || (strcmp(_tr_strz(_dvn), _tr_strz(_tr_str_lit("packed"))) == 0)) || (strcmp(_tr_strz(_dvn), _tr_strz(_tr_str_lit("aligned"))) == 0)) || (strcmp(_tr_strz(_dvn), _tr_strz(_tr_str_lit("union"))) == 0))) {
                    /* pass */
                    _is_vt = true;
                }
                /* pass */
                _dvi = (_dvi + 1LL);
                _tr_str_release(_dvn);
            }
            /* pass */
            c->is_class = (!_is_vt);
            /* pass */
            c->decorators = decorators;
            /* pass */
            /* unsafe block */
            /* pass */
            (*c_ptr = Decl_ctor_DClass(c));
        } else if (1) {
            __auto_type _ = _t178;
            /* pass */
            /* pass */
        }
        /* pass */
        return c_ptr;
    } else if (_t177.tag == Token_KwEnum) {
        /* pass */
        long long _eline = Parser_cur_line(self);
        /* pass */
        Decl* e_ptr = Parser_parse_enum_decl(self);
        /* pass */
        __auto_type _t179 = (*e_ptr);
        if (_t179.tag == Decl_DEnum) {
            __auto_type e = _t179.data.DEnum.enm;
            /* pass */
            e->line = _eline;
            /* pass */
            e->is_public = is_public;
            /* pass */
            e->decorators = decorators;
            /* pass */
            /* unsafe block */
            /* pass */
            (*e_ptr = Decl_ctor_DEnum(e));
        } else if (1) {
            __auto_type _ = _t179;
            /* pass */
            /* pass */
        }
        /* pass */
        return e_ptr;
    } else if (_t177.tag == Token_KwInterface) {
        /* pass */
        long long _iline = Parser_cur_line(self);
        /* pass */
        Decl* i_ptr = Parser_parse_interface_decl(self);
        /* pass */
        __auto_type _t180 = (*i_ptr);
        if (_t180.tag == Decl_DInterface) {
            __auto_type i = _t180.data.DInterface.iface;
            /* pass */
            i->line = _iline;
            /* pass */
            i->is_public = is_public;
            /* pass */
            i->decorators = decorators;
            /* pass */
            /* unsafe block */
            /* pass */
            (*i_ptr = Decl_ctor_DInterface(i));
        } else if (1) {
            __auto_type _ = _t180;
            /* pass */
            /* pass */
        }
        /* pass */
        return i_ptr;
    } else if (_t177.tag == Token_KwExtend) {
        /* pass */
        return Parser_parse_extend_decl(self);
    } else if (_t177.tag == Token_KwDecorator) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        long long _dl = Parser_cur_line(self);
        /* pass */
        FunctionDef* _df = Parser_parse_function_def(self, false);
        /* pass */
        _df->line = _dl;
        /* pass */
        _df->is_public = is_public;
        /* pass */
        return box_decl(Decl_ctor_DDecoratorDef(_df));
    } else if (_t177.tag == Token_KwMacro) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        long long _ml = Parser_cur_line(self);
        /* pass */
        FunctionDef* _mf = Parser_parse_function_def(self, false);
        /* pass */
        _mf->line = _ml;
        /* pass */
        _mf->is_public = is_public;
        /* pass */
        _mf->is_macro = true;
        /* pass */
        return box_decl(Decl_ctor_DDecoratorDef(_mf));
    } else if (_t177.tag == Token_KwExtern) {
        /* pass */
        return Parser_parse_extern_decl(self);
    } else if (_t177.tag == Token_KwAsync) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t181 = Parser_peek(self);
        if (_t181.tag == Token_KwDef) {
            /* pass */
            FunctionDef* f = Parser_parse_function_def(self, false);
            /* pass */
            f->is_async = true;
            /* pass */
            f->is_public = is_public;
            /* pass */
            f->is_export = is_export;
            /* pass */
            f->decorators = decorators;
            /* pass */
            return box_decl(Decl_ctor_DFunction(f));
        } else if (1) {
            __auto_type _ = _t181;
            /* pass */
            /* pass */
        }
    } else if (1) {
        __auto_type _ = _t177;
        /* pass */
        /* pass */
    }
    /* pass */
    return box_decl(Decl_ctor_DTopLevelStmt(Parser_parse_stmt(self)));
}

__attribute__((hot)) Decl* Parser_parse_from_import(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr path = Parser_consume_module_ident(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Dot().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr _strtmp_t182 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(path), _tr_strz(_tr_str_lit(".")))); TrStr _cr = (Parser_consume_module_ident(self)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(path);
        path = _strtmp_t182;
    }
    /* pass */
    __auto_type _t183 = Parser_peek(self);
    if (_t183.tag == Token_KwImport) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t183;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* il = (void*)List_ptr_new();
    /* pass */
    bool multi = false;
    /* pass */
    __auto_type _t184 = Parser_peek(self);
    if ((_t184.tag == Token_LParen || _t184.tag == Token_LBracket)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        multi = true;
    } else if (1) {
        __auto_type _ = _t184;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        if (multi) {
            /* pass */
            Parser_skip_newlines_and_indent(self);
        } else {
            /* pass */
            Parser_skip_newlines(self);
        }
        /* pass */
        __auto_type _t185 = Parser_peek(self);
        if ((_t185.tag == Token_RParen || _t185.tag == Token_RBracket || _t185.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if ((_t185.tag == Token_Newline || _t185.tag == Token_Dedent)) {
            /* pass */
            if ((!multi)) {
                /* pass */
                going = false;
            }
        } else if (1) {
            __auto_type _ = _t185;
            /* pass */
            /* pass */
        }
        /* pass */
        if (going) {
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Star().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                List_ptr_append(il, ImportItem_init(_tr_str_lit("*")));
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else if ((!multi)) {
                    /* pass */
                    going = false;
                }
            } else {
                /* pass */
                TrStr item_name = Parser_consume_ident(self);
                /* pass */
                if ((strcmp(_tr_strz(item_name), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ImportItem* item = ImportItem_init(item_name);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                        /* pass */
                        item->alias = Parser_consume_ident(self);
                        /* pass */
                        if ((strcmp(_tr_strz(item->alias), _tr_strz(_tr_str_lit(""))) != 0)) {
                            /* pass */
                            _tr_dict_set(self->import_aliases, _tr_strz(item->alias), _tr_str_box(_tr_str_retain(item->name)));
                        }
                    }
                    /* pass */
                    List_ptr_append(il, _tr_obj_retain(item));
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    } else if ((!multi)) {
                        /* pass */
                        going = false;
                    }
                } else {
                    /* pass */
                    going = false;
                }
            }
        }
    }
    /* pass */
    if (multi) {
        /* pass */
        __auto_type _t186 = Parser_peek(self);
        if ((_t186.tag == Token_RParen || _t186.tag == Token_RBracket)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t186;
            /* pass */
            /* pass */
        }
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_decl(Decl_ctor_DFromImport(path, il));
}

__attribute__((hot)) Decl* Parser_parse_import(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr path = Parser_consume_module_ident(self);
    /* pass */
    while ((Parser_peek(self).tag == Token_make_Dot().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Star().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
            /* pass */
            List_ptr* ils = (void*)List_ptr_new();
            /* pass */
            List_ptr_append(ils, ImportItem_init(_tr_str_lit("*")));
            /* pass */
            return box_decl(Decl_ctor_DFromImport(path, ils));
        }
        /* pass */
        TrStr _strtmp_t187 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(path), _tr_strz(_tr_str_lit(".")))); TrStr _cr = (Parser_consume_module_ident(self)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(path);
        path = _strtmp_t187;
    }
    /* pass */
    TrStr alias = _tr_str_lit("");
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwAs().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr _strtmp_t188 = Parser_consume_ident(self);
        _tr_str_release(alias);
        alias = _strtmp_t188;
    }
    /* pass */
    Parser_expect_newline(self);
    /* pass */
    return box_decl(Decl_ctor_DImport(path, alias));
}

__attribute__((hot)) void Parser_parse_generic_bound(Parser* self, TrStr gname, List_ptr* constraints) {
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        GenericConstraint* gc = GenericConstraint_init(gname);
        /* pass */
        ({ TrStr _at_t189 = (Parser_consume_ident(self)); List_ptr_append(gc->bounds, box_asttype(AstType_init(_at_t189))); _tr_str_release(_at_t189); });
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Plus().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t190 = (Parser_consume_ident(self)); List_ptr_append(gc->bounds, box_asttype(AstType_init(_at_t190))); _tr_str_release(_at_t190); });
        }
        /* pass */
        List_ptr_append(constraints, _tr_obj_retain(gc));
    }
}

__attribute__((hot)) FunctionDef* Parser_parse_function_def(Parser* self, bool is_method) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr name = Parser_consume_ident(self);
    /* pass */
    FunctionDef* f = FunctionDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while ((Parser_peek(self).tag != Token_make_RBracket().tag)) {
            /* pass */
            TrStr _gpn = Parser_consume_ident(self);
            /* pass */
            List_TrStr_append(f->generics, _gpn);
            /* pass */
            Parser_parse_generic_bound(self, _gpn, f->constraints);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_gpn);
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->params = Parser_parse_param_list(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    __auto_type _t191 = Parser_peek(self);
    if (_t191.tag == Token_KwThrows) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->throws_ty = box_asttype(Parser_parse_type(self));
    } else if (1) {
        __auto_type _ = _t191;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Arrow().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        AstType* ret_t = Parser_parse_type(self);
        /* pass */
        if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ret_t->from_param = Parser_consume_ident(self);
            /* pass */
            List_TrStr_append(ret_t->from_regions, ret_t->from_param);
            /* pass */
            while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                TrStr rgn2 = Parser_consume_ident(self);
                /* pass */
                ret_t->from_param = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ret_t->from_param), _tr_strz(_tr_str_lit(",")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(rgn2)); _tr_str_release(_cl); _cres; });
                /* pass */
                List_TrStr_append(ret_t->from_regions, rgn2);
                _tr_str_release(rgn2);
            }
        }
        /* pass */
        f->ret_ty = box_asttype(ret_t);
    }
    /* pass */
    bool w_is = false;
    /* pass */
    __auto_type _t192 = Parser_peek(self);
    if (_t192.tag == Token_Ident) {
        __auto_type wkw0 = _t192.data.Ident.name;
        /* pass */
        if ((strcmp(_tr_strz(wkw0), _tr_strz(_tr_str_lit("where"))) == 0)) {
            /* pass */
            w_is = true;
        }
    } else if (1) {
        __auto_type _ = _t192;
        /* pass */
    }
    /* pass */
    if (((!w_is) && (Parser_peek(self).tag == Token_make_Newline().tag))) {
        /* pass */
        long long scan = self->pos;
        /* pass */
        bool scanning = true;
        /* pass */
        while (scanning) {
            /* pass */
            __auto_type _t193 = List_Token_get(self->tokens, scan);
            if ((_t193.tag == Token_Newline || _t193.tag == Token_Indent)) {
                /* pass */
                scan = (scan + 1LL);
            } else if (1) {
                __auto_type _ = _t193;
                /* pass */
                scanning = false;
            }
        }
        /* pass */
        __auto_type _t194 = List_Token_get(self->tokens, scan);
        if (_t194.tag == Token_Ident) {
            __auto_type wkw1 = _t194.data.Ident.name;
            /* pass */
            if ((strcmp(_tr_strz(wkw1), _tr_strz(_tr_str_lit("where"))) == 0)) {
                /* pass */
                w_is = true;
                /* pass */
                self->pos = scan;
            }
        } else if (1) {
            __auto_type _ = _t194;
            /* pass */
        }
    }
    /* pass */
    if (w_is) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        bool w_paren = false;
        /* pass */
        if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            w_paren = true;
            /* pass */
            Parser_skip_newlines_and_indent(self);
        }
        /* pass */
        bool w_more = true;
        /* pass */
        while (w_more) {
            /* pass */
            if (w_paren) {
                /* pass */
                Parser_skip_newlines_and_indent(self);
            }
            /* pass */
            if ((w_paren && (Parser_peek(self).tag == Token_make_RParen().tag))) {
                /* pass */
                w_more = false;
            } else {
                /* pass */
                TrStr w_a = Parser_consume_ident(self);
                /* pass */
                TrStr w_kw = Parser_consume_ident(self);
                /* pass */
                TrStr w_b = Parser_consume_ident(self);
                /* pass */
                List_TrStr_append(f->outlives_a, w_a);
                /* pass */
                List_TrStr_append(f->outlives_b, w_b);
                /* pass */
                if (w_paren) {
                    /* pass */
                    Parser_skip_newlines_and_indent(self);
                }
                /* pass */
                if ((w_paren && (Parser_peek(self).tag == Token_make_Comma().tag))) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                } else {
                    /* pass */
                    w_more = false;
                }
            }
        }
        /* pass */
        if (w_paren) {
            /* pass */
            Parser_skip_newlines_and_indent(self);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        f->body = Parser_parse_block(self);
    } else {
        /* pass */
        Parser_expect_newline(self);
        /* pass */
        f->body = Block_init();
    }
    /* pass */
    _tr_str_release(name);
    return f;
}

__attribute__((hot)) Decl* Parser_parse_class_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr name = Parser_consume_ident(self);
    /* pass */
    ClassDef* c = ClassDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while ((Parser_peek(self).tag != Token_make_RBracket().tag)) {
            /* pass */
            TrStr _cgpn = Parser_consume_ident(self);
            /* pass */
            List_TrStr_append(c->generics, _cgpn);
            /* pass */
            Parser_parse_generic_bound(self, _cgpn, c->constraints);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(_cgpn);
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        ({ TrStr _at_t195 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _at_t195); _tr_str_release(_at_t195); });
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t196 = (Parser_consume_ident(self)); List_TrStr_append(c->region_params, _at_t196); _tr_str_release(_at_t196); });
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwExtends().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        ({ TrStr _at_t197 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _at_t197); _tr_str_release(_at_t197); });
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t198 = (Parser_consume_ident(self)); List_TrStr_append(c->base_classes, _at_t198); _tr_str_release(_at_t198); });
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwImplements().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (true) {
            /* pass */
            ({ TrStr _at_t199 = (Parser_consume_ident(self)); List_TrStr_append(c->iface_names, _at_t199); _tr_str_release(_at_t199); });
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else {
                /* pass */
                break;
            }
        }
    }
    /* pass */
    bool inline_body = false;
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t200 = Parser_peek(self);
        if ((_t200.tag == Token_Newline || _t200.tag == Token_Eof)) {
            /* pass */
            /* pass */
        } else if (1) {
            __auto_type _ = _t200;
            /* pass */
            inline_body = true;
        }
    }
    /* pass */
    if (inline_body) {
        /* pass */
        bool inl = true;
        /* pass */
        while (inl) {
            /* pass */
            bool in_pub = false;
            /* pass */
            __auto_type _t201 = Parser_peek(self);
            if (_t201.tag == Token_KwPub) {
                /* pass */
                in_pub = true;
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t201;
                /* pass */
                /* pass */
            }
            /* pass */
            __auto_type _t202 = Parser_peek(self);
            if (_t202.tag == Token_KwPass) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (_t202.tag == Token_KwDef) {
                /* pass */
                long long _iml = Parser_cur_line(self);
                /* pass */
                FunctionDef* im = Parser_parse_function_def(self, true);
                /* pass */
                im->line = _iml;
                /* pass */
                im->is_public = in_pub;
                /* pass */
                List_ptr_append(c->methods, _tr_obj_retain(im));
                _tr_obj_release(im, _trdrop_FunctionDef);
            } else if (_t202.tag == Token_Ident) {
                __auto_type ifn = _t202.data.Ident.name;
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType** ity = (AstType**)(0LL);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    ity = box_asttype(Parser_parse_type(self));
                }
                /* pass */
                List_ptr_append(c->fields, FieldDef_init(ifn, ity));
            } else if (1) {
                __auto_type _ = _t202;
                /* pass */
                inl = false;
            }
            /* pass */
            __auto_type _t203 = Parser_peek(self);
            if (_t203.tag == Token_Semicolon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t203;
                /* pass */
                inl = false;
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Newline().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        _tr_str_release(name);
        return box_decl(Decl_ctor_DClass(c));
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t204 = Parser_peek(self);
    if (_t204.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t204;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        if ((strcmp(_tr_strz(c->docstring), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            __auto_type _t205 = Parser_peek(self);
            if (_t205.tag == Token_TripleStrLit) {
                __auto_type _ds = _t205.data.TripleStrLit.val;
                /* pass */
                c->docstring = _tr_str_retain(_ds);
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                Parser_skip_newlines(self);
            } else if (1) {
                __auto_type _ = _t205;
                /* pass */
                /* pass */
            }
        }
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, _tr_obj_retain(d));
            /* pass */
            Parser_expect_newline(self);
            _tr_obj_release(d, _trdrop_Decorator);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t206 = Parser_peek(self);
        if (_t206.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t206;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t207 = Parser_peek(self);
        if ((_t207.tag == Token_Dedent || _t207.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t207.tag == Token_KwDef) {
            /* pass */
            long long _mline = Parser_cur_line(self);
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->line = _mline;
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(c->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t207.tag == Token_KwAsync) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type _t208 = Parser_peek(self);
            if (_t208.tag == Token_KwDef) {
                /* pass */
                long long _mline = Parser_cur_line(self);
                /* pass */
                FunctionDef* m = Parser_parse_function_def(self, true);
                /* pass */
                m->line = _mline;
                /* pass */
                m->is_async = true;
                /* pass */
                m->is_public = is_p;
                /* pass */
                m->decorators = decorators;
                /* pass */
                List_ptr_append(c->methods, _tr_obj_retain(m));
                _tr_obj_release(m, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t208;
                /* pass */
                /* pass */
            }
        } else if (_t207.tag == Token_Ident) {
            __auto_type fname = _t207.data.Ident.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            AstType** ty_ptr = (AstType**)(0LL);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                AstType* fty = Parser_parse_type(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    fty->from_param = Parser_consume_ident(self);
                    /* pass */
                    List_TrStr_append(fty->from_regions, fty->from_param);
                    /* pass */
                    while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                        /* pass */
                        TrStr frgn2 = Parser_consume_ident(self);
                        /* pass */
                        fty->from_param = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(fty->from_param), _tr_strz(_tr_str_lit(",")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(frgn2)); _tr_str_release(_cl); _cres; });
                        /* pass */
                        List_TrStr_append(fty->from_regions, frgn2);
                        _tr_str_release(frgn2);
                    }
                }
                /* pass */
                ty_ptr = box_asttype(fty);
            }
            /* pass */
            FieldDef* fld = FieldDef_init(fname, ty_ptr);
            /* pass */
            List_ptr_append(c->fields, _tr_obj_retain(fld));
            /* pass */
            Parser_expect_newline(self);
            _tr_obj_release(fld, _trdrop_FieldDef);
        } else if (_t207.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t207;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    _tr_str_release(name);
    return box_decl(Decl_ctor_DClass(c));
}

__attribute__((hot)) Decl* Parser_parse_enum_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr name = Parser_consume_ident(self);
    /* pass */
    EnumDef* e = EnumDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        ({ TrStr _at_t209 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _at_t209); _tr_str_release(_at_t209); });
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t210 = (Parser_consume_ident(self)); List_TrStr_append(e->region_params, _at_t210); _tr_str_release(_at_t210); });
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwImplements().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (true) {
            /* pass */
            ({ TrStr _at_t211 = (Parser_consume_ident(self)); List_TrStr_append(e->iface_names, _at_t211); _tr_str_release(_at_t211); });
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else {
                /* pass */
                break;
            }
        }
    }
    /* pass */
    bool e_inline = false;
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t212 = Parser_peek(self);
        if ((_t212.tag == Token_Newline || _t212.tag == Token_Eof)) {
            /* pass */
            /* pass */
        } else if (1) {
            __auto_type _ = _t212;
            /* pass */
            e_inline = true;
        }
    }
    /* pass */
    if (e_inline) {
        /* pass */
        bool einl = true;
        /* pass */
        while (einl) {
            /* pass */
            __auto_type _t213 = Parser_peek(self);
            if (_t213.tag == Token_KwPass) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (_t213.tag == Token_Ident) {
                __auto_type evn = _t213.data.Ident.name;
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                VariantDef* evd = VariantDef_init(evn);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                    /* pass */
                    evd->fields = Parser_parse_param_list(self);
                    /* pass */
                    if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                        /* pass */
                        self->pos = (self->pos + 1LL);
                    }
                }
                /* pass */
                List_ptr_append(e->variants, _tr_obj_retain(evd));
                _tr_obj_release(evd, _trdrop_VariantDef);
            } else if (1) {
                __auto_type _ = _t213;
                /* pass */
                einl = false;
            }
            /* pass */
            __auto_type _t214 = Parser_peek(self);
            if ((_t214.tag == Token_Comma || _t214.tag == Token_Semicolon)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t214;
                /* pass */
                einl = false;
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Newline().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        _tr_str_release(name);
        return box_decl(Decl_ctor_DEnum(e));
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t215 = Parser_peek(self);
    if (_t215.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t215;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, _tr_obj_retain(d));
            /* pass */
            Parser_expect_newline(self);
            _tr_obj_release(d, _trdrop_Decorator);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t216 = Parser_peek(self);
        if (_t216.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t216;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t217 = Parser_peek(self);
        if ((_t217.tag == Token_Dedent || _t217.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t217.tag == Token_Ident) {
            __auto_type vname = _t217.data.Ident.name;
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            VariantDef* vd = VariantDef_init(vname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                vd->fields = Parser_parse_param_list(self);
                /* pass */
                if ((Parser_peek(self).tag == Token_make_RParen().tag)) {
                    /* pass */
                    self->pos = (self->pos + 1LL);
                }
            }
            /* pass */
            List_ptr_append(e->variants, _tr_obj_retain(vd));
            /* pass */
            Parser_expect_newline(self);
            _tr_obj_release(vd, _trdrop_VariantDef);
        } else if (_t217.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(e->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t217.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t217;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    _tr_str_release(name);
    return box_decl(Decl_ctor_DEnum(e));
}

__attribute__((hot)) Decl* Parser_parse_interface_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr name = Parser_consume_ident(self);
    /* pass */
    InterfaceDef* i = InterfaceDef_init(name);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_LBracket().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        while (((Parser_peek(self).tag != Token_make_RBracket().tag) && (Parser_peek(self).tag != Token_make_Eof().tag))) {
            /* pass */
            TrStr gname = Parser_consume_ident(self);
            /* pass */
            if ((strcmp(_tr_strz(gname), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                List_TrStr_append(i->generics, gname);
            }
            /* pass */
            if ((Parser_peek(self).tag == Token_make_Comma().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
            }
            _tr_str_release(gname);
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_RBracket().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        ({ TrStr _at_t218 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _at_t218); _tr_str_release(_at_t218); });
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            ({ TrStr _at_t219 = (Parser_consume_ident(self)); List_TrStr_append(i->region_params, _at_t219); _tr_str_release(_at_t219); });
        }
    }
    /* pass */
    bool i_inline = false;
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        __auto_type _t220 = Parser_peek(self);
        if ((_t220.tag == Token_Newline || _t220.tag == Token_Eof)) {
            /* pass */
            /* pass */
        } else if (1) {
            __auto_type _ = _t220;
            /* pass */
            i_inline = true;
        }
    }
    /* pass */
    if (i_inline) {
        /* pass */
        bool iinl = true;
        /* pass */
        while (iinl) {
            /* pass */
            bool i_pub = false;
            /* pass */
            __auto_type _t221 = Parser_peek(self);
            if (_t221.tag == Token_KwPub) {
                /* pass */
                i_pub = true;
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t221;
                /* pass */
                /* pass */
            }
            /* pass */
            __auto_type _t222 = Parser_peek(self);
            if (_t222.tag == Token_KwPass) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (_t222.tag == Token_KwDef) {
                /* pass */
                FunctionDef* im2 = Parser_parse_function_def(self, true);
                /* pass */
                im2->is_public = i_pub;
                /* pass */
                List_ptr_append(i->methods, _tr_obj_retain(im2));
                _tr_obj_release(im2, _trdrop_FunctionDef);
            } else if (1) {
                __auto_type _ = _t222;
                /* pass */
                iinl = false;
            }
            /* pass */
            __auto_type _t223 = Parser_peek(self);
            if (_t223.tag == Token_Semicolon) {
                /* pass */
                self->pos = (self->pos + 1LL);
            } else if (1) {
                __auto_type _ = _t223;
                /* pass */
                iinl = false;
            }
        }
        /* pass */
        if ((Parser_peek(self).tag == Token_make_Newline().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
        }
        /* pass */
        _tr_str_release(name);
        return box_decl(Decl_ctor_DInterface(i));
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t224 = Parser_peek(self);
    if (_t224.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t224;
        /* pass */
        /* pass */
    }
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t225 = Parser_peek(self);
        if (_t225.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t225;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t226 = Parser_peek(self);
        if ((_t226.tag == Token_Dedent || _t226.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t226.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            List_ptr_append(i->methods, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t226.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t226;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    _tr_str_release(name);
    return box_decl(Decl_ctor_DInterface(i));
}

__attribute__((hot)) Decl* Parser_parse_extend_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr target = Parser_consume_ident(self);
    /* pass */
    if ((Parser_peek(self).tag == Token_make_KwFrom().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr _erg = Parser_consume_ident(self);
        /* pass */
        while ((Parser_peek(self).tag == Token_make_Comma().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            TrStr _strtmp_t227 = Parser_consume_ident(self);
            _tr_str_release(_erg);
            _erg = _strtmp_t227;
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t228 = Parser_peek(self);
    if (_t228.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t228;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        List_ptr* decorators = (void*)List_ptr_new();
        /* pass */
        while ((Parser_peek(self).tag == Token_make_At().tag)) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            __auto_type dname = Parser_consume_ident(self);
            /* pass */
            Decorator* d = Decorator_init(dname);
            /* pass */
            if ((Parser_peek(self).tag == Token_make_LParen().tag)) {
                /* pass */
                self->pos = (self->pos + 1LL);
                /* pass */
                d->args = Parser_parse_arg_list(self);
            }
            /* pass */
            List_ptr_append(decorators, _tr_obj_retain(d));
            /* pass */
            Parser_expect_newline(self);
            _tr_obj_release(d, _trdrop_Decorator);
        }
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t229 = Parser_peek(self);
        if (_t229.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t229;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t230 = Parser_peek(self);
        if ((_t230.tag == Token_Dedent || _t230.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t230.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, true);
            /* pass */
            m->is_public = is_p;
            /* pass */
            m->decorators = decorators;
            /* pass */
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (_t230.tag == Token_KwPass) {
            /* pass */
            self->pos = (self->pos + 1LL);
            /* pass */
            Parser_expect_newline(self);
        } else if (1) {
            __auto_type _ = _t230;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Decl d = Decl_ctor_DExtend(target, fl);
    /* pass */
    _tr_str_release(target);
    return box_decl(d);
}

__attribute__((hot)) Decl* Parser_parse_extern_decl(Parser* self) {
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    TrStr abi = _tr_str_lit("C");
    /* pass */
    __auto_type _t231 = Parser_peek(self);
    if (_t231.tag == Token_StrLit) {
        __auto_type s = _t231.data.StrLit.val;
        /* pass */
        self->pos = (self->pos + 1LL);
        /* pass */
        TrStr _strtmp_t232 = _tr_str_retain(s);
        _tr_str_release(abi);
        abi = _strtmp_t232;
    } else if (1) {
        __auto_type _ = _t231;
        /* pass */
        /* pass */
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Colon().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    Parser_skip_newlines(self);
    /* pass */
    __auto_type _t233 = Parser_peek(self);
    if (_t233.tag == Token_Indent) {
        /* pass */
        self->pos = (self->pos + 1LL);
    } else if (1) {
        __auto_type _ = _t233;
        /* pass */
        /* pass */
    }
    /* pass */
    List_ptr* fl = (void*)List_ptr_new();
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        Parser_skip_newlines(self);
        /* pass */
        bool is_p = false;
        /* pass */
        __auto_type _t234 = Parser_peek(self);
        if (_t234.tag == Token_KwPub) {
            /* pass */
            is_p = true;
            /* pass */
            self->pos = (self->pos + 1LL);
        } else if (1) {
            __auto_type _ = _t234;
            /* pass */
            /* pass */
        }
        /* pass */
        __auto_type _t235 = Parser_peek(self);
        if ((_t235.tag == Token_Dedent || _t235.tag == Token_Eof)) {
            /* pass */
            going = false;
        } else if (_t235.tag == Token_KwDef) {
            /* pass */
            FunctionDef* m = Parser_parse_function_def(self, false);
            /* pass */
            m->is_public = is_p;
            /* pass */
            if ((m->params->len > 0LL)) {
                /* pass */
                Param* _ext_last = ((Param*)List_ptr_get(m->params, (m->params->len - 1LL)));
                /* pass */
                if (_ext_last->is_variadic) {
                    /* pass */
                    ((Param*)List_ptr_pop(m->params));
                    /* pass */
                    m->is_variadic = true;
                }
            }
            /* pass */
            List_ptr_append(fl, _tr_obj_retain(m));
            _tr_obj_release(m, _trdrop_FunctionDef);
        } else if (1) {
            __auto_type _ = _t235;
            /* pass */
            self->pos = (self->pos + 1LL);
        }
    }
    /* pass */
    if ((Parser_peek(self).tag == Token_make_Dedent().tag)) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
    /* pass */
    return box_decl(Decl_ctor_DExtern(abi, fl));
}

__attribute__((hot)) Expr* box_expr(Expr e) {
    /* pass */
    /* unsafe block */
    /* pass */
    Expr* p = ((Expr*)_tr_c_calloc((size_t)(1LL), sizeof(Expr)));
    /* pass */
    (*p = e);
    /* pass */
    return p;
}

__attribute__((hot)) Stmt* box_stmt(Stmt s) {
    /* pass */
    /* unsafe block */
    /* pass */
    Stmt* p = ((Stmt*)_tr_c_calloc((size_t)(1LL), sizeof(Stmt)));
    /* pass */
    (*p = s);
    /* pass */
    return p;
}

__attribute__((hot)) Decl* box_decl(Decl d) {
    /* pass */
    /* unsafe block */
    /* pass */
    Decl* p = ((Decl*)_tr_c_calloc((size_t)(1LL), sizeof(Decl)));
    /* pass */
    (*p = d);
    /* pass */
    return p;
}

__attribute__((hot)) AstType** box_asttype(AstType* t) {
    /* pass */
    /* unsafe block */
    /* pass */
    AstType** p = ((AstType**)_tr_c_calloc((size_t)(1LL), sizeof(AstType*)));
    /* pass */
    (*p = t);
    /* pass */
    return p;
}

__attribute__((hot)) TrStr _nth_source_line(TrStr src, long long n) {
    /* pass */
    if ((n <= 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    char* p = ((char*)(_tr_strz(src)));
    /* pass */
    long long i = 0LL;
    /* pass */
    long long line = 1LL;
    /* pass */
    StringBuilder* sb = StringBuilder_init(80LL);
    /* pass */
    while (true) {
        /* pass */
        long long c = 0LL;
        /* pass */
        /* unsafe block */
        /* pass */
        c = ((long long)((*(p + i))));
        /* pass */
        if ((c == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if ((line == n)) {
            /* pass */
            if (((c == 10LL) || (c == 13LL))) {
                /* pass */
                break;
            }
            /* pass */
            StringBuilder_append_char(sb, c);
        } else if ((line > n)) {
            /* pass */
            break;
        }
        /* pass */
        if ((c == 10LL)) {
            /* pass */
            line = (line + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _spaces(long long n) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((n + 1LL));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        StringBuilder_append_char(sb, 32LL);
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) long long _find_fmt_colon(TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = _tr_str_char_at_code(_tr_strz(s), i);
        /* pass */
        if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
            /* pass */
            depth = (depth - 1LL);
        } else if (((c == 58LL) && (depth == 0LL))) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

