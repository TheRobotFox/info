#pragma once
#include "info_internal.h"
#include "format.h"
#include "stream.h"
#include <stdio.h>
#include <stdbool.h>


struct info_internal_stream
{
        FILE *f;
        bool ANSI_support;
        info_formats formats;
};

bool info_internal_stream_output(info_stream stream, info_Msg message);
