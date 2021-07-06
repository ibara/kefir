#ifndef KEFIR_AST_ANALYZER_NODES_H_
#define KEFIR_AST_ANALYZER_NODES_H_

#include "kefir/ast/node.h"
#include "kefir/ast/context.h"

#define ANALYZE_NODE(id, type)                                                                                       \
    kefir_result_t kefir_ast_analyze_##id##_node(struct kefir_mem *, const struct kefir_ast_context *, const type *, \
                                                 struct kefir_ast_node_base *)

ANALYZE_NODE(constant, struct kefir_ast_constant);
ANALYZE_NODE(identifier, struct kefir_ast_identifier);
ANALYZE_NODE(unary_operation, struct kefir_ast_unary_operation);
ANALYZE_NODE(binary_operation, struct kefir_ast_binary_operation);
ANALYZE_NODE(array_subscript, struct kefir_ast_array_subscript);
ANALYZE_NODE(function_call, struct kefir_ast_function_call);
ANALYZE_NODE(struct_member, struct kefir_ast_struct_member);
ANALYZE_NODE(string_literal, struct kefir_ast_string_literal);
ANALYZE_NODE(declaration, struct kefir_ast_declaration);
ANALYZE_NODE(type_name, struct kefir_ast_type_name);
ANALYZE_NODE(cast_operator, struct kefir_ast_cast_operator);
ANALYZE_NODE(generic_selection, struct kefir_ast_generic_selection);
ANALYZE_NODE(conditional_operator, struct kefir_ast_conditional_operator);
ANALYZE_NODE(assignment_operator, struct kefir_ast_assignment_operator);
ANALYZE_NODE(comma_operator, struct kefir_ast_comma_operator);
ANALYZE_NODE(compound_literal, struct kefir_ast_compound_literal);
ANALYZE_NODE(static_assertion, struct kefir_ast_static_assertion);
ANALYZE_NODE(labeled_statement, struct kefir_ast_labeled_statement);
ANALYZE_NODE(case_statement, struct kefir_ast_case_statement);
ANALYZE_NODE(expression_statement, struct kefir_ast_expression_statement);
ANALYZE_NODE(compound_statement, struct kefir_ast_compound_statement);
ANALYZE_NODE(conditional_statement, struct kefir_ast_conditional_statement);
ANALYZE_NODE(switch_statement, struct kefir_ast_switch_statement);
ANALYZE_NODE(while_statement, struct kefir_ast_while_statement);
ANALYZE_NODE(do_while_statement, struct kefir_ast_do_while_statement);
ANALYZE_NODE(for_statement, struct kefir_ast_for_statement);
ANALYZE_NODE(goto_statement, struct kefir_ast_goto_statement);
ANALYZE_NODE(continue_statement, struct kefir_ast_continue_statement);
ANALYZE_NODE(break_statement, struct kefir_ast_break_statement);
ANALYZE_NODE(return_statement, struct kefir_ast_return_statement);
ANALYZE_NODE(function_definition, struct kefir_ast_function_definition);
ANALYZE_NODE(translation_unit, struct kefir_ast_translation_unit);

#undef ANALYZE_NODE

#endif
