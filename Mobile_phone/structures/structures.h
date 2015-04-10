#ifndef STRUCTURES_H
#define STRUCTURES_H

#define THEME_NAME_LENGTH 50
#define CONTACT_NAME_LENGTH 50
#define TELEPHONE_NUM_LENGTH 11
#define CONTACT_LENGTH 62
#define BRIGHTNESS_LENGTH 5
#define PICTURE_NAME_LENGTH 50
#define GSM_RESPONSE_LENGTH 261

// Calls structures

struct Call{

  char callerTelNum[CONTACT_NAME_LENGTH];

};

struct Calls{

  // All unread calls

  struct Call** missedCalls;

  // Add unread call

  struct Call call;

  // Number of missed calls

  int numOfCalls;

  // Flags

  unsigned int missedCall : 1;

  unsigned int getMissedCalls : 1;

  unsigned int deleteCalls : 1;

};

// Message structures

// AudioPlayer structures

// gallery structures

struct Gallery{

  // All pictures

  char** pictures;

  // Number of pictures

  int numOfPics;

};

// Contacts structures

struct Contact{

  char name[CONTACT_NAME_LENGTH];

  char telNumber[TELEPHONE_NUM_LENGTH];

  int id;

};

struct Contacts{

  // All contacts

  struct Contact** contacts;

  // Number of contacts

  int numOfContacts;

  // Add/get/del contact

  struct Contact contact;

  // Flags

  unsigned int getContacts : 1;

  unsigned int getContact : 1;

  unsigned int addContact : 1;

  unsigned int deleteContact : 1;

  unsigned int contactFound : 1;

};

// Settings structures

struct Configuration{

  // Theme name

  char theme[THEME_NAME_LENGTH];

  // All themes

  char** themes;

  // Brightness

  unsigned char brightness;

  // Number of themes

  int numOfThemes;

  // Flags

  unsigned int getConfig : 1;

  unsigned int writeConfig : 1;

};

// Notifications structures

struct Notification{

  // Caller

  struct Call caller;

  // Flags

  unsigned int newSms : 1;

  unsigned int callStarted : 1;

  unsigned int callEnded : 1;

};

#endif
