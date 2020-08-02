/* This code is based on the example from:
 *
 * https://wiki.osdev.org/Bare_Bones
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "terminal.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BASE_ADDRESS 0xB8000
/* Number of characters of the longest string this code can
 * handle. This limit is also used for safety restriction in the
 * terminal_strlen() function.
 */
#define TERMINAL_MAX_STRLEN VGA_WIDTH

/* Hardware text mode color constants. */
enum vga_color {
		VGA_COLOR_BLACK = 0,
		VGA_COLOR_BLUE = 1,
		VGA_COLOR_GREEN = 2,
		VGA_COLOR_CYAN = 3,
		VGA_COLOR_RED = 4,
		VGA_COLOR_MAGENTA = 5,
		VGA_COLOR_BROWN = 6,
		VGA_COLOR_LIGHT_GREY = 7,
		VGA_COLOR_DARK_GREY = 8,
		VGA_COLOR_LIGHT_BLUE = 9,
		VGA_COLOR_LIGHT_GREEN = 10,
		VGA_COLOR_LIGHT_CYAN = 11,
		VGA_COLOR_LIGHT_RED = 12,
		VGA_COLOR_LIGHT_MAGENTA = 13,
		VGA_COLOR_LIGHT_BROWN = 14,
		VGA_COLOR_WHITE = 15,
};

static size_t     terminal_row;
static size_t     terminal_column;
static uint8_t    terminal_color;
static uint16_t * terminal_buffer;

static uint8_t  vga_entry_color(enum vga_color fg, enum vga_color bg);
static uint16_t vga_entry(unsigned char uc);
static size_t   terminal_strlen(const char* str);
static void     terminal_putchar_at(char c, size_t x, size_t y);
static void     terminal_putchar(char c);
static void     terminal_uint32_to_hexstr(uint32_t number, char * return_string);

static uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | (bg << 4);
}

static uint16_t vga_entry(unsigned char uc)
{
    return ((uint16_t) uc) | (((uint16_t) terminal_color) << 8);
}

static size_t terminal_strlen(const char* str)
{
    size_t len = 0;

    while ((str[len] != '\0') && (len < TERMINAL_MAX_STRLEN)) {
	len++;
    }

    return len;
}

static void terminal_putchar_at(char c, size_t x, size_t y)
{
    const size_t index = (y * VGA_WIDTH) + x;

    terminal_buffer[index] = vga_entry(c);
}

static void terminal_putchar(char c)
{
    terminal_putchar_at(c, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
	terminal_column = 0;
	if (++terminal_row == VGA_HEIGHT) {
	    terminal_row = 0;
	}
    }
}

static char terminal_num_to_hexchar(uint8_t number)
{
    if (number >= 16) {
	return '?';
    } else if (number < 10) {
	return '0' + number;
    } else {
	return 'a' + number - 10;
    }
}

static void terminal_uint32_to_hexstr(uint32_t number, char * const return_string)
{
    /* The resulting string will always be 10 characters long. 2 for
     * hexadecimal prefix and 8 for the value.
     */
    size_t i = 10;

    /* Terminate the string. */
    return_string[i--] = '\0';

    do {
	/* Write the remainders from backwards. This will result in
	 * correct string representation.
	 */
	return_string[i--] = terminal_num_to_hexchar(number % 16);
	number /= 16;
    } while (number != 0);

    /* Pad the value to 8 characters. */
    while (i > 1) {
	return_string[i--] = '0';
    }

    /* Add hex prefix. */
    return_string[i--] = 'x';
    return_string[i] = '0';
}

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *) VGA_BASE_ADDRESS;

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
	for (size_t x = 0; x < VGA_WIDTH; x++) {
	    terminal_putchar(' ');
	}
    }
}

void terminal_write_string(const char * const string)
{
    const size_t len = terminal_strlen(string);

    for (size_t i = 0; i < len; i++) {
	terminal_putchar(string[i]);
    }
}

void terminal_write_uint32(uint32_t number)
{
    char s[TERMINAL_MAX_STRLEN + 1];

    terminal_uint32_to_hexstr(number, s);
    terminal_write_string(s);
}
