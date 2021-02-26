#include "kefir/ast/node.h"
#include "kefir/ast/node_internal.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

NODE_VISIT_IMPL(ast_unary_operation_visit, kefir_ast_unary_operation, unary_operation)

kefir_result_t ast_unary_operation_free(struct kefir_mem *mem, struct kefir_ast_node_base *base) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(base != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST node base"));
    ASSIGN_DECL_CAST(struct kefir_ast_unary_operation *, node,
        base->self);
    REQUIRE_OK(KEFIR_AST_NODE_FREE(mem, node->arg));
    KEFIR_FREE(mem, node);
    return KEFIR_OK;
}

const struct kefir_ast_node_class AST_UNARY_OPERATION_CLASS = {
    .type = KEFIR_AST_UNARY_OPERATION,
    .visit = ast_unary_operation_visit,
    .free = ast_unary_operation_free
};

struct kefir_ast_unary_operation *kefir_ast_new_unary_operation(struct kefir_mem *mem,
                                                            kefir_ast_unary_operation_type_t type,
                                                            struct kefir_ast_node_base *arg) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(arg != NULL, NULL);
    struct kefir_ast_unary_operation *oper = KEFIR_MALLOC(mem, sizeof(struct kefir_ast_unary_operation));
    REQUIRE(oper != NULL, NULL);
    oper->base.klass = &AST_UNARY_OPERATION_CLASS;
    oper->base.self = oper;
    oper->base.expression_type = NULL;
    oper->type = type;
    oper->arg = arg;
    return oper;
}