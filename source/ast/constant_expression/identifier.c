#include "kefir/ast/constant_expression_impl.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

kefir_result_t kefir_ast_evaluate_identifier_node(struct kefir_mem *mem,
                                              const struct kefir_ast_context *context,
                                              const struct kefir_ast_identifier *node,
                                              struct kefir_ast_constant_expression_value *value) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(context != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST context"));
    REQUIRE(node != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST constant node"));
    REQUIRE(value != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST constant expression value pointer"));
    REQUIRE(node->base.properties.category == KEFIR_AST_NODE_CATEGORY_EXPRESSION &&
        node->base.properties.expression_props.constant_expression,
        KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected constant expression AST node"));

    const struct kefir_ast_scoped_identifier *scoped_id = NULL;
    REQUIRE_OK(context->resolve_ordinary_identifier(
        context, node->identifier, &scoped_id));
    switch (scoped_id->klass) {
            case KEFIR_AST_SCOPE_IDENTIFIER_ENUM_CONSTANT: {
                *value = scoped_id->enum_constant.value->value;
            } break;

            case KEFIR_AST_SCOPE_IDENTIFIER_OBJECT:
                return KEFIR_SET_ERROR(KEFIR_NOT_IMPLEMENTED, "Object identifier constants are not implemented yet");

            case KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION:
            case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_TAG:
            case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_DEFINITION:
            case KEFIR_AST_SCOPE_IDENTIFIER_LABEL:
                return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected object or constant identifier");
    }
    return KEFIR_OK;
}