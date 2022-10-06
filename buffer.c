#include <stdlib.h>
#include <string.h>
#include "info_internal.h"

struct info_internal_buffer
{
        info_char *str;
        size_t size;
        size_t cursor;
};
info_buffer info_internal_buffer_create(size_t size)
{
        info_buffer b = malloc(sizeof(struct info_internal_buffer));
        memset(b, 0, sizeof(struct info_internal_buffer));
        if(!b)
                PANIC("Malloc failed!")

        if(size)
                info_internal_buffer_reserve(b,size);
        return b;
}
bool info_internal_buffer_append(info_buffer buf, const info_char *str, size_t length)
{
        if(info_internal_buffer_reserve(buf, buf->cursor+length))
                return true;

        memcpy(buf->str+buf->cursor, str, length*sizeof(info_char));
        buf->cursor+=length;
        return false;
}
bool info_internal_buffer_consume(info_buffer buffer, info_buffer co)
{
        bool res = info_internal_buffer_append(buffer, co->str, co->cursor);
        info_internal_buffer_free(co);
        return res;
}

bool info_internal_buffer_vprintf(info_buffer buffer, const info_char *format, va_list args)
{
        if(!format)
                INTERNAL("printf format is NULL")
        int res;
        va_list arg_tmp;
#ifdef INFO_WIDE

        info_internal_buffer_reserve(buffer, 100);
        va_copy(arg_tmp, args);
        while((res=vswprintf(buffer->str+buffer->cursor, buffer->size-buffer->cursor, format, args))==-1)
        {
                va_copy(arg_tmp, args);
                info_internal_buffer_reserve(buffer, buffer->size*2);
        }

        buffer->cursor+=res;
        return false;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(buffer->str+buffer->cursor ,0, format, arg_tmp);
        va_end(arg_tmp);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length)

        info_internal_buffer_reserve(buffer, buffer->cursor+length+1);

        res = vsnprintf(buffer->str+buffer->cursor, length+1, format, args);
        buffer->cursor+=length;
        return res!=length;
#endif

}
bool info_internal_buffer_printf(info_buffer buffer, const info_char *format, ...)
{
        va_list args;
        va_start(args, format);
        bool res = info_internal_buffer_vprintf(buffer, format, args);
        va_end(args);
        return res;
}

bool info_internal_buffer_reserve(info_buffer buf, size_t size)
{
        if(size<=buf->size)
                return false;
        buf->str = realloc(buf->str, size*sizeof(info_char));
        buf->size=size;
        if(!buf->str)
                INTERNAL("realloc failed!")
        return false;
}

size_t info_internal_buffer_tell(info_buffer buffer)
{
        return buffer->cursor;
}

void info_internal_buffer_grow(info_buffer buffer, size_t size)
{
        info_internal_buffer_reserve(buffer, size);
        buffer->size=size;
}


bool info_internal_buffer_seek(info_buffer buffer, size_t pos)
{
        if(pos>buffer->size)
                INTERNAL("seek exceeds buffer length %lu<%lu", buffer->size, pos);
        buffer->cursor=pos;
        return false;
}


info_char *info_internal_buffer_str(info_buffer buffer)
{
        info_internal_buffer_reserve(buffer, buffer->cursor+1);
        buffer->str[buffer->cursor]=0;
        return buffer->str;
}
void info_internal_buffer_free(info_buffer buffer)
{
        free(buffer->str);
        free(buffer);
}
