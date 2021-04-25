#include "kefir/ast-translator/scope/global_scope_layout.h"
#include "kefir/ast-translator/translator.h"
#include "kefir/ast-translator/layout.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"
#include "kefir/ast-translator/scope/scope_layout_impl.h"

kefir_result_t kefir_ast_translator_global_scope_layout_init(struct kefir_mem *mem,
                                              struct kefir_ir_module *module,
                                              struct kefir_ast_translator_global_scope_layout *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(module != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR module"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    REQUIRE_OK(kefir_list_init(&layout->external_objects));
    REQUIRE_OK(kefir_list_init(&layout->external_thread_local_objects));
    REQUIRE_OK(kefir_list_init(&layout->static_objects));
    REQUIRE_OK(kefir_list_init(&layout->static_thread_local_objects));
    
    REQUIRE_OK(kefir_list_on_remove(&layout->external_objects, kefir_ast_translator_scoped_identifier_remove, NULL));
    REQUIRE_OK(kefir_list_on_remove(&layout->external_thread_local_objects, kefir_ast_translator_scoped_identifier_remove, NULL));
    REQUIRE_OK(kefir_list_on_remove(&layout->static_objects, kefir_ast_translator_scoped_identifier_remove, NULL));
    REQUIRE_OK(kefir_list_on_remove(&layout->static_thread_local_objects, kefir_ast_translator_scoped_identifier_remove, NULL));

    layout->static_layout = kefir_ir_module_new_type(mem, module, 0, &layout->static_layout_id);
    REQUIRE(layout->static_layout != NULL,
        KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate new IR type"));
    layout->static_thread_local_layout = kefir_ir_module_new_type(mem, module, 0, &layout->static_thread_local_layout_id);
    REQUIRE(layout->static_thread_local_layout != NULL,
        KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate new IR type"));
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translator_global_scope_layout_free(struct kefir_mem *mem,
                                              struct kefir_ast_translator_global_scope_layout *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    REQUIRE_OK(kefir_list_free(mem, &layout->external_objects));
    REQUIRE_OK(kefir_list_free(mem, &layout->external_thread_local_objects));
    REQUIRE_OK(kefir_list_free(mem, &layout->static_objects));
    REQUIRE_OK(kefir_list_free(mem, &layout->static_thread_local_objects));
    layout->static_layout = NULL;
    layout->static_thread_local_layout = NULL;
    return KEFIR_OK;
}

static kefir_result_t translate_scoped_identifier_type(struct kefir_mem *mem,
                                                     struct kefir_ir_module *module,
                                                     struct kefir_ast_translator_type_resolver *type_resolver,
                                                     const struct kefir_ast_translator_environment *env,
                                                     const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                     struct kefir_ir_type **type_ptr) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier_object *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    KEFIR_AST_SCOPE_SET_CLEANUP(scoped_identifier, kefir_ast_translator_scoped_identifer_payload_free, NULL);
    scoped_identifier_layout->type = kefir_ir_module_new_type(mem, module, 0, &scoped_identifier_layout->type_id);
    REQUIRE(scoped_identifier_layout->type != NULL, KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Unable to allocate new IR type"));
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, scoped_identifier_layout->type));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));

    REQUIRE_OK(kefir_ast_translator_evaluate_type_layout(mem, env, scoped_identifier_layout->layout, scoped_identifier_layout->type));
    *type_ptr = scoped_identifier_layout->type;

    REQUIRE_OK(KEFIR_AST_TRANSLATOR_TYPE_RESOLVER_REGISTER_OBJECT(mem, type_resolver,
        scoped_identifier_layout->type_id, scoped_identifier_layout->type, scoped_identifier_layout->layout));
    return KEFIR_OK;
}

static kefir_result_t translate_extern_identifier(struct kefir_mem *mem,
                                                struct kefir_ir_module *module,
                                                struct kefir_ast_translator_type_resolver *type_resolver,
                                                const struct kefir_ast_translator_environment *env,
                                                struct kefir_ast_translator_global_scope_layout *layout,
                                                const char *identifier,
                                                const struct kefir_ast_scoped_identifier *scoped_identifier) {      
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, type_resolver, env, scoped_identifier, &type));
    REQUIRE_OK(kefir_ast_translator_scoped_identifier_insert(mem, identifier, scoped_identifier, &layout->external_objects));
    return KEFIR_OK;
}

static kefir_result_t translate_extern_thread_local_identifier(struct kefir_mem *mem,
                                                             struct kefir_ir_module *module,
                                                             struct kefir_ast_translator_type_resolver *type_resolver,
                                                             const struct kefir_ast_translator_environment *env,
                                                             struct kefir_ast_translator_global_scope_layout *layout,
                                                             const char *identifier,
                                                             const struct kefir_ast_scoped_identifier *scoped_identifier) {
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, type_resolver, env, scoped_identifier, &type));
    REQUIRE_OK(kefir_ast_translator_scoped_identifier_insert(mem, identifier, scoped_identifier,
        &layout->external_thread_local_objects));
    return KEFIR_OK;
}

static kefir_result_t translate_thread_local_identifier(struct kefir_mem *mem,
                                                      struct kefir_ir_module *module,
                                                      struct kefir_ast_translator_type_resolver *type_resolver,
                                                      const struct kefir_ast_translator_environment *env,
                                                      struct kefir_ast_translator_global_scope_layout *layout,
                                                      const char *identifier,
                                                      const struct kefir_ast_scoped_identifier *scoped_identifier) {
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, type_resolver, env, scoped_identifier, &type));
    REQUIRE_OK(kefir_ast_translator_scoped_identifier_insert(mem, identifier, scoped_identifier,
        &layout->external_thread_local_objects));
    return KEFIR_OK;
}

static kefir_result_t translate_static_identifier(struct kefir_mem *mem,
                                                struct kefir_ast_translator_type_resolver *type_resolver,
                                                const struct kefir_ast_translator_environment *env,
                                                struct kefir_ast_translator_global_scope_layout *layout,
                                                const char *identifier,
                                                const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier_object *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    KEFIR_AST_SCOPE_SET_CLEANUP(scoped_identifier, kefir_ast_translator_scoped_identifer_payload_free, NULL);
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, layout->static_layout));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));
    scoped_identifier_layout->type_id = layout->static_layout_id;
    scoped_identifier_layout->type = layout->static_layout;

    REQUIRE_OK(kefir_ast_translator_evaluate_type_layout(mem, env, scoped_identifier_layout->layout, scoped_identifier_layout->type));
    REQUIRE_OK(kefir_ast_translator_scoped_identifier_insert(mem, identifier, scoped_identifier, &layout->static_objects));

    REQUIRE_OK(KEFIR_AST_TRANSLATOR_TYPE_RESOLVER_REGISTER_OBJECT(mem, type_resolver, scoped_identifier_layout->type_id,
        scoped_identifier_layout->type, scoped_identifier_layout->layout));
    return KEFIR_OK;                             
}

static kefir_result_t translate_static_thread_local_identifier(struct kefir_mem *mem,
                                                             struct kefir_ast_translator_type_resolver *type_resolver,
                                                             const struct kefir_ast_translator_environment *env,
                                                             struct kefir_ast_translator_global_scope_layout *layout,
                                                             const char *identifier,
                                                             const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier_object *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    KEFIR_AST_SCOPE_SET_CLEANUP(scoped_identifier, kefir_ast_translator_scoped_identifer_payload_free, NULL);
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, layout->static_thread_local_layout));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));
    scoped_identifier_layout->type_id = layout->static_thread_local_layout_id;
    scoped_identifier_layout->type = layout->static_thread_local_layout;

    REQUIRE_OK(kefir_ast_translator_evaluate_type_layout(mem, env, scoped_identifier_layout->layout, scoped_identifier_layout->type));
    REQUIRE_OK(kefir_ast_translator_scoped_identifier_insert(mem, identifier, scoped_identifier,
        &layout->static_thread_local_objects));

    REQUIRE_OK(KEFIR_AST_TRANSLATOR_TYPE_RESOLVER_REGISTER_OBJECT(mem, type_resolver, scoped_identifier_layout->type_id,
        scoped_identifier_layout->type, scoped_identifier_layout->layout));
    return KEFIR_OK;
}

static kefir_result_t translate_global_scoped_identifier_object(struct kefir_mem *mem,
                                                              struct kefir_ir_module *module,
                                                              struct kefir_ast_translator_type_resolver *type_resolver,
                                                              const char *identifier,
                                                              const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                              struct kefir_ast_translator_global_scope_layout *layout,
                                                              const struct kefir_ast_translator_environment *env) {
    REQUIRE(scoped_identifier->klass == KEFIR_AST_SCOPE_IDENTIFIER_OBJECT, KEFIR_OK);
    switch (scoped_identifier->object.storage) {
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN:
            REQUIRE_OK(translate_extern_identifier(mem, module, type_resolver, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN_THREAD_LOCAL:
            REQUIRE_OK(translate_extern_thread_local_identifier(mem, module, type_resolver, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_THREAD_LOCAL:
            REQUIRE_OK(translate_thread_local_identifier(mem, module, type_resolver, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC:
            REQUIRE_OK(translate_static_identifier(mem, type_resolver, env, layout, identifier, scoped_identifier));
            break;
        
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC_THREAD_LOCAL:
            REQUIRE_OK(translate_static_thread_local_identifier(mem, type_resolver, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_AUTO:
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_REGISTER:
            return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "File-scope variable cannot have auto/register storage");
    }
    return KEFIR_OK;
}

static kefir_result_t translate_global_scoped_identifier_function(struct kefir_mem *mem,
                                                                struct kefir_ir_module *module,
                                                                struct kefir_ast_type_bundle *type_bundle,
                                                                const struct kefir_ast_type_traits *type_traits,
                                                                struct kefir_ast_translator_type_resolver *type_resolver,
                                                                const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                                const struct kefir_ast_translator_environment *env) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier_function *, scoped_identifier_func,
        scoped_identifier->payload.ptr);
    KEFIR_AST_SCOPE_SET_CLEANUP(scoped_identifier, kefir_ast_translator_scoped_identifer_payload_free, NULL);
    REQUIRE_OK(kefir_ast_translator_function_declaration_init(mem,
        env, type_bundle, type_traits, module, type_resolver, scoped_identifier->function.type, NULL, &scoped_identifier_func->declaration));
    REQUIRE_OK(KEFIR_AST_TRANSLATOR_TYPE_RESOLVER_REGISTER_FUNCTION(mem, type_resolver, scoped_identifier_func->declaration));
    return KEFIR_OK;
}

static kefir_result_t translate_global_scoped_identifier(struct kefir_mem *mem,
                                                       struct kefir_ir_module *module,
                                                       struct kefir_ast_type_bundle *type_bundle,
                                                       const struct kefir_ast_type_traits *type_traits,
                                                       struct kefir_ast_translator_type_resolver *type_resolver,
                                                       const char *identifier,
                                                       const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                       struct kefir_ast_translator_global_scope_layout *layout,
                                                       const struct kefir_ast_translator_environment *env) {
    switch (scoped_identifier->klass) {
        case KEFIR_AST_SCOPE_IDENTIFIER_OBJECT:
            REQUIRE_OK(translate_global_scoped_identifier_object(mem, module, type_resolver,
                identifier, scoped_identifier, layout, env));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION:
            REQUIRE_OK(translate_global_scoped_identifier_function(mem, module, type_bundle, type_traits, type_resolver, scoped_identifier, env));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_ENUM_CONSTANT:
        case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_TAG:
        case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_DEFINITION:
        case KEFIR_AST_SCOPE_IDENTIFIER_LABEL:
            break;
    }
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translator_build_global_scope_layout(struct kefir_mem *mem,
                                                   struct kefir_ir_module *module,
                                                   struct kefir_ast_global_context *context,
                                                   const struct kefir_ast_translator_environment *env,
                                                   struct kefir_ast_translator_type_resolver *type_resolver,
                                                   struct kefir_ast_translator_global_scope_layout *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(module != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR module"));
    REQUIRE(context != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global context"));
    REQUIRE(env != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST translator environment"));
    REQUIRE(type_resolver != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST translator type resolver"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    struct kefir_ast_identifier_flat_scope_iterator iter;
    kefir_result_t res;
    for (res = kefir_ast_identifier_flat_scope_iter(&context->object_identifiers, &iter);
        res == KEFIR_OK;
        res = kefir_ast_identifier_flat_scope_next(&context->object_identifiers, &iter)) {
        REQUIRE_OK(translate_global_scoped_identifier(mem, module, &context->type_bundle, context->type_traits, type_resolver,
            iter.identifier, iter.value, layout, env));
    }
    REQUIRE(res == KEFIR_ITERATOR_END, res);

    for (res = kefir_ast_identifier_flat_scope_iter(&context->function_identifiers, &iter);
        res == KEFIR_OK;
        res = kefir_ast_identifier_flat_scope_next(&context->function_identifiers, &iter)) {
        REQUIRE_OK(translate_global_scoped_identifier_function(mem, module, &context->type_bundle, context->type_traits, type_resolver,
            iter.value, env));
    }
    REQUIRE(res == KEFIR_ITERATOR_END, res);
    return KEFIR_OK;
}
