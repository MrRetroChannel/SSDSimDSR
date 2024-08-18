#include "ssd.h"
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

void write_ssd(SSD* ssd, u8* data) {
    
}