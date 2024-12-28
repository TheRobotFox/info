#ifndef INFO_DEF_H_
#define INFO_DEF_H_

#include "ANSI.h"
#include "List.h"
#include "info_char.h"
#include "info_string.h"

#define TAB_WIDTH 4

#define FIELDS(f) \
    f(Level)      \
    f(Time)       \
    f(File)       \
    f(Line)       \
    f(Func)

enum info_Field_Type {
    FIELDS(ENUM)
    FIELDS_COUNT
};

struct info_DrawCall;
LIST_INC(struct info_DrawCall, DrawCall)
LIST_INC(struct info_Style, Style)

struct info_DrawCall
{
    enum {
        STYLE,
        TEXT,
        FIELD
    } kind;
    union {
        struct {
            struct List_Style *styles;
            struct List_DrawCall *sub;
        } styled;
        struct info_Slice text;
        enum info_Field_Type field;
    };
};

struct info_Origin
{
	const char *file;
	size_t line;
	const char *func;
};
struct info_Data
{
    struct info_Origin origin;
    const info_char *prefix;
    size_t level, current_len, prefix_len;
};

struct info_Msg{
	struct info_Data data;
    struct List_DrawCall *prefix;
    struct List_DrawCall *content;
};


#endif // INFO_DEF_H_
