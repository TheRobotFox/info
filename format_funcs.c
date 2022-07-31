#include "format_funcs.h"

typedef struct info_format_function_arg* Arg;

static List timestamp(List args)
{
        const char *fmt = "[%d:%.2d:%.2d]";
        // check args
        if(List_size(args)>1)
                return 0;
        if(List_size(args)==1){
                Arg arg = List_get(args,0);
                if(arg->type==INT)
                        return 0;
                fmt = arg->str;
        }

        List drawcall_list = List_create(sizeof( struct info_internal_drawcall));

        time_t time_rel;
        struct tm *time_struct;
        time(&time_rel);
        time_struct = gmtime(&time_rel);
        info_internal_drawcall_printf(drawcall_list, TEXT, fmt, time_struct->tm_hour, time_struct->tm_min, time_struct->tm_sec);
        return drawcall_list;
}

static List info_tag(List args)
{
        extern struct info_internal_Msg *info_internal_format_message_current;
struct info_format_function functions[] = {
        {"timestamp", 't', timestamp},
};

size_t functions_count = sizeof(functions)/sizeof(*functions);
