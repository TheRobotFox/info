#pragma once
#include "info_internal.h"
#include "ANSI_internal.h"
#include <stdbool.h>


bool info_internal_format_str_eval(const char *format, size_t len, bool ANSI, info_buffer out);
bool info_format_Msg_format(info_Msg msg, info_Formats format, bool ANSI, info_buffer out);
