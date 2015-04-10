#include "contactsScreen.h"

// gdispImage structures

static gdispImage background_p;
static gdispImage home_p;
static gdispImage add_p;
static gdispImage del_p;
static gdispImage back_p;

// GHandles

static GHandle mainContainer_c;
GHandle contacts_contacts_l;
GHandle home_contacts_b;
GHandle add_contacts_b;
GHandle save_contacts_b;
GHandle delete_contacts_b;
GHandle back_contacts_b;
GHandle telNum_contacts_lb;
GHandle name_contacts_lb;

void createContactsScreen(char* theme, struct Contact** contacts, int numOfContacts){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* home_f;
  GFILE* add_f;
  GFILE* del_f;

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

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/addButton.gif", sizeof(path));

  add_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/delButton.gif", sizeof(path));

  del_f = gfileOpen(path, "rb");

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

  // Create list of contacts

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 10;
  wi.g.width = 220;
  wi.g.height = 200;
  wi.g.parent = mainContainer_c;
  wi.text = "Contacts";
  wi.customDraw = 0;
  wi.customParam = 0;
  wi.customStyle = 0;

  contacts_contacts_l = gwinListCreate(NULL, &wi, FALSE);

  // Set smooth scrool

  gwinListSetScroll(contacts_contacts_l, scrollSmooth);

  // Create contactsHome_b

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

  home_contacts_b = gwinButtonCreate(0, &wi);

  // Create addContact_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(add_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "add";

  }
  else{

    gdispImageOpenGFile(&add_p, add_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &add_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  add_contacts_b = gwinButtonCreate(0, &wi);

  // Create delContact_b

  wi.g.show = TRUE;
  wi.g.x = 170;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(del_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "del";

  }
  else{

    gdispImageOpenGFile(&del_p, del_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &del_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  delete_contacts_b = gwinButtonCreate(0, &wi);

  // Add items into a list

  for(i = 0; i < numOfContacts; i++){

    gwinListAddItem(contacts_contacts_l, contacts[i]->name, TRUE);

  }

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyContactsScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&home_p);

  gdispImageClose(&add_p);

  gdispImageClose(&del_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void createContactAddScreen(char* theme){

  // GFile pointers and vars

  GFILE* background_f;
  GFILE* add_f;
  GFILE* back_f;

  GWidgetInit wi;

  char path[100];

  gwinWidgetClearInit(&wi);

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Open all pictures

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/background/background.gif", sizeof(path));

  background_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/addButton.gif", sizeof(path));

  add_f = gfileOpen(path, "rb");

  memset(path, 0, sizeof(path)); strcat(path, "/themes/"); strlcat(path, theme, sizeof(path)); strlcat(path, "/icons/back.gif", sizeof(path));

  back_f = gfileOpen(path, "rb");

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

  // Create name label

  wi.g.y = 10;
  wi.g.x = 10;
  wi.g.width = 220;
  wi.g.height = 20;
  wi.text = "Enter a name";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  name_contacts_lb = gwinButtonCreate(NULL, &wi);

  // Create tel number label

  wi.g.y = 40;
  wi.g.x = 10;
  wi.g.width = 220;
  wi.g.height = 20;
  wi.text = "000000000";
  wi.g.parent = mainContainer_c;
  wi.customDraw = gwinButtonDraw_Normal;
  wi.customStyle = &WhiteWidgetStyle;
  wi.customParam = 0;
  wi.g.show = TRUE;

  telNum_contacts_lb = gwinButtonCreate(NULL, &wi);

  // KEYBOARD

  wi.g.y = 90;
  wi.g.x = 11;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "0";

  _0_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 33;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "1";

  _1_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 55;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "2";

  _2_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 77;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "3";

  _3_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 99;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "4";

  _4_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 121;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "5";

  _5_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 143;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "6";

  _6_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 165;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "7";

  _7_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 187;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "8";

  _8_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 90;
  wi.g.x = 209;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "9";

  _9_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 11;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "a";

  _a_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 33;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "b";

  _b_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 55;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "c";

  _c_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 77;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "d";

  _d_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 99;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "e";

  _e_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 121;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "f";

  _f_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 143;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "g";

  _g_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 165;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "h";

  _h_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 187;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "i";

  _i_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 120;
  wi.g.x = 209;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "j";

  _j_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 11;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "k";

  _k_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 33;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "l";

  _l_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 55;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "m";

  _m_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 77;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "n";

  _n_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 99;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "o";

  _o_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 121;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "p";

  _p_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 143;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "r";

  _r_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 165;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "s";

  _s_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 187;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "t";

  _t_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 150;
  wi.g.x = 209;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "u";

  _u_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 11;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "v";

  _v_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 33;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "z";

  _z_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 55;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "x";

  _x_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 77;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "y";

  _y_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 99;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "q";

  _q_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 121;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = ".";

  _dot_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 143;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = ",";

  _comma_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 165;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "/";

  _slash_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 187;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = "-";

  _exclamation_b = gwinButtonCreate(NULL, &wi);

  wi.g.y = 180;
  wi.g.x = 209;
  wi.g.width = 20;
  wi.g.height = 20;
  wi.text = " ";

  _space_b = gwinButtonCreate(NULL, &wi);

  // Create saveContact_b

  wi.g.show = TRUE;
  wi.g.x = 10;
  wi.g.y = 240;
  wi.g.width = 60;
  wi.g.height = 60;
  wi.g.parent = mainContainer_c;

  if(add_f == NULL){

    wi.customDraw = gwinButtonDraw_Normal;
    wi.customStyle = &WhiteWidgetStyle;
    wi.customParam = 0;
    wi.text = "add";

  }
  else{

    gdispImageOpenGFile(&add_p, add_f);
    wi.customDraw = gwinButtonDraw_Image;
    wi.customParam = &add_p;
    wi.customStyle = 0;
    wi.text = "";

  }

  save_contacts_b = gwinButtonCreate(0, &wi);

  // Create backContact_b

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

  back_contacts_b = gwinButtonCreate(0, &wi);

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}

void destroyContactAddScreen(void){

  // Set high priority level

  chThdSetPriority(HIGHPRIO);

  // Close all images

  gdispImageClose(&background_p);

  gdispImageClose(&add_p);

  gdispImageClose(&back_p);

  // Destroy all widgets and mainContainer_c

  gwinDestroy(mainContainer_c);

  // Set GHandles to NULL

  contacts_contacts_l = NULL;

  home_contacts_b = NULL;

  add_contacts_b = NULL;

  save_contacts_b = NULL;

  delete_contacts_b = NULL;

  back_contacts_b = NULL;

  telNum_contacts_lb = NULL;

  name_contacts_lb = NULL;

  _0_b = NULL;_1_b = NULL;_2_b = NULL;_3_b = NULL;_4_b = NULL;_5_b = NULL;_6_b = NULL;_7_b = NULL;_8_b = NULL;_9_b = NULL;

  _a_b = NULL; _b_b = NULL;_c_b = NULL;_d_b = NULL;_e_b = NULL;_f_b = NULL;_g_b = NULL;_h_b = NULL;_i_b = NULL;_j_b = NULL;

  _k_b = NULL;_l_b = NULL;_m_b = NULL;_n_b = NULL;_o_b = NULL;_p_b = NULL;_r_b = NULL;_s_b = NULL;_t_b = NULL;_u_b = NULL;

  _v_b = NULL;_z_b = NULL;_x_b = NULL;_y_b = NULL;_q_b = NULL;_dot_b = NULL;_comma_b = NULL;_slash_b = NULL;_exclamation_b = NULL;

  _space_b = NULL;

  // Set normal priority level

  chThdSetPriority(NORMALPRIO);

}
