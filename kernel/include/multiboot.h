#pragma once

#include <types.h>

typedef struct boot_data {
    int version;
    char* bootloader;
    char* cmdline;
    uint64_t mmap_size;
    uint64_t mmap_length;
    void* mmap;
} boot_data_t;

extern boot_data_t boot_data;

void multiboot_init(uint64_t magic, void* info);
int multiboot_next_memory_area(uint64_t idx, uint32_t* type, uint64_t* start, uint64_t* end);
