#include "stm32f4xx.h"
#include "vars.h"


// commands

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode

#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control

#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
#define LCD_BACKLIGHT 0x08 // used to pick out the backlight flag since _displaycontrol will never set itself

typedef enum {PLAYER, VOLUME, TREBLE, BASS} MODE;

// flags for display/cursor shift

#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set

#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define EN 0x04
#define RS 0x01

#define ADDR 0x4e

void initLcdPeriph(void);
void initLcd(void);
void stopI2C(void);
void startI2C(void);
void sendCmd(uint8_t cmd);
void sendChar(uint8_t chr);
void sendCharOn(uint8_t chr, uint8_t row, uint8_t col);
void setCursor(uint8_t row, uint8_t col);
void delay(uint32_t ms);
void writeMode(MODE mode);
void writeVolIncr(void);
void writeVolDcrs(void);
void writeTrebIncr(void);
void writeTrebDcrs(void);
void writeBassIncr(void);
void writeBassDcrs(void);
void writePause(void);
void writePlay(void);
void writeStop(void);
void writeNext(void);
void writePrev(void);