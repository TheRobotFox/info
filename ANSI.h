#pragma once
#include "info_string.h"
#include <stdint.h>
#include "List.h"

#define INFO_STYLES(f)     \
        f(INVERT)     \
        f(STRIKE)     \
        f(OVERLINE)   \
        f(FAINT)      \
        f(ITALIC)      \
        f(BOLD)       \
        f(NORMAL)     \
        f(UNDERLINE)  \
        f(DOUBLE_UNDERLINE)  \
        f(FOREGROUND) \
        f(BACKGROUND)

#define ENUM(x) x,

enum info_Style_Type {
        INFO_STYLES(ENUM)
        STYLES_COUNT,
        INTENSITY
};

extern const char *info_styles_str[STYLES_COUNT];
extern const char *info_styles_str_short[STYLES_COUNT];

#define INFO_COLORS(f) \
        f(WHITE)       \
        f(RED)         \
        f(GREEN)       \
        f(BLUE)        \
        f(CYAN)        \
        f(LIGHTRED)    \
        f(YELLOW)      \
        f(LIGHTGREEN)  \
        f(LIGHTBLUE)


struct info_Color
{
        uint8_t r,g,b;
};

enum info_Color_Type {
        INFO_COLORS(ENUM)
        COLORS_COUNT
};

extern const char *info_colors_str[COLORS_COUNT];
extern const struct info_Color info_colors_val[COLORS_COUNT];

#define INFO_ANSI_COLOR(R,G,B) (const struct info_ANSI_color){R,G,B}

struct info_Style
{
        enum info_Style_Type kind;
        union{
                int mode;
                struct info_Color color;
        };
};

typedef struct info_ANSI ANSI;

#define INFO_ANSI_normal_c INFO_ANSI_normal_color_c(175,175,175)
#define INFO_ANSI_normal INFO_ANSI_normal_color(175,175,175)

void info_ansi_apply(struct info_Style to, struct info_Style from, info_String *str);
