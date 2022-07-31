#include "stream_internal.h"

static void info_internal_stream_formats_set(info_stream stream)
{
        for(int i=0; i<(enum INFO_TYPE)COUNT; i++)
        {
                if(!stream->format[i])
                        stream->format[i]=info_foramt_default;
        }
}

//info_stream info_stram_create(FILE *f,

bool info_internal_stream_output(info_stream stream, struct info_internal_Msg *message)
{
        info_internal_stream_formats_set(stream);
        info_buffer buffer = info_internal_buffer_create(0);

        info_internal_format_eval(stream->format,stream->ANSI_support, buffer, message);
        info_internal_buffer_free(buffer);
}
