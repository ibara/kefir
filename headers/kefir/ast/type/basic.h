#ifndef KEFIR_AST_TYPE_BASIC_H_
#define KEFIR_AST_TYPE_BASIC_H_

#include "kefir/ast/type/base.h"

typedef struct kefir_ast_basic_type_properties {
    kefir_bool_t signedness;
    kefir_size_t rank;
} kefir_ast_basic_type_properties_t;

#define SCALAR_TYPE(id) \
    const struct kefir_ast_type *kefir_ast_type_##id()
SCALAR_TYPE(void);
SCALAR_TYPE(bool);
SCALAR_TYPE(unsigned_char);
SCALAR_TYPE(signed_char);
SCALAR_TYPE(unsigned_short);
SCALAR_TYPE(signed_short);
SCALAR_TYPE(unsigned_int);
SCALAR_TYPE(signed_int);
SCALAR_TYPE(unsigned_long);
SCALAR_TYPE(signed_long);
SCALAR_TYPE(unsigned_long_long);
SCALAR_TYPE(signed_long_long);
SCALAR_TYPE(float);
SCALAR_TYPE(double);
#undef SCALAR_TYPE

#define KEFIR_AST_TYPE_IS_SIGNED_INTEGER(base) \
    ((base)->tag == KEFIR_AST_TYPE_SCALAR_SIGNED_CHAR || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_SIGNED_SHORT || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_SIGNED_INT || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_SIGNED_LONG || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_SIGNED_LONG_LONG)
#define KEFIR_AST_TYPE_IS_UNSIGNED_INTEGER(base) \
    ((base)->tag == KEFIR_AST_TYPE_SCALAR_BOOL || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_UNSIGNED_CHAR || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_UNSIGNED_SHORT || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_UNSIGNED_INT || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG_LONG)
#define KEFIR_AST_TYPE_IS_INTEGRAL_TYPE(base) \
    (KEFIR_AST_TYPE_IS_SIGNED_INTEGER(base) || \
        KEFIR_AST_TYPE_IS_UNSIGNED_INTEGER(base) || \
        (base)->tag == KEFIR_AST_TYPE_ENUMERATION)
#define KEFIR_AST_TYPE_IS_ARITHMETIC_TYPE(base) \
    (KEFIR_AST_TYPE_IS_INTEGRAL_TYPE(base) || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_FLOAT || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_DOUBLE)
#define KEFIR_AST_TYPE_IS_SCALAR_TYPE(base) \
    (KEFIR_AST_TYPE_IS_ARITHMETIC_TYPE(base) || \
        (base)->tag == KEFIR_AST_TYPE_SCALAR_POINTER)

const struct kefir_ast_type *kefir_ast_type_flip_integer_singedness(const struct kefir_ast_type *);

#endif