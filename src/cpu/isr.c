#include "../include/io.h"
#include <stdint.h>

volatile uint16_t* vga = (volatile uint16_t*)0xB8000;

// Cursor tracking variables
static int cursor_x = 0;
static int cursor_y = 0;

// Basic scancode mapping for English QWERTY
static const char kbd_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
};

// Helper function to manage the cursor and screen boundaries
void terminal_putchar(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        // 80 columns per row in standard VGA text mode
        int index = (cursor_y * 80) + cursor_x;
        vga[index] = 0x0F00 | c; // White text on black background
        cursor_x++;
    }

    // Line wrap if we hit the right edge
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    // Screen wrap if we hit the bottom (Scrolling logic comes later)
    if (cursor_y >= 25) {
        cursor_y = 0; 
    }
}

void keyboard_handler() {
    uint8_t scancode = inb(0x60);
    
    // Check if bit 7 is set (0x80) - this is a key release
    if (!(scancode & 0x80)) {
        // This is a press event
        if (scancode < sizeof(kbd_map)) {
            char c = kbd_map[scancode];
            if (c != 0) {
                terminal_putchar(c);
            }
        }
    }
    
    // Acknowledge the interrupt to the PIC (Master PIC)
    outb(0x20, 0x20); 
}

void isr_handler() {
    const char *msg = "Exception Caught!";
    char *vga_ptr = (char*)0xB8000;
    
    // Print in red (0x4F) at the very top of the screen
    for(int i = 0; msg[i] != '\0'; i++) {
        vga_ptr[i*2] = msg[i];
        vga_ptr[i*2+1] = 0x4F;
    }
    
    // Halt the CPU completely on exception
    while(1) {
        asm volatile ("cli; hlt");
    }
}