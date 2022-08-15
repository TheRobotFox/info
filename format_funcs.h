#pragma once
#include "format.h"
#include "format_internal.h"
#include <time.h>

struct info_format_function
{
        const char *name;
        char shortname;
        List (*func)(List);
};

enum info_format_function_arg_type
{
        STRING,
        INT
};

struct info_format_function_arg
{
        enum info_format_function_arg_type type;
        union {
                const char *str;
                int num;
        };
};


struct info_internal_format_substring
{
        const char *name;
        size_t offset_start, offset_end;
};

struct formatting_info
{
        info_Msg current;
        info_buffer buffer;
        //size_t line;
        size_t offset;
        List substrings;
};

extern struct formatting_info formatting_info;

extern struct info_format_function functions[];
extern size_t functions_count;
