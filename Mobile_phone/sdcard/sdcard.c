#include "sdcard.h"

static int getNumOfThemes(void);

static int getNumOfPictures(void);

static int getNumOfContacts(void);

static int getNumOfCalls(void);

static bool_t readLine(GFILE*, char*, short);

static void writeLine(GFILE*, char*);

static void toString(char[], int);

void initSdio(void){

  // Initialize SDIO

  sdcStart(&SDCD1, NULL);

  sdcConnect(&SDCD1);

}

void readConfiguration(struct Configuration* config){

  GFILE* file;

  char brightness[BRIGHTNESS_LENGTH];

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("configuration/configuration.txt", "r");

  if(file){

    // Read theme name

    readLine(file, config->theme, THEME_NAME_LENGTH);

    // Read brightness

    readLine(file, brightness, BRIGHTNESS_LENGTH);

    config->brightness = atoi(brightness);

    // Close file

    gfileClose(file);

  }
  else{

    // Set default values

    strlcpy(config->theme, "default", THEME_NAME_LENGTH); // Default theme

    config->brightness = 255; // max brightness

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void writeConfiguration(struct Configuration* config){

  GFILE* file;

  char brightness[BRIGHTNESS_LENGTH];

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("configuration/configuration.txt", "w");

  if(file){

    // Write theme into a file

    writeLine(file, config->theme);

    // Write brightness into a file

    toString(brightness, config->brightness);

    writeLine(file, brightness);

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void getThemes(struct Configuration* config){

  int i;

  gfileList* fileList;

  const char* theme;

  // Free resources

  if(config->numOfThemes > 0){

    for(i = 0; i < config->numOfThemes; i++){

      chHeapFree(config->themes[i]);

    }

    chHeapFree(config->themes);

  }

  // get number of themes

  config->numOfThemes = getNumOfThemes();

  // Allocate resources

  config->themes = chHeapAlloc(NULL, config->numOfThemes * sizeof(char*));

  for(i = 0; i < config->numOfThemes; i++){

    config->themes[i] = chHeapAlloc(NULL, THEME_NAME_LENGTH * sizeof(char));

  }

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open filelist

  fileList = gfileOpenFileList('F', "themes", TRUE);

  if(fileList){

    for(i = 0; (theme = gfileReadFileList(fileList)) != NULL; i++){

      strlcpy(config->themes[i], theme, THEME_NAME_LENGTH);

    }

    // Close fileList

    gfileCloseFileList(fileList);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

static int getNumOfThemes(void){

  gfileList* fileList;

  int cnt = 0;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open filelist

  fileList = gfileOpenFileList('F', "themes", TRUE);

  if(fileList){

    while(gfileReadFileList(fileList) != NULL)
      cnt++;

    // Close fileList

    gfileCloseFileList(fileList);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

  // Return number of themes

  return cnt;

}

void getPictures(struct Gallery* gallery){

  int i;

  gfileList* fileList;

  const char* picture;

  // Free resources

  if(gallery->numOfPics > 0){

    for(i = 0; i < gallery->numOfPics; i++){

      chHeapFree(gallery->pictures[i]);

    }

    chHeapFree(gallery->pictures);

  }

  // Get number of pictures

  gallery->numOfPics = getNumOfPictures();

  // Allocate resources

  gallery->pictures = chHeapAlloc(NULL, gallery->numOfPics * sizeof(char*));

  for(i = 0; i < gallery->numOfPics; i++){

    gallery->pictures[i] = chHeapAlloc(NULL, PICTURE_NAME_LENGTH * sizeof(char));

  }


  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open filelist

  fileList = gfileOpenFileList('F', "gallery", FALSE);

  if(fileList){

    for(i = 0; (picture = gfileReadFileList(fileList)) != NULL; i++){

      strlcpy(gallery->pictures[i], picture, PICTURE_NAME_LENGTH);

    }

    // Close fileList

    gfileCloseFileList(fileList);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

static int getNumOfPictures(void){

  gfileList* fileList;

  int cnt = 0;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open filelist

  fileList = gfileOpenFileList('F', "gallery", FALSE);

  if(fileList){

    while(gfileReadFileList(fileList) != NULL)
      cnt++;

    // Close fileList

    gfileCloseFileList(fileList);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

  // Return number of pictures

  return cnt;

}

void getContacts(struct Contacts* contacts){

  int i;

  GFILE* file;

  char contact[CONTACT_LENGTH];

  // Free resources

  if(contacts->numOfContacts > 0){

    for(i = 0; i < contacts->numOfContacts; i++){

      chHeapFree(contacts->contacts[i]);

    }

    chHeapFree(contacts->contacts);

  }

  // get number of contacts

  contacts->numOfContacts = getNumOfContacts();

  // Allocate resources

  contacts->contacts = chHeapAlloc(NULL, contacts->numOfContacts * sizeof(struct Contact*));

  for(i = 0; i < contacts->numOfContacts; i++){

    contacts->contacts[i] = chHeapAlloc(NULL, sizeof(struct Contact));

  }

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("contacts/contacts.txt", "r");

  if(file){

    // Read contacts

    for(i = 0; i < contacts->numOfContacts; i++){

      readLine(file, contact, CONTACT_LENGTH);

      // Check if there is : inside a string

      if(strchr(contact, ':')){

        // Copy name

        strlcpy(contacts->contacts[i]->name, contact, (int)(strchr(contact, ':') - contact) + 1);

        // Copy number

        strlcpy(contacts->contacts[i]->telNumber, strchr(contact, ':') + 1, TELEPHONE_NUM_LENGTH);

        // Set id

        contacts->contacts[i]->id = i;

      }
      else
        i--; // Skip line... There is no : inside a string

    }

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void addContact(struct Contacts* contacts){

  GFILE* file;

  char contact[CONTACT_LENGTH];

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("contacts/contacts.txt", "a");

  if(file){

    // Set cursor

    gfileSetPos(file, gfileGetSize(file));

    memset(contact, 0, CONTACT_LENGTH);

    strlcat(contact, contacts->contact.name, CONTACT_LENGTH);

    strlcat(contact, ":", CONTACT_LENGTH);

    strlcat(contact, contacts->contact.telNumber, CONTACT_LENGTH);

    // Write line into a file

    writeLine(file, contact);

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void deleteContact(struct Contacts* contacts){

  GFILE* file;

  char contact[CONTACT_LENGTH];

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("contacts/contacts.txt", "w");

  if(file){

    int i;

    for(i = 0; i < contacts->numOfContacts; i++){

      if(contacts->contacts[i]->id != contacts->contact.id){

        memset(contact, 0, CONTACT_LENGTH);

        strlcat(contact, contacts->contacts[i]->name, CONTACT_LENGTH);

        strlcat(contact, ":", CONTACT_LENGTH);

        strlcat(contact, contacts->contacts[i]->telNumber, CONTACT_LENGTH);

        // Write line into a file

        writeLine(file, contact);

      }

    }

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

bool_t getContact(struct Contacts* contacts){

  int i;

  // Set contact found flag to zero

  contacts->contactFound = 0;

  for(i = 0; i < contacts->numOfContacts; i++){

    if(strcmp(contacts->contact.telNumber, contacts->contacts[i]->telNumber) == 0){

      // Copy the name into the name field

      strlcpy(contacts->contact.name, contacts->contacts[i]->name, CONTACT_NAME_LENGTH);

      // Set contact found flag

      contacts->contactFound = 1;

      break;

    }

  }

}

static int getNumOfContacts(void){

  GFILE* file;

  char contact[CONTACT_LENGTH];

  bool_t eof;

  int cnt = 0;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("contacts/contacts.txt", "r");

  if(file){

    do{

      eof = readLine(file, contact, CONTACT_LENGTH);

      // Increase a counter if : is inside a string

      if(strchr(contact, ':'))
        cnt++;

    }
    while(eof);

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

  return cnt;

}

void getCalls(struct Calls* calls){

  int i;

  GFILE* file;

  char call[CONTACT_NAME_LENGTH];

  // Free resources

  if(calls->numOfCalls > 0){

    for(i = 0; i < calls->numOfCalls; i++){

      chHeapFree(calls->missedCalls[i]);

    }

    chHeapFree(calls->missedCalls);

  }

  // get number of contacts

  calls->numOfCalls = getNumOfCalls();

  // Allocate resources

  calls->missedCalls = chHeapAlloc(NULL, calls->numOfCalls * sizeof(struct Call*));

  for(i = 0; i < calls->numOfCalls; i++){

    calls->missedCalls[i] = chHeapAlloc(NULL, sizeof(struct Call));

  }

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("phone/missed_calls.txt", "r");

  if(file){

    for(i = 0; i < calls->numOfCalls; i++){

      readLine(file, call, CONTACT_NAME_LENGTH);

      // Check if length is greater than zero

      if(strlen(call) > 0){

        strlcpy(calls->missedCalls[i]->callerTelNum, call, CONTACT_NAME_LENGTH);

      }
      else
        i--; // Skip line... size of a string is zero

    }

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void addCall(char* call){

  GFILE* file;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("phone/missed_calls.txt", "a");

  if(file){

    // Set cursor

    gfileSetPos(file, gfileGetSize(file));

    // Write line into a file

    writeLine(file, call);

    // Close file

    gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

void deleteCalls(struct Calls* calls){

  GFILE* file;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open the file in a write mode

  file = gfileOpen("phone/missed_calls.txt", "w");

  if(file){

    int i;

    // Free resources

    if(calls->numOfCalls > 0){

      for(i = 0; i < calls->numOfCalls; i++){

        chHeapFree(calls->missedCalls[i]);

      }

      chHeapFree(calls->missedCalls);

    }

    calls->numOfCalls = 0;

    gfileClose(file); // Close the file

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

}

static int getNumOfCalls(void){

  GFILE* file;

  char call[CONTACT_NAME_LENGTH];

  bool_t eof;

  int cnt = 0;

  // Raise priority to HIGH

  chThdSetPriority(HIGHPRIO);

  // Open file

  file = gfileOpen("phone/missed_calls.txt", "r");

  if(file){

    do{

        eof = readLine(file, call, CONTACT_NAME_LENGTH);

        // Increase a counter if string length is greater than zero

        if(strlen(call) > 0)
          cnt++;

      }
      while(eof);

      // Close file

      gfileClose(file);

  }

  // Raise priority to normal

  chThdSetPriority(NORMALPRIO);

  return cnt;

}

static bool_t readLine(GFILE* file, char* buffer, short n){

  char chr;

  short cnt = 0;

  if(file){

    while(!gfileEOF(file)){

      // Read next char

      while(gfileRead(file, &chr, 1) != 1);

      if(chr == '\n')
        break;
      else if(chr == '\r')
        continue;
      else if(cnt < n){

        buffer[cnt] = chr;

        cnt++;

      }

    }

    // Add string terminating byte

    buffer[cnt] = '\0';

    // Return false if eof... otherwise true

    if(!gfileEOF(file))
      return TRUE;

  }

  // Return false

  return FALSE;

}

static void writeLine(GFILE* file, char* buffer){

  int numOfBytes = strlen(buffer);

  if(file){

    while(numOfBytes > 0){

      numOfBytes = numOfBytes - gfileWrite(file, buffer, numOfBytes);

    }

    // New line

    gfileWrite(file, "\r", 1);

    gfileWrite(file, "\n", 1);

  }

}

static void toString(char str[], int num){

  int i, rem, len = 0, n;

  n = num;

  while (n != 0){

      len++;

      n /= 10;
   }

  for (i = 0; i < len; i++){

      rem = num % 10;

      num = num / 10;

      str[len - (i + 1)] = rem + '0';

  }

  str[len] = '\0';

}
