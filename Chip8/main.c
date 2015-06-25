#include "stm32f4xx.h"
#include "Cpu.h"
#include "Timers.h"
#include "Games.h"
#include "Input.h"
#include "Memory.h"
#include "Display.h"
#include "delay.h"

void main(void)
{
  
  Key one, two;
  
  struct Game game;
  
  // Init systick timer
  
  SysTick_Init();
  
  // Init Memory, Input, Display
  
  initMemory();
  
  initInput();
  
  initDisplay();
  
  // Display all games
  
  displayListOfGames();
  
  // Select game
  
  while((one =  waitUntilKeyPressed()) > 9);
  
  while((two = waitUntilKeyPressed()) > 9);
  
  // Clear screen
  
  clearDisplay();
  
  // Get selected game
  
  game = getGame(one * 10 + two);
  
  // Init cpu
  
  InitCpu(game.game, game.size);
  
  while(1)
  {
 
    fetch();
 
    execute();
    
    decreaseTimers();
    
    Delay(4000);
    
  }
}