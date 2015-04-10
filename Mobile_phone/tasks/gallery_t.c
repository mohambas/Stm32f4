#include "gallery_t.h"

threadreturn_t gallery_t(void* param){

  // Structure

  struct Gallery gallery;

  Thread* thrd;

  // Set numOfPics to zero

  gallery.numOfPics = 0;

  for(;;){

    // Wait for message

    thrd = chMsgWait();

    // Get pictures

    getPictures(&gallery);

    // Send a pointer

    chMsgRelease(thrd, (msg_t)&gallery);

  }

}
