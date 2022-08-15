#include "stream_internal.h"
#include "format_internal.h"

//static void info_internal_stream_formats_set(info_stream stream)
//{
//        for(int i=0; i<(enum INFO_TYPE)COUNT; i++)
//        {
//                if(!stream->format[i])
//                        stream->format[i]=info_foramt_default;
//        }
//}

//info_stream info_stram_create(FILE *f,

bool info_internal_stream_output(info_stream stream, info_Msg message)
{
        info_buffer buffer = info_internal_buffer_create(0);

        if(info_format_Msg_format(message, stream->format, stream->ANSI_support, buffer))
                INTERNAL("Could not eval message!")
        fwrite(info_internal_buffer_str(buffer), 1, info_internal_buffer_tell(buffer), stream->f);
        fputc('\n', stream->f);
        if(stream->ANSI_support)
                info_internal_ANSI_stream_reset(stream->f);
        fflush(stream->f);
        info_internal_buffer_free(buffer);
        return false;
}
