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
        info_buffer buffer = info_internal_buffer_create(0);

        struct info_format format = info_format_select(format_current, info_internal_formats[message->type]);
        format = info_format_select(format, stream->formats[message->type]);
        if(info_format_Msg_format(message, format, stream->ANSI_support, buffer))
                INTERNAL("Could not eval message!")



        info_char *str = info_internal_buffer_str(buffer);
        size_t len = info_internal_buffer_tell(buffer);
        if(str[len-1]==INFO_STR('\n'))
                info_internal_buffer_grow(buffer, len-1);

        FPUTS(info_internal_buffer_str(buffer), stdout);
        FPUTC(INFO_STR('\n'), stream->f);
        if(stream->ANSI_support)
                info_internal_ANSI_stream_reset(stream->f);
        fflush(stream->f);
        info_internal_buffer_free(buffer);
        return false;
}
