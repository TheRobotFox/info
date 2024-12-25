#include "info_string.h"
#include "arena.h"
#include "info_internal.h"
#include <stdarg.h>
#include <stdlib.h>

info_String info_string_create(info_Arena arena, size_t cap)
{
    return (info_String){
    .data = {
        .str = info_arena_alloc(arena, cap*sizeof(info_char)),
        .len = 0
    },
    .cap = cap
    };
}

int info_string_realloc(info_Arena arena, info_String *s, size_t cap)
{
    s->data.str = info_arena_realloc(arena, s->data.str, s->cap, cap);
    s->cap=cap;
    return !s->data.str;
}

int info_string_vprintf(info_Arena arena, info_String *str, const info_char *format, va_list args)
{
        if(!format)
                INTERNAL("printf format is NULL")
        int res;
        va_list arg_tmp;

#ifdef INFO_WIDE

        size_t cap = str->cap;
        while((res=vswprintf(str->data.str+str->data.len, str->cap-str->data.len, format, args))==-1) {
            cap*=2;
            if(info_string_realloc(arena, str, cap))
                INTERNAL("Allocation Failed!")
            va_copy(arg_tmp, args);
        }
        str->data.len += res;

        /* if(info_string_realloc(&s, s.len+res)) */
        /*     INTERNAL("Allocation Failed!") */

        return 0;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(NULL ,0, format, arg_tmp)+1;
        va_end(arg_tmp);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length);

        info_string_realloc(arena, str, str->data.len+length);

        res = vsnprintf(str->data.str+str->data.len, length, format, args);
        return res!=length;
#endif

}

int info_string_printf(info_Arena arena, info_String *str, const info_char *format, ...)
{
        va_list args;
        va_start(args, format);
        int res = info_string_vprintf(arena, str, format, args);
        va_end(args);
        return res;
}
