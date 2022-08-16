#include "ANSI_internal.h"

typedef uint8_t ANSI_rule[3];


static char *info_internal_ANSI_mode_apply(int8_t a, int8_t b)
{
        if(a==b)
                return 0;

}

static bool info_internal_ANSI_color_cmp(struct info_ANSI_color a, struct info_ANSI_color b)
{
        return a.r==b.r && a.g==b.g && a.b==b.b;
}

ANSI current = {1};
ANSI normal = {1};
void info_internal_ANSI_stream_reset(FILE *f)
{
        fwrite("\033[0m", 1, 4, f);
        current=normal;
}
void info_internal_ANSI_switch(info_buffer out, ANSI new)
{
        if(new.normal){
                //if(!current.normal)
                        info_internal_buffer_printf(out, "\033[0m");
                return;
        }
        if(!info_internal_ANSI_color_cmp(new.forground, current.forground))
                info_internal_buffer_printf(out, "\033[38;2;%d;%d;%dm", new.forground.r, new.forground.g, new.forground.b);
        current = new;

}

