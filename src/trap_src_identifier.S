.globl exception_handler
.globl interrupt_handler
.weak  exception_handler
.weak  interrupt_handler


raw_trap_handler:
    sw      x5, -4(sp)
    sw      x6, -8(sp)
    csrr    x5, mcause
    li      x6, 0x80000000
    and     x5, x5, x6
    lw      x6, -8(sp)
    beqz    x5, raw_exception_handler
    lw      x5, -4(sp)
    j       interrupt_handler
raw_exception_handler:
    lw      x5, -4(sp)
    j       exception_handler


exception_handler:
    mret

interrupt_handler:
    mret