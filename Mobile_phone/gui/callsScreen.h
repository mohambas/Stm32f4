#ifndef CALLS_SCREEN_H
#define CALLS_SCREEN_H

#include "gfx.h"
#include "string.h"
#include "structures.h"

#define CALLS_SCREEN_ID 2
#define MAKE_CALL_SCREEN_ID 3

void createCallsScreen(char*, struct Call**, int);
void destroyCallsScreen(void);
void createMakeCallScreen(char*);
void destroyMakeCallScreen(void);

#endif
