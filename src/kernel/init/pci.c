#include <stdbool.h>

#include "io_port.h"
#include "pci.h"
#include "terminal.h"

#define PCI_IO_CONFIG_ADDRESS 0xCF8
#define PCI_IO_CONFIG_DATA 0xCFC

#define PCI_CONFIG_DEVICE_NUMBER_MASK 0x1f
#define PCI_CONFIG_FUNCTION_NUMBER_MASK 0x07
#define PCI_CONFIG_REGISTER_OFFSET_MASK 0xfc

#define PCI_CONFIG_ENABLE_BIT_OFFSET 31
#define PCI_CONFIG_BUS_NUMBER_OFFSET 16
#define PCI_CONFIG_DEVICE_NUMBER_OFFSET 11
#define PCI_CONFIG_FUNCTION_NUMBER_OFFSET 8

struct pci_header_common {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t prog_if;
    uint8_t subclass;
    uint8_t class_code;
    uint8_t header_type;
};

static uint32_t pci_config_read_dword(uint8_t bus_number, uint8_t device_number,
                                      uint8_t function_number,
                                      uint8_t byte_offset)
{
    const uint32_t enable = 1U << PCI_CONFIG_ENABLE_BIT_OFFSET;
    const uint32_t bus = ((uint32_t)bus_number) << PCI_CONFIG_BUS_NUMBER_OFFSET;
    const uint32_t device =
        ((uint32_t)(device_number & PCI_CONFIG_DEVICE_NUMBER_MASK))
        << PCI_CONFIG_DEVICE_NUMBER_OFFSET;
    const uint32_t function =
        ((uint32_t)(function_number & PCI_CONFIG_FUNCTION_NUMBER_MASK))
        << PCI_CONFIG_FUNCTION_NUMBER_OFFSET;
    const uint32_t offset =
        (uint32_t)(byte_offset & PCI_CONFIG_REGISTER_OFFSET_MASK);

    const uint32_t addr = enable | bus | device | function | offset;

    io_port_out_dword(PCI_IO_CONFIG_ADDRESS, addr);

    return io_port_in_dword(PCI_IO_CONFIG_DATA);
}

static uint16_t pci_config_read_word(uint8_t bus_number, uint8_t device_number,
                                     uint8_t function_number,
                                     uint8_t byte_offset)
{
    const uint32_t dw = pci_config_read_dword(bus_number, device_number,
                                              function_number, byte_offset);
    const uint8_t offset = byte_offset & 0x02;
    const uint16_t word = (uint16_t)(dw >> (8 * offset));

    return word;
}

static uint8_t pci_config_read_byte(uint8_t bus_number, uint8_t device_number,
                                    uint8_t function_number,
                                    uint8_t byte_offset)
{
    const uint32_t dw = pci_config_read_dword(bus_number, device_number,
                                              function_number, byte_offset);
    const uint8_t offset = byte_offset & 0x03;
    const uint8_t byte = (uint8_t)(dw >> (8 * offset));

    return byte;
}

static void pci_read_header_common(uint8_t bus_number, uint8_t device_number,
                                   uint8_t function_number,
                                   struct pci_header_common *const pci_header)
{
    pci_header->vendor_id = 0;
    pci_header->device_id = 0;
    pci_header->prog_if = 0;
    pci_header->subclass = 0;
    pci_header->class_code = 0;
    pci_header->header_type = 0;

    pci_header->vendor_id =
        pci_config_read_word(bus_number, device_number, function_number, 0x00);
    if (pci_header->vendor_id != 0xFFFF) {
        pci_header->device_id = pci_config_read_word(bus_number, device_number,
                                                     function_number, 0x02);
        pci_header->prog_if = pci_config_read_byte(bus_number, device_number,
                                                   function_number, 0x09);
        pci_header->subclass = pci_config_read_byte(bus_number, device_number,
                                                    function_number, 0x0A);
        pci_header->class_code = pci_config_read_byte(bus_number, device_number,
                                                      function_number, 0x0B);
        pci_header->header_type = pci_config_read_byte(
            bus_number, device_number, function_number, 0x0E);
    }
}

static bool
pci_is_multifunction_device(const struct pci_header_common *const pci_header)
{
    return ((pci_header->header_type & 0x80) != 0x00);
}

static void
pci_print_header_common(const struct pci_header_common *const pci_header)
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

static void pci_print_functions(uint8_t bus_number, uint8_t device_number)
{
    uint8_t function = 0;
    struct pci_header_common pci_header;

    pci_read_header_common(bus_number, device_number, function, &pci_header);

    if (pci_header.vendor_id != 0xFFFF) {
        pci_print_header_common(&pci_header);

        if (pci_is_multifunction_device(&pci_header)) {
            for (function = 1; function < 8; ++function) {
                pci_read_header_common(bus_number, device_number, function,
                                       &pci_header);
                if (pci_header.vendor_id != 0xFFFF) {
                    pci_print_header_common(&pci_header);
                }
            }
        }
    }
}

void pci_print_devices(void)
{
    terminal_write_string("\n");
    terminal_write_string("PCI devices:\n");

    terminal_write_string(
        "  VendorID   DeviceID   Class      Subclass   ProgIF\n");

    for (uint16_t bus = 0; bus < 256; ++bus) {
        for (uint8_t device = 0; device < 32; ++device) {
            pci_print_functions(bus, device);
        }
    }
}
