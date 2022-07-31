#include "buffer.h"
#include "info.h"
#include "stream_internal.h"
#include <stdio.h>

#define INTERNAL_ERROR(...) { printf("[INTERNAL ERROR] In %s: ", __FUNCTION__); printf(__VA_ARGS__); putchar('\n');}
#define INTERNAL(...) {INTERNAL_ERROR(__VA_ARGS__); return true;}
#define PANIC(...) {INTERNAL_ERROR(__VA_ARGS__); exit(1); }
enum info_internal_drawcall_type
{
        ANSI,
        TEXT,
};

struct info_internal_drawcall
{
        enum info_internal_drawcall_type type;
        info_buffer content;
};

struct info_internal_Msg
{
        enum INFO_TYPE type;
        struct info_Origin origin;
        List drawcall_list;
};

extern const char *info_internal_type_ids[];
bool info_internal_drawcall_vprintf(List drawcall_list, enum info_internal_drawcall_type type, const char *format, va_list args);
bool info_internal_drawcall_printf(List drawcall_list, enum info_internal_drawcall_type type, const char *format, ...);
