
.code32

# Declare constants for the Multiboot header
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Multiboot header - Required by GRUB to recognize your kernel
.section .multiboot, "a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Setup the stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# Entry point
.section .text
.global _start
.extern kernel_main  # Correct placement

_start:
    cli                 # <-- THE MISSING MUZZLE: Disable interrupts immediately!

    # Initialize stack pointer
    mov $stack_top, %esp

    # Call your C kernel
    call kernel_main

    # Safety: If kernel_main returns, trap the CPU
    cli
.Lhang:
    hlt
    jmp .Lhang
