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

  uint8_t lineIndex;
  uint8_t pixleIndex;
  uint8_t vf = 0;
  
  // Iterate through sprite lines
  
  for(lineIndex = 0; lineIndex < size; lineIndex++){
    
    // Iterate through pixels of a sprite line
    
    for(pixleIndex = 0; pixleIndex < 8; pixleIndex++){
      
      // Check if the current sprite line pixel is set to one
      
      if(sprite[lineIndex] >> 7 == 1){
      
        // If the pixel needs to be changed, set vf flag
        
        if(display[(y + lineIndex) * WIDTH + x + pixleIndex] & (sprite[lineIndex] >> 7) == 1)
          vf = 1;
        display[(y + lineIndex) * WIDTH + x + pixleIndex] ^= sprite[lineIndex] >> 7;
        
        // Display or clear pixels
     
        display[(y + lineIndex) * WIDTH + x + pixleIndex] == 1 ? LCD_SetTextColor(PIXEL_COLOR) : LCD_SetTextColor(BACKGROUND_COLOR);
        
        if((y + lineIndex) * WIDTH + x + pixleIndex <= WIDTH * HEIGHT)
          LCD_DrawFullRect((HEIGHT - (y + lineIndex)) * PIXEL_SIZE, (x + pixleIndex) * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE);
        
      }
        
    sprite[lineIndex] = sprite[lineIndex] << 1;
            
    }
    
  }
  
  return vf;
   
}

void displayListOfGames(void){

  uint8_t nameIndex;
  
  short y;
  
  // Go through the list of game names
  
  for(nameIndex = 0; nameIndex < NUM_OF_GAMES; nameIndex++){
    
    LCD_StringLine(0, y, (uint8_t*)gameNames[nameIndex]);
    
    y+=13;
    
  }
  
}

void clearDisplay(void){

  memset(display, 0, sizeof(display));
  
  LCD_Clear(BACKGROUND_COLOR);
  
}
