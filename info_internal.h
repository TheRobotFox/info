#pragma once

#define INTERNAL_ERROR(format, ...) { PRINTF(INFO_STR("[INTERNAL ERROR] %s: "), __func__); PRINTF(INFO_STR(format), ##__VA_ARGS__); FPUTC(INFO_STR('\n'), stdout);}
#define INTERNAL(...) {INTERNAL_ERROR(__VA_ARGS__); return -1;}
#define PANIC(...) {INTERNAL_ERROR(__VA_ARGS__); exit(1); }

/* struct info_Msg */
/* { */
/*         enum INFO_TYPE type; */
/*         struct info_Origin origin; */
/*         drawcall_list d; */
/*         size_t indentation; */
/*         ANSI current; */
/*         ANSI start; */
/* }; */
/* typedef struct info_internal_Msg* info_Msg; */

/* extern const info_char *info_internal_tags[]; */
/* extern const ANSI info_internal_ANSI[]; */
/* extern info_formats info_internal_formats; */
/* extern struct info_format format_current; */
/* extern ANSI ansi_prefix; */
/* extern const size_t tab_width; */
