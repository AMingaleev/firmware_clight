#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#include "stdint.h"
#include "stm32f1xx.h"

typedef enum LedList{
    CHANNEL_OUT_1 = 0,
    CHANNEL_OUT_2,
    CHANNEL_OUT_3,
    CHANNEL_OUT_4,
    CHANNEL_OUT_5,
    CHANNEL_OUT_6,
    CHANNEL_OUT_7,
    CHANNEL_OUT_8,
    CHANNEL_OUT_9,
    CHANNEL_OUT_10,
    CHANNEL_OUT_11,
    CHANNEL_OUT_12,
    CHANNEL_OUT_13,
    CHANNEL_OUT_14,
    CHANNEL_OUT_15,
    CHANNEL_OUT_16,
    CHANNEL_OUT_INDEX,
}LedList_e;

void LED_Setup(void);
void LedDriver_On(LedList_e index);
void LedDriver_Off(LedList_e index);
void LedDriver_Toogle(LedList_e index);
void LedDriver_ShiftOn(LedList_e channel, uint8_t shift);
void LedDriver_ShiftOff(LedList_e channel, uint8_t shift);
FlagStatus LedDriver_isFlagRise(void);
FlagStatus LedDriver_isFlagFall(void);
void LedDriver_SetFlagRise(void);
void LedDriver_ResetFlagRise(void);
void LedDriver_SetFlagFall(void);
void LedDriver_ResetFlagFall(void);

void LedDriver_TimerRiseStart(void);
void LedDriver_TimerRiseStop(void);
void LedDriver_TimerFallStart(void);
void LedDriver_TimerFallStop(void);

void LedDriver_SetFrequence(uint32_t frequence);
void LedDriver_SetBrightness(uint32_t brightness);
#endif
