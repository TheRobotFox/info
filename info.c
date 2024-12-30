#include "ANSI.h"
#include "List.h"
#include "info_char.h"
#include "info_def.h"
#include "info_string.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define STR(x) [x]=#x,
const char *field_str[FIELDS_COUNT] = {
FIELDS(STR)
};

LIST_IMPL(struct info_DrawCall, DrawCall)
LIST_IMPL(struct info_Style, Style)

static const info_char* parse_color(struct info_Color *col, const info_char *text)
{
    for(size_t i=0; i<COLORS_COUNT; i++){
        size_t len = strlen(info_colors_str[i]);
        if(!STRCMP(info_colors_str[i], text, len)){
            *col = info_colors_val[i];
            return text+len;
        }
    }
    char *end;
    col->r = strtoul(text, &end, 10);
    if(!(*end == ',')) return NULL;
    col->g = strtoul(end+1, &end, 10);
    if(!(*end == ',')) return NULL;
    col->b = strtoul(end+1, &end, 10);
    if(!(*end == ')')) return NULL;
    return end;
}

static const info_char* parse_style(struct info_Style *style, const info_char *text, const char *styles[])
{
    style->kind = 0;
    style->mode = 1;
    if(*text == INFO_STR('!')){
        style->mode = 0;
        text++;
    }

    for(;style->kind<STYLES_COUNT; style->kind++){
        if(!styles[style->kind]) continue;
        size_t len = strlen(styles[style->kind]);
        if(!strncmp(text, styles[style->kind], len)){
            text += len;
            switch(style->kind){
            case FOREGROUND:
            case BACKGROUND:
                if( !style->mode || !(text = parse_color(&style->color, text)) ) return NULL;
                if(*text++!=')') return NULL;
                break;
            case NORMAL:
                style->mode = 0;
                style->kind = INTENSITY;
                break;
            case BOLD:
                style->mode = 1;
                style->kind = INTENSITY;
                break;
            case FAINT:
                style->mode = -1;
                style->kind = INTENSITY;
                break;
            default:
                break;
            }
            return text;
        }
    }
    return NULL;
}

static const info_char* info_parse_styles(List_Style list, const info_char *text, const info_char *end)
{
    while( text < end ){
        while(*text == INFO_STR(' ')) ++text;

        struct info_Style style;
        const info_char *next;
        if(!(( next = parse_style(&style, text, info_styles_str_short) )
            || ( next = parse_style(&style, text, info_styles_str) ))) return NULL;
        text = next;

        List_Style_insert(list, style);

        while(*text == INFO_STR(' ')) ++text;

        if(!(*text == INFO_STR(',') || *text == INFO_STR(':'))) return NULL;
        text++;
    }
    return text;
}

static struct List_DrawCall* parse(const info_char *text, const info_char *end);
static const info_char* info_parse_group(struct info_DrawCall *dc, const info_char *text, const info_char *end)
{
    const info_char *col = STRSTR(text, ":");
    if(col && col < end){
        if(!info_parse_styles(&dc->styled.styles, text, col) ){
                while(dc->styled.styles) List_Style_pop(&dc->styled.styles);
                return NULL;
        }
        dc->styled.sub = parse(col+1, end);
    } else {
        // parse Field
        dc->kind = FIELD;
        for(size_t i=0; i<FIELDS_COUNT; i++){
            size_t len = end-text;
            if(!STRCMP(text, field_str[i], len)){
                dc->field = i;
                return text+len;
            }
        }
    }
    return end;
}

static const info_char* next_cb(const info_char *text, const info_char *end){
    int lvl = 0;
    do {
        if(*text==INFO_STR('{')){
            if(*(text+1)==INFO_STR('{')) text+=2;
            else lvl++;
        }
        if(*text==INFO_STR('}')) lvl--;
    } while(lvl && ++text<end);
    return text;
}
static struct List_DrawCall* parse(const info_char *text, const char *end)
{
    struct List_DrawCall *list = NULL;
    if(text>=end) return list;

    const char *ob=STRSTR(text, "{");
    if(ob==NULL || ob>end) ob = end;
    else if(ob[1] == '{'){
        list = parse(ob+2, end);
        List_DrawCall_insert(&list, (struct info_DrawCall){.kind = TEXT, .text = {.str = "{", .len = 1}});
        return list;
    }

    if(ob!=text){
        if(ob < end && ob[-1]!='{')
            list = parse(ob, end);
        end = ob < end ? ob : end;
        return (List_DrawCall_insert(&list, (struct info_DrawCall){.kind = TEXT, .text = {.str = text, .len = end - text}}),
            list);
    }

    const char *cb = next_cb(ob, end);

    list = parse(cb+1, end);
    struct info_DrawCall dc = { 0 };
    if(( ob < cb && cb < end)){

        if(info_parse_group(&dc, text+1, cb)){
            List_DrawCall_insert(&list, dc);
            return list;
        }
    }
    // Mark Error Red
    dc.kind = STYLE;
    dc.styled.sub = NULL;
    dc.styled.styles = NULL;
    List_Style_insert(&dc.styled.styles, (struct info_Style){.kind=FOREGROUND, .color={255,0,0}});
    List_Style_insert(&dc.styled.styles, (struct info_Style){.kind=STRIKE, .mode=1});
    List_DrawCall_insert(&dc.styled.sub, (struct info_DrawCall){.kind = TEXT, .text = {.str = text, .len = cb+1 - text}}); //TODO FIXME on error not stoping strike through
    List_DrawCall_insert(&list, dc);
    return list;
}

struct List_DrawCall* info_parse(const info_char *text)
{
    return parse(text, text+strlen(text));
}

LIST_INC(struct List_Style*, Styles)
LIST_IMPL(struct List_Style*, Styles)

static struct info_Style style_get_prev(enum info_Style_Type type, List_Styles history)
{
        struct List_Styles *layers = *history;
        struct List_Style **layer;
        while(( layer = List_Styles_get(&layers) )){

            struct info_Style *current;
            while(( current = List_Style_get(layer) )){
                if(current->kind == type){
                        return *current;
                }
            }
            layers = List_Styles_next(&layers);
        }

        struct info_Style res = {.kind = type};
        if(type == INTENSITY) res.mode = 0;
        else if(type == FOREGROUND || type == BACKGROUND) res.color = info_colors_val[WHITE];
        return res;
}

static void info_tabulate(size_t n, info_String *str, struct info_Data *data)
{
    if(!n) return;
    size_t offset = data->current_len%TAB_WIDTH,
        len = n*TAB_WIDTH-offset;
    info_char *buf = malloc(sizeof(info_char)*len);
    for(size_t i=0; i<len; i++) buf[i] = ' ';
    info_string_puts(str, buf, len);
    data->current_len += len;
    free(buf);
}

static void info_render_list(List_DrawCall list,
                             info_String *str,
                             List_Styles history,
                             struct info_Data *data);

static void info_drawcall_render(struct info_DrawCall *dc,
                                 info_String *str,
                                 List_Styles history,
                                 struct info_Data *data)
{
    switch(dc->kind){
        case TEXT:{
            const info_char *ptr = dc->text.str;
            for(size_t i=0; i<dc->text.len; i++){
                if(dc->text.str[i]=='\n'){
                    size_t len = dc->text.str+i-ptr+1;
                    info_string_puts(str, ptr, len);
                    data->current_len += len;
                    ptr = dc->text.str+i+1;

                    // indent to prefix
                    info_char *buf = malloc(sizeof(info_char)*data->prefix_len);
                    for(size_t i=0; i<data->prefix_len; i++) buf[i] = ' ';
                    info_string_puts(str, buf, data->prefix_len);
                    data->current_len += data->prefix_len;
                    free(buf);
                } else if(dc->text.str[i] == '\t'){
                    info_tabulate(1, str, data);
                    ptr = dc->text.str+i+1;
                } else if(dc->text.str[i] == '\r'){
                    size_t len = dc->text.str+i-ptr;
                    info_string_puts(str, ptr, len);
                    info_string_puts(str, "\\r", 2);
                    ptr = dc->text.str+i+1;
                    data->current_len += 2+len;
                }
            }
            size_t len = dc->text.str+dc->text.len-ptr;
            info_string_puts(str, ptr, len);
            data->current_len += len;
        } break;
        case STYLE:{
            struct List_Style *ptr = dc->styled.styles;
            struct info_Style *current;
            while( (current = List_Style_get(&ptr)) ){
                struct info_Style prev = style_get_prev(current->kind, history);
                info_ansi_apply(*current, prev, str); // activate
                ptr = List_Style_next(&ptr);
            }

            List_Styles_insert(history, dc->styled.styles);
            info_render_list(&dc->styled.sub, str, history, data);
            List_Styles_pop(history);

            while(dc->styled.styles){
                struct info_Style current = List_Style_pop(&dc->styled.styles),
                    prev = style_get_prev(current.kind, history);
                info_ansi_apply(prev, current, str); // deactivate
            }
        } break;
        case FIELD:
            switch (dc->field) {
                case File:
                    info_string_puts(str, data->origin.file, strlen(data->origin.file));
                    data->current_len += strlen(data->origin.file);
                    break;
                case Func:
                    info_string_puts(str, data->origin.func, strlen(data->origin.func));
                    data->current_len += strlen(data->origin.func);
                    break;
                case Line:
                    data->current_len += info_string_printf(str, "%d", data->origin.line);
                    break;
                case Time:{
                    time_t time_rel;
                    struct tm *tm;
                    time(&time_rel);
                    tm = localtime(&time_rel);
                    data->current_len += info_string_printf(str, INFO_STR("%d:%.2d:%.2d"), tm->tm_hour, tm->tm_min, tm->tm_sec);
                    } break;
                case Level:{
                    info_tabulate(data->level, str, data);
                } break;
                default: break;
            }
    }
}

static void info_render_list(List_DrawCall list,
                             info_String *str,
                             List_Styles history,
                             struct info_Data *data)
{
    while(*list){
        struct info_DrawCall dc = List_DrawCall_pop(list);
        info_drawcall_render(&dc, str, history, data);
    }
}

FILE *out = NULL;
struct info_Msg msg = { 0 };
int hold = 0, holding = 0;

void info_msg(struct info_Origin origin, const info_char *prefix)
{
    if(msg.str.str) free(msg.str.str);

    msg = (struct info_Msg){ 0 };
    msg.data.origin = origin;

    struct List_DrawCall *list = info_parse(prefix);
    struct List_Styles *history = NULL;
    info_String str = info_string_create(2<<10);
    info_render_list(&list, &str, &history, &msg.data);
    msg.data.prefix_len = msg.data.current_len;
    msg.str = str;
}

void info_printf(const info_char *format, ...)
{
    if(!out) out = stderr;

    if(!holding)
        FPUTS(msg.str.str, out);

    info_String rendered = info_string_create(1<<10);

    va_list args;
    va_start(args, format);
    info_string_vprintf(&rendered, format, args);

    va_end(args);

    struct List_DrawCall *list = info_parse(rendered.str);
    struct List_Styles *history = NULL;
    info_String str = info_string_create(1<<10);
    info_render_list(&list, &str, &history, &msg.data);

    FPUTS(str.str, out);

    free(rendered.str);

    if(!hold){
        FPUTC('\n', out);
        holding = 0;
    }else{
        holding = 1;
    }
    free(str.str);
}

void info_hold(int set)
{
    if(!set && holding){
        FPUTC('\n', out);
        holding = 0;
    }
    hold = set;
}
