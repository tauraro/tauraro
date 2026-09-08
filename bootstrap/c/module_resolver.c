#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) ModuleResolver* ModuleResolver_init() {
    /* pass */
    ModuleResolver* r = ((ModuleResolver*)_tr_obj_alloc(sizeof(ModuleResolver)));
    /* pass */
    r->visited = _tr_dict_new(128LL);
    /* pass */
    r->all_decls = (void*)List_ptr_new();
    /* pass */
    r->search_paths = (void*)List_TrStr_new();
    /* pass */
    r->mod_dot_paths = (void*)List_TrStr_new();
    /* pass */
    r->mod_file_paths = (void*)List_TrStr_new();
    /* pass */
    r->all_decl_modules = (void*)List_TrStr_new();
    /* pass */
    r->current_mod = _tr_str_lit("");
    /* pass */
    r->parse_errors = 0LL;
    /* pass */
    List_TrStr_append(r->search_paths, _tr_str_lit("."));
    /* pass */
    List_TrStr_append(r->search_paths, _tr_str_lit("tauraro"));
    /* pass */
    List_TrStr_append(r->search_paths, _tr_str_lit(".."));
    /* pass */
    List_TrStr_append(r->search_paths, _tr_str_lit("../.."));
    /* pass */
    List_TrStr_append(r->search_paths, _tr_str_lit("std"));
    /* pass */
    return r;
}

__attribute__((hot)) void ModuleResolver_add_search_path(ModuleResolver* self, TrStr p) {
    /* pass */
    if ((strcmp(_tr_strz(p), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        return;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->search_paths->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(self->search_paths, i)), _tr_strz(p)) == 0)) {
            /* pass */
            return;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    List_TrStr_append(self->search_paths, p);
}

__attribute__((hot)) TrStr ModuleResolver_dir_of_path(ModuleResolver* self, TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long n = 0LL;
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long last = (0LL - 1LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            last = i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    if ((last < 0LL)) {
        /* pass */
        return _tr_str_lit("");
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(path), 0LL, last));
}

__attribute__((hot)) TrStr ModuleResolver_base_of_path(ModuleResolver* self, TrStr path) {
    /* pass */
    char* p = ((char*)(_tr_strz(path)));
    /* pass */
    long long n = 0LL;
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    long long last = (0LL - 1LL);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < n)) {
        /* pass */
        long long c = ((long long)((*(p + i))));
        /* pass */
        if (((c == 47LL) || (c == 92LL))) {
            /* pass */
            last = i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return _tr_str_wrap(_tr_str_slice(_tr_strz(path), (last + 1LL), n));
}

__attribute__((hot)) Program* ModuleResolver_resolve_main(ModuleResolver* self, TrStr main_path) {
    /* pass */
    long long dir_end = 0LL;
    /* pass */
    long long ci = 0LL;
    /* pass */
    char* mp = ((char*)(_tr_strz(main_path)));
    /* pass */
    while ((((long long)((*(mp + ci)))) != 0LL)) {
        /* pass */
        long long ch = ((long long)((*(mp + ci))));
        /* pass */
        if (((ch == 47LL) || (ch == 92LL))) {
            /* pass */
            dir_end = ci;
        }
        /* pass */
        ci = (ci + 1LL);
    }
    /* pass */
    if ((dir_end > 0LL)) {
        /* pass */
        TrStr dir_str = _tr_str_lit("");
        /* pass */
        /* unsafe block */
        /* pass */
        TrStr _strtmp_t236 = _tr_str_wrap(_tr_str_slice(_tr_strz(main_path), 0LL, dir_end));
        _tr_str_release(dir_str);
        dir_str = _strtmp_t236;
        /* pass */
        List_TrStr_append(self->search_paths, dir_str);
        /* pass */
        long long par_end = 0LL;
        /* pass */
        long long pj = 0LL;
        /* pass */
        char* dp = ((char*)(_tr_strz(dir_str)));
        /* pass */
        while ((((long long)((*(dp + pj)))) != 0LL)) {
            /* pass */
            long long dch = ((long long)((*(dp + pj))));
            /* pass */
            if (((dch == 47LL) || (dch == 92LL))) {
                /* pass */
                par_end = pj;
            }
            /* pass */
            pj = (pj + 1LL);
        }
        /* pass */
        TrStr parent_dir = _tr_str_lit(".");
        /* pass */
        if ((par_end > 0LL)) {
            /* pass */
            /* unsafe block */
            /* pass */
            TrStr _strtmp_t237 = _tr_str_wrap(_tr_str_slice(_tr_strz(dir_str), 0LL, par_end));
            _tr_str_release(parent_dir);
            parent_dir = _strtmp_t237;
            /* pass */
            List_TrStr_append(self->search_paths, parent_dir);
            /* pass */
            long long gp_end = 0LL;
            /* pass */
            long long gj = 0LL;
            /* pass */
            char* gp_p = ((char*)(_tr_strz(parent_dir)));
            /* pass */
            while ((((long long)((*(gp_p + gj)))) != 0LL)) {
                /* pass */
                long long gch = ((long long)((*(gp_p + gj))));
                /* pass */
                if (((gch == 47LL) || (gch == 92LL))) {
                    /* pass */
                    gp_end = gj;
                }
                /* pass */
                gj = (gj + 1LL);
            }
            /* pass */
            if ((gp_end > 0LL)) {
                /* pass */
                TrStr gp_dir = _tr_str_lit("");
                /* pass */
                /* unsafe block */
                /* pass */
                TrStr _strtmp_t238 = _tr_str_wrap(_tr_str_slice(_tr_strz(parent_dir), 0LL, gp_end));
                _tr_str_release(gp_dir);
                gp_dir = _strtmp_t238;
                /* pass */
                List_TrStr_append(self->search_paths, gp_dir);
                _tr_str_release(gp_dir);
            }
        }
        /* pass */
        void* vh = _tr_opendir(_tr_strz(parent_dir));
        /* pass */
        if ((((unsigned long long)(vh)) != ((unsigned long long)(0LL)))) {
            /* pass */
            TrStr vname = _tr_str_wrap(_tr_readdir(vh));
            /* pass */
            while ((strcmp(_tr_strz(vname), _tr_strz(_tr_str_lit(""))) != 0)) {
                /* pass */
                if (((strcmp(_tr_strz(vname), _tr_strz(_tr_str_lit("."))) != 0) && (strcmp(_tr_strz(vname), _tr_strz(_tr_str_lit(".."))) != 0))) {
                    /* pass */
                    TrStr vpkg_src = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(parent_dir), _tr_strz(_tr_str_lit("/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(vname)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/src"))); _tr_str_release(_cl); _cres; });
                    /* pass */
                    if (_tr_dir_exists(_tr_strz(vpkg_src))) {
                        /* pass */
                        List_TrStr_append(self->search_paths, vpkg_src);
                    }
                }
                /* pass */
                TrStr _strtmp_t239 = _tr_str_wrap(_tr_readdir(vh));
                _tr_str_release(vname);
                vname = _strtmp_t239;
            }
            /* pass */
            _tr_closedir(vh);
            _tr_str_release(vname);
        }
    }
    /* pass */
    ModuleResolver_resolve_file(self, main_path, true);
    /* pass */
    Program* p = Program_init();
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < self->all_decls->len)) {
        /* pass */
        Program_push(p, ((Decl*)List_ptr_get(self->all_decls, i)));
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return p;
}

__attribute__((hot)) void ModuleResolver_resolve_file(ModuleResolver* self, TrStr path, bool is_root) {
    /* pass */
    if (_tr_dict_contains(self->visited, _tr_strz(path))) {
        /* pass */
        return;
    }
    /* pass */
    _tr_dict_set(self->visited, _tr_strz(path), true);
    /* pass */
    TrStr source = read_file(path);
    /* pass */
    if ((strcmp(_tr_strz(source), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        char* p_ptr = ((char*)(_tr_strz(path)));
        /* pass */
        long long slen = 0LL;
        /* pass */
        while ((((long long)((*(p_ptr + slen)))) != 0LL)) {
            /* pass */
            slen = (slen + 1LL);
        }
        /* pass */
        bool has_tr = false;
        /* pass */
        if ((slen >= 3LL)) {
            /* pass */
            if ((((long long)((*(p_ptr + (slen - 3LL))))) == 46LL)) {
                /* pass */
                if ((((long long)((*(p_ptr + (slen - 2LL))))) == 116LL)) {
                    /* pass */
                    if ((((long long)((*(p_ptr + (slen - 1LL))))) == 114LL)) {
                        /* pass */
                        has_tr = true;
                    }
                }
            }
        }
        /* pass */
        if ((!has_tr)) {
            /* pass */
            TrStr _strtmp_t241 = ({ TrStr _at_t240 = (_tr_strx_concat(_tr_strz(path), _tr_strz(_tr_str_lit(".tr")))); __auto_type _wr = (read_file(_at_t240)); _tr_str_release(_at_t240); _wr; });
            _tr_str_release(source);
            source = _strtmp_t241;
        }
        /* pass */
        if ((strcmp(_tr_strz(source), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            _tr_str_release(source);
            return;
        }
    }
    /* pass */
    bool file_trusted = (ModuleResolver__path_is_lib(self, path) || ModuleResolver__source_is_trusted(self, source));
    /* pass */
    Lexer* lexer = Lexer_init(source);
    /* pass */
    List_Token* tokens = Lexer_tokenize(lexer);
    /* pass */
    Parser* parser = Parser_init(tokens, lexer->token_lines);
    /* pass */
    parser->current_file = _tr_str_retain(path);
    /* pass */
    parser->cols = lexer->token_cols;
    /* pass */
    parser->src_text = _tr_str_retain(source);
    /* pass */
    Program* prog = Parser_parse_program(parser);
    /* pass */
    self->parse_errors = (self->parse_errors + parser->error_count);
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < Program_len(prog))) {
        /* pass */
        Decl* decl_ptr = Program_get(prog, i);
        /* pass */
        __auto_type _t242 = (*decl_ptr);
        if (_t242.tag == Decl_DImport) {
            __auto_type mod_path = _t242.data.DImport.path;
__auto_type alias = _t242.data.DImport.alias;
            /* pass */
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (_t242.tag == Decl_DFromImport) {
            __auto_type mod_path = _t242.data.DFromImport.path;
__auto_type items = _t242.data.DFromImport.items;
            /* pass */
            ModuleResolver_resolve_module_path(self, mod_path);
        } else if (1) {
            __auto_type _ = _t242;
            /* pass */
            if (file_trusted) {
                /* pass */
                ModuleResolver__mark_decl_lib(self, decl_ptr);
            }
            /* pass */
            List_ptr_append(self->all_decls, decl_ptr);
            /* pass */
            List_TrStr_append(self->all_decl_modules, self->current_mod);
        }
        /* pass */
        i = (i + 1LL);
    }
    _tr_str_release(source);
    _tr_obj_release(lexer, _trdrop_Lexer);
    _tr_obj_release(parser, _trdrop_Parser);
    _tr_obj_release(prog, _trdrop_Program);
}

__attribute__((hot)) bool ModuleResolver__path_is_lib(ModuleResolver* self, TrStr path) {
    /* pass */
    return (((ModuleResolver__contains(self, path, _tr_str_lit("std/")) || ModuleResolver__contains(self, path, _tr_str_lit("std\\"))) || ModuleResolver__contains(self, path, _tr_str_lit("core/"))) || ModuleResolver__contains(self, path, _tr_str_lit("core\\")));
}

__attribute__((hot)) bool ModuleResolver__source_is_trusted(ModuleResolver* self, TrStr source) {
    /* pass */
    long long n = _tr_strlen(_tr_strz(source));
    /* pass */
    long long lim = 400LL;
    /* pass */
    if ((n < lim)) {
        /* pass */
        lim = n;
    }
    /* pass */
    if ((lim <= 0LL)) {
        /* pass */
        return false;
    }
    /* pass */
    return ({ TrStr _at_t243 = (_tr_str_wrap(_tr_str_slice(_tr_strz(source), 0LL, lim))); __auto_type _wr = (ModuleResolver__contains(self, _at_t243, _tr_str_lit("@trusted"))); _tr_str_release(_at_t243); _wr; });
}

__attribute__((hot)) bool ModuleResolver__contains(ModuleResolver* self, TrStr hay, TrStr needle) {
    /* pass */
    long long hl = _tr_strlen(_tr_strz(hay));
    /* pass */
    long long nl = _tr_strlen(_tr_strz(needle));
    /* pass */
    if (((nl == 0LL) || (nl > hl))) {
        /* pass */
        return false;
    }
    /* pass */
    long long i = 0LL;
    /* pass */
    while (((i + nl) <= hl)) {
        /* pass */
        if (({ TrStr _wt_t244 = (_tr_str_wrap(_tr_str_slice(_tr_strz(hay), i, (i + nl)))); __auto_type _wr = ((strcmp(_wt_t244.data, _tr_strz(needle)) == 0)); _tr_str_release(_wt_t244); _wr; })) {
            /* pass */
            return true;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return false;
}

__attribute__((hot)) void ModuleResolver__mark_decl_lib(ModuleResolver* self, Decl* dp) {
    /* pass */
    if ((((unsigned long long)(dp)) == ((unsigned long long)(0LL)))) {
        /* pass */
        return;
    }
    /* pass */
    __auto_type _t245 = (*dp);
    if (_t245.tag == Decl_DFunction) {
        __auto_type f = _t245.data.DFunction.func;
        f->is_lib = true;
    } else if (_t245.tag == Decl_DClass) {
        __auto_type c = _t245.data.DClass.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t245.tag == Decl_DActor) {
        __auto_type c = _t245.data.DActor.cls;
        ModuleResolver__mark_methods_lib(self, c);
    } else if (_t245.tag == Decl_DExtend) {
        __auto_type ms = _t245.data.DExtend.methods;
        /* pass */
        long long i = 0LL;
        /* pass */
        while ((i < ms->len)) {
            /* pass */
            ((FunctionDef*)List_ptr_get(ms, i))->is_lib = true;
            /* pass */
            i = (i + 1LL);
        }
    } else if (1) {
        __auto_type _ = _t245;
        /* pass */
    }
}

__attribute__((hot)) void ModuleResolver__mark_methods_lib(ModuleResolver* self, ClassDef* c) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < c->methods->len)) {
        /* pass */
        ((FunctionDef*)List_ptr_get(c->methods, i))->is_lib = true;
        /* pass */
        i = (i + 1LL);
    }
}

__attribute__((hot)) void ModuleResolver_resolve_recursive(ModuleResolver* self, TrStr path) {
    /* pass */
    ModuleResolver_resolve_file(self, path, false);
}

__attribute__((hot)) void ModuleResolver_resolve_module_path(ModuleResolver* self, TrStr mod_path) {
    /* pass */
    long long n = 0LL;
    /* pass */
    char* p = ((char*)(_tr_strz(mod_path)));
    /* pass */
    while ((((long long)((*(p + n)))) != 0LL)) {
        /* pass */
        n = (n + 1LL);
    }
    /* pass */
    TrStr dir_path = _tr_str_wrap(_tr_str_slice(_tr_strz(mod_path), 0LL, n));
    /* pass */
    char* bp = ((char*)(_tr_strz(dir_path)));
    /* pass */
    long long j = 0LL;
    /* pass */
    while ((j < n)) {
        /* pass */
        if ((((long long)((*(bp + j)))) == 46LL)) {
            /* pass */
            /* unsafe block */
            /* pass */
            (*(bp + j) = ((char)(47LL)));
        }
        /* pass */
        j = (j + 1LL);
    }
    /* pass */
    TrStr saved_mod = _tr_str_retain(self->current_mod);
    /* pass */
    self->current_mod = _tr_str_retain(mod_path);
    /* pass */
    long long sidx = 0LL;
    /* pass */
    while ((sidx < self->search_paths->len)) {
        /* pass */
        TrStr base = List_TrStr_get(self->search_paths, sidx);
        /* pass */
        TrStr pkg_path = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dir_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/mod.tr"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if (file_exists(pkg_path)) {
            /* pass */
            bool already = _tr_dict_contains(self->visited, _tr_strz(pkg_path));
            /* pass */
            ModuleResolver_resolve_file(self, pkg_path, false);
            /* pass */
            if ((!already)) {
                /* pass */
                List_TrStr_append(self->mod_dot_paths, mod_path);
                /* pass */
                List_TrStr_append(self->mod_file_paths, pkg_path);
            }
            /* pass */
            self->current_mod = _tr_str_retain(saved_mod);
            /* pass */
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            return;
        }
        /* pass */
        TrStr flat_path = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dir_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".tr"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if (file_exists(flat_path)) {
            /* pass */
            bool already = _tr_dict_contains(self->visited, _tr_strz(flat_path));
            /* pass */
            ModuleResolver_resolve_file(self, flat_path, false);
            /* pass */
            if ((!already)) {
                /* pass */
                List_TrStr_append(self->mod_dot_paths, mod_path);
                /* pass */
                List_TrStr_append(self->mod_file_paths, flat_path);
            }
            /* pass */
            self->current_mod = _tr_str_retain(saved_mod);
            /* pass */
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            return;
        }
        /* pass */
        TrStr pkg_src_path = ({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dir_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/src/"))); _tr_str_release(_cl); _cres; })); TrStr _cr = (ModuleResolver_base_of_path(self, dir_path)); TrStr _cres = _tr_strx_concat(_cl.data, _cr.data); _tr_str_release(_cl); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(".tr"))); _tr_str_release(_cl); _cres; });
        /* pass */
        if (file_exists(pkg_src_path)) {
            /* pass */
            ({ TrStr _at_t246 = (({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(base), _tr_strz(_tr_str_lit("/")))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(dir_path)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("/src"))); _tr_str_release(_cl); _cres; })); ModuleResolver_add_search_path(self, _at_t246); _tr_str_release(_at_t246); });
            /* pass */
            bool already3 = _tr_dict_contains(self->visited, _tr_strz(pkg_src_path));
            /* pass */
            ModuleResolver_resolve_file(self, pkg_src_path, false);
            /* pass */
            if ((!already3)) {
                /* pass */
                List_TrStr_append(self->mod_dot_paths, mod_path);
                /* pass */
                List_TrStr_append(self->mod_file_paths, pkg_src_path);
            }
            /* pass */
            self->current_mod = _tr_str_retain(saved_mod);
            /* pass */
            _tr_str_release(dir_path);
            _tr_str_release(saved_mod);
            _tr_str_release(base);
            _tr_str_release(pkg_path);
            _tr_str_release(flat_path);
            _tr_str_release(pkg_src_path);
            return;
        }
        /* pass */
        sidx = (sidx + 1LL);
        _tr_str_release(base);
        _tr_str_release(pkg_path);
        _tr_str_release(flat_path);
        _tr_str_release(pkg_src_path);
    }
    /* pass */
    self->current_mod = _tr_str_retain(saved_mod);
    _tr_str_release(dir_path);
    _tr_str_release(saved_mod);
}

__attribute__((hot)) bool decl_is_pub(Decl d) {
    /* pass */
    __auto_type _t247 = d;
    if (_t247.tag == Decl_DFunction) {
        __auto_type f = _t247.data.DFunction.func;
        /* pass */
        return f->is_public;
    } else if (_t247.tag == Decl_DClass) {
        __auto_type c = _t247.data.DClass.cls;
        /* pass */
        return c->is_public;
    } else if (_t247.tag == Decl_DEnum) {
        __auto_type e = _t247.data.DEnum.enm;
        /* pass */
        return e->is_public;
    } else if (_t247.tag == Decl_DInterface) {
        __auto_type i = _t247.data.DInterface.iface;
        /* pass */
        return i->is_public;
    } else if (_t247.tag == Decl_DActor) {
        __auto_type c = _t247.data.DActor.cls;
        /* pass */
        return c->is_public;
    } else if (_t247.tag == Decl_DExtern) {
        __auto_type abi = _t247.data.DExtern.abi;
__auto_type funcs = _t247.data.DExtern.functions;
        /* pass */
        return true;
    } else if (_t247.tag == Decl_DExtend) {
        __auto_type target = _t247.data.DExtend.target;
__auto_type methods = _t247.data.DExtend.methods;
        /* pass */
        return true;
    } else if (_t247.tag == Decl_DDecoratorDef) {
        __auto_type f = _t247.data.DDecoratorDef.func;
        /* pass */
        return f->is_public;
    } else if (1) {
        __auto_type _ = _t247;
        /* pass */
        return false;
    }
}

