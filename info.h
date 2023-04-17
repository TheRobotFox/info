#pragma once
#include "List/List.h"
#include "ANSI.h"
#include <wchar.h>
#include <stdarg.h>

// Thanks to https://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
#define INFO_GENERATE_ENUM(ENUM) ENUM,

#include "info_conf.h"

#undef INFO_GENERATE_ENUM

#define INFO_INTERNAL_CAT_X(a, b) a##b
#ifdef INFO_WIDE
  #define INFO_STR(str) L##str
  #define INFO_STR_X(str) INFO_INTERNAL_CAT_X(L,str)
	#define STRLEN wcslen
	#define STRTOL wcstol
	#define STRCMP wcsncmp
	#define FPUTS fputws
	#define FPUTC fputwc
	#define PRINTF wprintf
	typedef wchar_t info_char;
#else
  #define INFO_STR(str) str
	#define INFO_STR_X INFO_STR
	#define STRLEN strlen
	#define STRTOL strtol
	#define STRCMP strncmp
	#define FPUTS fputs
	#define FPUTC fputc
	#define PRINTF printf
	typedef char info_char;
#endif

#include "format.h"
extern const struct info_format format_default;
extern const struct info_format format_structured;

struct info_Origin
{
	const info_char *file;
	size_t line;
	const char *func;
};

void info_printf(const info_char *format, ...);
void info_Msg_origin(struct info_Origin origin);
void info_Msg_type(enum INFO_TYPE type);
void info_hold(void);
void info_release(void);
void info_color(ANSI ansi);
int info_indent(int n);
void info_seg_begin();
void info_seg_end();
void info_mode(struct info_format format);


// MARCOS
#define PRINT(fmt,...) info_printf(INFO_STR(fmt), ##__VA_ARGS__);
#define INFO_INTERNAL_MSG_SETUP(type) {info_Msg_origin((struct info_Origin){INFO_STR_X(__FILE__), __LINE__, __func__}); info_Msg_type(type);}
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
#define COLOR(r,g,b) {info_color(INFO_ANSI_normal_color(r,g,b));}
#define MODE(m) info_mode(m);

