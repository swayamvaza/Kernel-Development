# Compiler and Assembler
CC=i686-elf-gcc
AS=i686-elf-as

# Build flags
CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS=-T src/linker.ld -ffreestanding -O2 -nostdlib

all: myos.bin

myos.bin: boot.o kernel.o
	$(CC) $(LDFLAGS) -o myos.bin boot.o kernel.o -lgcc

boot.o: src/boot.s
	$(AS) src/boot.s -o boot.o

kernel.o: src/kernel.c
	$(CC) -c src/kernel.c -o kernel.o $(CFLAGS)

run: all
	qemu-system-i386 -kernel myos.bin -d int,cpu_reset -no-reboot

clean:
	rm -rf myos.bin boot.o kernel.o