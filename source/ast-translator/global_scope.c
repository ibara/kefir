#include "kefir/ast-translator/global_scope.h"
#include "kefir/ast-translator/translator.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

static kefir_result_t on_entry_removal(struct kefir_mem *mem,
                              struct kefir_hashtree *tree,
                              kefir_hashtree_key_t key,
                              kefir_hashtree_value_t value,
                              void *paylod) {
    UNUSED(tree);
    UNUSED(key);
    UNUSED(paylod);
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INTERNAL_ERROR, "Expected valid memory allocator"));
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier,
        value);
    REQUIRE(scoped_identifier != NULL, KEFIR_SET_ERROR(KEFIR_INTERNAL_ERROR, "Expected valid hash tree value"));
    if (scoped_identifier != NULL) {
        if (scoped_identifier->layout != NULL) {
            REQUIRE_OK(kefir_ast_type_layout_free(mem, scoped_identifier->layout));
        }
        scoped_identifier->type = NULL;
        scoped_identifier->type_id = 0;
        scoped_identifier->layout = NULL;
    }
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translator_global_scope_init(struct kefir_mem *mem,
                                              struct kefir_ir_module *module,
                                              struct kefir_ast_translator_global_scope *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(module != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR module"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    REQUIRE_OK(kefir_hashtree_init(&layout->external_objects, &kefir_hashtree_str_ops));
    REQUIRE_OK(kefir_hashtree_init(&layout->external_thread_local_objects, &kefir_hashtree_str_ops));
    REQUIRE_OK(kefir_hashtree_init(&layout->static_objects, &kefir_hashtree_str_ops));
    REQUIRE_OK(kefir_hashtree_init(&layout->static_thread_local_objects, &kefir_hashtree_str_ops));
    
    REQUIRE_OK(kefir_hashtree_on_removal(&layout->external_objects, on_entry_removal, NULL));
    REQUIRE_OK(kefir_hashtree_on_removal(&layout->external_thread_local_objects, on_entry_removal, NULL));
    REQUIRE_OK(kefir_hashtree_on_removal(&layout->static_objects, on_entry_removal, NULL));
    REQUIRE_OK(kefir_hashtree_on_removal(&layout->static_thread_local_objects, on_entry_removal, NULL));

    layout->static_layout = kefir_ir_module_new_type(mem, module, 0, &layout->static_layout_id);
    REQUIRE(layout->static_layout != NULL,
        KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate new IR type"));
    layout->static_thread_local_layout = kefir_ir_module_new_type(mem, module, 0, &layout->static_thread_local_layout_id);
    REQUIRE(layout->static_thread_local_layout != NULL,
        KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Failed to allocate new IR type"));
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translator_global_scope_free(struct kefir_mem *mem,
                                              struct kefir_ast_translator_global_scope *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    REQUIRE_OK(kefir_hashtree_free(mem, &layout->external_objects));
    REQUIRE_OK(kefir_hashtree_free(mem, &layout->external_thread_local_objects));
    REQUIRE_OK(kefir_hashtree_free(mem, &layout->static_objects));
    REQUIRE_OK(kefir_hashtree_free(mem, &layout->static_thread_local_objects));
    layout->static_layout = NULL;
    layout->static_thread_local_layout = NULL;
    return KEFIR_OK;
}

static kefir_result_t translate_scoped_identifier_type(struct kefir_mem *mem,
                                                     struct kefir_ir_module *module,
                                                     const struct kefir_ast_translator_environment *env,
                                                     const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                     struct kefir_ir_type **type_ptr) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    scoped_identifier_layout->type = kefir_ir_module_new_type(mem, module, 0, &scoped_identifier_layout->type_id);
    REQUIRE(scoped_identifier_layout->type != NULL, KEFIR_SET_ERROR(KEFIR_UNKNOWN_ERROR, "Unable to allocate new IR type"));
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, scoped_identifier_layout->type));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));
    *type_ptr = scoped_identifier_layout->type;
    return KEFIR_OK;
}

static kefir_result_t translate_extern_identifier(struct kefir_mem *mem,
                                                struct kefir_ir_module *module,
                                                const struct kefir_ast_translator_environment *env,
                                                struct kefir_ast_translator_global_scope *layout,
                                                const char *identifier,
                                                const struct kefir_ast_scoped_identifier *scoped_identifier) {      
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);  
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(kefir_hashtree_insert(mem, &layout->external_objects,
        (kefir_hashtree_key_t) identifier, (kefir_hashtree_value_t) scoped_identifier_layout));
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, env, scoped_identifier, &type));
    scoped_identifier_layout->external = scoped_identifier->object.external;
    return KEFIR_OK;
}

static kefir_result_t translate_extern_thread_local_identifier(struct kefir_mem *mem,
                                                             struct kefir_ir_module *module,
                                                             const struct kefir_ast_translator_environment *env,
                                                             struct kefir_ast_translator_global_scope *layout,
                                                             const char *identifier,
                                                             const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(kefir_hashtree_insert(mem, &layout->external_thread_local_objects,
        (kefir_hashtree_key_t) identifier, (kefir_hashtree_value_t) scoped_identifier_layout));
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, env, scoped_identifier, &type));
    scoped_identifier_layout->external = scoped_identifier->object.external;
    return KEFIR_OK;
}

static kefir_result_t translate_thread_local_identifier(struct kefir_mem *mem,
                                                      struct kefir_ir_module *module,
                                                      const struct kefir_ast_translator_environment *env,
                                                      struct kefir_ast_translator_global_scope *layout,
                                                      const char *identifier,
                                                      const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    struct kefir_ir_type *type = NULL;
    REQUIRE_OK(kefir_hashtree_insert(mem, &layout->external_thread_local_objects,
        (kefir_hashtree_key_t) identifier, (kefir_hashtree_value_t) scoped_identifier_layout));
    REQUIRE_OK(translate_scoped_identifier_type(mem, module, env, scoped_identifier, &type));
    scoped_identifier_layout->external = false;
    return KEFIR_OK;
}

static kefir_result_t translate_static_identifier(struct kefir_mem *mem,
                                                const struct kefir_ast_translator_environment *env,
                                                struct kefir_ast_translator_global_scope *layout,
                                                const char *identifier,
                                                const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_hashtree_insert(mem, &layout->static_objects,
        (kefir_hashtree_key_t) identifier, (kefir_hashtree_value_t) scoped_identifier_layout));
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, layout->static_layout));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));
    scoped_identifier_layout->type_id = layout->static_layout_id;
    scoped_identifier_layout->type = layout->static_layout;
    scoped_identifier_layout->external = false;
    return KEFIR_OK;                             
}

static kefir_result_t translate_static_thread_local_identifier(struct kefir_mem *mem,
                                                             const struct kefir_ast_translator_environment *env,
                                                             struct kefir_ast_translator_global_scope *layout,
                                                             const char *identifier,
                                                             const struct kefir_ast_scoped_identifier *scoped_identifier) {
    ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, scoped_identifier_layout,
        scoped_identifier->payload.ptr);
    struct kefir_irbuilder_type builder;
    REQUIRE_OK(kefir_hashtree_insert(mem, &layout->static_thread_local_objects,
        (kefir_hashtree_key_t) identifier, (kefir_hashtree_value_t) scoped_identifier_layout));
    REQUIRE_OK(kefir_irbuilder_type_init(mem, &builder, layout->static_thread_local_layout));
    REQUIRE_OK(kefir_ast_translate_object_type(mem, scoped_identifier->object.type,
        scoped_identifier->object.alignment->value, env, &builder, &scoped_identifier_layout->layout));
    REQUIRE_OK(KEFIR_IRBUILDER_TYPE_FREE(&builder));
    scoped_identifier_layout->type_id = layout->static_thread_local_layout_id;
    scoped_identifier_layout->type = layout->static_thread_local_layout;
    scoped_identifier_layout->external = false;
    return KEFIR_OK;
}

static kefir_result_t translate_global_scoped_identifier(struct kefir_mem *mem,
                                                       struct kefir_ir_module *module,
                                                       const char *identifier,
                                                       const struct kefir_ast_scoped_identifier *scoped_identifier,
                                                       struct kefir_ast_translator_global_scope *layout,
                                                       const struct kefir_ast_translator_environment *env) {
    REQUIRE(scoped_identifier->klass == KEFIR_AST_SCOPE_IDENTIFIER_OBJECT, KEFIR_OK);
    switch (scoped_identifier->object.storage) {
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN:
            REQUIRE_OK(translate_extern_identifier(mem, module, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN_THREAD_LOCAL:
            REQUIRE_OK(translate_extern_thread_local_identifier(mem, module, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_THREAD_LOCAL:
            REQUIRE_OK(translate_thread_local_identifier(mem, module, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC:
            REQUIRE_OK(translate_static_identifier(mem, env, layout, identifier, scoped_identifier));
            break;
        
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC_THREAD_LOCAL:
            REQUIRE_OK(translate_static_thread_local_identifier(mem, env, layout, identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_AUTO:
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_REGISTER:
            return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "File-scope variable cannot have auto/register storage");
    }
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translate_global_scope(struct kefir_mem *mem,
                                                   struct kefir_ir_module *module,
                                                   const struct kefir_ast_global_context *context,
                                                   const struct kefir_ast_translator_environment *env,
                                                   struct kefir_ast_translator_global_scope *layout) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(module != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR module"));
    REQUIRE(context != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global context"));
    REQUIRE(env != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST translator environment"));
    REQUIRE(layout != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST global scope layout"));
    struct kefir_ast_identifier_flat_scope_iterator iter;
    kefir_result_t res;
    for (res = kefir_ast_identifier_flat_scope_iter(&context->object_identifiers, &iter);
        res == KEFIR_OK;
        res = kefir_ast_identifier_flat_scope_next(&context->object_identifiers, &iter)) {
        REQUIRE_OK(translate_global_scoped_identifier(mem, module, iter.identifier, iter.value, layout, env));
    }
    REQUIRE(res == KEFIR_ITERATOR_END, res);
    return KEFIR_OK;
}