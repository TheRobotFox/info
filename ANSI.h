#include <stdint.h>

struct info_ANSI_color
{
        uin8_t r,g,b;
};

struct info_ANSI
{
        uint8_t normal:1; // 0
        uint8_t bold:1; //  1 | 22
        uint8_t underline:1; // 4 | 24
        uint8_t invert:1; // 7 | 27

        struct info_ANSI_color forground,
                                        background;
};


