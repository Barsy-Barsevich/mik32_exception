#pragma once

#include "mik32_hal.h"
#include "scr1_csr_encoding.h"

typedef enum
{
    EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED = 0,
    // EXCEPTION_INSTRUCTION_ACCESS_FAULT = 1,
    EXCEPTION_ILLEGAL_INSTRUCTION = 2,
    // EXCEPTION_BREAKPOINT = 3,
    EXCEPTION_LOAD_ADDRESS_MISSALIGNED = 4,
    // EXCEPTION_LOAD_ACCESS_FAULT = 5,
    EXCEPTION_STORE_ADDRESS_MISALIGNED = 6,
    // EXCEPTION_STORE_ACCESS_FAULT = 7,
    // EXCEPTION_ECALL_FROM_U_MODE = 8,
    // EXCEPTION_ECALL_FROM_S_MODE = 9,
    EXCEPTION_ECALL_FROM_M_MODE = 11,
    // EXCEPTION_INSTRUCTION_PAGE_FAULT = 12,
    // EXCEPTION_LOAD_PAGE_FAULT = 13,
    // EXCEPTION_STORE_PAGE_FAULT = 15
} mik32_exception_test_t;

void exception_handler(void);
void exception_decoder(uint32_t mcause, uint32_t pc, uint32_t instruction);

void exception_caller(mik32_exception_test_t exception);