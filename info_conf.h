#pragma once
// configure info
//
// use wide characters
//#define INFO_WIDE
//
//
// define your types here
#define INFO_FOREACH_FUNC(FUNC) \
        FUNC(INFO)              \
        FUNC(SUCCESS)           \
        FUNC(ERROR)             \
        FUNC(SEG)               \
        FUNC(FATAL)
//
enum INFO_TYPE
{
	ZERO,
	INFO_FOREACH_FUNC(INFO_GENERATE_ENUM)
//

//
	INFO_COUNT
};
//
//
//
//
//
//
//
