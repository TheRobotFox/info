#include "stream_internal.h"
#include "format_internal.h"

//static void info_internal_stream_formats_set(info_stream stream)
//{
//        for(int i=0; i<(enum INFO_TYPE)INFO_COUNT; i++)
//        {
//                if(!stream->format[i])
//                        stream->format[i]=info_foramt_default;
//        }
//}

//info_stream info_stram_create(FILE *f,

bool info_internal_stream_output(info_stream stream, info_Msg message)
{
        List buffer = info_buffer_create(0);

        struct info_format format = info_format_overlay(format_current, info_internal_formats[message->type]);
        format = info_format_overlay(format, stream->formats[message->type]);
        if(info_format_message_compile(message, format, stream->ANSI_support, buffer))
                INTERNAL("Could not eval message!")



        info_char *str = info_buffer_str(buffer);
        size_t len = List_size(buffer);
        if(str[len-1]==INFO_STR('\n'))
                List_resize(buffer, len-1);

        fwrite(str, sizeof(info_char), List_size(buffer), stream->f);
        FPUTC(INFO_STR('\n'), stream->f);
        if(stream->ANSI_support)
                info_internal_ANSI_stream_reset(stream->f);
        fflush(stream->f);
        List_free(buffer);
        return false;
}
