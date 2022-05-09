#pragma once

#include <types.h>

#define PAGE_SIZE 4096

void pmm_init();
uint64_t pmm_alloc_frame();
void pmm_free_frame(uint64_t frame);
