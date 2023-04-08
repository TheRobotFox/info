#include "drawcall_list.h"
#include "stream_internal.h"
#include <stddef.h>
#include <time.h>
#include <locale.h>

struct info_drawcall
{
        enum info_internal_drawcall_content_stream content_stream;
        ANSI ansi;
        const info_char *text;
};

struct drawcall_iterator
{
        drawcall_iterator it;
        size_t content_index;
        size_t contenet_size;
        List *current;
};

drawcall_list info_drawcall_list_create()
{
        return List_create(sizeof(struct info_drawcall));
}

bool info_internal_drawcall_vprintf(drawcall_list l, enum info_internal_drawcall_content_stream stream, ANSI ansi, const info_char *format, va_list args)
{
        if(!format)
                INTERNAL("printf format is NULL")
        int res;
        va_list arg_tmp;
        struct info_drawcall d;
        d.content_stream=stream;
        d.ansi=ansi;

#ifdef INFO_WIDE

        size_t length=100;
        do{
                length*=2;
                List_reserve(d.content, length);
                va_copy(arg_tmp, args);
        }
        while((res=vswprintf(List_end(d.content), List_capacity(d.content)-List_size(d.content), format, args))==-1)

        List_grow(d.content, List_size(d.content)+res+1);

        return false;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(NULL ,0, format, arg_tmp);
        va_end(arg_tmp);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length)

        List_reserve(d.content, length+1);

        res = vsnprintf(List_end(d.content), length+1, format, args);

        List_grow(d.content, length+1);
        return res!=length;
#endif

}

bool info_internal_drawcall_printf(drawcall_list l, enum info_internal_drawcall_content_stream stream, const info_char *format, ...)
{
        va_list args;
        va_start(args, format);
        bool res = info_internal_drawcall_vprintf(l, stream, format, args);
        va_end(args);
        return res;
}

bool info_drawcall_vprintf(drawcall_list l, const info_char *format, va_list args)
{
	return info_internal_drawcall_vprintf(l, CONTENT_TEXT, format, args);
}

bool info_drawcall_printf(List drawcall_list, enum info_internal_drawcall_content_stream stream, ANSI ansi, const info_char *format, ...)
{
	va_list args;
	va_start(args, format);
	bool ret = info_internal_drawcall_vprintf(drawcall_list, stream, format, args);
	va_end(args);
	return ret;
}

drawcall_iterator info_drawcall_text_iterator_create(drawcall_list d)
{
        drawcall_iterator it = malloc(sizeof(*it));
        it->source=d;
        it->index=-1;
        info_drawcall_iterator_advance(it);
        return it;
}

bool info_drawcall_iterator_advance(drawcall_iterator it)
{
        if(it->index<List_size(it->source))
                it->index++;
        else
                it->valid=false;
        return it->valid;
}

void* info_drawcall_iterator_get(drawcall_iterator it)
{
        if(!it->valid)
                return NULL;
        return List_get(it->source, it->index);
}

drawcall_iterator info_drawcall_iterator_copy(drawcall_iterator it)
{
        drawcall_iterator new = malloc(sizeof(*it));
        *new=*it;
        return new;
}

void info_drawcall_iterator_free(drawcall_iterator it)
{
        free(it);
}

bool info_drawcall_copy(drawcall_iterator start, drawcall_iterator end, drawcall_list out)
{
        drawcall_list in = start->source;
        if(end->source!=in)
                INTERNAL("iterators to different lists")
        struct info_drawcall d = List_get(start->source, start->list_index);
        List tmp = List_append(out, NULL);
        size_t end;
        if(start->list_index==end->list_index)
                end=end->string_index;
        else
                end=List_size(d.text);
        for(size_t l=start->string_index; l<end->list_index-1; l++)
        for(size_t l=start->list_index+1; l<end->list_index-1; l++)
        {
                
}
