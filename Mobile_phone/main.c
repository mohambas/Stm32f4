#include "calls_t.h"
#include "messages_t.h"
#include "audioPlayer_t.h"
#include "gallery_t.h"
#include "contacts_t.h"
#include "configuration_t.h"
#include "gui_t.h"
#include "uart_t.h"
#include "gfx.h"
#include "sdcard.h"
#include "gsm.h"

static WORKING_AREA(callsThread, 512);

static WORKING_AREA(messagesThread, 256);

static WORKING_AREA(audioPlayerThread, 256);

static WORKING_AREA(galleryThread, 256);

static WORKING_AREA(contactsThread, 512);

static WORKING_AREA(configurationThread, 512);

static WORKING_AREA(guiThread, 2048);

static WORKING_AREA(uartThread, 6000);

Thread *calls_thrd_h, *messages_thrd_h, *audioPlayer_thrd_h , *gallery_thrd_h, *configuration_thrd_h, *gui_thrd_h, *contacts_thrd_h, *uart_thrd_h;

Mailbox gsmResponse;

int main(){

  msg_t gsmResponseBuff[5];

  // Initialize the display

  gfxInit();

  // Init sdio

  initSdio();

  // Init uart4

  initGsm();

  // Set default fonts and style

  gwinSetDefaultFont(gdispOpenFont("UI2"));

  gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);

  gdispClear(White);

  // Init heap

  _heap_init();

  // Create mailbox

  chMBInit(&gsmResponse, gsmResponseBuff, 5);

  // Create threads

  calls_thrd_h = chThdCreateStatic(callsThread, sizeof(callsThread), NORMALPRIO, calls_t, NULL);

  messages_thrd_h = chThdCreateStatic(messagesThread, sizeof(messagesThread), NORMALPRIO, messages_t, NULL);

  audioPlayer_thrd_h = chThdCreateStatic(audioPlayerThread, sizeof(audioPlayerThread), NORMALPRIO, audioPlayer_t, NULL);

  gallery_thrd_h = chThdCreateStatic(galleryThread, sizeof(galleryThread), NORMALPRIO, gallery_t, NULL);

  contacts_thrd_h = chThdCreateStatic(contactsThread, sizeof(contactsThread), NORMALPRIO, contacts_t, NULL);

  configuration_thrd_h = chThdCreateStatic(configurationThread, sizeof(configurationThread), NORMALPRIO, configuration_t, NULL);

  gui_thrd_h = chThdCreateStatic(guiThread, sizeof(guiThread), NORMALPRIO, gui_t, NULL);

  uart_thrd_h = chThdCreateStatic(uartThread, sizeof(uartThread), NORMALPRIO, uart_t, NULL);

  while(1){


  }

  return 1;

}
