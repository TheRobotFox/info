#ifndef INFO_CHAR_H_
#define INFO_CHAR_H_


#define INFO_INTERNAL_CAT_X(a, b) a##b

#ifdef INFO_WCHAR
  #include <wchar.h>
  #define INFO_STR(str) L##str
  #define INFO_STR_X(str) INFO_INTERNAL_CAT_X(L,str)
	#define STRLEN wcslen
	#define STRTOL wcstol
	#define STRCMP wcsncmp
	#define FPUTS fputws
	#define FPUTC fputwc
	#define PRINTF wprintf
	typedef wchar_t info_char;
#else
    #define INFO_STR(str) str
	#define INFO_STR_X INFO_STR
	#define STRLEN strlen
	#define STRTOL strtol
	#define STRCMP strncmp
	#define FPUTS fputs
	#define FPUTC fputc
	#define PRINTF printf
	typedef char info_char;
#endif


#endif // INFO_CHAR_H_
