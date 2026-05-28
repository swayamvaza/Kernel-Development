.global isr_common_stub
.extern isr_handler

# This is the generic stub for exceptions
isr_common_stub:
    pusha
    push %ds
    
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    
    call isr_handler
    
    pop %ds
    popa
    # REMOVED: add $8, %esp (This was corrupting your stack before iret)
    iret


.global keyboard_handler_stub
.extern keyboard_handler

# This is the specific stub for the keyboard
keyboard_handler_stub:
    pusha
    push %ds
    
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    call keyboard_handler

    # REMOVED: Assembly EOI. 
    # Your C function keyboard_handler() already handles outb(0x20, 0x20)
    
    pop %ds
    popa
    iret
    