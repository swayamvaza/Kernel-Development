#include "../include/gdt.h"

struct gdt_entry gdt[3];
struct gdt_ptr gp;

// This function sets a descriptor in the GDT
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    // 0: Null Descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    // 1: Code Segment (Base 0, Limit 4GB, Access 0x9A)
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    // 2: Data Segment (Base 0, Limit 4GB, Access 0x92)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Call assembly function to load GDT
    gdt_flush((unsigned int)&gp);
}