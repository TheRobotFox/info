#include "ANSI.h"
#include "info_string.h"

#define STR(x) [x]=#x,
const char *info_styles_str[STYLES_COUNT] = {
        INFO_STYLES(STR)
};
const char *info_styles_str_short[STYLES_COUNT] = {
        [INVERT] = "IV",
        [STRIKE] = "ST",
        [OVERLINE] = "OL",
        [FAINT] = "FT",
        [BOLD] = "BF",
        [UNDERLINE] = "UL",
        [NORMAL] = "NF",
        [FOREGROUND] = "F(",
        [BACKGROUND] = "B("
};

const int info_styles_val[][2] = {
        [INVERT]    = {27, 7},
        [STRIKE]    = {29, 9},
        [OVERLINE]  = {55,53},
        [FAINT]     = {-1, 2},
        [BOLD]      = {-1, 1},
        [UNDERLINE] = {24, 4},
        [NORMAL]    = {-1, 22},
        [DOUBLE_UNDERLINE] = {24, 21},
        [FOREGROUND] = {39, 38},
        [BACKGROUND] = {49, 48}
};

const char *info_colors_str[COLORS_COUNT] = {
        INFO_COLORS(STR)
};

const struct info_Color info_colors_val[COLORS_COUNT] = {
        [WHITE]      = {.r = 220, .g = 220, .b = 220},
        [GREY]      = {.r = 150, .g = 150, .b = 150},
        [RED]        = {.r = 255, .g = 0  , .b = 0  },
        [GREEN]      = {.r = 0  , .g = 255, .b = 0  },
        [BLUE]       = {.r = 0  , .g = 0  , .b = 255},
        [CYAN]       = {.r = 0  , .g = 200, .b = 255},
        [LIGHTRED]   = {.r = 225, .g = 100 , .b = 100 },
        [YELLOW]     = {.r = 255, .g = 220, .b = 0  },
        [LIGHTGREEN] = {.r = 60 , .g = 180, .b = 60 },
        [LIGHTBLUE]  = {.r = 120 , .g = 120 , .b = 225}
};


void info_ansi_apply(struct info_Style to, struct info_Style from, info_String *str)
{
        if(to.kind == FOREGROUND || to.kind == BACKGROUND){
                if(to.mode==0 && from.mode==0) return;
                if(to.mode==0) {
                        info_string_printf(str, INFO_STR("\033[%dm"), info_styles_val[to.kind][0]);
                } else {
                        struct info_Color a = to.color, b = from.color;
                        if(a.r == b.r
                        && a.g == b.g
                        && a.b == b.b) return; // same color => do nothing
                        info_string_printf(str, INFO_STR("\033[%d;2;%d;%d;%dm"), info_styles_val[to.kind][1], a.r,a.g,a.b);
                }
        } else {
                if(from.mode==to.mode) return;
                if(to.kind == INTENSITY){
                        int res;
                        switch(to.mode){
                                case -1: res = 2; break;
                                case 1: res = 1; break;
                                default: res = 22; break;
                        }
                        info_string_printf(str, INFO_STR("\033[%dm"), res);

                } else {
                        info_string_printf(str, INFO_STR("\033[%dm"), info_styles_val[to.kind][to.mode]);
                }
        }
}
