#include "../include/io.h"

// Basic mapping (simplified scancode set 1)
unsigned char kbd_map[] = { 
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 
    '-', '=', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P' 
};

void keyboard_handler() {
    unsigned char scancode = inb(0x60);
    
    // Check if key was pressed (bit 7 is 0 for press, 1 for release)
    if (scancode < sizeof(kbd_map)) {
        char c = kbd_map[scancode];
        if (c != 0) {
            terminal_putchar(c); // Print directly to your VGA buffer
        }
    }
    outb(0x20, 0x20); // Acknowledge PIC
}