#include "blink.h"

#define BRIGHTNESS_VALUE_MAX        (900)
#define BRIGHTNESS_VALUE_STEP       (10)

FlagStatus restart = RESET;

static uint32_t brightness = BRIGHTNESS_VALUE_STEP;

void Blink_AllSet(void) {
    for (LedList_e i = CHANNEL_OUT_1; i < CHANNEL_OUT_INDEX; i++) {
        LedDriver_On(i);
    }
}

void Blink_AllReset(void) {
    for (LedList_e i = CHANNEL_OUT_1; i < CHANNEL_OUT_INDEX; i++) {
        LedDriver_Off(i);
    }
}

uint8_t Blink_Increase(void) {
    uint8_t result = BLINK_STEP_DECREASE_DONE;
    LedDriver_SetBrightness(brightness);
    if (LedDriver_isFlagRise() == SET) {
            if (brightness < BRIGHTNESS_VALUE_MAX) {
                brightness += BRIGHTNESS_VALUE_STEP;
            }
            else {
                result = BLINK_STEP_INCREASE_DONE;
            }
            Blink_AllSet();
            LedDriver_ResetFlagRise();
            LedDriver_TimerFallStart();
    }
    if (LedDriver_isFlagFall() == SET) {
        Blink_AllReset();
        LedDriver_ResetFlagFall();
        LedDriver_TimerRiseStart();
    }
    return result;
}

uint8_t Blink_Decrease(void) {
    uint8_t result = BLINK_STEP_INCREASE_DONE;
    LedDriver_SetBrightness(brightness);
    if (LedDriver_isFlagRise() == SET) {
        if (brightness > BRIGHTNESS_VALUE_STEP) {
            brightness -= BRIGHTNESS_VALUE_STEP;
        }
        else {
            result = BLINK_STEP_DECREASE_DONE;
        }
        Blink_AllSet();
        LedDriver_ResetFlagRise();
        LedDriver_TimerFallStart();
    }
    if (LedDriver_isFlagFall() == SET) {
        Blink_AllReset();
        LedDriver_ResetFlagFall();
        LedDriver_TimerRiseStart();
    }
    return result;
}

void Blink_ChannelOn(LedList_e channel) {
    LedDriver_On(channel);
}

void Blink_ChannelOff(LedList_e channel) {
     LedDriver_Off(channel);
}