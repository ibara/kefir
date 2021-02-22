#ifndef KEFIR_AST_CONSTANTS_H_
#define KEFIR_AST_CONSTANTS_H_

#include "kefir/core/basic-types.h"

typedef enum kefir_ast_node_type {
    KEFIR_AST_CONSTANT,
    KEFIR_AST_IDENTIFIER,
    KEFIR_AST_UNARY_OPERATION,
    KEFIR_AST_BINARY_OPERATION
} kefir_ast_node_type_t;

typedef enum kefir_ast_constant_type {
    KEFIR_AST_BOOL_CONSTANT,
    KEFIR_AST_CHAR_CONSTANT,
    KEFIR_AST_INT_CONSTANT,
    KEFIR_AST_UINT_CONSTANT,
    KEFIR_AST_LONG_CONSTANT,
    KEFIR_AST_ULONG_CONSTANT,
    KEFIR_AST_LONG_LONG_CONSTANT,
    KEFIR_AST_ULONG_LONG_CONSTANT,
    KEFIR_AST_FLOAT_CONSTANT,
    KEFIR_AST_DOUBLE_CONSTANT
} kefir_ast_constant_type_t;

typedef enum kefir_ast_unary_operation_type {
    KEFIR_AST_OPERATION_PLUS,
    KEFIR_AST_OPERATION_NEGATE,
    KEFIR_AST_OPERATION_INVERT,
    KEFIR_AST_OPERATION_LOGICAL_NEGATE,
} kefir_ast_unary_operation_type_t;

typedef enum kefir_ast_binary_operation_type {
    KEFIR_AST_OPERATION_ADD,
    KEFIR_AST_OPERATION_SUBTRACT,
    KEFIR_AST_OPERATION_MULTIPLY,
    KEFIR_AST_OPERATION_DIVIDE,
    KEFIR_AST_OPERATION_MODULO,
    KEFIR_AST_OPERATION_SHIFT_LEFT,
    KEFIR_AST_OPERATION_SHIFT_RIGHT
} kefir_ast_binary_operation_type_t;

typedef enum kefir_ast_storage_duration {
    KEFIR_AST_STORAGE_DURATION_STATIC,
    KEFIR_AST_STORAGE_DURATION_THREAD,
    KEFIR_AST_STORAGE_DURATION_AUTOMATIC,
    KEFIR_AST_STORAGE_DURATION_ALLOCATED
} kefir_ast_storage_duration_t;

typedef enum kefir_ast_scoped_identifier_class {
    KEFIR_AST_SCOPE_IDENTIFIER_OBJECT,
    KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION,
    KEFIR_AST_SCOPE_IDENTIFIER_TYPE_TAG,
    KEFIR_AST_SCOPE_IDENTIFIER_TYPE_DEFINITION,
    KEFIR_AST_SCOPE_IDENTIFIER_LABEL
} kefir_ast_scoped_identifier_class_t;

typedef enum kefir_ast_scoped_identifier_linkage {
    KEFIR_AST_SCOPED_IDENTIFIER_EXTERNAL_LINKAGE,
    KEFIR_AST_SCOPED_IDENTIFIER_INTERNAL_LINKAGE,
    KEFIR_AST_SCOPED_IDENTIFIER_NONE_LINKAGE
} kefir_ast_scoped_identifier_linkage_t;

typedef enum kefir_ast_scoped_identifier_storage {
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_THREAD_LOCAL,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN_THREAD_LOCAL,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC_THREAD_LOCAL,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_AUTO,
    KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_REGISTER
} kefir_ast_scoped_identifier_storage_t;

typedef void *kefir_ast_constant_expression_t; // TODO: Temporary type for marking const expressions

#endif