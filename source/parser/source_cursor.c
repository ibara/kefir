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

#include "kefir/parser/source_cursor.h"
#include "kefir/core/util.h"
#include "kefir/core/error.h"

kefir_result_t kefir_lexer_source_cursor_init(struct kefir_lexer_source_cursor *cursor, const char *content,
                                              kefir_size_t length, const char *source_id) {
    REQUIRE(cursor != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid lexer source cursor"));
    REQUIRE(content != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid content"));
    REQUIRE(source_id != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid source identifier"));

    *cursor = (struct kefir_lexer_source_cursor){0};
    cursor->content = content;
    cursor->length = length;
    REQUIRE_OK(kefir_source_location_init(&cursor->location, source_id, 1, 1));
    return KEFIR_OK;
}

kefir_char32_t kefir_lexer_source_cursor_at(const struct kefir_lexer_source_cursor *cursor, kefir_size_t count) {
    REQUIRE(cursor != NULL, U'\0');

    kefir_char32_t character = U'\0';
    kefir_size_t index = cursor->index;
    mbstate_t mbstate = cursor->mbstate;
    do {
        size_t rc = mbrtoc32(&character, cursor->content + index, cursor->length - index, &mbstate);
        switch (rc) {
            case 0:
            case (size_t) -1:
            case (size_t) -2:
            case (size_t) -3:
                character = U'\0';
                count = 0;
                break;

            default:
                index += rc;
                break;
        }
    } while (count--);
    return character;
}

kefir_result_t kefir_lexer_source_cursor_next(struct kefir_lexer_source_cursor *cursor, kefir_size_t count) {
    REQUIRE(cursor != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid lexer source cursor"));
    while (count--) {
        kefir_char32_t chr;
        size_t rc = mbrtoc32(&chr, cursor->content + cursor->index, cursor->length - cursor->index, &cursor->mbstate);
        switch (rc) {
            case (size_t) -1:
            case (size_t) -2:
            case (size_t) -3:
                return KEFIR_SET_ERROR(KEFIR_MALFORMED_ARG, "Invalid lexer source cursor input");

            case 0:
                return KEFIR_OK;

            default:
                cursor->index += rc;
                if (chr == U'\n') {
                    cursor->location.column = 1;
                    cursor->location.line++;
                } else {
                    cursor->location.column++;
                }
                break;
        }
    }
    return KEFIR_OK;
}

kefir_result_t kefir_lexer_source_cursor_save(const struct kefir_lexer_source_cursor *cursor,
                                              struct kefir_lexer_source_cursor_state *state) {
    REQUIRE(cursor != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid lexer source cursor"));
    REQUIRE(state != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer lexer source cursor state"));

    state->index = cursor->index;
    state->mbstate = cursor->mbstate;
    state->location = cursor->location;
    return KEFIR_OK;
}

kefir_result_t kefir_lexer_source_cursor_restore(struct kefir_lexer_source_cursor *cursor,
                                                 const struct kefir_lexer_source_cursor_state *state) {
    REQUIRE(cursor != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid lexer source cursor"));
    REQUIRE(state != NULL,
            KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid pointer lexer source cursor state"));

    cursor->index = state->index;
    cursor->mbstate = state->mbstate;
    cursor->location = state->location;
    return KEFIR_OK;
}

const char *kefir_lexer_source_cursor_begin(struct kefir_lexer_source_cursor *cursor) {
    REQUIRE(cursor != NULL, NULL);
    return cursor->content;
}

const char *kefir_lexer_source_cursor_current(struct kefir_lexer_source_cursor *cursor) {
    REQUIRE(cursor != NULL, NULL);
    return cursor->content + cursor->index;
}

const char *kefir_lexer_source_cursor_end(struct kefir_lexer_source_cursor *cursor) {
    REQUIRE(cursor != NULL, NULL);
    return cursor->content + cursor->length;
}

kefir_result_t kefir_lexer_cursor_match_string(const struct kefir_lexer_source_cursor *cursor,
                                               const kefir_char32_t *string) {
    REQUIRE(cursor != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid lexer source cursor"));
    REQUIRE(string != NULL, KEFIR_SET_ERROR(KEFIR_INVALID_PARAMETER, "Expected valid string"));

    for (kefir_size_t index = 0; string[index] != '\0'; index++) {
        if (kefir_lexer_source_cursor_at(cursor, index) != string[index]) {
            return KEFIR_SET_ERROR(KEFIR_NO_MATCH, "Cannot match provided string");
        }
    }
    return KEFIR_OK;
}
