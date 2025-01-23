#ifndef INFO_INTERNAL_H_
#define INFO_INTERNAL_H_

#include "ANSI.h"
#include "List.h"

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
            List_Style styles;
            List_DrawCall sub;
        } styled;
        struct info_Slice text;
        enum info_Field_Type field;
    };
};

#endif // INFO_INTERNAL_H_
