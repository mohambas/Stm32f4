#include "mainScreen.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage calls_p;
static gdispImage messages_p;
static gdispImage audioPlayer_p;
static gdispImage gallery_p;
static gdispImage contacts_p;
static gdispImage settings_p;
static gdispImage notification_b_p;
static gdispImage notificationSms_p;
static gdispImage notificationCall_p;
static gdispImage notificationCallSms_p;

// GHandles

static GHandle mainContainer_c;
static GHandle notificationSmsCall_c;
static GHandle notificationSms_c;
static GHandle notificationCall_c;
static GHandle notificationBothCallSms_c;
GHandle calls_mainScreen_b;
GHandle messages_mainScreen_b;
GHandle audioPlayer_mainScreen_b;
GHandle gallery_mainScreen_b;
GHandle settings_mainScreen_b;
GHandle contacts_mainScreen_b;
GHandle notifications_mainScreen_b;


void createMainScreen(char* theme){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* calls_f;
  GFILE* messages_f;
  GFILE* audioPlayer_f;
  GFILE* gallery_f;
  GFILE* contacts_f;
  GFILE* settings_f;
  GFILE* notification_b_f;
  GFILE* notificationSms_f;
  GFILE* notificationCall_f;
  GFILE* notificationCallSms_f;

  GWidgetInit wi;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/calls.gif", sizeof(path));

  calls_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/messages.gif", sizeof(path));

  messages_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/audioPlayer.gif", sizeof(path));

  audioPlayer_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/gallery.gif", sizeof(path));

  gallery_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/settings.gif", sizeof(path));

  settings_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/contacts.gif", sizeof(path));

  contacts_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/notificationButton.gif", sizeof(path));

  notification_b_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/smsNotif.gif", sizeof(path));

  notificationSms_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/callNotif.gif", sizeof(path));

  notificationCall_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/callSmsNotif.gif", sizeof(path));

  notificationCallSms_f = gfileOpen(path, "rb");

  // Create mainContainer_c

  wi.g.show = TRUE;
  wi.g.x = 0;
  wi.g.y = 0;
  wi.g.width = 240;
  wi.g.height = 320;
  wi.g.parent = 0;
  wi.text = "Container";

  if(background_f == NULL){

    wi.customDraw = gwinContainerDraw_Std;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;

  }
  else{

    gdispImageOpenGFile(&background_p, background_f);
    wi.customDraw = gwinContainerDraw_Image;
    wi.customParam = &background_p;
    wi.customStyle = 0;

  }

  mainContainer_c = gwinContainerCreate(NULL, &wi, 0);

  // Create calls_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 150;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(calls_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Calls";

  }
  else{

    gdispImageOpenGFile(&calls_p, calls_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &calls_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  calls_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create message_b

  wi.g.show = TRUE;
  wi.g.x = 90;
  wi.g.y = 150;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(messages_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Sms";

  }
  else{

    gdispImageOpenGFile(&messages_p, messages_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &messages_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  messages_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create audioPlayer_b

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 150;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(audioPlayer_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Player";

  }
  else{

    gdispImageOpenGFile(&audioPlayer_p, audioPlayer_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &audioPlayer_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  audioPlayer_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create gallery_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(gallery_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Gallery";

  }
  else{

    gdispImageOpenGFile(&gallery_p, gallery_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &gallery_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  gallery_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create contacts_b

  wi.g.show = TRUE;
  wi.g.x = 90;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(contacts_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Contacts";

  }
  else{

    gdispImageOpenGFile(&contacts_p, contacts_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &contacts_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  contacts_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create settings_b

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(settings_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Settings";

  }
  else{

    gdispImageOpenGFile(&settings_p, settings_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &settings_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  settings_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create container for sms and call notification containers

  wi.g.show = FALSE;
  wi.g.x = 10;
  wi.g.y = 10;
  wi.g.width = 232;
  wi.g.height = 42;
  wi.g.parent = mainContainer_c;
  wi.text = "Sms and call notification mainContainer_c";
  wi.customDraw = gwinContainerDraw_Transparent;
  wi.customParam = 0;
  wi.customStyle = &WhiteWidgetStyle;

  notificationSmsCall_c = gwinContainerCreate(NULL, &wi, 0);

  // Create notification button

  wi.g.show = FALSE;
  wi.g.x = 0;
  wi.g.y = 0;
  wi.g.width = 54;
  wi.g.height = 42;
  wi.g.parent = notificationSmsCall_c;

  if(notification_b_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "N button";

  }
  else{

    gdispImageOpenGFile(&notification_b_p, notification_b_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &notification_b_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  notifications_mainScreen_b = gwinButtonCreate(NULL, &wi);

  // Create sms notification container

  wi.g.show = FALSE;
  wi.g.x = 54;
  wi.g.y = 0;
  wi.g.width = 166;
  wi.g.height = 42;
  wi.g.parent = notificationSmsCall_c;
  wi.text = "Sms notification container";

  if(notificationSms_f == NULL){

    wi.customDraw = gwinContainerDraw_Std;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;

  }
  else{

    gdispImageOpenGFile(&notificationSms_p, notificationSms_f);
    wi.customDraw = gwinContainerDraw_Image;
    wi.customParam = &notificationSms_p;
    wi.customStyle = 0;

  }

  notificationSms_c = gwinContainerCreate(NULL, &wi, 0);

  // Create call notification container

  wi.g.show = FALSE;
  wi.g.x = 54;
  wi.g.y = 0;
  wi.g.width = 166;
  wi.g.height = 42;
  wi.g.parent = notificationSmsCall_c;
  wi.text = "Call notification container";

  if(notificationCall_f == NULL){

    wi.customDraw = gwinContainerDraw_Std;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;

  }
  else{

    gdispImageOpenGFile(&notificationCall_p, notificationCall_f);
    wi.customDraw = gwinContainerDraw_Image;
    wi.customParam = &notificationCall_p;
    wi.customStyle = 0;

  }

  notificationCall_c = gwinContainerCreate(NULL, &wi, 0);

  // Create call and sms notification container

  wi.g.show = FALSE;
  wi.g.x = 54;
  wi.g.y = 0;
  wi.g.width = 166;
  wi.g.height = 42;
  wi.g.parent = notificationSmsCall_c;
  wi.text = "Call and sms notification container";

  if(notificationCallSms_f == NULL){

    wi.customDraw = gwinContainerDraw_Std;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;

  }
  else{

    gdispImageOpenGFile(&notificationCallSms_p, notificationCallSms_f);
    wi.customDraw = gwinContainerDraw_Image;
    wi.customParam = &notificationCallSms_p;
    wi.customStyle = 0;

  }

  notificationBothCallSms_c = gwinContainerCreate(NULL, &wi, 0);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}


void destroyMainScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&calls_p);

  gdispImageClose(&messages_p);

  gdispImageClose(&audioPlayer_p);

  gdispImageClose(&gallery_p);

  gdispImageClose(&settings_p);

  gdispImageClose(&contacts_p);

  gdispImageClose(&notification_b_p);

  gdispImageClose(&notificationSms_p);

  gdispImageClose(&notificationCall_p);

  gdispImageClose(&notificationCallSms_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  calls_mainScreen_b = NULL;

  messages_mainScreen_b = NULL;

  audioPlayer_mainScreen_b = NULL;

  gallery_mainScreen_b = NULL;

  settings_mainScreen_b = NULL;

  contacts_mainScreen_b = NULL;

  notifications_mainScreen_b = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}



void showSmsNotification(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  gwinShow(notificationSmsCall_c);

  gwinShow(notifications_mainScreen_b);

  gwinShow(notificationSms_c);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}


void showCallNotification(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  gwinShow(notificationSmsCall_c);

  gwinShow(notifications_mainScreen_b);

  gwinShow(notificationCall_c);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}


void showSmsCallNotification(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  gwinShow(notificationSmsCall_c);

  gwinShow(notifications_mainScreen_b);

  gwinShow(notificationBothCallSms_c);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}


void hideNotifications(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  gwinHide(notifications_mainScreen_b);

  gwinHide(notificationCall_c);

  gwinHide(notificationSms_c);

  gwinHide(notificationBothCallSms_c);

  gwinHide(notificationSmsCall_c);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
