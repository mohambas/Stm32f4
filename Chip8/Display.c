#include "Display.h"

// Variables

static unsigned char display[WIDTH * HEIGHT];

void initDisplay(void){

  // Init ssd1289
  
  LCD_Init();
  
  // Clear display
  
  LCD_Clear(BACKGROUND_COLOR);
  
}

uint8_t showSprite(uint8_t x, uint8_t y, uint8_t* sprite, uint8_t size){

  // Variables
  
  uint8_t xT, yT, vf, spriteCounter = 0;
  
  // Apply mod operations on x and y to wrap around if x bigger is than WIDTH or y is bigger than HEIGHT
  
  x = x % WIDTH; y = y % HEIGHT;yT = y;
  
  while(yT < (y + size) && yT < HEIGHT){
    
    // Set xT to x
    
    xT = x;
  
    while(xT < (x + 8) && xT < WIDTH){
    
      // Check if sprite bits overlap with bits in display array
      
      if(display[yT * WIDTH + xT] & (sprite[spriteCounter] >> 7) == 1){
        
        // Set flag because collision is detected
      
        vf = 1;
        
      }
      
      // Xor
      
      display[yT * WIDTH + xT] = display[yT * WIDTH + xT] ^ (sprite[spriteCounter] >> 7);
      
      // Refresh display pixel
      
      display[yT * WIDTH + xT] ? LCD_SetTextColor(PIXEL_COLOR) : LCD_SetTextColor(BACKGROUND_COLOR);
      
      LCD_DrawFullRect((HEIGHT - yT) * PIXEL_SIZE, xT * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
      
      // Shift sprite left
      
      sprite[spriteCounter] = sprite[spriteCounter] << 1;
      
      // Increase x
      
      xT++;
      
    }
    
    // Increase y and spriteCounter
    
    yT++;spriteCounter++;
    
  }
  
  // Return flag
  
  return vf;
  
  
}

void displayListOfGames(void){

  uint8_t nameIndex;
  
  short y;
  
  // Go through the names
  
  for(nameIndex = 0; nameIndex < NUM_OF_GAMES; nameIndex++){
    
    LCD_StringLine(0, y, (uint8_t*)gameNames[nameIndex]);
    
    y+=13;
    
  }
  
}

void clearDisplay(void){

  memset(display, 0, sizeof(display));
  
  LCD_Clear(BACKGROUND_COLOR);
  
}