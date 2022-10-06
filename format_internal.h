#pragma once
#include "info_internal.h"
#include "ANSI.h"
#include "ANSI_internal.h"
#include <stdbool.h>


bool info_internal_format_str_eval(const info_char *format, size_t len, bool ANSI, info_buffer out);
bool info_format_Msg_format(info_Msg msg, struct info_format format, bool ANSI, info_buffer out);

struct info_format info_format_select(struct info_format a, struct info_format b);
