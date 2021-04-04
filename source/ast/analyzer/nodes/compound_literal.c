#include "kefir/ast/analyzer/nodes.h"
#include "kefir/ast/analyzer/initializer.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

kefir_result_t kefir_ast_analyze_compound_literal_node(struct kefir_mem *mem,
                                                   const struct kefir_ast_context *context,
                                                   const struct kefir_ast_compound_literal *node,
                                                   struct kefir_ast_node_base *base) {
    UNUSED(mem);
    REQUIRE(context != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST context"));
    REQUIRE(node != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST compound literal"));
    REQUIRE(base != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST base node"));
    
    const struct kefir_ast_type *unqualified_type = kefir_ast_unqualified_type(node->type);
    REQUIRE(!KEFIR_AST_TYPE_IS_INCOMPLETE(unqualified_type) ||
        (unqualified_type->tag == KEFIR_AST_TYPE_ARRAY && unqualified_type->array_type.boundary == KEFIR_AST_ARRAY_UNBOUNDED),
        KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Compound literal type shall be complete object or array of unknown length"));
    struct kefir_ast_initializer_properties initializer_properties;
    REQUIRE_OK(kefir_ast_analyze_initializer(mem, context, node->type, node->initializer, &initializer_properties));
    REQUIRE_OK(kefir_ast_node_properties_init(&base->properties));
    base->properties.category = KEFIR_AST_NODE_CATEGORY_EXPRESSION;
    base->properties.type = initializer_properties.type;
    base->properties.expression_props.lvalue = true;
    base->properties.expression_props.addressable = true;
    base->properties.expression_props.constant_expression = initializer_properties.constant;
    return KEFIR_OK;
}