.global keyboard_handler_stub
.extern keyboard_handler
.extern outb

keyboard_handler_stub:
    pusha               # Save general purpose registers
    call keyboard_handler
    
    # Acknowledge the interrupt by sending EOI (End of Interrupt) to the Master PIC
    mov $0x20, %al
    mov $0x20, %dx
    out %al, %dx
    
    popa                # Restore registers
    iret                # Return from interrupt
    