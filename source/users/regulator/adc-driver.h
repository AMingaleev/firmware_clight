#ifndef __ADC_DRIVER_H
#define __ADC_DRIVER_H

#include <stdint.h>

void ADC_Setup(void);
uint8_t is_FlagAdc(void);
void Speed_AdcStart(void);
uint32_t get_Brightness(void);
uint32_t get_Frequence(void);

#endif