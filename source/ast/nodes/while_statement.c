#include "kefir/ast/node.h"
#include "kefir/ast/node_internal.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

NODE_VISIT_IMPL(ast_while_statement_visit, kefir_ast_while_statement, while_statement)

struct kefir_ast_node_base *ast_while_statement_clone(struct kefir_mem *, struct kefir_ast_node_base *);

kefir_result_t ast_while_statement_free(struct kefir_mem *mem, struct kefir_ast_node_base *base) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valstmt memory allocator"));
    REQUIRE(base != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valstmt AST node base"));
    ASSIGN_DECL_CAST(struct kefir_ast_while_statement *, node, base->self);
    REQUIRE_OK(KEFIR_AST_NODE_FREE(mem, node->controlling_expr));
    node->controlling_expr = NULL;
    REQUIRE_OK(KEFIR_AST_NODE_FREE(mem, node->body));
    node->body = NULL;
    KEFIR_FREE(mem, node);
    return KEFIR_OK;
}

const struct kefir_ast_node_class AST_WHILE_STATEMENT_CLASS = {.type = KEFIR_AST_WHILE_STATEMENT,
                                                               .visit = ast_while_statement_visit,
                                                               .clone = ast_while_statement_clone,
                                                               .free = ast_while_statement_free};

struct kefir_ast_node_base *ast_while_statement_clone(struct kefir_mem *mem, struct kefir_ast_node_base *base) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(base != NULL, NULL);
    ASSIGN_DECL_CAST(struct kefir_ast_while_statement *, node, base->self);
    struct kefir_ast_while_statement *clone = KEFIR_MALLOC(mem, sizeof(struct kefir_ast_while_statement));
    REQUIRE(clone != NULL, NULL);
    clone->base.klass = &AST_WHILE_STATEMENT_CLASS;
    clone->base.self = clone;
    kefir_result_t res = kefir_ast_node_properties_clone(&clone->base.properties, &node->base.properties);
    REQUIRE_ELSE(res == KEFIR_OK, {
        KEFIR_FREE(mem, clone);
        return NULL;
    });

    clone->controlling_expr = KEFIR_AST_NODE_CLONE(mem, node->controlling_expr);
    REQUIRE_ELSE(clone->controlling_expr != NULL, {
        KEFIR_FREE(mem, clone);
        return NULL;
    });

    clone->body = KEFIR_AST_NODE_CLONE(mem, node->body);
    REQUIRE_ELSE(clone->body != NULL, {
        KEFIR_AST_NODE_FREE(mem, clone->controlling_expr);
        KEFIR_FREE(mem, clone);
        return NULL;
    });
    return KEFIR_AST_NODE_BASE(clone);
}

struct kefir_ast_while_statement *kefir_ast_new_while_statement(struct kefir_mem *mem,
                                                                struct kefir_ast_node_base *controlling_expr,
                                                                struct kefir_ast_node_base *body) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(controlling_expr != NULL, NULL);
    REQUIRE(body != NULL, NULL);

    struct kefir_ast_while_statement *stmt = KEFIR_MALLOC(mem, sizeof(struct kefir_ast_while_statement));
    REQUIRE(stmt != NULL, NULL);
    stmt->base.klass = &AST_WHILE_STATEMENT_CLASS;
    stmt->base.self = stmt;
    kefir_result_t res = kefir_ast_node_properties_init(&stmt->base.properties);
    REQUIRE_ELSE(res == KEFIR_OK, {
        KEFIR_FREE(mem, stmt);
        return NULL;
    });

    stmt->controlling_expr = controlling_expr;
    stmt->body = body;
    return stmt;
}
