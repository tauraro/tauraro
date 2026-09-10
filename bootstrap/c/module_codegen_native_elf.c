#include "tauraro_types.h"

long long _index_of(List_TrStr* names, TrStr s);
void _shdr(ByteBuf* out, long long name, long long ty, long long flags, long long addr, long long offset, long long size, long long link, long long info, long long align, long long entsize);
long long _align8(long long n);

__attribute__((hot)) long long _index_of(List_TrStr* names, TrStr s) {
    /* pass */
    long long i = 0LL;
    /* pass */
    while ((i < names->len)) {
        /* pass */
        if ((strcmp(_tr_strz(List_TrStr_get(names, i)), _tr_strz(s)) == 0)) {
            /* pass */
            return i;
        }
        /* pass */
        i = (i + 1LL);
    }
    /* pass */
    return (-1LL);
}

__attribute__((hot)) void _shdr(ByteBuf* out, long long name, long long ty, long long flags, long long addr, long long offset, long long size, long long link, long long info, long long align, long long entsize) {
    /* pass */
    ByteBuf_u32(out, name);
    /* pass */
    ByteBuf_u32(out, ty);
    /* pass */
    ByteBuf_u64(out, flags);
    /* pass */
    ByteBuf_u64(out, addr);
    /* pass */
    ByteBuf_u64(out, offset);
    /* pass */
    ByteBuf_u64(out, size);
    /* pass */
    ByteBuf_u32(out, link);
    /* pass */
    ByteBuf_u32(out, info);
    /* pass */
    ByteBuf_u64(out, align);
    /* pass */
    ByteBuf_u64(out, entsize);
}

__attribute__((hot)) long long _align8(long long n) {
    /* pass */
    long long r = n;
    /* pass */
    while (((r % 8LL) != 0LL)) {
        /* pass */
        r = (r + 1LL);
    }
    /* pass */
    return r;
}

__attribute__((hot)) bool write_elf_object(TrStr out_path, List_ptr* funcs, List_TrStr* externs, List_TrStr* strings, long long n_globals) {
    /* pass */
    ByteBuf* text = ByteBuf_init();
    /* pass */
    List_ptr* trelocs = (void*)List_ptr_new();
    /* pass */
    List_i64* func_base = (void*)List_i64_new();
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < funcs->len)) {
        /* pass */
        EncodedFunc* f = ((EncodedFunc*)List_ptr_get(funcs, fi));
        /* pass */
        long long base = text->len;
        /* pass */
        List_i64_append(func_base, base);
        /* pass */
        ByteBuf_append_buf(text, f->code);
        /* pass */
        long long ri = 0LL;
        /* pass */
        while ((ri < f->relocs->len)) {
            /* pass */
            Reloc* r = ((Reloc*)List_ptr_get(f->relocs, ri));
            /* pass */
            TextReloc* tr = ((TextReloc*)_tr_obj_alloc(sizeof(TextReloc)));
            /* pass */
            tr->roff = (base + r->offset);
            /* pass */
            tr->sym = _tr_str_retain(r->symbol);
            /* pass */
            tr->kind = r->kind;
            /* pass */
            tr->str_idx = r->str_idx;
            /* pass */
            List_ptr_append(trelocs, _tr_obj_retain(tr));
            /* pass */
            ri = (ri + 1LL);
            _tr_obj_release(tr, _trdrop_TextReloc);
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    ByteBuf* rodata = ByteBuf_init();
    /* pass */
    List_i64* str_off = (void*)List_i64_new();
    /* pass */
    long long sxi = 0LL;
    /* pass */
    while ((sxi < strings->len)) {
        /* pass */
        List_i64_append(str_off, rodata->len);
        /* pass */
        ({ TrStr _at_t3434 = (List_TrStr_get(strings, sxi)); ByteBuf_cstr(rodata, _at_t3434); _tr_str_release(_at_t3434); });
        /* pass */
        sxi = (sxi + 1LL);
    }
    /* pass */
    ByteBuf* symtab = ByteBuf_init();
    /* pass */
    ByteBuf* strtab = ByteBuf_init();
    /* pass */
    List_TrStr* sym_names = (void*)List_TrStr_new();
    /* pass */
    ByteBuf_zeros(symtab, 24LL);
    /* pass */
    ByteBuf_u8(strtab, 0LL);
    /* pass */
    ByteBuf_u32(symtab, 0LL);
    /* pass */
    ByteBuf_u8(symtab, 3LL);
    /* pass */
    ByteBuf_u8(symtab, 0LL);
    /* pass */
    ByteBuf_u16(symtab, 2LL);
    /* pass */
    ByteBuf_u64(symtab, 0LL);
    /* pass */
    ByteBuf_u64(symtab, 0LL);
    /* pass */
    ByteBuf_u32(symtab, 0LL);
    /* pass */
    ByteBuf_u8(symtab, 3LL);
    /* pass */
    ByteBuf_u8(symtab, 0LL);
    /* pass */
    ByteBuf_u16(symtab, 3LL);
    /* pass */
    ByteBuf_u64(symtab, 0LL);
    /* pass */
    ByteBuf_u64(symtab, 0LL);
    /* pass */
    fi = 0LL;
    /* pass */
    while ((fi < funcs->len)) {
        /* pass */
        EncodedFunc* f2 = ((EncodedFunc*)List_ptr_get(funcs, fi));
        /* pass */
        long long noff = strtab->len;
        /* pass */
        ByteBuf_cstr(strtab, f2->name);
        /* pass */
        ByteBuf_u32(symtab, noff);
        /* pass */
        ByteBuf_u8(symtab, 18LL);
        /* pass */
        ByteBuf_u8(symtab, 0LL);
        /* pass */
        ByteBuf_u16(symtab, 1LL);
        /* pass */
        ByteBuf_u64(symtab, List_i64_get(func_base, fi));
        /* pass */
        ByteBuf_u64(symtab, f2->code->len);
        /* pass */
        List_TrStr_append(sym_names, f2->name);
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    long long ei = 0LL;
    /* pass */
    while ((ei < externs->len)) {
        /* pass */
        TrStr ex = List_TrStr_get(externs, ei);
        /* pass */
        long long noff2 = strtab->len;
        /* pass */
        ByteBuf_cstr(strtab, ex);
        /* pass */
        ByteBuf_u32(symtab, noff2);
        /* pass */
        ByteBuf_u8(symtab, 16LL);
        /* pass */
        ByteBuf_u8(symtab, 0LL);
        /* pass */
        ByteBuf_u16(symtab, 0LL);
        /* pass */
        ByteBuf_u64(symtab, 0LL);
        /* pass */
        ByteBuf_u64(symtab, 0LL);
        /* pass */
        List_TrStr_append(sym_names, ex);
        /* pass */
        ei = (ei + 1LL);
        _tr_str_release(ex);
    }
    /* pass */
    ByteBuf* rela = ByteBuf_init();
    /* pass */
    long long tri = 0LL;
    /* pass */
    while ((tri < trelocs->len)) {
        /* pass */
        TextReloc* trr = ((TextReloc*)List_ptr_get(trelocs, tri));
        /* pass */
        if ((trr->kind == 1LL)) {
            /* pass */
            ByteBuf_u64(rela, trr->roff);
            /* pass */
            ByteBuf_u64(rela, ((1LL * 4294967296LL) + 2LL));
            /* pass */
            ByteBuf_u64(rela, (List_i64_get(str_off, trr->str_idx) - 4LL));
        } else if ((trr->kind == 3LL)) {
            /* pass */
            ByteBuf_u64(rela, trr->roff);
            /* pass */
            ByteBuf_u64(rela, ((2LL * 4294967296LL) + 2LL));
            /* pass */
            ByteBuf_u64(rela, ((trr->str_idx * 8LL) - 4LL));
        } else if ((trr->kind == 4LL)) {
            /* pass */
            long long fsidx = (_index_of(sym_names, trr->sym) + 3LL);
            /* pass */
            ByteBuf_u64(rela, trr->roff);
            /* pass */
            ByteBuf_u64(rela, ((fsidx * 4294967296LL) + 2LL));
            /* pass */
            ByteBuf_u64(rela, (0LL - 4LL));
        } else {
            /* pass */
            long long sidx = (_index_of(sym_names, trr->sym) + 3LL);
            /* pass */
            ByteBuf_u64(rela, trr->roff);
            /* pass */
            ByteBuf_u64(rela, ((sidx * 4294967296LL) + 4LL));
            /* pass */
            ByteBuf_u64(rela, (0LL - 4LL));
        }
        /* pass */
        tri = (tri + 1LL);
    }
    /* pass */
    ByteBuf* shstr = ByteBuf_init();
    /* pass */
    ByteBuf_u8(shstr, 0LL);
    /* pass */
    long long n_text = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".text"));
    /* pass */
    long long n_rodata = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".rodata"));
    /* pass */
    long long n_bss = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".bss"));
    /* pass */
    long long n_rela = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".rela.text"));
    /* pass */
    long long n_symtab = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".symtab"));
    /* pass */
    long long n_strtab = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".strtab"));
    /* pass */
    long long n_shstr = shstr->len;
    /* pass */
    ByteBuf_cstr(shstr, _tr_str_lit(".shstrtab"));
    /* pass */
    long long text_off = 64LL;
    /* pass */
    long long off = _align8((text_off + text->len));
    /* pass */
    long long rodata_off = off;
    /* pass */
    off = _align8((off + rodata->len));
    /* pass */
    long long bss_off = off;
    /* pass */
    long long rela_off = off;
    /* pass */
    off = (off + rela->len);
    /* pass */
    long long symtab_off = off;
    /* pass */
    off = (off + symtab->len);
    /* pass */
    long long strtab_off = off;
    /* pass */
    off = (off + strtab->len);
    /* pass */
    long long shstr_off = off;
    /* pass */
    off = _align8((off + shstr->len));
    /* pass */
    long long shoff = off;
    /* pass */
    ByteBuf* out = ByteBuf_init();
    /* pass */
    ByteBuf_u8(out, 127LL);
    /* pass */
    ByteBuf_u8(out, 69LL);
    /* pass */
    ByteBuf_u8(out, 76LL);
    /* pass */
    ByteBuf_u8(out, 70LL);
    /* pass */
    ByteBuf_u8(out, 2LL);
    /* pass */
    ByteBuf_u8(out, 1LL);
    /* pass */
    ByteBuf_u8(out, 1LL);
    /* pass */
    ByteBuf_u8(out, 0LL);
    /* pass */
    ByteBuf_zeros(out, 8LL);
    /* pass */
    ByteBuf_u16(out, 1LL);
    /* pass */
    ByteBuf_u16(out, 62LL);
    /* pass */
    ByteBuf_u32(out, 1LL);
    /* pass */
    ByteBuf_u64(out, 0LL);
    /* pass */
    ByteBuf_u64(out, 0LL);
    /* pass */
    ByteBuf_u64(out, shoff);
    /* pass */
    ByteBuf_u32(out, 0LL);
    /* pass */
    ByteBuf_u16(out, 64LL);
    /* pass */
    ByteBuf_u16(out, 0LL);
    /* pass */
    ByteBuf_u16(out, 0LL);
    /* pass */
    ByteBuf_u16(out, 64LL);
    /* pass */
    ByteBuf_u16(out, 8LL);
    /* pass */
    ByteBuf_u16(out, 7LL);
    /* pass */
    ByteBuf_append_buf(out, text);
    /* pass */
    ByteBuf_align_to(out, 8LL);
    /* pass */
    ByteBuf_append_buf(out, rodata);
    /* pass */
    ByteBuf_align_to(out, 8LL);
    /* pass */
    ByteBuf_append_buf(out, rela);
    /* pass */
    ByteBuf_append_buf(out, symtab);
    /* pass */
    ByteBuf_append_buf(out, strtab);
    /* pass */
    ByteBuf_append_buf(out, shstr);
    /* pass */
    ByteBuf_align_to(out, 8LL);
    /* pass */
    long long bss_size = (n_globals * 8LL);
    /* pass */
    _shdr(out, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL, 0LL);
    /* pass */
    _shdr(out, n_text, 1LL, 6LL, 0LL, text_off, text->len, 0LL, 0LL, 16LL, 0LL);
    /* pass */
    _shdr(out, n_rodata, 1LL, 2LL, 0LL, rodata_off, rodata->len, 0LL, 0LL, 1LL, 0LL);
    /* pass */
    _shdr(out, n_bss, 8LL, 3LL, 0LL, bss_off, bss_size, 0LL, 0LL, 8LL, 0LL);
    /* pass */
    _shdr(out, n_rela, 4LL, 64LL, 0LL, rela_off, rela->len, 5LL, 1LL, 8LL, 24LL);
    /* pass */
    _shdr(out, n_symtab, 2LL, 0LL, 0LL, symtab_off, symtab->len, 6LL, 3LL, 8LL, 24LL);
    /* pass */
    _shdr(out, n_strtab, 3LL, 0LL, 0LL, strtab_off, strtab->len, 0LL, 0LL, 1LL, 0LL);
    /* pass */
    _shdr(out, n_shstr, 3LL, 0LL, 0LL, shstr_off, shstr->len, 0LL, 0LL, 1LL, 0LL);
    /* pass */
    _tr_obj_release(text, _trdrop_ByteBuf);
    List_ptr_free_obj(trelocs, _trdrop_TextReloc);
    List_i64_free(func_base);
    _tr_obj_release(rodata, _trdrop_ByteBuf);
    List_i64_free(str_off);
    _tr_obj_release(symtab, _trdrop_ByteBuf);
    _tr_obj_release(strtab, _trdrop_ByteBuf);
    _tr_obj_release(rela, _trdrop_ByteBuf);
    _tr_obj_release(shstr, _trdrop_ByteBuf);
    _tr_obj_release(out, _trdrop_ByteBuf);
    return ByteBuf_write_file(out, out_path);
}

