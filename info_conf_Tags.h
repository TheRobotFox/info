#pragma once
//
// add own Tags to X-Macro
//
#define INFO_TAGS(x) \
        x(INFO)      \
        x(SUCCESS)   \
        x(ERROR)     \
        x(SEG)       \
        x(FATAL)



#define _ANSI(ENUM) [ENUM] = INFO_ANSI_normal_c,

/* #define _FMT(STRING) [STRING] = {NULL,NULL}, */
