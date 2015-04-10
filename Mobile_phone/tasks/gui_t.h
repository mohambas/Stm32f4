#ifndef GUI_T_H
#define GUI_T_H

#include "structures.h"
#include "gfx.h"
#include "mainScreen.h"
#include "callScreen.h"
#include "settingsScreen.h"
#include "gallery.h"
#include "contactsScreen.h"
#include "callsScreen.h"
#include "gsm.h"

// Extern

extern Thread *configuration_thrd_h;
extern Thread *gallery_thrd_h;
extern Thread *contacts_thrd_h;
extern Thread *calls_thrd_h;

extern Mailbox gsmResponse;

extern GHandle calls_mainScreen_b;
extern GHandle home_calls_b;
extern GHandle clear_calls_b;
extern GHandle calls_calls_l;
extern GHandle open_calls_b;
extern GHandle call_calls_b;
extern GHandle back_calls_b;
extern GHandle c_0_b;
extern GHandle c_1_b;
extern GHandle c_2_b;
extern GHandle c_3_b;
extern GHandle c_4_b;
extern GHandle c_5_b;
extern GHandle c_6_b;
extern GHandle c_7_b;
extern GHandle c_8_b;
extern GHandle c_9_b;
extern GHandle telNum_calls_lb;
extern GHandle decline_call_b;
extern GHandle accept_call_b;
extern GHandle messages_mainScreen_b;
extern GHandle audioPlayer_mainScreen_b;
extern GHandle gallery_mainScreen_b;
extern GHandle settings_mainScreen_b;
extern GHandle contacts_mainScreen_b;
extern GHandle home_contacts_b;
extern GHandle delete_contacts_b;
extern GHandle add_contacts_b;
extern GHandle save_contacts_b;
extern GHandle back_contacts_b;
extern GHandle contacts_contacts_l;
extern GHandle name_contacts_lb;
extern GHandle telNum_contacts_lb;
extern GHandle notifications_mainScreen_b;
extern GHandle home_settings_b;
extern GHandle home_gallery_b;
extern GHandle brightness_settings_s;
extern GHandle themes_settings_l;
extern GHandle nextImage_gallery_b;
extern GHandle previousImage_gallery_b;

threadreturn_t gui_t(void*);

#endif
