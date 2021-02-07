#ifndef KEFIR_IR_MNEMONIC_H_
#define KEFIR_IR_MNEMONIC_H_

#include "kefir/ir/opcodes.h"

// Do not edit this file. It is autogenerated. Edit resources/opcodes.xml instead.
#define KEFIR_IROPCODE_MNEMONIC_RESERVED "reserved"
#define KEFIR_IROPCODE_MNEMONIC_NOP "nop"
#define KEFIR_IROPCODE_MNEMONIC_JMP "jmp"
#define KEFIR_IROPCODE_MNEMONIC_BRANCH "branch"
#define KEFIR_IROPCODE_MNEMONIC_RET "ret"
#define KEFIR_IROPCODE_MNEMONIC_INVOKE "invoke"
#define KEFIR_IROPCODE_MNEMONIC_INVOKEV "invokev"
#define KEFIR_IROPCODE_MNEMONIC_PUSHI64 "push"
#define KEFIR_IROPCODE_MNEMONIC_PUSHU64 "push"
#define KEFIR_IROPCODE_MNEMONIC_POP "pop"
#define KEFIR_IROPCODE_MNEMONIC_PICK "pick"
#define KEFIR_IROPCODE_MNEMONIC_PUT "put"
#define KEFIR_IROPCODE_MNEMONIC_INSERT "insert"
#define KEFIR_IROPCODE_MNEMONIC_XCHG "xchg"
#define KEFIR_IROPCODE_MNEMONIC_DROP "drop"
#define KEFIR_IROPCODE_MNEMONIC_IADD "iadd"
#define KEFIR_IROPCODE_MNEMONIC_IADD1 "iadd1"
#define KEFIR_IROPCODE_MNEMONIC_ISUB "isub"
#define KEFIR_IROPCODE_MNEMONIC_IMUL "imul"
#define KEFIR_IROPCODE_MNEMONIC_IDIV "idiv"
#define KEFIR_IROPCODE_MNEMONIC_IMOD "imod"
#define KEFIR_IROPCODE_MNEMONIC_INEG "ineg"
#define KEFIR_IROPCODE_MNEMONIC_INOT "inot"
#define KEFIR_IROPCODE_MNEMONIC_IAND "iand"
#define KEFIR_IROPCODE_MNEMONIC_IOR "ior"
#define KEFIR_IROPCODE_MNEMONIC_IXOR "ixor"
#define KEFIR_IROPCODE_MNEMONIC_ILSHIFT "ishl"
#define KEFIR_IROPCODE_MNEMONIC_IRSHIFT "ishr"
#define KEFIR_IROPCODE_MNEMONIC_IARSHIFT "isar"
#define KEFIR_IROPCODE_MNEMONIC_IEQUALS "iequals"
#define KEFIR_IROPCODE_MNEMONIC_IGREATER "igreater"
#define KEFIR_IROPCODE_MNEMONIC_ILESSER "ilesser"
#define KEFIR_IROPCODE_MNEMONIC_IABOVE "iabove"
#define KEFIR_IROPCODE_MNEMONIC_IBELOW "ibelow"
#define KEFIR_IROPCODE_MNEMONIC_BAND "band"
#define KEFIR_IROPCODE_MNEMONIC_BOR "bor"
#define KEFIR_IROPCODE_MNEMONIC_BNOT "bnot"
#define KEFIR_IROPCODE_MNEMONIC_TRUNCATE1 "trunc1"
#define KEFIR_IROPCODE_MNEMONIC_EXTEND8 "extend8"
#define KEFIR_IROPCODE_MNEMONIC_EXTEND16 "extend16"
#define KEFIR_IROPCODE_MNEMONIC_EXTEND32 "extend32"
#define KEFIR_IROPCODE_MNEMONIC_OFFSETPTR "offsetptr"
#define KEFIR_IROPCODE_MNEMONIC_ELEMENTPTR "elementptr"
#define KEFIR_IROPCODE_MNEMONIC_GETGLOBAL "getglobal"
#define KEFIR_IROPCODE_MNEMONIC_GETLOCALS "getlocals"
#define KEFIR_IROPCODE_MNEMONIC_LOAD8U "load8u"
#define KEFIR_IROPCODE_MNEMONIC_LOAD8I "load8i"
#define KEFIR_IROPCODE_MNEMONIC_LOAD16U "load16u"
#define KEFIR_IROPCODE_MNEMONIC_LOAD16I "load16i"
#define KEFIR_IROPCODE_MNEMONIC_LOAD32U "load32u"
#define KEFIR_IROPCODE_MNEMONIC_LOAD32I "load32i"
#define KEFIR_IROPCODE_MNEMONIC_LOAD64 "load64"
#define KEFIR_IROPCODE_MNEMONIC_STORE8 "store8"
#define KEFIR_IROPCODE_MNEMONIC_STORE16 "store16"
#define KEFIR_IROPCODE_MNEMONIC_STORE32 "store32"
#define KEFIR_IROPCODE_MNEMONIC_STORE64 "store64"
#define KEFIR_IROPCODE_MNEMONIC_VARARG_START "startvarg"
#define KEFIR_IROPCODE_MNEMONIC_VARARG_COPY "copyvarg"
#define KEFIR_IROPCODE_MNEMONIC_VARARG_GET "getvarg"
#define KEFIR_IROPCODE_MNEMONIC_VARARG_END "endvarg"
#define KEFIR_IROPCODE_MNEMONIC_PUSHF32 "pushf32"
#define KEFIR_IROPCODE_MNEMONIC_PUSHF64 "pushf64"
#define KEFIR_IROPCODE_MNEMONIC_F32ADD "f32add"
#define KEFIR_IROPCODE_MNEMONIC_F32SUB "f32sub"
#define KEFIR_IROPCODE_MNEMONIC_F32MUL "f32mul"
#define KEFIR_IROPCODE_MNEMONIC_F32DIV "f32div"
#define KEFIR_IROPCODE_MNEMONIC_F32NEG "f32neg"
#define KEFIR_IROPCODE_MNEMONIC_F64ADD "f64add"
#define KEFIR_IROPCODE_MNEMONIC_F64SUB "f64sub"
#define KEFIR_IROPCODE_MNEMONIC_F64MUL "f64mul"
#define KEFIR_IROPCODE_MNEMONIC_F64DIV "f64div"
#define KEFIR_IROPCODE_MNEMONIC_F64NEG "f64neg"
#define KEFIR_IROPCODE_MNEMONIC_F32EQUALS "f32equals"
#define KEFIR_IROPCODE_MNEMONIC_F32GREATER "f32greater"
#define KEFIR_IROPCODE_MNEMONIC_F32LESSER "f32lesser"
#define KEFIR_IROPCODE_MNEMONIC_F64EQUALS "f64equals"
#define KEFIR_IROPCODE_MNEMONIC_F64GREATER "f64greater"
#define KEFIR_IROPCODE_MNEMONIC_F64LESSER "f64lesser"
#define KEFIR_IROPCODE_MNEMONIC_F32CINT "f32cint"
#define KEFIR_IROPCODE_MNEMONIC_F64CINT "f64cint"
#define KEFIR_IROPCODE_MNEMONIC_INTCF32 "intcf32"
#define KEFIR_IROPCODE_MNEMONIC_INTCF64 "intcf64"
#define KEFIR_IROPCODE_MNEMONIC_UINTCF32 "uintcf32"
#define KEFIR_IROPCODE_MNEMONIC_UINTCF64 "uintcf64"
#define KEFIR_IROPCODE_MNEMONIC_F32CF64 "f32cf64"
#define KEFIR_IROPCODE_MNEMONIC_F64CF32 "f64cf32"

const char *kefir_iropcode_mnemonic(kefir_iropcode_t);

#endif
