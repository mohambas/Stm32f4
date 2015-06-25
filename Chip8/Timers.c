#include "Timers.h"

static uint8_t DELAY_TIMER, AUDIO_TIMER;

void decreaseTimers(void){

  if(DELAY_TIMER != 0)
    DELAY_TIMER--;
  
  if(AUDIO_TIMER != 0){
    
    AUDIO_TIMER--;
    
    // Play sound
    
  }
  
}

void setDelayTimer(uint8_t timer){

  DELAY_TIMER = timer;
  
}

uint8_t getDelayTimer(void){
  
  return DELAY_TIMER;
  
}


void setAudioTimer(uint8_t timer){

  AUDIO_TIMER = timer;
  
}
