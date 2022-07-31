#include "info.h"
#include "info_internal.h"
#include "stream.h"
#include <stddef.h>

#define INFO_GENERATE_STRING(STRING) [STRING] = #STRING,

const char *info_internal_type_ids[] = {
	INFO_FOREACH_FUNC(INFO_GENERATE_STRING)
	[FATAL] = "FATAL ERROR"
};

struct info_internal_Msg message;
bool hold;
info_stream output;

void info_Msg_origin(struct info_Origin origin)
{
	message.origin=origin;
};

void info_Msg_type(enum INFO_TYPE type)
{
	message.type=type;
};

void info_hold(void)
{
	hold=true;
};

void info_release(void)
{
	hold=false;
	info_internal_stream_output(output, &message);
};

bool info_internal_drawcall_vprintf(List drawcall_list, enum info_internal_drawcall_type type, const char *format, va_list args)
{
	struct info_internal_drawcall* drawcall = List_append(drawcall_list, NULL);
	drawcall->type=type;
	info_internal_buffer_vprintf(drawcall->content, format, args);
}
bool info_internal_drawcall_printf(List drawcall_list, enum info_internal_drawcall_type type, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	info_internal_drawcall_vprintf(drawcall_list, type, format, args);
}

void info_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	info_internal_drawcall_vprintf(message.drawcall_list, format, args);
	if(!hold)
		info_release();
}
