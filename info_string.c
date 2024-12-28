#include "info_string.h"
#include "info_char.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

info_String info_string_create(size_t cap)
{
    return (info_String){
    .str = malloc(cap*sizeof(info_char)),
    .len = 0,
    .cap = cap
    };
}

int info_string_realloc(info_String *s, size_t cap)
{
    s->str = realloc(s->str, cap);
    s->cap=cap;
    return !s->str;
}

int info_string_vprintf(info_String *str, const info_char *format, va_list args)
{
        if(!format)
            return -1;
        int res;
        va_list arg_tmp;

#ifdef INFO_WIDE

        size_t cap = str->cap;
        while((res=vswprintf(str->str+str->len, str->cap-str->len, format, args))==-1) {
            cap*=2;
            if(info_string_realloc(str, cap))
                return -1;
            va_copy(arg_tmp, args);
        }
        str->len += res;

        /* if(info_string_realloc(&s, s.len+res)) */
        /*     INTERNAL("Allocation Failed!") */

        return res;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(NULL ,0, format, arg_tmp);
        va_end(arg_tmp);
        if(length<0)
            return -1;

        info_string_realloc(str, str->len+length+1);

        res = vsnprintf(str->str+str->len, length+1, format, args);
        str->len += length;
        return res;
#endif

}

int info_string_printf(info_String *str, const info_char *format, ...)
{
        va_list args;
        va_start(args, format);
        int res = info_string_vprintf(str, format, args);
        va_end(args);
        return res;
}

int info_string_puts(info_String *str, const info_char *s, size_t len)
{
    if(str->len+len >= str->cap) info_string_realloc(str, str->len+len+1);
    memcpy(str->str+str->len*sizeof(info_char), s, len*sizeof(info_char));
    str->len+=len;
    str->str[str->len] = 0;
    return 0;

}

size_t info_string_length(info_String *str)
{
    return str->len;
}
