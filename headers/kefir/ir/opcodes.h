#ifndef KEFIR_IR_OPCODES_H_
#define KEFIR_IR_OPCODES_H_

#include "kefir/core/base.h"

typedef enum kefir_iropcode {
    KEFIR_IROPCODE_RESERVED = 0x00,
    // Flow control
    KEFIR_IROPCODE_NOP,
    KEFIR_IROPCODE_JMP,
    KEFIR_IROPCODE_BRANCH,
    KEFIR_IROPCODE_RET,
    KEFIR_IROPCODE_INVOKE,
    KEFIR_IROPCODE_INVOKEV,

    // Stack control
    KEFIR_IROPCODE_PUSH,
    KEFIR_IROPCODE_POP,
    KEFIR_IROPCODE_PICK,
    KEFIR_IROPCODE_PUT,
    KEFIR_IROPCODE_INSERT,
    KEFIR_IROPCODE_XCHG,
    KEFIR_IROPCODE_DROP,

    // Integer arithmetics
    KEFIR_IROPCODE_IADD,
    KEFIR_IROPCODE_IADD1,
    KEFIR_IROPCODE_ISUB,
    KEFIR_IROPCODE_IMUL,
    KEFIR_IROPCODE_IDIV,
    KEFIR_IROPCODE_IMOD,
    KEFIR_IROPCODE_INEG,
    
    // Bitwise
    KEFIR_IROPCODE_INOT,
    KEFIR_IROPCODE_IAND,
    KEFIR_IROPCODE_IOR,
    KEFIR_IROPCODE_IXOR,
    KEFIR_IROPCODE_ILSHIFT,
    KEFIR_IROPCODE_IRSHIFT,
    KEFIR_IROPCODE_IARSHIFT,

    // Conditions
    KEFIR_IROPCODE_IEQUALS,
    KEFIR_IROPCODE_IGREATER,
    KEFIR_IROPCODE_ILESSER,
    KEFIR_IROPCODE_IABOVE,
    KEFIR_IROPCODE_IBELOW,
    KEFIR_IROPCODE_BAND,
    KEFIR_IROPCODE_BOR,
    KEFIR_IROPCODE_BNOT,

    // Type conversion
    KEFIR_IROPCODE_TRUNCATE1,
    KEFIR_IROPCODE_EXTEND8,
    KEFIR_IROPCODE_EXTEND16,
    KEFIR_IROPCODE_EXTEND32,

    // Data structures
    KEFIR_IROPCODE_OFFSETPTR,
    KEFIR_IROPCODE_ELEMENTPTR,

    // Memory
    KEFIR_IROPCODE_LOAD8U,
    KEFIR_IROPCODE_LOAD8I,
    KEFIR_IROPCODE_LOAD16U,
    KEFIR_IROPCODE_LOAD16I,
    KEFIR_IROPCODE_LOAD32U,
    KEFIR_IROPCODE_LOAD32I,
    KEFIR_IROPCODE_LOAD64,
    KEFIR_IROPCODE_STORE8,
    KEFIR_IROPCODE_STORE16,
    KEFIR_IROPCODE_STORE32,
    KEFIR_IROPCODE_STORE64,
    KEFIR_IROPCODE_GETLOCALS,
    KEFIR_IROPCODE_GETGLOBAL,

    // Built-ins
    KEFIR_IROPCODE_VARARG_START,
    KEFIR_IROPCODE_VARARG_COPY,
    KEFIR_IROPCODE_VARARG_GET,
    KEFIR_IROPCODE_VARARG_END,
} kefir_iropcode_t;

#endif