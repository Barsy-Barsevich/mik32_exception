#include "mik32_exception.h"


void INT_ATTR exception_handler(void)
{
    while (1)
    {
        uint32_t mcause;
        uint32_t pc;
        uint32_t instruction;
        asm volatile(
            "csrr       %0, mcause"     "\n\t"
            "csrr       %1, mepc"       "\n\t"
            "lw         %2, 0(%1)"      "\n\t"
            : "=r" (mcause)
            , "=r" (pc)
            , "=r" (instruction)
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
    xprintf(" occured at PC=0x%08X\n", pc);
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
    case EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED:
        asm volatile(
            "li         t0, 0x7FFFFFFF"     "\n\t"
            "jalr       ra, t0, 0"          "\n\t"
        );
        break;
    // ...
    }
}