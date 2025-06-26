# MIK32 Exception handling library
*For debugging*

\[[RU](./RU_README.md)/EN]

---
## System requirements
### Libraries
- `mik32_transaction`
- `mik32_stdio`

---
## Abstract
Exception means critical situation when the processor do not know what is to do. Exceptions can be caused by illegal instruction reading, reading or writing to protected memory area, alignment error etc. On the MIK32 platform based on SCR1 core any exception or interrupt events call the `MTVEC` trap address.

This library contains a fast trap handler that find out, what caused the trap calling, interrupt or exception. In interrupt case it jumps to interrupt handler. In exception case it jumps to weak exception handler (can be overwritten). By default, exception handler writes to the serial, which exception type happened.

---
## Enums
#### `mik32_exception_test_t`
- `EXCEPTION_ILLEGAL_INSTRUCTION = 2` - illegal instruction execution attempt;
- `EXCEPTION_LOAD_ADDRESS_MISSALIGNED = 4` - alignment error while loading value;
- `EXCEPTION_STORE_ADDRESS_MISALIGNED = 6` - alignment error while storing value;
- `EXCEPTION_ECALL_FROM_M_MODE = 11` - `ECALL` instruction executed.

---
## Methods descriptions
#### `void mik32_exception_handler(void)`
A weak exception handler. Can be overwritten.
#### `mik32_exception_decoder(uint32_t mcause, uint32_t pc, uint32_t instruction)`
Decodes the exception reason.
- `mcause` - `MCAUSE` register contents;
- `pc` - `PC` (`x31`) contents while exception;
- `instruction` - opcode, caused the exception.
#### `void exception_caller(`[`mik32_exception_test_t`](#mik32_exception_test_t)`)`
Calls the needed exception (for tests).
