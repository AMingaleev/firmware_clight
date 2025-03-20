#include "led_driver.h"
#include "switch.h"
#include "adc-driver.h"
#include "signal_extern.h"
#include "blink_turn.h"
#include "blink_break.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_cortex.h"
#include "stdint.h"

volatile uint32_t blink_time = 0;
uint32_t freq = 0;

void HAL_SYSTICK_Callback(void) {
  HAL_IncTick();
}

static SignalExtern_e     signal;                               //внешние сигналы
static uint32_t switch_comm = 0, prev_switch_comm = 0;          //переключатели

static void Error_Handler(void);

void main()
{
    HAL_Init();
    RCC_OscInitTypeDef Osc = {0};
    RCC_ClkInitTypeDef Clk = {0};
    Osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    Osc.HSIState = RCC_HSI_ON;                              // Частота 8MHz
    Osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;   // Вкл калибровку
    Osc.PLL.PLLState = RCC_PLL_ON;                          // Вкл PL
    Osc.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;             // 8/2 = 4MHz

    Osc.PLL.PLLMUL = RCC_PLL_MUL12;                         //4MHz * 12 = 48MHz
    if (HAL_RCC_OscConfig(&Osc) != HAL_OK)
    {
        Error_Handler();
    }

    Clk.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    Clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    Clk.AHBCLKDivider = RCC_SYSCLK_DIV1;                    //HCLK 48MHz
    Clk.APB1CLKDivider = RCC_HCLK_DIV2;                     //APB1 48MHz / 2 =  24MHz
    Clk.APB2CLKDivider = RCC_HCLK_DIV1;                     //APB2 48MHz

    if (HAL_RCC_ClockConfig(&Clk, FLASH_LATENCY_1) != HAL_OK)
    {
        Error_Handler();
    }
    LED_Setup();
    Switch_Setup();
    ADC_Setup();

    uint32_t switch_time = 0;
    uint32_t filter_freq;

    while(1) {
/*получить значение от внешних сигналов*/
        signal = SignalExtern_Readpins();
#if DEBUG
//        signal = SIGNAL_EXTERN_NONE;
//        signal = SIGNAL_EXTERN_HEAD;
//        signal = SIGNAL_EXTERN_BREAK;
        signal = SIGNAL_EXTERN_TURN;
//        signal = SIGNAL_EXTERN_SPECIAL;
#endif
/*получить значение в каком положении перемычки*/
        switch_comm = Switch_getState();
        if (switch_comm != prev_switch_comm) {
            if (switch_time > HAL_GetTick() || HAL_GetTick() - switch_time > 10) {
                switch_comm = Switch_getState();
            }
            switch_time = HAL_GetTick();
        }
        prev_switch_comm = switch_comm;

/*получить значение от потенциометра*/
        if (is_FlagAdc()) {                             //если флаг прерывание АЦП установлен
            filter_freq = get_Frequence();              //получить значение периода
            freq = (freq + filter_freq) / 2;            //фильтр второго порядка
            Speed_AdcStart();                           //запустить АЦП
        }

/*Выбрать сценарий и зажечь светодиоды*/
        switch (signal) {
            case SIGNAL_EXTERN_NONE:
            break;
            case SIGNAL_EXTERN_HEAD:
            break;
            case SIGNAL_EXTERN_BREAK:
                    BlinkPlay_BreakSignal(prev_switch_comm, freq);
            break;
            case SIGNAL_EXTERN_TURN:
                if (blink_time > HAL_GetTick() || HAL_GetTick() - blink_time > freq) {
                    BlinkPlay_TurnSignal(prev_switch_comm, freq);
                    blink_time = HAL_GetTick();
                }
            break;
            default:
            break;
        }
    }
}

void SysTick_Handler(void) {
    HAL_SYSTICK_IRQHandler();
}

void Error_Handler(void) {
    while(1){}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    HAL_TIM_Base_Stop_IT(htim);
    if (htim->Instance == TIM3) {
        LedDriver_SetFlagRise();
    }

    if (htim->Instance == TIM2) {
        LedDriver_SetFlagFall();
    }
}

