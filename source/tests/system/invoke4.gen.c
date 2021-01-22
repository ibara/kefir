#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/function.h"
#include "kefir/ir/module.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    struct kefir_ir_module module;
    REQUIRE_OK(kefir_ir_module_alloc(mem, &module));
    
    struct kefir_ir_type *proxyadd_decl_params = kefir_ir_module_new_type(mem, &module, 3, NULL),
                       *proxyadd_decl_result = kefir_ir_module_new_type(mem, &module, 3, NULL);
    REQUIRE(proxyadd_decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(proxyadd_decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *proxyadd_decl =
        kefir_ir_module_new_function_declaration(mem, &module, "proxyadd", NULL, proxyadd_decl_params, false, proxyadd_decl_result);
    REQUIRE(proxyadd_decl != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function *proxyadd = kefir_ir_module_new_function(mem, &module, proxyadd_decl->identifier, NULL, 1024);
    REQUIRE(proxyadd != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE_OK(kefir_ir_module_declare_global(mem, &module, proxyadd_decl->identifier));

    kefir_ir_module_id_t addstruct_id;
    struct kefir_ir_type *addstruct_decl_params = kefir_ir_module_new_type(mem, &module, 4, NULL),
                       *addstruct_decl_result = kefir_ir_module_new_type(mem, &module, 3, NULL);
    REQUIRE(addstruct_decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(addstruct_decl_result != NULL, KEFIR_INTERNAL_ERROR);
    struct kefir_ir_function_decl *addstruct_decl =
        kefir_ir_module_new_function_declaration(mem, &module,
            kefir_ir_module_symbol(mem, &module, "addstruct", &addstruct_id), NULL, addstruct_decl_params, false, addstruct_decl_result);
    REQUIRE(addstruct_decl != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE_OK(kefir_ir_module_declare_external(mem, &module, addstruct_decl->identifier));

    kefir_codegen_amd64_sysv_init(&codegen, stdout, mem);
    codegen.asmgen.settings.enable_comments = false;

    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_params, KEFIR_IR_TYPE_STRUCT, 0, 1));
    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_params, KEFIR_IR_TYPE_ARRAY, 0, 4));
    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_params, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_result, KEFIR_IR_TYPE_STRUCT, 0, 1));
    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_result, KEFIR_IR_TYPE_ARRAY, 0, 4));
    REQUIRE_OK(kefir_ir_type_append_v(proxyadd_decl_result, KEFIR_IR_TYPE_INT64, 0, 0));
    kefir_irblock_append(&proxyadd->body, KEFIR_IROPCODE_PUSH, 3);
    kefir_irblock_append(&proxyadd->body, KEFIR_IROPCODE_XCHG, 1);
    kefir_irblock_append(&proxyadd->body, KEFIR_IROPCODE_INVOKE, addstruct_id);

    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_params, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_params, KEFIR_IR_TYPE_STRUCT, 0, 1));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_params, KEFIR_IR_TYPE_ARRAY, 0, 4));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_params, KEFIR_IR_TYPE_INT64, 0, 0));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_result, KEFIR_IR_TYPE_STRUCT, 0, 1));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_result, KEFIR_IR_TYPE_ARRAY, 0, 4));
    REQUIRE_OK(kefir_ir_type_append_v(addstruct_decl_result, KEFIR_IR_TYPE_INT64, 0, 0));

    REQUIRE_OK(KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &module));
    REQUIRE_OK(KEFIR_CODEGEN_CLOSE(&codegen.iface));
    REQUIRE_OK(kefir_ir_module_free(mem, &module));
    return EXIT_SUCCESS;
}