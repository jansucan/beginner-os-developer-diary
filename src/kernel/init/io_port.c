#include "io_port.h"

void io_port_out_dword(uint16_t port, uint32_t value)
{
    register uint16_t p __asm__("dx") = port;
    register uint32_t v __asm__("eax") = value;

    __asm__("out %[value], %[port]" : : [ value ] "r"(v), [ port ] "r"(p));
}

uint32_t io_port_in_dword(uint16_t port)
{
    register uint16_t p __asm__("dx") = port;
    register uint32_t v __asm__("eax");

    __asm__("in %[port], %[value]" : [ value ] "=r"(v) : [ port ] "r"(p));

    return v;
}
