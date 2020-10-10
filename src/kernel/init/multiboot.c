#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"
#include "multiboot.h"

struct multiboot_info_struct {
  uint32_t flags;
  uint8_t not_used[40];
  uint32_t mmap_length;
  uint32_t mmap_addr;
};

struct multiboot_mmap_entry {
  uint32_t entry_size;
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
};

static uint32_t multiboot_get_info_struct_addr(void)
{
    uint32_t a;
    /* Bootloader passed an address of the multiboot information structure in
     * EBX.
     */
    asm ("mov %%ebx,%0"
         : "=g"(a));

    return a;
}

void multiboot_get_memory_map(void)
{
    ;
}

void multiboot_print_memory_map(void)
{
    struct __attribute__((packed)) multiboot_info_struct * info_struct =
      (struct multiboot_info_struct *)multiboot_get_info_struct_addr();

    terminal_write_string("Multiboot info structure address: ");
    terminal_write_uint32((uint32_t)info_struct);
    terminal_write_string("\n");

    terminal_write_string("Memory map length: ");
    terminal_write_uint32(info_struct->mmap_length);
    terminal_write_string("\n");

    terminal_write_string("Memory map address: ");
    terminal_write_uint32(info_struct->mmap_addr);
    terminal_write_string("\n");

    terminal_write_string("Memory map:\n");
    terminal_write_string("  BaseAddrHigh  BaseAddrLow  LengthLow  LengthHigh  Type\n");

    uint8_t l = 0;
    while (l < info_struct->mmap_length) {
      struct __attribute__((packed)) multiboot_mmap_entry * me =
        (struct multiboot_mmap_entry *)(info_struct->mmap_addr + l);

      terminal_write_string("  ");

      terminal_write_uint32(me->base_addr_high);
      terminal_write_string("    ");

      terminal_write_uint32(me->base_addr_low);
      terminal_write_string("   ");

      terminal_write_uint32(me->length_high);
      terminal_write_string(" ");

      terminal_write_uint32(me->length_low);
      terminal_write_string("  ");

      switch (me->type) {
      case 1:
        terminal_write_string("AddressRangeMemory");
        break;

      case 2:
        terminal_write_string("AddressRangeReserved");
        break;

      case 3:
        terminal_write_string("AddressRangeACPI");
        break;

      case 4:
        terminal_write_string("AddressRangeNVS");
        break;

      case 5:
        terminal_write_string("AddressRangeUnusable");
        break;

      default:
        terminal_write_string("Undefined");
        break;
      }
      terminal_write_string("\n");

      l += me->entry_size;
    }

}
