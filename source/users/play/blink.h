#ifndef __BLINK_H
#define __BLINK_H

#include "led_driver.h"

#define BLINK_STEP_NONE                     (0)
#define BLINK_STEP_INCREASE_DONE            (1)
#define BLINK_STEP_DECREASE_DONE            (2)

#define BLINK_FLAG_BEGIN                    (0)
#define BLINK_FLAG_DONE                     (1)

typedef enum {
    BLINK_ACTION_CONTINUE = 0,
    BLINK_ACTION_DONE,
} blink_action_e;

typedef struct LedStrip {
    LedList_e head;
    LedList_e tail;
    LedList_e counter;
    uint8_t strip;
    uint32_t brightness;
}LedStrip_t;

extern FlagStatus restart;

void Blink_AllSet(void);
void Blink_AllReset(void);

uint8_t Blink_Increase(void);
uint8_t Blink_Decrease(void);

void Blink_ChannelOn(LedList_e channel);
void Blink_ChannelOff(LedList_e channel);

#endif