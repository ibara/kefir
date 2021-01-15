#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/module.h"
#include "kefir/ir/function.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"
#include "kefir/codegen/amd64/system-v/abi.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    struct kefir_ir_module module;
    REQUIRE_OK(kefir_ir_module_alloc(mem, &module));
    struct kefir_ir_type *func1_decl_params = kefir_ir_module_new_type(mem, &module, 0, NULL),
                       *func1_decl_result = kefir_ir_module_new_type(mem, &module, 0, NULL);
    REQUIRE(func1_decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(func1_decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *func1_decl =
        kefir_ir_module_new_function_declaration(mem, &module, "func1", func1_decl_params, func1_decl_result);
    REQUIRE(func1_decl != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function *func1 = kefir_ir_module_new_function(mem, &module, func1_decl->identifier, NULL, 1024);
    REQUIRE(func1 != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, func1_decl->identifier));

    kefir_ir_module_id_t func2_id;
    struct kefir_ir_type *func2_decl_params = kefir_ir_module_new_type(mem, &module, 3, NULL),
                       *func2_decl_result = kefir_ir_module_new_type(mem, &module, 1, NULL);
    REQUIRE(func2_decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(func2_decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *func2_decl =
        kefir_ir_module_new_function_declaration(mem, &module,
            kefir_ir_module_symbol(mem, &module, "func2", &func2_id), func2_decl_params, func2_decl_result);
    REQUIRE(func2_decl != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE_OK(kefir_ir_module_declare_external(mem, &module, func2_decl->identifier));

    kefir_ir_module_id_t func3_id;
    struct kefir_ir_type *func3_decl_params = kefir_ir_module_new_type(mem, &module, 3, NULL),
                       *func3_decl_result = kefir_ir_module_new_type(mem, &module, 1, NULL);
    REQUIRE(func3_decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(func3_decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *func3_decl =
        kefir_ir_module_new_function_declaration(mem, &module,
            kefir_ir_module_symbol(mem, &module, "func3", &func3_id), func3_decl_params, func3_decl_result);
    REQUIRE(func3_decl != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE_OK(kefir_ir_module_declare_external(mem, &module, func3_decl->identifier));

    kefir_codegen_amd64_sysv_init(&codegen, stdout, mem);
    codegen.asmgen.settings.enable_comments = false;

    kefir_irblock_append(&func1->body, KEFIR_IROPCODE_INVOKE, func2_id);
    kefir_irblock_append(&func1->body, KEFIR_IROPCODE_INVOKE, func3_id);

    REQUIRE_OK(kefir_ir_type_append_v(func2_decl_params, KEFIR_IR_TYPE_INT, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func2_decl_params, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func2_decl_params, KEFIR_IR_TYPE_INT, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func2_decl_result, KEFIR_IR_TYPE_INT, 0, 0));

    REQUIRE_OK(kefir_ir_type_append_v(func3_decl_params, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func3_decl_params, KEFIR_IR_TYPE_BOOL, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func3_decl_params, KEFIR_IR_TYPE_FLOAT32, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(func3_decl_result, KEFIR_IR_TYPE_FLOAT64, 0, 0));

    REQUIRE_OK(KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &module));
    REQUIRE_OK(KEFIR_CODEGEN_CLOSE(&codegen.iface));
    REQUIRE_OK(kefir_ir_module_free(mem, &module));
    return KEFIR_OK;
}