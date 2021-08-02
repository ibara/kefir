#include "kefir/parser/rule_helpers.h"
#include "kefir/parser/builder.h"

static kefir_result_t builder_callback(struct kefir_mem *mem, struct kefir_parser_ast_builder *builder, void *payload) {
    UNUSED(payload);
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(builder != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid parser AST builder"));
    struct kefir_parser *parser = builder->parser;

    REQUIRE_OK(kefir_parser_ast_builder_scan(mem, builder, KEFIR_PARSER_RULE_FN(shift_expression), NULL));
    kefir_bool_t scan_additive = true;
    do {
        if (PARSER_TOKEN_IS_PUNCTUATOR(parser, 0, KEFIR_PUNCTUATOR_LESS_THAN)) {
            REQUIRE_OK(PARSER_SHIFT(parser));
            REQUIRE_OK(kefir_parser_ast_builder_scan(mem, builder, KEFIR_PARSER_RULE_FN(shift_expression), NULL));
            REQUIRE_OK(kefir_parser_ast_builder_binary_operation(mem, builder, KEFIR_AST_OPERATION_LESS));
        } else if (PARSER_TOKEN_IS_PUNCTUATOR(parser, 0, KEFIR_PUNCTUATOR_GREATER_THAN)) {
            REQUIRE_OK(PARSER_SHIFT(parser));
            REQUIRE_OK(kefir_parser_ast_builder_scan(mem, builder, KEFIR_PARSER_RULE_FN(shift_expression), NULL));
            REQUIRE_OK(kefir_parser_ast_builder_binary_operation(mem, builder, KEFIR_AST_OPERATION_GREATER));
        } else if (PARSER_TOKEN_IS_PUNCTUATOR(parser, 0, KEFIR_PUNCTUATOR_LESS_OR_EQUAL)) {
            REQUIRE_OK(PARSER_SHIFT(parser));
            REQUIRE_OK(kefir_parser_ast_builder_scan(mem, builder, KEFIR_PARSER_RULE_FN(shift_expression), NULL));
            REQUIRE_OK(kefir_parser_ast_builder_binary_operation(mem, builder, KEFIR_AST_OPERATION_LESS_EQUAL));
        } else if (PARSER_TOKEN_IS_PUNCTUATOR(parser, 0, KEFIR_PUNCTUATOR_GREATER_OR_EQUAL)) {
            REQUIRE_OK(PARSER_SHIFT(parser));
            REQUIRE_OK(kefir_parser_ast_builder_scan(mem, builder, KEFIR_PARSER_RULE_FN(shift_expression), NULL));
            REQUIRE_OK(kefir_parser_ast_builder_binary_operation(mem, builder, KEFIR_AST_OPERATION_GREATER_EQUAL));
        } else {
            scan_additive = false;
        }
    } while (scan_additive);

    return KEFIR_OK;
}

kefir_result_t KEFIR_PARSER_RULE_FN(relational_expression)(struct kefir_mem *mem, struct kefir_parser *parser,
                                                           struct kefir_ast_node_base **result, void *payload) {
    APPLY_PROLOGUE(mem, parser, result, payload);
    REQUIRE_OK(kefir_parser_ast_builder_wrap(mem, parser, result, builder_callback, NULL));
    return KEFIR_OK;
}