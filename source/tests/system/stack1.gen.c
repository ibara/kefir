#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/function.h"
#include "kefir/ir/module.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    kefir_codegen_amd64_sysv_init(&codegen, stdout, mem);

    struct kefir_ir_module module;
    REQUIRE_OK(kefir_ir_module_init(&module));
    struct kefir_ir_function_decl decl;
    struct kefir_ir_function func;
    struct kefir_ir_type *decl_params = kefir_ir_module_new_type(mem, &module, 3),
                       *decl_result = kefir_ir_module_new_type(mem, &module, 1);
    REQUIRE(decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(decl_result != NULL, KEFIR_INTERNAL_ERROR);
    kefir_ir_function_decl_alloc(mem, "insert00", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    REQUIRE(decl_params != NULL, KEFIR_INTERNAL_ERROR);
    REQUIRE(decl_result != NULL, KEFIR_INTERNAL_ERROR);
    kefir_ir_function_decl_alloc(mem, "insert01", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert02", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert10", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert11", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert12", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert20", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert21", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "insert22", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INSERT, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg00", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg01", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg02", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg10", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg11", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg12", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg20", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg21", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "xchg22", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_XCHG, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop00", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop01", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop10", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop11", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop20", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    decl_params = kefir_ir_module_new_type(mem, &module, 3),
    decl_result = kefir_ir_module_new_type(mem, &module, 1);
    kefir_ir_function_decl_alloc(mem, "drop21", decl_params, decl_result, &decl);
    kefir_ir_function_alloc(mem, &decl, 1024, &func);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(func.declaration->result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);
    kefir_ir_function_free(mem, &func);
    kefir_ir_function_decl_free(mem, &decl);

    REQUIRE_OK(kefir_ir_module_free(mem, &module));
    KEFIR_CODEGEN_CLOSE(&codegen.iface);
    return EXIT_SUCCESS;
}