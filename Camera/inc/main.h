#ifndef MAIN_H
#define MAIN_H

#include "stdlib.h"
#include "uart.h"
#include "camera.h"
#include "picojpeg.h"

#define MAX_MODE_LEN 15
#define PICTURE_WIDTH 320
#define PICTURE_HEIGHT 240

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void main();
unsigned char readPictureBytes(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data);

#endif
