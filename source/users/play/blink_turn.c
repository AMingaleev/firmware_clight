#include "blink_turn.h"
#include "blink.h"
#include "switch.h"

#define TOKEN_TAKE          (1)
#define TOKEN_GIVE          (0)
#define LED_ARRAY_MAX       (16)

//static void BlinkPlay_ResetHeadCounter(void);
//static void BlinkPlay_ResetTailCounter(void);

static void BlinkPlay_Turn0(void);
static void BlinkPlay_Turn1(void);
static void BlinkPlay_Turn2(void);
static void BlinkPlay_Turn3(void);
static void BlinkPlay_Turn4(void);
static void BlinkPlay_Turn5(void);

static volatile uint32_t tms = 0;
static uint32_t frequence = 0;

static void (*turn_function_callback[TURN_BLINK_INDEX])(void) = {
    BlinkPlay_Turn0,
    BlinkPlay_Turn1,
    BlinkPlay_Turn2,
    BlinkPlay_Turn3,
    BlinkPlay_Turn4,
    BlinkPlay_Turn5,
};

static LedList_e head_counter     = CHANNEL_OUT_1;
static LedList_e tail_counter     = CHANNEL_OUT_1;
static LedList_e turn_2_counter   = CHANNEL_OUT_1;
static LedList_e turn_3_counter   = CHANNEL_OUT_INDEX;

static void BlinkPlay_ResetHeadCounter(void){
    head_counter = CHANNEL_OUT_1;
}

/**
TURN 0
—ветодиоды по очередно включаютс€ по все длинне ленты, в конце все гаснут
*/
static void BlinkPlay_Turn0(void) {
    static LedStrip_t strip;

    if (!strip.head) {
       Blink_AllReset();
    }

    if (strip.head < CHANNEL_OUT_INDEX) {
        LedDriver_On(strip.head);
        strip.head++;
    }
    else {
        strip.head = CHANNEL_OUT_1;
    }
}

/**
  TURN 1
  —ветодиды по очередно гаснут по все длине ленты, в конце все загораютс€
*/
static void BlinkPlay_Turn1(void) {
    if (!head_counter){
        Blink_AllSet();
    }

    if (head_counter < CHANNEL_OUT_INDEX) {
        LedDriver_Off(head_counter);
        head_counter++;
    }
    else {
        head_counter = CHANNEL_OUT_1;
    }
}

/**
  TURN2
*/
static void BlinkPlay_Turn2(void) {
    const uint8_t strip = 5;

    if (!head_counter) {
        Blink_AllReset();
    }

    if (head_counter < CHANNEL_OUT_INDEX) {
        LedDriver_On(head_counter);
        head_counter++;
    }
    else  if (head_counter >= CHANNEL_OUT_INDEX && tail_counter >= CHANNEL_OUT_INDEX) {
        head_counter    = CHANNEL_OUT_1;
        tail_counter    = CHANNEL_OUT_1;
        turn_2_counter  = CHANNEL_OUT_1;
    }

    if (turn_2_counter >= strip) {
        LedDriver_Off(tail_counter);
        tail_counter++;
    }
    turn_2_counter++;
}

/**
TURN3
—ветодиоды по очередно загараютс€ создают эффект стройки
*/
static void BlinkPlay_Turn3(void) {
    static uint8_t token = TOKEN_GIVE;

    if (!turn_3_counter) {
        Blink_AllReset();
    }

    if (!head_counter) {
        token = TOKEN_TAKE;
    }

    if (token) {
        LedDriver_On(head_counter);
        head_counter++;
    }

    if (!token) {
        LedDriver_Off(tail_counter);
        tail_counter++;
    }

    if (head_counter >= turn_3_counter && turn_3_counter)
    {
        LedDriver_On(--head_counter);
        turn_3_counter--;
        head_counter = CHANNEL_OUT_1;
        tail_counter = CHANNEL_OUT_1;
    }

    if (!turn_3_counter) {
        head_counter    = CHANNEL_OUT_1;
        tail_counter    = CHANNEL_OUT_1;
        turn_3_counter  = CHANNEL_OUT_INDEX;
        Blink_AllReset();
    }

    token = (token == TOKEN_TAKE) ? TOKEN_GIVE : TOKEN_TAKE;
}

/**
TURN4
—ветодиод поочередно заграетс€ и поочередну гаснет
*/
static void BlinkPlay_Turn4(void) {
  static uint8_t token = TOKEN_GIVE;

  if (head_counter < CHANNEL_OUT_INDEX) {
    if (!token) {
      LedDriver_On(head_counter);
    }
    else {
      LedDriver_Off(head_counter);
    }
   head_counter++;
  }
  else {
    BlinkPlay_ResetHeadCounter();
    token = (token == TOKEN_TAKE) ? TOKEN_GIVE : TOKEN_TAKE;
  }
}

static void BlinkPlay_Turn5(void){
    static FlagStatus side_flag = RESET;
    static LedStrip_t strip;
    strip.strip = 5;

    if (!side_flag) {                                           //если бежит в одну сторону
        if (strip.head < CHANNEL_OUT_INDEX) {                   //пока head не дошел до конца
            LedDriver_On(strip.head);                           //включаю светодиод
            strip.head++;                                       //прибавл€ю head
        }
        if (strip.head > strip.strip) {                         //если head больше 5 светодиодов
            LedDriver_Off(strip.tail);                          //начинию гасить tail
            strip.tail++;                                       //прибавл€ю tail
        }
        if (strip.tail >= CHANNEL_OUT_INDEX) {                  //если tail дошел до конца
             side_flag = SET;                                   //устанавливаю флаг в обратную сторону
             strip.head = CHANNEL_OUT_16;                       //устанавливаю позиции начальный позиции head и tail
             strip.tail = CHANNEL_OUT_16;
        }
    }
    else {
        if (strip.tail > CHANNEL_OUT_1) {
            LedDriver_On(strip.tail);
            strip.tail--;
        }

        if (strip.tail < (CHANNEL_OUT_INDEX - strip.strip)) {
            LedDriver_Off(strip.head);
            strip.head--;
        }
        if (strip.head == CHANNEL_OUT_1) {
            side_flag = RESET;
        }
    }
}

void BlinkPlay_TurnSignal(uint8_t jumper, uint32_t freq) {
    static uint8_t prev_jumper;

    if (jumper != prev_jumper && jumper < TURN_BLINK_INDEX) {
        Blink_AllReset();
        prev_jumper = jumper;
        head_counter = CHANNEL_OUT_1;
        tail_counter = CHANNEL_OUT_1;
        turn_3_counter = CHANNEL_OUT_INDEX;
    }
    turn_function_callback[prev_jumper]();
}