#include "drawcall_list.h"
#include "stream_internal.h"
#include <stddef.h>
#include <time.h>
#include <locale.h>

IMPLEMENT_LIST(struct info_drawcall, text)


struct drawcall_iterator
{
        drawcall_list source;
        size_t content_index;
        size_t content_size;
        bool valid;
        List current;
        size_t current_index;
};

drawcall_list info_drawcall_list_create()
{
        return LIST_create(struct info_drawcall);
}

List info_buffer_create(size_t size)
{
        List l = LIST_create(info_char);
        List_reserve(l, size);
        return l;
}

info_char* info_buffer_str(List buffer)
{
        size_t len = List_size(buffer);
        List_reserve(buffer, len+1);
        info_char* str = List_start(buffer);
        str[len]=0;
        return str;
}

bool info_buffer_vprintf(List buffer, const info_char *format, va_list args)
{
        if(!format)
                INTERNAL("printf format is NULL")
        int res;
        va_list arg_tmp;

#ifdef INFO_WIDE

        size_t length=100;
        do{
                length*=2;
                List_reserve(buffer, length);
                va_copy(arg_tmp, args);
        }
        while((res=vswprintf(List_end(buffer), List_capacity(buffer)-List_size(buffer), format, args))==-1)

        List_resize(buffer, List_size(buffer)+res);

        return false;
#else
        va_copy(arg_tmp, args);

        int length = vsnprintf(NULL ,0, format, arg_tmp)+1;
        va_end(arg_tmp);
        if(length<0)
                INTERNAL("vsnprintf error: %d", length)

        List_reserve(buffer, List_size(buffer)+length);

        res = vsnprintf(List_end(buffer), length, format, args);

        List_resize(buffer, List_size(buffer)+length);
        return res!=length-1;
#endif

}

bool info_buffer_printf(List buffer, const info_char *format, ...)
{
        va_list args;
        va_start(args, format);
        bool res = info_buffer_vprintf(buffer, format, args);
        va_end(args);
        return res;
}

bool info_drawcall_vprintf(drawcall_list l, ANSI ansi, const info_char *format, va_list args)
{
        struct info_drawcall d = {.ansi=ansi, .text=info_buffer_create(0), .num=1337};
        List_push(l, &d);
	return info_buffer_vprintf(d.text, format, args);
}

bool info_drawcall_printf(List drawcall_list, ANSI ansi, const info_char *format, ...)
{
	va_list args;
	va_start(args, format);
	bool ret = info_drawcall_vprintf(drawcall_list, ansi, format, args);
	va_end(args);
	return ret;
}

drawcall_iterator info_drawcall_iterator_start(drawcall_list d)
{
        size_t len = List_size(d);
        if(len){
                drawcall_iterator it = (drawcall_iterator)malloc(sizeof(*it));
                it->source=d;
                it->valid=true;
                it->content_index=0;
                it->current_index=0;
                it->current = (LIST_at(struct info_drawcall)(d, 0))->text;
                it->content_size=List_size(it->current);
                return it;
        }
        return NULL;
}
drawcall_iterator info_drawcall_iterator_end(drawcall_list d)
{
        size_t len = List_size(d);
        if(len){
                drawcall_iterator it = (drawcall_iterator)malloc(sizeof(*it));
                it->source=d;
                it->valid=true;
                it->current_index=len;
                it->current = (LIST_at(struct info_drawcall)(d, len-1))->text;
                it->content_size=List_size(it->current);
                it->content_index=it->content_size;
                return it;
        }
        return NULL;
}

bool info_drawcall_iterator_advance(drawcall_iterator it)
{
        if(it->content_index<it->content_size-1){

                it->content_index++;
        }else{

                if(it->current_index<List_size(it->source) && it->valid){

                        it->current_index++;
                        it->current=List_GET_REF(List, it->source, it->current_index);
                        it->content_size=List_size(it->current);
                        it->content_index=0;
                }else{

                        it->valid=false;
                }
        }
        return it->valid;
}

info_char* info_drawcall_iterator_get(drawcall_iterator it)
{
        if(!it->valid)
                return NULL;
        return List_at(it->current, it->content_index);
}

ANSI info_drawcall_iterator_get_ansi(drawcall_iterator it)
{
        return List_GET_REF(struct info_drawcall, it->source, it->current_index).ansi;
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

void info_drawcall_list_free(drawcall_list l)
{
        LIST_forward_m(struct info_drawcall, text)(l, List_free);
        List_free(l);
}
bool info_drawcall_list_copy(drawcall_list out, drawcall_iterator start, drawcall_iterator end)
{
        drawcall_list in = start->source;
        if(end->source!=in)
                INTERNAL("iterators to different lists")
        if(start->current_index>end->current_index)
                INTERNAL("Iterator cannot copy backwards")

        struct info_drawcall a, b;
        size_t content_index=start->content_index;
        info_char *e;

        for(size_t current_index=start->current_index; current_index<end->current_index; current_index++)
        {
                a = LIST_get(struct info_drawcall)(in, current_index);
                b.ansi=a.ansi;
                b.text = info_buffer_create(0);
                List_resize(b.text, List_size(a.text));

                if(current_index!=end->current_index)
                        e = List_end(a.text);
                else
                        e = List_at(a.text, end->content_index);

                for(info_char *s=List_at(a.text, content_index),
                              *t=List_start(b.text); s!=e; s++, t++)
                        *t=*s;
                List_push(out, &b);
                content_index=0;
        }
        return false;
}
