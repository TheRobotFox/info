#include "ANSI_internal.h"
#include <stdbool.h>


struct info_stream
{
        FILE *f;
        const char *prefix_format;
        const char *format;
        bool ANSI_support;
        struct info_internal_ANSI current;


