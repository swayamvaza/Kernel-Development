#ifndef GDT_H
#define GDT_H

// Represents a single entry in the GDT
struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed));

// Pointer to the GDT structure
struct gdt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void gdt_install();
extern void gdt_flush(unsigned int gdt_ptr_address); // Add this line

#endif