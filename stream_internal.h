#include "stream.h"
#include "format.h"
#include <stdbool.h>


struct info_internal_stream
{
        FILE *f;
        bool ANSI_support;
        info_Formats format;
};

bool info_internal_stream_output(info_stream stream, struct info_internal_Msg *message);
