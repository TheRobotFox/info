#pragma once
#include <stdarg.h>
#include "info_def.h"

void info_printf(struct info_Origin, const info_char *prefix, const info_char *format, ...);
info_String info_render(struct info_Msg msg);
struct List_DrawCall* info_parse(const info_char *text);
void info_seg_begin();
void info_seg_end();

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

#define _PRINTF(...) info_printf((struct info_Origin){__FILE__, __LINE__, __func__}, __VA_ARGS__)
#define _PREFIX(TAG) "[{Time}]" TAG " {F(200,200,120):{Func}}: {Level}"

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
#define FATAL(...) _PRINTF(_PREFIX("[{F(RED):FATAL}] {UL:{File}:{Line}} in"), __VA_ARGS__)
#endif
