#include "buffer.h"
#include <stdlib.h>
#include <string.h>
#include "info_internal.h"

struct info_internal_buffer
{
        char *str;
        size_t size;
        size_t cursor;
};
info_buffer info_internal_buffer_create(size_t size)
{
        info_buffer b = malloc(sizeof(struct info_internal_buffer));
        if(!b)
                PANIC("Malloc failed!")

        if(size)
                info_internal_buffer_reserve(b,size);
        return b;
}
bool info_internal_buffer_append(info_buffer buf, const char *str, size_t length)
{
        if(info_internal_buffer_reserve(buf, buf->cursor+length))
                return true;

        memcpy(buf->str+buf->cursor, str, length);
        return false;
}
bool info_internal_buffer_cat(info_buffer buffer, info_buffer co)
{
        bool res = info_internal_buffer_append(buffer, co->str, co->cursor);
        info_internal_buffer_free(co);
        return res;
}

bool info_internal_buffer_vprintf(info_buffer buffer, const char *format, va_list args)
{
        int length = vsnprintf(NULL,0, format, args);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length)

        info_internal_buffer_reserve(buffer, buffer->cursor+length);

        return vsprintf(buffer->str, format, args)!=length;
}
bool info_internal_buffer_printf(info_buffer buffer, const char *format, ...)
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
        buf->str = realloc(buf->str, size);
        if(!buf->str)
                INTERNAL("realloc failed!")
        return false;
}

size_t info_internal_buffer_tell(info_buffer buffer)
{
        return buffer->cursor;
}

bool info_internal_buffer_seek(info_buffer buffer, size_t pos)
{
        if(pos>=buffer->size)
                INTERNAL("seek exceeds buffer length %d<%d", buffer->size, pos);
        buffer->cursor=pos;
        return false;
}


char *info_internal_buffer_str(info_buffer buffer){
        return buffer->str;
}
void info_internal_buffer_free(info_buffer buffer)
{
        free(buffer->str);
        free(buffer);
}
