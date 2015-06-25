#ifndef DISPLAY_H
#define DISPLAY_H

#include "ssd1289.h"
#include "Games.h"

extern char* gameNames[NUM_OF_GAMES];

// Defines

#define BACKGROUND_COLOR LCD_COLOR_WHITE
//#define BACKGROUND_COLOR LCD_COLOR_BLUE
//#define BACKGROUND_COLOR LCD_COLOR_RED
#define PIXEL_COLOR LCD_COLOR_BLACK
//#define PIXEL_COLOR LCD_COLOR_GREEN
#define PIXEL_SIZE 5
#define WIDTH 64
#define HEIGHT 32
#define SPRITE_SIZE 8

void initDisplay(void);

uint8_t showSprite(uint8_t, uint8_t, uint8_t*, uint8_t);

void displayListOfGames(void);

void clearDisplay(void);

#endif