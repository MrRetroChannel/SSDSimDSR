#ifndef SSD_H
#define SSD_H

#include <stdio.h>
#include "ssd_config.h"
#include "page.h"
#include "hash_map.h"

#define PAGE_COUNT (SSD_STORAGE / PAGE_SIZE)

hash_map_define(ssd_map, u8*, u8*)

typedef struct {
    FILE* SSDFile;
    u32 wear_level[PAGE_COUNT];
    u32 crc[PAGE_COUNT];
    page pages[PAGE_COUNT];
    ssd_map map; // Map shares stack memory with pages
} SSD;

SSD* init_ssd(const char* fileName);

void free_ssd(SSD* ssd);

void save_file_ssd(SSD* ssd); // Move all pages to file

void write_ssd(SSD* ssd, u8* key, u8* value); // Write data to hashmap

u8* read_ssd(SSD* ssd, u8* key);

void remove_ssd(SSD* ssd, u8* key); // Remove data from hashmap

#endif