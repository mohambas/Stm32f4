#ifndef INPUT_H
#define INPUT_H

#include "stm32f4xx.h"
#include "delay.h"

#define ROW1 GPIO_Pin_0
#define ROW2 GPIO_Pin_1
#define ROW3 GPIO_Pin_4
#define ROW4 GPIO_Pin_5

#define COL1 GPIO_Pin_7
#define COL2 GPIO_Pin_8
#define COL3 GPIO_Pin_11
#define COL4 GPIO_Pin_12

#define SIZE_ROW 4
#define SIZE_COL 4


typedef enum {

  ZERO,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  A,
  B,
  C,
  D,
  E,
  F,
  NONE
  
} Key;

void initInput(void);

uint8_t isKeyPressed(Key);

Key waitUntilKeyPressed(void);

#endif