#include "arena.h"
#include <stdlib.h>


struct info_Arena
{
    void *start, *current, *end;
    struct info_Arena *next;
};

info_Arena info_arena_create(size_t size)
{
    info_Arena a = malloc(sizeof(struct info_Arena));
    *a = (struct info_Arena){ 0 };
    a->start = malloc(size),
    a->end = a->start+size;
    a->current = a->start;
    return a;
}

void* info_arena_alloc(info_Arena arena, size_t size)
{
    void *next = arena->current+size;
    if( next >= arena->end){
        if(!arena->next){
            arena->next = malloc(sizeof(struct info_Arena));
            size_t alloc = size>ARENA_CAP ? size : ARENA_CAP;
            *arena->next = info_arena_create(alloc);
            if(!arena->next->start) return NULL;
        }
        return info_arena_alloc(arena->next, size);
    }
    void *res = arena->current;
    arena->current = next;
    return res;
}

void* info_arena_realloc(info_Arena arena, void *old, size_t old_len, size_t new_len)
{
    if(arena->current == old-old_len){
        arena+=new_len-old_len;
        return old;
    }
    void *new_ptr = info_arena_alloc(arena, new_len);
    memcpy(new_ptr, old, old_len);
    return new_ptr;
}
void info_arena_free(info_Arena arena)
{
    if(!arena) return;
    free(arena->start);
    info_arena_free(arena->next);
    free(arena);
}
