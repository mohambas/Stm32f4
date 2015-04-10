#ifndef UART_T_H
#define UART_T_H

#include "structures.h"
#include "gfx.h"
#include "string.h"
#include "gsm.h"
#include "slre.h"

extern Thread *calls_thrd_h;

extern Mailbox gsmResponse;

threadreturn_t uart_t(void*);

#endif
