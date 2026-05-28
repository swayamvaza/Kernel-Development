CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Isrc/include
LDFLAGS=-T src/linker.ld -ffreestanding -O2 -nostdlib

OBJS = boot.o kernel.o cpu/gdt.o cpu/gdt_flush.o cpu/idt.o cpu/idt_load.o cpu/isr_asm.o cpu/isr.o

all: myos.bin

myos.bin: $(OBJS)
	$(CC) $(LDFLAGS) -o myos.bin $(OBJS) -lgcc

# Bootloader Entry
boot.o: src/boot.s
	$(AS) src/boot.s -o boot.o

# Kernel C Code
kernel.o: src/kernel.c
	$(CC) -c src/kernel.c -o kernel.o $(CFLAGS)

# GDT
cpu/gdt.o: src/cpu/gdt.c
	@mkdir -p cpu
	$(CC) -c src/cpu/gdt.c -o cpu/gdt.o $(CFLAGS)

cpu/gdt_flush.o: src/cpu/gdt_flush.s
	@mkdir -p cpu
	$(AS) src/cpu/gdt_flush.s -o cpu/gdt_flush.o

# IDT
cpu/idt.o: src/cpu/idt.c
	@mkdir -p cpu
	$(CC) -c src/cpu/idt.c -o cpu/idt.o $(CFLAGS)

cpu/idt_load.o: src/cpu/idt_load.s
	@mkdir -p cpu
	$(AS) src/cpu/idt_load.s -o cpu/idt_load.o

# ISR
cpu/isr.o: src/cpu/isr.c
	@mkdir -p cpu
	$(CC) -c src/cpu/isr.c -o cpu/isr.o $(CFLAGS)

cpu/isr_asm.o: src/cpu/isr.s
	@mkdir -p cpu
	$(AS) src/cpu/isr.s -o cpu/isr_asm.o

# Execution and Cleanup
run: all
	qemu-system-i386 -kernel myos.bin -d int,cpu_reset -no-reboot

clean:
	rm -rf myos.bin *.o cpu/*.o