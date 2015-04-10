#include "gallery.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage home_p;
static gdispImage next_p;
static gdispImage prev_p;

// GHandles

static GHandle mainContainer_c;
static GHandle pic_box;
GHandle home_gallery_b;
GHandle nextImage_gallery_b;
GHandle previousImage_gallery_b;



void createGalleryScreen(char* theme){

  // GFILE pointers and vars

  GFILE* background_f;
  GFILE* home_f;
  GFILE* next_f;
  GFILE* prev_f;

  GWidgetInit wi;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/homeButton.gif", sizeof(path));

  home_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/nextButton.gif", sizeof(path));

  next_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/prevButton.gif", sizeof(path));

  prev_f = gfileOpen(path, "rb");

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

  // Create imagebox pic_box

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 10;
  wi.g.width = 220;
  wi.g.height = 200;
  wi.g.parent = mainContainer_c;
  wi.text = "ImageBox";

  pic_box = gwinImageCreate(NULL, &wi.g);

  // Create home_b

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

  home_gallery_b = gwinButtonCreate(0, &wi);

  // Create next_b button

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(next_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Next";

  }
  else{

    gdispImageOpenGFile(&next_p, next_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &next_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  nextImage_gallery_b = gwinButtonCreate(0, &wi);

  // Create prev_b button

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(prev_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Prev";

  }
  else{

    gdispImageOpenGFile(&prev_p, prev_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &prev_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  previousImage_gallery_b = gwinButtonCreate(0, &wi);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyGalleryScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&home_p);

  gdispImageClose(&next_p);

  gdispImageClose(&prev_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  home_gallery_b = NULL;

  nextImage_gallery_b = NULL;

  previousImage_gallery_b = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void setPicture(char* picture){

  GFILE* pic_f;

  char path[100];

  memset(path, 0, sizeof(path)); strcat(path, "/gallery/"); strlcat(path, picture, sizeof(path));

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open new picture

  pic_f = gfileOpen(path, "rb");

  gwinImageOpenGFile(pic_box, pic_f);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
