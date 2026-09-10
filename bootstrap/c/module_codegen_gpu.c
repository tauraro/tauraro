#include "tauraro_types.h"


__attribute__((malloc,returns_nonnull,hot)) GpuGenerator* GpuGenerator_init() {
    /* pass */
    GpuGenerator* g = ((GpuGenerator*)_tr_obj_alloc(sizeof(GpuGenerator)));
    /* pass */
    g->ok = true;
    /* pass */
    g->fail_note = _tr_str_lit("");
    /* pass */
    g->n_kernels = 0LL;
    /* pass */
    return g;
}

__attribute__((hot)) TrStr GpuGenerator_emit(GpuGenerator* self, HirProgram* prog, TrStr target) {
    /* pass */
    TrStr tgt = _tr_str_retain(target);
    /* pass */
    if ((strcmp(_tr_strz(tgt), _tr_strz(_tr_str_lit(""))) == 0)) {
        /* pass */
        TrStr _strtmp_t3538 = _tr_str_lit("spirv");
        _tr_str_release(tgt);
        tgt = _strtmp_t3538;
    }
    /* pass */
    StringBuilder* sb = StringBuilder_init(2048LL);
    /* pass */
    if ((strcmp(_tr_strz(tgt), _tr_strz(_tr_str_lit("nvptx"))) == 0)) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("; Tauraro GPU kernels -> NVPTX (CUDA PTX)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("target triple = \"nvptx64-nvidia-cuda\"\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("target datalayout = \"e-i64:64-i128:128-v16:16-v32:32-n16:32:64\"\n\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.tid.x()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.tid.y()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.tid.z()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.x()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.y()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ntid.z()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.x()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.y()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.ctaid.z()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare i32 @llvm.nvvm.read.ptx.sreg.nctaid.x()\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare void @llvm.nvvm.barrier0()\n\n"));
    } else {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("; Tauraro GPU kernels -> SPIR-V (OpenCL)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("target triple = \"spirv64-unknown-unknown\"\n\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z13get_global_idj(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z12get_local_idj(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z12get_group_idj(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z14get_local_sizej(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z15get_global_sizej(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func i64 @_Z14get_num_groupsj(i32)\n"));
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("declare spir_func void @_Z7barrierj(i32)\n\n"));
    }
    /* pass */
    List_ptr* dev_fns = (void*)List_ptr_new();
    /* pass */
    long long df = 0LL;
    /* pass */
    while ((df < prog->functions->len)) {
        /* pass */
        HirFunction* fd = ((HirFunction*)List_ptr_get(prog->functions, df));
        /* pass */
        if (fn_is_device(fd)) {
            /* pass */
            List_ptr_append(dev_fns, _tr_obj_retain(fd));
        }
        /* pass */
        df = (df + 1LL);
    }
    /* pass */
    long long de = 0LL;
    /* pass */
    while ((de < dev_fns->len)) {
        /* pass */
        GpuEmitter* e0 = GpuEmitter_init(tgt);
        /* pass */
        e0->dev_fns = dev_fns;
        /* pass */
        ({ TrStr _sbt_t3539 = (GpuEmitter_emit_device_fn(e0, ((HirFunction*)List_ptr_get(dev_fns, de)))); StringBuilder_append(sb, _sbt_t3539); _tr_str_release(_sbt_t3539); });
        /* pass */
        if ((!e0->ok)) {
            /* pass */
            self->ok = false;
            /* pass */
            if ((strcmp(_tr_strz(self->fail_note), _tr_strz(_tr_str_lit(""))) == 0)) {
                /* pass */
                self->fail_note = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("device fn '")), _tr_strz(((HirFunction*)List_ptr_get(dev_fns, de))->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e0->fail_note)); _tr_str_release(_cl); _cres; });
            }
        }
        /* pass */
        de = (de + 1LL);
        _tr_obj_release(e0, _trdrop_GpuEmitter);
    }
    /* pass */
    long long fi = 0LL;
    /* pass */
    while ((fi < prog->functions->len)) {
        /* pass */
        HirFunction* f = ((HirFunction*)List_ptr_get(prog->functions, fi));
        /* pass */
        if (fn_is_kernel(f)) {
            /* pass */
            self->n_kernels = (self->n_kernels + 1LL);
            /* pass */
            GpuEmitter* e = GpuEmitter_init(tgt);
            /* pass */
            e->dev_fns = dev_fns;
            /* pass */
            ({ TrStr _sbt_t3540 = (GpuEmitter_emit_kernel(e, f)); StringBuilder_append(sb, _sbt_t3540); _tr_str_release(_sbt_t3540); });
            /* pass */
            if ((!e->ok)) {
                /* pass */
                self->ok = false;
                /* pass */
                if ((strcmp(_tr_strz(self->fail_note), _tr_strz(_tr_str_lit(""))) == 0)) {
                    /* pass */
                    self->fail_note = ({ TrStr _cl = (({ TrStr _cl = (_tr_strx_concat(_tr_strz(_tr_str_lit("kernel '")), _tr_strz(f->name))); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit("': "))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(e->fail_note)); _tr_str_release(_cl); _cres; });
                }
            }
        }
        /* pass */
        fi = (fi + 1LL);
    }
    /* pass */
    if ((self->n_kernels == 0LL)) {
        /* pass */
        self->ok = false;
        /* pass */
        if ((strcmp(_tr_strz(self->fail_note), _tr_strz(_tr_str_lit(""))) == 0)) {
            /* pass */
            self->fail_note = _tr_str_lit("no @kernel functions found");
        }
    }
    /* pass */
    if (((strcmp(_tr_strz(tgt), _tr_strz(_tr_str_lit("nvptx"))) == 0) && (self->n_kernels > 0LL))) {
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("\n!nvvm.annotations = !{"));
        /* pass */
        long long fi2 = 0LL;
        /* pass */
        long long ki = 0LL;
        /* pass */
        while ((fi2 < prog->functions->len)) {
            /* pass */
            HirFunction* f2 = ((HirFunction*)List_ptr_get(prog->functions, fi2));
            /* pass */
            if (fn_is_kernel(f2)) {
                /* pass */
                if ((ki > 0LL)) {
                    /* pass */
                    StringBuilder_append(sb, _tr_str_lit(", "));
                }
                /* pass */
                ({ TrStr _sbt_t3541 = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("!")), _cr.data); _tr_str_release(_cr); _cres; })); StringBuilder_append(sb, _sbt_t3541); _tr_str_release(_sbt_t3541); });
                /* pass */
                ki = (ki + 1LL);
            }
            /* pass */
            fi2 = (fi2 + 1LL);
        }
        /* pass */
        StringBuilder_append(sb, _tr_str_lit("}\n"));
        /* pass */
        long long fi3 = 0LL;
        /* pass */
        long long ki2 = 0LL;
        /* pass */
        while ((fi3 < prog->functions->len)) {
            /* pass */
            HirFunction* f3 = ((HirFunction*)List_ptr_get(prog->functions, fi3));
            /* pass */
            if (fn_is_kernel(f3)) {
                /* pass */
                ({ TrStr _sbt_t3542 = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cl = (({ TrStr _cr = (_tr_str_wrap(_tr_int_to_str((long long)(ki2)))); TrStr _cres = _tr_strx_concat(_tr_strz(_tr_str_lit("!")), _cr.data); _tr_str_release(_cr); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(" = !{ptr @"))); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(f3->name)); _tr_str_release(_cl); _cres; })); TrStr _cres = _tr_strx_concat(_cl.data, _tr_strz(_tr_str_lit(", !\"kernel\", i32 1}\n"))); _tr_str_release(_cl); _cres; })); StringBuilder_append(sb, _sbt_t3542); _tr_str_release(_sbt_t3542); });
                /* pass */
                ki2 = (ki2 + 1LL);
            }
            /* pass */
            fi3 = (fi3 + 1LL);
        }
    }
    /* pass */
    _tr_str_release(tgt);
    return StringObj_as_str(StringBuilder_to_string(sb));
}

