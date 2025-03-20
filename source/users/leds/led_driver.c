#include "led_driver.h"

#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"

#include "config.h"

#define APB1_FREQUENCE_HZ                       (48000000)
#define LED_BLINK_FREQUENCE_HZ                  (100000)
#define TIMER_PRESCALER_HZ                      (APB1_FREQUENCE_HZ/LED_BLINK_FREQUENCE_HZ)       //100KHz
#define LED_FREQUNCE_DEFAULT                    (1000)
#define LED_BRIGHTNESS_DEFAULT                  (100)

typedef struct LedConfig {
    GPIO_TypeDef      *gpio;
    GPIO_InitTypeDef  type;
    GPIO_PinState     state;
} LedConfig_t;

static LedConfig_t leds[CHANNEL_OUT_INDEX] = {
    {.gpio = CHANNEL_1_GPIO,	{.Pin = CHANNEL_1_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_1
    {.gpio = CHANNEL_2_GPIO,	{.Pin = CHANNEL_2_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_2
    {.gpio = CHANNEL_3_GPIO,	{.Pin = CHANNEL_3_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_3
    {.gpio = CHANNEL_4_GPIO,	{.Pin = CHANNEL_4_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_4
    {.gpio = CHANNEL_5_GPIO,	{.Pin = CHANNEL_5_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_5
    {.gpio = CHANNEL_6_GPIO,	{.Pin = CHANNEL_6_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_6
    {.gpio = CHANNEL_7_GPIO,	{.Pin = CHANNEL_7_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_7
    {.gpio = CHANNEL_8_GPIO,	{.Pin = CHANNEL_8_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_8
    {.gpio = CHANNEL_9_GPIO,	{.Pin = CHANNEL_9_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_9
    {.gpio = CHANNEL_10_GPIO,	{.Pin = CHANNEL_10_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_10
    {.gpio = CHANNEL_11_GPIO,	{.Pin = CHANNEL_11_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_11
    {.gpio = CHANNEL_12_GPIO,	{.Pin = CHANNEL_12_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_12
    {.gpio = CHANNEL_13_GPIO,	{.Pin = CHANNEL_13_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_13
    {.gpio = CHANNEL_14_GPIO,	{.Pin = CHANNEL_14_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_14
    {.gpio = CHANNEL_15_GPIO,	{.Pin = CHANNEL_15_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_15
    {.gpio = CHANNEL_16_GPIO,	{.Pin = CHANNEL_16_PIN,	    .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Speed = GPIO_SPEED_FREQ_LOW}, .state = GPIO_PIN_RESET}, //CHANNEL_OUT_16
};

static TIM_HandleTypeDef timerRise;
static TIM_HandleTypeDef timerFall;

static FlagStatus isFlagRise = RESET;
static FlagStatus isFlagFall = RESET;

void LedDriver_On(LedList_e channel) {
    HAL_GPIO_WritePin(leds[channel].gpio, leds[channel].type.Pin, GPIO_PIN_SET);
}

void LedDriver_Off(LedList_e channel) {
    HAL_GPIO_WritePin(leds[channel].gpio, leds[channel].type.Pin, GPIO_PIN_RESET);
}

void LedDriver_Toogle(LedList_e channel) {
    HAL_GPIO_TogglePin(leds[channel].gpio, leds[channel].type.Pin);
}

void LedDriver_SetFlagRise(void) {
    isFlagRise = SET;
}

void LedDriver_ResetFlagRise(void) {
    isFlagRise = RESET;
}

void LedDriver_SetFlagFall(void) {
    isFlagFall = SET;
}

void LedDriver_ResetFlagFall(void) {
    isFlagFall = RESET;
}

FlagStatus LedDriver_isFlagRise(void) {
    return isFlagRise;
}

FlagStatus LedDriver_isFlagFall(void) {
    return isFlagFall;
}

void LedDriver_TimerRiseStart(void) {
    HAL_TIM_Base_Start_IT(&timerRise);
}

void LedDriver_TimerRiseStop(void) {
    HAL_TIM_Base_Stop_IT(&timerRise);
}

void LedDriver_TimerFallStart(void) {
    HAL_TIM_Base_Start_IT(&timerFall);
}

void LedDriver_TimerFallStop(void) {
    HAL_TIM_Base_Stop_IT(&timerFall);
}

void LedDriver_SetFrequence(uint32_t frequence) {
    TIM3->ARR = frequence;
}

void LedDriver_SetBrightness(uint32_t brightness) {
    TIM2->ARR = brightness;
}


void LED_Setup(void) {
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

/*Clock setup*/
    __HAL_RCC_GPIOA_CLK_ENABLE();       //GPIOA clock enable
    __HAL_RCC_GPIOB_CLK_ENABLE();       //GPIOB clock enable
    __HAL_RCC_AFIO_CLK_ENABLE();        //AFIO clock enable
    __HAL_RCC_TIM3_CLK_ENABLE();        //TIM3 clock enable
    __HAL_RCC_TIM2_CLK_ENABLE();        //TIM2 clock enable

/*Afio setup*/
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

/*Gpio setup*/
    for (LedList_e i = CHANNEL_OUT_1; i < CHANNEL_OUT_INDEX; i++) {
        HAL_GPIO_Init(leds[i].gpio, &leds[i].type);
        HAL_GPIO_WritePin(leds[i].gpio, leds[i].type.Pin, leds[i].state);
    }

/*TIM3 setup*/
    timerRise.Instance          = TIM3;
    timerRise.Init.Prescaler    = (TIMER_PRESCALER_HZ - 1);
    timerRise.Init.CounterMode  = TIM_COUNTERMODE_UP;
    timerRise.Init.Period       = LED_FREQUNCE_DEFAULT;
    timerRise.Init.ClockDivision    =TIM_CLOCKDIVISION_DIV1;
    timerRise.Init.AutoReloadPreload    = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIM_Base_Init(&timerRise) != HAL_OK) {
        while(1);
    };

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&timerRise, &sClockSourceConfig) != HAL_OK) {
        while(1);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&timerRise, &sMasterConfig) != HAL_OK) {
        while(1);
    }

/*TIM2 setup*/
    timerFall.Instance          = TIM2;
    timerFall.Init.Prescaler    = (TIMER_PRESCALER_HZ - 1);
    timerFall.Init.CounterMode  = TIM_COUNTERMODE_UP;
    timerFall.Init.Period       = LED_BRIGHTNESS_DEFAULT;
    timerFall.Init.ClockDivision    =TIM_CLOCKDIVISION_DIV1;
    timerFall.Init.AutoReloadPreload    = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIM_Base_Init(&timerFall) != HAL_OK) {
        while(1);
    };

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&timerFall, &sClockSourceConfig) != HAL_OK) {
        while(1);
    }

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&timerFall, &sMasterConfig) != HAL_OK) {
        while(1);
    }

    HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    LedDriver_TimerRiseStart();
    LedDriver_TimerFallStart();
}

void TIM3_IRQHandler(void) {
    HAL_TIM_IRQHandler(&timerRise);
}

void TIM2_IRQHandler(void) {
    HAL_TIM_IRQHandler(&timerFall);
}





