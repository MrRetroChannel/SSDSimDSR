#ifndef CRC_H
#define CRC_H
#include "util/int_types.h"
#include <stdlib.h>

extern const u32 crc_table[256];

u32 count_crc32(u8* data, size_t length);

#endif