#ifndef INFO_FORMAT_H_
#define INFO_FORMAT_H_

#include "info_gen.h"

/* #define _INFO_LL(A,B) {A, (int[])B} */

#define _INFO_LL_IND() _INFO_LL_
#define _INFO_LL_(a,b) {a _INFO_IF2(_INFO_IS_END2(b))(_INFO_EMPTY (), , (int[]) b)}
/* #define _INFO_LL(a,b) _INFO_EVAL(_INFO_LL_(a,b)) */

#define _INFO_FORMAT_IND()  _INFO_FORMAT
#define _INFO_FORMAT(...) _INFO_DEFER80(_INFO_LL_IND)() _INFO_RECURSE(_INFO_FORMAT_, __VA_ARGS__)
#define FORMAT(...) _INFO_EVAL1(_INFO_FORMAT(__VA_ARGS__))

#define _INFO_ARG_IS_PAREN(a) _INFO_CHECK(_INFO_EXPAND(_INFO_PROBE a))

#define _INFO_FORMAT_SET(var, ...) _INFO_DEFER100(_INFO_EXPAND)(set: var, _INFO_DEFER4(_INFO_FORMAT_IND)()(__VA_ARGS__))
#define _INFO_FORMAT_(str, ...) printf(str, __VA_ARGS__)

/* FORMAT(SET(a,("TEXT %d", d), GET("STR")), ("AWD"),b) */
/* (_INFO_FORMAT_1, (_INFO_FORMAT_2, (_INFO_FORMAT_3, _END))) */

#endif // INFO_FORMAT_H_
