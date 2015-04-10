#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "gfx.h"
#include "string.h"

#define SETTINGS_SCREEN_ID 8

void createSettingsScreen(char*, char**, int, unsigned char);
void destroySettingsScreen(void);

#endif
