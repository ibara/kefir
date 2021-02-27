#include <stdio.h>
#include "kefir/core/util.h"
#include "kefir/test/unit_test.h"
#include <assert.h>

DECLARE_CASE(amd64_sysv_abi_data_test1);
DECLARE_CASE(amd64_sysv_abi_data_test2);
DECLARE_CASE(amd64_sysv_abi_data_test3);
DECLARE_CASE(amd64_sysv_abi_data_test4);
DECLARE_CASE(amd64_sysv_abi_data_test5);
DECLARE_CASE(amd64_sysv_abi_data_test6);
DECLARE_CASE(amd64_sysv_abi_data_test7);
DECLARE_CASE(amd64_sysv_abi_data_test8);
DECLARE_CASE(amd64_sysv_abi_data_test9);
DECLARE_CASE(amd64_sysv_abi_classification_test1);
DECLARE_CASE(amd64_sysv_abi_classification_test2);
DECLARE_CASE(amd64_sysv_abi_classification_test3);
DECLARE_CASE(amd64_sysv_abi_classification_test4);
DECLARE_CASE(amd64_sysv_abi_classification_test5);
DECLARE_CASE(amd64_sysv_abi_classification_test6);
DECLARE_CASE(amd64_sysv_abi_classification_test7);
DECLARE_CASE(amd64_sysv_abi_classification_test8);
DECLARE_CASE(amd64_sysv_abi_classification_test9);
DECLARE_CASE(amd64_sysv_abi_classification_test10);
DECLARE_CASE(amd64_sysv_abi_classification_test11);
DECLARE_CASE(amd64_sysv_abi_classification_test12);
DECLARE_CASE(amd64_sysv_abi_classification_test13);
DECLARE_CASE(amd64_sysv_abi_classification_test14);
DECLARE_CASE(amd64_sysv_abi_classification_test15);
DECLARE_CASE(amd64_sysv_abi_allocation_test1);
DECLARE_CASE(amd64_sysv_abi_allocation_test2);
DECLARE_CASE(amd64_sysv_abi_allocation_test3);
DECLARE_CASE(amd64_sysv_abi_allocation_test4);
DECLARE_CASE(amd64_sysv_abi_allocation_test5);
DECLARE_CASE(amd64_sysv_abi_allocation_test6);

DECLARE_CASE(ast_type_analysis_integer_promotion1);
DECLARE_CASE(ast_type_analysis_arithmetic_conversion1);
DECLARE_CASE(ast_type_analysis_arithmetic_conversion2);
DECLARE_CASE(ast_type_analysis_arithmetic_conversion3);
DECLARE_CASE(ast_scope_rules1);
DECLARE_CASE(ast_scope_rules2);
DECLARE_CASE(ast_scope_rules3);
DECLARE_CASE(ast_scope_rules4);
DECLARE_CASE(ast_scope_rules5);
DECLARE_CASE(ast_scope_rules6);
DECLARE_CASE(ast_scope_rules7);
DECLARE_CASE(ast_scope_rules8);
DECLARE_CASE(ast_scope_rules9);
DECLARE_CASE(ast_scope_rules10);
DECLARE_CASE(ast_scope_rules11);
DECLARE_CASE(ast_scope_rules12);
DECLARE_CASE(ast_scope_rules13);
DECLARE_CASE(ast_type_construction1);
DECLARE_CASE(ast_type_construction2);
DECLARE_CASE(ast_type_construction3);
DECLARE_CASE(ast_type_construction4);
DECLARE_CASE(ast_type_construction5);
DECLARE_CASE(ast_type_construction6);
DECLARE_CASE(ast_type_construction7);
DECLARE_CASE(ast_type_construction8);
DECLARE_CASE(ast_nodes_constants);
DECLARE_CASE(ast_nodes_identifiers);
DECLARE_CASE(ast_nodes_string_literals);
DECLARE_CASE(ast_nodes_array_subscripts);
DECLARE_CASE(ast_nodes_function_calls);
DECLARE_CASE(ast_nodes_struct_members);
DECLARE_CASE(ast_nodes_unary_operations);
DECLARE_CASE(ast_nodes_binary_operations);
DECLARE_CASE(ast_type_compatibility1);
DECLARE_CASE(ast_type_basic_compatibility);
DECLARE_CASE(ast_type_enum_compatibility);

TEST_SUITE(mainSuite,
    &amd64_sysv_abi_data_test1,
    &amd64_sysv_abi_data_test2,
    &amd64_sysv_abi_data_test3,
    &amd64_sysv_abi_data_test4,
    &amd64_sysv_abi_data_test5,
    &amd64_sysv_abi_data_test6,
    &amd64_sysv_abi_data_test7,
    &amd64_sysv_abi_data_test8,
    &amd64_sysv_abi_data_test9,
    &amd64_sysv_abi_classification_test1,
    &amd64_sysv_abi_classification_test2,
    &amd64_sysv_abi_classification_test3,
    &amd64_sysv_abi_classification_test4,
    &amd64_sysv_abi_classification_test5,
    &amd64_sysv_abi_classification_test6,
    &amd64_sysv_abi_classification_test7,
    &amd64_sysv_abi_classification_test8,
    &amd64_sysv_abi_classification_test9,
    &amd64_sysv_abi_classification_test10,
    &amd64_sysv_abi_classification_test11,
    &amd64_sysv_abi_classification_test12,
    &amd64_sysv_abi_classification_test13,
    &amd64_sysv_abi_classification_test14,
    &amd64_sysv_abi_classification_test15,
    &amd64_sysv_abi_allocation_test1,
    &amd64_sysv_abi_allocation_test2,
    &amd64_sysv_abi_allocation_test3,
    &amd64_sysv_abi_allocation_test4,
    &amd64_sysv_abi_allocation_test5,
    &amd64_sysv_abi_allocation_test6,
    &ast_type_analysis_integer_promotion1,
    &ast_type_analysis_arithmetic_conversion1,
    &ast_type_analysis_arithmetic_conversion2,
    &ast_type_analysis_arithmetic_conversion3,
    &ast_scope_rules1,
    &ast_scope_rules2,
    &ast_scope_rules3,
    &ast_scope_rules4,
    &ast_scope_rules5,
    &ast_scope_rules6,
    &ast_scope_rules7,
    &ast_scope_rules8,
    &ast_scope_rules9,
    &ast_scope_rules10,
    &ast_scope_rules11,
    &ast_scope_rules12,
    &ast_scope_rules13,
    &ast_type_construction1,
    &ast_type_construction2,
    &ast_type_construction3,
    &ast_type_construction4,
    &ast_type_construction5,
    &ast_type_construction6,
    &ast_type_construction7,
    &ast_type_construction8,
    &ast_nodes_constants,
    &ast_nodes_identifiers,
    &ast_nodes_string_literals,
    &ast_nodes_array_subscripts,
    &ast_nodes_function_calls,
    &ast_nodes_struct_members,
    &ast_nodes_unary_operations,
    &ast_nodes_binary_operations,
    &ast_type_compatibility1,
    &ast_type_basic_compatibility,
    &ast_type_enum_compatibility
);

int main(int argc, const char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    return kft_run_test_suite(mainSuite, mainSuiteLength, NULL);
}