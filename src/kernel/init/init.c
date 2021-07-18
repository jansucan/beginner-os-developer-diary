/* This code is based on the example from:
 *
 * https://wiki.osdev.org/Bare_Bones
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "pci.h"
#include "terminal.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error                                                                         \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

static void print_pci_device_list_header(void)
{
    terminal_write_string("\n");
    terminal_write_string("PCI devices:\n");

    terminal_write_string(
        "  VendorID   DeviceID   Class      Subclass   ProgIF\n");
}

static void
print_pci_header_common(const struct pci_header_common *const pci_header)
{
    terminal_write_string("  ");
    terminal_write_uint32(pci_header->vendor_id);
    terminal_write_string(" ");
    terminal_write_uint32(pci_header->device_id);
    terminal_write_string(" ");
    terminal_write_uint32(pci_header->class_code);
    terminal_write_string(" ");
    terminal_write_uint32(pci_header->subclass);
    terminal_write_string(" ");
    terminal_write_uint32(pci_header->prog_if);
    terminal_write_string("\n");
}

// cppcheck-suppress unusedFunction
void kernel_main(void)
{
    terminal_initialize();

    multiboot_print_memory_map();

    // List PCI devices
    print_pci_device_list_header();

    struct pci_function_address pci_address;
    struct pci_header_common pci_header;

    pci_function_iterator_init(&pci_address, &pci_header);

    while (pci_function_iterator_next(&pci_address, &pci_header)) {
        print_pci_header_common(&pci_header);
    }
}
