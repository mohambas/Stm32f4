#ifndef CPU_H
#define CPU_G

#include "stm32f4xx.h"

#define FONTS_START_ADDR 0x50
#define PROGRAM_START_ADDR 0x200

#define CPU_NUM_OF_CAT 16
#define CPU_STACK_SIZE 16

void InitCpu(const uint8_t*, int);

void fetch(void);

void execute(void);

#endif