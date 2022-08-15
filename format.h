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
#define INFO_FORMAT_FORMAT_DEFAULT "%([%t][%i] %f: %)%<content>"
#define INFO_FORMAT_NEWLINE_DEFAULT "\n%w"

struct info_format
{
        const char *format;
        const char *newline;
};

static char *indent = "\t";
static struct info_format info_format_default = { INFO_FORMAT_FORMAT_DEFAULT, INFO_FORMAT_NEWLINE_DEFAULT};
static ANSI ansi_prefix = {1};

typedef struct info_format info_Formats[(enum INFO_TYPE)COUNT];

