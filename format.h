#include "info.h"

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
// %[0-9]x custom function
// %[0-9]c use specific contetn stream



// DRAFT 4
// %% -> %
// %<f> == %f | short name
// %{str arg}[num arg]<func_name>
// %[expr args]...<func_name>
// %{name}( -> start substring name
// %) end substring
#define INFO_FORMAT_FORMAT_DEFAULT "[%t][%i]%d %f: %c"
#define INFO_FORMAT_NEWLINE_DEFAULT "\n"

struct info_format
{
        const char *format;
        const char *newline;
};

char *indent "\t";
struct info_format info_foramt_default = { INFO_FORMAT_FORMAT_DEFAULT, INFO_FORMAT_NEWLINE_DEFAULT, true};

typedef struct info_format info_Formats[(enum INFO_TYPE)COUNT];

