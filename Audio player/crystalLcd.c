#include "crystalLcd.h"

void initLcdPeriph(void){

    GPIO_InitTypeDef GPIO_InitStruct; // GPIO struct
    I2C_InitTypeDef I2C_InitStruct; // I2C Struct
  
    // Omogoèi uro za GPIOx

  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  
    // Init GPIOx 

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // Open drain(tako pise da mora bit)
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // Pull up(tako pise da mora bit)
    
    GPIO_Init(GPIOB, &GPIO_InitStruct);

      
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2); // SCL
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2); // SDA
      
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); // Omogoèi uro na za I2C2
    
      
    // Init I2C 
    
      
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C; // I2C mode... Zgleda da jih je vec
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // pojma nimam kaj je to... Baje je 50% standard XD
    I2C_InitStruct.I2C_OwnAddress1 = 0x00; // Vrjetno samo kadar je Slave??
    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable; // Onemogocene ACK... Rabimo pri Read... Mi pisemo
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // naslovi so 7 bitni
    I2C_Init(I2C2, &I2C_InitStruct);
    I2C_Cmd(I2C2, ENABLE);
    
    // Delay more than 100ms.. Look into specs
    
    delay(1000);

}

void initLcd(void){

  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)); // Pogledamo ce je naprava zasedena
  
  // Posljemo najprej ukaze 0x03 in to 3x
    
  sendCmd(0x03); // To je za resetirat display
    
  sendCmd(0x03); // To je za resetirat display
    
  sendCmd(0x03); // To je za resetirat display
    
  sendCmd(0x02); // 4 bit mode...function set brez nic...samo 4 bit mode
    
  sendCmd(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE); // Ponovimo functionset se 1x in dolocimo vse kar rabi
   
  sendCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
    
  sendCmd(LCD_CLEARDISPLAY);
   
  sendCmd(LCD_ENTRYMODESET | LCD_ENTRYLEFT);


}

void startI2C(void){

  I2C_GenerateSTART(I2C2, ENABLE); // Generate start bit
    
  // Wait until slave gets start bit
    
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send address

  I2C_Send7bitAddress(I2C2, ADDR, I2C_Direction_Transmitter);
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

}

void stopI2C(void){

  // Generate stop bit
  
  I2C_GenerateSTOP(I2C2, ENABLE);

}

void sendCmd(uint8_t cmd){
  
  startI2C();

  // Send first 4 bits
  
  I2C_SendData(I2C2, (cmd & 0xF0) | LCD_BACKLIGHT | EN);
  
  // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  I2C_SendData(I2C2, ((cmd & 0xF0) & ~EN) | LCD_BACKLIGHT); // Enable pin set to 0
  
  // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  
  // Send second 4 bits
  
  I2C_SendData(I2C2, ((cmd << 4) & 0xF0) | LCD_BACKLIGHT | EN);
  
  // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  I2C_SendData(I2C2, (((cmd << 4) & 0xF0) & ~EN) | LCD_BACKLIGHT);
  
  // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  stopI2C();


}

void sendChar(uint8_t chr){
  
  startI2C();

  // Send first 4 bits
  
  I2C_SendData(I2C2, (chr & 0xF0) | LCD_BACKLIGHT | EN | RS);
  
  // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  I2C_SendData(I2C2, ((chr & 0xF0) & ~EN) | LCD_BACKLIGHT);
  
  delay(10);
  
  
  // Poslji druge 4 bite
  
  I2C_SendData(I2C2, ((chr << 4) & 0xF0) | LCD_BACKLIGHT | EN | RS);
  
 // Wait for flag that data are sent
  
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  delay(10);
  
  I2C_SendData(I2C2, (((chr << 4) & 0xF0) & ~EN) | LCD_BACKLIGHT);
  
  delay(10);
  
  stopI2C();

}

void sendCharOn(uint8_t chr, uint8_t row, uint8_t col){
  
  // Preveri, ce so koordinate v dovoljenem dosegu
  
  if(row >= 1 && row <= 4 & col >= 1 && col <= 20){
    
    // Preveri vrstico in zapisi crko na pravilno mesto
    
    if(row == 2){
      
      sendCmd(LCD_SETDDRAMADDR | 0x40 + col - 1);
      
      sendChar(chr);
      
    }
    else if(row == 3){
      
      sendCmd(LCD_SETDDRAMADDR | 0x14 + col - 1);
      
      sendChar(chr);
      
    }
    else if(row == 4){
      
      sendCmd(LCD_SETDDRAMADDR | 0x54 + col - 1);
      
      sendChar(chr);
      
    }
    else{
      
      sendCmd(LCD_SETDDRAMADDR | col - 1);
      
      sendChar(chr);
      
    }
    
    
  }

    

}

void setCursor(uint8_t row, uint8_t col){
  
   // Preveri, ce so koordinate v dovoljenem dosegu
  
  if(row >= 1 && row <= 4 & col >= 1 && col <= 20){
    
    // Preveri vrstico in zapisi crko na pravilno mesto
    
    if(row == 2){
      
      sendCmd(LCD_SETDDRAMADDR | 0x40 + col - 1);
    }
    
    else if(row == 3){
      
      sendCmd(LCD_SETDDRAMADDR | 0x14 + col - 1);
    }
    
    else if(row == 4){
      
      sendCmd(LCD_SETDDRAMADDR | 0x54 + col - 1);
    }
    
    else{
      
      sendCmd(LCD_SETDDRAMADDR | col - 1);
    }
    
    
  }

}

void writeMode(MODE mode){
  
  uint8_t i;

  // Clear first row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',1, i);
  
  }
  
  // mode
  
  sendCharOn('M',1, 1);
  sendCharOn('o',1, 2);
  sendCharOn('d',1, 3);
  sendCharOn('e',1, 4);
  sendCharOn(':',1, 5);
  
  if(mode == PLAYER){
  
    sendCharOn('M',1, 6);
    sendCharOn('u',1, 7);
    sendCharOn('s',1, 8);
    sendCharOn('i',1, 9);
    sendCharOn('c',1, 10);
    sendCharOn(' ',1, 11);
    sendCharOn('c',1, 12);
    sendCharOn('o',1, 13);
    sendCharOn('n',1, 14);
    sendCharOn('t',1, 15);
    sendCharOn('r',1, 16);
    sendCharOn('o',1, 17);
    sendCharOn('l',1, 18);
  
  }
  else if(mode == VOLUME){
  
    sendCharOn('V',1, 6);
    sendCharOn('o',1, 7);
    sendCharOn('l',1, 8);
    sendCharOn('u',1, 9);
    sendCharOn('m',1, 10);
    sendCharOn('e',1, 11);
    sendCharOn(' ',1, 12);
    sendCharOn('c',1, 13);
    sendCharOn('o',1, 14);
    sendCharOn('n',1, 15);
    sendCharOn('t',1, 16);
    sendCharOn('r',1, 17);
    sendCharOn('o',1, 18);
    sendCharOn('l',1, 19);
  
  }
  else if(mode == TREBLE){
  
    sendCharOn('T',1, 6);
    sendCharOn('r',1, 7);
    sendCharOn('e',1, 8);
    sendCharOn('b',1, 9);
    sendCharOn('l',1, 10);
    sendCharOn('e',1, 11);
    sendCharOn(' ',1, 12);
    sendCharOn('c',1, 13);
    sendCharOn('o',1, 14);
    sendCharOn('n',1, 15);
    sendCharOn('t',1, 16);
    sendCharOn('r',1, 17);
    sendCharOn('o',1, 18);
    sendCharOn('l',1, 19);

  }
  else{

    sendCharOn('B',1, 6);
    sendCharOn('a',1, 7);
    sendCharOn('s',1, 8);
    sendCharOn('s',1, 9);
    sendCharOn(' ',1, 10);
    sendCharOn('c',1, 11);
    sendCharOn('o',1, 12);
    sendCharOn('n',1, 13);
    sendCharOn('t',1, 14);
    sendCharOn('r',1, 15);
    sendCharOn('o',1, 16);
    sendCharOn('l',1, 17);


  }
}

void writeVolIncr(){

   uint8_t i;
  
  // Volume++
  
  sendCharOn('V',4, 1);
  sendCharOn('o',4, 2);
  sendCharOn('l',4, 3);
  sendCharOn('u',4, 4);
  sendCharOn('m',4, 5);
  sendCharOn('e',4, 6);
  sendCharOn('+',4, 7);
  sendCharOn('+',4, 8);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writeVolDcrs(){

   uint8_t i;
  
  // Volume++
  
  sendCharOn('V',4, 1);
  sendCharOn('o',4, 2);
  sendCharOn('l',4, 3);
  sendCharOn('u',4, 4);
  sendCharOn('m',4, 5);
  sendCharOn('e',4, 6);
  sendCharOn('-',4, 7);
  sendCharOn('-',4, 8);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writeTrebIncr(){

  uint8_t i;
  
  // Treble++
  
  sendCharOn('T',4, 1);
  sendCharOn('r',4, 2);
  sendCharOn('e',4, 3);
  sendCharOn('b',4, 4);
  sendCharOn('l',4, 5);
  sendCharOn('e',4, 6);
  sendCharOn('+',4, 7);
  sendCharOn('+',4, 8);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  

}

void writeTrebDcrs(){

  uint8_t i;
  
  // Treble--
  
  sendCharOn('T',4, 1);
  sendCharOn('r',4, 2);
  sendCharOn('e',4, 3);
  sendCharOn('b',4, 4);
  sendCharOn('l',4, 5);
  sendCharOn('e',4, 6);
  sendCharOn('-',4, 7);
  sendCharOn('-',4, 8);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  

}

void writeBassIncr(){

  uint8_t i;
  
  // Bass++
  
  sendCharOn('B',4, 1);
  sendCharOn('a',4, 2);
  sendCharOn('s',4, 3);
  sendCharOn('s',4, 4);
  sendCharOn('+',4, 5);
  sendCharOn('+',4, 6);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  

}

void writeBassDcrs(){

  uint8_t i;
  
  // Bass--
  
  sendCharOn('B',4, 1);
  sendCharOn('a',4, 2);
  sendCharOn('s',4, 3);
  sendCharOn('s',4, 4);
  sendCharOn('-',4, 5);
  sendCharOn('-',4, 6);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  

}

void writePlay(void){

   uint8_t i;
  
  sendCharOn('P',4, 1);
  sendCharOn('l',4, 2);
  sendCharOn('a',4, 3);
  sendCharOn('y',4, 4);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writePause(void){

   uint8_t i;
  
  sendCharOn('P',4, 1);
  sendCharOn('a',4, 2);
  sendCharOn('u',4, 3);
  sendCharOn('s',4, 4);
  sendCharOn('e',4, 5);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writeStop(void){

   uint8_t i;
  
  sendCharOn('S',4, 1);
  sendCharOn('t',4, 2);
  sendCharOn('o',4, 3);
  sendCharOn('p',4, 4);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writeNext(void){

   uint8_t i;
  
  sendCharOn('N',4, 1);
  sendCharOn('e',4, 2);
  sendCharOn('x',4, 3);
  sendCharOn('t',4, 4);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void writePrev(void){

   uint8_t i;
  
  sendCharOn('P',4, 1);
  sendCharOn('r',4, 2);
  sendCharOn('e',4, 3);
  sendCharOn('v',4, 4);
  
  // Delay
  
  delay(0xfff);
  
  // Clear last row
  
  for(i = 1; i <= 20; i++){
  
    sendCharOn(' ',4, i);
  
  }
  
}

void delay(uint32_t ms) {
  
  ms *= 3360;
  
  while(ms--){
    
    __NOP();
    
  }
  
}