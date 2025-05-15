#include "mik32_exception.h"


void INT_ATTR exception_handler(void)
{
    while (1)
    {
        uint32_t mcause;
        uint32_t pc;
        uint32_t instruction;
        uint32_t dummy1, dummy2;
        asm volatile(
            "csrr       %[mcause], mcause"     "\n\t"
            "csrr       %[pc], mepc"       "\n\t"
            "lh         %[instr],0(%[pc])"      "\n\t"
            "li         %[temp1],0b11"       "\n\t"
            "and        %[temp2],%[temp1],%[instr]"     "\n\t"
            "bne        %[temp1],%[temp2],metka_%="     "\n\t"
            "mv         %[temp1],x0"                    "\n\t"
            "lh         %[temp1],2(%[pc])"              "\n\t"
            "li         %[temp2],16"                    "\n\t"
            "sll        %[temp1],%[temp1],%[temp2]"     "\n\t"
            "add        %[instr],%[instr],%[temp1]"     "\n\t"
            "metka_%=:"
            : [mcause] "=r" (mcause)
            , [pc] "=r" (pc)
            , [instr] "=r" (instruction)
            , [temp1] "=r" (dummy1)
            , [temp2] "=r" (dummy2)
        );
        exception_decoder(mcause, pc, instruction);
        HAL_DelayMs(1000);
    }
}


void exception_decoder(uint32_t mcause, uint32_t pc, uint32_t instruction)
{
    xprintf("Exception (0x%X): ", mcause);
    switch (mcause)
    {
        case MCAUSE_INSTRUCTION_ADDRESS_MISALIGNED:
            xprintf("Instruction address misaligned");
            break;
        case MCAUSE_INSTRUCTION_ACCESS_FAULT:
            xprintf("Instruction access fault");
            break;
        case MCAUSE_ILLEGAL_INSTRUCTION:
            xprintf("Illegal instruction: (0x%08X)", instruction);
            break;
        case MCAUSE_BREAKPOINT:
            xprintf("Breakpoint");
            break;
        case MCAUSE_LOAD_ADDRESS_MISSALIGNED:
            xprintf("Load address misaligned");
            break;
        case MCAUSE_LOAD_ACCESS_FAULT:
            xprintf("Load access fault");
            break;
        case MCAUSE_STORE_ADDRESS_MISALIGNED:
            xprintf("Store address misaligned");
            break;
        case MCAUSE_STORE_ACCESS_FAULT:
            xprintf("Store access fault");
            break;
        case MCAUSE_ECALL_FROM_U_MODE:
            xprintf("ECALL from U-mode");
            break;
        case MCAUSE_ECALL_FROM_S_MODE:
            xprintf("ECALL from S-mode");
            break;
        case MCAUSE_ECALL_FROM_M_MODE:
            xprintf("ECALL from M-mode");
            break;
        case MCAUSE_INSTRUCTION_PAGE_FAULT:
            xprintf("Instruction page fault");
            break;
        case MCAUSE_STORE_PAGE_FAULT:
            xprintf("Store page fault");
            break;
        default:
            xprintf("Unexpected exception code (0x%X)", mcause);
    }
    /* PC-4 because MEPC contains the address of next instruction */
    xprintf(" occured at PC=0x%08X, instruction=0x%08X\n", pc, instruction);
}


void exception_caller(mik32_exception_test_t exception)
{
    switch (exception)
    {
    case EXCEPTION_ECALL_FROM_M_MODE:
        asm volatile(
            "addi       x10, x10, 2"        "\n\t" // ???
            "ecall"                         "\n\t"
        );
        break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
        asm volatile(
            "li         t0, 8"              "\n\t"
            "csrrs      x0, mhartid, t0"    "\n\t"
        );
        break;
    case EXCEPTION_LOAD_ADDRESS_MISSALIGNED:
        asm volatile(
            "li         s0, 0x7FFFFFFF"     "\n\t"
            "lw         s0, 0(s0)"          "\n\t"
        );
        break;
    case EXCEPTION_STORE_ADDRESS_MISALIGNED:
        asm volatile(
            "li         t0, 0x7FFFFFFE"     "\n\t"
            "sw         t0, 0(t0)"          "\n\t"
        );
        break;
    // ...
    }
}