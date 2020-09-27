#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_MEMORY_MAP_MAX_ENTRIES 32

struct multiboot_memory_map_entry {
    uint32_t base_addr_low;
    uint32_t base_addr_high;
    uint32_t length_low;
    uint32_t length_high;
    uint32_t type;
    uint32_t extended_attributes;
};

void multiboot_get_memory_map(void);
void multiboot_print_memory_map(void);

#endif
