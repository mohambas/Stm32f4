#include "camera.h"

// Static function declarations
static uint8_t readNextByte(void);
static void sendNextByte(uint8_t byte);
static uint8_t checkResponse(uint8_t* response, uint8_t command);
static uint8_t getProtocolSign(uint8_t* response);
static uint8_t getSerialNum(uint8_t* response);
static uint8_t getCommand(uint8_t* response);
static uint8_t getStatus(uint8_t* response);
static uint8_t getLength(uint8_t* response);
static uint8_t* getData(uint8_t* response);

static uint8_t serialNumber;
static uint8_t responseBuffer[MAX_RESPONSE_LEN]; // Command response buffer

void initCameraUart(void){

    initUart(UART_GPIO, UART_INTERFACE, UART_RX, UART_TX, UART_BAUDRATE_38400);

}

void sendCommand(uint8_t command, uint8_t* data, uint8_t dataLen){

    uint8_t dataCount = 0, dataRespLen = 0;

    // Send a protocol sign
    sendNextByte(0x56);
    // Send a serial number
    sendNextByte(serialNumber);
    // Send a command
    sendNextByte(command);
    // Send a data length
    sendNextByte(dataLen);
    // Send data
    while(dataLen > 0){
        sendNextByte(*data++);
        dataLen--;
    }

    // Read a response
    while(dataCount < dataRespLen + MAX_RESPONSE_WITHOUT_DATA){
        // Read a byte and save it in responseBuffer
        responseBuffer[dataCount] = readNextByte();
        // Get a data length if is is equals 4
        if(dataCount == 4)
            dataRespLen = responseBuffer[dataCount];
        dataCount++;
    }
}

RESPONSE setBaudRate(uint8_t* mode){

    RESPONSE response;

    // Send command and save response in responseBuffer
    sendCommand(SET_PORT, mode, 3);
    // Check if a received response is valid
    if(checkResponse(responseBuffer, SET_PORT) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = 0;
        // Set a data pointer
        response.responsePtr = NULL;
        // Reinit a camera with baudrate 115200
        initUart(UART_GPIO, UART_INTERFACE, UART_RX, UART_TX, UART_BAUDRATE_115200);
    }
    else
        response.status = WRONG_RESPONSE;

    return response;

}

RESPONSE systemReset(void){

    RESPONSE response;

    // Send a command and save response in responseBuffer
    sendCommand(SYSTEM_RESET, NULL, 0);
    // Check if received response is valid
    if(checkResponse(responseBuffer, SYSTEM_RESET) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = 0;
        // Set a data pointer
        response.responsePtr = NULL;
    }
    else
        response.status = WRONG_RESPONSE;

    return response;

}

RESPONSE readFbuf(uint8_t* mode){

    RESPONSE response;

    // Send a command and save response in responseBuffer
    sendCommand(READ_FBUF, mode, 12);
    // Check if received response is valid
    if(checkResponse(responseBuffer, READ_FBUF) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = 0;
        // Set a data pointer
        response.responsePtr = NULL;
    }
    else
        response.status = WRONG_RESPONSE;

    return response;
    
}

RESPONSE getFbufLen(uint8_t* mode){

    RESPONSE response;

    // Send a command and save response in responseBuffer
    sendCommand(GET_FBUF_LEN, mode, 1);
    // Check if received response is valid
    if(checkResponse(responseBuffer, GET_FBUF_LEN) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = getLength(responseBuffer);
        // Set a data pointer
        response.responsePtr = getData(responseBuffer);
    }
    else
        response.status = WRONG_RESPONSE;

    return response;

}

RESPONSE fbufControl(uint8_t* mode){

    RESPONSE response;

    // Send command and save response in responseBuffer
    sendCommand(FBUF_CTRL, mode, 1);
    // Check if received response is valid
    if(checkResponse(responseBuffer, FBUF_CTRL) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = 0;
        // Set a data pointer
        response.responsePtr = NULL;
    }
    else
        response.status = WRONG_RESPONSE;

    return response;

}

RESPONSE setDownsize(uint8_t* mode){

    RESPONSE response;

    // Send a command and save response in responseBuffer
    sendCommand(DOWNSIZE_CTRL, mode, 1);
    // Check if received response is valid
    if(checkResponse(responseBuffer, DOWNSIZE_CTRL) == 1){
        // Set a flag
        response.status = getStatus(responseBuffer);
        // Set a data length
        response.responseLen = 0;
        // Set a data pointer
        response.responsePtr = NULL;
    }
    else
        response.status = WRONG_RESPONSE;

    return response;

}

void readNBytes(uint8_t *buffer, uint32_t numOfBytes){

    uint32_t i = 0;

    while(i < numOfBytes){
        if(buffer != NULL)
            buffer[i++] = readNextByte();
        else{
            // Dummy read
            readNextByte();
            i++;        
        }
    }  

}

static uint8_t readNextByte(void){

    while(flagStatus(UART_INTERFACE, USART_SR_RXNE) != 1);
    // Read a byte
    return readByte(UART_INTERFACE);

}

static void sendNextByte(uint8_t byte){

    while(flagStatus(UART_INTERFACE, USART_SR_TXE) != 1);
    sendByte(UART_INTERFACE, byte);

}

static uint8_t checkResponse(uint8_t* response, uint8_t command){

    if(getProtocolSign(response) == 0x76 && getSerialNum(response) == serialNumber && getCommand(response) == command
    && getStatus(response) >= EXEC_COMMAND_RIGHT && getStatus(response) <= COMMAND_RECV_EXEC_WRONG && getLength(response) >= 0)
        return 1;
    return 0;

}

static uint8_t getProtocolSign(uint8_t* response){
    return response[PROTOCOL_SIGN_OFFSET];
}

static uint8_t getSerialNum(uint8_t* response){
    return response[SERIAL_NUMBER_OFFSET];
}

static uint8_t getCommand(uint8_t* response){
    return response[COMMAND_OFFSET];
}

static uint8_t getStatus(uint8_t* response){
    return response[STATUS_OFFSET];
}

static uint8_t getLength(uint8_t* response){
    return response[DATA_LENGTH_OFFSET];
}

static uint8_t* getData(uint8_t* response){
    return &response[DATA_OFFSET];
}
