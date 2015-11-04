#include "main.h"

// Vars
pjpeg_image_info_t imageInfo;
RESPONSE response;
uint8_t mode[MAX_MODE_LEN];
uint8_t MCURow, MCUCol, MCURowX, MCUColY;

void main(){

    // Init LCD
    LCD_Init();

    // Clear the screen
    LCD_Clear(LCD_COLOR_WHITE);

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
    /*mode[0] = 5;
    response = setDownsize(mode);
    if(response.status != EXEC_COMMAND_RIGHT)
        for(;;); // Error*/


    for(;;){

        // Stop a current frame
        mode[0] = STOP_CURRENT_FRAME;     
        response = fbufControl(mode);
        if(response.status != EXEC_COMMAND_RIGHT)
            for(;;); // Error

        // Init JPEG decoder
        if(pjpeg_decode_init(&imageInfo, readPictureBytes, NULL, 0) != 0)
            for(;;); // Error

        // Draw image    
        while(pjpeg_decode_mcu() != PJPG_NO_MORE_BLOCKS){
       
            MCUColY = 0;

            // Draw block
            while(MCUColY < imageInfo.m_MCUHeight){
                while(MCURowX < imageInfo.m_MCUWidth){
                    Pixel(240 - MCUCol * imageInfo.m_MCUHeight + MCUColY, MCURow * imageInfo.m_MCUWidth + MCURowX,ASSEMBLE_RGB(*(imageInfo.m_pMCUBufR + (MCUColY * imageInfo.m_MCUWidth + MCURowX)),*(imageInfo.m_pMCUBufG + (MCUColY * imageInfo.m_MCUWidth + MCURowX)),*(imageInfo.m_pMCUBufB + (MCUColY * imageInfo.m_MCUWidth + MCURowX))));
                    MCURowX++;
                }
                MCURowX = 0;
                MCUColY++;           
            }

            if(MCURow == imageInfo.m_MCUSPerRow){
                MCURow = 0;
                MCUCol++;            
            }
            else
                MCURow++;
    
        }

        MCURow = MCUCol = 0;

        // Clear the screen
        LCD_Clear(LCD_COLOR_WHITE);

        // Resume a frame
        mode[0] = RESUME_FRAME;     
        response = fbufControl(mode);
        if(response.status != EXEC_COMMAND_RIGHT)
            for(;;); // Error
        Delay(1000);
    }

}

unsigned char readPictureBytes(unsigned char* pBuf, unsigned char buf_size, unsigned char *pBytes_actually_read, void *pCallback_data){

    static uint32_t picLen;
    static uint32_t picAddr;
    uint16_t bytesToRead;

    if(picLen == 0){
        // Reset picAddr
        picAddr = 0;
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
    }

    // Read a picture data
    mode[0] = CURRENT_FRAME;
    mode[1] = DMA_DATA_TRANSFER;
    bytesToRead = MIN(buf_size - (buf_size % 8), picLen);
    mode[2] = picAddr >> 24;mode[3] = (picAddr >> 16) & 0xFF;
    mode[4] = (picAddr >> 8) & 0xFF; mode[5] = picAddr & 0xFF;
    mode[6] = bytesToRead >> 24;mode[7] = (bytesToRead >> 16) & 0xFF;
    mode[8] = (bytesToRead >> 8) & 0xFF; mode[9] = bytesToRead & 0xFF;
    mode[10] = (DELAY_001MS >> 8);mode[11] = DELAY_001MS & 0xFF;
    readFbuf(mode);
    if(response.status != EXEC_COMMAND_RIGHT)
        for(;;); // Error
    // Read N bytes of a picture data
    readNBytes(pBuf, bytesToRead);
    // Read last five bytes which correspond to response(dummy read)
    readNBytes(NULL, 5);
    
    picLen -= bytesToRead;
    picAddr +=bytesToRead;
    // Number of bytes actually read
    *pBytes_actually_read = bytesToRead;

    return 0;

}


