#include "kefir/ast-translator/scope/translator.h"
#include "kefir/ast-translator/context.h"
#include "kefir/ast/runtime.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

static kefir_result_t translate_externals(struct kefir_mem *mem, struct kefir_ir_module *module,
                                          const struct kefir_ast_translator_global_scope_layout *global_scope) {
    for (const struct kefir_list_entry *iter = kefir_list_head(&global_scope->external_objects); iter != NULL;
         kefir_list_next(&iter)) {
        ASSIGN_DECL_CAST(const struct kefir_ast_translator_scoped_identifier_entry *, scoped_identifier, iter->value);
        switch (scoped_identifier->value->klass) {
            case KEFIR_AST_SCOPE_IDENTIFIER_OBJECT: {
                ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier_object *, identifier_data,
                                 scoped_identifier->value->payload.ptr);
                if (scoped_identifier->value->object.external) {
                    REQUIRE_OK(kefir_ir_module_declare_external(mem, module, scoped_identifier->identifier));
                } else {
                    struct kefir_ir_data *data = kefir_ir_module_new_named_data(
                        mem, module, scoped_identifier->identifier, identifier_data->type_id);
                    REQUIRE(data != NULL, KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate IR named data"));
                    REQUIRE_OK(kefir_ir_data_finalize(data));

                    REQUIRE_OK(kefir_ir_module_declare_global(mem, module, scoped_identifier->identifier));
                }
            } break;

            case KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION: {
                if (scoped_identifier->value->function.external) {
                    REQUIRE_OK(kefir_ir_module_declare_external(mem, module, scoped_identifier->identifier));
                } else if (scoped_identifier->value->function.storage != KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC) {
                    REQUIRE_OK(kefir_ir_module_declare_global(mem, module, scoped_identifier->identifier));
                }
            } break;

            default:
                return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Unable to translator global scoped identifier");
        }
    }
    return KEFIR_OK;
}

static kefir_result_t translate_static(struct kefir_mem *mem, struct kefir_ir_module *module,
                                       const struct kefir_ast_translator_global_scope_layout *global_scope) {
    if (kefir_ir_type_total_length(global_scope->static_layout) > 0) {
        struct kefir_ir_data *static_data = kefir_ir_module_new_named_data(
            mem, module, KEFIR_AST_TRANSLATOR_STATIC_VARIABLES_IDENTIFIER, global_scope->static_layout_id);
        REQUIRE(static_data != NULL, KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate IR named data"));
        REQUIRE_OK(kefir_ir_data_finalize(static_data));
    }

    for (const struct kefir_list_entry *iter = kefir_list_head(&global_scope->static_objects); iter != NULL;
         kefir_list_next(&iter)) {
        ASSIGN_DECL_CAST(const struct kefir_ast_translator_scoped_identifier_entry *, scoped_identifier, iter->value);

        switch (scoped_identifier->value->klass) {
            case KEFIR_AST_SCOPE_IDENTIFIER_OBJECT:
            case KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION:
                // Do nothing
                break;

            default:
                return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Unable to translator global scoped identifier");
        }
    }
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translate_global_scope(struct kefir_mem *mem, struct kefir_ir_module *module,
                                                const struct kefir_ast_translator_global_scope_layout *global_scope) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(module != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR module"));
    REQUIRE(global_scope != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST translator global scope"));

    REQUIRE_OK(translate_externals(mem, module, global_scope));
    REQUIRE_OK(translate_static(mem, module, global_scope));
    // TODO Implement thread-local objects

    return KEFIR_OK;
}
