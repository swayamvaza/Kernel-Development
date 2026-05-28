#include "../include/idt.h"
#include "../include/io.h"

struct idt_entry idt[256];
struct idt_ptr idtp; // NOTE: Ensure idtp is NOT also defined in your .s file to avoid collision

extern void idt_load(); // Defined in assembly
extern void isr_common_stub();
extern void keyboard_handler_stub();

void pic_remap() {
    outb(0x20, 0x11); // Start initialization
    outb(0xA0, 0x11);
    outb(0x21, 0x20); // Set Master PIC offset to 32
    outb(0xA1, 0x28); // Set Slave PIC offset to 40
    outb(0x21, 0x04); // Tell Master there is a slave
    outb(0xA1, 0x02); // Tell Slave its cascade identity
    outb(0x21, 0x01); // 8086 mode
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  // Unmask all interrupts
    outb(0xA1, 0x0);
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[num].base_low = (base & 0xFFFF);
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    pic_remap();
    
    // FILL ALL 256 GATES WITH A DEFAULT HANDLER FIRST
    // This catches the Timer (32) and all exceptions (0-31) safely
    for(int i = 0; i < 256; i++) {
        idt_set_gate(i, (unsigned long)isr_common_stub, 0x08, 0x8E); 
    }
    
    // Now overwrite the specific ones you care about
    idt_set_gate(33, (unsigned long)keyboard_handler_stub, 0x08, 0x8E);
    
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;
    
    idt_load();
}