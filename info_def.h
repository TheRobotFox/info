#ifndef INFO_DEF_H_
#define INFO_DEF_H_

#include "ANSI.h"
#include "Map.h"
#include "arena.h"
#include "info_conf.h"
#include "info_char.h"
#include "info_string.h"
#include <stdio.h>
#include <stdlib.h>

LIST_INC(void*, ptr);

struct info_DrawCall;
LIST_INC(struct info_DrawCall, DrawCall);

struct info_Arg {
    enum{
        NUMBER,
        STRING
    } kind;
    union{
        int num;
        const info_char *str;
    };
};
struct info_Var {
    const char *name;
    struct info_Arg arg;
};
MAP_INC(const char *, struct info_Var, vars);

struct info_Msg;

struct info_DrawCall {
    enum {
        STYLE,
        TEXT,
        CALL,
    } kind;
    union {
        struct {
            ANSI style;
            struct List_DrawCall *sub;
        } style;
        const info_char *text;
        struct {
            struct List_DrawCall*
            (*func)(struct info_Arg *,
                    size_t,
                    List_vars vars,
                    struct info_Arena *arena);

            struct info_Arg *args;
            size_t argc;
        };
    };
};

struct info_Origin
{
	const char *file;
	size_t line;
	const char *func;
};
struct info_Msg{
	FILE *destination;
    List_DrawCall format;
	struct info_Origin origin;
    struct List_vars *vars;
    struct info_Arena arena;
};


#endif // INFO_DEF_H_
