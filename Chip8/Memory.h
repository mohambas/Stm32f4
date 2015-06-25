#ifndef MEMORY_H
#define MEMORY_H

#include "stm32f4xx.h"
#include <string.h>

#define MEMORY_SIZE 4096
#define SUCCESS 1
#define ERROR 0
#define CELL_SIZE 2
#define MSB 0
#define LSB 1

struct memoryCell{

  uint8_t data;
  
  char status;
  
};

void initMemory(void);

struct memoryCell write(uint16_t address, uint8_t value);

struct memoryCell read(uint16_t address);

#endif