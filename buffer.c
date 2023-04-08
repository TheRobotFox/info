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
bool info_internal_buffer_append_buf(info_buffer buffer, info_buffer co)
{
        bool res = info_internal_buffer_append(buffer, co->str, co->cursor);
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
