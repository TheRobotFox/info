#pragma once
#include "List/List.h"
#include "ANSI.h"
#include <stdarg.h>

// Thanks to https://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
#define INFO_FOREACH_FUNC(FUNC) \
        FUNC(INFO)     		    	\
        FUNC(SUCCESS)	        	\
        FUNC(ERROR)      	    	\
        FUNC(SEG)     	      	\

#define INFO_GENERATE_ENUM(ENUM) ENUM,
#define INFO_GENERATE_ANSI(ENUM) [ENUM] = (struct info_ANSI){1},

enum INFO_TYPE
{
	ZERO,
	INFO_FOREACH_FUNC(INFO_GENERATE_ENUM)


	FATAL,


	INFO_COUNT
};

#undef INFO_GENERATE_ENUM


static ANSI info_ANSI[] = {
	// set defaults
	INFO_FOREACH_FUNC(INFO_GENERATE_ANSI)
	[ZERO] = INFO_ANSI_normal_color(255,255,255),

	[INFO] = INFO_ANSI_normal_color(0, 200, 255),
	[SEG] = INFO_ANSI_normal_color(200, 255, 0),
	[ERROR] = INFO_ANSI_normal_color(200, 70, 70),
	[FATAL] = INFO_ANSI_normal_color(255, 0, 0),
	[SUCCESS] = INFO_ANSI_normal_color(0, 255, 0),
};

struct info_Origin
{
	const char *file;
	size_t line;
	const char *func;
};

void info_printf(const char *format, ...);
void info_Msg_origin(struct info_Origin origin);
void info_Msg_type(enum INFO_TYPE type);
void info_hold(void);
void info_release(void);
void info_color(ANSI ansi);
void info_indent(int n);
void info_seg_begin();
void info_seg_end();
void info_reset();


// MARCOS
#define PRINT(...) info_printf(__VA_ARGS__);
#define INFO_INTERNAL_MSG_SETUP(type) {info_Msg_origin((struct info_Origin){__FILE__, __LINE__, __FUNCTION__}); info_Msg_type(type);}
#define INFO_INTERNAL_MSG(type, ...) {info_release(); INFO_INTERNAL_MSG_SETUP(type); PRINT(__VA_ARGS__)}
#define INFO(...) INFO_INTERNAL_MSG(INFO, __VA_ARGS__)
#define ERROR(...) INFO_INTERNAL_MSG(ERROR, __VA_ARGS__)
#define FATAL(...) {INFO_INTERNAL_MSG(FATAL, __VA_ARGS__) exit(-1);}
#define SUCCESS(...) INFO_INTERNAL_MSG(SUCCESS, __VA_ARGS__)
#define INDENT(n) info_indent(n);
#define SEG_BEGIN(name) {INFO_INTERNAL_MSG_SETUP(SEG) info_seg_begin(name); INDENT(1) }
#define SEG_END {INFO_INTERNAL_MSG_SETUP(SEG) INDENT(-1) info_seg_end();}
#define HOLD info_hold();
#define RELEASE info_release();
#define RESET info_reset();
#define COLOR(r,g,b) {info_color(INFO_ANSI_normal_color(r,g,b));}

