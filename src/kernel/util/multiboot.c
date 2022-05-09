#include <multiboot.h>

#include <string.h>
#include <debug.h>
#include <memory.h>

#define MBOOT2_MAGIC 0x36d76289
#define MBOOT2_CMDLINE 1
#define MBOOT2_BOOTLOADER 2
#define MBOOT2_MEMORY_MAP 6

#define NEXT_TAG(tag) ((mb2_tag_t*)((uint8_t*)tag + ((tag->size + 7) & ~7)))

typedef struct mb2_info {
    uint32_t size;
    uint32_t reserved;
    uint8_t tags[];
} mb2_info_t;

typedef struct mb2_tag {
    uint32_t type;
    uint32_t size;
    uint8_t data[];
} mb2_tag_t;

typedef struct mb2_mmap {
    uint32_t entry_size;
    uint32_t entry_version;
    uint8_t entries[];
} mb2_mmap_t;

typedef struct mb2_mmap_entry {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} mb2_mmap_entry_t;

boot_data_t boot_data;

static void parse_multiboot2(mb2_info_t* info) {
    mb2_tag_t* tag = (void*)&info->tags[0];
    mb2_mmap_t* mmap;
    while (tag->type) {
        switch (tag->type) {
            case MBOOT2_BOOTLOADER:
                boot_data.bootloader = (char*)&tag->data[0];
                break;
            case MBOOT2_CMDLINE:
                boot_data.command_line = (char*)&tag->data[0];
                break;
            case MBOOT2_MEMORY_MAP:
                mmap = (void*)&tag->data[0];
                boot_data.mmap_size = tag->size - 16;
                boot_data.mmap_length = boot_data.mmap_size / mmap->entry_size;
                boot_data.mmap = (void*)&mmap->entries[0];
                break;
            default:
                break;
        }
        tag = NEXT_TAG(tag);
    }
}

void multiboot_init(uint64_t magic, void* info) {
    if (magic == MBOOT2_MAGIC) {
        boot_data.version = 2;
        parse_multiboot2(info);
    } else {
        FATAL("bootloader not supported!\n");
        for (;;);
    }
}

int multiboot_memory_used(uint64_t start, uint64_t size) {
#define overlap(st, sz) ((uint64_t)(st) < (start + size) && ((uint64_t)(st) + (sz)) > start)
    if (overlap(boot_data.bootloader, strlen(boot_data.bootloader))
        && overlap(boot_data.command_line, strlen(boot_data.command_line))
        && overlap(boot_data.mmap, boot_data.mmap_size)) {
        return 1;
    } else {
        return 0;
    }
}

int multiboot_get_memory_area(uint64_t idx, uint64_t* type, uint64_t* start, uint64_t* end) {
    if (idx >= boot_data.mmap_length) {
        return 0;
    }

    if (boot_data.version == 2) {
        uint64_t entry_size = boot_data.mmap_size / boot_data.mmap_length;
        mb2_mmap_entry_t* entry = (void*)((uint64_t)boot_data.mmap + idx * entry_size);
        *type = entry->type;
        *start = entry->base_addr;
        *end = entry->base_addr + entry->length;
    }

    return 1;
}
