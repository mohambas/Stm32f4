#include "Input.h"

static Key pressedKey(void);

static Key keys[SIZE_ROW][SIZE_COL] = {

  {ONE, TWO, THREE, A},
  {FOUR, FIVE, SIX, B},
  {SEVEN, EIGHT, NINE, C},
  {E, ZERO, F, D}
  
};

static uint32_t rows[SIZE_ROW] = {

  ROW1, ROW2, ROW3, ROW4
  
};

static uint32_t cols[SIZE_COL] = {

  COL1, COL2, COL3, COL4
  
};

void initInput(void){
  
  // GPIO structure
  
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable clock for GPIOB
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  // Configure output pins
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; // Set pins to output mode
  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // Set pins' output to open drain
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Max freq 50 Mhz
  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // Enable pull up resistor
  
  // Init outputs
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Configure input pins
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_11 | GPIO_Pin_12;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // Set pins to input mode
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // Max freq 50 Mhz
  
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; ; // Disable pull up/down resistors
  
  // Init inputs
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
}

uint8_t isKeyPressed(Key key){
  
    if(pressedKey() == key)
      return 1;
    
    return 0;
  
}

Key waitUntilKeyPressed(void){
  
  Key key;

  while((key = pressedKey()) == NONE);
  
  while(pressedKey() != NONE);
  
  return key;

}

static Key pressedKey(void){
  
  Key pressedKey = NONE;
  
  for(uint8_t i = 0; i <SIZE_ROW; i++){
    
    GPIO_SetBits(GPIOB, rows[i]);
  
    for(uint8_t j = 0; j < SIZE_COL; j++){
      
      if(GPIO_ReadInputDataBit(GPIOB, cols[j])){
        
        // Delay
        
        delay_nms(5);
        
        // Check again
        
        if(GPIO_ReadInputDataBit(GPIOB, cols[j]))
          pressedKey = keys[i][j];
        
        break;
        
      }
      
    }
    
    GPIO_ResetBits(GPIOB, rows[i]);
    
    if(pressedKey != NONE)
      break;
    
  }
  
  return pressedKey;

}