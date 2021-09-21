/*
    SPDX-License-Identifier: GPL-3.0

    Copyright (C) 2020-2021  Jevgenijs Protopopovs

    This file is part of Kefir project.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "kefir/preprocessor/macro.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"
#include <string.h>

static kefir_result_t user_macro_argc(const struct kefir_preprocessor_macro *macro, kefir_size_t *argc_ptr) {
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));
    REQUIRE(argc_ptr != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer to argument count"));
    ASSIGN_DECL_CAST(struct kefir_preprocessor_user_macro *, user_macro, macro->payload);

    REQUIRE(macro->type == KEFIR_PREPROCESSOR_MACRO_FUNCTION,
            KEFIR_SET_ERROR(KEFIR_INVALID_REQUEST, "Unable to retrieve argument count of object macro"));
    *argc_ptr = kefir_list_length(&user_macro->parameters) + (user_macro->vararg ? 1 : 0);
    return KEFIR_OK;
}

static kefir_result_t user_macro_apply(struct kefir_mem *mem, const struct kefir_preprocessor_macro *macro,
                                       const struct kefir_list *args, struct kefir_token_buffer *buffer) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));
    REQUIRE(buffer != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid token buffer"));
    ASSIGN_DECL_CAST(struct kefir_preprocessor_user_macro *, user_macro, macro->payload);

    if (macro->type == KEFIR_PREPROCESSOR_MACRO_FUNCTION) {
        REQUIRE(args != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid macro argument list"));
        return KEFIR_SET_ERROR(KEFIR_NOT_IMPLEMENTED, "Macro substitution is not implemented yet");
    } else {
        REQUIRE(args == NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected empty macro argument list"));

        struct kefir_token token;
        for (kefir_size_t i = 0; i < user_macro->replacement.length; i++) {
            REQUIRE_OK(kefir_token_copy(mem, &token, &user_macro->replacement.tokens[i]));
            if (token.klass == KEFIR_TOKEN_IDENTIFIER && strcmp(token.identifier, macro->identifier) == 0) {
                token.preprocessor_props.skip_identifier_subst = true;
            }
            kefir_result_t res = kefir_token_buffer_emplace(mem, buffer, &token);
            REQUIRE_ELSE(res == KEFIR_OK, {
                kefir_token_free(mem, &token);
                return res;
            });
        }
    }

    return KEFIR_OK;
}

struct kefir_preprocessor_user_macro *kefir_preprocessor_user_macro_new_object(struct kefir_mem *mem,
                                                                               struct kefir_symbol_table *symbols,
                                                                               const char *identifier) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(identifier != NULL, NULL);

    if (symbols != NULL) {
        identifier = kefir_symbol_table_insert(mem, symbols, identifier, NULL);
        REQUIRE(identifier != NULL, NULL);
    }

    struct kefir_preprocessor_user_macro *macro = KEFIR_MALLOC(mem, sizeof(struct kefir_preprocessor_user_macro));
    REQUIRE(macro != NULL, NULL);

    kefir_result_t res = kefir_token_buffer_init(&macro->replacement);
    REQUIRE_ELSE(res == KEFIR_OK, {
        KEFIR_FREE(mem, macro);
        return NULL;
    });

    macro->macro.identifier = identifier;
    macro->macro.type = KEFIR_PREPROCESSOR_MACRO_OBJECT;
    macro->macro.payload = macro;
    macro->macro.argc = user_macro_argc;
    macro->macro.apply = user_macro_apply;
    return macro;
}

struct kefir_preprocessor_user_macro *kefir_preprocessor_user_macro_new_function(struct kefir_mem *mem,
                                                                                 struct kefir_symbol_table *symbols,
                                                                                 const char *identifier) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(identifier != NULL, NULL);

    if (symbols != NULL) {
        identifier = kefir_symbol_table_insert(mem, symbols, identifier, NULL);
        REQUIRE(identifier != NULL, NULL);
    }

    struct kefir_preprocessor_user_macro *macro = KEFIR_MALLOC(mem, sizeof(struct kefir_preprocessor_user_macro));
    REQUIRE(macro != NULL, NULL);

    kefir_result_t res = kefir_list_init(&macro->parameters);
    REQUIRE_ELSE(res == KEFIR_OK, {
        KEFIR_FREE(mem, macro);
        return NULL;
    });

    res = kefir_token_buffer_init(&macro->replacement);
    REQUIRE_ELSE(res == KEFIR_OK, {
        kefir_list_free(mem, &macro->parameters);
        KEFIR_FREE(mem, macro);
        return NULL;
    });

    macro->macro.identifier = identifier;
    macro->macro.type = KEFIR_PREPROCESSOR_MACRO_FUNCTION;
    macro->vararg = false;
    macro->macro.payload = macro;
    macro->macro.argc = user_macro_argc;
    macro->macro.apply = user_macro_apply;
    return macro;
}

kefir_result_t kefir_preprocessor_user_macro_free(struct kefir_mem *mem, struct kefir_preprocessor_user_macro *macro) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_token_buffer_free(mem, &macro->replacement));
    if (macro->macro.type == KEFIR_PREPROCESSOR_MACRO_FUNCTION) {
        REQUIRE_OK(kefir_list_free(mem, &macro->parameters));
    }
    macro->vararg = false;
    macro->macro.identifier = NULL;
    macro->macro.payload = NULL;
    macro->macro.argc = NULL;
    macro->macro.apply = NULL;
    KEFIR_FREE(mem, macro);
    return KEFIR_OK;
}

static kefir_result_t free_macro(struct kefir_mem *mem, struct kefir_hashtree *tree, kefir_hashtree_key_t key,
                                 kefir_hashtree_value_t value, void *payload) {
    UNUSED(tree);
    UNUSED(key);
    UNUSED(payload);
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    ASSIGN_DECL_CAST(struct kefir_preprocessor_user_macro *, macro, value);
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_preprocessor_user_macro_free(mem, macro));
    return KEFIR_OK;
}

static kefir_result_t user_macro_locate(const struct kefir_preprocessor_macro_scope *scope, const char *identifier,
                                        const struct kefir_preprocessor_macro **macro_ptr) {
    REQUIRE(scope != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro scope"));
    REQUIRE(identifier != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid identifier"));
    REQUIRE(macro_ptr != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer to preprocessor macro"));

    ASSIGN_DECL_CAST(struct kefir_preprocessor_user_macro_scope *, user_macro_scope, scope->payload);
    const struct kefir_preprocessor_user_macro *user_macro;
    REQUIRE_OK(kefir_preprocessor_user_macro_scope_at(user_macro_scope, identifier, &user_macro));
    *macro_ptr = &user_macro->macro;
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_init(const struct kefir_preprocessor_user_macro_scope *parent,
                                                        struct kefir_preprocessor_user_macro_scope *scope) {
    REQUIRE(scope != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer to preprocessor macro scope"));

    REQUIRE_OK(kefir_hashtree_init(&scope->macros, &kefir_hashtree_str_ops));
    REQUIRE_OK(kefir_hashtree_on_removal(&scope->macros, free_macro, NULL));
    scope->parent = parent;
    scope->scope.locate = user_macro_locate;
    scope->scope.payload = scope;
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_free(struct kefir_mem *mem,
                                                        struct kefir_preprocessor_user_macro_scope *scope) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(scope != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro scope"));

    REQUIRE_OK(kefir_hashtree_free(mem, &scope->macros));
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_insert(struct kefir_mem *mem,
                                                          struct kefir_preprocessor_user_macro_scope *scope,
                                                          struct kefir_preprocessor_user_macro *macro) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(scope != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro scope"));
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_hashtree_insert(mem, &scope->macros, (kefir_hashtree_key_t) macro->macro.identifier,
                                     (kefir_hashtree_value_t) macro));
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_at(const struct kefir_preprocessor_user_macro_scope *scope,
                                                      const char *identifier,
                                                      const struct kefir_preprocessor_user_macro **macro_ptr) {
    REQUIRE(scope != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro scope"));
    REQUIRE(identifier != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid dentifier"));
    REQUIRE(macro_ptr != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer to preprocessor macro"));

    struct kefir_hashtree_node *node = NULL;
    kefir_result_t res = kefir_hashtree_at(&scope->macros, (kefir_hashtree_key_t) identifier, &node);
    if (res == KEFIR_NOT_FOUND && scope->parent != NULL) {
        REQUIRE_OK(kefir_preprocessor_user_macro_scope_at(scope->parent, identifier, macro_ptr));
    } else {
        REQUIRE_OK(res);
        *macro_ptr = (const struct kefir_preprocessor_user_macro *) node->value;
    }
    return KEFIR_OK;
}
