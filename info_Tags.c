#include "info_conf_Tags.h"

enum info_Tags
{
	NONE,

    #define _ENUM(Tag) Tag,
	INFO_TAGS(_ENUM)
    #undef _ENUM

	INFO_COUNT
};

const char* info_Tags_str[]={
[ZERO] = INFO_STR("UNDEFINED"),
    #define _STR(STRING) [STRING] = INFO_STR(#STRING),
}
