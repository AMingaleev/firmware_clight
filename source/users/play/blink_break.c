#include "blink_break.h"
#include "blink.h"
#include "switch.h"
#include "led_driver.h"

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

#define BREAK_BLINK_FUNCTION_MAX  (6)

static void BlinkBreak_0(void);
static void BlinkBreak_1(void);
static void BlinkBreak_2(void);
static void BlinkBreak_3(void);
static void BlinkBreak_4(void);
static void BlinkBreak_5(void);

static volatile uint32_t tms = 0;          
static uint32_t frequence = 0;

static void (*break_function_callback[BREAK_BLINK_FUNCTION_MAX])(void) = {
    BlinkBreak_0,
    BlinkBreak_1,
    BlinkBreak_2,
    BlinkBreak_3,
    BlinkBreak_4,
    BlinkBreak_5,
};

static void BlinkBreak_0(void) {
    Blink_AllSet();
}

static void BlinkBreak_1(void) {
    static int8_t flag = BLINK_STEP_DECREASE_DONE;
    if (flag != BLINK_STEP_INCREASE_DONE) {
        flag = Blink_Increase();
    }
    if (flag != BLINK_STEP_DECREASE_DONE) {
        flag = Blink_Decrease();
    }
}

static void BlinkBreak_2(void) {
    static FlagStatus flag = RESET;
    uint8_t i = 0;

    if (restart == RESET) {
        do {
            if ((tms > HAL_GetTick() || HAL_GetTick() - tms > 100) && !flag) {
                Blink_AllSet();
                tms = HAL_GetTick();
                flag = SET;
            }

            if ((tms > HAL_GetTick() || HAL_GetTick() - tms > 100) && flag) {
                Blink_AllReset();
                tms = HAL_GetTick();
                flag = RESET;
                i++;
            }
        }while (i != 4);
    }
    restart = SET;
    Blink_AllSet();
}

static void BlinkBreak_3(void) {
    static FlagStatus flag = RESET;
    uint8_t i = 0;

    if (restart == RESET) {
        do {
            if ((tms > HAL_GetTick() || HAL_GetTick() - tms > 100) && !flag) {
                Blink_AllSet();
                tms = HAL_GetTick();
                flag = SET;
            }

            if ((tms > HAL_GetTick() || HAL_GetTick() - tms > 100) && flag) {
                Blink_AllReset();
                tms = HAL_GetTick();
                flag = RESET;
                i++;
            }
        }while (i != 4);
    }
    restart = SET;
    BlinkBreak_1();
}

static void BlinkBreak_4(void) {
    static uint8_t flag = BLINK_STEP_NONE;

    if (flag != BLINK_STEP_INCREASE_DONE) {
        flag = Blink_Increase();
    }
    else {
        Blink_AllSet();
    }
}

static void BlinkBreak_5(void) {
    const uint8_t STEP_1 = 1;
    const uint8_t STEP_2 = 2;
    const uint8_t STEP_3 = 3;
    const uint8_t STEP_4 = 4;

    static LedList_e counter;
    static uint8_t step = 0;

    if (restart == RESET) {
        Blink_AllReset();
        counter = CHANNEL_OUT_1;
        step = 1;
    }

    if (step == STEP_1) {
        if (tms > HAL_GetTick() || HAL_GetTick() - tms > frequence) {
            LedDriver_On(counter);
            counter++;
            tms = HAL_GetTick();
        }

        if (counter == CHANNEL_OUT_INDEX) {
            counter = CHANNEL_OUT_16;
            step++;
            Blink_AllReset();
        }
    }

    if (step == STEP_2) {
        if (tms > HAL_GetTick() || HAL_GetTick() - tms > frequence) {
            LedDriver_On(counter);
            counter--;
            tms = HAL_GetTick();
        }

        if (counter == CHANNEL_OUT_1) {
            counter = CHANNEL_OUT_1;
            step++;
            Blink_AllReset();
        }
    }

    if (step == STEP_3) {
        if (tms > HAL_GetTick() || HAL_GetTick() - tms > frequence) {
            LedDriver_On(counter);
            counter++;
            tms = HAL_GetTick();
        }

        if (counter == CHANNEL_OUT_INDEX) {
            counter = CHANNEL_OUT_16;
            step++;
        }
    }

    if (step == STEP_4) {
        Blink_AllSet();
    }

    restart = SET;
}


void BlinkPlay_BreakSignal(uint8_t jumper, uint32_t freq) {
    static uint8_t prev_jumper;
    frequence = freq;

    if (jumper != prev_jumper && jumper < BREAK_BLINK_FUNCTION_MAX) {
        Blink_AllReset();
        prev_jumper = jumper;
        restart = RESET;
    }

    break_function_callback[prev_jumper]();
}
