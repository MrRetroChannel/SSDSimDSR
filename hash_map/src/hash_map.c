#include "include/hash_map.h"

#define INIT_HASH 5381

u32 __default_hash(void* data, size_t size) {
    int hash = INIT_HASH;
    u8* data_bytes = (u8*)data;

    for (int i = 0; i < size; ++i) {
        hash = ((hash << 5) + hash) + data_bytes[i];
    }
    return hash;
}