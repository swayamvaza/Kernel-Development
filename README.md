# BareBones from OSdev forum

A minimal, freestanding 32-bit x86 kernel built from scratch.

## Project Overview
its a monolithic kernel designed to run on bare-metal hardware. It implements the Multiboot standard, allowing it to be loaded by the GRUB bootloader. The kernel bypasses host-system libraries (like `libc`) to interface directly with the VGA text buffer for hardware-level output. (i still dont get it)



## Prerequisites
To build this kernel, you must use an `i686-elf` cross-compiler toolchain. This prevents the compiler from linking against host-specific libraries (like `glibc`), which do not exist in a bare-metal environment.
NOTE: the following commands will work with arch based distro only 


## Installation (Arch/EndeavourOS)
```bash
# Install essential build tools
sudo pacman -S --needed base-devel

# Install pre-built cross-compiler toolchain
yay -S i686-elf-binutils-bin i686-elf-gcc-bin



```

## Project Structure

.
├── Makefile        # Automated build system
└── src/
    ├── boot.s      # Assembly entry point (Multiboot header)
    ├── kernel.c    # Kernel main logic
    └── linker.ld   # Memory layout definition

## Build n Run Cmd

| Command | Action |
| :--- | :--- |
| `make` | Compiles source files and links them into `myos.bin` |
| `make run` | Builds the project and launches QEMU with debug logs |
| `make clean` | Removes object files and binary outputs |


## Debugging

The **`make run`** target is configured with specific QEMU flags to assist in kernel debugging:

* **`-d int,cpu_reset`**: Enables logging for hardware interrupts and CPU reset events.
* **`-no-reboot`**: Halts the emulator on a Triple Fault, allowing you to examine the register dump provided by QEMU.


## Roadmap


* [x] Toolchain Setup

* [x] Multiboot Header Integration

* [x] VGA Text Driver

* [ ] Interrupt Descriptor Table (IDT) (might need to sacrifice someone)

* [ ] Keyboard Driver (aukat se bahar)