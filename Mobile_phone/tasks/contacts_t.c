#include "contacts_t.h"

threadreturn_t contacts_t(void* param){

  // Structure

  struct Contacts contacts;

  Thread* thrd;

  msg_t msg_cont;

  // Set numOfContacts to 0

  contacts.numOfContacts = 0;

  for(;;){

    // Wait for message

    thrd = chMsgWait();

    // Read message

    msg_cont = chMsgGet(thrd);

    if(((struct Contacts*)msg_cont)->getContacts == 1){

      getContacts(&contacts);

      chMsgRelease(thrd, (msg_t)&contacts);

    }
    else if(((struct Contacts*)msg_cont)->getContact == 1){

      strlcpy(contacts.contact.telNumber, ((struct Contacts*)msg_cont)->contact.telNumber, TELEPHONE_NUM_LENGTH);

      getContact(&contacts);

      chMsgRelease(thrd, (msg_t)&contacts);

    }
    else if(((struct Contacts*)msg_cont)->addContact == 1){

      strlcpy(contacts.contact.name, ((struct Contacts*)msg_cont)->contact.name, CONTACT_NAME_LENGTH);

      strlcpy(contacts.contact.telNumber, ((struct Contacts*)msg_cont)->contact.telNumber, TELEPHONE_NUM_LENGTH);

      addContact(&contacts);

      getContacts(&contacts);

      chMsgRelease(thrd, (msg_t)&contacts);

    }
    else if(((struct Contacts*)msg_cont)->deleteContact == 1){

      contacts.contact.id = ((struct Contacts*)msg_cont)->contact.id;

      deleteContact(&contacts);

      getContacts(&contacts);

      chMsgRelease(thrd, (msg_t)&contacts);

    }

  }

}
