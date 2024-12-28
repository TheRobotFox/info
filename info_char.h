#ifndef INFO_CHAR_H_
#define INFO_CHAR_H_


#define INFO_INTERNAL_CAT_X(a, b) a##b

#ifdef INFO_WCHAR
  #include <wchar.h>
  #define INFO_STR(str) L##str
  #define INFO_STR_X(str) INFO_INTERNAL_CAT_X(L,str)
	#define STRLEN wcslen
	#define STRSTR wcstr
	#define STRTOL wcstol
	#define STRCMP wcsncmp
	#define FPUTS fputws
	#define FPUTC fputwc
	#define PRINTF wprintf
	#define VFPRINTF vfwprintf
	typedef wchar_t info_char;
#else
    #include <string.h>
    #define INFO_STR(str) str
	#define INFO_STR_X INFO_STR
	#define STRLEN strlen
	#define STRSTR strstr
	#define STRTOL strtol
	#define STRCMP strncmp
	#define FPUTS fputs
	#define FPUTC fputc
	#define PRINTF printf
	#define VFPRINTF vfprintf
	typedef char info_char;
#endif


#endif // INFO_CHAR_H_
