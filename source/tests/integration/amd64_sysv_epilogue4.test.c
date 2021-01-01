#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/function.h"
#include "kefir/ir/module.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"
#include "kefir/codegen/amd64/system-v/abi.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    struct kefir_ir_module module;
    REQUIRE_OK(kefir_ir_module_alloc(mem, &module));
    struct kefir_ir_type *decl_params = kefir_ir_module_new_type(mem, &module, 0),
                       *decl_result = kefir_ir_module_new_type(mem, &module, 5);
    REQUIRE(decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *decl =
        kefir_ir_module_new_function_declaration(mem, &module, "func1", decl_params, decl_result);
    REQUIRE(decl != NULL, KEFIR_INTERNAL_ERROR);
    kefir_ir_function_t func;
    struct kefir_amd64_sysv_function sysv_func;
    REQUIRE_OK(kefir_codegen_amd64_sysv_init(&codegen, stdout, mem));
    codegen.asmgen.settings.enable_comments = false;
    codegen.asmgen.settings.enable_identation = false;

    REQUIRE_OK(kefir_ir_function_alloc(mem, decl, 0, &func));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_STRUCT, 0, 2));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_ARRAY, 0, 2));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_ARRAY, 0, 2));
    REQUIRE_OK(kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_INT32, 0, 0));

    REQUIRE_OK(kefir_amd64_sysv_function_alloc(mem, &func, &sysv_func));
    REQUIRE_OK(kefir_amd64_sysv_function_epilogue(&codegen, &sysv_func));
    REQUIRE_OK(kefir_amd64_sysv_function_free(mem, &sysv_func));

    REQUIRE_OK(KEFIR_CODEGEN_CLOSE(&codegen.iface));
    REQUIRE_OK(kefir_ir_function_free(mem, &func));
    REQUIRE_OK(kefir_ir_module_free(mem, &module));
    return KEFIR_OK;
}