#include "settingsScreen.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage home_p;

// GHandles

static GHandle mainContainer_c;
GHandle home_settings_b;
GHandle brightness_settings_s;
GHandle themes_settings_l;

void createSettingsScreen(char* theme, char** themes, int numOfThemes, unsigned char brightness){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* home_f;

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

  // create mainContainer_c

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

  // Create slider

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 25;
  wi.g.width = 220;
  wi.g.height = 20;
  wi.g.parent = mainContainer_c;
  wi.text = "Brightness";
  wi.customDraw = gwinSliderDraw_Std;
  wi.customParam = 0;
  wi.customStyle = &WhiteWidgetStyle;
  brightness_settings_s = gwinSliderCreate(0, &wi);
  gwinSliderSetRange(brightness_settings_s, 20, 100);

  // Create list of available themes

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 60;
  wi.g.width = 220;
  wi.g.height = 150;
  wi.g.parent = mainContainer_c;
  wi.text = "Themes";
  wi.customDraw = 0;
  wi.customParam = 0;
  wi.customStyle = 0;

  themes_settings_l = gwinListCreate(NULL, &wi, FALSE);

  // Set smooth scrool

  gwinListSetScroll(themes_settings_l, scrollSmooth);

  // Set brightness from configuration struct

  gwinSliderSetPosition(brightness_settings_s, brightness);

  // Create settingsHome_b

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

  home_settings_b = gwinButtonCreate(0, &wi);

  // Add items into a list

  for(i = 0; i < numOfThemes; i++){

    gwinListAddItem(themes_settings_l, themes[i], TRUE);

  }

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroySettingsScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&home_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  home_settings_b = NULL;

  brightness_settings_s = NULL;

  themes_settings_l = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
