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
    kefir_codegen_amd64_sysv_init(&codegen, stdout, mem);
    codegen.asmgen.settings.enable_comments = false;

    REQUIRE_OK(kefir_ir_type_alloc(mem, 0, &decl_params));
    REQUIRE_OK(kefir_ir_type_alloc(mem, 0, &decl_result));
    REQUIRE_OK(kefir_ir_function_decl_alloc(mem, "func1", &decl_params, &decl_result, &decl));
    REQUIRE_OK(kefir_ir_function_alloc(mem, &decl, 1024, &func));
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_NOP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_JMP, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BRANCH, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PUSH, 1000);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_DROP, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IADD, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IADD1, 1);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_ISUB, 2);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IMUL, 3);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IDIV, 4);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IMOD, 5);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INEG, 6);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_INOT, 7);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IAND, 8);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IOR, 9);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IXOR, 10);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IRSHIFT, 11);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IARSHIFT, 12);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_ILSHIFT, 13);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IEQUALS, 14);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IGREATER, 15);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_ILESSER, 16);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BAND, 17);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BOR, 18);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BNOT, 19);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_TRUNCATE1, 20);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_EXTEND8, 21);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_EXTEND16, 22);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_EXTEND32, 23);
    REQUIRE_OK(KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func));

    REQUIRE_OK(KEFIR_CODEGEN_CLOSE(&codegen.iface));
    REQUIRE_OK(kefir_ir_function_free(mem, &func));
    REQUIRE_OK(kefir_ir_function_decl_free(mem, &decl));
    REQUIRE_OK(kefir_ir_type_free(mem, &decl_result));
    REQUIRE_OK(kefir_ir_type_free(mem, &decl_params));
    return KEFIR_OK;
}