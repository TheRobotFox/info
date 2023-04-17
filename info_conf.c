#include "info_internal.h"
#include "format.h"
//
#define INFO_GENERATE_ANSI(ENUM) [ENUM] = (const struct info_ANSI){1},
#define INFO_GENERATE_STRING(STRING) [STRING] = INFO_STR(#STRING),
#define INFO_GENERATE_FORMAT(STRING) [STRING] = {NULL,NULL},
//
//
//
#define FORMAT_DEFAULT INFO_STR("%([%t][%i] %d%{%f}[200,200,120]a: %)%<content>")
#define NEWLINE_DEFAULT INFO_STR("\n%{%g}w")
//
#define FORMAT_STRUCTURED INFO_STR("%s%c")
#define NEWLINE_STRUCTURED INFO_STR("\n%d")
//
//
// set default colors for types
const ANSI info_internal_ANSI[] = {
	// set defaults
	INFO_FOREACH_FUNC(INFO_GENERATE_ANSI)
	[ZERO] = INFO_ANSI_normal_color(255,255,255),
//
	[INFO] = INFO_ANSI_normal_color(0, 200, 255),
	[SEG] = INFO_ANSI_normal_color(200, 255, 0),
	[ERROR] = INFO_ANSI_normal_color(200, 70, 70),
	[FATAL] = INFO_ANSI_normal_color(255, 0, 0),
	[SUCCESS] = INFO_ANSI_normal_color(0, 255, 0),
};
//
// set type names
const info_char *info_internal_tags[] = {
	INFO_FOREACH_FUNC(INFO_GENERATE_STRING)
	[FATAL] = INFO_STR("FATAL ERROR")
};
//
// default formats
info_formats info_internal_formats = {
	INFO_FOREACH_FUNC(INFO_GENERATE_FORMAT)
//
	[FATAL] = {INFO_STR("%([%t][%i] At %{%F}[255][0][0]a:%L in %{%f}[255][0][0]a: %)%c"),
             INFO_STR("\n%{%g}w")},

	[ZERO]  = {INFO_STR("%(%d%)%c"),
						 INFO_STR("\n%{%g}w")}
};
//
// Message prefix ANSI
ANSI ansi_prefix = {1};
//
const info_char *indent = INFO_STR("    ");
//
// example formats
const struct info_format format_default = { FORMAT_DEFAULT, NEWLINE_DEFAULT};
const struct info_format format_structured = { FORMAT_STRUCTURED, NEWLINE_STRUCTURED};
//
struct info_format format_current = format_default;
