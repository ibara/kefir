#include "kefir/ast-translator/typeconv.h"
#include "kefir/ast-translator/util.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

static kefir_result_t cast_to_float32(struct kefir_irbuilder_block *builder,
                                    const struct kefir_ast_type *origin) {
    if (KEFIR_AST_TYPE_IS_SIGNED_INTEGER(origin)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_INTCF32, 0));
    } else if (KEFIR_AST_TYPE_IS_UNSIGNED_INTEGER(origin)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_UINTCF32, 0));
    } else if (origin->tag == KEFIR_AST_TYPE_SCALAR_DOUBLE) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_F64CF32, 0));
    } else {
        return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Cannot cast pointers to floating-point values");
    }
    return KEFIR_OK;
}

static kefir_result_t cast_to_float64(struct kefir_irbuilder_block *builder,
                                    const struct kefir_ast_type *origin) {
    if (KEFIR_AST_TYPE_IS_SIGNED_INTEGER(origin)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_INTCF64, 0));
    } else if (KEFIR_AST_TYPE_IS_UNSIGNED_INTEGER(origin)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_UINTCF64, 0));
    } else if (origin->tag == KEFIR_AST_TYPE_SCALAR_FLOAT) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_F32CF64, 0));
    } else {
        return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Cannot cast pointers to floating-point values");
    }
    return KEFIR_OK;
}

static kefir_result_t cast_to_integer(struct kefir_irbuilder_block *builder,
                                    const struct kefir_ast_type *origin) {
    if (KEFIR_AST_TYPE_IS_INTEGRAL_TYPE(origin) || origin->tag == KEFIR_AST_TYPE_SCALAR_POINTER) {
        // Do nothing
    } else if (origin->tag == KEFIR_AST_TYPE_SCALAR_FLOAT) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_F32CINT, 0));
    } else if (origin->tag == KEFIR_AST_TYPE_SCALAR_DOUBLE) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_F64CINT, 0));
    } else {
        return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Unexpected type in integral conversion");
    }
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translate_typeconv(struct kefir_irbuilder_block *builder,
                                        const struct kefir_ast_type *origin,
                                        const struct kefir_ast_type *destination) {
    REQUIRE(builder != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR block builder"));
    REQUIRE(origin != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid origin AST type"));
    REQUIRE(destination != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid destination AST type"));

    const struct kefir_ast_type *normalized_origin = kefir_ast_translator_normalize_type(origin);
    const struct kefir_ast_type *normalized_destination = kefir_ast_translator_normalize_type(destination);

    REQUIRE(KEFIR_AST_TYPE_IS_SCALAR_TYPE(normalized_origin) || normalized_destination->tag == KEFIR_AST_TYPE_VOID,
        KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected origin AST type to be scalar"));
    REQUIRE(KEFIR_AST_TYPE_IS_SCALAR_TYPE(normalized_destination) || normalized_destination->tag == KEFIR_AST_TYPE_VOID,
        KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected destination AST type to be scalar or void"));
    REQUIRE(!KEFIR_AST_TYPE_SAME(normalized_origin, normalized_destination), KEFIR_OK);

    switch (normalized_destination->tag) {
        case KEFIR_AST_TYPE_VOID:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDI64(builder, KEFIR_IROPCODE_POP, 0));
            break;

        case KEFIR_AST_TYPE_SCALAR_POINTER:
            REQUIRE(KEFIR_AST_TYPE_IS_INTEGRAL_TYPE(normalized_origin) ||
                normalized_origin->tag == KEFIR_AST_TYPE_SCALAR_POINTER,
                KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected origin type to be integral or pointer"));
            break;

        case KEFIR_AST_TYPE_SCALAR_FLOAT:
            REQUIRE_OK(cast_to_float32(builder, normalized_origin));
            break;

        case KEFIR_AST_TYPE_SCALAR_DOUBLE:
            REQUIRE_OK(cast_to_float64(builder, normalized_origin));
            break;

        default:
            REQUIRE_OK(cast_to_integer(builder, normalized_origin));
            break;
    }
    return KEFIR_OK;
}