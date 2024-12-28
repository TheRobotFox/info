#ifndef INFO_STRING_H_
#define INFO_STRING_H_

#include "info_char.h"
#include <stdio.h>

struct info_Slice {
    const info_char *str;
    size_t len;
};
typedef struct info_String
{
    info_char *str;
    size_t len, cap;
} info_String;

info_String info_string_create(size_t cap);
int info_string_vprintf(info_String *str, const info_char *format, va_list args);
int info_string_printf(info_String *str, const info_char *format, ...);
int info_string_puts(info_String *str, const info_char *s, size_t len);
size_t info_string_length(info_String *str);


#endif // INFO_STRING_H_
