#include "uart_t.h"

threadreturn_t uart_t(void* param){

  // Structures

  struct Calls calls;

  struct Notification notification;

  struct slre_cap cap;

  static char buffer[GSM_RESPONSE_LENGTH] = {0};

  // Sms id buffer

  char smsId[4];

  // Calling time

  long time = 0;

  // Flag for receiving call

  bool_t receivingCall = FALSE;

  for(;;){

    // Read data and check if the length is greater than zero

    if(sdReadLine(buffer, GSM_RESPONSE_LENGTH, FALSE) > 0){

      // Check if there is a new call

      if(strstr(buffer, "CLIP") != NULL){

        // Try to get a number from a buffer

        if(slre_match("\"([^\"]*)\"", buffer, sizeof(buffer), &cap, 1, 0) > 0){

          // Indicate a receiving call if receivingFlag is not set

          if(!receivingCall){

            // Copy a tel. number into a structures

            strncpy(calls.call.callerTelNum, cap.ptr, cap.len);

            calls.call.callerTelNum[cap.len] = '\0';

            strncpy(notification.caller.callerTelNum, cap.ptr, cap.len);

            notification.caller.callerTelNum[cap.len] = '\0';

            // Indicate a receiving call to a gui_t

            notification.callStarted = 1;

            notification.callEnded = 0;

            notification.newSms = 0;

            chMBPost(&gsmResponse, (msg_t)&notification, TIME_INFINITE);

            receivingCall = TRUE;

          }

          // Refresh time variable

          time = chTimeNow();

        }

      }

      // Check if there is a new sms

      else if(strstr(buffer, "CMTI") != NULL){

        if(slre_match("(\\d+)[^\\d]*$", buffer, sizeof(buffer), &cap, 1, 0) > 0){

          strncpy(smsId, cap.ptr, cap.len);

          // Send command to read an sms

          getSms(smsId);

          // Delete sms with current index

          deleteSms(smsId);

        }

      }

      // Read a sms

      else if(strstr(buffer, "CMGR") != NULL){

        if(slre_match("\"(+[^/:a-zA-Z\"]*)\"", buffer, sizeof(buffer), &cap, 1, 0) > 0){

          // Indicate a new sms to a gui_t thread

          notification.callStarted = 0;

          notification.callEnded = 0;

          notification.newSms = 1;

          // Read sms content in a blocking mode

          sdReadLine(buffer, GSM_RESPONSE_LENGTH, TRUE);

          // Get a message


        }

      }

      // End call if NO CARRIER, BUSY or ERROR messages are received

      else if(strstr(buffer, "NO CARRIER") != NULL || strstr(buffer, "BUSY") != NULL){

        // Indicate that a call has been canceled or that nobody has answered the call

        notification.callStarted = 0;

        notification.callEnded = 1;

        notification.newSms = 0;

        chMBPost(&gsmResponse, (msg_t)&notification, TIME_INFINITE);

      }

      else if(strstr(buffer, "OK") != NULL){

        // Reset receivingCall flag

        receivingCall = FALSE;

      }

    }

    // Check time period between two calling sounds(beep in the phone). If this period is greater than 5 seconds we can assume that the call
    // has ended

    if(receivingCall){

      if(chTimeNow() - time >= 6000){

        // Reset receivingCall flag

        receivingCall = FALSE;

        // End call

        hangUp();

        calls.missedCall = 1;

        calls.getMissedCalls = 0;

        calls.deleteCalls = 0;

        // Send tel number to the calls_t(missed call)

        chMsgSend(calls_thrd_h, (msg_t)&calls);

        // Indicate that a call has ended

        notification.callStarted = 0;

        notification.callEnded = 1;

        notification.newSms = 0;

        chMBPost(&gsmResponse, (msg_t)&notification, TIME_INFINITE);

      }

    }

  }

}

