#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) Lexer* Lexer_init(TrStr source) {
    /* pass */
    Lexer* lx = ((Lexer*)_tr_obj_alloc(sizeof(Lexer)));
    /* pass */
    /* unsafe block */
    /* pass */
    lx->src = ((char*)(_tr_strz(source)));
    /* pass */
    long long src_len = 0LL;
    /* pass */
    /* unsafe block */
    /* pass */
    char* p = lx->src;
    /* pass */
    while ((((long long)((*(p + src_len)))) != 0LL)) {
        /* pass */
        src_len = (src_len + 1LL);
    }
    /* pass */
    lx->len = src_len;
    /* pass */
    lx->pos = 0LL;
    /* pass */
    lx->line = 1LL;
    /* pass */
    lx->line_start = 0LL;
    /* pass */
    lx->tok_col = 1LL;
    /* pass */
    lx->indent_stack = (void*)List_i64_new();
    /* pass */
    List_i64_append(lx->indent_stack, 0LL);
    /* pass */
    lx->pending_dedents = 0LL;
    /* pass */
    lx->token_lines = (void*)List_i64_new();
    /* pass */
    lx->token_cols = (void*)List_i64_new();
    /* pass */
    lx->record_comments = false;
    /* pass */
    lx->comment_lines = (void*)List_i64_new();
    /* pass */
    lx->comment_texts = (void*)List_TrStr_new();
    /* pass */
    lx->comment_trailing = (void*)List_bool_new();
    /* pass */
    return lx;
}

__attribute__((hot)) void Lexer_push_loc(Lexer* self) {
    /* pass */
    List_i64_append(self->token_lines, self->line);
    /* pass */
    List_i64_append(self->token_cols, self->tok_col);
}

__attribute__((hot)) long long Lexer_peek(Lexer* self) {
    /* pass */
    if ((self->pos >= self->len)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    /* unsafe block */
    /* pass */
    return ((long long)((*(self->src + self->pos))));
}

__attribute__((hot)) long long Lexer_peek_at(Lexer* self, long long offset) {
    /* pass */
    long long p = (self->pos + offset);
    /* pass */
    if ((p >= self->len)) {
        /* pass */
        return 0LL;
    }
    /* pass */
    /* unsafe block */
    /* pass */
    return ((long long)((*(self->src + p))));
}

__attribute__((hot)) bool Lexer__at_eol_after_ws(Lexer* self) {
    /* pass */
    long long k = 0LL;
    /* pass */
    while (true) {
        /* pass */
        long long ch = Lexer_peek_at(self, k);
        /* pass */
        if (((ch == 32LL) || (ch == 9LL))) {
            /* pass */
            k = (k + 1LL);
        } else {
            /* pass */
            break;
        }
    }
    /* pass */
    long long ch2 = Lexer_peek_at(self, k);
    /* pass */
    if ((ch2 == 0LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((ch2 == 35LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if (char_is_newline(ch2)) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) long long Lexer_advance(Lexer* self) {
    /* pass */
    long long c = Lexer_peek(self);
    /* pass */
    self->pos = (self->pos + 1LL);
    /* pass */
    if ((c == 10LL)) {
        /* pass */
        self->line = (self->line + 1LL);
        /* pass */
        self->line_start = self->pos;
    }
    /* pass */
    return c;
}

__attribute__((hot)) bool Lexer_at_end(Lexer* self) {
    /* pass */
    return (self->pos >= self->len);
}

__attribute__((hot)) void Lexer_skip_spaces(Lexer* self) {
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        long long c = Lexer_peek(self);
        /* pass */
        if (char_is_space(c)) {
            /* pass */
            Lexer_advance(self);
        } else {
            /* pass */
            break;
        }
    }
}

__attribute__((hot)) void Lexer_skip_comment(Lexer* self, bool trailing) {
    /* pass */
    if (self->record_comments) {
        /* pass */
        StringBuilder* sb = StringBuilder_init(32LL);
        /* pass */
        while (((!Lexer_at_end(self)) && (!char_is_newline(Lexer_peek(self))))) {
            /* pass */
            StringBuilder_append_char(sb, Lexer_advance(self));
        }
        /* pass */
        List_i64_append(self->comment_lines, self->line);
        /* pass */
        ({ TrStr _at_t6 = (StringObj_as_str(StringBuilder_to_string(sb))); List_TrStr_append(self->comment_texts, _at_t6); _tr_str_release(_at_t6); });
        /* pass */
        List_bool_append(self->comment_trailing, trailing);
        StringBuilder__tr_fn_free(sb);
    } else {
        /* pass */
        while (((!Lexer_at_end(self)) && (!char_is_newline(Lexer_peek(self))))) {
            /* pass */
            Lexer_advance(self);
        }
    }
}

__attribute__((hot)) Token Lexer_read_int(Lexer* self) {
    /* pass */
    if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 120LL) || (Lexer_peek_at(self, 1LL) == 88LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((char_is_hex(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                unsigned long long digit = ((unsigned long long)(0LL));
                /* pass */
                if (((c >= 48LL) && (c <= 57LL))) {
                    /* pass */
                    digit = ((unsigned long long)((c - 48LL)));
                } else if (((c >= 65LL) && (c <= 70LL))) {
                    /* pass */
                    digit = ((unsigned long long)(((c - 65LL) + 10LL)));
                } else {
                    /* pass */
                    digit = ((unsigned long long)(((c - 97LL) + 10LL)));
                }
                /* pass */
                val = ((val * ((unsigned long long)(16LL))) + digit);
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 98LL) || (Lexer_peek_at(self, 1LL) == 66LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((((Lexer_peek(self) == 48LL) || (Lexer_peek(self) == 49LL)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(2LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else if (((Lexer_peek(self) == 48LL) && ((Lexer_peek_at(self, 1LL) == 111LL) || (Lexer_peek_at(self, 1LL) == 79LL)))) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        Lexer_advance(self);
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((((Lexer_peek(self) >= 48LL) && (Lexer_peek(self) <= 55LL)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(8LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    } else {
        /* pass */
        unsigned long long val = ((unsigned long long)(0LL));
        /* pass */
        while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
            /* pass */
            long long c = Lexer_advance(self);
            /* pass */
            if ((c != 95LL)) {
                /* pass */
                val = ((val * ((unsigned long long)(10LL))) + ((unsigned long long)((c - 48LL))));
            }
        }
        /* pass */
        if (((Lexer_peek(self) == 46LL) && char_is_digit(Lexer_peek_at(self, 1LL)))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            double frac = 0.0;
            /* pass */
            double divisor = 10.0;
            /* pass */
            while ((char_is_digit(Lexer_peek(self)) || (Lexer_peek(self) == 95LL))) {
                /* pass */
                long long c = Lexer_advance(self);
                /* pass */
                if ((c != 95LL)) {
                    /* pass */
                    frac = (frac + ((double)((c - 48LL)) / divisor));
                    /* pass */
                    divisor = (divisor * 10.0);
                }
            }
            /* pass */
            double result = ((double)(((long long)(val))) + frac);
            /* pass */
            if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                bool exp_neg = false;
                /* pass */
                if ((Lexer_peek(self) == 45LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    exp_neg = true;
                } else if ((Lexer_peek(self) == 43LL)) {
                    /* pass */
                    Lexer_advance(self);
                }
                /* pass */
                long long exp = 0LL;
                /* pass */
                while (char_is_digit(Lexer_peek(self))) {
                    /* pass */
                    exp = ((exp * 10LL) + (Lexer_advance(self) - 48LL));
                }
                /* pass */
                long long ei = 0LL;
                /* pass */
                if (exp_neg) {
                    /* pass */
                    while ((ei < exp)) {
                        /* pass */
                        result = (result / 10.0);
                        /* pass */
                        ei = (ei + 1LL);
                    }
                } else {
                    /* pass */
                    while ((ei < exp)) {
                        /* pass */
                        result = (result * 10.0);
                        /* pass */
                        ei = (ei + 1LL);
                    }
                }
            }
            /* pass */
            return Token_ctor_FloatLit(result);
        }
        /* pass */
        if (((Lexer_peek(self) == 101LL) || (Lexer_peek(self) == 69LL))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            bool exp_neg = false;
            /* pass */
            if ((Lexer_peek(self) == 45LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                exp_neg = true;
            } else if ((Lexer_peek(self) == 43LL)) {
                /* pass */
                Lexer_advance(self);
            }
            /* pass */
            long long exp = 0LL;
            /* pass */
            while (char_is_digit(Lexer_peek(self))) {
                /* pass */
                exp = ((exp * 10LL) + (Lexer_advance(self) - 48LL));
            }
            /* pass */
            __auto_type result = (double)(((long long)(val)));
            /* pass */
            long long ei = 0LL;
            /* pass */
            if (exp_neg) {
                /* pass */
                while ((ei < exp)) {
                    /* pass */
                    result = (result / 10.0);
                    /* pass */
                    ei = (ei + 1LL);
                }
            } else {
                /* pass */
                while ((ei < exp)) {
                    /* pass */
                    result = (result * 10.0);
                    /* pass */
                    ei = (ei + 1LL);
                }
            }
            /* pass */
            return Token_ctor_FloatLit(result);
        }
        /* pass */
        return Token_ctor_IntLit(((long long)(val)));
    }
}

__attribute__((hot)) Token Lexer_read_triple_string(Lexer* self, long long quote) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(128LL);
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        if ((((Lexer_peek(self) == quote) && (Lexer_peek_at(self, 1LL) == quote)) && (Lexer_peek_at(self, 2LL) == quote))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            return ({ TrStr _at_t7 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_TripleStrLit(_at_t7)); _tr_str_release(_at_t7); _wr; });
        }
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    return ({ TrStr _at_t8 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_TripleStrLit(_at_t8)); _tr_str_release(_at_t8); _wr; });
}

__attribute__((hot)) Token Lexer_read_string(Lexer* self, long long quote) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            long long esc = Lexer_advance(self);
            /* pass */
            if ((esc == 110LL)) {
                /* pass */
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
                /* pass */
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
                /* pass */
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
                /* pass */
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
                /* pass */
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
                /* pass */
                StringBuilder_append_char(sb, 0LL);
            } else {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
                /* pass */
                StringBuilder_append_char(sb, esc);
            }
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return ({ TrStr _at_t9 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_StrLit(_at_t9)); _tr_str_release(_at_t9); _wr; });
}

__attribute__((hot)) Token Lexer_read_char(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long val = 0LL;
    /* pass */
    if ((Lexer_peek(self) == 92LL)) {
        /* pass */
        Lexer_advance(self);
        /* pass */
        long long esc = Lexer_advance(self);
        /* pass */
        if ((esc == 110LL)) {
            /* pass */
            val = 10LL;
        } else if ((esc == 116LL)) {
            /* pass */
            val = 9LL;
        } else if ((esc == 114LL)) {
            /* pass */
            val = 13LL;
        } else if ((esc == 92LL)) {
            /* pass */
            val = 92LL;
        } else if ((esc == 39LL)) {
            /* pass */
            val = 39LL;
        } else if ((esc == 48LL)) {
            /* pass */
            val = 0LL;
        } else {
            /* pass */
            val = esc;
        }
    } else {
        /* pass */
        val = Lexer_advance(self);
    }
    /* pass */
    if ((Lexer_peek(self) == 39LL)) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return Token_ctor_CharLit(val);
}

__attribute__((hot)) Token Lexer_read_fstring(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        StringBuilder_append_char(sb, c);
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return ({ TrStr _at_t10 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_FStrLit(_at_t10)); _tr_str_release(_at_t10); _wr; });
}

__attribute__((hot)) Token Lexer_read_triple_fstring(Lexer* self, long long quote) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(128LL);
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        if ((((Lexer_peek(self) == quote) && (Lexer_peek_at(self, 1LL) == quote)) && (Lexer_peek_at(self, 2LL) == quote))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            return ({ TrStr _at_t11 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_FStrLit(_at_t11)); _tr_str_release(_at_t11); _wr; });
        }
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    return ({ TrStr _at_t12 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_FStrLit(_at_t12)); _tr_str_release(_at_t12); _wr; });
}

__attribute__((hot)) Token Lexer_read_raw_string(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return ({ TrStr _at_t13 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_RawStrLit(_at_t13)); _tr_str_release(_at_t13); _wr; });
}

__attribute__((hot)) Token Lexer_read_byte_string(Lexer* self) {
    /* pass */
    Lexer_advance(self);
    /* pass */
    long long quote = Lexer_advance(self);
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (((!Lexer_at_end(self)) && (Lexer_peek(self) != quote))) {
        /* pass */
        long long c = Lexer_advance(self);
        /* pass */
        if ((c == 92LL)) {
            /* pass */
            long long esc = Lexer_advance(self);
            /* pass */
            if ((esc == 110LL)) {
                /* pass */
                StringBuilder_append_char(sb, 10LL);
            } else if ((esc == 116LL)) {
                /* pass */
                StringBuilder_append_char(sb, 9LL);
            } else if ((esc == 114LL)) {
                /* pass */
                StringBuilder_append_char(sb, 13LL);
            } else if ((esc == 92LL)) {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
            } else if ((esc == 39LL)) {
                /* pass */
                StringBuilder_append_char(sb, 39LL);
            } else if ((esc == 34LL)) {
                /* pass */
                StringBuilder_append_char(sb, 34LL);
            } else if ((esc == 48LL)) {
                /* pass */
                StringBuilder_append_char(sb, 0LL);
            } else {
                /* pass */
                StringBuilder_append_char(sb, 92LL);
                /* pass */
                StringBuilder_append_char(sb, esc);
            }
        } else {
            /* pass */
            StringBuilder_append_char(sb, c);
        }
    }
    /* pass */
    if ((!Lexer_at_end(self))) {
        /* pass */
        Lexer_advance(self);
    }
    /* pass */
    return ({ TrStr _at_t14 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (Token_ctor_ByteStrLit(_at_t14)); _tr_str_release(_at_t14); _wr; });
}

__attribute__((hot)) Token Lexer_read_ident(Lexer* self) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(64LL);
    /* pass */
    while (char_is_alnum(Lexer_peek(self))) {
        /* pass */
        StringBuilder_append_char(sb, Lexer_advance(self));
    }
    /* pass */
    return ({ TrStr _at_t15 = (StringObj_as_str(StringBuilder_to_string(sb))); __auto_type _wr = (keyword_to_token(_at_t15)); _tr_str_release(_at_t15); _wr; });
}

__attribute__((hot)) List_Token* Lexer_tokenize(Lexer* self) {
    /* pass */
    if (((self->pos == 0LL) && (self->len >= 3LL))) {
        /* pass */
        if ((((Lexer_peek_at(self, 0LL) == 239LL) && (Lexer_peek_at(self, 1LL) == 187LL)) && (Lexer_peek_at(self, 2LL) == 191LL))) {
            /* pass */
            self->pos = (self->pos + 3LL);
        }
    }
    /* pass */
    List_Token* tokens = (void*)List_Token_new();
    /* pass */
    bool at_line_start = true;
    /* pass */
    bool trailing_dot = false;
    /* pass */
    long long nesting = 0LL;
    /* pass */
    List_i64* susp_nesting = (void*)List_i64_new();
    /* pass */
    List_i64* susp_baseline = (void*)List_i64_new();
    /* pass */
    bool block_kw_seen = false;
    /* pass */
    while ((!Lexer_at_end(self))) {
        /* pass */
        if ((self->pending_dedents > 0LL)) {
            /* pass */
            self->tok_col = 1LL;
            /* pass */
            List_Token_append(tokens, Token_make_Dedent());
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            self->pending_dedents = (self->pending_dedents - 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if (at_line_start) {
            /* pass */
            at_line_start = false;
            /* pass */
            long long indent = 0LL;
            /* pass */
            while ((!Lexer_at_end(self))) {
                /* pass */
                long long c_peek = Lexer_peek(self);
                /* pass */
                if ((c_peek == 32LL)) {
                    /* pass */
                    indent = (indent + 1LL);
                    /* pass */
                    Lexer_advance(self);
                } else if ((c_peek == 9LL)) {
                    /* pass */
                    indent = (indent + 4LL);
                    /* pass */
                    Lexer_advance(self);
                } else {
                    /* pass */
                    break;
                }
            }
            /* pass */
            if (Lexer_at_end(self)) {
                /* pass */
                break;
            }
            /* pass */
            long long next_c = Lexer_peek(self);
            /* pass */
            if ((char_is_newline(next_c) || (next_c == 35LL))) {
                /* pass */
                if ((next_c == 35LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    Lexer_skip_comment(self, false);
                } else {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    if (((next_c == 13LL) && (Lexer_peek(self) == 10LL))) {
                        /* pass */
                        Lexer_advance(self);
                    }
                }
                /* pass */
                at_line_start = true;
                /* pass */
                continue;
            }
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                self->tok_col = ((self->pos - self->line_start) + 1LL);
                /* pass */
                if (((next_c != 46LL) && (!trailing_dot))) {
                    /* pass */
                    long long cur_indent = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
                    /* pass */
                    if ((indent > cur_indent)) {
                        /* pass */
                        List_i64_append(self->indent_stack, indent);
                        /* pass */
                        List_Token_append(tokens, Token_make_Indent());
                        /* pass */
                        Lexer_push_loc(self);
                    } else if ((indent < cur_indent)) {
                        /* pass */
                        while ((self->indent_stack->len > 1LL)) {
                            /* pass */
                            long long top = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
                            /* pass */
                            if ((top <= indent)) {
                                /* pass */
                                break;
                            }
                            /* pass */
                            self->indent_stack->len = (self->indent_stack->len - 1LL);
                            /* pass */
                            List_Token_append(tokens, Token_make_Dedent());
                            /* pass */
                            Lexer_push_loc(self);
                        }
                        /* pass */
                        long long newtop = List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL));
                        /* pass */
                        if ((newtop != indent)) {
                            /* pass */
                            self->tok_col = ((self->pos - self->line_start) + 1LL);
                            /* pass */
                            List_Token_append(tokens, Token_ctor_Error(_tr_str_lit("inconsistent indentation: this line does not line up with any enclosing block (check for mixed tabs and spaces, or an off-by-one indent)")));
                            /* pass */
                            Lexer_push_loc(self);
                        }
                    }
                }
                /* pass */
                trailing_dot = false;
                /* pass */
                while ((susp_nesting->len > 0LL)) {
                    /* pass */
                    long long _base = List_i64_get(susp_baseline, (susp_baseline->len - 1LL));
                    /* pass */
                    if ((indent > _base)) {
                        /* pass */
                        break;
                    }
                    /* pass */
                    nesting = List_i64_get(susp_nesting, (susp_nesting->len - 1LL));
                    /* pass */
                    susp_nesting->len = (susp_nesting->len - 1LL);
                    /* pass */
                    susp_baseline->len = (susp_baseline->len - 1LL);
                    /* pass */
                    block_kw_seen = false;
                }
            }
        }
        /* pass */
        long long c = Lexer_peek(self);
        /* pass */
        self->tok_col = ((self->pos - self->line_start) + 1LL);
        /* pass */
        if (char_is_space(c)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_newline(c)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            if (((c == 13LL) && (Lexer_peek(self) == 10LL))) {
                /* pass */
                Lexer_advance(self);
            }
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                if (_last_tok_is_dot(tokens)) {
                    /* pass */
                    trailing_dot = true;
                } else if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
                    /* pass */
                    List_Token_append(tokens, Token_make_Newline());
                    /* pass */
                    Lexer_push_loc(self);
                }
                /* pass */
                at_line_start = true;
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 35LL)) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_skip_comment(self, true);
            /* pass */
            if ((nesting == 0LL)) {
                /* pass */
                if (_last_tok_is_dot(tokens)) {
                    /* pass */
                    trailing_dot = true;
                } else if ((!_peek_next_line_dot(self->src, self->pos, self->len))) {
                    /* pass */
                    List_Token_append(tokens, Token_make_Newline());
                    /* pass */
                    Lexer_push_loc(self);
                }
                /* pass */
                at_line_start = true;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_digit(c)) {
            /* pass */
            List_Token_append(tokens, Lexer_read_int(self));
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 34LL) || (c == 39LL))) {
            /* pass */
            if ((((c == 34LL) && (Lexer_peek_at(self, 1LL) == 34LL)) && (Lexer_peek_at(self, 2LL) == 34LL))) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                Lexer_advance(self);
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Lexer_read_triple_string(self, 34LL));
                /* pass */
                Lexer_push_loc(self);
            } else if (((c == 39LL) && ((Lexer_peek_at(self, 2LL) == 39LL) || (Lexer_peek_at(self, 1LL) == 92LL)))) {
                /* pass */
                List_Token_append(tokens, Lexer_read_char(self));
                /* pass */
                Lexer_push_loc(self);
            } else {
                /* pass */
                List_Token_append(tokens, Lexer_read_string(self, c));
                /* pass */
                Lexer_push_loc(self);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (((((c == 102LL) && (Lexer_peek_at(self, 1LL) == 34LL)) && (Lexer_peek_at(self, 2LL) == 34LL)) && (Lexer_peek_at(self, 3LL) == 34LL))) {
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            Lexer_advance(self);
            /* pass */
            List_Token_append(tokens, Lexer_read_triple_fstring(self, 34LL));
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 102LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_fstring(self));
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 114LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_raw_string(self));
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        if (((c == 98LL) && ((Lexer_peek_at(self, 1LL) == 34LL) || (Lexer_peek_at(self, 1LL) == 39LL)))) {
            /* pass */
            List_Token_append(tokens, Lexer_read_byte_string(self));
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        if (char_is_alpha(c)) {
            /* pass */
            Token _idtok = Lexer_read_ident(self);
            /* pass */
            __auto_type _t16 = _idtok;
            if (_t16.tag == Token_KwDo) {
                block_kw_seen = true;
            } else if (_t16.tag == Token_KwIf) {
                block_kw_seen = true;
            } else if (_t16.tag == Token_KwMatch) {
                block_kw_seen = true;
            } else if (_t16.tag == Token_KwLoop) {
                block_kw_seen = true;
            } else if (1) {
                __auto_type _ = _t16;
                /* pass */
            }
            /* pass */
            List_Token_append(tokens, _idtok);
            /* pass */
            Lexer_push_loc(self);
            /* pass */
            continue;
        }
        /* pass */
        Lexer_advance(self);
        /* pass */
        if ((c == 61LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_EqEq());
            } else if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_FatArrow());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Eq());
            }
        } else if ((c == 33LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_NotEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Bang());
            }
        } else if ((c == 60LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_LtEq());
            } else if ((nc == 60LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_LtLtEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_LtLt());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Lt());
            }
        } else if ((c == 62LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_GtEq());
            } else if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_GtGtEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_GtGt());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Gt());
            }
        } else if ((c == 43LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PlusEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Plus());
            }
        } else if ((c == 45LL)) {
            /* pass */
            long long nc = Lexer_peek(self);
            /* pass */
            if ((nc == 62LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_Arrow());
            } else if ((nc == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_MinusEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Minus());
            }
        } else if ((c == 42LL)) {
            /* pass */
            if ((Lexer_peek(self) == 42LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_StarStarEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_StarStar());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_StarEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Star());
            }
        } else if ((c == 47LL)) {
            /* pass */
            if ((Lexer_peek(self) == 47LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_FloorDivEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_FloorDiv());
                }
            } else if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_SlashEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Slash());
            }
        } else if ((c == 37LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PercentEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Percent());
            }
        } else if ((c == 46LL)) {
            /* pass */
            if ((Lexer_peek(self) == 46LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                if ((Lexer_peek(self) == 46LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDotDot());
                } else if ((Lexer_peek(self) == 61LL)) {
                    /* pass */
                    Lexer_advance(self);
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDotEq());
                } else {
                    /* pass */
                    List_Token_append(tokens, Token_make_DotDot());
                }
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Dot());
            }
        } else if ((c == 40LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LParen());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 41LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RParen());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 91LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LBracket());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 93LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RBracket());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 123LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_LBrace());
            /* pass */
            nesting = (nesting + 1LL);
        } else if ((c == 125LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_RBrace());
            /* pass */
            if ((nesting > 0LL)) {
                /* pass */
                nesting = (nesting - 1LL);
            }
        } else if ((c == 58LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Colon());
            /* pass */
            if ((((nesting > 0LL) && block_kw_seen) && Lexer__at_eol_after_ws(self))) {
                /* pass */
                List_i64_append(susp_nesting, nesting);
                /* pass */
                List_i64_append(susp_baseline, List_i64_get(self->indent_stack, (self->indent_stack->len - 1LL)));
                /* pass */
                nesting = 0LL;
            }
            /* pass */
            block_kw_seen = false;
        } else if ((c == 44LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Comma());
        } else if ((c == 59LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Semicolon());
        } else if ((c == 64LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_At());
        } else if ((c == 63LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Question());
        } else if ((c == 126LL)) {
            /* pass */
            List_Token_append(tokens, Token_make_Tilde());
        } else if ((c == 38LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_AmpEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Amp());
            }
        } else if ((c == 124LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_PipeEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Pipe());
            }
        } else if ((c == 94LL)) {
            /* pass */
            if ((Lexer_peek(self) == 61LL)) {
                /* pass */
                Lexer_advance(self);
                /* pass */
                List_Token_append(tokens, Token_make_CaretEq());
            } else {
                /* pass */
                List_Token_append(tokens, Token_make_Caret());
            }
        } else {
            /* pass */
            Lexer_advance(self);
            /* pass */
            List_Token_append(tokens, Token_ctor_Error(_tr_str_lit("?")));
        }
        /* pass */
        Lexer_push_loc(self);
    }
    /* pass */
    self->tok_col = 1LL;
    /* pass */
    while ((self->indent_stack->len > 1LL)) {
        /* pass */
        self->indent_stack->len = (self->indent_stack->len - 1LL);
        /* pass */
        List_Token_append(tokens, Token_make_Dedent());
        /* pass */
        Lexer_push_loc(self);
    }
    /* pass */
    List_Token_append(tokens, Token_make_Eof());
    /* pass */
    Lexer_push_loc(self);
    /* pass */
    List_i64_free(susp_nesting);
    List_i64_free(susp_baseline);
    return tokens;
}

__attribute__((hot)) Token keyword_to_token(TrStr s) {
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("def"))) == 0)) {
        /* pass */
        return Token_make_KwDef();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("class"))) == 0)) {
        /* pass */
        return Token_make_KwClass();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("enum"))) == 0)) {
        /* pass */
        return Token_make_KwEnum();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("interface"))) == 0)) {
        /* pass */
        return Token_make_KwInterface();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("extend"))) == 0)) {
        /* pass */
        return Token_make_KwExtend();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("struct"))) == 0)) {
        /* pass */
        return Token_make_KwStruct();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("if"))) == 0)) {
        /* pass */
        return Token_make_KwIf();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("elif"))) == 0)) {
        /* pass */
        return Token_make_KwElif();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("else"))) == 0)) {
        /* pass */
        return Token_make_KwElse();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("for"))) == 0)) {
        /* pass */
        return Token_make_KwFor();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("while"))) == 0)) {
        /* pass */
        return Token_make_KwWhile();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("loop"))) == 0)) {
        /* pass */
        return Token_make_KwLoop();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("return"))) == 0)) {
        /* pass */
        return Token_make_KwReturn();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("break"))) == 0)) {
        /* pass */
        return Token_make_KwBreak();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("continue"))) == 0)) {
        /* pass */
        return Token_make_KwContinue();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("pass"))) == 0)) {
        /* pass */
        return Token_make_KwPass();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("match"))) == 0)) {
        /* pass */
        return Token_make_KwMatch();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("case"))) == 0)) {
        /* pass */
        return Token_make_KwCase();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("try"))) == 0)) {
        /* pass */
        return Token_make_KwTry();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("except"))) == 0)) {
        /* pass */
        return Token_make_KwExcept();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("finally"))) == 0)) {
        /* pass */
        return Token_make_KwFinally();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("raise"))) == 0)) {
        /* pass */
        return Token_make_KwRaise();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("assert"))) == 0)) {
        /* pass */
        return Token_make_KwAssert();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("with"))) == 0)) {
        /* pass */
        return Token_make_KwWith();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("defer"))) == 0)) {
        /* pass */
        return Token_make_KwDefer();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("asm"))) == 0)) {
        /* pass */
        return Token_make_KwAsm();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("taskgroup"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("task_group"))) == 0))) {
        /* pass */
        return Token_make_KwTaskGroup();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("implements"))) == 0)) {
        /* pass */
        return Token_make_KwImplements();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("import"))) == 0)) {
        /* pass */
        return Token_make_KwImport();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("from"))) == 0)) {
        /* pass */
        return Token_make_KwFrom();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("as"))) == 0)) {
        /* pass */
        return Token_make_KwAs();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("in"))) == 0)) {
        /* pass */
        return Token_make_KwIn();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("mut"))) == 0)) {
        /* pass */
        return Token_make_KwMut();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("shared"))) == 0)) {
        /* pass */
        return Token_make_KwShared();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("extern"))) == 0)) {
        /* pass */
        return Token_make_KwExtern();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("unsafe"))) == 0)) {
        /* pass */
        return Token_make_KwUnsafe();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("spawn"))) == 0)) {
        /* pass */
        return Token_make_KwSpawn();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("throws"))) == 0)) {
        /* pass */
        return Token_make_KwThrows();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("extends"))) == 0)) {
        /* pass */
        return Token_make_KwExtends();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("async"))) == 0)) {
        /* pass */
        return Token_make_KwAsync();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("await"))) == 0)) {
        /* pass */
        return Token_make_KwAwait();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("yield"))) == 0)) {
        /* pass */
        return Token_make_KwYield();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("pub"))) == 0)) {
        /* pass */
        return Token_make_KwPub();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("static"))) == 0)) {
        /* pass */
        return Token_make_KwStatic();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("const"))) == 0)) {
        /* pass */
        return Token_make_KwConst();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("actor"))) == 0)) {
        /* pass */
        return Token_make_KwActor();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("super"))) == 0)) {
        /* pass */
        return Token_make_KwSuper();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("export"))) == 0)) {
        /* pass */
        return Token_make_KwExport();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("lambda"))) == 0)) {
        /* pass */
        return Token_make_KwLambda();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("decorator"))) == 0)) {
        /* pass */
        return Token_make_KwDecorator();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("macro"))) == 0)) {
        /* pass */
        return Token_make_KwMacro();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("do"))) == 0)) {
        /* pass */
        return Token_make_KwDo();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("sizeof"))) == 0)) {
        /* pass */
        return Token_make_KwSizeOf();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("is"))) == 0)) {
        /* pass */
        return Token_make_KwIs();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("true"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("True"))) == 0))) {
        /* pass */
        return Token_make_KwTrue();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("false"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("False"))) == 0))) {
        /* pass */
        return Token_make_KwFalse();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("none"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("None"))) == 0))) {
        /* pass */
        return Token_make_KwNone();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("and"))) == 0)) {
        /* pass */
        return Token_make_KwAnd();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("or"))) == 0)) {
        /* pass */
        return Token_make_KwOr();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("not"))) == 0)) {
        /* pass */
        return Token_make_KwNot();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("i64"))) == 0))) {
        /* pass */
        return Token_make_KwInt();
    }
    /* pass */
    if (((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("float"))) == 0) || (strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("f64"))) == 0))) {
        /* pass */
        return Token_make_KwFloat();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return Token_make_KwBool();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("char"))) == 0)) {
        /* pass */
        return Token_make_KwChar();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("str"))) == 0)) {
        /* pass */
        return Token_make_KwStr();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("String"))) == 0)) {
        /* pass */
        return Token_make_KwString();
    }
    /* pass */
    if ((strcmp(_tr_strz(s), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        return Token_make_KwVoid();
    }
    /* pass */
    return Token_ctor_Ident(s);
}

__attribute__((hot)) bool char_is_digit(long long c) {
    /* pass */
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool char_is_alpha(long long c) {
    /* pass */
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool char_is_alnum(long long c) {
    /* pass */
    return (char_is_alpha(c) || char_is_digit(c));
}

__attribute__((hot)) bool char_is_space(long long c) {
    /* pass */
    return ((c == 32LL) || (c == 9LL));
}

__attribute__((hot)) bool char_is_newline(long long c) {
    /* pass */
    return ((c == 10LL) || (c == 13LL));
}

__attribute__((hot)) bool char_is_hex(long long c) {
    /* pass */
    return ((char_is_digit(c) || ((c >= 65LL) && (c <= 70LL))) || ((c >= 97LL) && (c <= 102LL)));
}

__attribute__((hot)) bool _peek_next_line_dot(char* src, long long pos, long long src_len) {
    /* pass */
    long long i = pos;
    /* pass */
    while ((i < src_len)) {
        /* pass */
        long long c = ((long long)((*(src + i))));
        /* pass */
        if (((c == 32LL) || (c == 9LL))) {
            /* pass */
            i = (i + 1LL);
        } else if ((c == 35LL)) {
            /* pass */
            while (((i < src_len) && (((long long)((*(src + i)))) != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
        } else if ((c == 10LL)) {
            /* pass */
            i = (i + 1LL);
        } else if ((c == 13LL)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            if (((i < src_len) && (((long long)((*(src + i)))) == 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
        } else {
            /* pass */
            return (c == 46LL);
        }
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _last_tok_is_dot(List_Token* tokens) {
    /* pass */
    if ((tokens->len == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    __auto_type _t17 = List_Token_get(tokens, (tokens->len - 1LL));
    if (_t17.tag == Token_Dot) {
        /* pass */
        if ((tokens->len < 2LL)) {
            /* pass */
            return false;
        }
        /* pass */
        __auto_type _t18 = List_Token_get(tokens, (tokens->len - 2LL));
        if (_t18.tag == Token_IntLit) {
            __auto_type _ = _t18.data.IntLit.val;
            return false;
        } else if (_t18.tag == Token_FloatLit) {
            __auto_type _ = _t18.data.FloatLit.val;
            return false;
        } else if (1) {
            __auto_type _ = _t18;
            return true;
        }
    } else if (1) {
        __auto_type _ = _t17;
        return false;
    }
}

