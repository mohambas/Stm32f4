#include "gsm.h"

void initGsm(void){

  SerialConfig uartCfg;

  // Speed 9600 bauds

  uartCfg.speed = 9600;

  // Set one stop bit

  uartCfg.cr2 = USART_CR2_STOP1_BITS;

  // Initialize usart2

  sdStart(&SD2, &uartCfg); // starts the serial driver with uartCfg as a config

  // Set text mode

  sdWrite(&SD2, (uint8_t *) "AT+CMGF=1\r", 10);

  chThdSleep(100);

  // Set new sms indication

  sdWrite(&SD2, (uint8_t *) "AT+CNMI=1,1,2\r", 14);

}

void callNumber(const char* number){

  sdWrite(&SD2, (uint8_t *) "ATD", 3);

  sdWrite(&SD2, (uint8_t *) number, strlen(number));

  sdWrite(&SD2, (uint8_t *) ";\r", 2);

  chThdSleep(100);

}

void hangUp(void){

  sdWrite(&SD2, (uint8_t *) "AT+CHUP\r", 8);

  chThdSleep(100);

}

void answerCall(void){

  sdWrite(&SD2, (uint8_t *) "ATA\r", 4);

  chThdSleep(100);

}

void deleteSms(char* index){

  sdWrite(&SD2, (uint8_t *) "AT+CMGD=", 8);

  sdWrite(&SD2, (uint8_t *) index, strlen(index));

  sdWrite(&SD2, (uint8_t *) "\r", 1);

  chThdSleep(500);

}

void getSms(char* index){

  sdWrite(&SD2, (uint8_t *) "AT+CMGR=", 8);

  sdWrite(&SD2, (uint8_t *) index, strlen(index));

  sdWrite(&SD2, (uint8_t *) "\r", 1);

  chThdSleep(1500);

}

int sdReadLine(char* buffer, int n, bool_t blocking){

  uint8_t chr;

  int cnt = 0;

  // Clear buffer

  memset(buffer, sizeof(buffer), 0);

  do{

    // Get the next char

    if(blocking){

      chr = sdGet(&SD2);

    }
    else{

      chr = sdGetTimeout(&SD2, MS2ST(100));

    }

    // Pass everything except \n and \r

    if(chr != '\n' && chr != '\r' && chr != 255){

      buffer[cnt] = chr;

      cnt++;

    }

  }
  while(chr != '\n' && cnt < n && !sdGetWouldBlock(&SD2));

  // Add null terminating string

  if(cnt > 0){

    buffer[cnt] = '\0';

  }

  return cnt;

}
