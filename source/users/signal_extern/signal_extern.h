#ifndef __SIGNAL_EXTERN_H
#define __SIGNAL_EXTERN_H

#include <stdint.h>

typedef enum {
    SIGNAL_EXTERN_NONE = 0,
    SIGNAL_EXTERN_HEAD,
    SIGNAL_EXTERN_TURN,
    SIGNAL_EXTERN_BREAK,
    SIGNAL_EXTERN_SPECIAL,
} SignalExtern_e;

void SignalExtern_Setup(void);
SignalExtern_e SignalExtern_Readpins(void);

#endif