#include "stm32f4_discovery.h"
#include "crystalLcd.h"
#include "codec.h"
#include "keypad.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_fatfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include <ctype.h>
#include "math.h"
#include "ff.h"

#define NUMOFSAMPLES 5000

typedef enum {PLAY, PAUSE, STOP} PLAYERMODE;


void playAudio(uint8_t start);
void getNumSongs(void);
void getSongs(void);
void setPreviousSong(void);
void setNextSong(void);
short getNextChunk(void);
void openFile(void);

