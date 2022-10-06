#pragma once
#include "info.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

typedef struct info_internal_buffer *info_buffer;

info_buffer info_internal_buffer_create(size_t size);
bool info_internal_buffer_append(info_buffer buf, const info_char *str, size_t length);
bool info_internal_buffer_consume(info_buffer buffer, info_buffer co);
bool info_internal_buffer_vprintf(info_buffer buffer, const info_char *format, va_list args);
bool info_internal_buffer_printf(info_buffer buffer, const info_char *format, ...);
void info_internal_buffer_grow(info_buffer buffer, size_t size);;
bool info_internal_buffer_reserve(info_buffer buf, size_t size);
size_t info_internal_buffer_tell(info_buffer buffer);
bool info_internal_buffer_seek(info_buffer buffer, size_t pos);
info_char *info_internal_buffer_str(info_buffer buffer);
void info_internal_buffer_free(info_buffer buffer);
