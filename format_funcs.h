#pragma once
#include "format.h"
#include "format_internal.h"
#include <time.h>

struct info_format_function
{
        const info_char *name;
        info_char shortname;
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
                List buf;
                int num;
        };
};


struct info_internal_format_substring
{
        List name;
        size_t offset_start, offset_end;
};

struct formatting_info
{
        info_Msg current;
        List buffer;
        struct info_format *format;
        //size_t line;
        size_t start_offset;
        size_t func_offset;
        List substrings;
};
size_t util_get_pos(const info_char *a, size_t length);

extern struct formatting_info formatting_info;

extern struct info_format_function functions[];
extern size_t functions_count;
