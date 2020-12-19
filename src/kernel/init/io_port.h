#ifndef IO_PORT_H
#define IO_PORT_H

#include <stdint.h>

void io_port_out_dword(uint16_t port, uint32_t value);
uint32_t io_port_in_dword(uint16_t port);

#endif
