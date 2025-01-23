#include "info.h"
#include "info_internal.h"
#include "info_def.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECIMAL 10
#define DEFAULT_STR_SIZE (1<<10)


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
    col->r = strtoul(text, &end, DECIMAL);
    if(!(*end == ',')) return NULL;
    col->g = strtoul(end+1, &end, DECIMAL);
    if(!(*end == ',')) return NULL;
    col->b = strtoul(end+1, &end, DECIMAL);
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

static const info_char* info_parse_styles(List_Style *list, const info_char *text, const info_char *end)
{
    while( text < end ){
        while(*text == INFO_STR(' ')) ++text;

        struct info_Style style;
        const info_char *next;
        if(!(( next = parse_style(&style, text, info_styles_str_short) )
            || ( next = parse_style(&style, text, info_styles_str) ))) return NULL;
        text = next;

        List_Style_push(list, style);

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
        const info_char *content_start = col+1, *content_end = end-1;
        while(*content_start==INFO_STR(' ')) content_start++;
        while(*content_end==INFO_STR(' ')) content_end--;
        dc->styled.sub = parse(content_start, content_end+1);
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

static const info_char* next_curly(const info_char *text, const info_char *end){
    while(text<end) {
        if(*text==INFO_STR('{')) return text;
        if(*text==INFO_STR('}')) return text;
        text++;
    };
    return NULL;
}
static const info_char* find_group_end(const info_char *text, const info_char *end){
    int lvl = 0;
    for(;;text++) {
        text = next_curly(text, end);
        if(!text) return NULL;
        if(text<end-1 && text[0]==text[1]) {
            text++;
        continue;
        }

        if(*text=='{') lvl++;
        else           lvl--;
        if(!lvl) return text;
    }
}

static struct List_DrawCall* parse(const info_char *text, const info_char *end)
{
    if(text>=end) return NULL;

    const info_char *next = next_curly(text, end);
    struct List_DrawCall *list;
    int mark_error = 0;
    struct info_DrawCall dc = { 0 };

    if(!next){
        list = NULL;
        next = end;
    } else {

        if(next[0]==next[1]) {
            // escape characters

            list = parse(next+2, end);
            dc = (struct info_DrawCall){.kind = TEXT, .text = {.str = next, .len = 1}};

        } else if(*next==INFO_STR('{')) {
            const info_char *cb = find_group_end(next, end);
            if(!cb){
                list = NULL;
                mark_error = 1;
                dc = (struct info_DrawCall){.kind = TEXT, .text = {.str = next, .len = end-next}};
            } else {

                list = parse(cb+1, end);
                if(!info_parse_group(&dc, next+1, cb)){
                    mark_error = 1;
                    dc = (struct info_DrawCall){.kind = TEXT, .text = {.str = next, .len = cb-next}};
                }
            }

        } else if(*next==INFO_STR('}')){
            dc = (struct info_DrawCall){.kind = TEXT, .text = {.str = "<}>", .len = 3}};
            mark_error = 1;
        }
        if(mark_error){

            // Mark Error Red
            struct info_DrawCall error = { 0 };
            error.kind = STYLE;
            List_Style_push(&error.styled.styles, (struct info_Style){.kind=FOREGROUND, .color={255,0,0}});
            List_Style_push(&error.styled.styles, (struct info_Style){.kind=STRIKE, .mode=1});
            List_DrawCall_push(&error.styled.sub, dc);
            List_DrawCall_push(&list, error);
        } else {
            List_DrawCall_push(&list, dc);
        }
    }
       
    List_DrawCall_push(&list, (struct info_DrawCall){.kind = TEXT, .text = {.str = text, .len = next-text}});
    return list;
}

struct List_DrawCall* info_parse(const info_char *text)
{
    return parse(text, text+strlen(text));
}

LIST_INC(struct List_Style*, Styles)
LIST_IMPL(struct List_Style*, Styles)

static struct info_Style style_get_prev(enum info_Style_Type type, List_Styles *history)
{
        List_Styles ptr = *history;
        struct List_Style **elem;
        while(( elem = List_Styles_next(&ptr) )){
            struct List_Style *ptr = *elem;

            struct info_Style *current;
            while(( current = List_Style_next(&ptr) )){
                if(current->kind == type){
                        return *current;
                }
            }
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
        len = (n*TAB_WIDTH)-offset;
    info_char *buf = malloc(sizeof(info_char)*len);
    for(size_t i=0; i<len; i++) buf[i] = ' ';
    info_string_puts(str, buf, len);
    data->current_len += len;
    free(buf);
}

static void info_render_list(List_DrawCall *list,
                             info_String *str,
                             List_Styles *history,
                             struct info_Data *data);

static void info_drawcall_render(struct info_DrawCall *dc,
                                 info_String *str,
                                 List_Styles *history,
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
                    data->current_len = data->prefix_len;
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
            List_Style ptr = dc->styled.styles;
            struct info_Style *current;
            while( (current = List_Style_next(&ptr)) ){
                struct info_Style prev = style_get_prev(current->kind, history);
                info_ansi_apply(*current, prev, str); // activate
            }

            List_Styles_push(history, dc->styled.styles);
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

static void info_render_list(List_DrawCall *list,
                             info_String *str,
                             List_Styles *history,
                             struct info_Data *data)
{
    while(*list){
        struct info_DrawCall dc = List_DrawCall_pop(list);
        info_drawcall_render(&dc, str, history, data);
    }
}

struct Segment{
    const char *name;
    struct timespec start;
};

LIST_INC(struct Segment, seg)
LIST_IMPL(struct Segment, seg)

FILE *out = NULL;
static struct info_Msg msg = { 0 };
static int hold = 0, holding = 0;
static List_seg segments = NULL;

void info_msg(struct info_Origin origin, const info_char *prefix)
{
    if(msg.str.str) free(msg.str.str);

    msg = (struct info_Msg){ 0 };
    msg.data.origin = origin;
    msg.data.level = List_seg_length(&segments);

    struct List_DrawCall *list = info_parse(prefix);
    struct List_Styles *history = NULL;
    info_String str = info_string_create(DEFAULT_STR_SIZE);
    info_render_list(&list, &str, &history, &msg.data);
    msg.data.prefix_len = msg.data.current_len;
    msg.str = str;
}

void info_printf(const info_char *format, ...)
{
    if(!out) out = stderr;

    if(!holding && msg.str.str )
        FPUTS(msg.str.str, out);

    info_String rendered = info_string_create(DEFAULT_STR_SIZE);

    va_list args;
    va_start(args, format);
    info_string_vprintf(&rendered, format, args);

    va_end(args);

    struct List_DrawCall *list = info_parse(rendered.str);
    struct List_Styles *history = NULL;
    info_String str = info_string_create(DEFAULT_STR_SIZE);
    info_render_list(&list, &str, &history, &msg.data);

    FPUTS(str.str, out);

    free(rendered.str);

    if(!hold){
        FPUTC('\n', out);
        holding = 0;
    }else{
        holding = 1;
    }
    fflush(out);
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

void info_seg_begin(const char *name, struct info_Origin org)
{
    info_msg(org, _PREFIX("[{F(LIGHTGREEN):SEG}]"));
    info_printf("Begin Segment '{BF:%s}'", name);
    struct timespec t = {0};
    timespec_get(&t, TIME_UTC);
    List_seg_push(&segments, (struct Segment){name, t});
}
void info_seg_end(struct info_Origin org)
{
    struct Segment s = List_seg_pop(&segments);
    struct timespec now = {0};
    timespec_get(&now, TIME_UTC);
    info_msg(org, _PREFIX("[{F(LIGHTGREEN):SEG}]"));
    info_printf("End Segment '{BF:%s}' took %.3fs", s.name,
                (now.tv_sec - s.start.tv_sec)
                + ((now.tv_nsec - s.start.tv_nsec) / 1000000000.0));
}
