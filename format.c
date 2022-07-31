#include "format_funcs.h"
#include <string.h>
#include <stdio.h>


struct info_internal_format_substring
{
        const char *name;
        char *start, end;
};

static size_t util_string_next(char c, const char * str, size_t len)
{
        for(int i=0; i<len; i++)
        {
                if(str[i]==c)
                        return i;
        }
        return 0;
}

static struct info_format_function* info_internal_format_function_by_name(const char *name, size_t len)
{
        for(int i=0; i<functions_count; i++)
        {
                if(!strncmp(name,functions[i].name,len))
                                return functions+i;
        }
        INTERNAL_ERROR("Could not find function \"%%<%s>\"!", name)
        return 0;
}
static struct info_format_function* info_internal_format_function_by_shortname(char shortname)
{
        for(int i=0; i<functions_count; i++)
        {
                if(shortname==functions[i].shortname)
                                return functions+i;
        }
        INTERNAL_ERROR("Could not find function '%%%c'!", shortname)
        return 0;
}

static bool info_internal_format_function_execute(struct info_format_function *f, List args, info_buffer out, bool ANSI)
{
        // execute func
        List draw = f->func(args);
        if(!draw)
                return true;
        // handle return drawcall list
        for(struct info_internal_drawcall *start=List_start(draw),
                        *end = List_end(draw); start!=end; start++)
        {
                if(start->type==ANSI && !ANSI)
                        continue;
                // append and free buffer
                info_internal_buffer_cat(out, start->content);
        }
        List_free(draw);
        return false;
}

static bool info_internal_format_arg_read(List args, enum info_format_function_arg_type type, const char *str, size_t len)
{
        info_buffer tmp = info_internal_buffer_create(len);
        if(info_internal_format_str_eval(str, len,tmp, false))
                return true;
        struct info_format_function_arg arg={0};
        arg.type=type;
        switch(type)
        {
                case INT:
                        arg.num=strtol(info_internal_buffer_str(tmp),NULL,10);
                        break;
                case STRING:
                        arg.str=info_internal_buffer_str(tmp);
                        break;
                default:
                        info_internal_buffer_free(tmp);
                        return true;
        }
        info_internal_buffer_free(tmp);
        return !List_append(args, &arg);
}

// very ugly
static size_t info_internal_format_solve(const char *str, size_t len, info_buffer buffer, bool ANSI)
{
        size_t ret=0;
        List args = 0;
        struct info_format_function *func;
start:
        switch(*str)
        {
        case '%':       // escape
                info_internal_buffer_printf(buffer, "%", 1);
                break;

        case '<':       // func by name
                {
                        // get string end
                        int end = util_string_next('>', str, len);
                        if(!end){
                                INTERNAL_ERROR("Expected '>'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        // consume string
                        ret+=end;
                        // get func by name
                        func = info_internal_format_function_by_name(str+1,end-1);
                        if(!func){
                                ret=-1;
                                break;
                        }
                        if(info_internal_format_function_execute(func,args, buffer, ANSI))
                                ret=-1;
                } break;

        case '[':       // eval args
                {
                        int end = util_string_next(']', str, len);
                        if(!end){
                                INTERNAL_ERROR("Expected ']'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        ret+=end;
                        if(!args)
                                args=List_create(sizeof(struct info_format_function_arg));

                        if(info_internal_format_arg_read(args, INT, ++str, end-1)){
                                ret=-1;
                                break;
                        }
                        str+=end;
                        goto start;
                } break;
        case '{':
                {
                        int end = util_string_next('}', str, len);
                        if(!end){
                                INTERNAL_ERROR("Expected '}'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        ret+=end;
                        if(!args)
                                args=List_create(sizeof(struct info_format_function_arg));

                        if(info_internal_format_arg_read(args, STRING, ++str, end-1)){
                                ret=-1;
                                break;
                        }
                        str+=end;
                        goto start;
                } break;

        default:        // func by shortname
                ret++;
                func = info_internal_format_function_by_shortname(*str);
                if(!func){
                        ret=-1;
                        break;
                }
                if(info_internal_format_function_execute(func,args, buffer, ANSI))
                        ret= -1;
        }

        if(args)
                List_free(args);

        return ret;
}

struct info_internal_Msg *info_internal_format_message_current;

bool info_internal_format_str_eval(const char *format, size_t len, bool ANSI, struct info_internal_buffer *out)
{

        info_buffer buffer = info_internal_buffer_create(len);

        size_t i=0, res;
        while((res=util_string_next('%', format+i, len-i)))
        {
                info_internal_buffer_append(buffer, format+i, res-1);
                i+=res;
                if(i==len-1)
                        INTERNAL("trailing %%")
                i++;

                if((res=info_internal_format_solve(format+i,len-i, buffer, ANSI))<0)
                        ;//INTERNAL_ERROR("parsing fomrat string failed at %d", i)
                i+=res;
        }

        return false;
}
