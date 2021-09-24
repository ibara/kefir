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

#ifndef KEFIR_PREPROCESSOR_PREPROCESSOR_H_
#define KEFIR_PREPROCESSOR_PREPROCESSOR_H_

#include "kefir/lexer/lexer.h"
#include "kefir/preprocessor/macro.h"
#include "kefir/preprocessor/source_file.h"
#include "kefir/preprocessor/directives.h"

typedef struct kefir_preprocessor_context {
    const struct kefir_preprocessor_macro_scope *macros;
    struct kefir_preprocessor_user_macro_scope user_macros;
    const struct kefir_preprocessor_source_locator *source_locator;
} kefir_preprocessor_context_t;

kefir_result_t kefir_preprocessor_context_init(struct kefir_preprocessor_context *,
                                               const struct kefir_preprocessor_source_locator *);
kefir_result_t kefir_preprocessor_context_free(struct kefir_mem *, struct kefir_preprocessor_context *);

typedef struct kefir_preprocessor {
    struct kefir_lexer lexer;
    struct kefir_preprocessor_context *context;
    struct kefir_preprocessor_directive_scanner directive_scanner;
} kefir_preprocessor_t;

kefir_result_t kefir_preprocessor_init(struct kefir_mem *, struct kefir_preprocessor *, struct kefir_symbol_table *,
                                       struct kefir_lexer_source_cursor *, const struct kefir_lexer_context *,
                                       struct kefir_preprocessor_context *);
kefir_result_t kefir_preprocessor_free(struct kefir_mem *, struct kefir_preprocessor *);

kefir_result_t kefir_preprocessor_skip_group(struct kefir_mem *, struct kefir_preprocessor *);
kefir_result_t kefir_preprocessor_run_group(struct kefir_mem *, struct kefir_preprocessor *,
                                            struct kefir_token_buffer *);
kefir_result_t kefir_preprocessor_run_substitutions(struct kefir_mem *, struct kefir_preprocessor *,
                                                    struct kefir_token_buffer *);
kefir_result_t kefir_preprocessor_run(struct kefir_mem *, struct kefir_preprocessor *, struct kefir_token_buffer *);

kefir_result_t kefir_preprocessor_token_convert(struct kefir_mem *, struct kefir_preprocessor *, struct kefir_token *,
                                                const struct kefir_token *);
kefir_result_t kefir_preprocessor_token_convert_buffer(struct kefir_mem *, struct kefir_preprocessor *,
                                                       struct kefir_token_buffer *, const struct kefir_token_buffer *);

#endif
