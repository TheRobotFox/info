#include "info_internal.h"
#include "stream_internal.h"
#include <stddef.h>
#include <time.h>

#define INFO_GENERATE_STRING(STRING) [STRING] = #STRING,

const char *info_internal_type_ids[] = {
	INFO_FOREACH_FUNC(INFO_GENERATE_STRING)
	[FATAL] = "FATAL ERROR"
};

struct info_internal_Msg message = { ZERO, {0}, NULL, 0, {1}};
bool hold=false, holding=false, col_unset, init=true;
struct info_internal_stream output = {0};
extern size_t last_prefix_buff_length;

#define MAX_SEG_NEST 128
struct Segment
{
	clock_t start;
	const char *name;
};

struct Segment seg_data[MAX_SEG_NEST];
size_t seg_index = 0;

void info_seg_begin(const char *name)
{
	HOLD
	PRINT("Segment ");

	if(name)
		PRINT("'%s' ", name);
	PRINT("started!");

	if(seg_index>=MAX_SEG_NEST){
		PRINT("Cannot record time, MAX_SEG_NEST (%d) reached", MAX_SEG_NEST)
		goto end;
	}

	seg_data[seg_index].start=clock();
	seg_data[seg_index].name=name;

	end:
	RELEASE
	seg_index++;
}

void info_seg_end(const char *func_name)
{
	seg_index--;

	HOLD
	PRINT("Segment ");
	if(seg_index<MAX_SEG_NEST){
		if(seg_data[seg_index].name)
			PRINT("'%s' ", seg_data[seg_index].name);
	}
	PRINT("has ended");
	if(seg_index<MAX_SEG_NEST)
		PRINT(" %gms", (double)(clock()-seg_data[seg_index].start)/CLOCKS_PER_SEC*1000);

	PRINT("!")
	RELEASE
}

void info_indent(int n)
{
	if(!n)
		message.indentation=0;
	message.indentation+=n;
}

void info_reset()
{
	last_prefix_buff_length=0;
}

void info_Msg_origin(struct info_Origin origin)
{
	message.origin=origin;
}

void info_Msg_type(enum INFO_TYPE type)
{
	message.type=type;
}

void info_hold(void)
{
	if(holding)
		info_release();
	hold=true;
}

void info_release(void)
{
	if(!holding)
		return;

	info_internal_stream_output(&output, &message);
	for(struct info_internal_drawcall* start = List_start(message.drawcall_list),
																	 * end = List_end(message.drawcall_list);
																	 start!=end; start++)
	{
		info_internal_buffer_free(start->content);
	}
	List_free(message.drawcall_list);
	message.drawcall_list=NULL;
	message.current=(ANSI){1};
	message.type=ZERO;
	col_unset=true;
	hold=false;
	holding=false;
}

void info_color(ANSI ansi)
{
	message.current=ansi;
	col_unset=false;
}

bool info_internal_drawcall_vprintf(List drawcall_list, enum info_internal_drawcall_content_stream stream, ANSI ansi, const char *format, va_list args)
{
	struct info_internal_drawcall* drawcall = List_append(drawcall_list, NULL);
	drawcall->content_stream=stream;
	drawcall->ansi=ansi;
	drawcall->content = info_internal_buffer_create(20);
	return info_internal_buffer_vprintf(drawcall->content, format, args);
}

bool info_internal_drawcall_printf(List drawcall_list, enum info_internal_drawcall_content_stream stream, ANSI ansi, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	bool ret = info_internal_drawcall_vprintf(drawcall_list, stream, ansi, format, args);
	va_end(args);
	return ret;
}

void info_printf(const char *format, ...)
{
	if(init)
	{
		holding=true;
		output.f=stdout;
		output.ANSI_support=true;
		for(int i=ZERO; i<INFO_COUNT; i++)
			output.format[i]=info_format_default;
	}

	if(col_unset){
		info_color(info_ANSI[message.type]);
		message.start=message.current;
	}
	if(!message.drawcall_list)
		message.drawcall_list = List_create(sizeof(struct info_internal_drawcall));

	va_list args;
	va_start(args, format);
	info_internal_drawcall_vprintf(message.drawcall_list, TEXT, message.current, format, args);
	if(!hold)
		info_release();
}
