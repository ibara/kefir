#include <string.h>
#include "kefir/ast-translator/translator_impl.h"
#include "kefir/ast-translator/translator.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"
#include "kefir/ir/module.h"
#include "kefir/ast-translator/scope.h"

static kefir_result_t translate_object_identifier(struct kefir_mem *mem,
                                                const struct kefir_ast_translator_context *context,
                                                struct kefir_irbuilder_block *builder,
                                                const char *identifier,
                                                const struct kefir_ast_scoped_identifier *scoped_identifier) {
    switch (scoped_identifier->object.storage) {
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN: {
            kefir_id_t id;
            REQUIRE(kefir_ir_module_symbol(mem, context->module, identifier, &id) != NULL,
                 KEFIR_SET_ERROR(KEFIR_MEMALLOC_FAILURE, "Failed to allocate IR module symbol"));
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_GETGLOBAL,
                id));
        } break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC: {
            ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, identifier_data,
                scoped_identifier->payload.ptr);
            kefir_id_t id;
            REQUIRE(kefir_ir_module_symbol(mem, context->module,
                KEFIR_AST_TRANSLATOR_STATIC_VARIABLES_IDENTIFIER, &id) != NULL,
                KEFIR_SET_ERROR(KEFIR_MEMALLOC_FAILURE, "Failed to allocate IR module symbol"));
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_GETGLOBAL,
                id));
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU32(builder, KEFIR_IROPCODE_ELEMENTPTR,
                identifier_data->type_id, identifier_data->layout->value));

        } break;

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_THREAD_LOCAL:
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_EXTERN_THREAD_LOCAL:
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_STATIC_THREAD_LOCAL:
            return KEFIR_SET_ERROR(KEFIR_NOT_IMPLEMENTED, "Thread_local variable support is not implemented yet");

        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_AUTO:
        case KEFIR_AST_SCOPE_IDENTIFIER_STORAGE_REGISTER: {
            ASSIGN_DECL_CAST(struct kefir_ast_translator_scoped_identifier *, identifier_data,
                scoped_identifier->payload.ptr);
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_GETLOCALS, 0));
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU32(builder, KEFIR_IROPCODE_ELEMENTPTR,
                identifier_data->type_id, identifier_data->layout->value));
        } break;
    }

    const struct kefir_ast_type *unqualified = kefir_ast_unqualified_type(scoped_identifier->object.type);
    if (unqualified->tag == KEFIR_AST_TYPE_ENUMERATION) {
        unqualified = kefir_ast_unqualified_type(unqualified->enumeration_type.underlying_type);
    }

    switch (unqualified->tag) {
        case KEFIR_AST_TYPE_VOID:
            return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Cannot load variable with void type");
            
        case KEFIR_AST_TYPE_SCALAR_BOOL:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD8U, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_CHAR:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD8U, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_CHAR:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD8U, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_SIGNED_CHAR:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD8I, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_SHORT:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD16U, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_SIGNED_SHORT:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD16I, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_INT:
        case KEFIR_AST_TYPE_SCALAR_FLOAT:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD32U, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_SIGNED_INT:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD32I, 0));
            break;
            
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG_LONG:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_LONG:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_LONG_LONG:
        case KEFIR_AST_TYPE_SCALAR_DOUBLE:
        case KEFIR_AST_TYPE_SCALAR_POINTER:
            REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_LOAD64, 0));
            break;
            
        case KEFIR_AST_TYPE_ENUMERATION:
            return KEFIR_SET_ERROR(KEFIR_INTERNAL_ERROR, "Unexpected enumeration type");
            
        case KEFIR_AST_TYPE_STRUCTURE:
        case KEFIR_AST_TYPE_UNION:
        case KEFIR_AST_TYPE_ARRAY:
        case KEFIR_AST_TYPE_FUNCTION:
            // Intentionally left blank
            break;
            
        case KEFIR_AST_TYPE_QUALIFIED:
            return KEFIR_SET_ERROR(KEFIR_INTERNAL_ERROR, "Unexpected qualified type");
            
    }
    return KEFIR_OK;
}

static kefir_result_t translate_enum_constant(struct kefir_irbuilder_block *builder,
                                            const struct kefir_ast_scoped_identifier *scoped_identifier) {
    if (KEFIR_AST_TYPE_IS_UNSIGNED_INTEGER(scoped_identifier->enum_constant.type)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_PUSHU64,
            (kefir_uint64_t) scoped_identifier->enum_constant.value->value.integer));
    } else if (KEFIR_AST_TYPE_IS_SIGNED_INTEGER(scoped_identifier->enum_constant.type)) {
        REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_PUSHI64,
            (kefir_int64_t) scoped_identifier->enum_constant.value->value.integer));
    } else {
        return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Enum constant cannon have non-integral type");
    }
    return KEFIR_OK;
}

static kefir_result_t translate_function_identifier(struct kefir_mem *mem,
                                                  const struct kefir_ast_translator_context *context,
                                                  struct kefir_irbuilder_block *builder,
                                                  const char *identifier) {
    REQUIRE(identifier != NULL && strlen(identifier) > 0,
        KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid function identifier"));
    kefir_id_t id;
    REQUIRE(kefir_ir_module_symbol(mem, context->module, identifier, &id) != NULL,
        KEFIR_SET_ERROR(KEFIR_MEMALLOC_FAILURE, "Failed to allocate IR module symbol"));
    REQUIRE_OK(KEFIR_IRBUILDER_BLOCK_APPENDU64(builder, KEFIR_IROPCODE_GETGLOBAL,
        id));
    return KEFIR_OK;
}

kefir_result_t kefir_ast_translate_identifier_node(struct kefir_mem *mem,
                                               const struct kefir_ast_translator_context *context,
                                               struct kefir_irbuilder_block *builder,
                                               const struct kefir_ast_identifier *node) {
    UNUSED(mem);
    REQUIRE(context != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST translation context"));
    REQUIRE(builder != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid IR block builder"));
    REQUIRE(node != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST identifier node"));

    const struct kefir_ast_scoped_identifier *scoped_identifier = NULL;
    REQUIRE_OK(context->ast_context->resolve_ordinary_identifier(context->ast_context, node->identifier,
        &scoped_identifier));
    switch (scoped_identifier->klass) {
        case KEFIR_AST_SCOPE_IDENTIFIER_OBJECT:
            REQUIRE_OK(translate_object_identifier(mem, context, builder, node->identifier, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_ENUM_CONSTANT:
            REQUIRE_OK(translate_enum_constant(builder, scoped_identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_FUNCTION:
            REQUIRE_OK(translate_function_identifier(mem, context, builder,
                scoped_identifier->function.type->function_type.identifier));
            break;

        case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_TAG:
        case KEFIR_AST_SCOPE_IDENTIFIER_TYPE_DEFINITION:
            return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Cannot translate type information");

        case KEFIR_AST_SCOPE_IDENTIFIER_LABEL:
            return KEFIR_SET_ERROR(KEFIR_NOT_IMPLEMENTED, "AST label identifiers are not implemented yet");

    }
    return KEFIR_OK;
}