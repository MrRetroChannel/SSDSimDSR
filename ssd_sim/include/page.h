#ifndef PAGE_H
#define PAGE_H

#include "util/int_types.h"
#include "ssd_config.h"

typedef struct {
    u8 data[PAGE_SIZE];
} page;

#endif