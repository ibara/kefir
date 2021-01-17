#ifndef KEFIR_CODEGEN_AMD64_LABELS_H_
#define KEFIR_CODEGEN_AMD64_LABELS_H_

#include "kefir/core/basic-types.h"

#define KEFIR_AMD64_SYSV_PROCEDURE_LABEL "%s"
#define KEFIR_AMD64_SYSV_PROCEDURE_BODY_LABEL "__%s_body"
#define KEFIR_AMD64_SYSV_PROCEDURE_EPILOGUE_LABEL "__%s_epilogue"
#define KEFIR_AMD64_SYSV_FUNCTION_GATE_LABEL "__%s_gate"
#define KEFIR_AMD64_SYSV_FUNCTION_VIRTUAL_GATE_LABEL "__%s_vgate"
#define KEFIR_AMD64_SYSV_FUNCTION_VARARG_START_LABEL "__%s_vararg_start"
#define KEFIR_AMD64_SYSV_FUNCTION_VARARG_END_LABEL "__%s_vararg_end"
#define KEFIR_AMD64_SYSV_FUNCTION_VARARG_ARG_LABEL "__%s_vararg_arg" KEFIR_INT64_FMT

#endif