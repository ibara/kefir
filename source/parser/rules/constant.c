#include "kefir/parser/rule_helpers.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

kefir_result_t kefir_parser_apply_rule_constant(struct kefir_mem *mem, struct kefir_parser *parser,
                                                struct kefir_ast_node_base **result, void *payload) {
    APPLY_PROLOGUE(mem, parser, result, payload);
    REQUIRE(PARSER_TOKEN_IS(parser, 0, KEFIR_TOKEN_CONSTANT),
            KEFIR_SET_ERROR(KEFIR_NO_MATCH, "Expected constant token"));
    const struct kefir_token *token = PARSER_CURSOR(parser, 0);
    switch (token->constant.type) {
        case KEFIR_CONSTANT_TOKEN_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_int(mem, token->constant.integer)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_LONG_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_long(mem, token->constant.integer)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_LONG_LONG_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_long_long(mem, token->constant.integer)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_UNSIGNED_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_uint(mem, token->constant.uinteger)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_UNSIGNED_LONG_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_ulong(mem, token->constant.uinteger)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_UNSIGNED_LONG_LONG_INTEGER:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_ulong_long(mem, token->constant.uinteger)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_FLOAT:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_float(mem, token->constant.float32)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_DOUBLE:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_double(mem, token->constant.float64)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_CHAR:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_char(mem, token->constant.character)),
                          "Failed to allocate AST constant");
            break;

        case KEFIR_CONSTANT_TOKEN_UCHAR:
            REQUIRE_ALLOC(result, KEFIR_AST_NODE_BASE(kefir_ast_new_constant_uint(mem, token->constant.unicode_char)),
                          "Failed to allocate AST constant");
            break;
    }
    REQUIRE_OK(PARSER_SHIFT(parser));
    return KEFIR_OK;
}
