#include <stdlib.h>
#include <stdio.h>
#include "kefir/ir/function.h"
#include "kefir/core/mem.h"
#include "kefir/core/util.h"
#include "kefir/codegen/amd64-sysv.h"

kefir_result_t kefir_int_test(struct kefir_mem *mem) {
    struct kefir_codegen_amd64 codegen;
    kefir_codegen_amd64_sysv_init(&codegen, stdout, mem);

    kefir_irfunction_t func;
    kefir_irfunction_alloc(mem, "factorial", 3, 1, 1024, &func);
    kefir_ir_type_append_v(&func.declaration.params, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_ir_type_append_v(&func.declaration.result, KEFIR_IR_TYPE_LONG, 0, 0);
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 0);       // 0:  [C; C]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PUSH, 1);       // 1:  [C; C; 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IGREATER, 0);   // 2:  [C; C > 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BRANCH, 6);     // 3:  [C]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PUSH, 1);       // 4:  [C; 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);        // 5:  -> 1
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 0);       // 6:  [C; S]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 1);       // 7:  [C; S; C]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IADD1, -1);     // 8:  [C; S; C - 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 0);       // 9:  [C; S; C - 1; C - 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PUSH, 1);       // 10: [C; S; C - 1; C - 1; 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IGREATER, 0);   // 11: [C; S; C - 1; C - 1 > 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_BRANCH, 15);    // 12: [C; S; C - 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_POP, 0);        // 13: [C; S]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_RET, 0);        // 14: -> S
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PICK, 0);       // 15: [C; S; C - 1; C - 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_PUT, 3);        // 16: [C - 1; S; C - 1]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_IMUL, 0);       // 17: [C - 1; S * (C - 1)]
    kefir_irblock_append(&func.body, KEFIR_IROPCODE_JMP, 7);        // 18: [C - 1; S * (C - 1)]

    KEFIR_CODEGEN_TRANSLATE(&codegen.iface, &func);

    KEFIR_CODEGEN_CLOSE(&codegen.iface);
    kefir_irfunction_free(mem, &func);
    return EXIT_SUCCESS;
}