#include "calls_t.h"

threadreturn_t calls_t(void* param){

  // Structures

  struct Calls calls;

  struct Contacts contacts;

  Thread* thrd;

  msg_t msg_calls;

  // Set number of calls to zero

  calls.numOfCalls = 0;

  for(;;){

   // Wait for message

   thrd = chMsgWait();

   // Read message

   msg_calls = chMsgGet(thrd);

    // Check getMissedCalls flag

    if(((struct Calls*)msg_calls)->getMissedCalls == 1){

      getCalls(&calls);

      // Send a pointer

      chMsgRelease(thrd, (msg_t)&calls);

    }

    // Check delete all flag

    else if(((struct Calls*)msg_calls)->deleteCalls == 1){

      deleteCalls(&calls);

      // Send a pointer

      chMsgRelease(thrd, (msg_t)&calls);

    }

    // Check missed call flag

    else if(((struct Calls*)msg_calls)->missedCall == 1){

      strlcpy(contacts.contact.telNumber, ((struct Calls*)msg_calls)->call.callerTelNum, CONTACT_NAME_LENGTH);

      contacts.addContact = 0;

      contacts.deleteContact = 0;

      contacts.getContact = 1;

      contacts.getContacts = 0;

      msg_calls = chMsgSend(contacts_thrd_h, (msg_t)&contacts);

      if(((struct Contacts*)msg_calls)->contactFound == 1){

        // Contact found... Add a name into a missed calls file

        addCall(((struct Contacts*)msg_calls)->contact.name);

      }
      else{

        // Contact not found... Add a tel.num into a missed calls file

        addCall(contacts.contact.telNumber);

      }

      chMsgRelease(thrd, 0);

    }

  }

}
