#include "format_funcs.h"
#include <string.h>
#include <stdio.h>

static int util_string_next(info_char c, const info_char * str, size_t len, size_t offset)
{
        for(int i=offset; i<len; i++)
        {
                if(str[i]==c)
                        return i-offset;
        }
        return -1;
}
static int util_closing(info_char o, info_char c, const info_char *str, size_t len, size_t offset)
{
        size_t depth=0, i=offset;
        do
        {
                if(str[i]==o)
                        depth++;
                else if(str[i]==c)
                        depth--;
                i++;
        } while(depth && i<len);
        if(depth)
                return -1;
        return i-offset-1;
}

size_t util_get_pos(const info_char *a, size_t length)
{
        size_t count = 0;
        bool skip=false;
        for(size_t i=0; i<length; i++){
                if(a[i]==INFO_STR('\033')) skip=true;

                if(skip || a[i]==0){
                        if(a[i]=='m') skip=false;
                }else{
                        if(a[i]==INFO_STR('\n')){
                                count=0;
                        }else if(a[i]==INFO_STR('\t'))
                                count+=tab_width-count%tab_width;
                        else
                                count++;
                }
        }

        return count;
}


static struct info_format_function* info_internal_format_function_by_name(const info_char *name, size_t len)
{
        for(int i=0; i<functions_count; i++)
        {
                if(!STRCMP(name,functions[i].name,len))
                                return functions+i;
        }
        INTERNAL_ERROR("Could not find function \"%%<%s>\"!", name)
        return 0;
}

static struct info_format_function* info_internal_format_function_by_shortname(info_char shortname)
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

static bool info_internal_format_arg_read(List args, enum info_format_function_arg_type type, const info_char *str, size_t len)
{
        info_buffer tmp = info_internal_buffer_create(len);
        if(info_internal_format_str_eval(str, len, false, tmp))
                return true;
        struct info_format_function_arg arg={0};
        arg.type=type;
        //fwrite(info_internal_buffer_str(tmp), 1 ,info_internal_buffer_tell(tmp), stdout); putchar('\n');
        switch(type)
        {
                case BUFFER:
                        arg.buf=tmp;
                        break;
                case INT:
                        arg.num=STRTOL(info_internal_buffer_str(tmp), NULL, 10);
                        info_internal_buffer_free(tmp);
                        break;
                default:
                        info_internal_buffer_free(tmp);
                        return true;
        }
        return !List_push(args, &arg);
}

// very ugly
static size_t info_internal_format_solve(const info_char *str, size_t len, info_buffer buffer, bool ANSI)
{
        size_t ret=0;
        List args = List_create(sizeof(struct info_format_function_arg));
        struct info_format_function *func;
start:
        switch(*str)
        {
        case INFO_STR('%'):       // escape
                info_internal_buffer_append(buffer, INFO_STR("%"), 1);
                ret++;
                break;
        case INFO_STR('<'):       // func by name
                {
                        // get string end
                        int end = util_closing(INFO_STR('<'), INFO_STR('>'), str, len, 0);
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

        case INFO_STR('['):       // eval args
                {
                        int end = util_closing(INFO_STR('['), INFO_STR(']'), str, len, 0);
                        if(end==-1){
                                INTERNAL_ERROR("Expected ']'!")
                                // indicate error
                                ret=-1;
                                break;
                        }

                        int next, offset=1;
                        while((next = util_string_next(',', str, end, offset))!=-1 && offset<len){
                                if(info_internal_format_arg_read(args, INT, str+offset, next)){
                                        ret=-1;
                                        break;
                                }
                                offset+=next+1;
                        }
                        if(info_internal_format_arg_read(args, INT, str+offset, end-offset)){
                                ret=-1;
                                break;
                        }

                        ret+=end+1;
                        str+=end+1;
                        len-=end+1;
                        goto start;
                } break;
        case INFO_STR('{'):
                {
                        int end = util_closing(INFO_STR('{'),INFO_STR('}'), str, len, 0);
                        if(end==-1){
                                INTERNAL_ERROR("Expected '}'!")
                                // indicate error
                                ret=-1;
                                break;
                        }
                        ret+=end+1;

                        if(info_internal_format_arg_read(args, BUFFER, str+1, end-1)){
                                ret=-1;
                                break;
                        }
                        str+=end+1;
                        len-=end+1;
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

        for(struct info_format_function_arg *start=List_start(args),
                                            *end = List_end(args);
                                            start!=end; start++)
        {
                if(start->type==BUFFER)
                        info_internal_buffer_free(start->buf);
        }
        List_free(args);

        return ret;
}

struct formatting_info formatting_info={0};

bool info_internal_format_str_eval(const info_char *format, size_t len, bool ANSI, info_buffer out)
{

        int i=0, res;
        while((res=util_string_next(INFO_STR('%'), format, len, i))!=-1)
        {
                if(ANSI){
                        info_internal_ANSI_switch(out, ansi_prefix);

                }
start:

                info_internal_buffer_append(out, format+i, res);
                i+=res;
                if(i==len-1)
                        INTERNAL("trailing %%")
                i++;

                if((res=info_internal_format_solve(format+i,len-i, out, ANSI))<0){
                        INTERNAL_ERROR("parsing format string failed at %d", info_internal_buffer_tell(formatting_info.buffer))
                        info_internal_buffer_append(out, INFO_STR("%"), 1);
                        res = util_string_next(INFO_STR('%'), format, len, i);
                        if(res<0)
                                break;
                        goto start;
                }
                i+=res;
        }
        info_internal_buffer_append(out, format+i, len-i);

        return false;
}

void info_format_substrings_clear(List substrings)
{
        // free substrings
        for(struct info_internal_format_substring *start = List_start(substrings),
                                                *end = List_end(substrings); start!=end; start++)
                info_internal_buffer_free(start->name);
        List_clear(substrings);
}

bool info_format_replace(info_buffer in, info_char from, const info_char *to, size_t len, bool expand,  bool ANSI_support, info_buffer out)
{
        const info_char *str = info_internal_buffer_str(in);
        size_t length=info_internal_buffer_tell(in);
        int start = 0, next;

        if(expand)
                formatting_info.buffer = in;

        while((next=util_string_next(from, str, length, start))>0)
        {
                info_internal_buffer_append(out, str+start, next);

                if(expand){
                        if(info_internal_format_str_eval(to, len, ANSI_support, out))
                        {
                                INTERNAL_ERROR("Could not eval newline string at pos %d\noriginal string:\n%s\n--------", start, to)
                                info_format_substrings_clear(formatting_info.substrings);
                                return true;
                        }
                } else{
                        info_internal_buffer_append(out, to, len);
                }

                start+=next+1;
        }
        info_internal_buffer_append(out, str+start, length-start);

        return false;
}

struct info_format info_format_select(struct info_format a, struct info_format b)
{
        if(b.format)
                a.format=b.format;
        if(b.newline)
                a.newline=b.newline;
        return a;
}

bool info_filter_newline(info_buffer in, bool ANSI, info_buffer out)
{
        const info_char *newline_str = formatting_info.format->newline;
        if(!newline_str)
                newline_str=INFO_STR("\n");
        return info_format_replace(in, INFO_STR('\n'), newline_str, STRLEN(newline_str), true, ANSI, out);
}

bool info_filter_expand_tabs(info_buffer in, bool ANSI, info_buffer out)
{
        size_t end=info_internal_buffer_tell(in);
        const char *str = info_internal_buffer_str(in);
        for(size_t i=0; i<end; i++)
        {
                if(str[i]=='\t'){
                        for(int i=0; i< tab_width; i++)
                                info_internal_buffer_putc(out, ' ');
                }else{
                                info_internal_buffer_putc(out, str[i]);
                }
        }
        return false;
}

void info_generate_whitespace(size_t size, info_buffer out)
{
        if(size>3){
                info_internal_buffer_putc(out, ' ');
                info_internal_buffer_putc(out, '<');

                for(size_t i=0; i<size-4; i++)
                        info_internal_buffer_putc(out, '-');

                info_internal_buffer_putc(out, '>');
                info_internal_buffer_putc(out, ' ');
        }else{
                info_internal_buffer_putc(out, '<');
                info_internal_buffer_putc(out, '>');
        }
}

bool info_filter_convert_whitespaces(info_buffer in, bool ANSI, info_buffer out)
{
        size_t i=0, end=info_internal_buffer_tell(in);
        const char *str = info_internal_buffer_str(in);
        while(i<end-1)
        {
                // if doublespace begin conversion
                if(str[i]==' ' && str[i+1]==' ')
                {
                        size_t count=0;
                        for(; str[i]==' ' && i<end; i++)
                                count++;
                        info_generate_whitespace(count, out);
                }
                info_internal_buffer_putc(out, str[i]);
                i++;
        }
        return false;
}
bool (*filters[])(info_buffer, bool, info_buffer) = {
        info_filter_newline,
        info_filter_expand_tabs,
        info_filter_convert_whitespaces,
};
size_t filter_count = sizeof(filters)/sizeof(*filters);



bool info_format_Msg_format(info_Msg msg, struct info_format format, bool ANSI, info_buffer out)
{
        const info_char *format_str;

        format_str = format.format;
        if(!format_str)
                format_str=INFO_STR("%c");

        info_buffer initial = info_internal_buffer_create(STRLEN(format_str));

        formatting_info.substrings = List_create(sizeof(struct info_internal_format_substring));
        formatting_info.current = msg;
        formatting_info.buffer = initial;
        formatting_info.start_offset=0;
        formatting_info.format=&format;

        info_internal_format_str_eval(format_str, STRLEN(format_str), ANSI, initial);

        info_buffer tmp = info_internal_buffer_create(info_internal_buffer_tell(initial));
        void *t;

        // apply filters
        for(int i=0; i<filter_count; i++)
        {
                // clear tmp buffer
                info_internal_buffer_seek(tmp, 0);

                filters[i](initial, ANSI, tmp);
                // swap buffers
                t=tmp;
                tmp=initial;
                initial=t;
        }

        info_internal_buffer_consume(out, initial);
        info_internal_buffer_free(tmp);

        info_format_substrings_clear(formatting_info.substrings);
        List_free(formatting_info.substrings);
        return false;
}
