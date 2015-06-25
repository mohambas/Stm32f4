#include "stdlib.h"
#include "time.h"
#include "Cpu.h"
#include "Memory.h"
#include "Input.h"
#include "Display.h"
#include "Timers.h"

static void zeroI(void);
static void oneI(void);
static void twoI(void);
static void threeI(void);
static void fourI(void);
static void fiveI(void);
static void sixI(void);
static void sevenI(void);
static void eightI(void);
static void nineI(void);
static void aI(void);
static void bI(void);
static void cI(void);
static void dI(void);
static void eI(void);
static void fI(void);

static void put(void);

static void get(void);

// Array of function pointers

static void (*cpuInstructionCat[CPU_NUM_OF_CAT])(void) = {zeroI, oneI, twoI, threeI, fourI, fiveI, sixI, sevenI, eightI, nineI, aI, bI, cI, dI, eI, fI};

// Fonts

uint8_t chip8_fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Registers

static uint8_t V[16]; // General purpose registers

static uint16_t I; // Index register

static uint16_t PC; // Program counter register

static uint16_t opcode; // Current opcode

static uint16_t STACK[CPU_STACK_SIZE]; // Stack

static short SP; // Stack pointer


void InitCpu(const uint8_t* game, int size){

  uint16_t offset;
  
  // Load program into memory
  
  for(offset = 0; offset < size; offset++){
  
    write(PROGRAM_START_ADDR + offset, game[offset]);
    
  }
  
  // Load fonts into memory
  
  for(offset = 0; offset < sizeof(chip8_fontset); offset++){
  
    write(FONTS_START_ADDR + offset, chip8_fontset[offset]);
    
  }
  
  // Set PC to PROGRAM_START_ADDR
  
  PC = PROGRAM_START_ADDR;
  
  // Set stack pointer

  SP = -1;
  
}

void fetch(void){
  
  // Memory cell

  struct memoryCell cell[2];
  
  // Read memory cell

  cell[MSB] = read(PC);
  
  cell[LSB] = read(PC + 1);
  
  if(cell[MSB].status == SUCCESS && cell[LSB].status == SUCCESS){
  
    opcode = (cell[MSB].data << 8) | cell[LSB].data;
    
    PC+=2;
    
  }
  else
    for(;;); // Error
  
}

void execute(void){
  
  cpuInstructionCat[(opcode & 0xF000) >> 12]();
  
}

static void zeroI(void){

  switch(opcode & 0xFFF){
  
    case 0x0E0:
      
      // Clear the screen
      
      clearDisplay();
      
      break;
    
  case 0x0EE:
    
      // Return from a subroutine
    
      get();
    
      break;
    
    default:
      
      // Calls RCA 1802 program at address NNN.
      
      put();
      
      PC = opcode & 0xFFF;
      
  }
  
}

static void oneI(void){
  
  // Jump to address NNN
  
  PC = opcode & 0xFFF;

}

static void twoI(void){
  
  // Calls subroutine at NNN
  
  put();
  
  PC = opcode & 0xFFF;

}

static void threeI(void){
  
  // Skips the next instruction if VX equals NN.
  
  if(V[(opcode & 0xF00) >> 8] == (opcode & 0xFF))
    PC+=2;

}

static void fourI(void){

  // Skips the next instruction if VX doesn't equal NN.
  
  if(V[(opcode & 0xF00) >> 8] != (opcode & 0xFF))
    PC+=2;
  
}

static void fiveI(void){
  
  // Skips the next instruction if VX equals VY.

  if(V[(opcode & 0xF00) >> 8] == V[(opcode & 0xF0) >> 4])
    PC+=2;
  
}

static void sixI(void){
  
  // Sets VX to NN.

  V[(opcode & 0xF00) >> 8] = opcode & 0xFF;
  
}

static void sevenI(void){

  // Adds NN to VX.
  
  V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] + (opcode & 0xFF);
  
}

static void eightI(void){

  switch(opcode & 0xF){
  
    case 0:
      
      // Sets VX to the value of VY
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF0) >> 4];
      
      break;
    
    case 1:
      
      // Sets VX to VX or VY
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] | V[(opcode & 0xF0) >> 4];
      
      break;
      
    case 2:
      
      // Sets VX to VX and VY
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] & V[(opcode & 0xF0) >> 4];
      
      break;
      
    case 3:
      
      // Sets VX to VX xor VY
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] ^ V[(opcode & 0xF0) >> 4];
      
      break;
      
    case 4:
      
      // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't
      
      if(V[(opcode & 0xF00) >> 8] + V[(opcode & 0xF0) >> 4] > 255)
        V[0xF] = 1;
      else
        V[0xF] = 0;
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] + V[(opcode & 0xF0) >> 4];
      
      break;
      
    case 5:
      
      // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't
      
      if(V[(opcode & 0xF00) >> 8] < V[(opcode & 0xF0) >> 4])
        V[0xF] = 0;
      else
        V[0xF] = 1;
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] - V[(opcode & 0xF0) >> 4];
      
      break;
      
    case 6:
      
      // Shifts VY right by one. VF is set to the value of the least significant bit of VY before the shift.
      
      V[0xF] = V[(opcode & 0xF0) >> 4] & 0x1;
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF0) >> 4] >> 1;
      
      break;
      
    case 7:
      
      // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
      
      if(V[(opcode & 0xF0) >> 4] < V[(opcode & 0xF00) >> 8])
        V[0xF] = 0;
      else
        V[0xF] = 1;
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF0) >> 4] - V[(opcode & 0xF00) >> 8];
      
      break;
      
    case 0xE:
      
      // Shifts VY left by one. VF is set to the value of the most significant bit of VX before the shift
      
      V[0xF] = (V[(opcode & 0xF0) >> 4] & 0x80) >> 7;
      
      V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF0) >> 4] << 1;
      
      break;
      
    default:
      
      // Wrong opcode
      
      for(;;);
      
  }
  
}

static void nineI(void){

  // Skips the next instruction if VX doesn't equal VY
  
  if(V[(opcode & 0xF00) >> 8] != V[(opcode & 0xF0) >> 4])
    PC+=2;
  
}

static void aI(void){

  // Sets I to the address NNN
  
  I = opcode & 0xFFF;
  
}

static void bI(void){

 // Jumps to the address NNN plus V0
  
  PC = (opcode & 0xFFF) + V[0];
  
}

static void cI(void){
  
  // Sets VX to a random number, masked by NN
  
  srand (time(NULL));
  
  V[(opcode & 0xF00) >> 8] = (rand() % 256) & (opcode & 0xFF);

}

static void dI(void){
  
  uint8_t offset;
  
  struct memoryCell cell;
  
  uint8_t* sprite = malloc((opcode & 0xF) * sizeof(uint8_t));
  
  for(offset = 0; offset < (opcode & 0xF); offset++){
  
    cell = read(I + offset);
    
    if(cell.status == SUCCESS)
      sprite[offset] = cell.data;
    else{
    
      // Error
      
      for(;;);
      
    }
    
  }
  
  // Draw sprite
  
  V[0xF] = showSprite(V[(opcode & 0xF00) >> 8], V[(opcode & 0xF0) >> 4], sprite, (opcode & 0xF));
  
  // Free memory
  
  free(sprite);

}

static void eI(void){

  switch(opcode & 0xFF){
  
  case 0x9E:
    
    // Skips the next instruction if the key stored in VX is pressed
    
    if(isKeyPressed((Key)V[(opcode & 0xF00) >> 8]))
      PC+=2;
    
    break;
    
  case 0xA1:
    
    // Skips the next instruction if the key stored in VX isn't pressed
    
    if(!isKeyPressed((Key)V[(opcode & 0xF00) >> 8]))
      PC+=2;
    
    break;
    
  default:
    
   // Wrong opcode
    
    for(;;);
    
  }
  
}

static void fI(void){

  switch(opcode & 0xFF){
  
    case 0x07:
      
      // Sets VX to the value of the delay timer
      
      V[(opcode & 0xF00) >> 8] = getDelayTimer();
      
      break;
      
    case 0x0A:
      
      // A key press is awaited, and then stored in VX
      
      V[(opcode & 0xF00) >> 8] = waitUntilKeyPressed();
      
      break;
      
    case 0x15:
      
      // Sets the delay timer to VX
      
      setDelayTimer(V[(opcode & 0xF00) >> 8]);
      
      break;
      
    case 0x18:
    
      // Sets the sound timer to VX
      
      setAudioTimer(V[(opcode & 0xF00) >> 8]);
      
      break;
      
    case 0x1E:
      
      // Adds VX to I
      
      I = I + V[(opcode & 0xF00) >> 8];
      
      break;
      
    case 0x29:
      
      // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font
      
      I = FONTS_START_ADDR + V[(opcode & 0xF00) >> 8] * 5;
      
      break;
      
    case 0x33:
      
      // Stores the Binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. 
      // (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
      
      write(I, V[(opcode & 0xF00) >> 8] / 100);
      
      write(I + 1, (V[(opcode & 0xF00) >> 8] / 10) % 10);
      
      write(I + 2, V[(opcode & 0xF00) >> 8] % 10);
      
      break;
      
    case 0x55:
      
      // Stores V0 to VX in memory starting at address I
      
      {
        
        uint8_t offset;
      
        for(offset = 0; offset <= ((opcode & 0xF00) >> 8); offset++){
      
          write(I + offset, V[offset]);
        
        }
        
      }
      
      break;
      
    case 0x65:
    
      // Fills V0 to VX with values from memory starting at address I
      
      {
      
        uint8_t offset;
        
        struct memoryCell cell;
        
        for(offset = 0; offset <= ((opcode & 0xF00) >> 8); offset++){
      
          cell = read(I + offset);
          
          if(cell.status == SUCCESS)
            V[offset] = cell.data;
          else{
          
            // Error
            
            for(;;);
            
          }
        
        }
        
      }
      
      break;
      
  default:
    
    // Wrong opcode
    
    for(;;);
    
    break;
      
  }
  
}

static void put(void){

  if(SP > CPU_STACK_SIZE){
  
    // Stack overflow
    
    for(;;);
    
  }
  
  SP++;
  
  STACK[SP] = PC;
  
}

static void get(void){

  if(SP < 0){
  
    // Error... Out of range
    
    for(;;);
    
  }
  
  PC = STACK[SP];
  
  SP--;
  
}