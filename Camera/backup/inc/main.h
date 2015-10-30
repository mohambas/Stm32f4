#ifndef MAIN_H
#define MAIN_H

#include "stdlib.h"
#include "uart.h"
#include "camera.h"
#include "picojpeg.h"

#define MAX_PIC_BUFF_LEN 256
#define MAX_MODE_LEN 15
#define PICTURE_WIDTH 320
#define PICTURE_HEIGHT 240

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void main();

#endif
