#ifndef MAP_H_
#define MAP_H_

#include "List.h"

// Extend List with Map operations

#define MAP_INC(K, T, ALIAS)                                           \
        _LIST_INC(T,ALIAS,)                                             \
    struct List_##ALIAS** List_##ALIAS##_find(struct List_##ALIAS **current_ptr, K key); \
    T* List_##ALIAS##_get(struct List_##ALIAS **current_ptr, K key);       \
    int List_##ALIAS##_remove(struct List_##ALIAS **current_ptr, K key, T *out);

#define MAP_IMPL(K, T, ALIAS, CMP)                                                        \
        _LIST_IMPL(T,ALIAS, )                                            \
    struct List_##ALIAS ** List_##ALIAS##_find(struct List_##ALIAS **current_ptr, K key){ \
    struct List_##ALIAS *current = *current_ptr;                                          \
        while(current){                                                                   \
            if(CMP(key, &current->val))                                                   \
                return current_ptr;                                                            \
            current_ptr = &current->next;                                                 \
            current = current->next;                                                      \
        }                                                                                 \
        return NULL;                                                                           \
    }                                                                                     \
    T* List_##ALIAS##_get(struct List_##ALIAS **map, K key) {                                     \
        struct List_##ALIAS **node = List_##ALIAS##_find(map, key);                               \
        if(!node) return NULL;                                                                \
        return &(*node)->val;                                                                  \
    }                                                                                     \
    int List_##ALIAS##_remove(struct List_##ALIAS **map, K key, T* out){                    \
        struct List_##ALIAS **node = List_##ALIAS##_find(map, key);                       \
        if(!node) return -1;                                                                  \
        if(out)                                                                           \
            *out = List_##ALIAS##_pop(node);                                              \
        else                                                                              \
            List_##ALIAS##_pop(node);                                                     \
        return 0;                                                                              \
    }

#endif // MAP_H_
