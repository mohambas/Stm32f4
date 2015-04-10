#include "gui_t.h"

static void getConfiguration(struct Configuration*);

static void setConfiguration(struct Configuration*);

static void getPictures(struct Gallery*);

static void getContacts(struct Contacts*);

static void addContact(struct Contacts*);

static void getContact(struct Contacts*);

static void deleteContact(struct Contacts*);

static void getMissedCalls(struct Calls*);

static void deleteMissedCalls(struct Calls*);

static void createNextScreen(int, struct Configuration*, struct Calls*, struct Contacts*);

static void destroyCurrentScreen(int);

// GHandles for keyboard... Shared with all gui forms who need a keyboard

GHandle _0_b;
GHandle _1_b;
GHandle _2_b;
GHandle _3_b;
GHandle _4_b;
GHandle _5_b;
GHandle _6_b;
GHandle _7_b;
GHandle _8_b;
GHandle _9_b;

GHandle _a_b;
GHandle _b_b;
GHandle _c_b;
GHandle _d_b;
GHandle _e_b;
GHandle _f_b;
GHandle _g_b;
GHandle _h_b;
GHandle _i_b;
GHandle _j_b;
GHandle _k_b;
GHandle _l_b;
GHandle _m_b;
GHandle _n_b;
GHandle _o_b;
GHandle _p_b;
GHandle _r_b;
GHandle _s_b;
GHandle _t_b;
GHandle _u_b;
GHandle _v_b;
GHandle _z_b;
GHandle _x_b;
GHandle _y_b;
GHandle _q_b;
GHandle _dot_b;
GHandle _comma_b;
GHandle _slash_b;
GHandle _exclamation_b;
GHandle _space_b;

threadreturn_t gui_t(void* param){

  // Structures

  struct Configuration config;

  struct Gallery gallery;

  struct Contacts contacts;

  struct Calls calls;

  // telNumberTxtField clicked(contacts)

  bool_t telNumberTxtField;

  // nameTxtField clicked(contacts)

  bool_t nameTxtField;

  // Buffer for tel.num and name of a contact when typing on a keyboard

  char contactNameTelBuff[CONTACT_LENGTH];

  // Buffer for tel.num which we are going to call

  char CallingTelNumberBuffer[TELEPHONE_NUM_LENGTH];

  // GListeners

  GListener gl;

  // Last screen id

  unsigned char lastScreenId = MAIN_SCREEN_ID;

  // Currently selected contact index

  int currentContactIndex = 0;

  // Current picture displayed in a gallery

  int pictureIndx = 0;

  GEvent* pe;

  // Attach listener

  geventListenerInit(&gl);

  gwinAttachListener(&gl);

  // Get configuration

  getConfiguration(&config);

  // get pictures for gallery

  getPictures(&gallery);

  // Get contacts

  getContacts(&contacts);

  // create main screen

  createMainScreen(config.theme);

  for(;;){

    // Get an event

    pe = geventEventWait(&gl, MS2ST(20));

    switch(pe->type){

      // Buttons

      case GEVENT_GWIN_BUTTON:

        if(((GEventGWinButton*)pe)->gwin == calls_mainScreen_b){

          // Get missed calls

          getMissedCalls(&calls);

          destroyCurrentScreen(lastScreenId);

          lastScreenId = CALLS_SCREEN_ID;

          createNextScreen(lastScreenId, &config, &calls, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == home_calls_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = MAIN_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == open_calls_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = MAKE_CALL_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == back_calls_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = CALLS_SCREEN_ID;

          createNextScreen(lastScreenId, &config, &calls, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == clear_calls_b){

          // Clear missed calls

          if(calls.numOfCalls > 0){

            deleteMissedCalls(&calls);

            gwinListDeleteAll(calls_calls_l);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == call_calls_b){

          // Check if tel number is entered

          if(strlen(gwinGetText(telNum_calls_lb)) > 0){

            strlcpy(contacts.contact.telNumber, gwinGetText(telNum_calls_lb), TELEPHONE_NUM_LENGTH);

            // Check if a contact exists under an entered number

            getContact(&contacts);

            // Make a call

            callNumber(gwinGetText(telNum_calls_lb));

            destroyCurrentScreen(lastScreenId);

            // Check if a contact is found... Create calling screen

            if(contacts.contactFound == 1){

              createCallScreen(config.theme, contacts.contact.name, FALSE);

            }
            else{

              createCallScreen(config.theme, contacts.contact.telNumber, FALSE);

            }

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == c_0_b || ((GEventGWinButton*)pe)->gwin == c_1_b
            || ((GEventGWinButton*)pe)->gwin == c_2_b || ((GEventGWinButton*)pe)->gwin == c_3_b
            || ((GEventGWinButton*)pe)->gwin == c_4_b || ((GEventGWinButton*)pe)->gwin == c_5_b
            || ((GEventGWinButton*)pe)->gwin == c_6_b || ((GEventGWinButton*)pe)->gwin == c_7_b
            || ((GEventGWinButton*)pe)->gwin == c_8_b || ((GEventGWinButton*)pe)->gwin == c_9_b){

          // Put number into a telNumCall_lb field

          if(strlen(CallingTelNumberBuffer) < TELEPHONE_NUM_LENGTH){

            strlcpy(CallingTelNumberBuffer, gwinGetText(telNum_calls_lb), TELEPHONE_NUM_LENGTH);

            strlcat(CallingTelNumberBuffer, gwinGetText(((GEventGWinButton*)pe)->gwin), TELEPHONE_NUM_LENGTH);

            gwinSetText(telNum_calls_lb, CallingTelNumberBuffer, TRUE);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == telNum_calls_lb){

          // Clear telNumCall_lb field

          gwinSetText(telNum_calls_lb, "", TRUE);

        }
        else if(((GEventGWinButton*)pe)->gwin == decline_call_b){

          // End a call

          hangUp();

          destroyCallScreen();

          createNextScreen(lastScreenId, &config, &calls, &contacts);

        }
        else if(((GEventGWinButton*)pe)->gwin == messages_mainScreen_b){


        }
        else if(((GEventGWinButton*)pe)->gwin == audioPlayer_mainScreen_b){


        }
        else if(((GEventGWinButton*)pe)->gwin == gallery_mainScreen_b){

          // Reset current picture index in gallery to zero

          pictureIndx = 0;

          destroyCurrentScreen(lastScreenId);

          lastScreenId = GALLERY_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

          // Show first picture

          if(gallery.numOfPics > 0){

            setPicture(gallery.pictures[pictureIndx]);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == home_gallery_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = MAIN_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == nextImage_gallery_b){

          // Show next picture

          if(gallery.numOfPics > 0){

            pictureIndx = (pictureIndx + 1) % gallery.numOfPics;

            setPicture(gallery.pictures[pictureIndx]);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == previousImage_gallery_b){

          // Show prev picture

          if(gallery.numOfPics > 0){

            pictureIndx = (pictureIndx - 1 < 0) ? (((pictureIndx - 1) % gallery.numOfPics) + gallery.numOfPics) : (pictureIndx - 1) % gallery.numOfPics;

            setPicture(gallery.pictures[pictureIndx]);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == settings_mainScreen_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = SETTINGS_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == home_settings_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = MAIN_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == contacts_mainScreen_b){

          // telNumberTxtField false

          telNumberTxtField = FALSE;

          // nameTxtField false

          nameTxtField = FALSE;

          // Set current index to -1

          currentContactIndex = -1;

          destroyCurrentScreen(lastScreenId);

          lastScreenId = CONTACTS_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, &contacts);

        }
        else if(((GEventGWinButton*)pe)->gwin == add_contacts_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = CONTACTS_ADD_SCREEN;

          createNextScreen(lastScreenId, &config, NULL, &contacts);

        }
        else if(((GEventGWinButton*)pe)->gwin == delete_contacts_b){

          // Delete a contact

          if(contacts.numOfContacts > 0 && gwinListGetSelected(contacts_contacts_l) >= 0){

            contacts.contact.id = gwinListGetSelected(contacts_contacts_l);

            // Delete selected contact

            deleteContact(&contacts);

            // Delete contact from list

            gwinListItemDelete(contacts_contacts_l, gwinListGetSelected(contacts_contacts_l));

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == save_contacts_b){

          // Check if a contact name and tel. number are entered

          if(strlen(gwinGetText(name_contacts_lb)) > 0 && strlen(gwinGetText(telNum_contacts_lb)) > 0){

            // Save a new contact

            strlcpy(contacts.contact.name, gwinGetText(name_contacts_lb), CONTACT_NAME_LENGTH);

            strlcpy(contacts.contact.telNumber, gwinGetText(telNum_contacts_lb), TELEPHONE_NUM_LENGTH);

            addContact(&contacts);

            destroyCurrentScreen(lastScreenId);

            lastScreenId = CONTACTS_SCREEN_ID;

            createNextScreen(lastScreenId, &config, NULL, &contacts);

          }

        }
        else if(((GEventGWinButton*)pe)->gwin == back_contacts_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = CONTACTS_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, &contacts);

        }
        else if(((GEventGWinButton*)pe)->gwin == home_contacts_b){

          destroyCurrentScreen(lastScreenId);

          lastScreenId = MAIN_SCREEN_ID;

          createNextScreen(lastScreenId, &config, NULL, NULL);

        }
        else if(((GEventGWinButton*)pe)->gwin == name_contacts_lb){

          // Clear name field

          gwinSetText(name_contacts_lb, "Enter a name", TRUE);

          telNumberTxtField = FALSE;

          nameTxtField = TRUE;

          // Clear contactNameTelBuff

          memset(contactNameTelBuff, 0, CONTACT_LENGTH);

        }
        else if(((GEventGWinButton*)pe)->gwin == telNum_contacts_lb){

          // Clear tel field

          gwinSetText(telNum_contacts_lb, "000000000", TRUE);

          telNumberTxtField = TRUE;

          nameTxtField = FALSE;

          // Clear contactNameTelBuff

          memset(contactNameTelBuff, 0, CONTACT_LENGTH);

        }
        else if(((GEventGWinButton*)pe)->gwin == notifications_mainScreen_b){

        }

        // Keyboard buttons

        else if(((GEventGWinButton*)pe)->gwin == _0_b || ((GEventGWinButton*)pe)->gwin == _1_b
            || ((GEventGWinButton*)pe)->gwin == _2_b || ((GEventGWinButton*)pe)->gwin == _3_b
            || ((GEventGWinButton*)pe)->gwin == _4_b || ((GEventGWinButton*)pe)->gwin == _5_b
            || ((GEventGWinButton*)pe)->gwin == _6_b || ((GEventGWinButton*)pe)->gwin == _7_b
            || ((GEventGWinButton*)pe)->gwin == _8_b || ((GEventGWinButton*)pe)->gwin == _9_b
            || ((GEventGWinButton*)pe)->gwin == _a_b || ((GEventGWinButton*)pe)->gwin == _b_b
            || ((GEventGWinButton*)pe)->gwin == _c_b || ((GEventGWinButton*)pe)->gwin == _d_b
            || ((GEventGWinButton*)pe)->gwin == _e_b || ((GEventGWinButton*)pe)->gwin == _f_b
            || ((GEventGWinButton*)pe)->gwin == _g_b || ((GEventGWinButton*)pe)->gwin == _h_b
            || ((GEventGWinButton*)pe)->gwin == _i_b || ((GEventGWinButton*)pe)->gwin == _j_b
            || ((GEventGWinButton*)pe)->gwin == _k_b || ((GEventGWinButton*)pe)->gwin == _l_b
            || ((GEventGWinButton*)pe)->gwin == _m_b || ((GEventGWinButton*)pe)->gwin == _n_b
            || ((GEventGWinButton*)pe)->gwin == _o_b || ((GEventGWinButton*)pe)->gwin == _p_b
            || ((GEventGWinButton*)pe)->gwin == _r_b || ((GEventGWinButton*)pe)->gwin == _s_b
            || ((GEventGWinButton*)pe)->gwin == _t_b || ((GEventGWinButton*)pe)->gwin == _u_b
            || ((GEventGWinButton*)pe)->gwin == _v_b || ((GEventGWinButton*)pe)->gwin == _z_b
            || ((GEventGWinButton*)pe)->gwin == _x_b || ((GEventGWinButton*)pe)->gwin == _y_b
            || ((GEventGWinButton*)pe)->gwin == _q_b || ((GEventGWinButton*)pe)->gwin == _dot_b
            || ((GEventGWinButton*)pe)->gwin == _comma_b ||((GEventGWinButton*)pe)->gwin == _slash_b
            || ((GEventGWinButton*)pe)->gwin == _exclamation_b || ((GEventGWinButton*)pe)->gwin == _space_b){

           // Put char into a nameTxtField(contacts)

           if(nameTxtField){

             // Clear name_lb

             if(strlen(contactNameTelBuff) == 0)
               gwinSetText(name_contacts_lb, "", TRUE);

             if(strlen(gwinGetText(name_contacts_lb)) < CONTACT_NAME_LENGTH){

               strcpy(contactNameTelBuff, gwinGetText(name_contacts_lb));

               strcat(contactNameTelBuff, gwinGetText(((GEventGWinButton*)pe)->gwin));

               gwinSetText(name_contacts_lb, contactNameTelBuff, TRUE);

             }

           }

           // Put char into a telNumberTxtField(contacts)

           else if(telNumberTxtField){

             // Clear telNum_lb

             if(strlen(contactNameTelBuff) == 0)
               gwinSetText(telNum_contacts_lb, "", TRUE);

             if(strlen(gwinGetText(telNum_contacts_lb)) < TELEPHONE_NUM_LENGTH && gwinGetText(((GEventGWinButton*)pe)->gwin)[0] >= 48
               && gwinGetText(((GEventGWinButton*)pe)->gwin)[0] <= 57){

               strcpy(contactNameTelBuff, gwinGetText(telNum_contacts_lb));

               strcat(contactNameTelBuff, gwinGetText(((GEventGWinButton*)pe)->gwin));

               gwinSetText(telNum_contacts_lb, contactNameTelBuff, TRUE);

             }

           }

        }

        break;

        // Slider

        case GEVENT_GWIN_SLIDER:

          // Set new brightness

          config.brightness = ((GEventGWinSlider *)pe)->position;

          // Refresh brightness

          gdispSetBacklight(config.brightness);

          // Send change to a configuration_t thread

          setConfiguration(&config);

          break;

        // list

        case GEVENT_GWIN_LIST:

          if(((GEventGWinButton*)pe)->gwin == themes_settings_l){

            // Set new theme

            strcpy(config.theme, config.themes[gwinListGetSelected(themes_settings_l)]);

            // Send change to a configuration_t thread

            setConfiguration(&config);

          }
          else if(((GEventGWinButton*)pe)->gwin == contacts_contacts_l){

            // Call a contact which was double clicked

            if(gwinListGetSelected(contacts_contacts_l) == currentContactIndex){

              // Make a call

              callNumber(contacts.contacts[currentContactIndex]->telNumber);

              destroyCurrentScreen(lastScreenId);

              // Create call screen

              createCallScreen(config.theme, contacts.contacts[currentContactIndex]->name, FALSE);

              // Set currently selected index to -1

              currentContactIndex = -1;

            }
            else{

              // Update currently selected index

              currentContactIndex = gwinListGetSelected(contacts_contacts_l);

            }

          }

          break;

        default:

          break;

    }

    // Check if there is a notification coming from a uart_t thread

    {

      msg_t msg_gui;

      if(chMBFetch(&gsmResponse, &msg_gui, TIME_IMMEDIATE) == RDY_OK){

        // Check flag for a new call

        if(((struct Notification*)msg_gui)->callStarted == 1){

          strlcpy(contacts.contact.telNumber, ((struct Notification*)msg_gui)->caller.callerTelNum, TELEPHONE_NUM_LENGTH);

          // Check if a contact exists under an entered number

          getContact(&contacts);

          destroyCurrentScreen(lastScreenId);

          if(contacts.contactFound == 1){

            createCallScreen(config.theme, contacts.contact.name, TRUE);

          }
          else{

            createCallScreen(config.theme, contacts.contact.telNumber, TRUE);

          }

        }

        // Check flag for an ended call

        else if(((struct Notification*)msg_gui)->callEnded == 1){

          destroyCallScreen();

          createNextScreen(lastScreenId, &config, &calls, &contacts);

        }

      }

    }

  }

}

static void getConfiguration(struct Configuration* config){

  msg_t msg_gui;

  // Get configuration structure

  config->getConfig = 1;config->writeConfig = 0;

  msg_gui = chMsgSend(configuration_thrd_h, (msg_t)config);

  // Set configuration structure for gui_t thread

  config->brightness = ((struct Configuration *)msg_gui)->brightness;

  strcpy(config->theme, ((struct Configuration*)msg_gui)->theme);

  config->themes =  ((struct Configuration*)msg_gui)->themes;

  config->numOfThemes =  ((struct Configuration*)msg_gui)->numOfThemes;

}

static void setConfiguration(struct Configuration* config){

  // Send pointer

  config->getConfig = 0;config->writeConfig = 1;

  chMsgSend(configuration_thrd_h, (msg_t)config);

}

static void getPictures(struct Gallery* gallery){

  msg_t msg_gui;

 // Get gallery structure

 msg_gui = chMsgSend(gallery_thrd_h, 0);

 gallery->numOfPics = ((struct Gallery*)msg_gui)->numOfPics;

 gallery->pictures = ((struct Gallery*)msg_gui)->pictures;


}

static void getContacts(struct Contacts* contacts){

  msg_t msg_gui;

  // Get contacts

  contacts->getContacts = 1;contacts->getContact = 0;contacts->addContact = 0;contacts->deleteContact = 0;

  msg_gui = chMsgSend(contacts_thrd_h, (msg_t)contacts);

  contacts->contacts = ((struct Contacts*)msg_gui)->contacts;

  contacts->numOfContacts = ((struct Contacts*)msg_gui)->numOfContacts;

}

static void addContact(struct Contacts* contacts){

  msg_t msg_gui;

  // Add contact

  contacts->getContacts = 0;contacts->getContact = 0;contacts->addContact = 1;contacts->deleteContact = 0;

  msg_gui = chMsgSend(contacts_thrd_h, (msg_t)contacts);

  contacts->contacts = ((struct Contacts*)msg_gui)->contacts;

  contacts->numOfContacts = ((struct Contacts*)msg_gui)->numOfContacts;

}

static void getContact(struct Contacts* contacts){

  msg_t msg_gui;

  // Get contact

  contacts->getContacts = 0;contacts->getContact = 1;contacts->addContact = 0;contacts->deleteContact = 0;

  msg_gui = chMsgSend(contacts_thrd_h, (msg_t)contacts);

  strlcpy(contacts->contact.name, ((struct Contacts*)msg_gui)->contact.name, CONTACT_NAME_LENGTH);

  // Copy flag

  contacts->contactFound = ((struct Contacts*)msg_gui)->contactFound;

}

static void deleteContact(struct Contacts* contacts){

  msg_t msg_gui;

  // Delete contact

  contacts->getContacts = 0;contacts->getContact = 0;contacts->addContact = 0;contacts->deleteContact = 1;

  msg_gui = chMsgSend(contacts_thrd_h, (msg_t)contacts);

  contacts->contacts = ((struct Contacts*)msg_gui)->contacts;

  contacts->numOfContacts = ((struct Contacts*)msg_gui)->numOfContacts;

}

static void getMissedCalls(struct Calls* calls){

  msg_t msg_gui;

  // Get missed calls

  calls->deleteCalls = 0;calls->getMissedCalls = 1;calls->missedCall = 0;

  msg_gui = chMsgSend(calls_thrd_h, (msg_t)calls);

  calls->missedCalls = ((struct Calls*)msg_gui)->missedCalls;

  calls->numOfCalls = ((struct Calls*)msg_gui)->numOfCalls;

}

static void deleteMissedCalls(struct Calls* calls){

  msg_t msg_gui;

  // Delete all missed calls

  calls->deleteCalls = 1;calls->getMissedCalls = 0;calls->missedCall = 0;

  msg_gui = chMsgSend(calls_thrd_h, (msg_t)calls);

  calls->numOfCalls = ((struct Calls*)msg_gui)->numOfCalls;

}

static void createNextScreen(int lastScreenId, struct Configuration* config, struct Calls* calls, struct Contacts* contacts){

  switch(lastScreenId){

    case CALLS_SCREEN_ID:

      createCallsScreen(config->theme,calls->missedCalls, calls->numOfCalls);

      break;

    case MAKE_CALL_SCREEN_ID:

      createMakeCallScreen(config->theme);

      break;

    case CONTACTS_SCREEN_ID:

      createContactsScreen(config->theme, contacts->contacts, contacts->numOfContacts);

      break;

     case CONTACTS_ADD_SCREEN:

       createContactAddScreen(config->theme);

       break;

     case GALLERY_SCREEN_ID:

       createGalleryScreen(config->theme);

       break;

     case MAIN_SCREEN_ID:

       createMainScreen(config->theme);

       break;

     case SETTINGS_SCREEN_ID:

       createSettingsScreen(config->theme, config->themes, config->numOfThemes, config->brightness);

       break;

     default:

       break;


  }

}

static void destroyCurrentScreen(int lastScreenId){

  switch(lastScreenId){

    case CALLS_SCREEN_ID:

      destroyCallsScreen();

      break;

    case MAKE_CALL_SCREEN_ID:

      destroyMakeCallScreen();

      break;

    case CONTACTS_SCREEN_ID:

      destroyContactsScreen();

      break;

    case CONTACTS_ADD_SCREEN:

      destroyContactAddScreen();

      break;

    case GALLERY_SCREEN_ID:

      destroyGalleryScreen();

      break;

    case MAIN_SCREEN_ID:

      destroyMainScreen();

      break;

    case SETTINGS_SCREEN_ID:

      destroySettingsScreen();

      break;

    default:

      break;

  }

}

