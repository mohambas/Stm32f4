#ifndef CAMERA_H
#define CAMERA_H

#include "uart.h"

// UART settings
#define UART_INTERFACE UART4
#define UART_BAUDRATE_9600 9600
#define UART_BAUDRATE_19200 19200
#define UART_BAUDRATE_38400 38400
#define UART_BAUDRATE_57600 57600
#define UART_BAUDRATE_115200 115200
#define UART_GPIO GPIOC
#define UART_RX 11
#define UART_TX 10

// Commands
#define SET_PORT 0x24 // Set port
#define SYSTEM_RESET 0x26 // System reset
#define READ_FBUF 0x32 // Read buffer register
#define GET_FBUF_LEN 0x34 // Get image lengths in frame buffer
#define FBUF_CTRL 0x36 // Control frame buffer register
#define DOWNSIZE_CTRL 0x54 // Downsize control

// SET_PORT baudrate values
#define B_9600_H   0xAE
#define B_9600_L   0xC8
#define B_19200_H  0x56
#define B_19200_L  0xE4
#define B_38400_H  0x2A
#define B_38400_L  0xF2
#define B_57600_H  0x1C
#define B_57600_L  0x1C
#define B_115200_H 0x0D
#define B_115200_L 0xA6
// SEL_PORT interface type
#define MCU_UART 1
// READ_BUF, GET_FBUF_LEN FBUF type
#define CURRENT_FRAME 0
#define NEXT_FRAME 1
// READ_BUF control mode
#define MCU_DATA_TRANSFER 0 // I don't understand the datasheet
#define DMA_DATA_TRANSFER 0b1010 // I don't understand the datasheet. Is mode 1010 really DMA mode?
// READ_BUF delays
#define DELAY_001MS 1
#define DELAY_1MS 100
#define DELAY_10MS 1000
// FBUF_CTRL control flags
#define STOP_CURRENT_FRAME 0
#define STOP_NEXT_FRAME 1
#define RESUME_FRAME 2
#define STEP_FRAME 3

// Status codes
#define EXEC_COMMAND_RIGHT 0
#define SYS_NOT_RECV_COMM 1
#define DATA_LEN_ERR 2
#define DATA_FORMAT_ERR 3
#define COMMAND_CANNOT_EXEC 4
#define COMMAND_RECV_EXEC_WRONG 5
#define WRONG_RESPONSE 6

// Response fields offsets
#define PROTOCOL_SIGN_OFFSET 0
#define SERIAL_NUMBER_OFFSET 1
#define COMMAND_OFFSET 2
#define STATUS_OFFSET 3
#define DATA_LENGTH_OFFSET 4
#define DATA_OFFSET 5

// Max response length
#define MAX_RESPONSE_LEN 22
// Response withoud data length
#define MAX_RESPONSE_WITHOUT_DATA 5

// Response structure definition
typedef struct{
    uint8_t status;
    uint8_t responseLen;
    uint8_t* responsePtr;
} RESPONSE;

void initCameraUart(void);
void sendCommand(uint8_t command, uint8_t* data, uint8_t dataLen);
void readNBytes(uint8_t *buffer, uint32_t numOfBytes);
RESPONSE setBaudRate(uint8_t* mode);
RESPONSE systemReset(void);
RESPONSE readFbuf(uint8_t* mode);
RESPONSE getFbufLen(uint8_t* mode);
RESPONSE fbufControl(uint8_t* mode);
RESPONSE setDownsize(uint8_t* mode);

#endif
