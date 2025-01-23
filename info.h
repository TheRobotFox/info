#pragma once
#include <stdarg.h>
#include "info_def.h"


#define TAB_WIDTH 4

void info_msg(struct info_Origin origin, const info_char *prefix);
void info_printf(const info_char *format, ...);
info_String info_render(struct info_Msg msg);
struct List_DrawCall* info_parse(const info_char *text);
void info_seg_begin(const char *name, struct info_Origin org);
void info_seg_end(struct info_Origin org);
void info_hold(int);

#ifndef INFO_LVL
#define INFO_LVL 3
#endif

// MARCOS
#if INFO_LVL<3
#define INFO() ;
#endif
#if INFO_LVL<2
#define SUCCESS(...) ;
#define SEG_BEGIN(name) ;
#define SEG_END ;
#define INDENT(n) 0
#endif
#if INFO_LVL<1
#define HOLD(...)
#define ERROR(...) ;
#endif
#if INFO_LVL<0
#define FATAL(...) ;
#endif

#define _ORIGIN (struct info_Origin){__FILE__, __LINE__, __func__}
#define _MSG(PREFIX) info_msg(_ORIGIN, PREFIX);
#define _PRINTF(P,...) do{_MSG(P); info_printf(__VA_ARGS__);}while(0)
#define _PREFIX(TAG) "[{Time}]" TAG " {F(200,200,120): {Func} }: {Level}"
#define HOLD info_hold(1);
#define RELEASE info_hold(0);
#define PRINT(...) info_printf(__VA_ARGS__)

#ifndef INFO
#define INFO(...) _PRINTF(_PREFIX("[{F(CYAN):INFO}]"), __VA_ARGS__ )
#endif
#ifndef WARNING
#define WARNING(...) _PRINTF(_PREFIX("[{F(YELLOW):WARNING}]"), __VA_ARGS__ )
#endif
#ifndef SUCCESS
#define SUCCESS(...) _PRINTF(_PREFIX("[{F(GREEN):SUCCESS}]"), __VA_ARGS__)
#endif
#ifndef ERROR
#define ERROR(...) _PRINTF(_PREFIX("[{F(LIGHTRED):ERROR}]"), __VA_ARGS__)
#endif
#ifndef FATAL
#define FATAL(...) _PRINTF(_PREFIX("[{F(RED):FATAL}] {UL:{File}: {Line} } in"), __VA_ARGS__)
#endif
#ifndef SEG_BEGIN
#define SEG_BEGIN(name) info_seg_begin(name, _ORIGIN)
#endif
#ifndef SEG_END
#define SEG_END info_seg_end(_ORIGIN)
#endif
