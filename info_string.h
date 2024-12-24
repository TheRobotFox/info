#ifndef INFO_STRING_H_
#define INFO_STRING_H_

#include "info_char.h"
#include <stdio.h>

typedef struct info_String
{
    info_char *text;
    size_t cap, len;
} info_String;

info_String info_string_create(size_t cap);
int info_string_vprintf(info_String *str, const info_char *format, va_list args);
int info_string_printf(info_String *str, const info_char *format, ...);


#endif // INFO_STRING_H_
