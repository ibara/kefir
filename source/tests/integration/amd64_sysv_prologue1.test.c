#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/function.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"
#include "kefir/codegen/amd64/system-v/abi.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    struct kefir_ir_type decl_params, decl_result;
    struct kefir_ir_function_decl decl;
    struct kefir_ir_function func;
    struct kefir_amd64_sysv_function sysv_func;
    REQUIRE_OK(kefir_codegen_amd64_sysv_init(&codegen, stdout, mem));
    codegen.asmgen.settings.enable_comments = false;
    codegen.asmgen.settings.enable_identation = false;

    REQUIRE_OK(kefir_ir_type_alloc(mem, 7, &decl_params));
    REQUIRE_OK(kefir_ir_type_alloc(mem, 1, &decl_result));
    REQUIRE_OK(kefir_ir_function_decl_alloc(mem, "func1", &decl_params, &decl_result, &decl));
    REQUIRE_OK(kefir_ir_function_alloc(mem, &decl, 0, &func));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_STRUCT, 0, 3));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_ARRAY, 0, 2));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_INT, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_FLOAT64, 0, 0));

    REQUIRE_OK(kefir_amd64_sysv_function_alloc(mem, &func, &sysv_func));
    REQUIRE_OK(kefir_amd64_sysv_function_prologue(&codegen, &sysv_func));
    REQUIRE_OK(kefir_amd64_sysv_function_free(mem, &sysv_func));

    REQUIRE_OK(KEFIR_CODEGEN_CLOSE(&codegen.iface));
    REQUIRE_OK(kefir_ir_function_free(mem, &func));
    REQUIRE_OK(kefir_ir_function_decl_free(mem, &decl));
    REQUIRE_OK(kefir_ir_type_free(mem, &decl_result));
    REQUIRE_OK(kefir_ir_type_free(mem, &decl_params));
    return KEFIR_OK;
}