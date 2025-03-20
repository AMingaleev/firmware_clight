#ifndef __BLINK_TURN_H
#define __BLINK_TURN_H

#include <stdint.h>

#define TURN_SIGNAL_SEQUENTILL_ON                       (0)
#define TURN_SIGNAL_SEQUENTILL_OFF                      (1)
#define TURN_SIGNAL_SEQUENTILL_ON_AFTER_5               (2)
#define TURN_SIGNAL_SEQUENTILL_BUILD                    (3)
#define TURN_SIGNAL_SEQUENTILL_ON_OFF                   (4)
#define TURN_SIGNAL_SEQUENTILL_FORWARD_BACK_5_STRIP     (5)
#define TURN_BLINK_INDEX                                (6)

void BlinkPlay_TurnSignal(uint8_t jumper, uint32_t freq);
 void BlinkPlay_AllReset(void);
  void BlinkPlay_AllSet(void);

#endif