#include <stdio.h>
#include <stdarg.h>
#include "kefir/codegen/amd64-sysv.h"
#include "kefir/codegen/amd64/shortcuts.h"
#include "kefir/codegen/amd64/asmgen.h"
#include "kefir/codegen/amd64/opcodes.h"
#include "kefir/codegen/amd64/labels.h"
#include "kefir/codegen/amd64/system-v/abi.h"
#include "kefir/codegen/amd64/system-v/runtime.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

static kefir_result_t cg_symbolic_opcode(kefir_iropcode_t opcode, const char **symbolic) {
    REQUIRE(symbolic != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected symbolic != NULL"));
    *symbolic = kefir_amd64_iropcode_handler(opcode);
    return *symbolic != NULL ? KEFIR_OK : KEFIR_MALFORMED_ARG;
}

static kefir_result_t cg_declare_opcode_handler(kefir_iropcode_t opcode, const char *handler, void *payload) {
    UNUSED(opcode);
    struct kefir_amd64_asmgen *asmgen = (struct kefir_amd64_asmgen *) payload;
    ASMGEN_EXTERNAL(asmgen, handler);
    return KEFIR_OK;
}

static kefir_result_t cg_module_prologue(struct kefir_codegen_amd64 *codegen) {
    ASMGEN_SECTION(&codegen->asmgen, ".text");
    ASMGEN_NEWLINE(&codegen->asmgen, 1);
    ASMGEN_COMMENT0(&codegen->asmgen, "Opcode handlers");
    REQUIRE_OK(kefir_amd64_iropcode_handler_list(cg_declare_opcode_handler, &codegen->asmgen));
    ASMGEN_COMMENT0(&codegen->asmgen, "Runtime functions");
    for (kefir_size_t i = 0; i < KEFIR_AMD64_SYSTEM_V_RUNTIME_SYMBOL_COUNT; i++) {
        ASMGEN_EXTERNAL(&codegen->asmgen, KEFIR_AMD64_SYSTEM_V_RUNTIME_SYMBOLS[i]);
    }
    ASMGEN_NEWLINE(&codegen->asmgen, 1);
    return KEFIR_OK;
}

static kefir_result_t cg_function_prologue(struct kefir_codegen_amd64 *codegen,
                                         const struct kefir_amd64_sysv_function *func) {
    ASMGEN_GLOBAL(&codegen->asmgen, KEFIR_AMD64_SYSV_PROCEDURE_LABEL, func->func->declaration.identifier);
    ASMGEN_NEWLINE(&codegen->asmgen, 1);
    ASMGEN_LABEL(&codegen->asmgen, KEFIR_AMD64_SYSV_PROCEDURE_LABEL, func->func->declaration.identifier);
    REQUIRE_OK(kefir_amd64_sysv_function_prologue(codegen, func));
    return KEFIR_OK;
}

static kefir_result_t cg_function_epilogue(struct kefir_codegen_amd64 *codegen,
                                         const struct kefir_amd64_sysv_function *func) {
    ASMGEN_LABEL(&codegen->asmgen, KEFIR_AMD64_SYSV_PROCEDURE_EPILOGUE_LABEL, func->func->declaration.identifier);
    REQUIRE_OK(kefir_amd64_sysv_function_epilogue(codegen, func));
    return KEFIR_OK;
}

static kefir_result_t cg_function_body(struct kefir_codegen_amd64 *codegen, const struct kefir_irfunction *func) {
    ASMGEN_INSTR(&codegen->asmgen, KEFIR_AMD64_MOV);
    ASMGEN_ARG0(&codegen->asmgen, KEFIR_AMD64_SYSV_ABI_PROGRAM_REG);
    ASMGEN_ARG(&codegen->asmgen, KEFIR_AMD64_SYSV_PROCEDURE_BODY_LABEL, func->declaration.identifier);
    ASMGEN_INSTR(&codegen->asmgen, KEFIR_AMD64_JMP);
    ASMGEN_ARG(&codegen->asmgen, KEFIR_AMD64_INDIRECT, KEFIR_AMD64_SYSV_ABI_PROGRAM_REG);
    ASMGEN_LABEL(&codegen->asmgen, KEFIR_AMD64_SYSV_PROCEDURE_BODY_LABEL, func->declaration.identifier);
    const struct kefir_irinstr *instr = NULL;
    const char *opcode_symbol = NULL;
    for (kefir_size_t pc = 0; pc < kefir_irblock_length(&func->body); pc++) {
        instr = kefir_irblock_at(&func->body, pc);
        REQUIRE(instr != NULL, KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Unable to fetch instruction from IR block"));
        if (instr->opcode == KEFIR_IROPCODE_RET) {
            ASMGEN_RAW(&codegen->asmgen, KEFIR_AMD64_QUAD);
            ASMGEN_ARG(&codegen->asmgen,
                KEFIR_AMD64_SYSV_PROCEDURE_EPILOGUE_LABEL, func->declaration.identifier);
            ASMGEN_ARG0(&codegen->asmgen, "0");
        } else {
            REQUIRE_OK(cg_symbolic_opcode(instr->opcode, &opcode_symbol));
            ASMGEN_RAW(&codegen->asmgen, KEFIR_AMD64_QUAD);
            ASMGEN_ARG0(&codegen->asmgen, opcode_symbol);
            ASMGEN_ARG(&codegen->asmgen, KEFIR_INT64_FMT, instr->arg);
        }
    }
    return KEFIR_OK;
}

static kefir_result_t cg_translate(struct kefir_codegen *cg_iface, const struct kefir_irfunction *func) {
    REQUIRE(cg_iface != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid code generator interface"));
    REQUIRE(cg_iface->data != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AMD64 code generator"));
    struct kefir_codegen_amd64 *codegen = (struct kefir_codegen_amd64 *) cg_iface->data;
    REQUIRE_OK(cg_module_prologue(codegen));
    struct kefir_amd64_sysv_function sysv_func;
    REQUIRE_OK(kefir_amd64_sysv_function_alloc(codegen->mem, func, &sysv_func));
    kefir_result_t res = cg_function_prologue(codegen, &sysv_func);
    REQUIRE_CHAIN(&res, cg_function_body(codegen, func));
    REQUIRE_CHAIN(&res, cg_function_epilogue(codegen, &sysv_func));
    kefir_amd64_sysv_function_free(codegen->mem, &sysv_func);
    return res;
}

static kefir_result_t cg_close(struct kefir_codegen *cg) {
    REQUIRE(cg != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid code generator interface"));
    REQUIRE(cg->data != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AMD64 code generator"));
    struct kefir_codegen_amd64 *codegen = (struct kefir_codegen_amd64 *) cg->data;
    KEFIR_AMD64_ASMGEN_CLOSE(&codegen->asmgen);
    return KEFIR_OK;
}

kefir_result_t kefir_codegen_amd64_sysv_init(struct kefir_codegen_amd64 *codegen, FILE *out, struct kefir_mem *mem) {
    REQUIRE(codegen != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AMD64 code generator pointer"));
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE_OK(kefir_amd64_nasm_gen_init(&codegen->asmgen, out));
    codegen->iface.translate = cg_translate;
    codegen->iface.close = cg_close;
    codegen->iface.data = codegen;
    codegen->mem = mem;
    return KEFIR_OK;
}