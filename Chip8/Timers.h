#ifndef TIMERS_H
#define TIMERS_H

#include "stm32f4xx.h"

void decreaseTimers(void);

void setDelayTimer(uint8_t);

uint8_t getDelayTimer(void);

void setAudioTimer(uint8_t);

#endif