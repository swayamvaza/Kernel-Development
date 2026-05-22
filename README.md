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

## Steps for you my friend

* i hope that cross compiler shit is installed successfully now we need to do is to make a dir `*osdev*` or `*doorsOS*` whatever

* make a src dir and then add three files `*boot.s*`, `*kernel.c*` and `*linked.ld*`

* why the frick i m even typing i have uploaded the whole shit lol(i forgor)


## Project Structure

![Hierarchy](images/Screenshot%20From%202026-05-22%2017-37-23.png)

## Build n Run Cmd

LOCK IN HERE

| Command | Action |
| :--- | :--- |
| `make` | Compiles source files and links them into `myos.bin` |
| `make run` | Builds the project and launches QEMU with debug logs |
| `make clean` | Removes object files and binary outputs |


## Debugging

* The **`make run`** target is configured with specific QEMU flags to assist in kernel debugging:

* **`-d int,cpu_reset`**: Enables logging for hardware interrupts and CPU reset events.
* **`-no-reboot`**: Halts the emulator on a Triple Fault, allowing you to examine the register dump provided by QEMU.


## Roadmap


* [x] Toolchain Setup

* [x] Multiboot Header Integration

* [x] VGA Text Driver

* [ ] Interrupt Descriptor Table (IDT) (might need to sacrifice someone)

* [ ] Keyboard Driver (aukat se bahar)