#ifndef ARENA_H_
#define ARENA_H_

#include <string.h>
#define ARENA_CAP (1<<12) /*4096*/

struct info_Arena;
typedef struct info_Arena* info_Arena;

info_Arena info_arena_create(size_t size);
void* info_arena_alloc(info_Arena arena, size_t size);
void* info_arena_realloc(info_Arena arena, void *old, size_t old_len, size_t new_len);
void info_arena_free(info_Arena arena);

#endif // ARENA_H_
