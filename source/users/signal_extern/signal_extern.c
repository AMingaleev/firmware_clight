#include "signal_extern.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_exti.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"

#define HEAD_BREAK_GPIO     (GPIOA)
#define HEAD_SIGNAL_PIN     (GPIO_PIN_0)
#define BREAK_SIGNAL_PIN    (GPIO_PIN_7)
#define TURN_GPIO           (GPIOB)
#define TURN_SIGNAL_PIN	    (GPIO_PIN_6)

static SignalExtern_e signal_extern;

static void SignalExtern_Init(void);

static void SignalExtern_Init(void){
  GPIO_InitTypeDef          gpio;

/*GPIO PA0 input*/
    gpio.Pin        = HEAD_SIGNAL_PIN;
    gpio.Mode       = GPIO_MODE_INPUT;
    gpio.Pull       = GPIO_PULLDOWN;
    gpio.Speed      = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(HEAD_BREAK_GPIO, &gpio);

/*GPIO PA7 input*/
    gpio.Pin        = BREAK_SIGNAL_PIN;
    gpio.Mode       = GPIO_MODE_INPUT;
    gpio.Pull       = GPIO_PULLDOWN;
    gpio.Speed      = GPIO_SPEED_LOW;
    HAL_GPIO_Init(HEAD_BREAK_GPIO, &gpio);

/*GPIO PA6 input*/
    gpio.Pin      = TURN_SIGNAL_PIN;
    gpio.Mode     = GPIO_MODE_INPUT;
    gpio.Pull     = GPIO_PULLDOWN;
    gpio.Speed    = GPIO_SPEED_LOW;
    HAL_GPIO_Init(TURN_GPIO, &gpio);
}

SignalExtern_e SignalExtern_Readpins(void) {
    signal_extern = SIGNAL_EXTERN_NONE;
    if (HAL_GPIO_ReadPin(HEAD_BREAK_GPIO, HEAD_SIGNAL_PIN) == GPIO_PIN_SET) {
        signal_extern = SIGNAL_EXTERN_HEAD;
    };
    if (HAL_GPIO_ReadPin(HEAD_BREAK_GPIO, BREAK_SIGNAL_PIN) == GPIO_PIN_SET) {
        signal_extern = SIGNAL_EXTERN_BREAK;
    };
    if (HAL_GPIO_ReadPin(TURN_GPIO, TURN_SIGNAL_PIN) == GPIO_PIN_SET) {
        signal_extern = SIGNAL_EXTERN_TURN;
    };
    return signal_extern;
}

void SignalExtern_Setup(void){
    SignalExtern_Init();
}




