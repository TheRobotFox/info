#ifndef INFO_FORMAT_H_
#define INFO_FORMAT_H_

#include "info_gen.h"


#define _INFO_FORMAT_IND()  _INFO_FORMAT
#define _INFO_FORMAT(...)  _INFO_LL(struct info_DrawCall, _INFO_FORMAT_, __VA_ARGS__)
#define FORMAT(...) _INFO_EVAL(_INFO_FORMAT(__VA_ARGS__))

#define _INFO_ARG_IS_PAREN(a) _INFO_CHECK(_INFO_EXPAND(_INFO_PROBE a))

#define _INFO_FORMAT_SET(var, ...) _INFO_FORMAT_CALL(set, STR(var), NUM(2), STR(_INFO_DEFER4(_INFO_FORMAT_IND)()(__VA_ARGS__)))


#define _INFO_ARGS_NUM(x) {.kind = NUMBER, .num = x}
#define _INFO_ARGS_STR(x) {.kind = STRING, .str = {x, sizeof(x)}}
#define _INFO_FORMAT_ARGS(...) _INFO_LL(struct info_Arg, _INFO_ARGS_, __VA_ARGS__)
#define _INFO_FORMAT_CALL(fn, ...) { .kind=CALL, .call={.func=fn, .args= _INFO_DEFER2(_INFO_FORMAT_ARGS)(__VA_ARGS__)} }

#define _INFO_FORMAT_(str, ...) printf(str, __VA_ARGS__)

FORMAT(SET())

#endif // INFO_FORMAT_H_
