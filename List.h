#ifndef LIST_H_
#define LIST_H_


// Basic generic Linked List

#include "stdlib.h"

#define _LIST_INC(T, ALIAS,...)                                      \
        struct List_##ALIAS;                                         \
        typedef struct List_##ALIAS** List_##ALIAS;                  \
        T* List_##ALIAS##_next(struct List_##ALIAS **node);           \
        T* List_##ALIAS##_insert(struct List_##ALIAS **node, T val); \
        T List_##ALIAS##_pop(struct List_##ALIAS **node);

#define _LIST_IMPL(T,ALIAS, ...)                                       \
struct List_##ALIAS                                                    \
{                                                                      \
        T val;                                                         \
        struct List_##ALIAS *next;                                     \
};                                                                     \
T* List_##ALIAS##_next(struct List_##ALIAS **node)                     \
{                                                                      \
        if(!*node) return NULL;                                             \
        T* res = &(*node)->val;                                        \
        *node = (*node)->next;                                         \
        return res;                                                         \
}                                                                      \
T* List_##ALIAS##_insert(struct List_##ALIAS **node, T val)            \
{                                                                      \
        struct List_##ALIAS* res = malloc(sizeof(struct List_##ALIAS));\
        if(!res) return NULL;                                              \
        *res = (struct List_##ALIAS){val, *node};                      \
        *node = res;                                                   \
        return &res->val;                                                  \
}                                                                      \
T List_##ALIAS##_pop(struct List_##ALIAS** node)                       \
{                                                                      \
        T res = (*node)->val;                                          \
        struct List_##ALIAS* next = (*node)->next;                     \
        free(*node);                                                   \
        *node = next;                                                  \
        return res;                                                         \
}
#define LIST_IMPL(...) _LIST_IMPL(__VA_ARGS__, __VA_ARGS__,)
#define LIST_INC(...) _LIST_INC(__VA_ARGS__, __VA_ARGS__,)


#endif // LIST_H_
