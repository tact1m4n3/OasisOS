#include <memory.h>

#include <string.h>
#include <debug.h>
#include <multiboot.h>

#define SET_BIT(i) frame_bitmap[i / 8] = frame_bitmap[i / 8] | (1 << (i % 8))
#define CLEAR_BIT(i) frame_bitmap[i / 8] = frame_bitmap[i / 8] & (~(1 << (i % 8)))
#define IS_SET(i) ((frame_bitmap[i / 8] >> (i % 8)) & 0x1)

extern void* kernel_start;
extern void* kernel_end;

static uint8_t* frame_bitmap;
static uint64_t n_frames;

static uint64_t get_highest_memory_addr() {
    uint64_t i = 0, type, start, end, addr = 0;
    while (multiboot_get_memory_area(i, &type, &start, &end)) {
        if (type == 1 && end > addr) {
            addr = end;
        }
        i++;
    }
    return addr;
}

static void* lookup_bitmap_location(uint64_t size) {
    uint64_t i = 0, type, start, end;
    while (multiboot_get_memory_area(i, &type, &start, &end)) {
        if (type == 1) {
            for (uint64_t j = start; j < end; j += PAGE_SIZE) {
                if (j + size > end) {
                    break;
                } else if (multiboot_is_memory_used(j, size) || (start < (uint64_t)&kernel_end && start + size > (uint64_t)&kernel_start)) {
                    continue;
                } else {
                    return (void*)j;
                }
            }
        }
        i++;
    }
    return (void*)-1;
}

static uint64_t lookup_first_free_frame() {
    for (uint64_t i = 0; i < n_frames; i++) {
        if (!IS_SET(i)) {
            return i;
        }
    }
    FATAL("pmm: no free frames left\n");
    for (;;);
}

void pmm_init() {
    uint64_t highest_addr = get_highest_memory_addr();
    uint64_t bitmap_size = highest_addr / 8 + ((highest_addr % 8 == 0) ? 0 : 1);
    n_frames = bitmap_size * 8;

    frame_bitmap = (uint8_t*)lookup_bitmap_location(bitmap_size);
    if (frame_bitmap == (void*)-1) {
        FATAL("pmm: failed to allocate bitmap of size %x\n", bitmap_size);
        for (;;);
    }

    memset(frame_bitmap, 0xff, bitmap_size);

    uint64_t i = 0, type, start, end;
    while (multiboot_get_memory_area(i, &type, &start, &end)) {
        if (type == 1) {
            for (uint64_t j = start; j < end; j += PAGE_SIZE) {
                if (!(multiboot_memory_used(j, PAGE_SIZE) || (start < (uint64_t)&kernel_end && start + PAGE_SIZE > (uint64_t)&kernel_start))) {
                    pmm_free_frame(j);
                }
            }
        }
        i++;
    }
}

uint64_t pmm_alloc_frame() {
    uint64_t idx = lookup_first_free_frame();
    SET_BIT(idx);
    return idx * PAGE_SIZE;
}

void pmm_free_frame(uint64_t frame) {
    uint32_t idx = frame / PAGE_SIZE;
    if (idx >= n_frames) {
        WARN("pmm: frame address out of range\n");
        return;
    }
    CLEAR_BIT(idx);
}
