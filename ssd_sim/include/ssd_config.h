#ifndef SSD_CONFIG_H
#define SSD_CONFIG_H
 
#define SSD_STORAGE (1 << 20) // 1 MB
#define ERASE_SIZE (1 << 14) // 16 KB
#define PAGE_SIZE (1 << 9) // 512 B

#define PAGE_WEAR_LEVEL 32 // Количество перезаписей до износа

#endif