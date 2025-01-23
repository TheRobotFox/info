#ifndef INFO_DEF_H_
#define INFO_DEF_H_

#include "info_string.h"
struct info_Origin
{
	const char *file;
	size_t line;
	const char *func;
};

struct info_Data
{
    struct info_Origin origin;
    const info_char *prefix;
    size_t level, current_len, prefix_len;
};

struct info_Msg{
	struct info_Data data;
    info_String str;
};

#endif // INFO_DEF_H_
