#include "codec.h"


// Vars

uint8_t CODEC_VOLUME = 0;

uint8_t CODEC_TREBLE = 10;

uint8_t CODEC_BASS = 10;

// Global DMA structure(for play())

DMA_InitTypeDef DMA_InitStruct;


void initCodecPeriph(){
  
  GPIO_InitTypeDef GPIO_InitStruct;

  I2S_InitTypeDef I2S_InitStruct;

  I2C_InitTypeDef I2C_InitStruct;
        
  NVIC_InitTypeDef NVIC_InitStruct;
        
  // Enable GPIOB clock
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
        
  // Enable GPIOA clock
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        
  // Enable GPIOC clock
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        
  // Enable GPIOD clock
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
        
  // Enable I2C1 Clock
        
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        
  // Enable SPI3(I2S3) clock
        
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        
  // Enable DMA1 clock
        
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        
        
  RCC_PLLI2SCmd(ENABLE); // Enable PLL module to provide accurate standard audio sampling frequencies
        
  // Init GPIO for CS43L22 reset pin
        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4; // Reset pin.. Negiran.. 0 -> Disabled, 1 - > Enabled
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; 
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStruct);
        
  // Init GPIO for I2C1(Control line for CS43L22 DAC with Ampilifer)
        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; // SCL, SDA
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // Alternate function  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD; // ?
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;; // ?
  GPIO_Init(GPIOB, &GPIO_InitStruct);
        
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
        
  // init GPIO for SPI3 - I2S
        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_12; //  MCLK( ?? ), SCLK( continuous serial clock ), SDIN( Serial data line.. Transmitted MSB first..)
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // Alternate function  
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // ?
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ?
  GPIO_Init(GPIOC, &GPIO_InitStruct);
        
  // init GPIO for SPI3 - I2S ->> WS
        
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4; //  WS( Word select ).. 0 -> Left channel... 1 -> Right channel
  GPIO_Init(GPIOA, &GPIO_InitStruct);
        
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
        
  // DMA struct init for I2S
        
  DMA_InitStruct.DMA_Channel = DMA_Channel_0; // Channel 0(i2s3 - spi3 tx)
  DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transfer from memory to I2S
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(SPI3->DR); // SPI3 Data register
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)0; // This will be set when play() function is called. Copy data from this location
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // Don't increment the peripheral 'memory
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // Increment the memory location
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 16b size transfer
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 16b size transfer
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal; // Normal mode. Not repeating
  DMA_InitStruct.DMA_BufferSize = 0; // This will be set when play() function is called. Number of data transfered. bufferSize * 16b
  DMA_InitStruct.DMA_Priority = DMA_Priority_High; // High priority... ???
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single; // No bursts
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // no bursts
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable; // FIFO mode disabled(direct mode)
  DMA_Init(DMA1_Stream7, &DMA_InitStruct);
        
  DMA_ITConfig(DMA1_Stream7, DMA_IT_TC, ENABLE); // Enable the transfer complete interrupt for DMA1 stream 7
             
  //Nvic init
        
  NVIC_InitStruct.NVIC_IRQChannel = DMA1_Stream7_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct); 

  // Init struct for I2C1
        
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_ClockSpeed = 100000;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle... Standard setting... http://arduino.cc/en/Tutorial/PWM
  I2C_InitStruct.I2C_OwnAddress1 = 0x33; // master mode? We don't need address
  I2C_InitStruct.I2C_Ack = I2C_Ack_Disable; // Disabled ack? We need when we read from slave ?
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // Address length 7 bit
  I2C_Init(I2C1, &I2C_InitStruct);
  I2C_Cmd(I2C1, ENABLE);
        
  // Init struct for SPI3(I2S3)
        
  I2S_InitStruct.I2S_Mode = I2S_Mode_MasterTx; // Transmit mode
  I2S_InitStruct.I2S_Standard = I2S_Standard_Phillips; // Phillips standard
  I2S_InitStruct.I2S_DataFormat = I2S_DataFormat_16b; // 16b samples
  I2S_InitStruct.I2S_MCLKOutput = I2S_MCLKOutput_Enable; // MCLK output enabled
  I2S_InitStruct.I2S_AudioFreq = I2S_AudioFreq_44k; // 44100 Hz
  I2S_InitStruct.I2S_CPOL = SPI_CPOL_Low; // In steady state is 0
  I2S_Init(SPI3, &I2S_InitStruct);
  I2S_Cmd(SPI3, ENABLE);
             
        
}

void initCodec(){

  
  uint8_t regValue;
  
  // Set CS43L22 reset to 0.. > Power on sequence begins... CS43L22_F2.pdf page 31
  
  GPIO_SetBits(GPIOD, GPIO_Pin_4);
   
  // Delay

  delayCodec(1000); // Delay funtion
  
  // Set the “Power Ctl 1” register (0x02) to 0x01. -> Keep codec disabled
  
  sendCmdDac(0x02, 0x01); // Write into Reg 0x02 Value 0x01... Keep codec off
  
  // Page 32... Initialization sequence
  
  sendCmdDac(0x00, 0x99); // Write into Reg 0x00 Value 0x99...
  
  sendCmdDac(0x47, 0x80); // Write into Reg 0x47 Value 0x80...
  
  regValue = readRegDac(0x32); // Read reg 0x32 value.
  
  sendCmdDac(0x32, regValue | 0x80); // Set bit 7 to 1
  
  regValue = readRegDac(0x32); // Read reg 0x32 value.
  
  sendCmdDac(0x32, regValue & (~0x80)); // Set bit 7 to 0
  
  sendCmdDac(0x00, 0x00); // Write into Reg 0x00 Value 0x00...
  
  // End initialization sequence
  
  // Set other stuff
  
  /* Look pages 25 and 1 and 24 and 21 */
  
  sendCmdDac(0x04, 0xAF); // Write into reg 0x04 Value 0xAF(Speaker channels A and B are off(xxxx1111 - > Speaker ampilifer is off... Headphone channels A and B are always on(1010xxxx)
  
  sendCmdDac(0x05, 0x81); // Write into Reg 0x05 Value 0x81(MCLK freq. auto detect...Sampling frequency - 44100 for example)
  
  sendCmdDac(0x06, 0x07); // Write into reg 0x06 Value 0x07(Slave mode -> Input only, Dac interface format I2S, audio word length 16b)
  
  // Sets the volume/gain of the PCM data from the serial datainput (SDIN) to the DSP. 
  
  sendCmdDac(0x1A, 0x00); // Write into reg 0x1A Value 0x00.. Set PCM volume channel A
  
  sendCmdDac(0x1B, 0x00); // Write into reg 0x1B Value 0x00.. Set PCM volume channel B
  
  // Sets the volume of the signal out the DSP.
  
  sendCmdDac(0x20, 0x00); // Write into reg 0x20 Value 0x00.. Set DSP putput volume channel A
  
  sendCmdDac(0x21, 0x00); // Write into reg 0x21 Value 0x00.. Set DSP putput volume channel B
  
  // Sets the volume of the signal out the DAC. Use this for regulating volume!
  
  sendCmdDac(0x22, CODEC_VOLUME); // Write into reg 0x22 Value 0x00.. Set DAC output channel A volume
  
  sendCmdDac(0x23, CODEC_VOLUME); // Write into reg 0x23 Value 0x00.. Set DAC output channel B volume
  
  // Sets treble and bass levels
  
  sendCmdDac(0x1E, 0x01);
  
  sendCmdDac(0x1F, CODEC_TREBLE << 4 | CODEC_BASS); // Adjust bass and treble levels..
  
  
  // Additional configuations to the codec. Configurations are done to shut down codec properly(if not configured it results in high noise after shut down)
  
  sendCmdDac(0x0A,0x00); // Disable the analog soft ramp... Wtf is this shit??
  
  sendCmdDac(0x27,0x00); // Disable the limiter attack level... Wtf is this shit ??
  
  // Set the “Power Ctl 1” register (0x02) to 0x9E. -> Enable codec
 
  sendCmdDac(0x02, 0x9E); // Write into Reg 0x02 Value 0x9E... Power on the codec


}

void sendCmdDac(uint8_t reg, uint8_t data){
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Pogledamo ce je naprava zasedena

  // Send start bit
  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  // Wait for I2C_EVENT_MASTER_MODE_SELECT flag that indicates that start bit was properly released to the line
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send 7 bit address

  I2C_Send7bitAddress(I2C1, CODEC_ADDR, I2C_Direction_Transmitter);
  
  // Wait for I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED flag that indicates that slave acknowledged his address
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  // Send reg(data) to the slave
  
  I2C_SendData(I2C1, reg); 
  
  // Wait until data transmitted to the line flag I2C_EVENT_MASTER_BYTE_TRANSMITTED was set
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  // Send data(into reg) to the slave
  
  I2C_SendData(I2C1, data); 
  
  // Wait until data transmitted to the line flag I2C_EVENT_MASTER_BYTE_TRANSMITTED was set
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  // Send stop bit
  
  I2C_GenerateSTOP(I2C1, ENABLE);

}

uint8_t readRegDac(uint8_t reg){
  
  uint8_t recv;
  
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Pogledamo ce je naprava zasedena
  
  // Send reg. address to the CS43L22
  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  // Wait for I2C_EVENT_MASTER_MODE_SELECT flag which indicates that start bit was properly released to the line
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send 7 bit address

  I2C_Send7bitAddress(I2C1, CODEC_ADDR, I2C_Direction_Transmitter);
  
  // Wait for I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED flag which indicates that slave acknowledged his address
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  // Send reg(data) to the slave
  
  I2C_SendData(I2C1, reg); 
  
  // Wait until data transmitted to the line flag I2C_EVENT_MASTER_BYTE_TRANSMITTED was set
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  // Send stop bit
  
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  // Wait until device is not busy anymore
  
  while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
  
  // READ DATA FROM REG
  
  I2C_AcknowledgeConfig(I2C1, DISABLE); // Disable ack because we read just one byte

  // Send start bit
  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  // Wait for I2C_EVENT_MASTER_MODE_SELECT flag that indicates that start bit was properly released to the line
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  // Send 7 bit address

  I2C_Send7bitAddress(I2C1, CODEC_ADDR, I2C_Direction_Receiver);
  
  // Wait for I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED flag that indicates that slave acknowledged his address
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  // wait until byte arrived
  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        
  
  // Read data
        
  recv = I2C_ReceiveData(I2C1);
  
  // Send stop bit
  
  I2C_GenerateSTOP(I2C1, ENABLE);
  
  return recv;

}

void sendAudioData(int16_t sample){
  
  while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
  
  SPI_I2S_SendData(SPI3, sample); // R and L channels switching(pin WS)
  

}

void increaseTreble(){

  if(CODEC_TREBLE > 0x05) // If treble is between -10.5 db and +12
  {
  
    CODEC_TREBLE--;
    
    sendCmdDac(0x1F, CODEC_TREBLE << 4 | CODEC_BASS);
    
  }

}

void decreaseTreble(){

  if(CODEC_TREBLE < 0xf) // If treble is between -10.5 db and +12
  {
  
    CODEC_TREBLE++;
    
    sendCmdDac(0x1F, CODEC_TREBLE << 4 | CODEC_BASS);
    
  }

}

void increaseBass(){

  if(CODEC_BASS > 0x05) // If bass is between -10.5 db and +12
  {
  
    CODEC_BASS--;
    
    sendCmdDac(0x1F, CODEC_TREBLE << 4 | CODEC_BASS);
    
  }

}

void decreaseBass(){

  if(CODEC_BASS < 0xf) // If bass is between -10.5 db and +12
  {
  
    CODEC_BASS++;
    
    sendCmdDac(0x1F, CODEC_TREBLE << 4 | CODEC_BASS);
    
  }

}

void increaseVolume(){

 if(CODEC_VOLUME == 0xFF)
 {
 
   CODEC_VOLUME = 0x00;
   
 }
 else if(CODEC_VOLUME > 0x01 && CODEC_VOLUME < 0xFF){
 
   CODEC_VOLUME++;
 
 }
 
 sendCmdDac(0x22, CODEC_VOLUME); // Set DAC output channel A volume
  
 sendCmdDac(0x23, CODEC_VOLUME); // Set DAC output channel B volume
  

}

void decreaseVolume(){

  
  if(CODEC_VOLUME == 0x00){
  
    CODEC_VOLUME = 0xFF;
    
  }
  else if(CODEC_VOLUME > 0x01){
  
    CODEC_VOLUME--;
  
  }
  
  sendCmdDac(0x22, CODEC_VOLUME); // Set DAC output channel A volume
  
  sendCmdDac(0x23, CODEC_VOLUME); // Set DAC output channel B volume
  

}

void play(const uint16_t * buffer, uint32_t bufferSize){
  
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)buffer;
  DMA_InitStruct.DMA_BufferSize = bufferSize;
  DMA_Init(DMA1_Stream7, &DMA_InitStruct);
  
  // Enable spi dma interface
       
  SPI_I2S_DMACmd(SPI3, SPI_I2S_DMAReq_Tx, ENABLE);
  
  DMA_Cmd(DMA1_Stream7, ENABLE); // Enable DMA
  

}

void pauseResume(uint8_t pause){

  if(pause == PAUSESONG){
  
    // Disable codec
    
    sendCmdDac(0x02, 0x01);
  
  }
  else{
  
    // Enable codec
    
    sendCmdDac(0x02, 0x9E);
    
  
  }
  
  
}


// Interrupt handler

void DMA1_Stream7_IRQHandler(){

   if (DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7))
   {
     
     needAnotherChunk = 1;
     
     DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TCIF7);
     
   }


}

void resetCodec(void){

  // Reset codec
      
  GPIO_ResetBits(GPIOD, GPIO_Pin_4);
  
}

void delayCodec(uint32_t ms) {
  
  ms *= 3360;
  
  while(ms--){
    
    __NOP();
    
  }
  
}