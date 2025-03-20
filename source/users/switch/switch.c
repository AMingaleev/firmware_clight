#include "switch.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_exti.h"

#define SWITCH_GPIO   (GPIOB)
#define SWITCH1_PIN   (GPIO_PIN_1)
#define SWITCH2_PIN   (GPIO_PIN_2)
#define SWITCH3_PIN   (GPIO_PIN_10)

static int32_t jumper = 0;    //переменная для переключателя

static void Switch_Init(void){
  GPIO_InitTypeDef gpio;

  //GPIO PB1 Input
  gpio.Pin    = GPIO_PIN_1;
  gpio.Mode   = GPIO_MODE_INPUT;
  gpio.Pull   = GPIO_PULLDOWN;
  gpio.Speed  = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio);

  //GPIO PB2 Input
  gpio.Pin    = GPIO_PIN_2;
  gpio.Mode   = GPIO_MODE_INPUT;
  gpio.Pull   = GPIO_PULLDOWN;
  gpio.Speed  = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio);

  //GPIO PB10 Input
  gpio.Pin    = GPIO_PIN_10;
  gpio.Mode   = GPIO_MODE_INPUT;
  gpio.Pull   = GPIO_PULLDOWN;
  gpio.Speed  = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &gpio);
}

uint32_t Switch_getState(void){
    if (HAL_GPIO_ReadPin(SWITCH_GPIO, SWITCH1_PIN)) {
      SET_BIT(jumper, 1);
    }
    else {
      CLEAR_BIT(jumper, 1);
    }

    if (HAL_GPIO_ReadPin(SWITCH_GPIO, SWITCH2_PIN)) {
      SET_BIT(jumper, 2);
    }
    else {
      CLEAR_BIT(jumper, 2);
    }

    if (HAL_GPIO_ReadPin(SWITCH_GPIO, SWITCH3_PIN)) {
      SET_BIT(jumper, 4);
    }
    else {
      CLEAR_BIT(jumper, 4);
    }

  return jumper;
}

void Switch_Setup(void){
  Switch_Init();
}



