#pragma once
#include "List/List.h"

#include "info_internal.h"
enum info_DrawCall_Type
{
        FCOLOR,
        BCOLOR,
        STYLE,
        TEXT
};
struct info_DrawCall
{
        enum info_DrawCall_Type type;
        union{
                struct info_color color;

                const char *text;
        }
};


bool info_drawcall_vprintf(drawcall_list l, ANSI ansi, const info_char *format, va_list args);
List info_buffer_create(size_t size);
info_char* info_buffer_str(List buffer);
bool info_drawcall_printf(List drawcall_list, ANSI ansi, const info_char *format, ...);
bool info_buffer_printf(List buffer, const info_char *format, ...);
bool info_buffer_vprintf(List buffer, const info_char *format, va_list args);

drawcall_list info_drawcall_list_create();
drawcall_iterator info_drawcall_iterator_start(drawcall_list d);
drawcall_iterator info_drawcall_iterator_end(drawcall_list d);
bool info_drawcall_iterator_advance(drawcall_iterator it);
info_char* info_drawcall_iterator_get(drawcall_iterator it);
void info_drawcall_iterator_free(drawcall_iterator it);
void info_drawcall_list_free(drawcall_list l);
bool info_drawcall_list_copy(drawcall_list out, drawcall_iterator start, drawcall_iterator end);

