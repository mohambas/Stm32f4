#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include "gfx.h"
#include "string.h"

#define MAIN_SCREEN_ID 7

void createMainScreen(char*);
void destroyMainScreen(void);
void showSmsNotification(void);
void showCallNotification(void);
void showSmsCallNotification(void);
void hideNotifications(void);


#endif
