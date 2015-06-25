#ifndef GAMES_H
#define GAMES_H

#include "stm32f4xx.h"

struct Game{
	
	const uint8_t* game;
	
	int size;
	
};

#define NUM_OF_GAMES 23

struct Game getGame(uint8_t);

#endif