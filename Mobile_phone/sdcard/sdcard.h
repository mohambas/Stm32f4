#ifndef SDCARD_H
#define SDCARD_H

#include "structures.h"
#include "gfx.h"
#include "string.h"
#include "stdlib.h"

void initSdio(void);
void readConfiguration(struct Configuration*);
void writeConfiguration(struct Configuration*);
void getThemes(struct Configuration*);
void getPictures(struct Gallery*);
void getContacts(struct Contacts*);
void addContact(struct Contacts*);
bool_t getContact(struct Contacts*);
void deleteContact(struct Contacts*);
void getCalls(struct Calls*);
void addCall(char*);
void deleteCalls(struct Calls*);

#endif
