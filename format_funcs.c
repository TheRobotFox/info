#include "format_funcs.h"
#include <string.h>
typedef struct info_format_function_arg* Arg;
#define RET_NOTHING return List_create(sizeof(struct info_drawcall));

#define STRING_SIZE 512
info_char store_buf[STRING_SIZE]={0};
size_t store_buf_length=0;

static bool util_strcmp(const info_char *a, const info_char *b, size_t len)
{
        for(size_t i=0; i<len; i++)
        {
                if(a[i]!=b[i])
                        return true;
                if(a[i]==0)
                        return false;
        }
        return false;
}

size_t util_offset_get()
{
        return List_size(formatting_info.buffer);
}

static bool util_load_args(List args, int num, ...)
{
        if(List_size(args)!=num)
                return false;

        va_list list, check;
        va_start(list, num);
        va_copy(check, list);

        for(struct info_format_function_arg *start = List_start(args),
                        *end = List_end(args); start!=end; start++)
        {
                if(start->type!=va_arg(check, enum info_format_function_arg_type))
                        return false;
                va_arg(check, void*);
        }

        for(struct info_format_function_arg *start = List_start(args),
                        *end = List_end(args); start!=end; start++)
        {
                switch(va_arg(list, enum info_format_function_arg_type))
                {
                        case INT: *va_arg(list, int*)=start->num; break;
                        case STRING: *va_arg(list, List*)=start->buf; break;
                }
        }
        return true;
}

static List util_print_n(info_char c, size_t n, ANSI ansi)
{
        List out = List_create(sizeof(struct info_drawcall));
        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=ansi;
        d->text=info_buffer_create(n);
        List_resize(d->text, n);

        info_char *str = info_buffer_str(d->text);

        for(size_t i=0; i<n; i++)
                str[i]=c;

        return out;
}

static List util_return_string(ANSI ansi, const info_char *str, size_t len)
{

        List out = List_create(sizeof(struct info_drawcall));
        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=ansi;
        d->text=info_buffer_create(len);
        List_append(d->text, str, len);
        return out;
}
static List timestamp(List args)
{
        const info_char *fmt = INFO_STR("%d:%.2d:%.2d");
        // check args
        if(List_size(args)!=0){
                INTERNAL_ERROR("got %d args but take at most 1", List_size(args))
                return NULL;
        }

        if(List_size(args)==1){
                Arg arg = List_start(args);
                if(arg->type!=STRING){
                        INTERNAL_ERROR("wrong arg type! Need STRING arg")
                        return NULL;
                }
                fmt = info_buffer_str(arg->buf);
        }

        List drawcall_list = List_create(sizeof( struct info_drawcall));

        time_t time_rel;
        struct tm *time_struct;
        time(&time_rel);
        time_struct = gmtime(&time_rel);
        if(info_drawcall_printf(drawcall_list, ansi_prefix, fmt, (int)time_struct->tm_hour, (int)time_struct->tm_min, (int)time_struct->tm_sec))
                INTERNAL_ERROR("print failed")
        return drawcall_list;
}

List substring_tmp = NULL;
static List substring_start(List args)
{
        if(!substring_tmp)
                substring_tmp = List_create(sizeof(struct info_internal_format_substring));

        struct info_internal_format_substring substring = {0};
        substring.offset_start = List_size(formatting_info.buffer);
        if(List_size(args)>1){
                INTERNAL_ERROR("got %d args but take at most 1", List_size(args))
                return NULL;
        }

        if(List_size(args)==1){
                Arg arg = List_at(args,0);
                if(arg->type!=STRING){
                        INTERNAL_ERROR("wrong arg type! Need STRING arg")
                        return NULL;
                }
                substring.name=info_buffer_create(List_size(arg->buf));
                List_append(substring.name,
                                        List_start(arg->buf),
                                        List_size(arg->buf));
        }
        if(!substring.name)
                substring.name = info_buffer_create(1);


        List_push(substring_tmp, &substring);
        RET_NOTHING
}

static List substring_end(List args)
{
        if(List_size(args)){
                INTERNAL_ERROR("got %d args but need 0", List_size(args))
                return NULL;
        }

        if(!List_size(substring_tmp)){
                INTERNAL_ERROR("Expected '(' before ')'")
                return NULL;
        }

        struct info_internal_format_substring *substring = List_pop(substring_tmp);
        substring->offset_end = List_size(formatting_info.buffer);

        List_push(formatting_info.substrings, substring);

        if(!List_size(substring_tmp)){
                List_free(substring_tmp);
                substring_tmp=NULL;
        }
        RET_NOTHING
}


static List info_tag(List args)
{
        int output_color = false;
        if(util_load_args(args, 0));
        else if(util_load_args(args, 1, INT, &output_color));
        else {
                INTERNAL_ERROR("wrong args")
                return NULL;
        }
        if(output_color){
                List out = List_create(sizeof(struct info_drawcall));
                struct info_ANSI_color col = info_internal_ANSI[formatting_info.current->type].forground;
                info_drawcall_printf(out, ansi_prefix, INFO_STR("%d,%d,%d"), col.r, col.g, col.b);
                return out;
        } else {
                enum INFO_TYPE type = formatting_info.current->type;
                const info_char *str = info_internal_tags[type];
                return util_return_string(info_internal_ANSI[type], str, STRLEN(str));
        }
}

static List info_content(List args)
{
        size_t argc = List_size(args);
        if(argc){
                INTERNAL_ERROR("context takes no arguments, but got %d", argc)
                return NULL;
        }

        List elements = formatting_info.current->d,
             out = info_drawcall_list_create();

        drawcall_iterator start = info_drawcall_iterator_start(elements),
                          end = info_drawcall_iterator_end(elements);
        info_drawcall_list_copy(out, start, end);
        info_drawcall_iterator_free(start);
        info_drawcall_iterator_free(end);
        return out;

}

List info_funcname(List args)
{
        if(List_size(args)){
                INTERNAL_ERROR("func name doesn't take any arguments")
                return NULL;
        }
        size_t len;
#ifdef INFO_WIDE
        info_char *str;
        const char *bstr = formatting_info.current->origin.func;
        len = strlen(bstr);
        str = malloc(len*sizeof(info_char));
        for(int i=0; i<len; i++)
                str[i]=btowc(bstr[i]);
#else
        const info_char *str;
        str = formatting_info.current->origin.func;
        len = STRLEN(str);
#endif
        List out = List_create(sizeof(struct info_drawcall));
        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=formatting_info.current->start;
        d->text=info_buffer_create(len);
        List_append(d->text, str, len);
#ifdef INFO_WIDE
        free(str);
#endif
        return out;
}

static List info_file(List args)
{
        if(!util_load_args(args, 0)){
                INTERNAL_ERROR("No args!")
                return NULL;
        }
        const info_char *str = formatting_info.current->origin.file;
        return util_return_string(ansi_prefix, str, STRLEN(str));
}

static List info_line(List args)
{
        if(!util_load_args(args, 0)){
                INTERNAL_ERROR("No args!")
                return NULL;
        }
        List out = List_create(sizeof(struct info_drawcall));
        info_drawcall_printf(out, ansi_prefix, INFO_STR("%lu"), formatting_info.current->origin.line);
        return out;
}

static List info_whitespaces(List args)
{

        if(List_size(args)!=1) {
                INTERNAL_ERROR("takes 1 argument, but got %d", List_size(args))
                return NULL;
        }
        Arg arg = List_at(args, 0);

        if(arg->type!=STRING){
                INTERNAL_ERROR("argument has to be string")
                return NULL;
        }

        int count = util_get_pos(List_start(arg->buf), List_size(arg->buf));
        return util_print_n(INFO_STR(' '), count, formatting_info.current->start);
}

static void info_internal_store(info_char *buf, size_t *buf_length, size_t size)
{
        const info_char * prefix_str = info_buffer_str(formatting_info.buffer);
        size_t lenght = util_offset_get();
        lenght = lenght<size ? lenght : size;
        *buf_length=lenght;
        for(int i=0; i<lenght; i++)
                buf[i]=prefix_str[i];
}

static List info_store(List args)
{
        if(List_size(args)>1){
                INTERNAL_ERROR("takes at most 1 argument");
                return NULL;
        }
        info_internal_store(store_buf, &store_buf_length,  STRING_SIZE);
        RET_NOTHING;
}


static List info_indentation(List args)
{

        unsigned short indentation = formatting_info.current->indentation;
        return util_print_n(INFO_STR('\t'), indentation, formatting_info.current->start);
}

static List info_structured(List args)
{
        static size_t last_indentation = 0;
        if(List_size(args)){
                INTERNAL_ERROR("Takes no args")
                return NULL;
        }
        size_t indentation = formatting_info.current->indentation;

        ANSI ansi = formatting_info.current->start;
        if(last_indentation==indentation)
                return util_print_n(INFO_STR('\t'), indentation, ansi);

        List drawcall_list = List_create(sizeof(struct info_drawcall));

        if(indentation>last_indentation){
                List tmp = util_print_n(INFO_STR('\t'), indentation-1, ansi);
                List_concat(drawcall_list, tmp);
                info_drawcall_printf(drawcall_list, ansi, INFO_STR("{\n"));
                List_free(tmp);
        }else{
                List tmp = util_print_n(INFO_STR('\t'), indentation, ansi);
                List_concat(drawcall_list, tmp);
                info_drawcall_printf(drawcall_list, formatting_info.current->start, INFO_STR("}\n"));
                List_free(tmp);

        }
        last_indentation=indentation;


        return drawcall_list;
}

static List info_substring_get(List args)
{
        int index=-1;
        if(!List_size(formatting_info.substrings)){
                INTERNAL_ERROR("No Subtrings where found")
                return NULL;
        }
        // check args
        if(List_size(args)>1){
                INTERNAL_ERROR("got %d args but take at most 1", List_size(args))
                return NULL;
        }
        if(List_size(args)==1){
                Arg arg = List_at(args,0);
                if(arg->type==INT){
                        index=arg->num;
                }else if(arg->type==STRING){
                       for(int i=0; i<List_size(formatting_info.substrings); i++)
                       {
                               struct info_internal_format_substring *sstr = List_at(formatting_info.substrings, i);
                               if(!util_strcmp(info_buffer_str(sstr->name), info_buffer_str(arg->buf), List_size(arg->buf))){
                                       index=i;
                                       goto fin;
                               }
                       }
                       INTERNAL_ERROR("substring \"%s\" could not be foud!", info_buffer_str(arg->buf))
                        return NULL;
                }
        }
fin:
        if(index>=(int)List_size(formatting_info.substrings) || List_size(formatting_info.substrings)==0){
                INTERNAL_ERROR("substring index %d out of range", index)
                return NULL;
        }
        struct info_internal_format_substring *sstr = List_at(formatting_info.substrings, index);
        List out = List_create(sizeof(struct info_drawcall));

        size_t lenght = sstr->offset_end-sstr->offset_start;
        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=formatting_info.current->start;
        d->text=info_buffer_create(lenght);
        info_char *str_start = info_buffer_str(formatting_info.buffer);
        List_append(d->text, str_start+sstr->offset_start, lenght);

        return out;
}

static List info_restore(List args)
{
        if(List_size(args)){
                INTERNAL_ERROR("takes no arguments!")
                return NULL;
        }

        List out = List_create(sizeof(struct info_drawcall));

        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=formatting_info.current->start;
        d->text=info_buffer_create(store_buf_length);
        List_append(d->text, store_buf, store_buf_length);

        return out;
}

static List info_ansi(List args)
{
        if(List_size(args)!=4){
usage:
                INTERNAL_ERROR("ansi needs 4 args {STR}[R][G][B]")
                return NULL;
        }
        Arg str = List_at(args, 0);
        if(str->type!=STRING)
                goto usage;
        Arg r = List_at(args, 1);
        Arg g = List_at(args, 2);
        Arg b = List_at(args, 3);
        if(r->type !=INT || g->type !=INT || b->type !=INT)
                goto usage;

        List out = LIST_create(struct info_drawcall);

        struct info_drawcall *d = List_push(out, NULL);
        d->ansi=INFO_ANSI_normal_color(r->num,g->num,b->num);
        d->text=info_buffer_create(List_size(str->buf));
        List_append(d->text, List_start(str->buf), List_size(str->buf));

        return out;
}
static List info_pos(List args)
{

}

struct info_format_function functions[] = {
        {INFO_STR("timestamp"), INFO_STR('t'), timestamp},
        {INFO_STR("tag"), INFO_STR('i'), info_tag},
        {INFO_STR("sub_beg"), INFO_STR('('), substring_start},
        {INFO_STR("sub_end"), INFO_STR(')'), substring_end},
        {INFO_STR("content"), INFO_STR('c'), info_content},
        {INFO_STR("function"), INFO_STR('f'), info_funcname},
        {INFO_STR("file"), INFO_STR('F'), info_file},
        {INFO_STR("line"), INFO_STR('L'), info_line},
        {INFO_STR("whitespaces"), INFO_STR('w'), info_whitespaces},
        {INFO_STR("indent"), INFO_STR('d'), info_indentation},
        {INFO_STR("sub_get"), INFO_STR('g'), info_substring_get},
        {INFO_STR("restore"), INFO_STR('r'), info_restore},
        {INFO_STR("ansi"), INFO_STR('a'), info_ansi},
        {INFO_STR("store"), INFO_STR('b'), info_store},
        {INFO_STR("pos"), INFO_STR('p'), info_pos},
        {INFO_STR("structured"), INFO_STR('s'), info_structured},

};

size_t functions_count = sizeof(functions)/sizeof(*functions);

