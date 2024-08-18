#include "ssd.h"
#include "crc.h"
#include "util/logger.h"
#include <stdlib.h>
#include <string.h>

u32 str_hash(char* str) {
    return __default_hash(str, strlen(str));
}

hash_map_implementation(ssd_map, u8*, u8*, str_hash, strcmp)

SSD* init_ssd(const char* fileName) {
    SSD* ssd = (SSD*)malloc(sizeof(SSD));
    FILE* ssdFile;
    if ((ssdFile = fopen(fileName, "r")) == NULL) {
        LOG_WARNING("SSD file %s does not exist, creating", fileName);
        ssdFile = fopen(fileName, "ab+");

        for (size_t i = 0; i < PAGE_COUNT; ++i) {
            ssd->wear_level[i] = count_crc32((u8*)(ssd->wear_level + i), sizeof(u32));
        }
    } else {
        LOG_INFO("Opened file %s", fileName);

        ssd->map = init_ssd_map(ssd->pages, PAGE_COUNT);
        fread(ssd->crc, sizeof(u32), PAGE_COUNT, ssdFile);
        fread(ssd->wear_level, sizeof(u32), PAGE_COUNT, ssdFile);
        fread(ssd->pages, sizeof(page), PAGE_COUNT, ssdFile);
    }

    ssd->SSDFile = ssdFile;

    return ssd;
}

void save_file_ssd(SSD* ssd) {
    FILE* ssdFile = ssd->SSDFile;
     
    fwrite(ssd->crc, sizeof(u32), PAGE_COUNT, ssdFile);
    fwrite(ssd->wear_level, sizeof(u32), PAGE_COUNT, ssdFile);
    fwrite(ssd->pages, sizeof(page), PAGE_COUNT, ssdFile);
}

void free_ssd(SSD* ssd) {
    fclose(ssd->SSDFile);
    free(ssd);
}

void write_ssd(SSD* ssd, u8* key, u8* value) {
    set_ssd_map(&ssd->map, key, value);
}

u8* read_ssd(SSD* ssd, u8* key) {
    u8** value = get_ssd_map(&ssd->map, key);

    if (ssd->crc[ssd->map.hash_chunk_size - 1] != count_crc32(*value, strlen(*value))) {
        LOG_ERROR("Invalid CRC by address %p", &ssd->crc[ssd->map.hash_chunk_size - 1]);
        return NULL;
    }

    return *value;
}