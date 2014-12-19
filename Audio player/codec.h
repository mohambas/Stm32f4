#include "stm32f4xx.h"
#include "vars.h"

#define CODEC_ADDR 0x94

#define PAUSESONG 1

// Sending commands and data functions

void sendCmdDac(uint8_t reg, uint8_t data);
uint8_t readRegDac(uint8_t reg);
void sendAudioData(int16_t sample);

// Init functions

void initCodecPeriph();
void initCodec(void);
void delayCodec(uint32_t ms);

// Volume functions

void increaseVolume(void);
void decreaseVolume(void);

// Sount control functions

void increaseTreble(void);
void decreaseTreble(void);
void increaseBass(void);
void decreaseBass(void);

// Play, pause and stop functions

void play(const uint16_t * buffer, uint32_t bufferSize);
void pauseResume(uint8_t pause);
void stop(void);
void resetCodec(void);


