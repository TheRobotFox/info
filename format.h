#pragma once
#include "info.h"
#include "ANSI.h"

//DRAFT 1
// MESSAGE INFO
// %t Timestamp
// %i INFO TAG
// %f Caller function
// %F Origin file
// %l line number
// %d put draw calls indentation
// %o last content_offset
//
// %c print content stream 0
//
// SUBSTRINGS
// %( open substring
// %) close substring
// %[0-9]p print substring
// %[0-9]w print substring as whitespaces
//
// REFLECTION
// %D get indentation depth
// %L get new line counter
//
// %% %

// TODO:
// %[0-9]c use specific contetn stream
// INFO Tag func
// SUBSTRINGS
// ANSI impl
// Refactor format solve
// test run O.o
// usw


// DRAFT 4
// %% -> %
// %<f> == %f | short name
// %{str arg}[num arg]<func_name>
// %[expr args]...<func_name>
// %{name}( -> start substring name
// %) end substring

struct info_format
{
        const info_char *format;
        const info_char *newline;
};

typedef struct info_format info_formats[INFO_COUNT];
