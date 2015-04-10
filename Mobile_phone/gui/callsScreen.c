#include "callsScreen.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage home_p;
static gdispImage clear_p;
static gdispImage call_p;
static gdispImage back_p;

// GHandles

static GHandle mainContainer_c;
GHandle open_calls_b;
GHandle home_calls_b;
GHandle clear_calls_b;
GHandle calls_calls_l;
GHandle call_calls_b;
GHandle back_calls_b;
GHandle telNum_calls_lb;
GHandle c_0_b;
GHandle c_1_b;
GHandle c_2_b;
GHandle c_3_b;
GHandle c_4_b;
GHandle c_5_b;
GHandle c_6_b;
GHandle c_7_b;
GHandle c_8_b;
GHandle c_9_b;

void createCallsScreen(char* theme, struct Call** missedCalls, int numOfCalls){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* home_f;
  GFILE* clear_f;
  GFILE* call_f;

  GWidgetInit wi;

  int i;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/homeButton.gif", sizeof(path));

  home_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/call.gif", sizeof(path));

  call_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/delButton.gif", sizeof(path));

  clear_f = gfileOpen(path, "rb");

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

  mainContainer_c = gwinContainerCreate(0, &wi, 0);

  // Create list of missed calls

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 10;
  wi.g.width = 220;
  wi.g.height = 200;;
  wi.g.parent = mainContainer_c;
  wi.text = "Missed calls";
  wi.customDraw = 0;
  wi.customParam = 0;
  wi.customStyle = 0;

  calls_calls_l = gwinListCreate(NULL, &wi, FALSE);

  // Set smooth scrool

  gwinListSetScroll(calls_calls_l, scrollSmooth);

  // Create callsHome_b

  wi.g.show = TRUE;
  wi.g.x = 90;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(home_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Home";

  }
  else{

    gdispImageOpenGFile(&home_p, home_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &home_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  home_calls_b = gwinButtonCreate(0, &wi);

  // Create openCall_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(call_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "OpenC";

  }
  else{

    gdispImageOpenGFile(&call_p, call_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &call_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  open_calls_b = gwinButtonCreate(0, &wi);

  // Create clearCalls_b

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(clear_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Clear";

  }
  else{

    gdispImageOpenGFile(&clear_p, clear_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &clear_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  clear_calls_b = gwinButtonCreate(0, &wi);

  // Add items into a list

  for(i = 0; i < numOfCalls; i++){

    gwinListAddItem(calls_calls_l, missedCalls[i]->callerTelNum, TRUE);

  }

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyCallsScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&home_p);

  gdispImageClose(&call_p);

  gdispImageClose(&clear_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  open_calls_b = NULL;

  home_calls_b = NULL;

  clear_calls_b = NULL;

  calls_calls_l = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void createMakeCallScreen(char* theme){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* back_f;
  GFILE* call_f;

  GWidgetInit wi;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/back.gif", sizeof(path));

  back_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/call.gif", sizeof(path));

  call_f = gfileOpen(path, "rb");

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

  mainContainer_c = gwinContainerCreate(0, &wi, 0);

  // Create makeCall_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(call_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Call";

    }
  else{

    gdispImageOpenGFile(&call_p, call_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &call_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  call_calls_b = gwinButtonCreate(0, &wi);

  // Create backMakeCall_b

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(back_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Back";

  }
  else{

    gdispImageOpenGFile(&back_p, back_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &back_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  back_calls_b = gwinButtonCreate(0, &wi);

  // Create name label

  wi.g.y = 10;
  wi.g.x = 10;
  wi.g.width = 220;
  wi.g.height = 20;
  wi.text = "";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  telNum_calls_lb = gwinButtonCreate(NULL, &wi);

  // Create c_1_b

  wi.g.y = 50;
  wi.g.x = 10;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "1";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_1_b = gwinButtonCreate(NULL, &wi);

  // Create c_2_b

  wi.g.y = 50;
  wi.g.x = 85;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "2";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_2_b = gwinButtonCreate(NULL, &wi);

  // Create c_3_b

  wi.g.y = 50;
  wi.g.x = 160;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "3";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_3_b = gwinButtonCreate(NULL, &wi);

  // Create c_4_b

  wi.g.y = 95;
  wi.g.x = 10;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "4";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_4_b = gwinButtonCreate(NULL, &wi);

  // Create c_5_b

  wi.g.y = 95;
  wi.g.x = 85;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "5";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_5_b = gwinButtonCreate(NULL, &wi);

  // Create c_6_b

  wi.g.y = 95;
  wi.g.x = 160;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "6";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_6_b = gwinButtonCreate(NULL, &wi);

  // Create c_7_b

  wi.g.y = 140;
  wi.g.x = 10;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "7";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_7_b = gwinButtonCreate(NULL, &wi);

  // Create c_8_b

  wi.g.y = 140;
  wi.g.x = 85;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "8";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_8_b = gwinButtonCreate(NULL, &wi);

  // Create c_9_b

  wi.g.y = 140;
  wi.g.x = 160;
  wi.g.width = 70;
  wi.g.height = 40;
  wi.text = "9";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_9_b = gwinButtonCreate(NULL, &wi);

  // Create c_0_b

  wi.g.y = 185;
  wi.g.x = 10;
  wi.g.width = 220;
  wi.g.height = 40;
  wi.text = "0";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  c_0_b = gwinButtonCreate(NULL, &wi);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyMakeCallScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&back_p);

  gdispImageClose(&call_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  call_calls_b = NULL;

  back_calls_b = NULL;

  telNum_calls_lb = NULL;

  c_0_b = NULL;

  c_1_b = NULL;

  c_2_b = NULL;

  c_3_b = NULL;

  c_4_b = NULL;

  c_5_b = NULL;

  c_6_b = NULL;

  c_7_b = NULL;

  c_8_b = NULL;

  c_9_b = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
