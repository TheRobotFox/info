#include "format_funcs.h"
#include <string.h>
#include <stdio.h>

static int util_string_next(char c, const char * str, size_t len, size_t offset)
{
        for(int i=offset; i<len; i++)
        {
                if(str[i]==c)
                        return i-offset;
        }
        return -1;
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
                if(ANSI)
                        info_internal_ANSI_switch(out, start->ansi);

                // append and free buffer
                info_internal_buffer_consume(out, start->content);
        }
        //printf("%s -> %d\n", f->name, info_internal_buffer_tell(out));
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
        List args = List_create(sizeof(struct info_format_function_arg));
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
                        int end = util_string_next('>', str, len, 0);
                        if(end==-1){
                                INTERNAL_ERROR("Expected '>'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        // consume string
                        ret+=end+1;
                        // get func by name
                        func = info_internal_format_function_by_name(str+1,end-1);
                        if(!func){
                                INTERNAL_ERROR("Function not found!")
                                ret=-1;
                                break;
                        }
                        if(info_internal_format_function_execute(func,args, buffer, ANSI))
                                ret=-1;
                } break;

        case '[':       // eval args
                {
                        int end = util_string_next(']', str, len, 0);
                        if(end==-1){
                                INTERNAL_ERROR("Expected ']'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        ret+=end+1;

                        if(info_internal_format_arg_read(args, INT, ++str, end-1)){
                                ret=-1;
                                break;
                        }
                        str+=end;
                        goto start;
                } break;
        case '{':
                {
                        int end = util_string_next('}', str, len, 0);
                        if(end==-1){
                                INTERNAL_ERROR("Expected '}'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        ret+=end+1;

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

        List_free(args);

        return ret;
}

struct formatting_info formatting_info;

bool info_internal_format_str_eval(const char *format, size_t len, bool ANSI, info_buffer out)
{

        size_t i=0, res;
        size_t pos = 0;
        while((res=util_string_next('%', format, len, i))!=-1)
        {
                if(ANSI){
                        pos+=info_internal_buffer_tell(out);
                        info_internal_ANSI_switch(out, ansi_prefix);
                        pos-=info_internal_buffer_tell(out);
                }else{
                        pos+=info_internal_buffer_tell(out);

                }
                formatting_info.offset=pos;

                info_internal_buffer_append(out, format+i, res);
                i+=res;
                if(i==len-1)
                        INTERNAL("trailing %%")
                i++;


                if((res=info_internal_format_solve(format+i,len-i, out, ANSI))<0)
                        ;//INTERNAL_ERROR("parsing fomrat string failed at %d", i)
                i+=res;
        }

        return false;
}

bool info_format_Msg_format(info_Msg msg, info_Formats format, bool ANSI, info_buffer out)
{
        const char *format_str = format[msg->type].format;
        info_buffer tmp = info_internal_buffer_create(strlen(format_str));

        formatting_info.substrings = List_create(sizeof(struct info_internal_format_substring));
        formatting_info.current = msg;
        formatting_info.buffer = tmp;
        info_internal_format_str_eval(format_str, strlen(format_str), ANSI, tmp);

        size_t length=info_internal_buffer_tell(tmp);
        const char *tmp_str = info_internal_buffer_str(tmp);

        const char *newline_str = format[msg->type].newline;
        size_t newline_str_len = strlen(newline_str);

        int start = 0, next;
        while((next=util_string_next('\n', tmp_str, length, start))>0)
        {
                info_internal_buffer_append(out, tmp_str+start, next);
                if(info_internal_format_str_eval(newline_str, newline_str_len, ANSI, out))
                {
                        INTERNAL_ERROR("Could not eval newline string at pos %d\noriginal string:\n%s\n--------", start, newline_str)
                        List_free(formatting_info.substrings);
                        return true;
                }
                start=next+1;
        }
        info_internal_buffer_append(out, tmp_str+start, length-start);

        List_free(formatting_info.substrings);
        info_internal_buffer_free(tmp);
        return false;
}
