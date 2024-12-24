#pragma once
#include <stdarg.h>
#include "info_def.h"

void info_print(struct info_Msg);
int info_indent(int n);
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
#define COLOR(r,g,b)
#define FATAL(...) ;
#endif
