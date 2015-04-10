#include "callScreen.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage accept_p;
static gdispImage decline_p;

// GHandles

static GHandle mainContainer_c;
static GHandle caller_l;
GHandle accept_call_b;
GHandle decline_call_b;



void createCallScreen(char* theme, char* caller, bool_t showAcceptButton){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* accept_f;
  GFILE* decline_f;

  GWidgetInit wi;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/accept.gif", sizeof(path));

  accept_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/decline.gif", sizeof(path));

  decline_f = gfileOpen(path, "rb");

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

  // Create caller_l

  wi.g.show = TRUE;
  wi.g.y = 10;
  wi.g.x = 10;
  wi.g.width = 220;
  wi.g.height = 20;
  wi.g.parent = mainContainer_c;
  wi.text = "";
  wi.customDraw = 0;
  wi.customParam = 0;
  wi.customStyle = 0;

  caller_l = gwinLabelCreate(NULL, &wi);

  // Create accept button

  if(showAcceptButton){

    // Show button

    wi.g.show = TRUE;

  }
  else{

    // Hide button

    wi.g.show = FALSE;

  }

  wi.g.x = 10;
  wi.g.y = 228;
  wi.g.width = 100;
  wi.g.height = 72;
  wi.g.parent = mainContainer_c;

  if(accept_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Accept";

  }   else{

    gdispImageOpenGFile(&accept_p, accept_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &accept_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  accept_call_b = gwinButtonCreate(0, &wi);

  // Create decline button

  wi.g.show = TRUE;
  wi.g.x = 130;
  wi.g.y = 228;
  wi.g.width = 100;
  wi.g.height = 72;

  if(decline_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "Decline";

  }
  else{

    gdispImageOpenGFile(&decline_p, decline_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &decline_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  decline_call_b = gwinButtonCreate(0, &wi);

  // Set a contact name to a label

  gwinSetText(caller_l, caller, TRUE);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyCallScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&accept_p);

  gdispImageClose(&decline_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  accept_call_b = NULL;

  decline_call_b = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
