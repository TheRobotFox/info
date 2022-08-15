#pragma once
#include <stdint.h>

struct info_ANSI_color
{
        uint8_t r,g,b;
};

#define INFO_ANSI_COLOR(R,G,B) (struct info_ANSI_color){R,G,B}


struct info_ANSI
{
        uint8_t normal:1; // 0
        uint8_t invert:1; // 7 | 27
        uint8_t strike:1; // 9 | 29
        uint8_t overline:1; // 53 | 5

        int8_t intensity:2; //  2 faint < 0; 22 normal = 0; 1 bold > 0
        uint8_t underline:2; //  24 non | 4 single | 21 double

        struct info_ANSI_color forground,
                               background;
};

typedef struct info_ANSI ANSI;

#define INFO_ANSI_normal_color(R,G,B) (struct info_ANSI){0,0,0,0,0,0, {R,G,B}, { 0,0,0 }}

