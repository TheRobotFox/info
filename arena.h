#ifndef ARENA_H_
#define ARENA_H_

#define ARENA_CAP (1<<12) /*4096*/

#include <stdio.h>
#include <stdlib.h>
struct info_Arena
{
    void *start, *current, *end;
    struct info_Arena *next;
};

struct info_Arena info_arena_create(size_t size)
{
    struct info_Arena a = { 0 };
    a.start = malloc(size);
    a.end = a.start+size;
    a.current = a.start;
    return a;
}

void* info_arena_alloc(struct info_Arena *arena, size_t size)
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

void info_arena_free(struct info_Arena *arena)
{
    if(!arena) return;
    free(arena->start);
    info_arena_free(arena->next);
}


#endif // ARENA_H_
