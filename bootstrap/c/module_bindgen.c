#include "tauraro_types.h"

bool _is_alpha(long long c);
bool _is_digit(long long c);
bool _is_alnum(long long c);
bool _is_space(long long c);
TrStr map_base(TrStr words);
TrStr map_type(TrStr words, long long stars);
TrMap* _runtime_symbols();
bool _is_prim_type_word(TrStr w);
bool _is_ignored_word(TrStr w);
bool _is_decl_term(long long kind, TrStr text);
TrStr _join_words(List_TrStr* words);
long long _to_int(TrStr s);
long long _skip_gnu_attrs(List_ptr* toks, long long pos);
TrStr _scan_typedef_name(List_ptr* toks, long long pos);
bool _typedef_is_fnptr(List_ptr* toks, long long pos);
TrStr _scan_fnptr_name(List_ptr* toks, long long pos);
bool _is_ident_byte(long long c);
TrStr _rename_word(TrStr text, TrStr old, TrStr new_);
TrStr _basename(TrStr p);
TrStr _marker_file(TrStr line);
bool _marker_is_system(TrStr line);
TrStr _filter_to_target(TrStr raw, TrStr target);
TrStr _macro_name(TrStr rest);
TrMap* _load_baseline(TrStr cc);
TrMap* _target_define_names(TrStr header);
TrStr _lstrip(TrStr s);
bool _is_builtin_ty_name(TrStr n);
bool _is_libc_ty_name(TrStr n);
bool _is_system_record_ty(TrStr n);
TrStr _ident_at(TrStr text, long long start);
TrStr _opaque_fallbacks(TrStr body, TrMap* defined);
bool _is_single_string_literal(TrStr val);
bool _is_single_int_literal(TrStr val);
void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow);
TrStr _cxx_of(TrStr cc);
TrStr _macro_args(long long n, long long mode, long long atype, long long tp);
TrStr _ent_name(TrStr e);
long long _ent_arity(TrStr e);
long long _ent_typepos(TrStr e);
bool _is_ident_ch(long long c);
bool _param_ptr_type(TrStr body, TrStr pname);
bool _balanced_delims(TrStr s);
List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline);
bool _has_cc_error(TrStr errtxt);
TrMap* _macro_bad_names(TrStr errtxt);
TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp);
List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake);
TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrMap* _macro_remaining(List_TrStr* macros, TrMap* done);
void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros);
void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra);
TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra);
TrStr _cxxwalk_src();
void _rm_files(TrStr files);
TrStr _local_exe(TrStr stem);
TrStr _detect_libclang(TrStr cc);
CppType* _cpp_parse_type(TrStr spelling);
TrStr _last_seg(TrStr s);
TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member);
TrStr _cpp_ident(TrStr s);
bool _is_clean_ident(TrStr s);
bool _is_tr_keyword(TrStr n);
TrStr _uniq_sym(TrStr base_sym, TrMap* used);
TrStr _cpp_tr_pname(TrStr pname);
TrStr _cpp_ctype(CppType* t);
TrStr _cpp_tr_type(CppType* t);
List_TrStr* _cpp_ret(CppType* rt, TrStr call);
TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque);
TrStr _cpp_qual(TrStr base, TrMap* class_qual);
List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque);
TrStr _ns_pop(TrStr path);
TrStr _ns_us(TrStr path);
TrStr _rstrip_cr(TrStr s);
TrStr _c_to_cpp(TrStr cn);
List_TrStr* _desc4(TrStr desc);
List_TrStr* _parse_tclass(TrStr rest);
TrStr _stars(long long n);
TrStr _ptr_wrap(TrStr inner, long long n);
TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names);
TrStr _shim_body(TrStr ret_ctype, TrStr body);
TrStr _fnptr_cast(TrStr fnty);
void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs);
TrStr _cpp_detect_include_dirs(TrStr cc);
TrStr _cpp_std_flag(TrStr extra);
long long _cpp_fatal_count(TrStr diag);
void _cpp_print_diag(TrStr diag);
bool _cpp_ir_is_empty(TrStr ir);
void _cpp_cleanup();
bool _is_expr_proxy_spec(TrStr s);
List_TrStr* _collect_specs(TrStr ir);

__attribute__((malloc,returns_nonnull,hot)) CTok* CTok_init(long long kind, TrStr text) {
    /* pass */
    CTok* t = ((CTok*)_tr_obj_alloc(sizeof(CTok)));
    /* pass */
    t->kind = kind;
    /* pass */
    t->text = _tr_str_retain(text);
    /* pass */
    return t;
}

__attribute__((malloc,returns_nonnull,hot)) Bindgen* Bindgen_init(List_ptr* toks) {
    /* pass */
    Bindgen* b = ((Bindgen*)_tr_obj_alloc(sizeof(Bindgen)));
    /* pass */
    b->toks = toks;
    /* pass */
    b->pos = 0LL;
    /* pass */
    b->funcs = StringBuilder_init(1024LL);
    /* pass */
    b->structs = StringBuilder_init(1024LL);
    /* pass */
    b->types = StringBuilder_init(512LL);
    /* pass */
    b->consts = StringBuilder_init(512LL);
    /* pass */
    b->n_funcs = 0LL;
    /* pass */
    b->n_structs = 0LL;
    /* pass */
    b->seen = _tr_dict_new(64LL);
    /* pass */
    b->defined = (void*)List_TrStr_new();
    /* pass */
    b->skip_syms = _runtime_symbols();
    /* pass */
    b->n_skipped = 0LL;
    /* pass */
    return b;
}

__attribute__((hot)) bool Bindgen_fresh(Bindgen* self, TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) == 0) || _tr_dict_contains(self->seen, _tr_strz(name)))) {
        /* pass */
        return false;
    }
    /* pass */
    _tr_dict_set(self->seen, _tr_strz(name), true);
    /* pass */
    List_TrStr_append(self->defined, name);
    /* pass */
    return true;
}

__attribute__((hot)) TrStr Bindgen_ct(Bindgen* self) {
    /* pass */
    return _tr_str_retain(((CTok*)List_ptr_get(self->toks, self->pos))->text);
}

__attribute__((hot)) long long Bindgen_ck(Bindgen* self) {
    /* pass */
    return ((CTok*)List_ptr_get(self->toks, self->pos))->kind;
}

__attribute__((hot)) void Bindgen_adv(Bindgen* self) {
    /* pass */
    if ((self->pos < (self->toks->len - 1LL))) {
        /* pass */
        self->pos = (self->pos + 1LL);
    }
}

__attribute__((hot)) bool Bindgen_is_punct(Bindgen* self, TrStr p) {
    /* pass */
    return ((Bindgen_ck(self) == 4LL) && (strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(p)) == 0));
}

__attribute__((hot)) long long Bindgen_nk(Bindgen* self) {
    /* pass */
    if (((self->pos + 1LL) < self->toks->len)) {
        /* pass */
        return ((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->kind;
    }
    /* pass */
    return 5LL;
}

__attribute__((hot)) TrStr Bindgen_nt(Bindgen* self) {
    /* pass */
    if (((self->pos + 1LL) < self->toks->len)) {
        /* pass */
        return _tr_str_retain(((CTok*)List_ptr_get(self->toks, (self->pos + 1LL)))->text);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) void Bindgen_skip_balanced_parens(Bindgen* self) {
    /* pass */
    if ((!Bindgen_is_punct(self, _tr_str_lit("(")))) {
        /* pass */
        return;
    }
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("("))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                return;
            }
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_struct_body(Bindgen* self) {
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
        /* pass */
        long long depth = 0LL;
        /* pass */
        while ((Bindgen_ck(self) != 5LL)) {
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                depth = (depth + 1LL);
            } else if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
                /* pass */
                depth = (depth - 1LL);
                /* pass */
                Bindgen_adv(self);
                /* pass */
                if ((depth == 0LL)) {
                    /* pass */
                    break;
                }
                /* pass */
                continue;
            }
            /* pass */
            Bindgen_adv(self);
        }
    }
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_to_semi(Bindgen* self) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
            /* pass */
            depth = (depth - 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit(";")) && (depth <= 0LL))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_braces(Bindgen* self) {
    /* pass */
    if ((!Bindgen_is_punct(self, _tr_str_lit("{")))) {
        /* pass */
        return;
    }
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                return;
            }
            /* pass */
            continue;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_attributes(Bindgen* self) {
    /* pass */
    while (((Bindgen_ck(self) == 0LL) && ((strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("__attribute__"))) == 0) || (strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("__declspec"))) == 0)))) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        Bindgen_skip_balanced_parens(self);
    }
}

__attribute__((hot)) bool Bindgen_func_def_follows(Bindgen* self) {
    /* pass */
    long long save = self->pos;
    /* pass */
    Bindgen_skip_balanced_parens(self);
    /* pass */
    bool isdef = Bindgen_is_punct(self, _tr_str_lit("{"));
    /* pass */
    self->pos = save;
    /* pass */
    return isdef;
}

__attribute__((hot)) List_TrStr* Bindgen_read_type_words(Bindgen* self) {
    /* pass */
    List_TrStr* words = (void*)List_TrStr_new();
    /* pass */
    long long stars = 0LL;
    /* pass */
    bool going = true;
    /* pass */
    while (going) {
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr w = Bindgen_ct(self);
            /* pass */
            if (_is_ignored_word(w)) {
                /* pass */
                Bindgen_adv(self);
            } else if (((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__declspec"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__attribute__"))) == 0))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                Bindgen_skip_balanced_parens(self);
            } else if ((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("union"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("enum"))) == 0))) {
                /* pass */
                Bindgen_adv(self);
            } else if (({ TrStr _at_t802 = (Bindgen_nt(self)); __auto_type _wr = (((((words->len >= 1LL) || (stars > 0LL)) && _is_decl_term(Bindgen_nk(self), _at_t802)) && (!_is_prim_type_word(w)))); _tr_str_release(_at_t802); _wr; })) {
                /* pass */
                going = false;
            } else {
                /* pass */
                List_TrStr_append(words, w);
                /* pass */
                Bindgen_adv(self);
            }
        } else if (Bindgen_is_punct(self, _tr_str_lit("*"))) {
            /* pass */
            stars = (stars + 1LL);
            /* pass */
            Bindgen_adv(self);
        } else {
            /* pass */
            going = false;
        }
    }
    /* pass */
    List_TrStr* res = (void*)List_TrStr_new();
    /* pass */
    ({ TrStr _at_t803 = (_join_words(words)); List_TrStr_append(res, _at_t803); _tr_str_release(_at_t803); });
    /* pass */
    ({ TrStr _at_t804 = (_tr_str_wrap(_tr_int_to_str((long long)(stars)))); List_TrStr_append(res, _at_t804); _tr_str_release(_at_t804); });
    /* pass */
    return res;
}

__attribute__((hot)) void Bindgen_emit_func(Bindgen* self, TrStr ret_words, long long ret_stars, TrStr name) {
    /* pass */
    if (_tr_dict_contains(self->skip_syms, _tr_strz(name))) {
        /* pass */
        self->n_skipped = (self->n_skipped + 1LL);
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        return;
    }
    /* pass */
    ({ TrStr _sbt_t805 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->funcs, _sbt_t805); _tr_str_release(_sbt_t805); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    bool first = true;
    /* pass */
    long long argn = 0LL;
    /* pass */
    bool variadic = false;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(")"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long ploop_before = self->pos;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("."))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            variadic = true;
            /* pass */
            continue;
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(","))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            continue;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr pbase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long pstars = ({ TrStr _at_t806 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t806)); _tr_str_release(_at_t806); _wr; });
        /* pass */
        TrStr pname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("(")) && (strcmp(_tr_strz(Bindgen_nt(self)), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t807 = Bindgen_ct(self);
                _tr_str_release(pname);
                pname = _strtmp_t807;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t808 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
                _tr_str_release(pname);
                pname = _strtmp_t808;
            }
            /* pass */
            if (_is_tr_keyword(pname)) {
                /* pass */
                TrStr _strtmp_t809 = _tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit("_")));
                _tr_str_release(pname);
                pname = _strtmp_t809;
            }
            /* pass */
            if ((!first)) {
                /* pass */
                StringBuilder_append(self->funcs, _tr_str_lit(", "));
            }
            /* pass */
            first = false;
            /* pass */
            ({ TrStr _sbt_t810 = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": Pointer[void]")))); StringBuilder_append(self->funcs, _sbt_t810); _tr_str_release(_sbt_t810); });
            /* pass */
            argn = (argn + 1LL);
            /* pass */
            if ((self->pos == ploop_before)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t811 = Bindgen_ct(self);
            _tr_str_release(pname);
            pname = _strtmp_t811;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        while (Bindgen_is_punct(self, _tr_str_lit("["))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (((!Bindgen_is_punct(self, _tr_str_lit("]"))) && (Bindgen_ck(self) != 5LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("]"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            pstars = (pstars + 1LL);
        }
        /* pass */
        if ((((strcmp(_tr_strz(pbase), _tr_strz(_tr_str_lit("void"))) == 0) && (pstars == 0LL)) && (strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t812 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(argn)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("arg")), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(pname);
            pname = _strtmp_t812;
        }
        /* pass */
        if (_is_tr_keyword(pname)) {
            /* pass */
            TrStr _strtmp_t813 = _tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit("_")));
            _tr_str_release(pname);
            pname = _strtmp_t813;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            StringBuilder_append(self->funcs, _tr_str_lit(", "));
        }
        /* pass */
        first = false;
        /* pass */
        ({ TrStr _sbt_t814 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit(": ")))); TrStr _cr = (map_type(pbase, pstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); StringBuilder_append(self->funcs, _sbt_t814); _tr_str_release(_sbt_t814); });
        /* pass */
        argn = (argn + 1LL);
        /* pass */
        if ((self->pos == ploop_before)) {
            /* pass */
            Bindgen_adv(self);
        }
        List_TrStr_free(tw);
        _tr_str_release(pbase);
        _tr_str_release(pname);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (variadic) {
        /* pass */
        StringBuilder_append(self->funcs, _tr_str_lit(", args..."));
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit(")"));
    /* pass */
    TrStr rt = map_type(ret_words, ret_stars);
    /* pass */
    if ((strcmp(_tr_strz(rt), _tr_strz(_tr_str_lit("void"))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t815 = (_tr_strx_concat(_tr_strz(_tr_str_lit(" -> ")), _tr_strz(rt))); StringBuilder_append(self->funcs, _sbt_t815); _tr_str_release(_sbt_t815); });
    }
    /* pass */
    StringBuilder_append(self->funcs, _tr_str_lit("\n"));
    /* pass */
    self->n_funcs = (self->n_funcs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
        /* pass */
        Bindgen_adv(self);
    }
    _tr_str_release(rt);
}

__attribute__((hot)) void Bindgen_emit_struct(Bindgen* self, TrStr name, bool is_union) {
    /* pass */
    if ((!Bindgen_fresh(self, name))) {
        /* pass */
        Bindgen_skip_struct_body(self);
        /* pass */
        return;
    }
    /* pass */
    if (is_union) {
        /* pass */
        StringBuilder_append(self->structs, _tr_str_lit("@union\n"));
    }
    /* pass */
    ({ TrStr _sbt_t816 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@value_type\nclass ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t816); _tr_str_release(_sbt_t816); });
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long nfields = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit("}"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        long long floop_before = self->pos;
        /* pass */
        if (((strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("union"))) == 0))) {
            /* pass */
            long long nsave = self->pos;
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                List_TrStr* nnames = (void*)List_TrStr_new();
                /* pass */
                List_TrStr* ntypes = (void*)List_TrStr_new();
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit("}"))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    long long nb = self->pos;
                    /* pass */
                    if (((strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(Bindgen_ct(self)), _tr_strz(_tr_str_lit("union"))) == 0))) {
                        /* pass */
                        Bindgen_adv(self);
                        /* pass */
                        if ((Bindgen_ck(self) == 0LL)) {
                            /* pass */
                            Bindgen_adv(self);
                        }
                        /* pass */
                        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                            /* pass */
                            Bindgen_skip_struct_body(self);
                            /* pass */
                            if ((self->pos == nb)) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            continue;
                        }
                        /* pass */
                        self->pos = nb;
                    }
                    /* pass */
                    List_TrStr* ntw = Bindgen_read_type_words(self);
                    /* pass */
                    TrStr nbase = List_TrStr_get(ntw, 0LL);
                    /* pass */
                    long long nstars = ({ TrStr _at_t817 = (List_TrStr_get(ntw, 1LL)); __auto_type _wr = (_to_int(_at_t817)); _tr_str_release(_at_t817); _wr; });
                    /* pass */
                    bool nmore = true;
                    /* pass */
                    while (nmore) {
                        /* pass */
                        if ((Bindgen_ck(self) != 0LL)) {
                            /* pass */
                            break;
                        }
                        /* pass */
                        TrStr nnm = Bindgen_ct(self);
                        /* pass */
                        Bindgen_adv(self);
                        /* pass */
                        TrStr narr = _tr_str_lit("");
                        /* pass */
                        bool nisarr = false;
                        /* pass */
                        while (Bindgen_is_punct(self, _tr_str_lit("["))) {
                            /* pass */
                            Bindgen_adv(self);
                            /* pass */
                            if ((Bindgen_ck(self) == 1LL)) {
                                /* pass */
                                TrStr _strtmp_t818 = Bindgen_ct(self);
                                _tr_str_release(narr);
                                narr = _strtmp_t818;
                            }
                            /* pass */
                            while (((!Bindgen_is_punct(self, _tr_str_lit("]"))) && (Bindgen_ck(self) != 5LL))) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            if (Bindgen_is_punct(self, _tr_str_lit("]"))) {
                                /* pass */
                                Bindgen_adv(self);
                            }
                            /* pass */
                            nisarr = true;
                        }
                        /* pass */
                        List_TrStr_append(nnames, nnm);
                        /* pass */
                        if ((nisarr && (strcmp(_tr_strz(narr), _tr_strz(_tr_str_lit(""))) != 0))) {
                            /* pass */
                            ({ TrStr _at_t819 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (map_type(nbase, nstars)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("[")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(narr)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(ntypes, _at_t819); _tr_str_release(_at_t819); });
                        } else {
                            /* pass */
                            ({ TrStr _at_t820 = (map_type(nbase, nstars)); List_TrStr_append(ntypes, _at_t820); _tr_str_release(_at_t820); });
                        }
                        /* pass */
                        if (Bindgen_is_punct(self, _tr_str_lit(","))) {
                            /* pass */
                            Bindgen_adv(self);
                        } else {
                            /* pass */
                            nmore = false;
                        }
                        _tr_str_release(nnm);
                        _tr_str_release(narr);
                    }
                    /* pass */
                    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    /* pass */
                    if ((self->pos == nb)) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    List_TrStr_free(ntw);
                    _tr_str_release(nbase);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                TrStr pfx = _tr_str_lit("");
                /* pass */
                if ((Bindgen_ck(self) == 0LL)) {
                    /* pass */
                    TrStr _strtmp_t821 = Bindgen_ct(self);
                    _tr_str_release(pfx);
                    pfx = _strtmp_t821;
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                long long nk = 0LL;
                /* pass */
                while ((nk < nnames->len)) {
                    /* pass */
                    TrStr nfn = List_TrStr_get(nnames, nk);
                    /* pass */
                    if ((strcmp(_tr_strz(pfx), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t822 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nfn)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(nfn);
                        nfn = _strtmp_t822;
                    }
                    /* pass */
                    ({ TrStr _sbt_t823 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(nfn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (List_TrStr_get(ntypes, nk)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t823); _tr_str_release(_sbt_t823); });
                    /* pass */
                    nfields = (nfields + 1LL);
                    /* pass */
                    nk = (nk + 1LL);
                    _tr_str_release(nfn);
                }
                /* pass */
                if ((self->pos == floop_before)) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                continue;
            }
            /* pass */
            self->pos = nsave;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr fbase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long fstars = ({ TrStr _at_t824 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t824)); _tr_str_release(_at_t824); _wr; });
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("(")) && (strcmp(_tr_strz(Bindgen_nt(self)), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            TrStr fpname = _tr_str_lit("");
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t825 = Bindgen_ct(self);
                _tr_str_release(fpname);
                fpname = _strtmp_t825;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            while (((!Bindgen_is_punct(self, _tr_str_lit(")"))) && (Bindgen_ck(self) != 5LL))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(")"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            if ((strcmp(_tr_strz(fpname), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                ({ TrStr _sbt_t826 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fpname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": Pointer[void]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t826); _tr_str_release(_sbt_t826); });
                /* pass */
                nfields = (nfields + 1LL);
            }
            /* pass */
            Bindgen_skip_attributes(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if ((self->pos == floop_before)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            continue;
        }
        /* pass */
        bool more = true;
        /* pass */
        bool first_decl = true;
        /* pass */
        while (more) {
            /* pass */
            long long dstars = fstars;
            /* pass */
            if ((!first_decl)) {
                /* pass */
                dstars = 0LL;
                /* pass */
                while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                    /* pass */
                    dstars = (dstars + 1LL);
                    /* pass */
                    Bindgen_adv(self);
                }
            }
            /* pass */
            first_decl = false;
            /* pass */
            if ((Bindgen_ck(self) != 0LL)) {
                /* pass */
                break;
            }
            /* pass */
            TrStr fname = Bindgen_ct(self);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            bool is_arr = false;
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit("["))) {
                /* pass */
                Bindgen_adv(self);
                /* pass */
                TrStr arrn = _tr_str_lit("");
                /* pass */
                if ((Bindgen_ck(self) == 1LL)) {
                    /* pass */
                    TrStr _strtmp_t827 = Bindgen_ct(self);
                    _tr_str_release(arrn);
                    arrn = _strtmp_t827;
                }
                /* pass */
                while (((!Bindgen_is_punct(self, _tr_str_lit("]"))) && (Bindgen_ck(self) != 5LL))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if (Bindgen_is_punct(self, _tr_str_lit("]"))) {
                    /* pass */
                    Bindgen_adv(self);
                }
                /* pass */
                if ((strcmp(_tr_strz(arrn), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ({ TrStr _sbt_t828 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": ["))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, dstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("; "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(arrn)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t828); _tr_str_release(_sbt_t828); });
                    /* pass */
                    nfields = (nfields + 1LL);
                    /* pass */
                    is_arr = true;
                }
                _tr_str_release(arrn);
            }
            /* pass */
            if ((!is_arr)) {
                /* pass */
                ({ TrStr _sbt_t829 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    pub ")), _tr_strz(fname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(fbase, dstars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t829); _tr_str_release(_sbt_t829); });
                /* pass */
                nfields = (nfields + 1LL);
            }
            /* pass */
            Bindgen_skip_attributes(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(","))) {
                /* pass */
                Bindgen_adv(self);
            } else {
                /* pass */
                more = false;
            }
            _tr_str_release(fname);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((self->pos == floop_before)) {
            /* pass */
            Bindgen_adv(self);
        }
        List_TrStr_free(tw);
        _tr_str_release(fbase);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if ((nfields == 0LL)) {
        /* pass */
        StringBuilder_append(self->structs, _tr_str_lit("    pass\n"));
    }
    /* pass */
    StringBuilder_append(self->structs, _tr_str_lit("\n"));
    /* pass */
    self->n_structs = (self->n_structs + 1LL);
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_skip_enum_expr(Bindgen* self) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("(")) || Bindgen_is_punct(self, _tr_str_lit("[")))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((Bindgen_is_punct(self, _tr_str_lit(")")) || Bindgen_is_punct(self, _tr_str_lit("]")))) {
            /* pass */
            depth = (depth - 1LL);
        } else if (((depth <= 0LL) && (Bindgen_is_punct(self, _tr_str_lit(",")) || Bindgen_is_punct(self, _tr_str_lit("}"))))) {
            /* pass */
            return;
        }
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_emit_enum(Bindgen* self, TrStr name) {
    /* pass */
    if (((strcmp(_tr_strz(name), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, name))) {
        /* pass */
        ({ TrStr _sbt_t830 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = c_int\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t830); _tr_str_release(_sbt_t830); });
    }
    /* pass */
    Bindgen_adv(self);
    /* pass */
    long long next_val = 0LL;
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit("}"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_skip_attributes(self);
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
            /* pass */
            break;
        }
        /* pass */
        if ((Bindgen_ck(self) != 0LL)) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            continue;
        }
        /* pass */
        TrStr ename = Bindgen_ct(self);
        /* pass */
        Bindgen_adv(self);
        /* pass */
        Bindgen_skip_attributes(self);
        /* pass */
        long long ev = next_val;
        /* pass */
        bool simple = true;
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("="))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 1LL)) {
                /* pass */
                ev = ({ TrStr _at_t831 = (Bindgen_ct(self)); __auto_type _wr = (_to_int(_at_t831)); _tr_str_release(_at_t831); _wr; });
                /* pass */
                Bindgen_adv(self);
                /* pass */
                if ((!(Bindgen_is_punct(self, _tr_str_lit(",")) || Bindgen_is_punct(self, _tr_str_lit("}"))))) {
                    /* pass */
                    simple = false;
                    /* pass */
                    Bindgen_skip_enum_expr(self);
                }
            } else {
                /* pass */
                simple = false;
                /* pass */
                Bindgen_skip_enum_expr(self);
            }
        }
        /* pass */
        if ((simple && Bindgen_fresh(self, ename))) {
            /* pass */
            ({ TrStr _sbt_t832 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ename))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ev)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->consts, _sbt_t832); _tr_str_release(_sbt_t832); });
            /* pass */
            next_val = (ev + 1LL);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit(","))) {
            /* pass */
            Bindgen_adv(self);
        }
        _tr_str_release(ename);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit("}"))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    while (((!Bindgen_is_punct(self, _tr_str_lit(";"))) && (Bindgen_ck(self) != 5LL))) {
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
        /* pass */
        Bindgen_adv(self);
    }
}

__attribute__((hot)) void Bindgen_parse_decl(Bindgen* self) {
    /* pass */
    if ((Bindgen_ck(self) == 5LL)) {
        /* pass */
        return;
    }
    /* pass */
    TrStr lead = Bindgen_ct(self);
    /* pass */
    if ((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("typedef"))) == 0)) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr agg = Bindgen_ct(self);
        /* pass */
        if (((strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("union"))) == 0))) {
            /* pass */
            bool is_u = (strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("union"))) == 0);
            /* pass */
            Bindgen_adv(self);
            /* pass */
            TrStr tag = _tr_str_lit("");
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t833 = Bindgen_ct(self);
                _tr_str_release(tag);
                tag = _strtmp_t833;
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                TrStr tname = _scan_typedef_name(self->toks, self->pos);
                /* pass */
                TrStr cname = _tr_str_retain(tname);
                /* pass */
                if ((strcmp(_tr_strz(cname), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t834 = _tr_str_retain(tag);
                    _tr_str_release(cname);
                    cname = _strtmp_t834;
                }
                /* pass */
                Bindgen_emit_struct(self, cname, is_u);
                /* pass */
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(tag);
                _tr_str_release(tname);
                _tr_str_release(cname);
                return;
            }
            /* pass */
            long long ostars = 0LL;
            /* pass */
            while (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                ostars = (ostars + 1LL);
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            TrStr oname = _tr_str_lit("");
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr _strtmp_t835 = Bindgen_ct(self);
                _tr_str_release(oname);
                oname = _strtmp_t835;
            }
            /* pass */
            if ((((ostars > 0LL) && (strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(tag), _tr_strz(_tr_str_lit(""))) != 0))) {
                /* pass */
                if (Bindgen_fresh(self, tag)) {
                    /* pass */
                    ({ TrStr _sbt_t836 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t836); _tr_str_release(_sbt_t836); });
                    /* pass */
                    self->n_structs = (self->n_structs + 1LL);
                }
                /* pass */
                if (Bindgen_fresh(self, oname)) {
                    /* pass */
                    ({ TrStr _sbt_t837 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer["))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tag)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t837); _tr_str_release(_sbt_t837); });
                }
            } else if (((strcmp(_tr_strz(oname), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, oname))) {
                /* pass */
                ({ TrStr _sbt_t838 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(oname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque handle\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t838); _tr_str_release(_sbt_t838); });
                /* pass */
                self->n_structs = (self->n_structs + 1LL);
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(tag);
            _tr_str_release(oname);
            return;
        }
        /* pass */
        if ((strcmp(_tr_strz(agg), _tr_strz(_tr_str_lit("enum"))) == 0)) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
                /* pass */
                TrStr ename = _scan_typedef_name(self->toks, self->pos);
                /* pass */
                Bindgen_emit_enum(self, ename);
                /* pass */
                _tr_str_release(lead);
                _tr_str_release(agg);
                _tr_str_release(ename);
                return;
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            return;
        }
        /* pass */
        if (_typedef_is_fnptr(self->toks, self->pos)) {
            /* pass */
            TrStr fpn = _scan_fnptr_name(self->toks, self->pos);
            /* pass */
            if (((strcmp(_tr_strz(fpn), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_fresh(self, fpn))) {
                /* pass */
                ({ TrStr _sbt_t839 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(fpn))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer[void]    # C function pointer\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t839); _tr_str_release(_sbt_t839); });
            }
            /* pass */
            Bindgen_skip_to_semi(self);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(agg);
            _tr_str_release(fpn);
            return;
        }
        /* pass */
        List_TrStr* tw = Bindgen_read_type_words(self);
        /* pass */
        TrStr abase = List_TrStr_get(tw, 0LL);
        /* pass */
        long long astars = ({ TrStr _at_t840 = (List_TrStr_get(tw, 1LL)); __auto_type _wr = (_to_int(_at_t840)); _tr_str_release(_at_t840); _wr; });
        /* pass */
        TrStr aname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t841 = Bindgen_ct(self);
            _tr_str_release(aname);
            aname = _strtmp_t841;
        }
        /* pass */
        if (((((strcmp(_tr_strz(aname), _tr_strz(_tr_str_lit(""))) != 0) && (strcmp(_tr_strz(abase), _tr_strz(_tr_str_lit(""))) != 0)) && (strcmp(_tr_strz(aname), _tr_strz(abase)) != 0)) && Bindgen_fresh(self, aname))) {
            /* pass */
            ({ TrStr _sbt_t842 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(aname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (map_type(abase, astars)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->types, _sbt_t842); _tr_str_release(_sbt_t842); });
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(agg);
        List_TrStr_free(tw);
        _tr_str_release(abase);
        _tr_str_release(aname);
        return;
    }
    /* pass */
    if (((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("struct"))) == 0) || (strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("union"))) == 0))) {
        /* pass */
        bool is_u2 = (strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("union"))) == 0);
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr tag2 = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t843 = Bindgen_ct(self);
            _tr_str_release(tag2);
            tag2 = _strtmp_t843;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((Bindgen_is_punct(self, _tr_str_lit("{")) && (strcmp(_tr_strz(tag2), _tr_strz(_tr_str_lit(""))) != 0))) {
            /* pass */
            Bindgen_emit_struct(self, tag2, is_u2);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(tag2);
            return;
        }
        /* pass */
        if (((Bindgen_is_punct(self, _tr_str_lit(";")) && (strcmp(_tr_strz(tag2), _tr_strz(_tr_str_lit(""))) != 0)) && Bindgen_fresh(self, tag2))) {
            /* pass */
            ({ TrStr _sbt_t844 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(tag2))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass    # opaque (forward-declared)\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(self->structs, _sbt_t844); _tr_str_release(_sbt_t844); });
            /* pass */
            self->n_structs = (self->n_structs + 1LL);
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(tag2);
        return;
    }
    /* pass */
    if ((strcmp(_tr_strz(lead), _tr_strz(_tr_str_lit("enum"))) == 0)) {
        /* pass */
        Bindgen_adv(self);
        /* pass */
        TrStr etag = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t845 = Bindgen_ct(self);
            _tr_str_release(etag);
            etag = _strtmp_t845;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if (Bindgen_is_punct(self, _tr_str_lit("{"))) {
            /* pass */
            Bindgen_emit_enum(self, etag);
            /* pass */
            _tr_str_release(lead);
            _tr_str_release(etag);
            return;
        }
        /* pass */
        Bindgen_skip_to_semi(self);
        /* pass */
        _tr_str_release(lead);
        _tr_str_release(etag);
        return;
    }
    /* pass */
    List_TrStr* rw = Bindgen_read_type_words(self);
    /* pass */
    TrStr rbase = List_TrStr_get(rw, 0LL);
    /* pass */
    long long rstars = ({ TrStr _at_t846 = (List_TrStr_get(rw, 1LL)); __auto_type _wr = (_to_int(_at_t846)); _tr_str_release(_at_t846); _wr; });
    /* pass */
    TrStr fname = _tr_str_lit("");
    /* pass */
    if ((Bindgen_ck(self) == 0LL)) {
        /* pass */
        TrStr _strtmp_t847 = Bindgen_ct(self);
        _tr_str_release(fname);
        fname = _strtmp_t847;
        /* pass */
        Bindgen_adv(self);
    }
    /* pass */
    if (((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit(""))) != 0) && Bindgen_is_punct(self, _tr_str_lit("(")))) {
        /* pass */
        if (Bindgen_func_def_follows(self)) {
            /* pass */
            Bindgen_skip_balanced_parens(self);
            /* pass */
            Bindgen_skip_braces(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
                /* pass */
                Bindgen_adv(self);
            }
            /* pass */
            _tr_str_release(lead);
            List_TrStr_free(rw);
            _tr_str_release(rbase);
            _tr_str_release(fname);
            return;
        }
        /* pass */
        Bindgen_emit_func(self, rbase, rstars, fname);
        /* pass */
        _tr_str_release(lead);
        List_TrStr_free(rw);
        _tr_str_release(rbase);
        _tr_str_release(fname);
        return;
    }
    /* pass */
    if (((strcmp(_tr_strz(fname), _tr_strz(_tr_str_lit(""))) == 0) && Bindgen_is_punct(self, _tr_str_lit("(")))) {
        /* pass */
        long long saved = self->pos;
        /* pass */
        Bindgen_adv(self);
        /* pass */
        long long pdstars = 0LL;
        /* pass */
        bool scan = true;
        /* pass */
        while (scan) {
            /* pass */
            if ((Bindgen_ck(self) == 0LL)) {
                /* pass */
                TrStr dw = Bindgen_ct(self);
                /* pass */
                if (((strcmp(_tr_strz(dw), _tr_strz(_tr_str_lit("__attribute__"))) == 0) || (strcmp(_tr_strz(dw), _tr_strz(_tr_str_lit("__declspec"))) == 0))) {
                    /* pass */
                    Bindgen_adv(self);
                    /* pass */
                    Bindgen_skip_balanced_parens(self);
                } else if (_is_ignored_word(dw)) {
                    /* pass */
                    Bindgen_adv(self);
                } else {
                    /* pass */
                    scan = false;
                }
            } else if (Bindgen_is_punct(self, _tr_str_lit("*"))) {
                /* pass */
                pdstars = (pdstars + 1LL);
                /* pass */
                Bindgen_adv(self);
            } else {
                /* pass */
                scan = false;
            }
        }
        /* pass */
        TrStr pdname = _tr_str_lit("");
        /* pass */
        if ((Bindgen_ck(self) == 0LL)) {
            /* pass */
            TrStr _strtmp_t848 = Bindgen_ct(self);
            _tr_str_release(pdname);
            pdname = _strtmp_t848;
            /* pass */
            Bindgen_adv(self);
        }
        /* pass */
        if ((((strcmp(_tr_strz(pdname), _tr_strz(_tr_str_lit(""))) != 0) && (pdstars == 0LL)) && Bindgen_is_punct(self, _tr_str_lit(")")))) {
            /* pass */
            Bindgen_adv(self);
            /* pass */
            if (Bindgen_is_punct(self, _tr_str_lit("("))) {
                /* pass */
                if (Bindgen_func_def_follows(self)) {
                    /* pass */
                    Bindgen_skip_balanced_parens(self);
                    /* pass */
                    Bindgen_skip_braces(self);
                    /* pass */
                    if (Bindgen_is_punct(self, _tr_str_lit(";"))) {
                        /* pass */
                        Bindgen_adv(self);
                    }
                    /* pass */
                    _tr_str_release(lead);
                    List_TrStr_free(rw);
                    _tr_str_release(rbase);
                    _tr_str_release(fname);
                    _tr_str_release(pdname);
                    return;
                }
                /* pass */
                Bindgen_emit_func(self, rbase, rstars, pdname);
                /* pass */
                _tr_str_release(lead);
                List_TrStr_free(rw);
                _tr_str_release(rbase);
                _tr_str_release(fname);
                _tr_str_release(pdname);
                return;
            }
        }
        /* pass */
        self->pos = saved;
    }
    /* pass */
    Bindgen_skip_to_semi(self);
    _tr_str_release(lead);
    List_TrStr_free(rw);
    _tr_str_release(rbase);
    _tr_str_release(fname);
}

__attribute__((hot)) void Bindgen_run(Bindgen* self) {
    /* pass */
    while ((Bindgen_ck(self) != 5LL)) {
        /* pass */
        long long before = self->pos;
        /* pass */
        Bindgen_parse_decl(self);
        /* pass */
        if ((self->pos == before)) {
            /* pass */
            Bindgen_adv(self);
        }
    }
}

__attribute__((hot)) bool _is_alpha(long long c) {
    /* pass */
    return ((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _is_digit(long long c) {
    /* pass */
    return ((c >= 48LL) && (c <= 57LL));
}

__attribute__((hot)) bool _is_alnum(long long c) {
    /* pass */
    return (_is_alpha(c) || _is_digit(c));
}

__attribute__((hot)) bool _is_space(long long c) {
    /* pass */
    return ((((c == 32LL) || (c == 9LL)) || (c == 10LL)) || (c == 13LL));
}

__attribute__((hot)) List_ptr* tokenize_c(TrStr src) {
    /* pass */
    List_ptr* toks = (void*)List_ptr_new();
    /* pass */
    long long n = _tr_strlen(_tr_strz(src));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(src)[i];
        /* pass */
        if (_is_space(c)) {
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 47LL))) {
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((((c == 47LL) && ((i + 1LL) < n)) && (_tr_strz(src)[(i + 1LL)] == 42LL))) {
            /* pass */
            i = (i + 2LL);
            /* pass */
            while ((((i + 1LL) < n) && (!((_tr_strz(src)[i] == 42LL) && (_tr_strz(src)[(i + 1LL)] == 47LL))))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 2LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 35LL)) {
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 10LL))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_is_alpha(c)) {
            /* pass */
            long long start = i;
            /* pass */
            while (((i < n) && _is_alnum(_tr_strz(src)[i]))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            ({ TrStr _at_t849 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(0LL, _at_t849)); _tr_str_release(_at_t849); });
            /* pass */
            continue;
        }
        /* pass */
        if (_is_digit(c)) {
            /* pass */
            long long start = i;
            /* pass */
            while (((i < n) && ((_is_alnum(_tr_strz(src)[i]) || (_tr_strz(src)[i] == 46LL)) || (_tr_strz(src)[i] == 120LL)))) {
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            ({ TrStr _at_t850 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(1LL, _at_t850)); _tr_str_release(_at_t850); });
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 34LL)) {
            /* pass */
            long long start = i;
            /* pass */
            i = (i + 1LL);
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 34LL))) {
                /* pass */
                if ((_tr_strz(src)[i] == 92LL)) {
                    /* pass */
                    i = (i + 1LL);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t851 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(2LL, _at_t851)); _tr_str_release(_at_t851); });
            /* pass */
            continue;
        }
        /* pass */
        if ((c == 39LL)) {
            /* pass */
            long long start = i;
            /* pass */
            i = (i + 1LL);
            /* pass */
            while (((i < n) && (_tr_strz(src)[i] != 39LL))) {
                /* pass */
                if ((_tr_strz(src)[i] == 92LL)) {
                    /* pass */
                    i = (i + 1LL);
                }
                /* pass */
                i = (i + 1LL);
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            ({ TrStr _at_t852 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), start, i))); List_ptr_append(toks, CTok_init(3LL, _at_t852)); _tr_str_release(_at_t852); });
            /* pass */
            continue;
        }
        /* pass */
        ({ TrStr _at_t853 = (_tr_str_wrap(_tr_str_slice(_tr_strz(src), i, (i + 1LL)))); List_ptr_append(toks, CTok_init(4LL, _at_t853)); _tr_str_release(_at_t853); });
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_ptr_append(toks, CTok_init(5LL, _tr_str_lit("")));
    /* pass */
    return toks;
}

__attribute__((hot)) TrStr map_base(TrStr words) {
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_int");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned int"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_uint");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_char");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uchar");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed char"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_schar");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("short"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("short int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed short"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_short");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned short"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned short int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ushort");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed long"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_long");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ulong");
    }
    /* pass */
    if ((((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long long int"))) == 0)) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("signed long long"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_longlong");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long long"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("unsigned long long int"))) == 0))) {
        /* pass */
        return _tr_str_lit("c_ulonglong");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("va_list"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("__builtin_va_list"))) == 0))) {
        /* pass */
        return _tr_str_lit("Pointer[void]");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("float"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_float");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("double"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_double");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("long double"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ldouble");
    }
    /* pass */
    if (((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("_Bool"))) == 0) || (strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("bool"))) == 0))) {
        /* pass */
        return _tr_str_lit("bool");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("size_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_size_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("ssize_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ssize_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int8_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int16_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int32_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("int64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_int64_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint8_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint16_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint32_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uint64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uint64_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("intptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_intptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("uintptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_uintptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(words), _tr_strz(_tr_str_lit("ptrdiff_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("c_ptrdiff_t");
    }
    /* pass */
    return _tr_str_retain(words);
}

__attribute__((hot)) TrStr map_type(TrStr words, long long stars) {
    /* pass */
    TrStr base = map_base(words);
    /* pass */
    if ((stars <= 0LL)) {
        /* pass */
        return base;
    }
    /* pass */
    TrStr inner = _tr_str_retain(base);
    /* pass */
    if ((strcmp(_tr_strz(base), _tr_strz(_tr_str_lit("c_char"))) == 0)) {
        /* pass */
        TrStr _strtmp_t854 = _tr_str_lit("char");
        _tr_str_release(inner);
        inner = _strtmp_t854;
    }
    /* pass */
    if ((strcmp(_tr_strz(base), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        TrStr _strtmp_t855 = _tr_str_lit("void");
        _tr_str_release(inner);
        inner = _strtmp_t855;
    }
    /* pass */
    TrStr ty = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(inner))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
    /* pass */
    long long d = 1LL;
    /* pass */
    while ((d < stars)) {
        /* pass */
        TrStr _strtmp_t856 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t856;
        /* pass */
        d = (d + 1LL);
    }
    /* pass */
    _tr_str_release(base);
    _tr_str_release(inner);
    return ty;
}

__attribute__((hot)) TrMap* _runtime_symbols() {
    /* pass */
    TrStr names = _tr_str_lit("printf fprintf sprintf snprintf vprintf vfprintf vsprintf vsnprintf scanf fscanf sscanf fopen fclose fread fwrite fseek ftell fgets fputs fgetc fputc getc putc getchar putchar puts perror fflush freopen setvbuf setbuf rewind remove rename tmpfile ungetc feof ferror clearerr ");
    /* pass */
    TrStr _strtmp_t857 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("malloc calloc realloc free aligned_alloc abort exit _Exit atexit quick_exit system getenv setenv unsetenv abs labs llabs atoi atol atoll atof strtol strtoul strtoll strtoull strtod strtof rand srand qsort bsearch div ldiv lldiv ")));
    _tr_str_release(names);
    names = _strtmp_t857;
    /* pass */
    TrStr _strtmp_t858 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("memcpy memmove memset memcmp memchr strcpy strncpy strcat strncat strcmp strncmp strchr strrchr strstr strlen strnlen strdup strndup strtok strspn strcspn strpbrk strerror strcoll strxfrm strcasecmp strncasecmp ")));
    _tr_str_release(names);
    names = _strtmp_t858;
    /* pass */
    TrStr _strtmp_t859 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("sqrt sin cos tan asin acos atan atan2 sinh cosh tanh exp exp2 log log2 log10 pow cbrt hypot floor ceil round trunc fabs fmod ldexp frexp modf fmin fmax copysign nextafter nan isnan isinf signbit ")));
    _tr_str_release(names);
    names = _strtmp_t859;
    /* pass */
    TrStr _strtmp_t860 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("read write open close lseek unlink stat fstat mkdir rmdir access dup dup2 pipe fork execve waitpid kill getpid ")));
    _tr_str_release(names);
    names = _strtmp_t860;
    /* pass */
    TrStr _strtmp_t861 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("CreateWindow CloseWindow CreateWindowExA CreateWindowExW GetMessage DispatchMessage MessageBox MessageBoxA MessageBoxW ")));
    _tr_str_release(names);
    names = _strtmp_t861;
    /* pass */
    TrStr _strtmp_t862 = _tr_strx_concat(_tr_strz(names), _tr_strz(_tr_str_lit("Rectangle Ellipse Polygon Polyline Arc Chord Pie RoundRect LineTo MoveToEx FillRect FrameRect InvertRect DrawIcon DrawText DrawTextEx TextOut GetObject LoadImage CreateFont PlaySound Polygon PolyBezier ")));
    _tr_str_release(names);
    names = _strtmp_t862;
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(names), _tr_strz(_tr_str_lit(" ")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(parts, i)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            ({ TrStr _dkt_t863 = (List_TrStr_get(parts, i)); _tr_dict_set(m, _tr_strz(_dkt_t863), true); _tr_str_release(_dkt_t863); });
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    _tr_str_release(names);
    List_TrStr_free(parts);
    return m;
}

__attribute__((hot)) bool _is_prim_type_word(TrStr w) {
    /* pass */
    return (((((((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("short"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("int"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("long"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("double"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("signed"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("unsigned"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("_Bool"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("bool"))) == 0));
}

__attribute__((hot)) bool _is_ignored_word(TrStr w) {
    /* pass */
    return (((((((((((((((((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("const"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("volatile"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("extern"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("static"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("inline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("register"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("auto"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__inline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__inline__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__forceinline"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("restrict"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__restrict"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__restrict__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__cdecl"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__stdcall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__fastcall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__thiscall"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("WINAPI"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("APIENTRY"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("__extension__"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("_Noreturn"))) == 0));
}

__attribute__((hot)) bool _is_decl_term(long long kind, TrStr text) {
    /* pass */
    if ((kind == 5LL)) {
        /* pass */
        return true;
    }
    /* pass */
    if ((kind != 4LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return (((((((strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(","))) == 0) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(";"))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit(")"))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("["))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("="))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("("))) == 0)) || (strcmp(_tr_strz(text), _tr_strz(_tr_str_lit("}"))) == 0));
}

__attribute__((hot)) TrStr _join_words(List_TrStr* words) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < words->len)) {
        /* pass */
        if ((i > 0LL)) {
            /* pass */
            TrStr _strtmp_t864 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(" ")));
            _tr_str_release(s);
            s = _strtmp_t864;
        }
        /* pass */
        TrStr _strtmp_t865 = ({ TrStr _cr = (List_TrStr_get(words, i)); TrStr _cres = _tr_strx_concat(_tr_strz(s), _cr.data); _tr_str_release(_cr); _cres; });
        _tr_str_release(s);
        s = _strtmp_t865;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) long long _to_int(TrStr s) {
    /* pass */
    long long r = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    bool neg = false;
    /* pass */
    if (((_tr_strlen(_tr_strz(s)) > 0LL) && (_tr_strz(s)[0LL] == 45LL))) {
        /* pass */
        neg = true;
        /* pass */
        i = 1LL;
    }
    /* pass */
    if (((((i + 1LL) < _tr_strlen(_tr_strz(s))) && (_tr_strz(s)[i] == 48LL)) && ((_tr_strz(s)[(i + 1LL)] == 120LL) || (_tr_strz(s)[(i + 1LL)] == 88LL)))) {
        /* pass */
        i = (i + 2LL);
        /* pass */
        long long hd = 0LL;
        /* pass */
        while (((i < _tr_strlen(_tr_strz(s))) && (hd < 15LL))) {
            /* pass */
            char hc = _tr_strz(s)[i];
            /* pass */
            if (((hc >= 48LL) && (hc <= 57LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 48LL));
            } else if (((hc >= 97LL) && (hc <= 102LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 87LL));
            } else if (((hc >= 65LL) && (hc <= 70LL))) {
                /* pass */
                r = ((r * 16LL) + (hc - 55LL));
            } else {
                /* pass */
                break;
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            hd = (hd + 1LL);
        }
        /* pass */
        if (neg) {
            /* pass */
            return (0LL - r);
        }
        /* pass */
        return r;
    }
    /* pass */
    long long dd = 0LL;
    /* pass */
    while (((i < _tr_strlen(_tr_strz(s))) && (dd < 18LL))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if (((c >= 48LL) && (c <= 57LL))) {
            /* pass */
            r = ((r * 10LL) + (c - 48LL));
        } else {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        dd = (dd + 1LL);
    }
    /* pass */
    if (neg) {
        /* pass */
        return (0LL - r);
    }
    /* pass */
    return r;
}

__attribute__((hot)) long long _skip_gnu_attrs(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while ((i < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if ((t->kind != 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        if (((strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("__attribute__"))) != 0) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("__declspec"))) != 0))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((i >= toks->len)) {
            /* pass */
            break;
        }
        /* pass */
        if ((!((((CTok*)List_ptr_get(toks, i))->kind == 4LL) && (strcmp(_tr_strz(((CTok*)List_ptr_get(toks, i))->text), _tr_strz(_tr_str_lit("("))) == 0)))) {
            /* pass */
            break;
        }
        /* pass */
        long long depth = 0LL;
        /* pass */
        while ((i < toks->len)) {
            /* pass */
            CTok* pt = ((CTok*)List_ptr_get(toks, i));
            /* pass */
            if (((pt->kind == 4LL) && (strcmp(_tr_strz(pt->text), _tr_strz(_tr_str_lit("("))) == 0))) {
                /* pass */
                depth = (depth + 1LL);
            } else if (((pt->kind == 4LL) && (strcmp(_tr_strz(pt->text), _tr_strz(_tr_str_lit(")"))) == 0))) {
                /* pass */
                depth = (depth - 1LL);
                /* pass */
                if ((depth == 0LL)) {
                    /* pass */
                    i = (i + 1LL);
                    /* pass */
                    break;
                }
            }
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    return i;
}

__attribute__((hot)) TrStr _scan_typedef_name(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    long long depth = 0LL;
    /* pass */
    while ((i < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("{"))) == 0))) {
            /* pass */
            depth = (depth + 1LL);
        } else if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("}"))) == 0))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth == 0LL)) {
                /* pass */
                long long ni = _skip_gnu_attrs(toks, (i + 1LL));
                /* pass */
                if (((ni < toks->len) && (((CTok*)List_ptr_get(toks, ni))->kind == 0LL))) {
                    /* pass */
                    return _tr_str_retain(((CTok*)List_ptr_get(toks, ni))->text);
                }
                /* pass */
                return _tr_str_lit("");
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool _typedef_is_fnptr(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while (((i + 1LL) < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit(";"))) == 0))) {
            /* pass */
            return false;
        }
        /* pass */
        if (((((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("("))) == 0)) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && (strcmp(_tr_strz(((CTok*)List_ptr_get(toks, (i + 1LL)))->text), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _scan_fnptr_name(List_ptr* toks, long long pos) {
    /* pass */
    long long i = pos;
    /* pass */
    while (((i + 2LL) < toks->len)) {
        /* pass */
        CTok* t = ((CTok*)List_ptr_get(toks, i));
        /* pass */
        if (((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit(";"))) == 0))) {
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        if (((((t->kind == 4LL) && (strcmp(_tr_strz(t->text), _tr_strz(_tr_str_lit("("))) == 0)) && (((CTok*)List_ptr_get(toks, (i + 1LL)))->kind == 4LL)) && (strcmp(_tr_strz(((CTok*)List_ptr_get(toks, (i + 1LL)))->text), _tr_strz(_tr_str_lit("*"))) == 0))) {
            /* pass */
            if ((((CTok*)List_ptr_get(toks, (i + 2LL)))->kind == 0LL)) {
                /* pass */
                return _tr_str_retain(((CTok*)List_ptr_get(toks, (i + 2LL)))->text);
            }
            /* pass */
            return _tr_str_lit("");
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_lit("");
}

__attribute__((hot)) bool _is_ident_byte(long long c) {
    /* pass */
    return (((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL));
}

__attribute__((hot)) TrStr _rename_word(TrStr text, TrStr old, TrStr new_) {
    /* pass */
    StringBuilder* sb = StringBuilder_init((_tr_strlen(_tr_strz(text)) + 16LL));
    /* pass */
    long long n = _tr_strlen(_tr_strz(text));
    /* pass */
    long long ol = _tr_strlen(_tr_strz(old));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        bool matched = false;
        /* pass */
        if (({ TrStr _wt_t866 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + ol)))); __auto_type _wr = ((((i + ol) <= n) && (strcmp(_wt_t866.data, _tr_strz(old)) == 0))); _tr_str_release(_wt_t866); _wr; })) {
            /* pass */
            bool before_ok = ((i == 0LL) || (!_is_ident_byte(_tr_strz(text)[(i - 1LL)])));
            /* pass */
            bool after_ok = (((i + ol) >= n) || (!_is_ident_byte(_tr_strz(text)[(i + ol)])));
            /* pass */
            if ((before_ok && after_ok)) {
                /* pass */
                StringBuilder_append(sb, new_);
                /* pass */
                i = (i + ol);
                /* pass */
                matched = true;
            }
        }
        /* pass */
        if ((!matched)) {
            /* pass */
            ({ TrStr _sbt_t867 = (_tr_str_wrap(_tr_str_slice(_tr_strz(text), i, (i + 1LL)))); StringBuilder_append(sb, _sbt_t867); _tr_str_release(_sbt_t867); });
            /* pass */
            i = (i + 1LL);
        }
    }
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _basename(TrStr p) {
    /* pass */
    long long last = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(p)))) {
        /* pass */
        char c = _tr_strz(p)[i];
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            last = (i + 1LL);
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(p), last, _tr_strlen(_tr_strz(p))));
}

__attribute__((hot)) TrStr _marker_file(TrStr line) {
    /* pass */
    if ((((_tr_strlen(_tr_strz(line)) < 3LL) || (_tr_strz(line)[0LL] != 35LL)) || (_tr_strz(line)[1LL] != 32LL))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    __auto_type q1 = ({ char* _t868 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t868 ? (long long)(_t868 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t869 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t869 ? (long long)(_t869 - (_tr_strz(rest))) : -1LL; });
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, q2));
}

__attribute__((hot)) bool _marker_is_system(TrStr line) {
    /* pass */
    __auto_type q1 = ({ char* _t870 = strstr(_tr_strz(line), _tr_strz(_tr_str_lit("\""))); _t870 ? (long long)(_t870 - (_tr_strz(line))) : -1LL; });
    /* pass */
    if ((q1 < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), (q1 + 1LL), _tr_strlen(_tr_strz(line))));
    /* pass */
    __auto_type q2 = ({ char* _t871 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("\""))); _t871 ? (long long)(_t871 - (_tr_strz(rest))) : -1LL; });
    /* pass */
    if ((q2 < 0LL)) {
        /* pass */
        _tr_str_release(rest);
        return false;
    }
    /* pass */
    return ({ TrStr _wt_t872 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); TrStr _wt_t873 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (q2 + 1LL), _tr_strlen(_tr_strz(rest))))); __auto_type _wr = ((({ char* _t874 = strstr(_wt_t873.data, _tr_strz(_tr_str_lit(" 3"))); _t874 ? (long long)(_t874 - (_wt_t873.data)) : -1LL; }) >= 0LL)); _tr_str_release(_wt_t872); _tr_str_release(_wt_t873); _wr; });
}

__attribute__((hot)) TrStr _filter_to_target(TrStr raw, TrStr target) {
    /* pass */
    StringBuilder* sb = StringBuilder_init(_tr_strlen(_tr_strz(raw)));
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(raw), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    bool cur_ok = false;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((strcmp(_tr_strz(_marker_file(ln)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            cur_ok = (!_marker_is_system(ln));
            /* pass */
            continue;
        }
        /* pass */
        if (cur_ok) {
            /* pass */
            StringBuilder_append(sb, ln);
            /* pass */
            StringBuilder_append(sb, _tr_str_lit("\n"));
        }
        _tr_str_release(ln);
    }
    /* pass */
    List_TrStr_free(lines);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) TrStr _macro_name(TrStr rest) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(rest)))) {
        /* pass */
        char c = _tr_strz(rest)[i];
        /* pass */
        if (((c == 32LL) || (c == 40LL))) {
            /* pass */
            break;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, i));
}

__attribute__((hot)) TrMap* _load_baseline(TrStr cc) {
    /* pass */
    TrMap* m = _tr_dict_new(512LL);
    /* pass */
    write_file(_tr_str_lit("_bindgen_empty.h"), _tr_str_lit(""));
    /* pass */
    ({ TrStr _aet_t875 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -E -dM \"_bindgen_empty.h\" > \"_bindgen_base.i\" 2>_bindgen_err.txt")))); _tr_system(_aet_t875.data); _tr_str_release(_aet_t875); });
    /* pass */
    if ((!file_exists(_tr_str_lit("_bindgen_base.i")))) {
        /* pass */
        return m;
    }
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(read_file(_tr_str_lit("_bindgen_base.i"))), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t876 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t876)); _tr_str_release(_at_t876); _wr; });
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
    /* pass */
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrMap* _target_define_names(TrStr header) {
    /* pass */
    TrMap* m = _tr_dict_new(128LL);
    /* pass */
    if ((!file_exists(header))) {
        /* pass */
        return m;
    }
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(read_file(header)), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t877 = (List_TrStr_get(lines, i)); __auto_type _wr = (_lstrip(_at_t877)); _tr_str_release(_at_t877); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = ({ TrStr _at_t878 = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))))); __auto_type _wr = (_macro_name(_at_t878)); _tr_str_release(_at_t878); _wr; });
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(nm);
    }
    /* pass */
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _lstrip(TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i < _tr_strlen(_tr_strz(s))) && ((_tr_strz(s)[i] == 32LL) || (_tr_strz(s)[i] == 9LL)))) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), i, _tr_strlen(_tr_strz(s))));
}

__attribute__((hot)) bool _is_builtin_ty_name(TrStr n) {
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("void"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("bool"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("char"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("int"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("float"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("str"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("Pointer"))) == 0)) {
        /* pass */
        return true;
    }
    /* pass */
    return _tr_str_starts_with(_tr_strz(n), _tr_strz(_tr_str_lit("c_")));
}

__attribute__((hot)) bool _is_libc_ty_name(TrStr n) {
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("FILE"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("fpos_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("va_list"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("jmp_buf"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("sigjmp_buf"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("time_t"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("clock_t"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("wchar_t"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("wint_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("mbstate_t"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("off_t"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("off64_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("mode_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("pid_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("uid_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("gid_t"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("dev_t"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ino_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("nlink_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("blkcnt_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("blksize_t"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("fd_set"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("sigset_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("DIR"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("pthread_t"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("sig_atomic_t"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_system_record_ty(TrStr n) {
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("_GUID"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("GUID"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IID"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("CLSID"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("FMTID"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("UUID"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IUnknown"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IDispatch"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IErrorInfo"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ITypeInfo"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ITypeLib"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IStream"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ISequentialStream"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IStorage"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IRecordInfo"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("IServiceProvider"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("VARIANT"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("VARIANTARG"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("DECIMAL"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("SAFEARRAY"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("SAFEARRAYBOUND"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("DISPPARAMS"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("EXCEPINFO"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("CY"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("BSTRBLOB"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("BLOB"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("FLAGGED_WORD_BLOB"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("POINT"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("POINTL"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("SIZE"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("SIZEL"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RECT"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("RECTL"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("FILETIME"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("LARGE_INTEGER"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ULARGE_INTEGER"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("LUID"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("MSG"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("POINTS"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _ident_at(TrStr text, long long start) {
    /* pass */
    long long i = start;
    /* pass */
    while (((i < _tr_strlen(_tr_strz(text))) && (((_tr_strz(text)[i] == 32LL) || (_tr_strz(text)[i] == 9LL)) || (_tr_strz(text)[i] == 91LL)))) {
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((i >= _tr_strlen(_tr_strz(text)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    char c = _tr_strz(text)[i];
    /* pass */
    if ((!((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL)))) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    long long b = i;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(text)))) {
        /* pass */
        c = _tr_strz(text)[i];
        /* pass */
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
            /* pass */
            i = (i + 1LL);
        } else {
            /* pass */
            break;
        }
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(text), b, i));
}

__attribute__((hot)) TrStr _opaque_fallbacks(TrStr body, TrMap* defined) {
    /* pass */
    TrMap* want = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* order = (void*)List_TrStr_new();
    /* pass */
    long long i = 0LL;
    /* pass */
    long long n = _tr_strlen(_tr_strz(body));
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(body)[i];
        /* pass */
        TrStr nm = _tr_str_lit("");
        /* pass */
        if ((c == 58LL)) {
            /* pass */
            TrStr _strtmp_t879 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t879;
            /* pass */
            i = (i + 1LL);
        } else if ((((c == 45LL) && ((i + 1LL) < n)) && (_tr_strz(body)[(i + 1LL)] == 62LL))) {
            /* pass */
            TrStr _strtmp_t880 = _ident_at(body, (i + 2LL));
            _tr_str_release(nm);
            nm = _strtmp_t880;
            /* pass */
            i = (i + 2LL);
        } else if ((c == 91LL)) {
            /* pass */
            TrStr _strtmp_t881 = _ident_at(body, (i + 1LL));
            _tr_str_release(nm);
            nm = _strtmp_t881;
            /* pass */
            i = (i + 1LL);
        } else {
            /* pass */
            i = (i + 1LL);
        }
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_is_builtin_ty_name(nm)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_is_libc_ty_name(nm)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(defined, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(want, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        _tr_dict_set(want, _tr_strz(nm), true);
        /* pass */
        List_TrStr_append(order, nm);
        _tr_str_release(nm);
    }
    /* pass */
    if ((order->len == 0LL)) {
        /* pass */
        Dict_free(want);
        List_TrStr_free(order);
        return _tr_str_lit("");
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(256LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("# Opaque fallbacks: types referenced by this header but defined in a system or\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("# sub-header (bound as opaque handles; pass by Pointer). Verify by-value layout.\n"));
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < order->len)) {
        /* pass */
        ({ TrStr _sbt_t882 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(order, k)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = Pointer[void]\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t882); _tr_str_release(_sbt_t882); });
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("\n"));
    /* pass */
    Dict_free(want);
    List_TrStr_free(order);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) bool _is_single_string_literal(TrStr val) {
    /* pass */
    if (((_tr_strlen(_tr_strz(val)) < 2LL) || (_tr_strz(val)[0LL] != 34LL))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 1LL;
    /* pass */
    bool esc = false;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(val)))) {
        /* pass */
        char c = _tr_strz(val)[i];
        /* pass */
        if (esc) {
            /* pass */
            esc = false;
        } else if ((c == 92LL)) {
            /* pass */
            esc = true;
        } else if ((c == 34LL)) {
            /* pass */
            long long j = (i + 1LL);
            /* pass */
            while ((j < _tr_strlen(_tr_strz(val)))) {
                /* pass */
                char t = _tr_strz(val)[j];
                /* pass */
                if (((t != 32LL) && (t != 9LL))) {
                    /* pass */
                    return false;
                }
                /* pass */
                j = (j + 1LL);
            }
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _is_single_int_literal(TrStr val) {
    /* pass */
    long long n = _tr_strlen(_tr_strz(val));
    /* pass */
    while (((n > 0LL) && ((_tr_strz(val)[(n - 1LL)] == 32LL) || (_tr_strz(val)[(n - 1LL)] == 9LL)))) {
        /* pass */
        n = (n - 1LL);
    }
    /* pass */
    if ((n == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    if (((_tr_strz(val)[0LL] == 45LL) || (_tr_strz(val)[0LL] == 43LL))) {
        /* pass */
        i = 1LL;
    }
    /* pass */
    if ((i >= n)) {
        /* pass */
        return false;
    }
    /* pass */
    bool hex = false;
    /* pass */
    if (((((i + 1LL) < n) && (_tr_strz(val)[i] == 48LL)) && ((_tr_strz(val)[(i + 1LL)] == 120LL) || (_tr_strz(val)[(i + 1LL)] == 88LL)))) {
        /* pass */
        hex = true;
        /* pass */
        i = (i + 2LL);
        /* pass */
        if ((i >= n)) {
            /* pass */
            return false;
        }
    }
    /* pass */
    while ((i < n)) {
        /* pass */
        char c = _tr_strz(val)[i];
        /* pass */
        bool ok = false;
        /* pass */
        if (((c >= 48LL) && (c <= 57LL))) {
            /* pass */
            ok = true;
        } else if ((hex && (((c >= 97LL) && (c <= 102LL)) || ((c >= 65LL) && (c <= 70LL))))) {
            /* pass */
            ok = true;
        }
        /* pass */
        if ((!ok)) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) void emit_defines(Bindgen* bg, TrStr defs, TrMap* baseline, TrMap* allow) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(defs), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))));
        /* pass */
        __auto_type sp = ({ char* _t883 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit(" "))); _t883 ? (long long)(_t883 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if ((sp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, sp));
        /* pass */
        if ((({ char* _t884 = strstr(_tr_strz(nm), _tr_strz(_tr_str_lit("("))); _t884 ? (long long)(_t884 - (_tr_strz(nm))) : -1LL; }) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!_tr_dict_contains(allow, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr val = ({ TrStr _at_t885 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (sp + 1LL), _tr_strlen(_tr_strz(rest))))); __auto_type _wr = (_lstrip(_at_t885)); _tr_str_release(_at_t885); _wr; });
        /* pass */
        if ((_tr_strlen(_tr_strz(val)) == 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        char c0 = _tr_strz(val)[0LL];
        /* pass */
        if ((c0 == 34LL)) {
            /* pass */
            if (_is_single_string_literal(val)) {
                /* pass */
                ({ TrStr _sbt_t886 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(val)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t886); _tr_str_release(_sbt_t886); });
            }
        } else if ((((c0 >= 48LL) && (c0 <= 57LL)) || (c0 == 45LL))) {
            /* pass */
            if (_is_single_int_literal(val)) {
                /* pass */
                long long iv = _to_int(val);
                /* pass */
                ({ TrStr _sbt_t887 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_int = "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(iv)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(bg->consts, _sbt_t887); _tr_str_release(_sbt_t887); });
            }
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(val);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) TrStr _cxx_of(TrStr cc) {
    /* pass */
    if ((strcmp(_tr_strz(cc), _tr_strz(_tr_str_lit("gcc"))) == 0)) {
        /* pass */
        return _tr_str_lit("g++");
    }
    /* pass */
    if ((strcmp(_tr_strz(cc), _tr_strz(_tr_str_lit("clang"))) == 0)) {
        /* pass */
        return _tr_str_lit("clang++");
    }
    /* pass */
    if ((({ char* _t888 = strstr(_tr_strz(cc), _tr_strz(_tr_str_lit("++"))); _t888 ? (long long)(_t888 - (_tr_strz(cc))) : -1LL; }) >= 0LL)) {
        /* pass */
        return _tr_str_retain(cc);
    }
    /* pass */
    return _tr_str_lit("c++");
}

__attribute__((hot)) TrStr _macro_args(long long n, long long mode, long long atype, long long tp) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    bool first = true;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        if ((i == tp)) {
            /* pass */
            if ((mode == 1LL)) {
                /* pass */
                if ((!first)) {
                    /* pass */
                    TrStr _strtmp_t889 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
                    _tr_str_release(s);
                    s = _strtmp_t889;
                }
                /* pass */
                TrStr _strtmp_t890 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("char")));
                _tr_str_release(s);
                s = _strtmp_t890;
                /* pass */
                first = false;
            }
            /* pass */
            i = (i + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((!first)) {
            /* pass */
            TrStr _strtmp_t891 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit(", ")));
            _tr_str_release(s);
            s = _strtmp_t891;
        }
        /* pass */
        first = false;
        /* pass */
        if ((mode == 0LL)) {
            /* pass */
            if ((atype == 0LL)) {
                /* pass */
                TrStr _strtmp_t892 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("void* a")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t892;
            } else {
                /* pass */
                TrStr _strtmp_t893 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("intptr_t a")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(s);
                s = _strtmp_t893;
            }
        } else if ((mode == 1LL)) {
            /* pass */
            TrStr _strtmp_t894 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("a")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(s);
            s = _strtmp_t894;
        } else {
            /* pass */
            if ((atype == 0LL)) {
                /* pass */
                TrStr _strtmp_t895 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("a")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": Pointer[void]"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t895;
            } else {
                /* pass */
                TrStr _strtmp_t896 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("a")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(i)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": c_intptr_t"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(s);
                s = _strtmp_t896;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _ent_name(TrStr e) {
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(e), 0LL, ({ char* _t897 = strstr(_tr_strz(e), _tr_strz(_tr_str_lit("|"))); _t897 ? (long long)(_t897 - (_tr_strz(e))) : -1LL; })));
}

__attribute__((hot)) long long _ent_arity(TrStr e) {
    /* pass */
    TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(e), (({ char* _t898 = strstr(_tr_strz(e), _tr_strz(_tr_str_lit("|"))); _t898 ? (long long)(_t898 - (_tr_strz(e))) : -1LL; }) + 1LL), _tr_strlen(_tr_strz(e))));
    /* pass */
    __auto_type b = ({ char* _t899 = strstr(_tr_strz(r), _tr_strz(_tr_str_lit("|"))); _t899 ? (long long)(_t899 - (_tr_strz(r))) : -1LL; });
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        return _to_int(r);
    }
    /* pass */
    return ({ TrStr _at_t900 = (_tr_str_wrap(_tr_str_slice(_tr_strz(r), 0LL, b))); __auto_type _wr = (_to_int(_at_t900)); _tr_str_release(_at_t900); _wr; });
}

__attribute__((hot)) long long _ent_typepos(TrStr e) {
    /* pass */
    TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(e), (({ char* _t901 = strstr(_tr_strz(e), _tr_strz(_tr_str_lit("|"))); _t901 ? (long long)(_t901 - (_tr_strz(e))) : -1LL; }) + 1LL), _tr_strlen(_tr_strz(e))));
    /* pass */
    __auto_type b = ({ char* _t902 = strstr(_tr_strz(r), _tr_strz(_tr_str_lit("|"))); _t902 ? (long long)(_t902 - (_tr_strz(r))) : -1LL; });
    /* pass */
    if ((b < 0LL)) {
        /* pass */
        _tr_str_release(r);
        return (-1LL);
    }
    /* pass */
    return ({ TrStr _at_t903 = (_tr_str_wrap(_tr_str_slice(_tr_strz(r), (b + 1LL), _tr_strlen(_tr_strz(r))))); __auto_type _wr = (_to_int(_at_t903)); _tr_str_release(_at_t903); _wr; });
}

__attribute__((hot)) bool _is_ident_ch(long long c) {
    /* pass */
    return (((((c >= 48LL) && (c <= 57LL)) || ((c >= 65LL) && (c <= 90LL))) || ((c >= 97LL) && (c <= 122LL))) || (c == 95LL));
}

__attribute__((hot)) bool _param_ptr_type(TrStr body, TrStr pname) {
    /* pass */
    long long plen = _tr_strlen(_tr_strz(pname));
    /* pass */
    if ((plen == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i + plen) <= _tr_strlen(_tr_strz(body)))) {
        /* pass */
        if (({ TrStr _wt_t904 = (_tr_str_wrap(_tr_str_slice(_tr_strz(body), i, (i + plen)))); __auto_type _wr = ((strcmp(_wt_t904.data, _tr_strz(pname)) == 0)); _tr_str_release(_wt_t904); _wr; })) {
            /* pass */
            bool okb = true;
            /* pass */
            if (((i > 0LL) && _is_ident_ch(_tr_strz(body)[(i - 1LL)]))) {
                /* pass */
                okb = false;
            }
            /* pass */
            long long j = (i + plen);
            /* pass */
            bool oka = true;
            /* pass */
            if (((j < _tr_strlen(_tr_strz(body))) && _is_ident_ch(_tr_strz(body)[j]))) {
                /* pass */
                oka = false;
            }
            /* pass */
            if ((okb && oka)) {
                /* pass */
                while (((j < _tr_strlen(_tr_strz(body))) && ((_tr_strz(body)[j] == 32LL) || (_tr_strz(body)[j] == 9LL)))) {
                    /* pass */
                    j = (j + 1LL);
                }
                /* pass */
                if (((j < _tr_strlen(_tr_strz(body))) && (_tr_strz(body)[j] == 42LL))) {
                    /* pass */
                    return true;
                }
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) bool _balanced_delims(TrStr s) {
    /* pass */
    long long depth = 0LL;
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((((c == 40LL) || (c == 91LL)) || (c == 123LL))) {
            /* pass */
            depth = (depth + 1LL);
        } else if ((((c == 41LL) || (c == 93LL)) || (c == 125LL))) {
            /* pass */
            depth = (depth - 1LL);
            /* pass */
            if ((depth < 0LL)) {
                /* pass */
                return false;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (depth == 0LL);
}

__attribute__((hot)) List_TrStr* _collect_fn_macros(TrStr defs, TrMap* baseline) {
    /* pass */
    List_TrStr* out = (void*)List_TrStr_new();
    /* pass */
    TrMap* seen = _tr_dict_new(128LL);
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(defs), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((!_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("#define "))))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), 8LL, _tr_strlen(_tr_strz(ln))));
        /* pass */
        __auto_type par = ({ char* _t905 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("("))); _t905 ? (long long)(_t905 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if ((par <= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type sp = ({ char* _t906 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit(" "))); _t906 ? (long long)(_t906 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if (((sp >= 0LL) && (sp < par))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, par));
        /* pass */
        if (_tr_str_starts_with(_tr_strz(nm), _tr_strz(_tr_str_lit("__")))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(baseline, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_dict_contains(seen, _tr_strz(nm))) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type _tr_v_close = ({ char* _t907 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit(")"))); _t907 ? (long long)(_t907 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if (((_tr_v_close < 0LL) || (_tr_v_close < par))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr plist = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), (par + 1LL), _tr_v_close));
        /* pass */
        if ((({ char* _t908 = strstr(_tr_strz(plist), _tr_strz(_tr_str_lit("..."))); _t908 ? (long long)(_t908 - (_tr_strz(plist))) : -1LL; }) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr body = ({ TrStr _at_t909 = (_tr_str_wrap(_tr_str_slice(_tr_strz(rest), (_tr_v_close + 1LL), _tr_strlen(_tr_strz(rest))))); __auto_type _wr = (_lstrip(_at_t909)); _tr_str_release(_at_t909); _wr; });
        /* pass */
        if ((strcmp(_tr_strz(body), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((!_balanced_delims(body))) {
            /* pass */
            continue;
        }
        /* pass */
        long long arity = 0LL;
        /* pass */
        TrStr pt = _tr_str_wrap(_tr_str_strip(_tr_strz(plist)));
        /* pass */
        if ((strcmp(_tr_strz(pt), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            arity = 1LL;
            /* pass */
            long long ci = 0LL;
            /* pass */
            while ((ci < _tr_strlen(_tr_strz(pt)))) {
                /* pass */
                if ((_tr_strz(pt)[ci] == 44LL)) {
                    /* pass */
                    arity = (arity + 1LL);
                }
                /* pass */
                ci = (ci + 1LL);
            }
        }
        /* pass */
        if ((arity > 8LL)) {
            /* pass */
            continue;
        }
        /* pass */
        long long typepos = (-1LL);
        /* pass */
        if (((((arity > 0LL) && (({ char* _t910 = strstr(_tr_strz(body), _tr_strz(_tr_str_lit("sizeof"))); _t910 ? (long long)(_t910 - (_tr_strz(body))) : -1LL; }) < 0LL)) && (({ char* _t911 = strstr(_tr_strz(body), _tr_strz(_tr_str_lit("offsetof"))); _t911 ? (long long)(_t911 - (_tr_strz(body))) : -1LL; }) < 0LL)) && (({ char* _t912 = strstr(_tr_strz(body), _tr_strz(_tr_str_lit("##"))); _t912 ? (long long)(_t912 - (_tr_strz(body))) : -1LL; }) < 0LL))) {
            /* pass */
            List_TrStr* parts = _tr_str_split(_tr_strz(plist), _tr_strz(_tr_str_lit(",")));
            /* pass */
            long long pi = 0LL;
            /* pass */
            long long nfound = 0LL;
            /* pass */
            while ((pi < parts->len)) {
                /* pass */
                TrStr pnm = _tr_str_wrap(_tr_str_strip(_tr_strz(List_TrStr_get(parts, pi))));
                /* pass */
                if (_param_ptr_type(body, pnm)) {
                    /* pass */
                    typepos = pi;
                    /* pass */
                    nfound = (nfound + 1LL);
                }
                /* pass */
                pi = (pi + 1LL);
                _tr_str_release(pnm);
            }
            /* pass */
            if ((nfound != 1LL)) {
                /* pass */
                typepos = (-1LL);
            }
        }
        /* pass */
        _tr_dict_set(seen, _tr_strz(nm), true);
        /* pass */
        ({ TrStr _at_t913 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(nm), _tr_strz(_tr_str_lit("|")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(arity)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("|"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(typepos)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); List_TrStr_append(out, _at_t913); _tr_str_release(_at_t913); });
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(nm);
        _tr_str_release(plist);
        _tr_str_release(body);
        _tr_str_release(pt);
    }
    /* pass */
    Dict_free(seen);
    List_TrStr_free(lines);
    return out;
}

__attribute__((hot)) bool _has_cc_error(TrStr errtxt) {
    /* pass */
    return ((({ char* _t914 = strstr(_tr_strz(errtxt), _tr_strz(_tr_str_lit(": error:"))); _t914 ? (long long)(_t914 - (_tr_strz(errtxt))) : -1LL; }) >= 0LL) || (({ char* _t915 = strstr(_tr_strz(errtxt), _tr_strz(_tr_str_lit(": error :"))); _t915 ? (long long)(_t915 - (_tr_strz(errtxt))) : -1LL; }) >= 0LL));
}

__attribute__((hot)) TrMap* _macro_bad_names(TrStr errtxt) {
    /* pass */
    TrMap* m = _tr_dict_new(64LL);
    /* pass */
    TrStr key = _tr_str_lit("in expansion of macro ");
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(errtxt), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        __auto_type fp = ({ char* _t916 = strstr(_tr_strz(ln), _tr_strz(key)); _t916 ? (long long)(_t916 - (_tr_strz(ln))) : -1LL; });
        /* pass */
        if ((fp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr after = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), (fp + _tr_strlen(_tr_strz(key))), _tr_strlen(_tr_strz(ln))));
        /* pass */
        __auto_type q1 = ({ char* _t917 = strstr(_tr_strz(after), _tr_strz(_tr_str_lit("'"))); _t917 ? (long long)(_t917 - (_tr_strz(after))) : -1LL; });
        /* pass */
        if ((q1 < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest2 = _tr_str_wrap(_tr_str_slice(_tr_strz(after), (q1 + 1LL), _tr_strlen(_tr_strz(after))));
        /* pass */
        __auto_type q2 = ({ char* _t918 = strstr(_tr_strz(rest2), _tr_strz(_tr_str_lit("'"))); _t918 ? (long long)(_t918 - (_tr_strz(rest2))) : -1LL; });
        /* pass */
        if ((q2 < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(rest2), 0LL, q2));
        /* pass */
        if ((strcmp(_tr_strz(nm), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ln);
        _tr_str_release(after);
        _tr_str_release(rest2);
        _tr_str_release(nm);
    }
    /* pass */
    _tr_str_release(key);
    List_TrStr_free(lines);
    return m;
}

__attribute__((hot)) TrStr _macro_shim_line(TrStr sym, TrStr nm, long long ar, long long form, long long atype, long long tp) {
    /* pass */
    if ((form == 0LL)) {
        /* pass */
        return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void* ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("){ return (void*)(intptr_t)("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")); }\n"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 0LL, atype, tp)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("){ "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 1LL, atype, tp)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("); }\n"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) List_TrStr* _macro_probe_write(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake) {
    /* pass */
    StringBuilder* probe = StringBuilder_init(8192LL);
    /* pass */
    ({ TrStr _sbt_t919 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#include \"")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(probe, _sbt_t919); _tr_str_release(_sbt_t919); });
    /* pass */
    StringBuilder_append(probe, _tr_str_lit("#include <stdint.h>\n"));
    /* pass */
    List_TrStr* order = (void*)List_TrStr_new();
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((!_tr_dict_contains(cand, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        long long tp = (0LL - 1LL);
        /* pass */
        if (bake) {
            /* pass */
            tp = _ent_typepos(ent);
            /* pass */
            if ((tp < 0LL)) {
                /* pass */
                continue;
            }
        }
        /* pass */
        ({ TrStr _at_t920 = (_tr_strx_concat(_tr_strz(_tr_str_lit("mP_")), _tr_strz(nm))); TrStr _sbt_t921 = (_macro_shim_line(_at_t920, nm, _ent_arity(ent), form, atype, tp)); StringBuilder_append(probe, _sbt_t921); _tr_str_release(_at_t920); _tr_str_release(_sbt_t921); });
        /* pass */
        List_TrStr_append(order, nm);
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    /* pass */
    ({ TrStr _at_t922 = (StringObj_as_str(StringBuilder_to_string(probe))); write_file(_tr_str_lit("_macroprobe.c"), _at_t922); _tr_str_release(_at_t922); });
    /* pass */
    StringBuilder__tr_fn_free(probe);
    return order;
}

__attribute__((hot)) TrMap* _macro_form_bad(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    List_TrStr* order = _macro_probe_write(header, macros, cand, form, atype, bake);
    /* pass */
    ({ TrStr _aet_t923 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cxx), _tr_strz(_tr_str_lit(" -std=c++17 -fsyntax-only -fmax-errors=0 -fpermissive -w ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(extra)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" _macroprobe.c > _macroprobe.err 2>&1"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t923.data); _tr_str_release(_aet_t923); });
    /* pass */
    TrStr errtxt = _tr_str_lit("");
    /* pass */
    if (file_exists(_tr_str_lit("_macroprobe.err"))) {
        /* pass */
        TrStr _strtmp_t924 = read_file(_tr_str_lit("_macroprobe.err"));
        _tr_str_release(errtxt);
        errtxt = _strtmp_t924;
    }
    /* pass */
    TrMap* bad = _macro_bad_names(errtxt);
    /* pass */
    TrStr key = _tr_str_lit("_macroprobe.c:");
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(errtxt), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = List_TrStr_get(lines, i);
        /* pass */
        i = (i + 1LL);
        /* pass */
        if ((({ char* _t925 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit(": error:"))); _t925 ? (long long)(_t925 - (_tr_strz(ln))) : -1LL; }) < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((({ char* _t926 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("is not allowed here"))); _t926 ? (long long)(_t926 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((({ char* _t927 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("expected unqualified-id"))); _t927 ? (long long)(_t927 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((({ char* _t928 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("expected declaration"))); _t928 ? (long long)(_t928 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        __auto_type fp = ({ char* _t929 = strstr(_tr_strz(ln), _tr_strz(key)); _t929 ? (long long)(_t929 - (_tr_strz(ln))) : -1LL; });
        /* pass */
        if ((fp < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr after = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), (fp + _tr_strlen(_tr_strz(key))), _tr_strlen(_tr_strz(ln))));
        /* pass */
        __auto_type colon = ({ char* _t930 = strstr(_tr_strz(after), _tr_strz(_tr_str_lit(":"))); _t930 ? (long long)(_t930 - (_tr_strz(after))) : -1LL; });
        /* pass */
        if ((colon < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        long long idx = ({ TrStr _at_t931 = (_tr_str_wrap(_tr_str_slice(_tr_strz(after), 0LL, colon))); __auto_type _wr = ((_to_int(_at_t931) - 3LL)); _tr_str_release(_at_t931); _wr; });
        /* pass */
        if (((idx >= 0LL) && (idx < order->len))) {
            /* pass */
            ({ TrStr _dkt_t932 = (List_TrStr_get(order, idx)); _tr_dict_set(bad, _tr_strz(_dkt_t932), true); _tr_str_release(_dkt_t932); });
        }
        _tr_str_release(ln);
        _tr_str_release(after);
    }
    /* pass */
    List_TrStr_free(order);
    _tr_str_release(errtxt);
    _tr_str_release(key);
    List_TrStr_free(lines);
    return bad;
}

__attribute__((hot)) TrMap* _macro_verify_form(TrStr header, List_TrStr* macros, TrMap* cand, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    TrMap* cur = cand;
    /* pass */
    long long iter = 0LL;
    /* pass */
    while ((iter < 12LL)) {
        /* pass */
        iter = (iter + 1LL);
        /* pass */
        TrMap* bad = _macro_form_bad(header, macros, cur, form, atype, bake, cxx, extra);
        /* pass */
        if ((bad->len == 0LL)) {
            /* pass */
            break;
        }
        /* pass */
        TrMap* next = _tr_dict_new(64LL);
        /* pass */
        long long k = 0LL;
        /* pass */
        while ((k < macros->len)) {
            /* pass */
            TrStr ent = List_TrStr_get(macros, k);
            /* pass */
            k = (k + 1LL);
            /* pass */
            TrStr nm = _ent_name(ent);
            /* pass */
            if ((_tr_dict_contains(cur, _tr_strz(nm)) && (!_tr_dict_contains(bad, _tr_strz(nm))))) {
                /* pass */
                _tr_dict_set(next, _tr_strz(nm), true);
            }
            _tr_str_release(ent);
            _tr_str_release(nm);
        }
        /* pass */
        if ((next->len == cur->len)) {
            /* pass */
            break;
        }
        /* pass */
        cur = next;
        Dict_free(bad);
    }
    /* pass */
    return cur;
}

__attribute__((hot)) TrMap* _macro_remaining(List_TrStr* macros, TrMap* done) {
    /* pass */
    TrMap* m = _tr_dict_new(64LL);
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(ent), 0LL, ({ char* _t933 = strstr(_tr_strz(ent), _tr_strz(_tr_str_lit("|"))); _t933 ? (long long)(_t933 - (_tr_strz(ent))) : -1LL; })));
        /* pass */
        if ((!_tr_dict_contains(done, _tr_strz(nm)))) {
            /* pass */
            _tr_dict_set(m, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    /* pass */
    return m;
}

__attribute__((hot)) void _macro_mark_done(TrMap* done, TrMap* ok, List_TrStr* macros) {
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _tr_str_wrap(_tr_str_slice(_tr_strz(ent), 0LL, ({ char* _t934 = strstr(_tr_strz(ent), _tr_strz(_tr_str_lit("|"))); _t934 ? (long long)(_t934 - (_tr_strz(ent))) : -1LL; })));
        /* pass */
        if (_tr_dict_contains(ok, _tr_strz(nm))) {
            /* pass */
            _tr_dict_set(done, _tr_strz(nm), true);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
}

__attribute__((hot)) void _macro_pass(TrStr header, List_TrStr* macros, TrMap* done, TrMap* plan, long long form, long long atype, bool bake, TrStr cxx, TrStr extra) {
    /* pass */
    TrMap* cand = _macro_remaining(macros, done);
    /* pass */
    if (bake) {
        /* pass */
        TrMap* c2 = _tr_dict_new(64LL);
        /* pass */
        long long ci = 0LL;
        /* pass */
        while ((ci < macros->len)) {
            /* pass */
            TrStr e2 = List_TrStr_get(macros, ci);
            /* pass */
            ci = (ci + 1LL);
            /* pass */
            TrStr n2 = _ent_name(e2);
            /* pass */
            if ((_tr_dict_contains(cand, _tr_strz(n2)) && (_ent_typepos(e2) >= 0LL))) {
                /* pass */
                _tr_dict_set(c2, _tr_strz(n2), true);
            }
            _tr_str_release(e2);
            _tr_str_release(n2);
        }
        /* pass */
        cand = c2;
    }
    /* pass */
    TrMap* ok = _macro_verify_form(header, macros, cand, form, atype, bake, cxx, extra);
    /* pass */
    TrStr bk = _tr_str_lit("0");
    /* pass */
    if (bake) {
        /* pass */
        TrStr _strtmp_t935 = _tr_str_lit("1");
        _tr_str_release(bk);
        bk = _strtmp_t935;
    }
    /* pass */
    long long k = 0LL;
    /* pass */
    while ((k < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k);
        /* pass */
        k = (k + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((_tr_dict_contains(ok, _tr_strz(nm)) && (!_tr_dict_contains(done, _tr_strz(nm))))) {
            /* pass */
            _tr_dict_set(done, _tr_strz(nm), true);
            /* pass */
            ({ TrStr _dvt_t936 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_str_wrap(_tr_int_to_str((long long)(form)))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("|"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(atype)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("|"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(bk)); _tr_str_release(_cl); _cres; })); _tr_dict_set(plan, _tr_strz(nm), _tr_str_box(_tr_str_retain(_dvt_t936))); _tr_str_release(_dvt_t936); });
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
    }
    Dict_free(ok);
    _tr_str_release(bk);
}

__attribute__((hot)) TrStr _gen_macro_shims(TrStr header, TrStr out, List_TrStr* macros, TrStr cxx, TrStr extra) {
    /* pass */
    if ((macros->len == 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    long long n_total = macros->len;
    /* pass */
    TrMap* done = _tr_dict_new(64LL);
    /* pass */
    TrMap* plan = _tr_dict_new(64LL);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 0LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 0LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 1LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 1LL, false, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 0LL, 0LL, true, cxx, extra);
    /* pass */
    _macro_pass(header, macros, done, plan, 1LL, 0LL, true, cxx, extra);
    /* pass */
    _tr_system(_tr_strz(_tr_str_lit("rm -f _macroprobe.c _macroprobe.err 2>/dev/null")));
    /* pass */
    StringBuilder* shim = StringBuilder_init(8192LL);
    /* pass */
    ({ TrStr _sbt_t937 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("// Auto-generated macro shims for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen --macros).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t937); _tr_str_release(_sbt_t937); });
    /* pass */
    ({ TrStr _sbt_t938 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#include \"")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t938); _tr_str_release(_sbt_t938); });
    /* pass */
    StringBuilder_append(shim, _tr_str_lit("#include <stdint.h>\n"));
    /* pass */
    StringBuilder_append(shim, _tr_str_lit("extern \"C\" {\n"));
    /* pass */
    StringBuilder* binds = StringBuilder_init(4096LL);
    /* pass */
    long long kept = 0LL;
    /* pass */
    long long n_ip = 0LL;
    /* pass */
    long long n_te = 0LL;
    /* pass */
    long long k2 = 0LL;
    /* pass */
    while ((k2 < macros->len)) {
        /* pass */
        TrStr ent = List_TrStr_get(macros, k2);
        /* pass */
        k2 = (k2 + 1LL);
        /* pass */
        TrStr nm = _ent_name(ent);
        /* pass */
        if ((!_tr_dict_contains(plan, _tr_strz(nm)))) {
            /* pass */
            continue;
        }
        /* pass */
        long long ar = _ent_arity(ent);
        /* pass */
        List_TrStr* spec = _tr_str_split(_tr_strz(_tr_str_retain(_tr_str_unbox(_tr_dict_get(plan, _tr_strz(nm))))), _tr_strz(_tr_str_lit("|")));
        /* pass */
        long long form = ({ TrStr _at_t939 = (List_TrStr_get(spec, 0LL)); __auto_type _wr = (_to_int(_at_t939)); _tr_str_release(_at_t939); _wr; });
        /* pass */
        long long atype = ({ TrStr _at_t940 = (List_TrStr_get(spec, 1LL)); __auto_type _wr = (_to_int(_at_t940)); _tr_str_release(_at_t940); _wr; });
        /* pass */
        long long tp = (0LL - 1LL);
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(spec, 2LL)), _tr_strz(_tr_str_lit("1"))) == 0)) {
            /* pass */
            tp = _ent_typepos(ent);
        }
        /* pass */
        ({ TrStr _at_t941 = (_tr_strx_concat(_tr_strz(_tr_str_lit("m_")), _tr_strz(nm))); TrStr _sbt_t942 = (_macro_shim_line(_at_t941, nm, ar, form, atype, tp)); StringBuilder_append(shim, _sbt_t942); _tr_str_release(_at_t941); _tr_str_release(_sbt_t942); });
        /* pass */
        TrStr bl = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def m_")), _tr_strz(nm))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_macro_args(ar, 2LL, atype, tp)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if ((form == 0LL)) {
            /* pass */
            TrStr _strtmp_t943 = _tr_strx_concat(_tr_strz(bl), _tr_strz(_tr_str_lit(" -> Pointer[void]")));
            _tr_str_release(bl);
            bl = _strtmp_t943;
        }
        /* pass */
        ({ TrStr _sbt_t944 = (_tr_strx_concat(_tr_strz(bl), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(binds, _sbt_t944); _tr_str_release(_sbt_t944); });
        /* pass */
        kept = (kept + 1LL);
        /* pass */
        if ((atype == 1LL)) {
            /* pass */
            n_ip = (n_ip + 1LL);
        }
        /* pass */
        if ((tp >= 0LL)) {
            /* pass */
            n_te = (n_te + 1LL);
        }
        _tr_str_release(ent);
        _tr_str_release(nm);
        List_TrStr_free(spec);
        _tr_str_release(bl);
    }
    /* pass */
    StringBuilder_append(shim, _tr_str_lit("}\n"));
    /* pass */
    if ((kept == 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    ({ TrStr _at_t945 = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("_macros.c")))); TrStr _at_t946 = (StringObj_as_str(StringBuilder_to_string(shim))); write_file(_at_t945, _at_t946); _tr_str_release(_at_t945); _tr_str_release(_at_t946); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(kept)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: macro shims: ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" of "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_total)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" function-like macros bound ("))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_ip)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" intptr, "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_te)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" type-erased) -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(out)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_macros.c (m_<NAME>)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    StringBuilder__tr_fn_free(shim);
    return StringObj_as_str(StringBuilder_to_string(binds));
}

__attribute__((hot)) void run_bindgen(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs, bool want_macros) {
    /* pass */
    TrStr tmp_decls = _tr_str_lit("_bindgen_decls.i");
    /* pass */
    TrStr tmp_defs = _tr_str_lit("_bindgen_defs.i");
    /* pass */
    ({ TrStr _aet_t947 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(extra))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -E \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_decls)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t947.data); _tr_str_release(_aet_t947); });
    /* pass */
    ({ TrStr _aet_t948 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cc), _tr_strz(extra))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -E -dM \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" > \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tmp_defs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" 2>>_bindgen_err.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t948.data); _tr_str_release(_aet_t948); });
    /* pass */
    if ((!file_exists(tmp_decls))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: preprocessing failed (see _bindgen_err.txt)"))); printf("\n"); });
        /* pass */
        _tr_str_release(tmp_decls);
        _tr_str_release(tmp_defs);
        return;
    }
    /* pass */
    TrStr src = ({ TrStr _at_t949 = (read_file(tmp_decls)); __auto_type _wr = (_filter_to_target(_at_t949, header)); _tr_str_release(_at_t949); _wr; });
    /* pass */
    List_ptr* toks = tokenize_c(src);
    /* pass */
    Bindgen* bg = Bindgen_init(toks);
    /* pass */
    Bindgen_run(bg);
    /* pass */
    TrMap* baseline = _load_baseline(cc);
    /* pass */
    ({ TrStr _at_t950 = (read_file(tmp_defs)); emit_defines(bg, _at_t950, baseline, _target_define_names(header)); _tr_str_release(_at_t950); });
    /* pass */
    TrStr macro_binds = _tr_str_lit("");
    /* pass */
    TrStr macro_shim = _tr_str_lit("");
    /* pass */
    if (want_macros) {
        /* pass */
        TrStr _strtmp_t953 = ({ TrStr _at_t951 = (read_file(tmp_defs)); TrStr _at_t952 = (_cxx_of(cc)); __auto_type _wr = (_gen_macro_shims(header, out, _collect_fn_macros(_at_t951, baseline), _at_t952, extra)); _tr_str_release(_at_t951); _tr_str_release(_at_t952); _wr; });
        _tr_str_release(macro_binds);
        macro_binds = _strtmp_t953;
        /* pass */
        if ((strcmp(_tr_strz(macro_binds), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            TrStr _strtmp_t954 = ({ TrStr _cl = (_basename(out)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_macros.c"))); _tr_str_release(_cl); _cres; });
            _tr_str_release(macro_shim);
            macro_shim = _strtmp_t954;
        }
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t955 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# Auto-generated FFI bindings for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t955); _tr_str_release(_sbt_t955); });
    /* pass */
    if ((strcmp(_tr_strz(macro_shim), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t956 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-shim: ")), _tr_strz(macro_shim))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\r\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t956); _tr_str_release(_sbt_t956); });
        /* pass */
        ({ TrStr _sbt_t957 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-cflags: -fpermissive ")), _tr_strz(extra))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\r\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t957); _tr_str_release(_sbt_t957); });
    }
    /* pass */
    if ((strcmp(_tr_strz(pkglibs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t958 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-linkflags:")), _tr_strz(pkglibs))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t958); _tr_str_release(_sbt_t958); });
    }
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("\n"));
    /* pass */
    TrStr ts = StringObj_as_str(StringBuilder_to_string(bg->types));
    /* pass */
    TrStr ss = StringObj_as_str(StringBuilder_to_string(bg->structs));
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(bg->consts));
    /* pass */
    TrStr fs = StringObj_as_str(StringBuilder_to_string(bg->funcs));
    /* pass */
    if ((strcmp(_tr_strz(macro_binds), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t959 = _tr_strx_concat(_tr_strz(fs), _tr_strz(macro_binds));
        _tr_str_release(fs);
        fs = _strtmp_t959;
    }
    /* pass */
    TrStr fb = ({ TrStr _at_t960 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ts), _tr_strz(ss))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fs)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_opaque_fallbacks(_at_t960, bg->seen)); _tr_str_release(_at_t960); _wr; });
    /* pass */
    if ((strcmp(_tr_strz(fb), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, fb);
    }
    /* pass */
    if ((strcmp(_tr_strz(ts), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t961 = (_tr_strx_concat(_tr_strz(ts), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t961); _tr_str_release(_sbt_t961); });
    }
    /* pass */
    if ((strcmp(_tr_strz(ss), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, ss);
    }
    /* pass */
    if ((strcmp(_tr_strz(cs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t962 = (_tr_strx_concat(_tr_strz(cs), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t962); _tr_str_release(_sbt_t962); });
    }
    /* pass */
    if ((strcmp(_tr_strz(fs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("extern \"C\":\n"));
        /* pass */
        StringBuilder_append(sb, fs);
    }
    /* pass */
    TrStr result = StringObj_as_str(StringBuilder_to_string(sb));
    /* pass */
    long long ri = 0LL;
    /* pass */
    long long n_renamed = 0LL;
    /* pass */
    while ((ri < bg->defined->len)) {
        /* pass */
        TrStr dn = List_TrStr_get(bg->defined, ri);
        /* pass */
        ri = (ri + 1LL);
        /* pass */
        if (_tr_dict_contains(bg->skip_syms, _tr_strz(dn))) {
            /* pass */
            TrStr _strtmp_t964 = ({ TrStr _at_t963 = (_tr_strx_concat(_tr_strz(dn), _tr_strz(_tr_str_lit("_")))); __auto_type _wr = (_rename_word(result, dn, _at_t963)); _tr_str_release(_at_t963); _wr; });
            _tr_str_release(result);
            result = _strtmp_t964;
            /* pass */
            n_renamed = (n_renamed + 1LL);
        }
        _tr_str_release(dn);
    }
    /* pass */
    write_file(out, result);
    /* pass */
    _rm_files(_tr_str_lit("_bindgen_decls.i _bindgen_defs.i _bindgen_base.i _bindgen_empty.h _bindgen_err.txt"));
    /* pass */
    TrStr msg = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: wrote ")), _tr_strz(out))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" — "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_structs)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" types, "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_funcs)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" functions"))); _tr_str_release(_cl); _cres; });
    /* pass */
    if ((bg->n_skipped > 0LL)) {
        /* pass */
        TrStr _strtmp_t965 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(msg), _tr_strz(_tr_str_lit(" (")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(bg->n_skipped)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" runtime/libc symbols skipped)"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(msg);
        msg = _strtmp_t965;
    }
    /* pass */
    ({ printf("%s", _tr_strz(msg)); printf("\n"); });
    _tr_str_release(tmp_decls);
    _tr_str_release(tmp_defs);
    _tr_str_release(src);
    List_ptr_free_obj(toks, _trdrop_CTok);
    _tr_obj_release(bg, _trdrop_Bindgen);
    _tr_str_release(macro_binds);
    _tr_str_release(macro_shim);
    _tr_str_release(ts);
    _tr_str_release(ss);
    _tr_str_release(cs);
    _tr_str_release(fs);
    _tr_str_release(fb);
    _tr_str_release(result);
    _tr_str_release(msg);
    StringBuilder__tr_fn_free(sb);
}

__attribute__((hot)) TrStr _cxxwalk_src() {
    /* pass */
    StringBuilder* sb = StringBuilder_init(8192LL);
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <clang-c/Index.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <stdio.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <string.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("#include <stdlib.h>\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static const char* S(CXString s){ const char* p=clang_getCString(s); return p?p:\"\"; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static const char* scalar(enum CXTypeKind k){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  switch(k){ case CXType_Bool:return \"bool\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    case CXType_Char_S:case CXType_Char_U:return \"c_char\"; case CXType_SChar:return \"c_schar\"; case CXType_UChar:return \"c_uchar\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    case CXType_Char16:return \"c_char16\"; case CXType_Char32:return \"c_char32\"; case CXType_WChar:return \"c_wchar\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    case CXType_Short:return \"c_short\"; case CXType_UShort:return \"c_ushort\"; case CXType_Int:return \"c_int\"; case CXType_UInt:return \"c_uint\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    case CXType_Long:return \"c_long\"; case CXType_ULong:return \"c_ulong\"; case CXType_LongLong:return \"c_longlong\"; case CXType_ULongLong:return \"c_ulonglong\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    case CXType_Float:return \"c_float\"; case CXType_Double:return \"c_double\"; case CXType_LongDouble:return \"c_ldouble\"; default:return \"\"; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static char g_params[8][64]; static CXType g_args[8]; static int g_np=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static char g_cbase[256]={0};   /* container template base (e.g. \"std::map\") for nested-type detection */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_inst=0;            /* 1 while walking template instantiations (--inst mode) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_hasbegin=0, g_hasend=0;   /* container exposes begin()/end() -> emit ITER helper */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* Collect NON-system external records used by value, so we can lay them out as @value_type\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   (a library value struct in a filtered sub-header would otherwise be an opaque handle). System\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   records (windows.h) are left on the bare path — zero risk to the working COM handling. */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static CXCursor g_ext[128]; static char g_extn[128][160]; static int g_next=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void collect_ext(CXType rec){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(g_inst) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXCursor decl=clang_getTypeDeclaration(rec);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXSourceLocation loc=clang_getCursorLocation(decl);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_Location_isFromMainFile(loc)) return;   /* main-file records already emitted via CLASS */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_Location_isInSystemHeader(loc)) return; /* system record -> bare path (unchanged) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_Type_getSizeOf(rec)<=0) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXString sp=clang_getTypeSpelling(rec); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(strstr(r,\"<\")||strstr(r,\"::\")){ clang_disposeString(sp); return; } /* skip templates / nested (kept simple) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(int i=0;i<g_next;i++) if(strcmp(g_extn[i],r)==0){ clang_disposeString(sp); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(g_next<128){ strncpy(g_extn[g_next],r,159); g_extn[g_next][159]=0; g_ext[g_next]=decl; g_next++; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_disposeString(sp);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void field_tautype(CXType t){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType c=clang_getCanonicalType(t);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(c.kind==CXType_ConstantArray){ CXType e=clang_getArrayElementType(c); long long n=clang_getArraySize(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"[\"); field_tautype(e); printf(\"; %lld]\", n); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(c.kind==CXType_Pointer){ printf(\"Pointer[void]\"); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  const char* sc=scalar(c.kind); if(sc[0]){ printf(\"%s\", sc); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(c.kind==CXType_Enum){ printf(\"c_int\"); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(c.kind==CXType_Record){ CXString sp=clang_getTypeSpelling(c); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    const char* seg=strrchr(r,':'); printf(\"%s\", seg?seg+1:r); clang_disposeString(sp); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  printf(\"c_int\");\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult sfields(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)!=CX_CXXPrivate && clang_getCXXAccessSpecifier(c)!=CX_CXXProtected){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"SFIELD \"); field_tautype(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void emit_extstructs(void){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(int i=0;i<g_next;i++){ printf(\"XSTRUCT %s\\n\", g_extn[i]); clang_visitChildren(g_ext[i], sfields, 0); printf(\"EXSTRUCT\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* whole-identifier-token search: does `needle` appear in `hay` bounded by non-ident chars? */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int has_token(const char* hay, const char* needle){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  int nl=strlen(needle); if(nl==0) return 0; const char* p=hay;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  while((p=strstr(p,needle))){ char b=(p==hay)?0:p[-1]; char a=p[nl];\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int lb=!((b>='A'&&b<='Z')||(b>='a'&&b<='z')||(b>='0'&&b<='9')||b=='_');\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int la=!((a>='A'&&a<='Z')||(a>='a'&&a<='z')||(a>='0'&&a<='9')||a=='_');\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(lb&&la) return 1; p+=nl; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return 0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_noncopy;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult copyctorck(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(c)==CXCursor_Constructor && clang_CXXConstructor_isCopyConstructor(c)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    enum CXAvailabilityKind av=clang_getCursorAvailability(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    enum CX_CXXAccessSpecifier acc=clang_getCXXAccessSpecifier(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(av==CXAvailability_NotAvailable || acc==CX_CXXPrivate || acc==CX_CXXProtected) g_noncopy=1; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int is_noncopyable(CXType t){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXCursor dc=clang_getTypeDeclaration(t); CXCursor def=clang_getCursorDefinition(dc); if(clang_Cursor_isNull(def)) def=dc;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  g_noncopy=0; clang_visitChildren(def, copyctorck, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(!g_noncopy){ CXCursor tm=clang_getSpecializedCursorTemplate(dc); if(!clang_Cursor_isNull(tm)){ CXCursor td=clang_getCursorDefinition(tm); if(clang_Cursor_isNull(td)) td=tm; clang_visitChildren(td, copyctorck, 0); } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return g_noncopy; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void classify(int depth,int ref,CXType cur){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  { CXString _ps=clang_getTypeSpelling(cur); int _pk=(strstr(clang_getCString(_ps),\"...\")!=0); clang_disposeString(_ps); if(_pk){ printf(\"%d~%d~d~\", depth, ref); return; } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXTypeKind k=cur.kind; const char* sc=scalar(k);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXType_Void) printf(\"%d~%d~v~\", depth, ref);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(sc[0]) printf(\"%d~%d~p~%s\", depth, ref, sc);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXType_Enum){ CXType u=clang_getEnumDeclIntegerType(clang_getTypeDeclaration(cur)); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\";\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~e~%s#%s\", depth, ref, S(sp), us); clang_disposeString(sp); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXType_Record){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(depth==0 && clang_Type_getSizeOf(cur)==CXTypeLayoutError_Incomplete){ CXCursor _td=clang_getTypeDeclaration(cur); CXCursor _tm=clang_getSpecializedCursorTemplate(_td); if(clang_Cursor_isNull(_tm) || clang_Cursor_isNull(clang_getCursorDefinition(_tm))){ printf(\"%d~%d~d~\", depth, ref); return; } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(depth==0 && is_noncopyable(cur)){ printf(\"%d~%d~d~\", depth, ref); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXString sp=clang_getTypeSpelling(cur); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(strncmp(r,\"std::function<\",14)==0 && clang_Type_getNumTemplateArguments(cur)>=1){ CXType _ft=clang_Type_getTemplateArgumentAsType(cur,0); CXString _fs=clang_getTypeSpelling(_ft); printf(\"%d~%d~f~%s\", depth, ref, S(_fs)); clang_disposeString(_fs); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(strncmp(r,\"std::basic_string\",17)==0 && (strstr(r,\"basic_string<char,\")||strstr(r,\"basic_string<char>\")||strstr(r,\"basic_string_view<char,\")||strstr(r,\"basic_string_view<char>\"))){ if(ref==1){ printf(\"%d~%d~d~\", depth, ref); } else { printf(\"%d~%d~s~string\", depth, ref); } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else printf(\"%d~%d~r~%s\", depth, ref, r); clang_disposeString(sp); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXType_FunctionProto){ CXString sp=clang_getTypeSpelling(clang_getCanonicalType(cur)); printf(\"%d~%d~f~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else { CXString sp=clang_getTypeSpelling(cur); printf(\"%d~%d~u~%s\", depth, ref, S(sp)); clang_disposeString(sp); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void classify_arg(int depth,int ref,CXType t){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType c=clang_getCanonicalType(t);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(c.kind==CXType_LValueReference||c.kind==CXType_RValueReference){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXType pt=clang_getPointeeType(c); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; c=clang_getCanonicalType(pt); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  classify(depth,ref,c); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void tds(CXType t){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  // ref: 0 = not-a-reference (value or pointer); 1 = mutable `T&`; 2 = const `const T&`.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  int depth=0,ref=0; CXType cur=t;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  while(1){ CXType cc=clang_getCanonicalType(cur);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(cc.kind==CXType_Pointer){depth++; CXType pt=clang_getPointeeType(cur); if(pt.kind==CXType_Invalid) pt=clang_getPointeeType(cc); cur=pt;}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(cc.kind==CXType_LValueReference||cc.kind==CXType_RValueReference){ CXType pt=clang_getPointeeType(cur); ref=clang_isConstQualifiedType(clang_getCanonicalType(pt))?2:1; depth++; cur=pt; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else if(cc.kind==CXType_ConstantArray||cc.kind==CXType_IncompleteArray||cc.kind==CXType_VariableArray||cc.kind==CXType_DependentSizedArray){ depth++; cur=clang_getArrayElementType(cc); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    else break; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType fcan=clang_getCanonicalType(cur);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(fcan.kind==CXType_MemberPointer){ printf(\"%d~%d~d~\", depth, ref); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(g_np>0){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // A template-parameter-dependent type has canonical spelling \"type-parameter-0-<i>\" (even\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // when written via a typedef like value_type/reference) — substitute the concrete arg i.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXString sp=clang_getTypeSpelling(fcan); const char* r=S(sp); if(strncmp(r,\"const \",6)==0)r+=6; if(strncmp(r,\"volatile \",9)==0)r+=9;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(strncmp(r,\"type-parameter-0-\",17)==0){ int idx=atoi(r+17); if(idx>=0 && idx<g_np){ clang_disposeString(sp); classify_arg(depth,ref,g_args[idx]); return; } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // fallback: match the parameter name literally (direct `T` uses)\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    for(int i=0;i<g_np;i++){ if(strcmp(r,g_params[i])==0){ clang_disposeString(sp); classify_arg(depth,ref,g_args[i]); return; } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // Sequence-container element accessors: `X::reference`/`::const_reference` = `value_type&` and\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // `::pointer`/`::const_pointer` = `value_type*` — libstdc++ routes these through an\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // `__alloc_traits<…>` typedef that stays dependent, but for vector/string/deque/list/set the\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // element type is the FIRST template arg. Resolve to it so at()/front()/back()/[] bind.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    { const char* seg=strrchr(r,':'); if(g_np>0 && seg && seg>r+1 && seg[-1]==':'){ seg++;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(strcmp(seg,\"reference\")==0||strcmp(seg,\"const_reference\")==0){ clang_disposeString(sp); classify(depth+1,1,clang_getCanonicalType(g_args[0])); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(strcmp(seg,\"pointer\")==0||strcmp(seg,\"const_pointer\")==0){ clang_disposeString(sp); classify(depth+1,0,clang_getCanonicalType(g_args[0])); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        // container `size_type`/`difference_type` also route through dependent allocator traits.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(strcmp(seg,\"size_type\")==0){ printf(\"%d~%d~p~c_size_t\", depth, ref); clang_disposeString(sp); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        if(strcmp(seg,\"difference_type\")==0){ printf(\"%d~%d~p~c_ptrdiff_t\", depth, ref); clang_disposeString(sp); return; } } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // Any type still carrying an un-substituted template param (self-type copy-ctor\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // `vector<_Tp,_Alloc>`, or an internal `__alloc_traits<…>` type — often kind Unexposed,\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // not Record) is dependent -> mark 'd' so the generator skips the whole method (its shim\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // would be un-nameable). Applies regardless of kind.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int dep=has_token(r,\"type-parameter\");\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    for(int i=0;i<g_np && !dep;i++) dep=has_token(r,g_params[i]);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // a NESTED type of the container written without template args (`std::map::value_compare`,\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // an uninstantiated `iterator`) is un-nameable in the shim -> skip the method.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(!dep && g_cbase[0]){ char nb[262]; snprintf(nb,sizeof nb,\"%s::\",g_cbase); if(strstr(r,nb)) dep=1; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(dep){ printf(\"%d~%d~d~\", depth, ref); clang_disposeString(sp); return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_disposeString(sp);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(fcan.kind==CXType_Record && depth==0) collect_ext(fcan);  /* by-value external record -> lay out */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  classify(depth,ref,fcan);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void params(CXCursor c){ int n=clang_Cursor_getNumArguments(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(int i=0;i<n;i++){ CXCursor a=clang_Cursor_getArgument(c,i); CXString nm=clang_getCursorSpelling(a); const char* nn=S(nm);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"PARAM \"); tds(clang_getArgType(clang_getCursorType(c),i)); if(nn[0])printf(\"|%s\\n\",nn); else printf(\"|a%d\\n\",i); clang_disposeString(nm); } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* A parameter with a default argument has a child that is neither a type/namespace/template ref\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   (those describe the type) — it's the default-value expression. Count the TRAILING defaults (C++\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   requires defaults to be trailing) so the generator can emit shorter overloads. */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_hasdef;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult defck(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k!=CXCursor_TypeRef && k!=CXCursor_NamespaceRef && k!=CXCursor_TemplateRef && k!=CXCursor_ParmDecl) g_hasdef=1;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int count_defaults(CXCursor c){ int n=clang_Cursor_getNumArguments(c), d=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(int i=n-1;i>=0;i--){ g_hasdef=0; clang_visitChildren(clang_Cursor_getArgument(c,i), defck, 0); if(g_hasdef) d++; else break; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return d; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult fld(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(c)==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult gvar(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(c)==CXCursor_VarDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* Inherited-method support: bind a derived class's PUBLIC base methods too (the shim calls them\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   through the derived pointer — public inheritance upcasts). Dedup by name so an override / diamond\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   base isn't bound twice; own methods are recorded first so they win. */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static char g_seenm[512][96]; static int g_nseenm=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_abstract=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static char g_curclass[256]={0};\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int seen_method(const char* nm){ for(int i=0;i<g_nseenm;i++) if(strcmp(g_seenm[i],nm)==0) return 1; if(g_nseenm<512){ strncpy(g_seenm[g_nseenm],nm,95); g_seenm[g_nseenm][95]=0; g_nseenm++; } return 0; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void emit_method(CXCursor c){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if((k==CXCursor_CXXMethod||k==CXCursor_Constructor) && clang_getCursorAvailability(c)==CXAvailability_NotAvailable) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c)));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    { CXString mn=clang_getCursorSpelling(c); CXString pn=clang_getCursorSpelling(clang_getCursorSemanticParent(c)); const char* pns=S(pn);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(g_curclass[0] && pns[0] && strcmp(pns,g_curclass)!=0) printf(\"|%s|%s\\n\", S(mn), pns); else printf(\"|%s\\n\", S(mn));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      clang_disposeString(mn); clang_disposeString(pn); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    params(c); printf(\"EMETHOD\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !g_abstract){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("}\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult ownmeth(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) seen_method(S(clang_getCursorSpelling(c)));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_CXXMethod||k==CXCursor_Constructor||k==CXCursor_Destructor) emit_method(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void emit_inherited(CXCursor basespec);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult inheritm(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic && !clang_CXXMethod_isStatic(c)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    const char* nm=S(clang_getCursorSpelling(c));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(nm[0]!='~' && !seen_method(nm)) emit_method(c); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void emit_inherited(CXCursor basespec){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType bt=clang_getCursorType(basespec);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXString bs=clang_getTypeSpelling(bt); const char* br=S(bs); int istmpl=(strstr(br,\"<\")!=NULL); clang_disposeString(bs);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(istmpl) return;                            /* skip template-base inheritance (kept simple) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_visitChildren(bdef, inheritm, 0); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult basespec(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(c)==CXCursor_CXXBaseSpecifier && clang_getCXXAccessSpecifier(c)==CX_CXXPublic) emit_inherited(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void collect_prot_base(CXCursor bs);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult collectprot(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_CXXMethod||k==CXCursor_UsingDeclaration){ enum CX_CXXAccessSpecifier a=clang_getCXXAccessSpecifier(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(a==CX_CXXProtected||a==CX_CXXPrivate) seen_method(S(clang_getCursorSpelling(c))); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_CXXBaseSpecifier) collect_prot_base(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void collect_prot_base(CXCursor bs){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType bt=clang_getCursorType(bs);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXString sp=clang_getTypeSpelling(bt); const char* r=S(sp); int istmpl=(strstr(r,\"<\")!=NULL); clang_disposeString(sp);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(istmpl) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXCursor bd=clang_getTypeDeclaration(bt); CXCursor bdef=clang_getCursorDefinition(bd); if(clang_Cursor_isNull(bdef)) bdef=bd;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_visitChildren(bdef, collectprot, 0); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_ntp; static char g_tpname[64];\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult ftcount(CXCursor cc, CXCursor pp, CXClientData dd){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter){ g_ntp++; strncpy(g_tpname, S(clang_getCursorSpelling(cc)), 63); g_tpname[63]=0; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int is_tparam(CXType t){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(t.kind==CXType_LValueReference||t.kind==CXType_RValueReference) t=clang_getPointeeType(t);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXString s=clang_getTypeSpelling(t); const char* r=S(s); if(strncmp(r,\"const \",6)==0)r+=6;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  int ok=(strcmp(r,g_tpname)==0); clang_disposeString(s); return ok; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static void emit_ftmpl(CXCursor c, const char* nm){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  g_ntp=0; g_tpname[0]=0; clang_visitChildren(c, ftcount, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(g_ntp!=1 || !g_tpname[0]) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXType ft=clang_getCursorType(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(!is_tparam(clang_getResultType(ft))) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  int np=clang_getNumArgTypes(ft); if(np<0) return;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(int i=0;i<np;i++){ if(!is_tparam(clang_getArgType(ft,i))) return; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  printf(\"FTMPL %d|%s\\n\", np, nm); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult _tpk_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind kk=clang_getCursorKind(cc);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(kk==CXCursor_TemplateTypeParameter||kk==CXCursor_NonTypeTemplateParameter||kk==CXCursor_TemplateTemplateParameter){ *(int*)dd=1; return CXChildVisit_Break; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("/* An uninstantiated primary/partial class template (has template-parameter children) can't be\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   bound as a concrete class — its methods would reference the bare template name (`__gmp_expr`\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("   without <args>). Only concrete classes and full instantiations are bindable. */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int is_template_pattern(CXCursor c){ int f=0; clang_visitChildren(c,_tpk_cb,&f); return f; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult nested(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)) || k==CXCursor_EnumDecl) return visit(c,p,d);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult visit(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(!clang_Location_isFromMainFile(clang_getCursorLocation(c))) return CXChildVisit_Continue;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c); CXString nmS=clang_getCursorSpelling(c); const char* nm=S(nmS);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_ClassTemplate||k==CXCursor_ClassTemplatePartialSpecialization){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_LinkageSpec){ clang_visitChildren(c,visit,0); clang_disposeString(nmS); return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_Namespace){ printf(\"NS %s\\n\",nm); clang_visitChildren(c,visit,0); printf(\"ENS\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if((k==CXCursor_ClassDecl||k==CXCursor_StructDecl)&&clang_isCursorDefinition(c)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(is_template_pattern(c)){ clang_disposeString(nmS); return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXType rt=clang_getCursorType(c); if(!clang_Cursor_isNull(clang_getSpecializedCursorTemplate(c))){ CXString tsx=clang_getTypeSpelling(rt); printf(\"TCLASS %s %d\\n\", clang_getCString(tsx), clang_isPODType(rt)); clang_disposeString(tsx); } else { printf(\"CLASS %s %d\\n\",nm, clang_isPODType(rt)); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int saved=g_nseenm; g_nseenm=0;             /* per-class method-name dedup set */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    int savedabs=g_abstract; g_abstract=clang_CXXRecord_isAbstract(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    char savedcc[256]; strncpy(savedcc,g_curclass,255); savedcc[255]=0; strncpy(g_curclass,nm,255); g_curclass[255]=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,fld,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,gvar,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,ownmeth,0);           /* own methods/ctors/dtor (records names) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,collectprot,0);       /* exclude names re-declared protected in a base */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,basespec,0);          /* inherited public methods (deduped) */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_visitChildren(c,nested,0);            /* nested classes + enums */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    g_nseenm=saved; g_abstract=savedabs; strncpy(g_curclass,savedcc,255); g_curclass[255]=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"ECLASS\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EMETHOD\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_Destructor) printf(\"DTOR\\n\");\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_FunctionDecl){ printf(\"FUNC %d \", count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\",nm); params(c); printf(\"EFUNC\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_VarDecl){ printf(\"GVAR \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\",nm); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_FunctionTemplate) emit_ftmpl(c, nm);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_EnumDecl){ CXType u=clang_getEnumDeclIntegerType(c); const char* us=scalar(u.kind); if(!us[0])us=\"c_int\"; printf(\"ENUM %s %s\\n\", nm[0]?nm:\"anon\", us); clang_visitChildren(c,visit,0); printf(\"EENUM\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_EnumConstantDecl) printf(\"EVAL %s %lld\\n\", nm, (long long)clang_getEnumConstantDeclValue(c));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_TypedefDecl || k==CXCursor_TypeAliasDecl){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // `typedef basic_stringstream<char> stringstream;` — a typedef to a template specialization\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // that no signature references. Emit a TDSPEC hint so bindgen force-instantiates + binds it\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    // (fixes template-only headers like <sstream>/<random>). std::string is handled by its typemap.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXType u=clang_getCanonicalType(clang_getTypedefDeclUnderlyingType(c));\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(u.kind==CXType_Record && clang_Type_getNumTemplateArguments(u)>0){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      CXString us=clang_getTypeSpelling(u); const char* r=S(us);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(strncmp(r,\"std::basic_string<char\",22)!=0) printf(\"TDSPEC %s\\n\", r);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      clang_disposeString(us); } }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_disposeString(nmS); return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult collectparams(CXCursor cc, CXCursor pp, CXClientData dd){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(clang_getCursorKind(cc)==CXCursor_TemplateTypeParameter && g_np<8){ strncpy(g_params[g_np], S(clang_getCursorSpelling(cc)), 63); g_params[g_np][63]=0; g_np++; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult imeth(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind k=clang_getCursorKind(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(k==CXCursor_FieldDecl && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"FIELD \"); tds(clang_getCursorType(c)); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_CXXMethod && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    { CXString ms=clang_getCursorSpelling(c); const char* mn=clang_getCString(ms);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(strcmp(mn,\"begin\")==0) g_hasbegin=1; else if(strcmp(mn,\"end\")==0) g_hasend=1;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      clang_disposeString(ms); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    printf(\"METHOD %c%c %d \", clang_CXXMethod_isStatic(c)?'s':'.', clang_CXXMethod_isConst(c)?'c':'.', count_defaults(c)); tds(clang_getResultType(clang_getCursorType(c))); printf(\"|%s\\n\", S(clang_getCursorSpelling(c))); params(c); printf(\"EMETHOD\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  else if(k==CXCursor_Constructor && clang_getCXXAccessSpecifier(c)==CX_CXXPublic){ printf(\"CTOR %d\\n\", count_defaults(c)); params(c); printf(\"ECTOR\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static int g_ownmc;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult ownmc_cb(CXCursor cc, CXCursor pp, CXClientData dd){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  enum CXCursorKind kk=clang_getCursorKind(cc);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(kk==CXCursor_CXXMethod||kk==CXCursor_Constructor) g_ownmc++;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("static enum CXChildVisitResult inst(CXCursor c, CXCursor p, CXClientData d){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if((clang_getCursorKind(c)==CXCursor_ClassDecl || clang_getCursorKind(c)==CXCursor_StructDecl) && clang_isCursorDefinition(c)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(is_template_pattern(c)) return CXChildVisit_Continue;   /* partial spec / primary pattern (`__gmp_expr<T,T>`) — not a concrete instantiation */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    CXCursor tmpl=clang_getSpecializedCursorTemplate(c);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(!clang_Cursor_isNull(tmpl)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // `getSpecializedCursorTemplate` may return a forward DECLARATION of the template (true for\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // std::map/set), which has no member cursors; get the DEFINITION so its methods are visible.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      CXCursor def=clang_getCursorDefinition(tmpl); if(clang_Cursor_isNull(def)) def=tmpl;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      CXType ct=clang_getCursorType(c); g_np=0; clang_visitChildren(def, collectparams, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      int na=clang_Type_getNumTemplateArguments(ct); for(int i=0;i<na && i<8;i++) g_args[i]=clang_Type_getTemplateArgumentAsType(ct,i);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      CXString cts=clang_getTypeSpelling(ct); { const char* cs=S(cts); const char* lt=strchr(cs,'<'); int bl=lt?(int)(lt-cs):(int)strlen(cs); if(bl>255)bl=255; memcpy(g_cbase,cs,bl); g_cbase[bl]=0; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      printf(\"TCLASS %s %d\\n\", S(cts), clang_isPODType(ct)); clang_disposeString(cts);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      g_hasbegin=0; g_hasend=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // Explicit specialization (own members>0) OVERRIDES the primary -> bind ITS members\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // (concrete, no substitution); implicit instantiation (0) -> primary + arg substitution.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      g_ownmc=0; clang_visitChildren(c, ownmc_cb, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      CXCursor msrc = (g_ownmc>0) ? c : def;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      clang_visitChildren(msrc, imeth, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // Smart pointers: `get()` returns `element_type*` via a metafunction-dependent typedef that\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // libclang won't reduce, and it's inherited from a template base — so synthesize it directly.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      // The element type is the first template arg (a pointer to it). Enables extracting the pointee.\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(na>0 && (strcmp(g_cbase,\"std::shared_ptr\")==0 || strcmp(g_cbase,\"std::unique_ptr\")==0 || strcmp(g_cbase,\"std::weak_ptr\")==0)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"METHOD .c \"); classify(1,0,clang_getCanonicalType(g_args[0])); printf(\"|get\\n\"); printf(\"EMETHOD\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(na>0 && g_hasbegin && g_hasend &&\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("         (strcmp(g_cbase,\"std::list\")==0||strcmp(g_cbase,\"std::forward_list\")==0||\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("          strcmp(g_cbase,\"std::set\")==0||strcmp(g_cbase,\"std::multiset\")==0||\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("          strcmp(g_cbase,\"std::unordered_set\")==0||strcmp(g_cbase,\"std::unordered_multiset\")==0)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"ITER \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      if(na>=2 && g_hasbegin && g_hasend &&\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("         (strcmp(g_cbase,\"std::map\")==0||strcmp(g_cbase,\"std::multimap\")==0||\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("          strcmp(g_cbase,\"std::unordered_map\")==0||strcmp(g_cbase,\"std::unordered_multimap\")==0)){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"ITERMAPKEY \"); classify(0,0,clang_getCanonicalType(g_args[0])); printf(\"\\n\");\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("        printf(\"ITERMAPVAL \"); classify(0,0,clang_getCanonicalType(g_args[1])); printf(\"\\n\"); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("      printf(\"ECLASS\\n\"); g_np=0; g_cbase[0]=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  return CXChildVisit_Continue; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("int main(int argc,char**argv){\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  int inst_mode=0, off=1;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(argc>1 && strcmp(argv[1],\"--inst\")==0){ inst_mode=1; off=2; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(argc<off+1){ fprintf(stderr,\"usage: cxxwalk [--inst] <header> [clang args...]\\n\"); return 2; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXIndex idx=clang_createIndex(0,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  CXTranslationUnit tu=clang_parseTranslationUnit(idx,argv[off],(const char**)(argv+off+1),argc-off-1,0,0,0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(!tu){ fprintf(stderr,\"cxxwalk: libclang could not create a translation unit\\n\"); return 1; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  unsigned nd=clang_getNumDiagnostics(tu), nerr=0, nfatal=0;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  for(unsigned i=0;i<nd;i++){ CXDiagnostic dg=clang_getDiagnostic(tu,i); enum CXDiagnosticSeverity sv=clang_getDiagnosticSeverity(dg);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    if(sv>=CXDiagnostic_Error){ CXString ds=clang_formatDiagnostic(dg,clang_defaultDiagnosticDisplayOptions()); fprintf(stderr,\"DIAG %s\\n\",clang_getCString(ds)); clang_disposeString(ds); nerr++; if(sv==CXDiagnostic_Fatal) nfatal++; }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("    clang_disposeDiagnostic(dg); }\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  fprintf(stderr,\"DIAGERR %u %u\\n\", nerr, nfatal);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  g_inst=inst_mode;\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_visitChildren(clang_getTranslationUnitCursor(tu), inst_mode?inst:visit, 0);\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  if(!inst_mode) emit_extstructs();   /* lay out by-value external (non-system) records */\n"));
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("  clang_disposeTranslationUnit(tu); clang_disposeIndex(idx); return 0; }\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(sb));
}

__attribute__((hot)) void _rm_files(TrStr files) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        ({ TrStr _aet_t966 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("del /q ")), _tr_strz(files))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" >nul 2>&1"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t966.data); _tr_str_release(_aet_t966); });
    } else {
        /* pass */
        ({ TrStr _aet_t967 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("rm -f ")), _tr_strz(files))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" 2>/dev/null"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t967.data); _tr_str_release(_aet_t967); });
    }
}

__attribute__((hot)) TrStr _local_exe(TrStr stem) {
    /* pass */
    if (_tr_is_windows()) {
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit(".\\")), _tr_strz(stem))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".exe"))); _tr_str_release(_cl); _cres; });
    }
    /* pass */
    return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("./")), _tr_strz(stem))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".exe"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr _detect_libclang(TrStr cc) {
    /* pass */
    write_file(_tr_str_lit("_cxxprobe.c"), _tr_str_lit("#include <clang-c/Index.h>\nint main(){ clang_createIndex(0,0); return 0; }\n"));
    /* pass */
    long long rc = ({ TrStr _aet_t968 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" _cxxprobe.c -o _cxxprobe.exe -lclang 2>_cxx_err.txt")))); __auto_type _wr = (_tr_system(_aet_t968.data)); _tr_str_release(_aet_t968); _wr; });
    /* pass */
    _rm_files(_tr_str_lit("_cxxprobe.c _cxxprobe.exe _cxx_err.txt"));
    /* pass */
    if ((rc == 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    StringBuilder* g = StringBuilder_init(768LL);
    /* pass */
    StringBuilder_append(g, _tr_str_lit("bindgen: `-h cpp` needs libclang (Clang's C API), which was not found.\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("         It is required ONLY for C++ headers; plain C headers never use it.\n\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Install it:\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Windows       winget install LLVM.LLVM   (or MSYS2: pacman -S mingw-w64-x86_64-clang)\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Debian/Ubuntu sudo apt install libclang-dev\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Fedora        sudo dnf install clang-devel\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  Arch          sudo pacman -S clang\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("  macOS         brew install llvm   (or Xcode Command Line Tools)\n\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Download        https://github.com/llvm/llvm-project/releases\n"));
    /* pass */
    StringBuilder_append(g, _tr_str_lit("Then re-run:    tauraroc bindgen <header.hpp> -h cpp\n"));
    /* pass */
    return StringObj_as_str(StringBuilder_to_string(g));
}

__attribute__((hot)) CppType* _cpp_parse_type(TrStr spelling) {
    /* pass */
    CppType* t = ((CppType*)_tr_obj_alloc(sizeof(CppType)));
    /* pass */
    t->base = _tr_str_lit("");
    /* pass */
    t->ptr = 0LL;
    /* pass */
    t->was_ptr = false;
    /* pass */
    t->was_ref = false;
    /* pass */
    t->is_prim = false;
    /* pass */
    TrStr norm = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(spelling)))) {
        /* pass */
        TrStr ch = _tr_str_wrap(_tr_str_slice(_tr_strz(spelling), i, (i + 1LL)));
        /* pass */
        if ((strcmp(_tr_strz(ch), _tr_strz(_tr_str_lit("*"))) == 0)) {
            /* pass */
            TrStr _strtmp_t969 = _tr_strx_concat(_tr_strz(norm), _tr_strz(_tr_str_lit(" * ")));
            _tr_str_release(norm);
            norm = _strtmp_t969;
        } else if ((strcmp(_tr_strz(ch), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            TrStr _strtmp_t970 = _tr_strx_concat(_tr_strz(norm), _tr_strz(_tr_str_lit(" & ")));
            _tr_str_release(norm);
            norm = _strtmp_t970;
        } else {
            /* pass */
            TrStr _strtmp_t971 = _tr_strx_concat(_tr_strz(norm), _tr_strz(ch));
            _tr_str_release(norm);
            norm = _strtmp_t971;
        }
        /* pass */
        i = (i + 1LL);
        _tr_str_release(ch);
    }
    /* pass */
    List_TrStr* words = _tr_str_split(_tr_strz(norm), _tr_strz(_tr_str_lit(" ")));
    /* pass */
    TrStr bw = _tr_str_lit("");
    /* pass */
    long long wi = 0LL;
    /* pass */
    while ((wi < words->len)) {
        /* pass */
        TrStr w = List_TrStr_get(words, wi);
        /* pass */
        wi = (wi + 1LL);
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if ((((((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("const"))) == 0) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("volatile"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("struct"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("class"))) == 0)) || (strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("enum"))) == 0))) {
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("*"))) == 0)) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ptr = true;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(w), _tr_strz(_tr_str_lit("&"))) == 0)) {
            /* pass */
            t->ptr = (t->ptr + 1LL);
            /* pass */
            t->was_ref = true;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t972 = _tr_str_retain(w);
            _tr_str_release(bw);
            bw = _strtmp_t972;
        } else {
            /* pass */
            TrStr _strtmp_t973 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(bw), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(w)); _tr_str_release(_cl); _cres; });
            _tr_str_release(bw);
            bw = _strtmp_t973;
        }
        _tr_str_release(w);
    }
    /* pass */
    t->base = _tr_str_retain(bw);
    /* pass */
    TrStr mb = map_base(bw);
    /* pass */
    if ((((strcmp(_tr_strz(mb), _tr_strz(bw)) != 0) || (strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit("void"))) == 0)) || (strcmp(_tr_strz(bw), _tr_strz(_tr_str_lit("bool"))) == 0))) {
        /* pass */
        t->is_prim = true;
    }
    /* pass */
    _tr_str_release(norm);
    List_TrStr_free(words);
    _tr_str_release(bw);
    _tr_str_release(mb);
    return t;
}

__attribute__((hot)) TrStr _last_seg(TrStr s) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(s), _tr_strz(_tr_str_lit("::")));
    /* pass */
    if ((parts->len == 0LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_retain(s);
    }
    /* pass */
    return List_TrStr_get(parts, (parts->len - 1LL));
}

__attribute__((hot)) TrStr _cpp_op_name(TrStr mname, long long nparams, bool is_member) {
    /* pass */
    if ((_tr_strlen(_tr_strz(mname)) < 9LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    TrStr g = _tr_str_wrap(_tr_str_slice(_tr_strz(mname), 8LL, _tr_strlen(_tr_strz(mname))));
    /* pass */
    char c0 = _tr_strz(g)[0LL];
    /* pass */
    if (((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 32LL))) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("");
    }
    /* pass */
    bool unary = false;
    /* pass */
    if (is_member) {
        /* pass */
        unary = (nparams == 0LL);
    } else {
        /* pass */
        unary = (nparams <= 1LL);
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("[]"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_index");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("()"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_call");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("->"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_arrow");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("++"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_inc");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("--"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_dec");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("=="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_eq");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("!="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_ne");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("<="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_le");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit(">="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_ge");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("<"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_lt");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit(">"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_gt");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("<<"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_lshift");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit(">>"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_rshift");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("+="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_iadd");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("-="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_isub");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("*="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_imul");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("/="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_idiv");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("%="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_imod");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("&="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_iand");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("|="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_ior");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("^="))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_ixor");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("*"))) == 0)) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit("op_deref");
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_mul");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("+"))) == 0)) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit("op_pos");
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_add");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("-"))) == 0)) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit("op_neg");
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_sub");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("/"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_div");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("%"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_mod");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("&"))) == 0)) {
        /* pass */
        if (unary) {
            /* pass */
            _tr_str_release(g);
            return _tr_str_lit("");
        }
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_bitand");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("|"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_bitor");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("^"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_xor");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("~"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_bitnot");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("!"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_lnot");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("&&"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_land");
    }
    /* pass */
    if ((strcmp(_tr_strz(g), _tr_strz(_tr_str_lit("||"))) == 0)) {
        /* pass */
        _tr_str_release(g);
        return _tr_str_lit("op_lor");
    }
    /* pass */
    _tr_str_release(g);
    return _tr_str_lit("");
}

__attribute__((hot)) TrStr _cpp_ident(TrStr s) {
    /* pass */
    TrStr out = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL))) {
            /* pass */
            TrStr _strtmp_t974 = ({ TrStr _cr = (_tr_str_wrap(_tr_str_slice(_tr_strz(s), i, (i + 1LL)))); TrStr _cres = _tr_strx_concat(_tr_strz(out), _cr.data); _tr_str_release(_cr); _cres; });
            _tr_str_release(out);
            out = _strtmp_t974;
        } else {
            /* pass */
            TrStr _strtmp_t975 = _tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit("_")));
            _tr_str_release(out);
            out = _strtmp_t975;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((strcmp(_tr_strz(out), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        _tr_str_release(out);
        return _tr_str_lit("T_");
    }
    /* pass */
    char c0 = _tr_strz(out)[0LL];
    /* pass */
    if (((c0 >= 48LL) && (c0 <= 57LL))) {
        /* pass */
        return _tr_strx_concat(_tr_strz(_tr_str_lit("T_")), _tr_strz(out));
    }
    /* pass */
    return out;
}

__attribute__((hot)) bool _is_clean_ident(TrStr s) {
    /* pass */
    if ((_tr_strlen(_tr_strz(s)) == 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    char c0 = _tr_strz(s)[0LL];
    /* pass */
    if ((!((((c0 >= 65LL) && (c0 <= 90LL)) || ((c0 >= 97LL) && (c0 <= 122LL))) || (c0 == 95LL)))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 1LL;
    /* pass */
    while ((i < _tr_strlen(_tr_strz(s)))) {
        /* pass */
        char c = _tr_strz(s)[i];
        /* pass */
        if ((!(((((c >= 65LL) && (c <= 90LL)) || ((c >= 97LL) && (c <= 122LL))) || ((c >= 48LL) && (c <= 57LL))) || (c == 95LL)))) {
            /* pass */
            return false;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return true;
}

__attribute__((hot)) bool _is_tr_keyword(TrStr n) {
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("in"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("is"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("as"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("or"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("and"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("not"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("if"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("else"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("elif"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("for"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("while"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("match"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("case"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("def"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("return"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("class"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("enum"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("type"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("interface"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("mut"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("pub"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("ref"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("from"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("import"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("pass"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("break"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("continue"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("true"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("false"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("none"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("super"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("with"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("try"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("raise"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("async"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("await"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if (((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("var"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("do"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("loop"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("defer"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("actor"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("decorator"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    if ((((((strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("extend"))) == 0) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("implements"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("extends"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("throws"))) == 0)) || (strcmp(_tr_strz(n), _tr_strz(_tr_str_lit("spawn"))) == 0))) {
        /* pass */
        return true;
    }
    /* pass */
    return false;
}

__attribute__((hot)) TrStr _uniq_sym(TrStr base_sym, TrMap* used) {
    /* pass */
    if (_tr_dict_contains(used, _tr_strz(base_sym))) {
        /* pass */
        long long n = (((long long)(uintptr_t)_tr_dict_get(used, _tr_strz(base_sym))) + 1LL);
        /* pass */
        _tr_dict_set(used, _tr_strz(base_sym), n);
        /* pass */
        return ({ TrStr _cl = (_tr_strx_concat(_tr_strz(base_sym), _tr_strz(_tr_str_lit("_")))); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    }
    /* pass */
    _tr_dict_set(used, _tr_strz(base_sym), 1LL);
    /* pass */
    return _tr_str_retain(base_sym);
}

__attribute__((hot)) TrStr _cpp_tr_pname(TrStr pname) {
    /* pass */
    if ((((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit("self"))) == 0) || (strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit("obj"))) == 0)) || _is_tr_keyword(pname))) {
        /* pass */
        return _tr_strx_concat(_tr_strz(pname), _tr_strz(_tr_str_lit("_")));
    }
    /* pass */
    return _tr_str_retain(pname);
}

__attribute__((hot)) TrStr _cpp_ctype(CppType* t) {
    /* pass */
    TrStr s = _tr_str_retain(t->base);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < t->ptr)) {
        /* pass */
        TrStr _strtmp_t976 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("*")));
        _tr_str_release(s);
        s = _strtmp_t976;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _cpp_tr_type(CppType* t) {
    /* pass */
    if (t->is_prim) {
        /* pass */
        return map_type(t->base, t->ptr);
    }
    /* pass */
    long long eff = t->ptr;
    /* pass */
    if ((eff < 1LL)) {
        /* pass */
        eff = 1LL;
    }
    /* pass */
    TrStr ty = _last_seg(t->base);
    /* pass */
    long long k = 1LL;
    /* pass */
    while ((k < eff)) {
        /* pass */
        TrStr _strtmp_t977 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t977;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    return ty;
}

__attribute__((hot)) List_TrStr* _cpp_ret(CppType* rt, TrStr call) {
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if (((strcmp(_tr_strz(rt->base), _tr_strz(_tr_str_lit("void"))) == 0) && (rt->ptr == 0LL))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("void"));
        /* pass */
        ({ TrStr _at_t978 = (_tr_strx_concat(_tr_strz(call), _tr_strz(_tr_str_lit(";")))); List_TrStr_append(r, _at_t978); _tr_str_release(_at_t978); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit(""));
        /* pass */
        return r;
    }
    /* pass */
    if (rt->is_prim) {
        /* pass */
        ({ TrStr _at_t979 = (_cpp_ctype(rt)); List_TrStr_append(r, _at_t979); _tr_str_release(_at_t979); });
        /* pass */
        ({ TrStr _at_t980 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t980); _tr_str_release(_at_t980); });
        /* pass */
        ({ TrStr _at_t981 = (_cpp_tr_type(rt)); List_TrStr_append(r, _at_t981); _tr_str_release(_at_t981); });
        /* pass */
        return r;
    }
    /* pass */
    TrStr trret = _cpp_tr_type(rt);
    /* pass */
    if (rt->was_ptr) {
        /* pass */
        ({ TrStr _at_t982 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t982); _tr_str_release(_at_t982); });
        /* pass */
        ({ TrStr _at_t983 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t983); _tr_str_release(_at_t983); });
        /* pass */
        List_TrStr_append(r, trret);
    } else if (rt->was_ref) {
        /* pass */
        ({ TrStr _at_t984 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t984); _tr_str_release(_at_t984); });
        /* pass */
        ({ TrStr _at_t985 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return &(")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t985); _tr_str_release(_at_t985); });
        /* pass */
        List_TrStr_append(r, trret);
    } else {
        /* pass */
        ({ TrStr _at_t986 = (_tr_strx_concat(_tr_strz(rt->base), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t986); _tr_str_release(_at_t986); });
        /* pass */
        ({ TrStr _at_t987 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return new ")), _tr_strz(rt->base))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t987); _tr_str_release(_at_t987); });
        /* pass */
        List_TrStr_append(r, trret);
    }
    /* pass */
    _tr_str_release(trret);
    return r;
}

__attribute__((hot)) TrStr _cpp_opaque_handle(TrStr base, long long nd, TrMap* class_names, TrMap* value_structs, TrMap* seen, StringBuilder* opaque) {
    /* pass */
    TrStr seg = _last_seg(base);
    /* pass */
    TrStr handle = _tr_str_retain(seg);
    /* pass */
    if ((_is_libc_ty_name(seg) || _is_system_record_ty(seg))) {
        /* pass */
        TrStr _strtmp_t988 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t988;
    } else if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
        /* pass */
        TrStr _strtmp_t989 = _tr_str_retain(seg);
        _tr_str_release(handle);
        handle = _strtmp_t989;
    } else if ((!_tr_dict_contains(class_names, _tr_strz(seg)))) {
        /* pass */
        TrStr _strtmp_t990 = _cpp_ident(base);
        _tr_str_release(handle);
        handle = _strtmp_t990;
        /* pass */
        if (({ TrStr _dkt_t991 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(handle))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t991)))); _tr_str_release(_dkt_t991); _wr; })) {
            /* pass */
            ({ TrStr _sbt_t992 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(handle))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t992); _tr_str_release(_sbt_t992); });
            /* pass */
            ({ TrStr _dkt_t993 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(handle))); _tr_dict_set(seen, _tr_strz(_dkt_t993), true); _tr_str_release(_dkt_t993); });
        }
    }
    /* pass */
    TrStr ty = _tr_str_retain(handle);
    /* pass */
    long long k = 1LL;
    /* pass */
    if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
        /* pass */
        k = 0LL;
    }
    /* pass */
    while ((k < nd)) {
        /* pass */
        TrStr _strtmp_t994 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(ty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(ty);
        ty = _strtmp_t994;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    _tr_str_release(seg);
    _tr_str_release(handle);
    return ty;
}

__attribute__((hot)) TrStr _cpp_qual(TrStr base, TrMap* class_qual) {
    /* pass */
    TrStr seg = _last_seg(base);
    /* pass */
    if (_tr_dict_contains(class_qual, _tr_strz(seg))) {
        /* pass */
        return _tr_str_retain(_tr_str_unbox(_tr_dict_get(class_qual, _tr_strz(seg))));
    }
    /* pass */
    _tr_str_release(seg);
    return _tr_str_retain(base);
}

__attribute__((hot)) List_TrStr* _cpp_ret_ex(TrStr desc, TrStr call, TrMap* value_structs, TrMap* class_names, TrMap* class_qual, TrMap* seen, StringBuilder* opaque) {
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* d4 = _desc4(desc);
    /* pass */
    long long rdepth = ({ TrStr _at_t995 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t995)); _tr_str_release(_at_t995); _wr; });
    /* pass */
    bool rref = (strcmp(_tr_strz(List_TrStr_get(d4, 1LL)), _tr_strz(_tr_str_lit("0"))) != 0);
    /* pass */
    TrStr cat = List_TrStr_get(d4, 2LL);
    /* pass */
    TrStr detail = List_TrStr_get(d4, 3LL);
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("v"))) == 0)) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("void"));
        /* pass */
        ({ TrStr _at_t996 = (_tr_strx_concat(_tr_strz(call), _tr_strz(_tr_str_lit(";")))); List_TrStr_append(r, _at_t996); _tr_str_release(_at_t996); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit(""));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("p"))) == 0)) {
        /* pass */
        TrStr cpp = _c_to_cpp(detail);
        /* pass */
        if ((rdepth == 0LL)) {
            /* pass */
            List_TrStr_append(r, cpp);
            /* pass */
            ({ TrStr _at_t997 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t997); _tr_str_release(_at_t997); });
            /* pass */
            List_TrStr_append(r, detail);
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(cpp);
            return r;
        }
        /* pass */
        TrStr cty = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cpp), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (_stars(rdepth)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        /* pass */
        if (rref) {
            /* pass */
            List_TrStr_append(r, cty);
            /* pass */
            ({ TrStr _at_t998 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (")), _tr_strz(cty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")(&("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("));"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t998); _tr_str_release(_at_t998); });
            /* pass */
            ({ TrStr _at_t999 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t999); _tr_str_release(_at_t999); });
        } else {
            /* pass */
            List_TrStr_append(r, cty);
            /* pass */
            ({ TrStr _at_t1000 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (")), _tr_strz(cty))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1000); _tr_str_release(_at_t1000); });
            /* pass */
            ({ TrStr _at_t1001 = (_ptr_wrap(detail, rdepth)); List_TrStr_append(r, _at_t1001); _tr_str_release(_at_t1001); });
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(cpp);
        _tr_str_release(cty);
        return r;
    }
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("e"))) == 0)) {
        /* pass */
        TrStr espell = List_TrStr_get(_tr_str_split(_tr_strz(detail), _tr_strz(_tr_str_lit("#"))), 0LL);
        /* pass */
        TrStr ealias = _last_seg(espell);
        /* pass */
        if ((rdepth == 0LL)) {
            /* pass */
            List_TrStr_append(r, espell);
            /* pass */
            ({ TrStr _at_t1002 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1002); _tr_str_release(_at_t1002); });
            /* pass */
            List_TrStr_append(r, ealias);
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(espell);
            _tr_str_release(ealias);
            return r;
        }
        /* pass */
        if (rref) {
            /* pass */
            ({ TrStr _at_t1003 = (_tr_strx_concat(_tr_strz(espell), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t1003); _tr_str_release(_at_t1003); });
            /* pass */
            ({ TrStr _at_t1004 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return &(")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1004); _tr_str_release(_at_t1004); });
            /* pass */
            ({ TrStr _at_t1005 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t1005); _tr_str_release(_at_t1005); });
        } else {
            /* pass */
            ({ TrStr _at_t1006 = (_tr_strx_concat(_tr_strz(espell), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t1006); _tr_str_release(_at_t1006); });
            /* pass */
            ({ TrStr _at_t1007 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1007); _tr_str_release(_at_t1007); });
            /* pass */
            ({ TrStr _at_t1008 = (_ptr_wrap(ealias, rdepth)); List_TrStr_append(r, _at_t1008); _tr_str_release(_at_t1008); });
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(espell);
        _tr_str_release(ealias);
        return r;
    }
    /* pass */
    if (((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("s"))) == 0) && (strcmp(_tr_strz(detail), _tr_strz(_tr_str_lit("string"))) == 0))) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("char*"));
        /* pass */
        ({ TrStr _at_t1009 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return _tr_cpp_strdup(")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1009); _tr_str_release(_at_t1009); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit("Pointer[char]"));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("f"))) == 0)) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("void*"));
        /* pass */
        ({ TrStr _at_t1010 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (void*)(")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1010); _tr_str_release(_at_t1010); });
        /* pass */
        List_TrStr_append(r, _tr_str_lit("Pointer[void]"));
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return r;
    }
    /* pass */
    TrStr seg = _last_seg(detail);
    /* pass */
    if ((((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("r"))) == 0) && _tr_dict_contains(value_structs, _tr_strz(seg))) && (rdepth == 0LL))) {
        /* pass */
        ({ TrStr _at_t1011 = (_cpp_qual(detail, class_qual)); List_TrStr_append(r, _at_t1011); _tr_str_release(_at_t1011); });
        /* pass */
        ({ TrStr _at_t1012 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1012); _tr_str_release(_at_t1012); });
        /* pass */
        List_TrStr_append(r, seg);
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        _tr_str_release(seg);
        return r;
    }
    /* pass */
    long long nd = rdepth;
    /* pass */
    if ((nd < 1LL)) {
        /* pass */
        nd = 1LL;
    }
    /* pass */
    TrStr handle = _cpp_opaque_handle(detail, nd, class_names, value_structs, seen, opaque);
    /* pass */
    TrStr cb = _cpp_qual(detail, class_qual);
    /* pass */
    if ((rdepth == 0LL)) {
        /* pass */
        ({ TrStr _at_t1013 = (_tr_strx_concat(_tr_strz(cb), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t1013); _tr_str_release(_at_t1013); });
        /* pass */
        ({ TrStr _at_t1014 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return new ")), _tr_strz(cb))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1014); _tr_str_release(_at_t1014); });
        /* pass */
        List_TrStr_append(r, handle);
    } else if (rref) {
        /* pass */
        ({ TrStr _at_t1015 = (_tr_strx_concat(_tr_strz(cb), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t1015); _tr_str_release(_at_t1015); });
        /* pass */
        ({ TrStr _at_t1016 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (")), _tr_strz(cb))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)(&("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("));"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1016); _tr_str_release(_at_t1016); });
        /* pass */
        List_TrStr_append(r, handle);
    } else {
        /* pass */
        ({ TrStr _at_t1017 = (_tr_strx_concat(_tr_strz(cb), _tr_strz(_tr_str_lit("*")))); List_TrStr_append(r, _at_t1017); _tr_str_release(_at_t1017); });
        /* pass */
        ({ TrStr _at_t1018 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return (")), _tr_strz(cb))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("*)("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(call)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(");"))); _tr_str_release(_cl); _cres; })); List_TrStr_append(r, _at_t1018); _tr_str_release(_at_t1018); });
        /* pass */
        List_TrStr_append(r, handle);
    }
    /* pass */
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    _tr_str_release(seg);
    _tr_str_release(handle);
    _tr_str_release(cb);
    return r;
}

__attribute__((hot)) TrStr _ns_pop(TrStr path) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(path), _tr_strz(_tr_str_lit("::")));
    /* pass */
    if ((parts->len <= 1LL)) {
        /* pass */
        List_TrStr_free(parts);
        return _tr_str_lit("");
    }
    /* pass */
    TrStr r = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (parts->len - 1LL))) {
        /* pass */
        if ((strcmp(_tr_strz(r), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t1019 = List_TrStr_get(parts, i);
            _tr_str_release(r);
            r = _strtmp_t1019;
        } else {
            /* pass */
            TrStr _strtmp_t1020 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(r), _tr_strz(_tr_str_lit("::")))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(r);
            r = _strtmp_t1020;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_free(parts);
    return r;
}

__attribute__((hot)) TrStr _ns_us(TrStr path) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(path), _tr_strz(_tr_str_lit("::")));
    /* pass */
    TrStr r = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < parts->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(parts, i)), _tr_strz(_tr_str_lit(""))) != 0)) {
            /* pass */
            if ((strcmp(_tr_strz(r), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t1021 = List_TrStr_get(parts, i);
                _tr_str_release(r);
                r = _strtmp_t1021;
            } else {
                /* pass */
                TrStr _strtmp_t1022 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(r), _tr_strz(_tr_str_lit("_")))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                _tr_str_release(r);
                r = _strtmp_t1022;
            }
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_free(parts);
    return r;
}

__attribute__((hot)) TrStr _rstrip_cr(TrStr s) {
    /* pass */
    long long n = _tr_strlen(_tr_strz(s));
    /* pass */
    while (({ TrStr _wt_t1023 = (_tr_str_wrap(_tr_str_slice(_tr_strz(s), (n - 1LL), n))); __auto_type _wr = (((n > 0LL) && (strcmp(_wt_t1023.data, _tr_strz(_tr_str_lit("\r"))) == 0))); _tr_str_release(_wt_t1023); _wr; })) {
        /* pass */
        n = (n - 1LL);
    }
    /* pass */
    if ((n == _tr_strlen(_tr_strz(s)))) {
        /* pass */
        return _tr_str_retain(s);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(s), 0LL, n));
}

__attribute__((hot)) TrStr _c_to_cpp(TrStr cn) {
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        return _tr_str_lit("void");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("bool"))) == 0)) {
        /* pass */
        return _tr_str_lit("bool");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_int"))) == 0)) {
        /* pass */
        return _tr_str_lit("int");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uint"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned int");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_char"))) == 0)) {
        /* pass */
        return _tr_str_lit("char");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_schar"))) == 0)) {
        /* pass */
        return _tr_str_lit("signed char");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uchar"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned char");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_wchar"))) == 0)) {
        /* pass */
        return _tr_str_lit("wchar_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_char16"))) == 0)) {
        /* pass */
        return _tr_str_lit("char16_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_char32"))) == 0)) {
        /* pass */
        return _tr_str_lit("char32_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_short"))) == 0)) {
        /* pass */
        return _tr_str_lit("short");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ushort"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned short");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_long"))) == 0)) {
        /* pass */
        return _tr_str_lit("long");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ulong"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned long");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_longlong"))) == 0)) {
        /* pass */
        return _tr_str_lit("long long");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ulonglong"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned long long");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_float"))) == 0)) {
        /* pass */
        return _tr_str_lit("float");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_double"))) == 0)) {
        /* pass */
        return _tr_str_lit("double");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ldouble"))) == 0)) {
        /* pass */
        return _tr_str_lit("long double");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_size_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("size_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ssize_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("ptrdiff_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_ptrdiff_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("ptrdiff_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_intptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("intptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uintptr_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("uintptr_t");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_int8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("signed char");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uint8_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned char");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_int16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("short");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uint16_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned short");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_int32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("int");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uint32_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned int");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_int64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("long long");
    }
    /* pass */
    if ((strcmp(_tr_strz(cn), _tr_strz(_tr_str_lit("c_uint64_t"))) == 0)) {
        /* pass */
        return _tr_str_lit("unsigned long long");
    }
    /* pass */
    return _tr_str_retain(cn);
}

__attribute__((hot)) List_TrStr* _desc4(TrStr desc) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(desc), _tr_strz(_tr_str_lit("~")));
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if ((parts->len < 3LL)) {
        /* pass */
        List_TrStr_append(r, _tr_str_lit("0"));
        /* pass */
        List_TrStr_append(r, _tr_str_lit("0"));
        /* pass */
        List_TrStr_append(r, _tr_str_lit("u"));
        /* pass */
        List_TrStr_append(r, desc);
        /* pass */
        List_TrStr_free(parts);
        return r;
    }
    /* pass */
    ({ TrStr _at_t1024 = (List_TrStr_get(parts, 0LL)); List_TrStr_append(r, _at_t1024); _tr_str_release(_at_t1024); });
    /* pass */
    ({ TrStr _at_t1025 = (List_TrStr_get(parts, 1LL)); List_TrStr_append(r, _at_t1025); _tr_str_release(_at_t1025); });
    /* pass */
    ({ TrStr _at_t1026 = (List_TrStr_get(parts, 2LL)); List_TrStr_append(r, _at_t1026); _tr_str_release(_at_t1026); });
    /* pass */
    TrStr det = _tr_str_lit("");
    /* pass */
    if ((parts->len > 3LL)) {
        /* pass */
        TrStr _strtmp_t1027 = List_TrStr_get(parts, 3LL);
        _tr_str_release(det);
        det = _strtmp_t1027;
    }
    /* pass */
    long long k = 4LL;
    /* pass */
    while ((k < parts->len)) {
        /* pass */
        TrStr _strtmp_t1028 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(det), _tr_strz(_tr_str_lit("~")))); TrStr _cr = (List_TrStr_get(parts, k)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
        _tr_str_release(det);
        det = _strtmp_t1028;
        /* pass */
        k = (k + 1LL);
    }
    /* pass */
    List_TrStr_append(r, det);
    /* pass */
    List_TrStr_free(parts);
    _tr_str_release(det);
    return r;
}

__attribute__((hot)) List_TrStr* _parse_tclass(TrStr rest) {
    /* pass */
    List_TrStr* parts = _tr_str_split(_tr_strz(rest), _tr_strz(_tr_str_lit(" ")));
    /* pass */
    List_TrStr* r = (void*)List_TrStr_new();
    /* pass */
    if ((parts->len < 2LL)) {
        /* pass */
        List_TrStr_append(r, rest);
        /* pass */
        List_TrStr_append(r, _tr_str_lit("0"));
        /* pass */
        List_TrStr_free(parts);
        return r;
    }
    /* pass */
    TrStr pod = List_TrStr_get(parts, (parts->len - 1LL));
    /* pass */
    TrStr sp = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < (parts->len - 1LL))) {
        /* pass */
        if ((strcmp(_tr_strz(sp), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            TrStr _strtmp_t1029 = List_TrStr_get(parts, i);
            _tr_str_release(sp);
            sp = _strtmp_t1029;
        } else {
            /* pass */
            TrStr _strtmp_t1030 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(sp), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (List_TrStr_get(parts, i)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(sp);
            sp = _strtmp_t1030;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(r, sp);
    /* pass */
    List_TrStr_append(r, pod);
    /* pass */
    List_TrStr_free(parts);
    _tr_str_release(pod);
    _tr_str_release(sp);
    return r;
}

__attribute__((hot)) TrStr _stars(long long n) {
    /* pass */
    TrStr s = _tr_str_lit("");
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t1031 = _tr_strx_concat(_tr_strz(s), _tr_strz(_tr_str_lit("*")));
        _tr_str_release(s);
        s = _strtmp_t1031;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return s;
}

__attribute__((hot)) TrStr _ptr_wrap(TrStr inner, long long n) {
    /* pass */
    TrStr t = _tr_str_retain(inner);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        TrStr _strtmp_t1032 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("Pointer[")), _tr_strz(t))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("]"))); _tr_str_release(_cl); _cres; });
        _tr_str_release(t);
        t = _strtmp_t1032;
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return t;
}

__attribute__((hot)) TrStr _cpp_field_type(TrStr desc, TrMap* value_structs, TrMap* enum_names) {
    /* pass */
    List_TrStr* d4 = _desc4(desc);
    /* pass */
    long long depth = ({ TrStr _at_t1033 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t1033)); _tr_str_release(_at_t1033); _wr; });
    /* pass */
    TrStr cat = List_TrStr_get(d4, 2LL);
    /* pass */
    TrStr detail = List_TrStr_get(d4, 3LL);
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("p"))) == 0)) {
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            return detail;
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        return _ptr_wrap(detail, depth);
    }
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("e"))) == 0)) {
        /* pass */
        TrStr ealias = ({ TrStr _at_t1034 = (List_TrStr_get(_tr_str_split(_tr_strz(detail), _tr_strz(_tr_str_lit("#"))), 0LL)); __auto_type _wr = (_last_seg(_at_t1034)); _tr_str_release(_at_t1034); _wr; });
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            return ealias;
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _ptr_wrap(ealias, depth);
    }
    /* pass */
    if ((strcmp(_tr_strz(cat), _tr_strz(_tr_str_lit("r"))) == 0)) {
        /* pass */
        TrStr seg = _last_seg(detail);
        /* pass */
        if ((depth == 0LL)) {
            /* pass */
            if (_tr_dict_contains(value_structs, _tr_strz(seg))) {
                /* pass */
                List_TrStr_free(d4);
                _tr_str_release(cat);
                _tr_str_release(detail);
                return seg;
            }
            /* pass */
            List_TrStr_free(d4);
            _tr_str_release(cat);
            _tr_str_release(detail);
            _tr_str_release(seg);
            return _tr_str_lit("Pointer[void]");
        }
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _ptr_wrap(seg, depth);
    }
    /* pass */
    if ((depth == 0LL)) {
        /* pass */
        List_TrStr_free(d4);
        _tr_str_release(cat);
        _tr_str_release(detail);
        return _tr_str_lit("Pointer[void]");
    }
    /* pass */
    List_TrStr_free(d4);
    _tr_str_release(cat);
    _tr_str_release(detail);
    return _ptr_wrap(_tr_str_lit("void"), depth);
}

__attribute__((hot)) TrStr _shim_body(TrStr ret_ctype, TrStr body) {
    /* pass */
    TrStr zero = _tr_str_lit("return {};");
    /* pass */
    if ((strcmp(_tr_strz(ret_ctype), _tr_strz(_tr_str_lit("void"))) == 0)) {
        /* pass */
        TrStr _strtmp_t1035 = _tr_str_lit("return;");
        _tr_str_release(zero);
        zero = _strtmp_t1035;
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("try { ")), _tr_strz(body))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" } catch (const std::exception& e) { _tr_cpp_set_error(e.what()); "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(zero)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" } catch (...) { _tr_cpp_set_error(\"C++ exception\"); "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(zero)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }"))); _tr_str_release(_cl); _cres; });
}

__attribute__((hot)) TrStr _fnptr_cast(TrStr fnty) {
    /* pass */
    __auto_type p = ({ char* _t1036 = strstr(_tr_strz(fnty), _tr_strz(_tr_str_lit("("))); _t1036 ? (long long)(_t1036 - (_tr_strz(fnty))) : -1LL; });
    /* pass */
    if ((p < 0LL)) {
        /* pass */
        return _tr_str_retain(fnty);
    }
    /* pass */
    return ({ TrStr _cl = (({ TrStr _cl = (_tr_str_wrap(_tr_str_slice(_tr_strz(fnty), 0LL, p))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(*)"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_str_slice(_tr_strz(fnty), p, _tr_strlen(_tr_strz(fnty))))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
}

__attribute__((hot)) void _cpp_generate(TrStr ir, TrStr header, TrStr out, TrStr shim_cflags, TrStr pkglibs) {
    /* pass */
    StringBuilder* opaque = StringBuilder_init(512LL);
    /* pass */
    StringBuilder* consts = StringBuilder_init(512LL);
    /* pass */
    StringBuilder* decls = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder* shim = StringBuilder_init(2048LL);
    /* pass */
    StringBuilder* usings = StringBuilder_init(256LL);
    /* pass */
    TrMap* seen = _tr_dict_new(64LL);
    /* pass */
    TrStr ns_path = _tr_str_lit("");
    /* pass */
    long long n_classes = 0LL;
    /* pass */
    long long n_fns = 0LL;
    /* pass */
    TrMap* sym_used = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* cstk_name = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* cstk_qual = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* cstk_pfx = (void*)List_TrStr_new();
    /* pass */
    TrStr fld_buf = _tr_str_lit("");
    /* pass */
    List_TrStr* fld_stack = (void*)List_TrStr_new();
    /* pass */
    TrMap* enum_names = _tr_dict_new(32LL);
    /* pass */
    TrMap* class_names = _tr_dict_new(32LL);
    /* pass */
    TrMap* value_structs = _tr_dict_new(32LL);
    /* pass */
    TrMap* class_qual = _tr_dict_new(32LL);
    /* pass */
    List_TrStr* ps_name = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* ps_pod = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* ps_nf = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* plines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long pli = 0LL;
    /* pass */
    while ((pli < plines->len)) {
        /* pass */
        TrStr pl = ({ TrStr _at_t1037 = (List_TrStr_get(plines, pli)); __auto_type _wr = (_rstrip_cr(_at_t1037)); _tr_str_release(_at_t1037); _wr; });
        /* pass */
        pli = (pli + 1LL);
        /* pass */
        if (_tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("ENUM ")))) {
            /* pass */
            TrStr erest = _tr_str_wrap(_tr_str_slice(_tr_strz(pl), 5LL, _tr_strlen(_tr_strz(pl))));
            /* pass */
            TrStr ename0 = List_TrStr_get(_tr_str_split(_tr_strz(erest), _tr_strz(_tr_str_lit(" "))), 0LL);
            /* pass */
            if ((strcmp(_tr_strz(ename0), _tr_strz(_tr_str_lit("anon"))) != 0)) {
                /* pass */
                _tr_dict_set(enum_names, _tr_strz(ename0), true);
            }
            _tr_str_release(erest);
        } else if (_tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("CLASS ")))) {
            /* pass */
            TrStr crest = _tr_str_wrap(_tr_str_slice(_tr_strz(pl), 6LL, _tr_strlen(_tr_strz(pl))));
            /* pass */
            List_TrStr* cparts = _tr_str_split(_tr_strz(crest), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            TrStr cnm = List_TrStr_get(cparts, 0LL);
            /* pass */
            TrStr cpod = _tr_str_lit("0");
            /* pass */
            if ((cparts->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1038 = List_TrStr_get(cparts, 1LL);
                _tr_str_release(cpod);
                cpod = _strtmp_t1038;
            }
            /* pass */
            _tr_dict_set(class_names, _tr_strz(cnm), true);
            /* pass */
            List_TrStr_append(ps_name, cnm);
            /* pass */
            List_TrStr_append(ps_pod, cpod);
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit("0"));
            _tr_str_release(crest);
            _tr_str_release(cnm);
            _tr_str_release(cpod);
        } else if (_tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("TCLASS ")))) {
            /* pass */
            List_TrStr* tp = ({ TrStr _at_t1039 = (_tr_str_wrap(_tr_str_slice(_tr_strz(pl), 7LL, _tr_strlen(_tr_strz(pl))))); __auto_type _wr = (_parse_tclass(_at_t1039)); _tr_str_release(_at_t1039); _wr; });
            /* pass */
            TrStr tnm = ({ TrStr _at_t1040 = (List_TrStr_get(tp, 0LL)); __auto_type _wr = (_cpp_ident(_at_t1040)); _tr_str_release(_at_t1040); _wr; });
            /* pass */
            _tr_dict_set(class_names, _tr_strz(tnm), true);
            /* pass */
            List_TrStr_append(ps_name, tnm);
            /* pass */
            ({ TrStr _at_t1041 = (List_TrStr_get(tp, 1LL)); List_TrStr_append(ps_pod, _at_t1041); _tr_str_release(_at_t1041); });
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit("0"));
            List_TrStr_free(tp);
            _tr_str_release(tnm);
        } else if (_tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("XSTRUCT ")))) {
            /* pass */
            TrStr xn0 = _tr_str_wrap(_tr_str_slice(_tr_strz(pl), 8LL, _tr_strlen(_tr_strz(pl))));
            /* pass */
            _tr_dict_set(class_names, _tr_strz(xn0), true);
            /* pass */
            List_TrStr_append(ps_name, xn0);
            /* pass */
            List_TrStr_append(ps_pod, _tr_str_lit("1"));
            /* pass */
            List_TrStr_append(ps_nf, _tr_str_lit("0"));
            _tr_str_release(xn0);
        } else if ((_tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("FIELD "))) || _tr_str_starts_with(_tr_strz(pl), _tr_strz(_tr_str_lit("SFIELD "))))) {
            /* pass */
            if ((ps_nf->len > 0LL)) {
                /* pass */
                List_TrStr_pop(ps_nf);
                /* pass */
                List_TrStr_append(ps_nf, _tr_str_lit("1"));
            }
        } else if (((strcmp(_tr_strz(pl), _tr_strz(_tr_str_lit("ECLASS"))) == 0) || (strcmp(_tr_strz(pl), _tr_strz(_tr_str_lit("EXSTRUCT"))) == 0))) {
            /* pass */
            if ((ps_name->len > 0LL)) {
                /* pass */
                TrStr nm2 = List_TrStr_pop(ps_name);
                /* pass */
                TrStr pod2 = List_TrStr_pop(ps_pod);
                /* pass */
                TrStr nf2 = List_TrStr_pop(ps_nf);
                /* pass */
                if (((strcmp(_tr_strz(pod2), _tr_strz(_tr_str_lit("1"))) == 0) && (strcmp(_tr_strz(nf2), _tr_strz(_tr_str_lit("1"))) == 0))) {
                    /* pass */
                    _tr_dict_set(value_structs, _tr_strz(nm2), true);
                }
                _tr_str_release(pod2);
                _tr_str_release(nf2);
            }
        }
        _tr_str_release(pl);
    }
    /* pass */
    TrStr mode = _tr_str_lit("");
    /* pass */
    TrStr m_flags = _tr_str_lit("..");
    /* pass */
    TrStr m_ret = _tr_str_lit("");
    /* pass */
    TrStr m_name = _tr_str_lit("");
    /* pass */
    TrStr m_qual = _tr_str_lit("");
    /* pass */
    long long m_ndef = 0LL;
    /* pass */
    List_TrStr* params = (void*)List_TrStr_new();
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr line = ({ TrStr _at_t1042 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1042)); _tr_str_release(_at_t1042); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("TDSPEC ")))) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr cur_class = _tr_str_lit("");
        /* pass */
        TrStr cur_class_qual = _tr_str_lit("");
        /* pass */
        TrStr class_pfx = _tr_str_lit("");
        /* pass */
        if ((cstk_name->len > 0LL)) {
            /* pass */
            TrStr _strtmp_t1043 = List_TrStr_get(cstk_name, (cstk_name->len - 1LL));
            _tr_str_release(cur_class);
            cur_class = _strtmp_t1043;
            /* pass */
            TrStr _strtmp_t1044 = List_TrStr_get(cstk_qual, (cstk_qual->len - 1LL));
            _tr_str_release(cur_class_qual);
            cur_class_qual = _strtmp_t1044;
            /* pass */
            TrStr _strtmp_t1045 = List_TrStr_get(cstk_pfx, (cstk_pfx->len - 1LL));
            _tr_str_release(class_pfx);
            class_pfx = _strtmp_t1045;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("CTOR")))) {
            /* pass */
            m_ndef = 0LL;
            /* pass */
            if ((_tr_strlen(_tr_strz(line)) > 5LL)) {
                /* pass */
                m_ndef = ({ TrStr _at_t1046 = (_tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))))); __auto_type _wr = (_to_int(_at_t1046)); _tr_str_release(_at_t1046); _wr; });
            }
            /* pass */
            TrStr _strtmp_t1047 = _tr_str_lit("ctor");
            _tr_str_release(mode);
            mode = _strtmp_t1047;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("DTOR"))) == 0)) {
            /* pass */
            if ((!_is_clean_ident(cur_class))) {
                /* pass */
                continue;
            }
            /* pass */
            TrStr ds = ({ TrStr _at_t1048 = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_delete")))); __auto_type _wr = (_uniq_sym(_at_t1048, sym_used)); _tr_str_release(_at_t1048); _wr; });
            /* pass */
            ({ TrStr _sbt_t1049 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("void ")), _tr_strz(ds))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self) { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_tr_str_lit("void"), _tr_str_lit("delete self;"))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1049); _tr_str_release(_sbt_t1049); });
            /* pass */
            ({ TrStr _sbt_t1050 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(ds))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(obj: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1050); _tr_str_release(_sbt_t1050); });
            /* pass */
            n_fns = (n_fns + 1LL);
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ECLASS"))) == 0)) {
            /* pass */
            if (({ TrStr _dkt_t1051 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); __auto_type _wr = ((_is_clean_ident(cur_class) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1051))))); _tr_str_release(_dkt_t1051); _wr; })) {
                /* pass */
                if ((_tr_dict_contains(value_structs, _tr_strz(cur_class)) && (strcmp(_tr_strz(fld_buf), _tr_strz(_tr_str_lit(""))) != 0))) {
                    /* pass */
                    ({ TrStr _sbt_t1052 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@value_type\nclass ")), _tr_strz(cur_class))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fld_buf)); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1052); _tr_str_release(_sbt_t1052); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t1053 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(cur_class))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1053); _tr_str_release(_sbt_t1053); });
                }
                /* pass */
                ({ TrStr _dkt_t1054 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t1054), true); _tr_str_release(_dkt_t1054); });
                /* pass */
                n_classes = (n_classes + 1LL);
            }
            /* pass */
            if ((cstk_name->len > 0LL)) {
                /* pass */
                List_TrStr_pop(cstk_name);
                /* pass */
                List_TrStr_pop(cstk_qual);
                /* pass */
                List_TrStr_pop(cstk_pfx);
            }
            /* pass */
            if ((fld_stack->len > 0LL)) {
                /* pass */
                TrStr _strtmp_t1055 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1055;
            } else {
                /* pass */
                TrStr _strtmp_t1056 = _tr_str_lit("");
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1056;
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("FIELD ")))) {
            /* pass */
            TrStr frest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* fbar = _tr_str_split(_tr_strz(frest), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr fdesc = List_TrStr_get(fbar, 0LL);
            /* pass */
            TrStr fnm = _tr_str_lit("");
            /* pass */
            if ((fbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1057 = List_TrStr_get(fbar, 1LL);
                _tr_str_release(fnm);
                fnm = _strtmp_t1057;
            }
            /* pass */
            if (_is_clean_ident(fnm)) {
                /* pass */
                if (_tr_dict_contains(value_structs, _tr_strz(cur_class))) {
                    /* pass */
                    TrStr fmap = _cpp_field_type(fdesc, value_structs, enum_names);
                    /* pass */
                    TrStr _strtmp_t1058 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fld_buf), _tr_strz(_tr_str_lit("    pub ")))); TrStr _cr = (_cpp_tr_pname(fnm)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fmap)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(fld_buf);
                    fld_buf = _strtmp_t1058;
                    _tr_str_release(fmap);
                } else if (((_is_clean_ident(cur_class) && (({ char* _t1059 = strstr(_tr_strz(fdesc), _tr_strz(_tr_str_lit("type-parameter"))); _t1059 ? (long long)(_t1059 - (_tr_strz(fdesc))) : -1LL; }) < 0LL)) && (({ char* _t1060 = strstr(_tr_strz(fdesc), _tr_strz(_tr_str_lit("~d~"))); _t1060 ? (long long)(_t1060 - (_tr_strz(fdesc))) : -1LL; }) < 0LL))) {
                    /* pass */
                    List_TrStr* fri = ({ TrStr _at_t1061 = (_tr_strx_concat(_tr_strz(_tr_str_lit("self->")), _tr_strz(fnm))); __auto_type _wr = (_cpp_ret_ex(fdesc, _at_t1061, value_structs, class_names, class_qual, seen, opaque)); _tr_str_release(_at_t1061); _wr; });
                    /* pass */
                    TrStr fsym = ({ TrStr _at_t1062 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fnm)); _tr_str_release(_cl); _cres; })); __auto_type _wr = (_uniq_sym(_at_t1062, sym_used)); _tr_str_release(_at_t1062); _wr; });
                    /* pass */
                    ({ TrStr _at_t1063 = (List_TrStr_get(fri, 0LL)); TrStr _at_t1064 = (List_TrStr_get(fri, 1LL)); TrStr _sbt_t1065 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(fri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fsym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self) { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1063, _at_t1064)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1065); _tr_str_release(_at_t1063); _tr_str_release(_at_t1064); _tr_str_release(_sbt_t1065); });
                    /* pass */
                    TrStr fd = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(fsym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(obj: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((strcmp(_tr_strz(List_TrStr_get(fri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t1066 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(fd), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(fri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        _tr_str_release(fd);
                        fd = _strtmp_t1066;
                    }
                    /* pass */
                    ({ TrStr _sbt_t1067 = (_tr_strx_concat(_tr_strz(fd), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t1067); _tr_str_release(_sbt_t1067); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(fsym);
                    _tr_str_release(fd);
                }
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ITER ")))) {
            /* pass */
            TrStr idesc = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            if (((_is_clean_ident(cur_class) && (({ char* _t1068 = strstr(_tr_strz(idesc), _tr_strz(_tr_str_lit("type-parameter"))); _t1068 ? (long long)(_t1068 - (_tr_strz(idesc))) : -1LL; }) < 0LL)) && (({ char* _t1069 = strstr(_tr_strz(idesc), _tr_strz(_tr_str_lit("~d~"))); _t1069 ? (long long)(_t1069 - (_tr_strz(idesc))) : -1LL; }) < 0LL))) {
                /* pass */
                List_TrStr* iri = _cpp_ret_ex(idesc, _tr_str_lit("(*it)"), value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr isym = ({ TrStr _at_t1070 = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_nth")))); __auto_type _wr = (_uniq_sym(_at_t1070, sym_used)); _tr_str_release(_at_t1070); _wr; });
                /* pass */
                ({ TrStr _at_t1071 = (List_TrStr_get(iri, 0LL)); TrStr _at_t1072 = (({ TrStr _cr = (List_TrStr_get(iri, 1LL)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("auto it = self->begin(); std::advance(it, (long)i); ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _sbt_t1073 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(iri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(isym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self, long i) { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1071, _at_t1072)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1073); _tr_str_release(_at_t1071); _tr_str_release(_at_t1072); _tr_str_release(_sbt_t1073); });
                /* pass */
                TrStr id = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(isym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(obj: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i: c_long)"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(iri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1074 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(id), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(iri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(id);
                    id = _strtmp_t1074;
                }
                /* pass */
                ({ TrStr _sbt_t1075 = (_tr_strx_concat(_tr_strz(id), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t1075); _tr_str_release(_sbt_t1075); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(isym);
                _tr_str_release(id);
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ITERMAPKEY "))) || _tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ITERMAPVAL "))))) {
            /* pass */
            bool is_key = _tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ITERMAPKEY ")));
            /* pass */
            TrStr mdesc = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 11LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            if (((_is_clean_ident(cur_class) && (({ char* _t1076 = strstr(_tr_strz(mdesc), _tr_strz(_tr_str_lit("type-parameter"))); _t1076 ? (long long)(_t1076 - (_tr_strz(mdesc))) : -1LL; }) < 0LL)) && (({ char* _t1077 = strstr(_tr_strz(mdesc), _tr_strz(_tr_str_lit("~d~"))); _t1077 ? (long long)(_t1077 - (_tr_strz(mdesc))) : -1LL; }) < 0LL))) {
                /* pass */
                TrStr mfield = _tr_str_lit("(it->second)");
                /* pass */
                TrStr mname = _tr_str_lit("_val_nth");
                /* pass */
                if (is_key) {
                    /* pass */
                    TrStr _strtmp_t1078 = _tr_str_lit("(it->first)");
                    _tr_str_release(mfield);
                    mfield = _strtmp_t1078;
                    /* pass */
                    TrStr _strtmp_t1079 = _tr_str_lit("_key_nth");
                    _tr_str_release(mname);
                    mname = _strtmp_t1079;
                }
                /* pass */
                List_TrStr* mri = _cpp_ret_ex(mdesc, mfield, value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr msym = ({ TrStr _at_t1080 = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(mname))); __auto_type _wr = (_uniq_sym(_at_t1080, sym_used)); _tr_str_release(_at_t1080); _wr; });
                /* pass */
                ({ TrStr _at_t1081 = (List_TrStr_get(mri, 0LL)); TrStr _at_t1082 = (({ TrStr _cr = (List_TrStr_get(mri, 1LL)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("auto it = self->begin(); std::advance(it, (long)i); ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _sbt_t1083 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(mri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(msym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class_qual)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self, long i) { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1081, _at_t1082)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1083); _tr_str_release(_at_t1081); _tr_str_release(_at_t1082); _tr_str_release(_sbt_t1083); });
                /* pass */
                TrStr md = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(msym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("(obj: "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", i: c_long)"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(mri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1084 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(md), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(mri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(md);
                    md = _strtmp_t1084;
                }
                /* pass */
                ({ TrStr _sbt_t1085 = (_tr_strx_concat(_tr_strz(md), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t1085); _tr_str_release(_sbt_t1085); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(mfield);
                _tr_str_release(mname);
                _tr_str_release(msym);
                _tr_str_release(md);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("GVAR ")))) {
            /* pass */
            TrStr grest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* gbar = _tr_str_split(_tr_strz(grest), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr gvdesc = List_TrStr_get(gbar, 0LL);
            /* pass */
            TrStr gvnm = _tr_str_lit("");
            /* pass */
            if ((gbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1086 = List_TrStr_get(gbar, 1LL);
                _tr_str_release(gvnm);
                gvnm = _strtmp_t1086;
            }
            /* pass */
            TrStr gcat = List_TrStr_get(_desc4(gvdesc), 2LL);
            /* pass */
            if ((((((_is_clean_ident(gvnm) && (({ char* _t1087 = strstr(_tr_strz(gvdesc), _tr_strz(_tr_str_lit("type-parameter"))); _t1087 ? (long long)(_t1087 - (_tr_strz(gvdesc))) : -1LL; }) < 0LL)) && (({ char* _t1088 = strstr(_tr_strz(gvdesc), _tr_strz(_tr_str_lit("~d~"))); _t1088 ? (long long)(_t1088 - (_tr_strz(gvdesc))) : -1LL; }) < 0LL)) && (strcmp(_tr_strz(gcat), _tr_strz(_tr_str_lit("u"))) != 0)) && (strcmp(_tr_strz(gcat), _tr_strz(_tr_str_lit("f"))) != 0)) && (strcmp(_tr_strz(gcat), _tr_strz(_tr_str_lit("v"))) != 0))) {
                /* pass */
                TrStr gqual = _tr_str_retain(gvnm);
                /* pass */
                TrStr gsym0 = _tr_strx_concat(_tr_strz(_tr_str_lit("g_")), _tr_strz(gvnm));
                /* pass */
                if ((strcmp(_tr_strz(cur_class), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1089 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t1089;
                    /* pass */
                    TrStr _strtmp_t1090 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t1090;
                } else if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1091 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gqual);
                    gqual = _strtmp_t1091;
                    /* pass */
                    TrStr _strtmp_t1092 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gvnm)); _tr_str_release(_cl); _cres; });
                    _tr_str_release(gsym0);
                    gsym0 = _strtmp_t1092;
                }
                /* pass */
                List_TrStr* gvri = _cpp_ret_ex(gvdesc, gqual, value_structs, class_names, class_qual, seen, opaque);
                /* pass */
                TrStr gvs = _uniq_sym(gsym0, sym_used);
                /* pass */
                ({ TrStr _at_t1093 = (List_TrStr_get(gvri, 0LL)); TrStr _at_t1094 = (List_TrStr_get(gvri, 1LL)); TrStr _sbt_t1095 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(gvri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(gvs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("() { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1093, _at_t1094)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1095); _tr_str_release(_at_t1093); _tr_str_release(_at_t1094); _tr_str_release(_sbt_t1095); });
                /* pass */
                TrStr gvd = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(gvs))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("()"))); _tr_str_release(_cl); _cres; });
                /* pass */
                if ((strcmp(_tr_strz(List_TrStr_get(gvri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1096 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(gvd), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(gvri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                    _tr_str_release(gvd);
                    gvd = _strtmp_t1096;
                }
                /* pass */
                ({ TrStr _sbt_t1097 = (_tr_strx_concat(_tr_strz(gvd), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t1097); _tr_str_release(_sbt_t1097); });
                /* pass */
                n_fns = (n_fns + 1LL);
                _tr_str_release(gqual);
                _tr_str_release(gsym0);
                _tr_str_release(gvs);
                _tr_str_release(gvd);
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("FTMPL ")))) {
            /* pass */
            TrStr ftrest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* ftbar = _tr_str_split(_tr_strz(ftrest), _tr_strz(_tr_str_lit("|")));
            /* pass */
            long long ftnp = ({ TrStr _at_t1098 = (List_TrStr_get(ftbar, 0LL)); __auto_type _wr = (_to_int(_at_t1098)); _tr_str_release(_at_t1098); _wr; });
            /* pass */
            TrStr ftnm = _tr_str_lit("");
            /* pass */
            if ((ftbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1099 = List_TrStr_get(ftbar, 1LL);
                _tr_str_release(ftnm);
                ftnm = _strtmp_t1099;
            }
            /* pass */
            if ((_is_clean_ident(ftnm) && (ftnp >= 0LL))) {
                /* pass */
                List_TrStr* ftys = (void*)List_TrStr_new();
                /* pass */
                List_TrStr_append(ftys, _tr_str_lit("int"));
                /* pass */
                List_TrStr_append(ftys, _tr_str_lit("double"));
                /* pass */
                List_TrStr* fctys = (void*)List_TrStr_new();
                /* pass */
                List_TrStr_append(fctys, _tr_str_lit("c_int"));
                /* pass */
                List_TrStr_append(fctys, _tr_str_lit("c_double"));
                /* pass */
                long long fti = 0LL;
                /* pass */
                while ((fti < ftys->len)) {
                    /* pass */
                    TrStr fcpp = List_TrStr_get(ftys, fti);
                    /* pass */
                    TrStr fcty = List_TrStr_get(fctys, fti);
                    /* pass */
                    fti = (fti + 1LL);
                    /* pass */
                    TrStr fshimp = _tr_str_lit("");
                    /* pass */
                    TrStr ftrp = _tr_str_lit("");
                    /* pass */
                    TrStr fcallargs = _tr_str_lit("");
                    /* pass */
                    long long fpi = 0LL;
                    /* pass */
                    while ((fpi < ftnp)) {
                        /* pass */
                        TrStr fan = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(fpi)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
                        /* pass */
                        fpi = (fpi + 1LL);
                        /* pass */
                        if ((strcmp(_tr_strz(fshimp), _tr_strz(_tr_str_lit(""))) != 0)) {
                            /* pass */
                            TrStr _strtmp_t1100 = _tr_strx_concat(_tr_strz(fshimp), _tr_strz(_tr_str_lit(", ")));
                            _tr_str_release(fshimp);
                            fshimp = _strtmp_t1100;
                        }
                        /* pass */
                        TrStr _strtmp_t1101 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fshimp), _tr_strz(fcpp))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fan)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fshimp);
                        fshimp = _strtmp_t1101;
                        /* pass */
                        if ((strcmp(_tr_strz(ftrp), _tr_strz(_tr_str_lit(""))) != 0)) {
                            /* pass */
                            TrStr _strtmp_t1102 = _tr_strx_concat(_tr_strz(ftrp), _tr_strz(_tr_str_lit(", ")));
                            _tr_str_release(ftrp);
                            ftrp = _strtmp_t1102;
                        }
                        /* pass */
                        TrStr _strtmp_t1103 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(ftrp), _tr_strz(fan))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcty)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(ftrp);
                        ftrp = _strtmp_t1103;
                        /* pass */
                        if ((strcmp(_tr_strz(fcallargs), _tr_strz(_tr_str_lit(""))) != 0)) {
                            /* pass */
                            TrStr _strtmp_t1104 = _tr_strx_concat(_tr_strz(fcallargs), _tr_strz(_tr_str_lit(", ")));
                            _tr_str_release(fcallargs);
                            fcallargs = _strtmp_t1104;
                        }
                        /* pass */
                        TrStr _strtmp_t1105 = _tr_strx_concat(_tr_strz(fcallargs), _tr_strz(fan));
                        _tr_str_release(fcallargs);
                        fcallargs = _strtmp_t1105;
                        _tr_str_release(fan);
                    }
                    /* pass */
                    TrStr fqual = _tr_str_retain(ftnm);
                    /* pass */
                    TrStr ftsym0 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ftnm), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcpp)); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t1106 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ftnm)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fqual);
                        fqual = _strtmp_t1106;
                        /* pass */
                        TrStr _strtmp_t1107 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ftnm)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcpp)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(ftsym0);
                        ftsym0 = _strtmp_t1107;
                    }
                    /* pass */
                    TrStr ftsym = _uniq_sym(ftsym0, sym_used);
                    /* pass */
                    TrStr fcall = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fqual), _tr_strz(_tr_str_lit("<")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcpp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(">("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcallargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    ({ TrStr _at_t1108 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(fcall))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t1109 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fcpp), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ftsym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fshimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(fcpp, _at_t1108)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1109); _tr_str_release(_at_t1108); _tr_str_release(_sbt_t1109); });
                    /* pass */
                    ({ TrStr _sbt_t1110 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(ftsym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ftrp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fcty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1110); _tr_str_release(_sbt_t1110); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(fcpp);
                    _tr_str_release(fcty);
                    _tr_str_release(fshimp);
                    _tr_str_release(ftrp);
                    _tr_str_release(fcallargs);
                    _tr_str_release(fqual);
                    _tr_str_release(ftsym0);
                    _tr_str_release(ftsym);
                    _tr_str_release(fcall);
                }
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("XSTRUCT ")))) {
            /* pass */
            TrStr xn = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 8LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr_append(cstk_name, xn);
            /* pass */
            List_TrStr_append(cstk_qual, xn);
            /* pass */
            List_TrStr_append(cstk_pfx, xn);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(xn), _tr_str_box(_tr_str_retain(xn)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1111 = _tr_str_lit("");
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1111;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("SFIELD ")))) {
            /* pass */
            TrStr srest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 7LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* sbar = _tr_str_split(_tr_strz(srest), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr sty = List_TrStr_get(sbar, 0LL);
            /* pass */
            TrStr snm = _tr_str_lit("");
            /* pass */
            if ((sbar->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1112 = List_TrStr_get(sbar, 1LL);
                _tr_str_release(snm);
                snm = _strtmp_t1112;
            }
            /* pass */
            if (_is_clean_ident(snm)) {
                /* pass */
                TrStr _strtmp_t1113 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(fld_buf), _tr_strz(_tr_str_lit("    pub ")))); TrStr _cr = (_cpp_tr_pname(snm)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sty)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; });
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1113;
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EXSTRUCT"))) == 0)) {
            /* pass */
            if (({ TrStr _dkt_t1114 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); __auto_type _wr = ((_is_clean_ident(cur_class) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1114))))); _tr_str_release(_dkt_t1114); _wr; })) {
                /* pass */
                if ((strcmp(_tr_strz(fld_buf), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    ({ TrStr _sbt_t1115 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("@value_type\nclass ")), _tr_strz(cur_class))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fld_buf)); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1115); _tr_str_release(_sbt_t1115); });
                } else {
                    /* pass */
                    ({ TrStr _sbt_t1116 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("class ")), _tr_strz(cur_class))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(":\n    pass\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(opaque, _sbt_t1116); _tr_str_release(_sbt_t1116); });
                }
                /* pass */
                ({ TrStr _dkt_t1117 = (_tr_strx_concat(_tr_strz(_tr_str_lit("class:")), _tr_strz(cur_class))); _tr_dict_set(seen, _tr_strz(_dkt_t1117), true); _tr_str_release(_dkt_t1117); });
                /* pass */
                n_classes = (n_classes + 1LL);
            }
            /* pass */
            if ((cstk_name->len > 0LL)) {
                /* pass */
                List_TrStr_pop(cstk_name);
                /* pass */
                List_TrStr_pop(cstk_qual);
                /* pass */
                List_TrStr_pop(cstk_pfx);
            }
            /* pass */
            if ((fld_stack->len > 0LL)) {
                /* pass */
                TrStr _strtmp_t1118 = List_TrStr_pop(fld_stack);
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1118;
            } else {
                /* pass */
                TrStr _strtmp_t1119 = _tr_str_lit("");
                _tr_str_release(fld_buf);
                fld_buf = _strtmp_t1119;
            }
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ENS"))) == 0)) {
            /* pass */
            TrStr _strtmp_t1120 = _ns_pop(ns_path);
            _tr_str_release(ns_path);
            ns_path = _strtmp_t1120;
            /* pass */
            continue;
        }
        /* pass */
        if ((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EENUM"))) == 0)) {
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("NS ")))) {
            /* pass */
            TrStr nn = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 3LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                TrStr _strtmp_t1121 = _tr_str_retain(nn);
                _tr_str_release(ns_path);
                ns_path = _strtmp_t1121;
            } else {
                /* pass */
                TrStr _strtmp_t1122 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(nn)); _tr_str_release(_cl); _cres; });
                _tr_str_release(ns_path);
                ns_path = _strtmp_t1122;
            }
            /* pass */
            if (({ TrStr _dkt_t1123 = (_tr_strx_concat(_tr_strz(_tr_str_lit("using:")), _tr_strz(ns_path))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t1123)))); _tr_str_release(_dkt_t1123); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1124 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("using namespace ")), _tr_strz(ns_path))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(usings, _sbt_t1124); _tr_str_release(_sbt_t1124); });
                /* pass */
                ({ TrStr _dkt_t1125 = (_tr_strx_concat(_tr_strz(_tr_str_lit("using:")), _tr_strz(ns_path))); _tr_dict_set(seen, _tr_strz(_dkt_t1125), true); _tr_str_release(_dkt_t1125); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("TCLASS ")))) {
            /* pass */
            List_TrStr* tp2 = ({ TrStr _at_t1126 = (_tr_str_wrap(_tr_str_slice(_tr_strz(line), 7LL, _tr_strlen(_tr_strz(line))))); __auto_type _wr = (_parse_tclass(_at_t1126)); _tr_str_release(_at_t1126); _wr; });
            /* pass */
            TrStr tspell = List_TrStr_get(tp2, 0LL);
            /* pass */
            TrStr tname = _cpp_ident(tspell);
            /* pass */
            List_TrStr_append(cstk_name, tname);
            /* pass */
            List_TrStr_append(cstk_qual, tspell);
            /* pass */
            List_TrStr_append(cstk_pfx, tname);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(tname), _tr_str_box(_tr_str_retain(tspell)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1127 = _tr_str_lit("");
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1127;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("CLASS ")))) {
            /* pass */
            TrStr crest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            TrStr ncls = List_TrStr_get(_tr_str_split(_tr_strz(crest), _tr_strz(_tr_str_lit(" "))), 0LL);
            /* pass */
            TrStr nqual = _tr_str_retain(ncls);
            /* pass */
            TrStr npfx = _tr_str_retain(ncls);
            /* pass */
            if ((strcmp(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t1128 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t1128;
            } else if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t1129 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(nqual);
                nqual = _strtmp_t1129;
            }
            /* pass */
            if ((strcmp(_tr_strz(class_pfx), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t1130 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t1130;
            } else if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                TrStr _strtmp_t1131 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(ncls)); _tr_str_release(_cl); _cres; });
                _tr_str_release(npfx);
                npfx = _strtmp_t1131;
            }
            /* pass */
            List_TrStr_append(cstk_name, ncls);
            /* pass */
            List_TrStr_append(cstk_qual, nqual);
            /* pass */
            List_TrStr_append(cstk_pfx, npfx);
            /* pass */
            _tr_dict_set(class_qual, _tr_strz(ncls), _tr_str_box(_tr_str_retain(nqual)));
            /* pass */
            List_TrStr_append(fld_stack, fld_buf);
            /* pass */
            TrStr _strtmp_t1132 = _tr_str_lit("");
            _tr_str_release(fld_buf);
            fld_buf = _strtmp_t1132;
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("ENUM ")))) {
            /* pass */
            TrStr erest = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* eps = _tr_str_split(_tr_strz(erest), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            TrStr en = List_TrStr_get(eps, 0LL);
            /* pass */
            TrStr eunder = _tr_str_lit("c_int");
            /* pass */
            if ((eps->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1133 = List_TrStr_get(eps, 1LL);
                _tr_str_release(eunder);
                eunder = _strtmp_t1133;
            }
            /* pass */
            if (({ TrStr _dkt_t1134 = (_tr_strx_concat(_tr_strz(_tr_str_lit("enum:")), _tr_strz(en))); __auto_type _wr = ((_is_clean_ident(en) && (!_tr_dict_contains(seen, _tr_strz(_dkt_t1134))))); _tr_str_release(_dkt_t1134); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1135 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("type ")), _tr_strz(en))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(eunder)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t1135); _tr_str_release(_sbt_t1135); });
                /* pass */
                ({ TrStr _dkt_t1136 = (_tr_strx_concat(_tr_strz(_tr_str_lit("enum:")), _tr_strz(en))); _tr_dict_set(seen, _tr_strz(_dkt_t1136), true); _tr_str_release(_dkt_t1136); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("EVAL ")))) {
            /* pass */
            TrStr er = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            List_TrStr* ep = _tr_str_split(_tr_strz(er), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            TrStr ename = List_TrStr_get(ep, 0LL);
            /* pass */
            TrStr eval_ = _tr_str_lit("0");
            /* pass */
            if ((ep->len > 1LL)) {
                /* pass */
                TrStr _strtmp_t1137 = List_TrStr_get(ep, 1LL);
                _tr_str_release(eval_);
                eval_ = _strtmp_t1137;
            }
            /* pass */
            if (({ TrStr _dkt_t1138 = (_tr_strx_concat(_tr_strz(_tr_str_lit("eval:")), _tr_strz(ename))); __auto_type _wr = ((!_tr_dict_contains(seen, _tr_strz(_dkt_t1138)))); _tr_str_release(_dkt_t1138); _wr; })) {
                /* pass */
                ({ TrStr _sbt_t1139 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(ename))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(eval_)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(consts, _sbt_t1139); _tr_str_release(_sbt_t1139); });
                /* pass */
                ({ TrStr _dkt_t1140 = (_tr_strx_concat(_tr_strz(_tr_str_lit("eval:")), _tr_strz(ename))); _tr_dict_set(seen, _tr_strz(_dkt_t1140), true); _tr_str_release(_dkt_t1140); });
            }
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("METHOD ")))) {
            /* pass */
            TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 7LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            TrStr _strtmp_t1141 = _tr_str_wrap(_tr_str_slice(_tr_strz(r), 0LL, 2LL));
            _tr_str_release(m_flags);
            m_flags = _strtmp_t1141;
            /* pass */
            TrStr r2 = _tr_str_wrap(_tr_str_slice(_tr_strz(r), 3LL, _tr_strlen(_tr_strz(r))));
            /* pass */
            __auto_type sp1 = ({ char* _t1142 = strstr(_tr_strz(r2), _tr_strz(_tr_str_lit(" "))); _t1142 ? (long long)(_t1142 - (_tr_strz(r2))) : -1LL; });
            /* pass */
            m_ndef = ({ TrStr _at_t1143 = (_tr_str_wrap(_tr_str_slice(_tr_strz(r2), 0LL, sp1))); __auto_type _wr = (_to_int(_at_t1143)); _tr_str_release(_at_t1143); _wr; });
            /* pass */
            TrStr r3 = _tr_str_wrap(_tr_str_slice(_tr_strz(r2), (sp1 + 1LL), _tr_strlen(_tr_strz(r2))));
            /* pass */
            List_TrStr* rn = _tr_str_split(_tr_strz(r3), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr _strtmp_t1144 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t1144;
            /* pass */
            TrStr _strtmp_t1145 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t1145;
            /* pass */
            TrStr _strtmp_t1146 = _tr_str_lit("");
            _tr_str_release(m_qual);
            m_qual = _strtmp_t1146;
            /* pass */
            if ((rn->len > 2LL)) {
                /* pass */
                TrStr _strtmp_t1147 = List_TrStr_get(rn, 2LL);
                _tr_str_release(m_qual);
                m_qual = _strtmp_t1147;
            }
            /* pass */
            TrStr _strtmp_t1148 = _tr_str_lit("method");
            _tr_str_release(mode);
            mode = _strtmp_t1148;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("FUNC ")))) {
            /* pass */
            TrStr r = _tr_str_wrap(_tr_str_slice(_tr_strz(line), 5LL, _tr_strlen(_tr_strz(line))));
            /* pass */
            __auto_type sp1 = ({ char* _t1149 = strstr(_tr_strz(r), _tr_strz(_tr_str_lit(" "))); _t1149 ? (long long)(_t1149 - (_tr_strz(r))) : -1LL; });
            /* pass */
            m_ndef = ({ TrStr _at_t1150 = (_tr_str_wrap(_tr_str_slice(_tr_strz(r), 0LL, sp1))); __auto_type _wr = (_to_int(_at_t1150)); _tr_str_release(_at_t1150); _wr; });
            /* pass */
            TrStr r3 = _tr_str_wrap(_tr_str_slice(_tr_strz(r), (sp1 + 1LL), _tr_strlen(_tr_strz(r))));
            /* pass */
            List_TrStr* rn = _tr_str_split(_tr_strz(r3), _tr_strz(_tr_str_lit("|")));
            /* pass */
            TrStr _strtmp_t1151 = List_TrStr_get(rn, 0LL);
            _tr_str_release(m_ret);
            m_ret = _strtmp_t1151;
            /* pass */
            TrStr _strtmp_t1152 = List_TrStr_get(rn, 1LL);
            _tr_str_release(m_name);
            m_name = _strtmp_t1152;
            /* pass */
            TrStr _strtmp_t1153 = _tr_str_lit("");
            _tr_str_release(m_qual);
            m_qual = _strtmp_t1153;
            /* pass */
            TrStr _strtmp_t1154 = _tr_str_lit("..");
            _tr_str_release(m_flags);
            m_flags = _strtmp_t1154;
            /* pass */
            TrStr _strtmp_t1155 = _tr_str_lit("func");
            _tr_str_release(mode);
            mode = _strtmp_t1155;
            /* pass */
            params = (void*)List_TrStr_new();
            /* pass */
            continue;
        }
        /* pass */
        if (_tr_str_starts_with(_tr_strz(line), _tr_strz(_tr_str_lit("PARAM ")))) {
            /* pass */
            ({ TrStr _at_t1156 = (_tr_str_wrap(_tr_str_slice(_tr_strz(line), 6LL, _tr_strlen(_tr_strz(line))))); List_TrStr_append(params, _at_t1156); _tr_str_release(_at_t1156); });
            /* pass */
            continue;
        }
        /* pass */
        if ((((strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EMETHOD"))) == 0) || (strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("ECTOR"))) == 0)) || (strcmp(_tr_strz(line), _tr_strz(_tr_str_lit("EFUNC"))) == 0))) {
            /* pass */
            bool is_static = ({ TrStr _wt_t1157 = (_tr_str_wrap(_tr_str_slice(_tr_strz(m_flags), 0LL, 1LL))); __auto_type _wr = ((strcmp(_wt_t1157.data, _tr_strz(_tr_str_lit("s"))) == 0)); _tr_str_release(_wt_t1157); _wr; });
            /* pass */
            bool is_const = ({ TrStr _wt_t1158 = (_tr_str_wrap(_tr_str_slice(_tr_strz(m_flags), 1LL, 2LL))); __auto_type _wr = ((strcmp(_wt_t1158.data, _tr_strz(_tr_str_lit("c"))) == 0)); _tr_str_release(_wt_t1158); _wr; });
            /* pass */
            TrStr m_sym = _tr_str_retain(m_name);
            /* pass */
            if (({ TrStr _wt_t1159 = (_tr_str_wrap(_tr_str_slice(_tr_strz(m_name), 0LL, 8LL))); __auto_type _wr = ((((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) != 0) && (_tr_strlen(_tr_strz(m_name)) >= 8LL)) && (strcmp(_wt_t1159.data, _tr_strz(_tr_str_lit("operator"))) == 0))); _tr_str_release(_wt_t1159); _wr; })) {
                /* pass */
                TrStr _strtmp_t1160 = _cpp_op_name(m_name, params->len, (strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0));
                _tr_str_release(m_sym);
                m_sym = _strtmp_t1160;
                /* pass */
                if ((strcmp(_tr_strz(m_sym), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t1161 = _tr_str_lit("");
                    _tr_str_release(mode);
                    mode = _strtmp_t1161;
                    /* pass */
                    continue;
                }
            }
            /* pass */
            if ((((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) == 0) || (strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0)) && (!_is_clean_ident(cur_class)))) {
                /* pass */
                TrStr _strtmp_t1162 = _tr_str_lit("");
                _tr_str_release(mode);
                mode = _strtmp_t1162;
                /* pass */
                continue;
            }
            /* pass */
            bool has_dep = ((({ char* _t1163 = strstr(_tr_strz(m_ret), _tr_strz(_tr_str_lit("type-parameter"))); _t1163 ? (long long)(_t1163 - (_tr_strz(m_ret))) : -1LL; }) >= 0LL) || (({ char* _t1164 = strstr(_tr_strz(m_ret), _tr_strz(_tr_str_lit("~d~"))); _t1164 ? (long long)(_t1164 - (_tr_strz(m_ret))) : -1LL; }) >= 0LL));
            /* pass */
            long long pj = 0LL;
            /* pass */
            while ((pj < params->len)) {
                /* pass */
                TrStr pvd = List_TrStr_get(params, pj);
                /* pass */
                if (((({ char* _t1165 = strstr(_tr_strz(pvd), _tr_strz(_tr_str_lit("type-parameter"))); _t1165 ? (long long)(_t1165 - (_tr_strz(pvd))) : -1LL; }) >= 0LL) || (({ char* _t1166 = strstr(_tr_strz(pvd), _tr_strz(_tr_str_lit("~d~"))); _t1166 ? (long long)(_t1166 - (_tr_strz(pvd))) : -1LL; }) >= 0LL))) {
                    /* pass */
                    has_dep = true;
                }
                /* pass */
                if ((((strcmp(_tr_strz(m_name), _tr_strz(_tr_str_lit("operator()"))) == 0) && _tr_str_starts_with(_tr_strz(pvd), _tr_strz(_tr_str_lit("0~")))) && (({ char* _t1167 = strstr(_tr_strz(pvd), _tr_strz(_tr_str_lit("~u~"))); _t1167 ? (long long)(_t1167 - (_tr_strz(pvd))) : -1LL; }) >= 0LL))) {
                    /* pass */
                    has_dep = true;
                }
                /* pass */
                pj = (pj + 1LL);
                _tr_str_release(pvd);
            }
            /* pass */
            if (has_dep) {
                /* pass */
                TrStr _strtmp_t1168 = _tr_str_lit("");
                _tr_str_release(mode);
                mode = _strtmp_t1168;
                /* pass */
                continue;
            }
            /* pass */
            List_TrStr* full_params = (void*)List_TrStr_new();
            /* pass */
            long long fpi = 0LL;
            /* pass */
            while ((fpi < params->len)) {
                /* pass */
                ({ TrStr _at_t1169 = (List_TrStr_get(params, fpi)); List_TrStr_append(full_params, _at_t1169); _tr_str_release(_at_t1169); });
                /* pass */
                fpi = (fpi + 1LL);
            }
            /* pass */
            long long min_arity = (full_params->len - m_ndef);
            /* pass */
            if ((min_arity < 0LL)) {
                /* pass */
                min_arity = 0LL;
            }
            /* pass */
            long long arity = full_params->len;
            /* pass */
            while ((arity >= min_arity)) {
                /* pass */
                List_TrStr* tparams = (void*)List_TrStr_new();
                /* pass */
                long long tti = 0LL;
                /* pass */
                while ((tti < arity)) {
                    /* pass */
                    ({ TrStr _at_t1170 = (List_TrStr_get(full_params, tti)); List_TrStr_append(tparams, _at_t1170); _tr_str_release(_at_t1170); });
                    /* pass */
                    tti = (tti + 1LL);
                }
                /* pass */
                params = tparams;
                /* pass */
                TrStr shimp = _tr_str_lit("");
                /* pass */
                TrStr trp = _tr_str_lit("");
                /* pass */
                TrStr fargs = _tr_str_lit("");
                /* pass */
                if (((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0) && (!is_static))) {
                    /* pass */
                    if (is_const) {
                        /* pass */
                        TrStr _strtmp_t1171 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("const ")), _tr_strz(cur_class_qual))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("* self"))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1171;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1172 = _tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("* self")));
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1172;
                    }
                    /* pass */
                    TrStr _strtmp_t1173 = _tr_strx_concat(_tr_strz(_tr_str_lit("obj: ")), _tr_strz(cur_class));
                    _tr_str_release(trp);
                    trp = _strtmp_t1173;
                }
                /* pass */
                long long pi = 0LL;
                /* pass */
                while ((pi < params->len)) {
                    /* pass */
                    TrStr pv = List_TrStr_get(params, pi);
                    /* pass */
                    pi = (pi + 1LL);
                    /* pass */
                    List_TrStr* bar = _tr_str_split(_tr_strz(pv), _tr_strz(_tr_str_lit("|")));
                    /* pass */
                    TrStr ptype = List_TrStr_get(bar, 0LL);
                    /* pass */
                    TrStr pname = _tr_str_lit("");
                    /* pass */
                    if ((bar->len > 1LL)) {
                        /* pass */
                        TrStr _strtmp_t1174 = List_TrStr_get(bar, 1LL);
                        _tr_str_release(pname);
                        pname = _strtmp_t1174;
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(pname), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1175 = ({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(pi)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("a")), _cr.data); _tr_str_release(_cr); _cres; });
                        _tr_str_release(pname);
                        pname = _strtmp_t1175;
                    }
                    /* pass */
                    TrStr spname = _tr_str_retain(pname);
                    /* pass */
                    if ((strcmp(_tr_strz(spname), _tr_strz(_tr_str_lit("self"))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1176 = _tr_str_lit("self_");
                        _tr_str_release(spname);
                        spname = _strtmp_t1176;
                    }
                    /* pass */
                    TrStr tpname = _cpp_tr_pname(pname);
                    /* pass */
                    List_TrStr* d4 = _desc4(ptype);
                    /* pass */
                    long long pdepth = ({ TrStr _at_t1177 = (List_TrStr_get(d4, 0LL)); __auto_type _wr = (_to_int(_at_t1177)); _tr_str_release(_at_t1177); _wr; });
                    /* pass */
                    TrStr prefk = List_TrStr_get(d4, 1LL);
                    /* pass */
                    bool pref = (strcmp(_tr_strz(prefk), _tr_strz(_tr_str_lit("0"))) != 0);
                    /* pass */
                    TrStr pcat = List_TrStr_get(d4, 2LL);
                    /* pass */
                    TrStr pdet = List_TrStr_get(d4, 3LL);
                    /* pass */
                    TrStr sc = _tr_str_lit("");
                    /* pass */
                    TrStr fw = _tr_str_lit("");
                    /* pass */
                    TrStr trt = _tr_str_lit("");
                    /* pass */
                    if ((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("p"))) == 0)) {
                        /* pass */
                        TrStr cpp = _c_to_cpp(pdet);
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1178 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cpp), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1178;
                            /* pass */
                            TrStr _strtmp_t1179 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1179;
                            /* pass */
                            TrStr _strtmp_t1180 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t1180;
                        } else if (((strcmp(_tr_strz(prefk), _tr_strz(_tr_str_lit("2"))) == 0) && (pdepth == 1LL))) {
                            /* pass */
                            TrStr _strtmp_t1181 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(cpp), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1181;
                            /* pass */
                            TrStr _strtmp_t1182 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1182;
                            /* pass */
                            TrStr _strtmp_t1183 = _tr_str_retain(pdet);
                            _tr_str_release(trt);
                            trt = _strtmp_t1183;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1184 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cpp), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (_stars(pdepth)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1184;
                            /* pass */
                            if (pref) {
                                /* pass */
                                TrStr _strtmp_t1185 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1185;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1186 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1186;
                            }
                            /* pass */
                            TrStr _strtmp_t1187 = _ptr_wrap(pdet, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t1187;
                        }
                    } else if ((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("e"))) == 0)) {
                        /* pass */
                        TrStr espell = List_TrStr_get(_tr_str_split(_tr_strz(pdet), _tr_strz(_tr_str_lit("#"))), 0LL);
                        /* pass */
                        TrStr ealias = _last_seg(espell);
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1188 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(espell), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1188;
                            /* pass */
                            TrStr _strtmp_t1189 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1189;
                            /* pass */
                            TrStr _strtmp_t1190 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t1190;
                        } else if (((strcmp(_tr_strz(prefk), _tr_strz(_tr_str_lit("2"))) == 0) && (pdepth == 1LL))) {
                            /* pass */
                            TrStr _strtmp_t1191 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(espell), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1191;
                            /* pass */
                            TrStr _strtmp_t1192 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1192;
                            /* pass */
                            TrStr _strtmp_t1193 = _tr_str_retain(ealias);
                            _tr_str_release(trt);
                            trt = _strtmp_t1193;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1194 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(espell), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (_stars(pdepth)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1194;
                            /* pass */
                            if (pref) {
                                /* pass */
                                TrStr _strtmp_t1195 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1195;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1196 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1196;
                            }
                            /* pass */
                            TrStr _strtmp_t1197 = _ptr_wrap(ealias, pdepth);
                            _tr_str_release(trt);
                            trt = _strtmp_t1197;
                        }
                    } else if (((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("s"))) == 0) && (strcmp(_tr_strz(pdet), _tr_strz(_tr_str_lit("string"))) == 0))) {
                        /* pass */
                        TrStr _strtmp_t1198 = _tr_strx_concat(_tr_strz(_tr_str_lit("const char* ")), _tr_strz(spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t1198;
                        /* pass */
                        TrStr _strtmp_t1199 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("std::string(")), _tr_strz(spname))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t1199;
                        /* pass */
                        TrStr _strtmp_t1200 = _tr_str_lit("Pointer[char]");
                        _tr_str_release(trt);
                        trt = _strtmp_t1200;
                    } else if ((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("f"))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1201 = _tr_strx_concat(_tr_strz(_tr_str_lit("void* ")), _tr_strz(spname));
                        _tr_str_release(sc);
                        sc = _strtmp_t1201;
                        /* pass */
                        TrStr _strtmp_t1202 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_fnptr_cast(pdet)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("(")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fw);
                        fw = _strtmp_t1202;
                        /* pass */
                        TrStr _strtmp_t1203 = _tr_str_lit("Pointer[void]");
                        _tr_str_release(trt);
                        trt = _strtmp_t1203;
                    } else if ((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("v"))) == 0)) {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1204 = ({ TrStr _cl = (({ TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("void ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1204;
                        /* pass */
                        if (pref) {
                            /* pass */
                            TrStr _strtmp_t1205 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1205;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1206 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1206;
                        }
                        /* pass */
                        TrStr _strtmp_t1207 = _ptr_wrap(_tr_str_lit("void"), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t1207;
                    } else if ((strcmp(_tr_strz(pcat), _tr_strz(_tr_str_lit("r"))) == 0)) {
                        /* pass */
                        TrStr seg = _last_seg(pdet);
                        /* pass */
                        if ((_tr_dict_contains(value_structs, _tr_strz(seg)) && (pdepth == 0LL))) {
                            /* pass */
                            TrStr _strtmp_t1208 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(pdet), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1208;
                            /* pass */
                            TrStr _strtmp_t1209 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1209;
                            /* pass */
                            TrStr _strtmp_t1210 = _tr_str_retain(seg);
                            _tr_str_release(trt);
                            trt = _strtmp_t1210;
                        } else {
                            /* pass */
                            long long nd = pdepth;
                            /* pass */
                            if ((nd < 1LL)) {
                                /* pass */
                                nd = 1LL;
                            }
                            /* pass */
                            TrStr _strtmp_t1211 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_cpp_qual(pdet, class_qual)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sc);
                            sc = _strtmp_t1211;
                            /* pass */
                            if ((pdepth == 0LL)) {
                                /* pass */
                                TrStr _strtmp_t1212 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1212;
                            } else if (pref) {
                                /* pass */
                                TrStr _strtmp_t1213 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                                _tr_str_release(fw);
                                fw = _strtmp_t1213;
                            } else {
                                /* pass */
                                TrStr _strtmp_t1214 = _tr_str_retain(spname);
                                _tr_str_release(fw);
                                fw = _strtmp_t1214;
                            }
                            /* pass */
                            TrStr _strtmp_t1215 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                            _tr_str_release(trt);
                            trt = _strtmp_t1215;
                        }
                    } else if ((strcmp(_tr_strz(pdet), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1216 = ({ TrStr _cl = (({ TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("void ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1216;
                        /* pass */
                        TrStr _strtmp_t1217 = _tr_str_retain(spname);
                        _tr_str_release(fw);
                        fw = _strtmp_t1217;
                        /* pass */
                        TrStr _strtmp_t1218 = _ptr_wrap(_tr_str_lit("void"), nd);
                        _tr_str_release(trt);
                        trt = _strtmp_t1218;
                    } else {
                        /* pass */
                        long long nd = pdepth;
                        /* pass */
                        if ((nd < 1LL)) {
                            /* pass */
                            nd = 1LL;
                        }
                        /* pass */
                        TrStr _strtmp_t1219 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pdet), _tr_strz(_tr_str_lit(" ")))); TrStr _cr = (_stars(nd)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(spname)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sc);
                        sc = _strtmp_t1219;
                        /* pass */
                        if ((pdepth == 0LL)) {
                            /* pass */
                            TrStr _strtmp_t1220 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1220;
                        } else if (pref) {
                            /* pass */
                            TrStr _strtmp_t1221 = _tr_strx_concat(_tr_strz(_tr_str_lit("*")), _tr_strz(spname));
                            _tr_str_release(fw);
                            fw = _strtmp_t1221;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1222 = _tr_str_retain(spname);
                            _tr_str_release(fw);
                            fw = _strtmp_t1222;
                        }
                        /* pass */
                        TrStr _strtmp_t1223 = _cpp_opaque_handle(pdet, nd, class_names, value_structs, seen, opaque);
                        _tr_str_release(trt);
                        trt = _strtmp_t1223;
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(shimp), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1224 = _tr_str_retain(sc);
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1224;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1225 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(shimp), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sc)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(shimp);
                        shimp = _strtmp_t1225;
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(fargs), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1226 = _tr_str_retain(fw);
                        _tr_str_release(fargs);
                        fargs = _strtmp_t1226;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1227 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(fargs), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fw)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(fargs);
                        fargs = _strtmp_t1227;
                    }
                    /* pass */
                    if ((strcmp(_tr_strz(trp), _tr_strz(_tr_str_lit(""))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1228 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(tpname), _tr_strz(_tr_str_lit(": ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trt)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(trp);
                        trp = _strtmp_t1228;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1229 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(trp), _tr_strz(_tr_str_lit(", ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(tpname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(": "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trt)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(trp);
                        trp = _strtmp_t1229;
                    }
                    _tr_str_release(pv);
                    List_TrStr_free(bar);
                    _tr_str_release(ptype);
                    _tr_str_release(pname);
                    _tr_str_release(spname);
                    _tr_str_release(tpname);
                    List_TrStr_free(d4);
                    _tr_str_release(prefk);
                    _tr_str_release(pcat);
                    _tr_str_release(pdet);
                    _tr_str_release(sc);
                    _tr_str_release(fw);
                    _tr_str_release(trt);
                }
                /* pass */
                TrStr call = _tr_str_lit("");
                /* pass */
                if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) == 0)) {
                    /* pass */
                    TrStr _strtmp_t1230 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("new ")), _tr_strz(cur_class_qual))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t1230;
                } else if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0)) {
                    /* pass */
                    TrStr mcall = _tr_str_retain(m_name);
                    /* pass */
                    if ((strcmp(_tr_strz(m_qual), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t1231 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(m_qual), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_name)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(mcall);
                        mcall = _strtmp_t1231;
                    }
                    /* pass */
                    if (is_static) {
                        /* pass */
                        TrStr _strtmp_t1232 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("::")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(call);
                        call = _strtmp_t1232;
                    } else {
                        /* pass */
                        TrStr _strtmp_t1233 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("self->")), _tr_strz(mcall))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                        _tr_str_release(call);
                        call = _strtmp_t1233;
                    }
                } else {
                    /* pass */
                    TrStr pre = _tr_str_lit("");
                    /* pass */
                    if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t1234 = _tr_strx_concat(_tr_strz(ns_path), _tr_strz(_tr_str_lit("::")));
                        _tr_str_release(pre);
                        pre = _strtmp_t1234;
                    }
                    /* pass */
                    TrStr _strtmp_t1235 = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(pre), _tr_strz(m_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(fargs)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(call);
                    call = _strtmp_t1235;
                    _tr_str_release(pre);
                }
                /* pass */
                if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("ctor"))) == 0)) {
                    /* pass */
                    TrStr sym = ({ TrStr _at_t1236 = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_new")))); __auto_type _wr = (_uniq_sym(_at_t1236, sym_used)); _tr_str_release(_at_t1236); _wr; });
                    /* pass */
                    ({ TrStr _at_t1237 = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("*")))); TrStr _at_t1238 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("return ")), _tr_strz(call))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";"))); _tr_str_release(_cl); _cres; })); TrStr _sbt_t1239 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(cur_class_qual), _tr_strz(_tr_str_lit("* ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1237, _at_t1238)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1239); _tr_str_release(_at_t1237); _tr_str_release(_at_t1238); _tr_str_release(_sbt_t1239); });
                    /* pass */
                    ({ TrStr _sbt_t1240 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") -> "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cur_class)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(decls, _sbt_t1240); _tr_str_release(_sbt_t1240); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym);
                } else {
                    /* pass */
                    List_TrStr* ri = _cpp_ret_ex(m_ret, call, value_structs, class_names, class_qual, seen, opaque);
                    /* pass */
                    TrStr sym0 = _tr_str_lit("");
                    /* pass */
                    if ((strcmp(_tr_strz(mode), _tr_strz(_tr_str_lit("method"))) == 0)) {
                        /* pass */
                        TrStr _strtmp_t1241 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(class_pfx), _tr_strz(_tr_str_lit("_")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_sym)); _tr_str_release(_cl); _cres; });
                        _tr_str_release(sym0);
                        sym0 = _strtmp_t1241;
                    } else {
                        /* pass */
                        if ((strcmp(_tr_strz(ns_path), _tr_strz(_tr_str_lit(""))) == 0)) {
                            /* pass */
                            TrStr _strtmp_t1242 = _tr_strx_concat(_tr_strz(_tr_str_lit("g_")), _tr_strz(m_sym));
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t1242;
                        } else {
                            /* pass */
                            TrStr _strtmp_t1243 = ({ TrStr _cl = (({ TrStr _cl = (_ns_us(ns_path)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(m_sym)); _tr_str_release(_cl); _cres; });
                            _tr_str_release(sym0);
                            sym0 = _strtmp_t1243;
                        }
                    }
                    /* pass */
                    TrStr sym = _uniq_sym(sym0, sym_used);
                    /* pass */
                    ({ TrStr _at_t1244 = (List_TrStr_get(ri, 0LL)); TrStr _at_t1245 = (List_TrStr_get(ri, 1LL)); TrStr _sbt_t1246 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (List_TrStr_get(ri, 0LL)); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(sym)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shimp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(") { "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_shim_body(_at_t1244, _at_t1245)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" }\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(shim, _sbt_t1246); _tr_str_release(_at_t1244); _tr_str_release(_at_t1245); _tr_str_release(_sbt_t1246); });
                    /* pass */
                    TrStr d = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    def ")), _tr_strz(sym))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("("))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(trp)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(")"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if ((strcmp(_tr_strz(List_TrStr_get(ri, 2LL)), _tr_strz(_tr_str_lit(""))) != 0)) {
                        /* pass */
                        TrStr _strtmp_t1247 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(d), _tr_strz(_tr_str_lit(" -> ")))); TrStr _cr = (List_TrStr_get(ri, 2LL)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
                        _tr_str_release(d);
                        d = _strtmp_t1247;
                    }
                    /* pass */
                    ({ TrStr _sbt_t1248 = (_tr_strx_concat(_tr_strz(d), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(decls, _sbt_t1248); _tr_str_release(_sbt_t1248); });
                    /* pass */
                    n_fns = (n_fns + 1LL);
                    _tr_str_release(sym0);
                    _tr_str_release(sym);
                    _tr_str_release(d);
                }
                /* pass */
                arity = (arity - 1LL);
                _tr_str_release(shimp);
                _tr_str_release(trp);
                _tr_str_release(fargs);
                _tr_str_release(call);
            }
            /* pass */
            TrStr _strtmp_t1249 = _tr_str_lit("");
            _tr_str_release(mode);
            mode = _strtmp_t1249;
            /* pass */
            continue;
        }
        _tr_str_release(line);
        _tr_str_release(cur_class);
        _tr_str_release(cur_class_qual);
        _tr_str_release(class_pfx);
    }
    /* pass */
    TrStr base = _tr_str_retain(out);
    /* pass */
    if (({ TrStr _wt_t1250 = (_tr_str_wrap(_tr_str_slice(_tr_strz(base), (_tr_strlen(_tr_strz(base)) - 3LL), _tr_strlen(_tr_strz(base))))); __auto_type _wr = (((_tr_strlen(_tr_strz(base)) > 3LL) && (strcmp(_wt_t1250.data, _tr_strz(_tr_str_lit(".tr"))) == 0))); _tr_str_release(_wt_t1250); _wr; })) {
        /* pass */
        TrStr _strtmp_t1251 = _tr_str_wrap(_tr_str_slice(_tr_strz(base), 0LL, (_tr_strlen(_tr_strz(base)) - 3LL)));
        _tr_str_release(base);
        base = _strtmp_t1251;
    }
    /* pass */
    TrStr shim_name = _tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("_shim.cpp")));
    /* pass */
    StringBuilder* sb = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t1252 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# Auto-generated C++ FFI bindings for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen -h cpp).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1252); _tr_str_release(_sbt_t1252); });
    /* pass */
    ({ TrStr _sbt_t1253 = (({ TrStr _cl = (({ TrStr _cr = (_basename(shim_name)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-shim: ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1253); _tr_str_release(_sbt_t1253); });
    /* pass */
    StringBuilder_append(sb, _tr_str_lit("# tauraro-cpp-lib: stdc++\n"));
    /* pass */
    if ((strcmp(_tr_strz(pkglibs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t1254 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-linkflags:")), _tr_strz(pkglibs))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1254); _tr_str_release(_sbt_t1254); });
    }
    /* pass */
    if ((strcmp(_tr_strz(shim_cflags), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t1255 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# tauraro-cpp-cflags: ")), _tr_strz(shim_cflags))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1255); _tr_str_release(_sbt_t1255); });
    }
    /* pass */
    ({ TrStr _sbt_t1256 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("# (opt out with --no-auto-cpp, then: c++ -c ")), _tr_strz(shim_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" ; tauraroc app.tr --link "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(base)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("_shim.o -lstdc++)\n\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t1256); _tr_str_release(_sbt_t1256); });
    /* pass */
    TrStr os_ = StringObj_as_str(StringBuilder_to_string(opaque));
    /* pass */
    TrStr cs = StringObj_as_str(StringBuilder_to_string(consts));
    /* pass */
    TrStr ds2 = StringObj_as_str(StringBuilder_to_string(decls));
    /* pass */
    TrMap* defined_names = _tr_dict_new(64LL);
    /* pass */
    List_TrStr* dlns = _tr_str_split(_tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(os_), _tr_strz(_tr_str_lit("\n")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(cs)); _tr_str_release(_cl); _cres; })), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long dii = 0LL;
    /* pass */
    while ((dii < dlns->len)) {
        /* pass */
        TrStr dl = List_TrStr_get(dlns, dii);
        /* pass */
        dii = (dii + 1LL);
        /* pass */
        if (_tr_str_starts_with(_tr_strz(dl), _tr_strz(_tr_str_lit("class ")))) {
            /* pass */
            TrStr dn = _ident_at(dl, 6LL);
            /* pass */
            if ((strcmp(_tr_strz(dn), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        } else if (_tr_str_starts_with(_tr_strz(dl), _tr_strz(_tr_str_lit("type ")))) {
            /* pass */
            TrStr dn = _ident_at(dl, 5LL);
            /* pass */
            if ((strcmp(_tr_strz(dn), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                _tr_dict_set(defined_names, _tr_strz(dn), true);
            }
        }
        _tr_str_release(dl);
    }
    /* pass */
    TrStr fb = ({ TrStr _at_t1257 = (_tr_strx_concat(_tr_strz(os_), _tr_strz(ds2))); __auto_type _wr = (_opaque_fallbacks(_at_t1257, defined_names)); _tr_str_release(_at_t1257); _wr; });
    /* pass */
    if ((strcmp(_tr_strz(fb), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, fb);
    }
    /* pass */
    if ((strcmp(_tr_strz(os_), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t1258 = (_tr_strx_concat(_tr_strz(os_), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t1258); _tr_str_release(_sbt_t1258); });
    }
    /* pass */
    if ((strcmp(_tr_strz(cs), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ TrStr _sbt_t1259 = (_tr_strx_concat(_tr_strz(cs), _tr_strz(_tr_str_lit("\n")))); StringBuilder_append(sb, _sbt_t1259); _tr_str_release(_sbt_t1259); });
    }
    /* pass */
    if ((strcmp(_tr_strz(ds2), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("extern \"C\":\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("    def tauraro_cpp_last_error() -> Pointer[char]\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("    def tauraro_cpp_clear_error()\n"));
        /* pass */
        StringBuilder_append(sb, ds2);
    }
    /* pass */
    ({ TrStr _at_t1260 = (StringObj_as_str(StringBuilder_to_string(sb))); write_file(out, _at_t1260); _tr_str_release(_at_t1260); });
    /* pass */
    StringBuilder* sh = StringBuilder_init(4096LL);
    /* pass */
    ({ TrStr _sbt_t1261 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("// Auto-generated C++ -> C shim for ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" (tauraro-bindgen -h cpp).\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1261); _tr_str_release(_sbt_t1261); });
    /* pass */
    ({ TrStr _sbt_t1262 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("// Compile:  c++ -c ")), _tr_strz(shim_name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1262); _tr_str_release(_sbt_t1262); });
    /* pass */
    ({ TrStr _sbt_t1263 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#include \"")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sh, _sbt_t1263); _tr_str_release(_sbt_t1263); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("#include <string>\n#include <string_view>\n#include <cstring>\n#include <cstdlib>\n#include <iterator>\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("// std::string return -> heap char* copy (caller owns it; free with the runtime free).\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("static char* _tr_cpp_strdup(std::string_view s){ char* p=(char*)malloc(s.size()+1); if(p){ memcpy(p, s.data(), s.size()); p[s.size()]=0; } return p; }\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("// Exception safety: a C++ exception must never cross `extern \"C\"` (it would std::terminate).\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("// Every wrapper catches, records the message here, and returns a zero value; the caller\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("// can read/clear it via tauraro_cpp_last_error()/tauraro_cpp_clear_error().\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("static thread_local std::string _tr_cpp_err;\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("static void _tr_cpp_set_error(const char* m){ _tr_cpp_err = m ? m : \"C++ exception\"; }\n"));
    /* pass */
    ({ TrStr _sbt_t1264 = (StringObj_as_str(StringBuilder_to_string(usings))); StringBuilder_append(sh, _sbt_t1264); _tr_str_release(_sbt_t1264); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("extern \"C\" {\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("const char* tauraro_cpp_last_error(void){ return _tr_cpp_strdup(_tr_cpp_err); }\n"));
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("void tauraro_cpp_clear_error(void){ _tr_cpp_err.clear(); }\n"));
    /* pass */
    ({ TrStr _sbt_t1265 = (StringObj_as_str(StringBuilder_to_string(shim))); StringBuilder_append(sh, _sbt_t1265); _tr_str_release(_sbt_t1265); });
    /* pass */
    StringBuilder_append(sh, _tr_str_lit("}\n"));
    /* pass */
    ({ TrStr _at_t1266 = (StringObj_as_str(StringBuilder_to_string(sh))); write_file(shim_name, _at_t1266); _tr_str_release(_at_t1266); });
    /* pass */
    ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: wrote ")), _tr_strz(out))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" + "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(shim_name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" — "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_classes)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" classes, "))); _tr_str_release(_cl); _cres; })); TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_fns)))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" wrappers"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    /* pass */
    ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: next:  import this module and build — tauraroc auto-compiles + links the shim (zero-cost)."))); printf("\n"); });
    _tr_str_release(ns_path);
    List_TrStr_free(cstk_name);
    List_TrStr_free(cstk_qual);
    List_TrStr_free(cstk_pfx);
    _tr_str_release(fld_buf);
    List_TrStr_free(fld_stack);
    List_TrStr_free(ps_name);
    List_TrStr_free(ps_pod);
    List_TrStr_free(ps_nf);
    List_TrStr_free(plines);
    _tr_str_release(mode);
    _tr_str_release(m_flags);
    _tr_str_release(m_ret);
    _tr_str_release(m_name);
    _tr_str_release(m_qual);
    List_TrStr_free(params);
    List_TrStr_free(lines);
    _tr_str_release(base);
    _tr_str_release(shim_name);
    _tr_str_release(os_);
    _tr_str_release(cs);
    _tr_str_release(ds2);
    List_TrStr_free(dlns);
    _tr_str_release(fb);
    StringBuilder__tr_fn_free(opaque);
    StringBuilder__tr_fn_free(consts);
    StringBuilder__tr_fn_free(decls);
    StringBuilder__tr_fn_free(shim);
    StringBuilder__tr_fn_free(usings);
    StringBuilder__tr_fn_free(sb);
    StringBuilder__tr_fn_free(sh);
}

__attribute__((hot)) TrStr _cpp_detect_include_dirs(TrStr cc) {
    /* pass */
    write_file(_tr_str_lit("_cxx_empty.cpp"), _tr_str_lit("\n"));
    /* pass */
    ({ TrStr _aet_t1267 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" -x c++ -E -v _cxx_empty.cpp > _cxx_pp.txt 2> _cxx_inc.txt")))); _tr_system(_aet_t1267.data); _tr_str_release(_aet_t1267); });
    /* pass */
    TrStr out = _tr_str_lit("");
    /* pass */
    if (file_exists(_tr_str_lit("_cxx_inc.txt"))) {
        /* pass */
        TrStr txt = read_file(_tr_str_lit("_cxx_inc.txt"));
        /* pass */
        List_TrStr* lines = _tr_str_split(_tr_strz(txt), _tr_strz(_tr_str_lit("\n")));
        /* pass */
        bool collecting = false;
        /* pass */
        long long li = 0LL;
        /* pass */
        while ((li < lines->len)) {
            /* pass */
            TrStr ln = ({ TrStr _at_t1268 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1268)); _tr_str_release(_at_t1268); _wr; });
            /* pass */
            li = (li + 1LL);
            /* pass */
            if ((({ char* _t1269 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("search starts here"))); _t1269 ? (long long)(_t1269 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
                /* pass */
                collecting = true;
                /* pass */
                continue;
            }
            /* pass */
            if ((({ char* _t1270 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("End of search list"))); _t1270 ? (long long)(_t1270 - (_tr_strz(ln))) : -1LL; }) >= 0LL)) {
                /* pass */
                collecting = false;
                /* pass */
                continue;
            }
            /* pass */
            if (collecting) {
                /* pass */
                TrStr d = _lstrip(ln);
                /* pass */
                __auto_type fpos = ({ char* _t1271 = strstr(_tr_strz(d), _tr_strz(_tr_str_lit(" (framework directory)"))); _t1271 ? (long long)(_t1271 - (_tr_strz(d))) : -1LL; });
                /* pass */
                if ((fpos >= 0LL)) {
                    /* pass */
                    TrStr _strtmp_t1272 = _tr_str_wrap(_tr_str_slice(_tr_strz(d), 0LL, fpos));
                    _tr_str_release(d);
                    d = _strtmp_t1272;
                }
                /* pass */
                if ((strcmp(_tr_strz(d), _tr_strz(_tr_str_lit(""))) != 0)) {
                    /* pass */
                    TrStr _strtmp_t1273 = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(out), _tr_strz(_tr_str_lit(" -I\"")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(d)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\""))); _tr_str_release(_cl); _cres; });
                    _tr_str_release(out);
                    out = _strtmp_t1273;
                }
            }
            _tr_str_release(ln);
        }
        _tr_str_release(txt);
    }
    /* pass */
    _rm_files(_tr_str_lit("_cxx_empty.cpp _cxx_pp.txt _cxx_inc.txt"));
    /* pass */
    return out;
}

__attribute__((hot)) TrStr _cpp_std_flag(TrStr extra) {
    /* pass */
    if ((({ char* _t1274 = strstr(_tr_strz(extra), _tr_strz(_tr_str_lit("-std="))); _t1274 ? (long long)(_t1274 - (_tr_strz(extra))) : -1LL; }) >= 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_lit("-std=c++17 ");
}

__attribute__((hot)) long long _cpp_fatal_count(TrStr diag) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(diag), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long f = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1275 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1275)); _tr_str_release(_at_t1275); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if ((({ char* _t1276 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("DIAGERR "))); _t1276 ? (long long)(_t1276 - (_tr_strz(ln))) : -1LL; }) == 0LL)) {
            /* pass */
            List_TrStr* parts = _tr_str_split(_tr_strz(ln), _tr_strz(_tr_str_lit(" ")));
            /* pass */
            if ((parts->len >= 3LL)) {
                /* pass */
                f = ({ TrStr _at_t1277 = (List_TrStr_get(parts, 2LL)); __auto_type _wr = (_to_int(_at_t1277)); _tr_str_release(_at_t1277); _wr; });
            }
        }
        _tr_str_release(ln);
    }
    /* pass */
    List_TrStr_free(lines);
    return f;
}

__attribute__((hot)) void _cpp_print_diag(TrStr diag) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(diag), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    long long shown = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1278 = (List_TrStr_get(lines, li)); __auto_type _wr = (_rstrip_cr(_at_t1278)); _tr_str_release(_at_t1278); _wr; });
        /* pass */
        li = (li + 1LL);
        /* pass */
        if (((({ char* _t1279 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("DIAG "))); _t1279 ? (long long)(_t1279 - (_tr_strz(ln))) : -1LL; }) == 0LL) && (shown < 12LL))) {
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cr = (_tr_str_wrap(_tr_str_slice(_tr_strz(ln), 5LL, _tr_strlen(_tr_strz(ln))))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("    ")), _cr.data); _tr_str_release(_cr); _cres; }))); printf("\n"); });
            /* pass */
            shown = (shown + 1LL);
        }
        _tr_str_release(ln);
    }
    List_TrStr_free(lines);
}

__attribute__((hot)) bool _cpp_ir_is_empty(TrStr ir) {
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long li = 0LL;
    /* pass */
    while ((li < lines->len)) {
        /* pass */
        if (({ TrStr _at_t1280 = (List_TrStr_get(lines, li)); __auto_type _wr = ((strcmp(_tr_strz(_rstrip_cr(_at_t1280)), _tr_strz(_tr_str_lit(""))) != 0)); _tr_str_release(_at_t1280); _wr; })) {
            /* pass */
            List_TrStr_free(lines);
            return false;
        }
        /* pass */
        li = (li + 1LL);
    }
    /* pass */
    List_TrStr_free(lines);
    return true;
}

__attribute__((hot)) void _cpp_cleanup() {
    /* pass */
    _rm_files(_tr_str_lit("_cxxwalk.c _cxxwalk.exe _cxx.ir _cxx_err.txt _cxx_diag.txt _cxx_force.cpp _cxx_inst.ir"));
}

__attribute__((hot)) bool _is_expr_proxy_spec(TrStr s) {
    /* pass */
    __auto_type lt = ({ char* _t1281 = strstr(_tr_strz(s), _tr_strz(_tr_str_lit("<"))); _t1281 ? (long long)(_t1281 - (_tr_strz(s))) : -1LL; });
    /* pass */
    if ((lt < 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    TrStr inner = _tr_str_wrap(_tr_str_slice(_tr_strz(s), (lt + 1LL), _tr_strlen(_tr_strz(s))));
    /* pass */
    if ((({ char* _t1282 = strstr(_tr_strz(inner), _tr_strz(_tr_str_lit("<"))); _t1282 ? (long long)(_t1282 - (_tr_strz(inner))) : -1LL; }) < 0LL)) {
        /* pass */
        _tr_str_release(inner);
        return false;
    }
    /* pass */
    if (_tr_str_starts_with(_tr_strz(s), _tr_strz(_tr_str_lit("std::")))) {
        /* pass */
        _tr_str_release(inner);
        return false;
    }
    /* pass */
    _tr_str_release(inner);
    return true;
}

__attribute__((hot)) List_TrStr* _collect_specs(TrStr ir) {
    /* pass */
    List_TrStr* specs = (void*)List_TrStr_new();
    /* pass */
    TrMap* seen = _tr_dict_new(16LL);
    /* pass */
    List_TrStr* lines = _tr_str_split(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")));
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < lines->len)) {
        /* pass */
        TrStr ln = ({ TrStr _at_t1283 = (List_TrStr_get(lines, i)); __auto_type _wr = (_rstrip_cr(_at_t1283)); _tr_str_release(_at_t1283); _wr; });
        /* pass */
        i = (i + 1LL);
        /* pass */
        if (_tr_str_starts_with(_tr_strz(ln), _tr_strz(_tr_str_lit("TDSPEC ")))) {
            /* pass */
            TrStr td = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), 7LL, _tr_strlen(_tr_strz(ln))));
            /* pass */
            if ((((({ char* _t1284 = strstr(_tr_strz(td), _tr_strz(_tr_str_lit("<"))); _t1284 ? (long long)(_t1284 - (_tr_strz(td))) : -1LL; }) >= 0LL) && (!_is_expr_proxy_spec(td))) && (!_tr_dict_contains(seen, _tr_strz(td))))) {
                /* pass */
                _tr_dict_set(seen, _tr_strz(td), true);
                /* pass */
                List_TrStr_append(specs, td);
            }
            /* pass */
            continue;
        }
        /* pass */
        __auto_type p = ({ char* _t1285 = strstr(_tr_strz(ln), _tr_strz(_tr_str_lit("~r~"))); _t1285 ? (long long)(_t1285 - (_tr_strz(ln))) : -1LL; });
        /* pass */
        if ((p < 0LL)) {
            /* pass */
            continue;
        }
        /* pass */
        TrStr rest = _tr_str_wrap(_tr_str_slice(_tr_strz(ln), (p + 3LL), _tr_strlen(_tr_strz(ln))));
        /* pass */
        TrStr detail = _tr_str_retain(rest);
        /* pass */
        __auto_type bar = ({ char* _t1286 = strstr(_tr_strz(rest), _tr_strz(_tr_str_lit("|"))); _t1286 ? (long long)(_t1286 - (_tr_strz(rest))) : -1LL; });
        /* pass */
        if ((bar >= 0LL)) {
            /* pass */
            TrStr _strtmp_t1287 = _tr_str_wrap(_tr_str_slice(_tr_strz(rest), 0LL, bar));
            _tr_str_release(detail);
            detail = _strtmp_t1287;
        }
        /* pass */
        if ((((({ char* _t1288 = strstr(_tr_strz(detail), _tr_strz(_tr_str_lit("<"))); _t1288 ? (long long)(_t1288 - (_tr_strz(detail))) : -1LL; }) >= 0LL) && (!_is_expr_proxy_spec(detail))) && (!_tr_dict_contains(seen, _tr_strz(detail))))) {
            /* pass */
            _tr_dict_set(seen, _tr_strz(detail), true);
            /* pass */
            List_TrStr_append(specs, detail);
        }
        _tr_str_release(ln);
        _tr_str_release(rest);
        _tr_str_release(detail);
    }
    /* pass */
    Dict_free(seen);
    List_TrStr_free(lines);
    return specs;
}

__attribute__((hot)) void run_bindgen_cpp(TrStr header, TrStr out, TrStr cc, TrStr extra, TrStr pkglibs) {
    /* pass */
    TrStr guide = _detect_libclang(cc);
    /* pass */
    if ((strcmp(_tr_strz(guide), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        ({ printf("%s", _tr_strz(guide)); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    ({ TrStr _at_t1289 = (_cxxwalk_src()); write_file(_tr_str_lit("_cxxwalk.c"), _at_t1289); _tr_str_release(_at_t1289); });
    /* pass */
    long long rc = ({ TrStr _aet_t1290 = (_tr_strx_concat(_tr_strz(cc), _tr_strz(_tr_str_lit(" _cxxwalk.c -o _cxxwalk.exe -lclang 2>_cxx_err.txt")))); __auto_type _wr = (_tr_system(_aet_t1290.data)); _tr_str_release(_aet_t1290); _wr; });
    /* pass */
    if (((rc != 0LL) || (!file_exists(_tr_str_lit("_cxxwalk.exe"))))) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: could not compile the libclang walker (see _cxx_err.txt)"))); printf("\n"); });
        /* pass */
        _tr_str_release(guide);
        return;
    }
    /* pass */
    TrStr clang_args = ({ TrStr _cl = (({ TrStr _cr = (_cpp_std_flag(extra)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("-x c++ ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cr = (_cpp_detect_include_dirs(cc)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
    /* pass */
    if ((strcmp(_tr_strz(extra), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t1291 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(clang_args), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(extra)); _tr_str_release(_cl); _cres; });
        _tr_str_release(clang_args);
        clang_args = _strtmp_t1291;
    }
    /* pass */
    ({ TrStr _aet_t1292 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_local_exe(_tr_str_lit("_cxxwalk"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" \""))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(header)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\" "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(clang_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" > _cxx.ir 2> _cxx_diag.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t1292.data); _tr_str_release(_aet_t1292); });
    /* pass */
    TrStr diag = _tr_str_lit("");
    /* pass */
    if (file_exists(_tr_str_lit("_cxx_diag.txt"))) {
        /* pass */
        TrStr _strtmp_t1293 = read_file(_tr_str_lit("_cxx_diag.txt"));
        _tr_str_release(diag);
        diag = _strtmp_t1293;
    }
    /* pass */
    long long nfatal = _cpp_fatal_count(diag);
    /* pass */
    if (({ TrStr _at_t1294 = (read_file(_tr_str_lit("_cxx.ir"))); __auto_type _wr = (((!file_exists(_tr_str_lit("_cxx.ir"))) || _cpp_ir_is_empty(_at_t1294))); _tr_str_release(_at_t1294); _wr; })) {
        /* pass */
        ({ printf("%s", _tr_strz(_tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: no bindable declarations found in ")), _tr_strz(header)))); printf("\n"); });
        /* pass */
        if ((({ char* _t1295 = strstr(_tr_strz(diag), _tr_strz(_tr_str_lit("DIAG "))); _t1295 ? (long long)(_t1295 - (_tr_strz(diag))) : -1LL; }) >= 0LL)) {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("bindgen: libclang could not fully parse the header:"))); printf("\n"); });
            /* pass */
            _cpp_print_diag(diag);
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit(""))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  Most 'file not found' errors mean a missing include dir. Re-run with the"))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  library's include path(s) (and any required defines), e.g.:"))); printf("\n"); });
            /* pass */
            ({ printf("%s", _tr_strz(({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("    tauraroc bindgen ")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" -h cpp -I<include-dir> [-I<more>] [-D<MACRO>]"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        } else {
            /* pass */
            ({ printf("%s", _tr_strz(_tr_str_lit("  (the header parsed cleanly but declares no public classes/functions to bind.)"))); printf("\n"); });
        }
        /* pass */
        _cpp_cleanup();
        /* pass */
        _tr_str_release(guide);
        _tr_str_release(clang_args);
        _tr_str_release(diag);
        return;
    }
    /* pass */
    TrStr ir = read_file(_tr_str_lit("_cxx.ir"));
    /* pass */
    List_TrStr* specs = _collect_specs(ir);
    /* pass */
    long long n_specs = 0LL;
    /* pass */
    if ((specs->len > 0LL)) {
        /* pass */
        StringBuilder* fsb = StringBuilder_init(512LL);
        /* pass */
        ({ TrStr _sbt_t1296 = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("#include \"")), _tr_strz(header))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("\"\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(fsb, _sbt_t1296); _tr_str_release(_sbt_t1296); });
        /* pass */
        long long si = 0LL;
        /* pass */
        while ((si < specs->len)) {
            /* pass */
            ({ TrStr _sbt_t1297 = (({ TrStr _cl = (({ TrStr _cr = (List_TrStr_get(specs, si)); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("template class ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(";\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(fsb, _sbt_t1297); _tr_str_release(_sbt_t1297); });
            /* pass */
            si = (si + 1LL);
        }
        /* pass */
        ({ TrStr _at_t1298 = (StringObj_as_str(StringBuilder_to_string(fsb))); write_file(_tr_str_lit("_cxx_force.cpp"), _at_t1298); _tr_str_release(_at_t1298); });
        /* pass */
        ({ TrStr _aet_t1299 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_local_exe(_tr_str_lit("_cxxwalk"))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" --inst _cxx_force.cpp "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(clang_args)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" > _cxx_inst.ir 2>> _cxx_diag.txt"))); _tr_str_release(_cl); _cres; })); _tr_system(_aet_t1299.data); _tr_str_release(_aet_t1299); });
        /* pass */
        if (({ TrStr _at_t1300 = (read_file(_tr_str_lit("_cxx_inst.ir"))); __auto_type _wr = ((file_exists(_tr_str_lit("_cxx_inst.ir")) && (!_cpp_ir_is_empty(_at_t1300)))); _tr_str_release(_at_t1300); _wr; })) {
            /* pass */
            TrStr _strtmp_t1301 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(ir), _tr_strz(_tr_str_lit("\n")))); TrStr _cr = (read_file(_tr_str_lit("_cxx_inst.ir"))); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; });
            _tr_str_release(ir);
            ir = _strtmp_t1301;
            /* pass */
            n_specs = specs->len;
        }
        StringBuilder__tr_fn_free(fsb);
    }
    /* pass */
    TrStr shim_cflags = _cpp_std_flag(extra);
    /* pass */
    if ((strcmp(_tr_strz(extra), _tr_strz(_tr_str_lit(""))) != 0)) {
        /* pass */
        TrStr _strtmp_t1302 = ({ TrStr _cl = (_tr_strx_concat(_tr_strz(shim_cflags), _tr_strz(_tr_str_lit(" ")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(extra)); _tr_str_release(_cl); _cres; });
        _tr_str_release(shim_cflags);
        shim_cflags = _strtmp_t1302;
    }
    /* pass */
    _cpp_generate(ir, header, out, shim_cflags, pkglibs);
    /* pass */
    if ((n_specs > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(n_specs)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: instantiated ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" template specialization(s)"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
    }
    /* pass */
    if ((nfatal > 0LL)) {
        /* pass */
        ({ printf("%s", _tr_strz(({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(nfatal)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("bindgen: WARNING — libclang reported ")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" fatal error(s); bindings may be INCOMPLETE:"))); _tr_str_release(_cl); _cres; }))); printf("\n"); });
        /* pass */
        _cpp_print_diag(diag);
        /* pass */
        ({ printf("%s", _tr_strz(_tr_str_lit("  Add the missing include dir(s) with -I<dir> for a complete binding."))); printf("\n"); });
    }
    /* pass */
    _cpp_cleanup();
    _tr_str_release(guide);
    _tr_str_release(clang_args);
    _tr_str_release(diag);
    _tr_str_release(ir);
    List_TrStr_free(specs);
    _tr_str_release(shim_cflags);
}

