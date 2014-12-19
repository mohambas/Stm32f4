#include "keypad.h"

void initKeypad(){

  GPIO_InitTypeDef GPIO_InitStruct;
  
  // Enable clock for GPIOA
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  // Enable clock for GPIOC
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  
  // Enable SYSCFG clock

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  // mode select

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN; 
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  
  // Play/Pause button

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; 
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // Stop button
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2; 
  GPIO_Init(GPIOA, &GPIO_InitStruct); 
  
  // Previous/Volume down button/trebble down...
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  // Next/volume up/trebble up...
  
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4; 
  GPIO_Init(GPIOE, &GPIO_InitStruct);
  
}

uint8_t getMode(void){

  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
  
}

uint8_t getPlayPause(void){

  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1);
  
}

uint8_t getStop(void){

  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
  
}

uint8_t getPrevInc(void){

  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
  
}

uint8_t getNextInc(void){

  return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
  
}




