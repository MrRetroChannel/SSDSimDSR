#ifndef HASH_MAP_H
#define HASH_MAP_H

#define MAX_KEY_SIZE 32
#define MAX_VALUE_SIZE 512

#include <stdlib.h>
#include <stdbool.h>
#include "util/int_types.h"

u32 __default_hash(void* data, size_t size);

// Generic map API
#define hash_map_define(typeName, keyType, valueType)                       \
    typedef struct {                                                        \
        keyType key;                                                        \
        valueType value;                                                    \
        bool has_value;                                                     \
        u32 hash;                                                           \
    } typeName##_pair;                                                      \
                                                                            \
    typedef struct {                                                        \
        typeName##_pair* hash_chunk;                                        \
        size_t hash_chunk_size;                                             \
    } typeName;                                                             \
                                                                            \
    typeName init_##typeName(void* memory_chunk, size_t memory_size);       \
    void free_##typeName(typeName* map);                                    \
    bool add_##typeName(typeName* map, keyType key, valueType value);       \
    valueType* get_##typeName(typeName* map, keyType key);                  \
    bool set_##typeName(typeName* map, keyType key, valueType value);       \
    bool remove_##typeName(typeName* map, keyType key);                     \


// Generic map realization
#define hash_map_implementation(typeName, keyType, valueType, hash_func, cmp_func)                                  \
    typeName init_##typeName(void* memory_chunk, size_t memory_size) {                                              \
        typeName map = { .hash_chunk = memory_chunk, .hash_chunk_size = memory_size };                              \
        if (memory_chunk == NULL) {                                                                                 \
            map.hash_chunk = (typeName##_pair*)malloc(memory_size * sizeof(typeName##_pair));                       \
        }                                                                                                           \
        return map;                                                                                                 \
    }                                                                                                               \
                                                                                                                    \
    void free_##typeName(typeName* map) {                                                                           \
        free(map->hash_chunk);                                                                                      \
    }                                                                                                               \
                                                                                                                    \
    bool add_##typeName(typeName* map, keyType key, valueType value) {                                              \
        typeName##_pair cell = { .key = key, .value = value, .has_value = true, .hash = hash_func(key) };           \
        u32 place = cell.hash % map->hash_chunk_size;                                                               \
        if (!map->hash_chunk[place].has_value) {                                                                    \
            map->hash_chunk[place] = cell;                                                                          \
            return true;                                                                                            \
        } else {                                                                                                    \
            if (cmp_func(key, map->hash_chunk[place].key) == 0) return false;                                       \
        }                                                                                                           \
                                                                                                                    \
        u32 init_place = place;                                                                                     \
        do {                                                                                                        \
            if (++place == map->hash_chunk_size) place = 0;                                                         \
            if (!map->hash_chunk[place].has_value) {                                                                \
                map->hash_chunk[place] = cell;                                                                      \
                return true;                                                                                        \
            }                                                                                                       \
        } while (place != init_place);                                                                              \
        return true;                                                                                                \
    }                                                                                                               \
                                                                                                                    \
    valueType* get_##typeName(typeName* map, keyType key) {                                                         \
        u32 hash = hash_func(key);                                                                                  \
        u32 place = hash % map->hash_chunk_size;                                                                    \
        typeName##_pair* cell = map->hash_chunk + place;                                                            \
        if (cell->has_value && cmp_func(key, cell->key) == 0) return &cell->value;                                  \
        u32 init_place = place;                                                                                     \
        do {                                                                                                        \
            if (++place == map->hash_chunk_size) place = 0;                                                         \
            cell = map->hash_chunk + place;                                                                         \
        } while  (place != init_place && cell->has_value && cell->hash == hash && cmp_func(key, cell->key) != 0);   \
                                                                                                                    \
        return place == init_place ? NULL : &cell->value;                                                           \
    }                                                                                                               \
                                                                                                                    \
    bool set_##typeName(typeName* map, keyType key, valueType value) {                                              \
        valueType* val = get_##typeName(map, key);                                                                  \
        if (val != NULL) *val = value;                                                                              \
    }                                                                                                               \
                                                                                                                    \
    bool remove_##typeName(typeName* map, keyType key) {                                                            \
        u32 hash = hash_func(key);                                                                                  \
        u32 place = hash % map->hash_chunk_size;                                                                    \
        typeName##_pair* cell = map->hash_chunk + place;                                                            \
        if (!cell->has_value) return false;                                                                         \
                                                                                                                    \
        u32 old_place = place;                                                                                      \
        if (cmp_func(key, cell->key) != 0) {                                                                        \
            do {                                                                                                    \
                if (++place == map->hash_chunk_size) place = 0;                                                     \
                if (old_place == place) return false;                                                               \
                                                                                                                    \
                cell = map->hash_chunk + place;                                                                     \
            } while (cell->has_value && cell->hash == hash && cmp_func(key, cell->key));                            \
        }                                                                                                           \
                                                                                                                    \
        while (place != old_place && cell->has_value && cell->hash == hash) {                                       \
            if (++place == map->hash_chunk_size) place = 0;                                                         \
            cell = map->hash_chunk + place;                                                                         \
        }                                                                                                           \
                                                                                                                    \
        if (place == 0) place = map->hash_chunk_size - 1; else --place;                                             \
        typeName##_pair lastCell = map->hash_chunk[place];                                                          \
        map->hash_chunk[place].has_value = false;                                                                   \
        map->hash_chunk[old_place] = lastCell;                                                                      \
                                                                                                                    \
        return true;                                                                                                \
    }                                                                                                               \

#endif