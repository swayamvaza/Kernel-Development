#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

/* VGA Hardware Constants */
#define VGA_WIDTH   80
#define VGA_HEIGHT  25
#define VGA_MEMORY  0xB8000 

/* Global state for the terminal */
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

/* Function Definitions */

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(void) 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            terminal_row++;
        }
    }

    /* SCROLLING LOGIC */
    if (terminal_row == VGA_HEIGHT) {
        for (size_t y = 1; y < VGA_HEIGHT; y++) {
            for (size_t x = 0; x < VGA_WIDTH; x++) {
                terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
            }
        }
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
        terminal_row = VGA_HEIGHT - 1;
    }
}

void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminal_writestring(const char* data) 
{
    terminal_write(data, strlen(data));
}

/* Print a pyramid pattern */
void print_pyramid(int height) {
    for (int i = 0; i < height; i++) {
        // Print leading spaces
        for (int j = 0; j < height - i - 1; j++) {
            terminal_putchar(' ');
        }
        // Print stars
        for (int k = 0; k < (2 * i + 1); k++) {
            terminal_putchar('*');
        }
        // Next line
        terminal_putchar('\n');
    }
}

/* The true entry point for your C code */
void kernel_main(void) 
{
    // 1. Clear the screen and set up the background
    terminal_initialize();

    // 2. Print our text
    terminal_writestring("Hello, kernel World!\n");
    terminal_writestring("I have successfully survived the Triple Fault.\n");
    terminal_writestring("Here is my pattern:\n\n");

    // 3. Draw the pyramid
    print_pyramid(5);

    /* 4. CPU TRAP: Halt forever so we don't crash into garbage memory */
    for (;;) {
        asm volatile ("hlt");
    }
}