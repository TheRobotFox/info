#include "info_internal.h"
#include "stream_internal.h"
#include <stddef.h>
#include <time.h>
#include <locale.h>


struct info_internal_Msg message = { ZERO, {0}, NULL, 0, {1}};
bool hold=false, holding=false, col_set=false, init=true;
struct info_internal_stream output = {0};

#define MAX_SEG_NEST 128
struct Segment
{
	clock_t start;
	const info_char *name;
};

struct Segment seg_data[MAX_SEG_NEST];
size_t seg_index = 0;

void info_seg_begin(const info_char *name)
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

void info_seg_end(const info_char *func_name)
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

void info_mode(struct info_format format)
{
	format_current=format;
}

int info_indent(int n)
{
	message.indentation+=n;
	return message.indentation;
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
	info_drawcall_list_free(message.d);
	message.d=NULL;
	message.current=(ANSI){1};
	message.type=ZERO;
	col_set=false;
	hold=false;
	holding=false;
}

void info_color(ANSI ansi)
{
	message.current=ansi;
	col_set=true;
}

void info_printf(const info_char *format, ...)
{
	if(init)
	{
		holding=true;
		output.f=stdout;
		output.ANSI_support=true;
		//output.formats={0};
		setlocale(LC_ALL,"en_US.UTF-8");
	}

	if(!col_set){
		//info_color(info_ANSI[message.type]);
		message.start=message.current;
	}
	if(!message.d)
		message.d= info_drawcall_list_create();

	va_list args;
	va_start(args, format);
	info_drawcall_vprintf(message.d, message.current, format, args);
	if(!hold)
		info_release();
}
