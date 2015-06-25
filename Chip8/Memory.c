#include "Memory.h"

// Memory array

static uint8_t memory[MEMORY_SIZE];

void initMemory(void){

  // Set memory to 0
  
  memset(memory, 0, sizeof(memory));
  
}

struct memoryCell read(uint16_t address){

  struct memoryCell cell;
  
  if(address < MEMORY_SIZE){
    
    cell.data = memory[address];
    
    cell.status = SUCCESS;
    
  }
  else
    cell.status = ERROR;
  
  return cell;
  
}

struct memoryCell write(uint16_t address, uint8_t data){

  struct memoryCell cell;
  
  if(address < MEMORY_SIZE){
    
    memory[address] = data;
    
    cell.status = SUCCESS;
    
  }
  else
    cell.status = ERROR;
  
  return cell;
  
}

