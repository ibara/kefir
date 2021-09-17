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

struct kefir_preprocessor_macro *kefir_preprocessor_macro_new(struct kefir_mem *mem, struct kefir_symbol_table *symbols,
                                                              const char *identifier) {
    REQUIRE(mem != NULL, NULL);
    REQUIRE(identifier != NULL, NULL);

    if (symbols != NULL) {
        identifier = kefir_symbol_table_insert(mem, symbols, identifier, NULL);
        REQUIRE(identifier != NULL, NULL);
    }

    struct kefir_preprocessor_macro *macro = KEFIR_MALLOC(mem, sizeof(struct kefir_preprocessor_macro));
    REQUIRE(macro != NULL, NULL);

    kefir_result_t res = kefir_list_init(&macro->parameters);
    REQUIRE_ELSE(res == KEFIR_OK, {
        KEFIR_FREE(mem, macro);
        return NULL;
    });
    res = kefir_token_buffer_init(mem, &macro->replacement);
    REQUIRE_ELSE(res == KEFIR_OK, {
        kefir_list_free(mem, &macro->parameters);
        KEFIR_FREE(mem, macro);
        return NULL;
    });

    macro->identifier = identifier;
    macro->vararg = false;
    return macro;
}

kefir_result_t kefir_preprocessor_macro_free(struct kefir_mem *mem, struct kefir_preprocessor_macro *macro) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_token_buffer_free(mem, &macro->replacement));
    REQUIRE_OK(kefir_list_free(mem, &macro->parameters));
    macro->vararg = false;
    macro->identifier = NULL;
    KEFIR_FREE(mem, macro);
    return KEFIR_OK;
}

static kefir_result_t free_macro(struct kefir_mem *mem, struct kefir_hashtree *tree, kefir_hashtree_key_t key,
                                 kefir_hashtree_value_t value, void *payload) {
    UNUSED(tree);
    UNUSED(key);
    UNUSED(payload);
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    ASSIGN_DECL_CAST(struct kefir_preprocessor_macro *, macro, value);
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_preprocessor_macro_free(mem, macro));
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_init(const struct kefir_preprocessor_user_macro_scope *parent,
                                                        struct kefir_preprocessor_user_macro_scope *scope) {
    REQUIRE(scope != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer to preprocessor macro scope"));

    REQUIRE_OK(kefir_hashtree_init(&scope->macros, &kefir_hashtree_str_ops));
    REQUIRE_OK(kefir_hashtree_on_removal(&scope->macros, free_macro, NULL));
    scope->parent = parent;
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
                                                          struct kefir_preprocessor_macro *macro) {
    REQUIRE(mem != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid memory allocator"));
    REQUIRE(scope != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro scope"));
    REQUIRE(macro != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid preprocessor macro"));

    REQUIRE_OK(kefir_hashtree_insert(mem, &scope->macros, (kefir_hashtree_key_t) macro->identifier,
                                     (kefir_hashtree_value_t) macro));
    return KEFIR_OK;
}

kefir_result_t kefir_preprocessor_user_macro_scope_at(const struct kefir_preprocessor_user_macro_scope *scope,
                                                      const char *identifier,
                                                      const struct kefir_preprocessor_macro **macro_ptr) {
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
        *macro_ptr = (const struct kefir_preprocessor_macro *) node->value;
    }
    return KEFIR_OK;
}
