#include "uart.h"
#include "xprintf.h"
#include "mik32_hal_irq.h"
#include "mik32_hal_gpio.h"

#include "mik32_exception.h"


void SystemClock_Config();
void GPIO_Init();

uint32_t flag = 0;

int main()
{
    HAL_Init();

    SystemClock_Config();

    UART_Init(UART_0, 3333, UART_CONTROL1_TE_M | UART_CONTROL1_M_8BIT_M, 0, 0);

    GPIO_Init();

    /* Разрешить прерывания по уровню для линии EPIC GPIO_IRQ */
    HAL_EPIC_MaskLevelSet(HAL_EPIC_GPIO_IRQ_MASK);
    /* Разрешить глобальные прерывания */
    HAL_IRQ_EnableInterrupts();

    while (1)
    {
        HAL_GPIO_TogglePin(GPIO_0, GPIO_PIN_3);
        HAL_GPIO_TogglePin(GPIO_1, GPIO_PIN_3);
        
        HAL_DelayMs(500);

        // exception_caller(EXCEPTION_INSTRUCTION_ADDRESS_MISALIGNED);
        // exception_caller(EXCEPTION_ILLEGAL_INSTRUCTION);
        // exception_caller(EXCEPTION_ECALL_FROM_M_MODE);
        // exception_caller(EXCEPTION_LOAD_ADDRESS_MISSALIGNED);
        exception_caller(EXCEPTION_STORE_ADDRESS_MISALIGNED);
    }
}

void SystemClock_Config(void)
{
    PCC_InitTypeDef PCC_OscInit = {0};

    PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
    PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
    PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
    PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
    PCC_OscInit.AHBDivider = 0;
    PCC_OscInit.APBMDivider = 0;
    PCC_OscInit.APBPDivider = 0;
    PCC_OscInit.HSI32MCalibrationValue = 128;
    PCC_OscInit.LSI32KCalibrationValue = 8;
    PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
    PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;
    HAL_PCC_Config(&PCC_OscInit);
}

void GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_PCC_GPIO_0_CLK_ENABLE();
    __HAL_PCC_GPIO_1_CLK_ENABLE();
    __HAL_PCC_GPIO_IRQ_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = HAL_GPIO_MODE_GPIO_OUTPUT;
    GPIO_InitStruct.Pull = HAL_GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIO_0, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIO_1, &GPIO_InitStruct);
}