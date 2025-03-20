#include "led_pattern.h"
#include "led_driver.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"

#define LED_STRIP_COUNT     (16)
#define LED_STATE_MAX       (10)

typedef struct LedPattern {
    uint8_t state_count;
    uint8_t power;
    uint32_t delay[LED_STATE_MAX];
} LedPattern_t;

