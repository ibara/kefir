#include "kefir/ast/type.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

static kefir_result_t free_qualified_type(struct kefir_mem *mem, const struct kefir_ast_type *type) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid memory allocator"));
    REQUIRE(type != NULL, KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Expected valid AST type"));
    KEFIR_FREE(mem, (void *) type);
    return KEFIR_OK;
}

static kefir_bool_t same_qualified_type(const struct kefir_ast_type *type1, const struct kefir_ast_type *type2) {
    REQUIRE(type1 != NULL, false);
    REQUIRE(type2 != NULL, false);
    REQUIRE(type1->tag == KEFIR_AST_TYPE_QUALIFIED &&
        type2->tag == KEFIR_AST_TYPE_QUALIFIED, false);
    return type1->qualified_type.qualification.constant == type2->qualified_type.qualification.constant &&
        type1->qualified_type.qualification.restricted == type2->qualified_type.qualification.restricted &&
        type1->qualified_type.qualification.volatile_type == type2->qualified_type.qualification.volatile_type &&
        KEFIR_AST_TYPE_SAME(type1->qualified_type.type, type2->qualified_type.type);
}

const struct kefir_ast_type *kefir_ast_type_qualified(struct kefir_mem *mem,
                                                 struct kefir_ast_type_storage *type_storage,
                                                 const struct kefir_ast_type *base_type,
                                                 struct kefir_ast_type_qualification qualification) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(base_type != NULL, NULL);
    struct kefir_ast_type *type = KEFIR_MALLOC(mem, sizeof(struct kefir_ast_type));
    REQUIRE(type != NULL, NULL);
    if (type_storage != NULL) {
        kefir_result_t res = kefir_list_insert_after(mem, &type_storage->types, kefir_list_tail(&type_storage->types), type);
        REQUIRE_ELSE(res == KEFIR_OK, {
            KEFIR_FREE(mem, type);
            return NULL;
        });
    }
    if (base_type->tag == KEFIR_AST_TYPE_QUALIFIED) {
        qualification.constant = qualification.constant || base_type->qualified_type.qualification.constant;
        qualification.restricted = qualification.restricted || base_type->qualified_type.qualification.restricted;
        qualification.volatile_type = qualification.volatile_type || base_type->qualified_type.qualification.volatile_type;
        base_type = base_type->qualified_type.type;
    }
    type->tag = KEFIR_AST_TYPE_QUALIFIED;
    type->basic = false;
    type->ops.same = same_qualified_type;
    type->ops.free = free_qualified_type;
    type->qualified_type.qualification = qualification;
    type->qualified_type.type = base_type;
    return type;
}

const struct kefir_ast_type *kefir_ast_unqualified_type(const struct kefir_ast_type *type) {
    switch (type->tag) {
        case KEFIR_AST_TYPE_VOID:
        case KEFIR_AST_TYPE_SCALAR_BOOL:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_CHAR:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_CHAR:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_SHORT:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_SHORT:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_INT:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_INT:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_LONG:
        case KEFIR_AST_TYPE_SCALAR_UNSIGNED_LONG_LONG:
        case KEFIR_AST_TYPE_SCALAR_SIGNED_LONG_LONG:
        case KEFIR_AST_TYPE_SCALAR_FLOAT:
        case KEFIR_AST_TYPE_SCALAR_DOUBLE:
        case KEFIR_AST_TYPE_SCALAR_POINTER:
        case KEFIR_AST_TYPE_ENUMERATION:
        case KEFIR_AST_TYPE_STRUCTURE:
        case KEFIR_AST_TYPE_UNION:
        case KEFIR_AST_TYPE_ARRAY:
        case KEFIR_AST_TYPE_FUNCTION:
            return type;

        case KEFIR_AST_TYPE_QUALIFIED:
            return type->qualified_type.type;
    }
    return NULL;
}