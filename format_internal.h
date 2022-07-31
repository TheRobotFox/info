#include "info_internal.h"
#include "ANSI_internal.h"
#include "format.h"
#include <stdbool.h>

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


bool info_internal_format_str_eval(const char *format, size_t len, bool ANSI, struct info_internal_buffer *out, struct info_internal_Msg *message);
