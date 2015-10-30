#include "main.h"

// Vars
pjpeg_image_info_t imageInfo;
RESPONSE response;
uint32_t picLen;
uint32_t picAddr;
uint8_t mode[MAX_MODE_LEN];
uint8_t picBuffer[MAX_PIC_BUFF_LEN];

void main(){

    // Init camera uart
    initCameraUart();

    // Set baudrate to 115200
    mode[0] = MCU_UART;
    mode[1] = B_115200_H;
    mode[2] = B_115200_L;
    response = setBaudRate(mode);
    if(response.status != EXEC_COMMAND_RIGHT)
        for(;;); // Error  

    // Set the resolution by means of a downsize option. Zoom out height and width by 1/2
    mode[0] = 5;
    response = setDownsize(mode);
    if(response.status != EXEC_COMMAND_RIGHT)
        for(;;); // Error


    for(;;){

        // Stop a current frame
        mode[0] = STOP_CURRENT_FRAME;     
        response = fbufControl(mode);
        if(response.status != EXEC_COMMAND_RIGHT)
            for(;;); // Error

        // Get a picture length
        mode[0] = CURRENT_FRAME;
        response = getFbufLen(mode);
        if(response.status != EXEC_COMMAND_RIGHT)
            for(;;); // Error
        picLen = *response.responsePtr++;
        picLen <<= 8;
        picLen |= *response.responsePtr++;
        picLen <<= 8;
        picLen |= *response.responsePtr++;
        picLen <<= 8;
        picLen |= *response.responsePtr++;

        // Read a picture data
        uint16_t bytesToRead;
        mode[0] = CURRENT_FRAME;
        mode[1] = DMA_DATA_TRANSFER;
        while(picLen > 0){
            bytesToRead = MIN(MAX_PIC_BUFF_LEN, picLen);
            mode[2] = picAddr >> 24;mode[3] = (picAddr >> 16) & 0xFF;
            mode[4] = (picAddr >> 8) & 0xFF; mode[5] = picAddr & 0xFF;
            mode[6] = bytesToRead >> 24;mode[7] = (bytesToRead >> 16) & 0xFF;
            mode[8] = (bytesToRead >> 8) & 0xFF; mode[9] = bytesToRead & 0xFF;
            mode[10] = (DELAY_001MS >> 8);mode[11] = DELAY_001MS & 0xFF;
            readFbuf(mode);
            if(response.status != EXEC_COMMAND_RIGHT)
                for(;;); // Error
            // Read N bytes of a picture data
            readNBytes(picBuffer, bytesToRead);
            // Read last five bytes which correspond to response(dummy read)
            readNBytes(NULL, 5);
            picLen -= bytesToRead;
            picAddr +=bytesToRead;
            // Process a picture data
        }

        // Reset the picAddr variable to 0
        picAddr = 0;

        // Resume a frame
        mode[0] = RESUME_FRAME;     
        response = fbufControl(mode);
        if(response.status != EXEC_COMMAND_RIGHT)
            for(;;); // Error
    }

}




