#include "info_string.h"
#include "info_internal.h"
#include <stdlib.h>

info_String info_string_create(size_t cap)
{
    return (info_String){
        .text = malloc(cap*sizeof(info_char)),
        .cap = cap
    };
}

int info_string_realloc(info_String *s, size_t cap)
{
    s->cap=cap;
    s->text = realloc(s->text, s->cap);
    return !s->text;
}

int info_string_vprintf(info_String *str, const info_char *format, va_list args)
{
        if(!format)
                INTERNAL("printf format is NULL")
        int res;
        va_list arg_tmp;

#ifdef INFO_WIDE

        size_t cap = str->cap;
        while((res=vswprintf(str->text+str->len, str->cap-str->len, format, args))==-1) {
            cap*=2;
            if(info_string_realloc(str, cap))
                INTERNAL("Allocation Failed!")
            va_copy(arg_tmp, args);
        }
        str->len += res;

        /* if(info_string_realloc(&s, s.len+res)) */
        /*     INTERNAL("Allocation Failed!") */

        return 0;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(NULL ,0, format, arg_tmp)+1;
        va_end(arg_tmp);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length);

        info_string_realloc(str, str->len+length);

        res = vsnprintf(str->text+str->len, length, format, args);
        return res!=length;
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

void info_string_free(info_String *str)
{
    free(str->text);
    str->len = 0;
    str->cap = 0;
}
