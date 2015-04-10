#ifndef GSM_T_H
#define GSM_T_H

#include "gfx.h"
#include "string.h"

void initGsm(void);
void callNumber(const char*);
void hangUp(void);
void answerCall(void);
void deleteSms(char*);
void getSms(char*);
int sdReadLine(char*, int, bool_t);

#endif

