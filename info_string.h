#ifndef INFO_STRING_H_
#define INFO_STRING_H_

#include "arena.h"
#include "info_char.h"
#include <stdio.h>

struct info_Slice {
    info_char *str;
    size_t len;
};
typedef struct info_String
{
    struct info_Slice data;
    size_t cap;
} info_String;

info_String info_string_create(info_Arena arena, size_t cap);
int info_string_vprintf(info_Arena arena, info_String *str, const info_char *format, va_list args);
int info_string_printf(info_Arena arena, info_String *str, const info_char *format, ...);


#endif // INFO_STRING_H_
