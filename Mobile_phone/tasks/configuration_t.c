#include "configuration_t.h"

threadreturn_t configuration_t(void* param){

  // Structure

  struct Configuration config;

  Thread* thrd;

  msg_t msg_conf;

  // Set numOfThemes to 0

  config.numOfThemes = 0;

  for(;;){

    // Wait for message

    thrd = chMsgWait();

    // Read message

    msg_conf = chMsgGet(thrd);

    if(((struct Configuration*)msg_conf)->writeConfig == 1){

      strlcpy(config.theme, ((struct Configuration*)msg_conf)->theme, THEME_NAME_LENGTH);

      config.brightness = ((struct Configuration*)msg_conf)->brightness;

      // Write configuration

      writeConfiguration(&config);

      chMsgRelease(thrd, 0);

    }
    else if(((struct Configuration*)msg_conf)->getConfig == 1){

      // Get configuration

      readConfiguration(&config);

      // Get themes

      getThemes(&config);

      // Send a pointer

      chMsgRelease(thrd, (msg_t)&config);

    }

  }

}
