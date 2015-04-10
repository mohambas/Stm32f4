#ifndef CALLS_T_H
#define CALLS_T_H

#include "structures.h"
#include "gfx.h"
#include "sdcard.h"

// Externs

extern Thread* contacts_thrd_h;

extern Mailbox addGetMissedCalls;
extern Mailbox sendMissedCalls;

threadreturn_t calls_t(void*);

#endif
