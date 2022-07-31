#include "List/List.h"
#include "ANSI.h"
#include <stdarg.h>

// Thanks to https://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
#define INFO_FOREACH_FUNC(FUNC) \
        FUNC(INFO)     		    \
        FUNC(SUCCESS)	        \
        FUNC(ERROR)      	    \
        FUNC(SEG)     	        \

#define INFO_GENERATE_ENUM(ENUM) ENUM,
#define INFO_GENERATE_ANSI(ENUM) [ENUM] = (struct info_ANSI){1},

enum INFO_TYPE
{
	ZERO,
	INFO_FOREACH_FUNC(INFO_GENERATE_ENUM)


	FATAL,


	COUNT
};

#undef INFO_GENERATE_ENUM


static struct info_ANSI info_ANSI[] = {
	// set defaults
	INFO_FOREACH_FUNC(INFO_GENERATE_ANSI)

	[INFO] = INFO_ANSI_normal_color(0, 200, 255),
	[SEG] = INFO_ANSI_normal_color(200, 255, 0),
	[ERROR] = INFO_ANSI_normal_color(200, 70, 70),
	[FATAL] = INFO_ANSI_normal_color(255, 0, 0),
	[SUCCESS] = INFO_ANSI_normal_color(0, 255, 0),
};

struct info_Origin
{
	const char *file;
	size_t line;
	const char *func;
};

void info_printf(const char *format, ...);
void info_Msg_origin(struct info_Origin origin);
void info_Msg_type(enum INFO_TYPE type);
void info_hold(void);
void info_release(void);


// MARCOS
#define PRINT(...)
#define INFO_INTERNAL_MSG_SETUP(type) {info_Msg_origin((struct Origin){__FILE__, __LINE__, __FUNCTION__}); info_Msg_type(type);}
#define INFO_INTERNAL_MSG(type, ...) {INFO_INTERNAL_MSG_SETUP(type); PRINT(__VA_ARGS__)}
#define INFO(...) MSG(INFO, __VA_ARGS__)
#define ERROR(...) MSG(ERROR, __VA_ARGS__)
#define FATAL(...) MSG(FATAL, __VA_ARGS__)
#define SUCCESS(...) MSG(SUCCESS, __VA_ARGS__)
#define SEG_BEGIN(name) {MSG_SETUP(SEG); info_seg_begin(name)}
#define HOLD info_hold();
#define RELEASE info_release();

