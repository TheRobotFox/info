#pragma once
#include "info_internal.h"

enum info_internal_drawcall_content_stream
{
        CONTENT_TEXT,
        CONTENT_ANSI
};

typedef List drawcall_list;
typedef struct drawcall_iterator *drawcall_iterator;

bool info_drawcall_vprintf(drawcall_list l, const info_char *format, va_list args);
bool info_drawcall_printf(List drawcall_list, enum info_internal_drawcall_content_stream stream, ANSI ansi, const info_char *format, ...);

drawcall_iterator info_drawcall_iterator_create(drawcall_list d, enum info_internal_drawcall_content_stream stream);
bool info_drawcall_iterator_advance(drawcall_iterator it);
void* info_drawcall_iterator_get(drawcall_iterator it);
