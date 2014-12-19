#include "main.h"

// vars

uint16_t audio_sample_buffer_one[NUMOFSAMPLES]; // Buffer one

uint16_t audio_sample_buffer_two[NUMOFSAMPLES]; // Buffer two

long numberOfSongs; // Number of songs

short buffer1Length, buffer2Length ; // Number of samples

uint8_t volatile needAnotherChunk = 0; // Flag for DMA interrupt... End of transfer

long currentSongIndex; // Current song

char ** songs = NULL; // All songs

MODE mode;

PLAYERMODE playermode;

FATFS fatfs; // Fatfs struct

FIL fil; // File struct

DIR dir; // Directory struct

FILINFO fno; // Fileinfo struct

uint8_t selectedBuffer; // Selected buffer from which samples are read

char lfn[_MAX_LFN + 1]; // lfname

main(void){
  
  long freePtr;
  
  //Initialize delays
  
  TM_DELAY_Init();
  
  // Initialization of display
  
  initLcdPeriph();
  
  initLcd();
  
  // Initialization of CS43L22 periph
  
  initCodecPeriph();
  
  // init codec
  
  initCodec();
  
  // Initialization of keypad
  
  initKeypad();
  
  // Declare lfname buffer

  fno.lfname = lfn;
    
  fno.lfsize = sizeof(lfn);
  
  // Check for sd card and mount
  
  if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)){
  
    // Mounting of Fs
  
    if (f_mount(&fatfs, "", 1) != FR_OK){
  
      // Error... Cannot mount
    
      while(1);
    
    }
    
    // Update number of songs
      
    getNumSongs();
      
    // Update list of songs
      
    getSongs();
  
    // Check if casd is inserted
    
  }
  
  // Set and write PLAYER mode... Set playermode STOP
  
  writeMode(PLAYER);
  
  mode = PLAYER;
  
  playermode = STOP;
  
  // Main for loop
  
  while(1){
  
    // Check if SD card is not present and wait for it
    
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)){
      
      // Unmount drive
      
      f_mount(0, "", 1);
      
      // Free songs(memory)
      
      if(songs != NULL){
        
        // Free space for songs
        
        for(freePtr = 0; freePtr < numberOfSongs; freePtr++){
        
          free(songs[freePtr]);
          
        }
        
        // Free pointer to songs
      
        free(songs);
        
        songs = NULL;
        
      }
    
      // Wait!
      
      while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5));
      
      delay(0xfff);
      
      // Mount!
      
      if (f_mount(&fatfs, "", 1) != FR_OK){
  
        // Error... Cannot mount
    
        while(1);
    
      }
      
      // Update number of songs
      
      getNumSongs();
      
      // Update list of songs
      
      getSongs();
      
      // Playermode STOP
      
      playermode = STOP;
      
    }
    
    // Go to the beginning if numberOfSongs is zero
    
    if(!numberOfSongs){
    
      continue;
      
    }
    
    // MENU modes
    
    if(mode == PLAYER && getMode()){
      
      // Pause audio
      
      pauseResume(1);
    
      writeMode(VOLUME);
      
      // Set mode
      
      mode = VOLUME;
      
    }
    else if(mode == VOLUME && getMode()){
      
      // pause audio
      
      pauseResume(1);
    
      writeMode(TREBLE);
      
      // Set mode
      
      mode = TREBLE;
      
      
    }
    else if(mode == TREBLE && getMode()){
      
      // pause audio
      
      pauseResume(1);
    
      writeMode(BASS);
      
      // Set mode
      
      mode = BASS;
      
    }
    else if(mode == BASS && getMode()){
      
      // Resume audio
      
      pauseResume(0);
    
      writeMode(PLAYER);
      
      // Set mode
      
      mode = PLAYER;
      
    }
    
    // PLAYER modes
    
    else if(mode == PLAYER && (playermode == PLAY || playermode == PAUSE) && getStop()){ // From PLAY/PAUSE to STOP
    
      // Reinit codec
      
      resetCodec();
      
      initCodec();
      
      // Close file
      
      f_close(&fil);
      
      // Set stop mode
      
      playermode = STOP;
      
      writeStop();
      
    }
    else if(mode == PLAYER && playermode == STOP && getPlayPause()){ // From STOP to PLAY
    
      // Set PLAY mode
      
      playermode = PLAY;
      
      // Play audio from beginning
      
      // Reinit codec
      
      resetCodec();
      
      initCodec();
      
      writePlay();
      
      openFile();
      
      playAudio(1);  
      
      
    }
    else if(mode == PLAYER && playermode == PLAY && getPlayPause()){ // From PLAY to PAUSE
    
      // Set PAUSE mode
      
      playermode = PAUSE;
      
      // pause audio
      
      pauseResume(1);
      
      writePause();
      
    }
    else if(mode == PLAYER && playermode == PAUSE && getPlayPause()){
    
      // Set PLAY mode
      
      playermode = PLAY;
      
      // play audio
      
      pauseResume(0);
      
      writePlay();
      
    }

    // Next, Previous
    
    else if(mode == PLAYER && getNextInc()){ // NEXT
      
      // Reinit codec
      
      resetCodec();
      
      initCodec();
      
      // Close file
    
      f_close(&fil);
      
      // Set next song
      
      setNextSong();
      
      // Open file
      
      openFile();
      
      writeNext();
      
      if(playermode == PLAY || playermode == PAUSE){
      
        // Play
      
        playAudio(1);
        
      }
    
    }
    else if(mode == PLAYER && getPrevInc()){ // PREV
    
      // Reinit codec
      
      resetCodec();
      
      initCodec();
      
      // Close file

      f_close(&fil);
      
      // Set next song
      
      setPreviousSong();
      
      // Open file
      
      openFile();
      
      writePrev();
      
       if(playermode == PLAY || playermode == PAUSE){
      
        // Play
      
        playAudio(1);
        
      }
      
    }
      
    // VOLUME mode
    
    else if(mode == VOLUME && getNextInc()){
    
      // increase volume
      
      increaseVolume();
      
      writeVolIncr();
      
    }
    else if(mode == VOLUME && getPrevInc()){
    
      // Decrease volume
      
      decreaseVolume();
      
      writeVolDcrs();
      
    }
    
    // TREBLE mode
    
    else if(mode == TREBLE && getNextInc()){
    
      // increase treble
      
      increaseTreble();
      
      writeTrebIncr();
      
    }
    else if(mode == TREBLE && getPrevInc()){
    
      // Decrease treble
      
      decreaseTreble();
      
      writeTrebDcrs();
      
    }
    
    // BASS mode
    
    else if(mode == BASS && getNextInc()){
    
      // increase bass
      
      increaseBass();
      
      writeBassIncr();
      
    }
    else if(mode == BASS && getPrevInc()){
    
      // Decrease bass
      
      decreaseBass();
      
      writeBassDcrs();
      
    }
    
    // Check if interrupt happened
    
    if(needAnotherChunk){
      
      // Check if we reached end of file
      
      if(buffer1Length == 0 || buffer2Length == 0){
      
        // Go to next song
        
        setNextSong();
        
        // Reinit codec
      
        resetCodec();
      
        initCodec();
      
        // Open file
      
        openFile();
        
        // Play new song
        
        playAudio(1);
        
      }
      else{

        // Get new chunk
      
        playAudio(0);

        
      }
      
    }
    

  }

  
}


void getNumSongs(void){
  
  uint8_t i;
  
  // Reset numberOfSongs to 0
  
  numberOfSongs = 0;
  
  // f_readdir status
  
  uint8_t status;
  
  // Extention buffer
  
  char extBuffer[4];
  
  // Open current directory
  
  if(f_opendir(&dir, "/") != FR_OK){
  
    // Error... Cannot open dir
    
    while(1);
    
  }
  
  // Walk through files
  
  for(;;){
    
    // Read a directory item
  
    status = f_readdir(&dir, &fno);
    
    if(status != FR_OK){
    
      // Error... Cannot read directory item
      
      while(1);
      
    }
    else if(fno.lfname[0] == '.'){
      
      continue;
    }
    else if(fno.lfname[0] == 0){
    
      // End of directory.. break
      
      break;
      
    }
    else{
    
      // Get last three letters
      
      strncpy(extBuffer, &fno.lfname[strlen(fno.lfname) - 3], 3);
      
      // Add terminating char
      
      extBuffer[3] = '\0';
      
      // Convert to lower case
      
      i = 0;
      
      while(extBuffer[i] != '\0'){
      
        extBuffer[i] = tolower(extBuffer[i]);
        
        i++;
        
      }
      
      // Check if file has wav extention
      
      if(strcmp(extBuffer, "wav") == 0){
      
        // Increase counter
        
        numberOfSongs++;
        
      }
      
    }
    
  }

  // If num of songs > 0... Set currentSongIndex to 0
    
  if(numberOfSongs > 0){
    
    currentSongIndex = 0;
  }
  
  // Close dir
  
  f_closedir(&dir);

}

void getSongs(void){
  
  long i, j;
  
  // Extention buffer
  
  char extBuffer[4];
  
  // f_readdir status
  
  uint8_t status;
  
  // Open current directory
  
  if(f_opendir(&dir, "/") != FR_OK){
  
    // Error... Cannot open dir
    
    while(1);
    
  }
  
  // Malloc
  
  songs = malloc(numberOfSongs * sizeof(char *));
  
  // Walk through files
  
  j = 0;
  
  for(;;){
    
    // Read a directory item
  
    status = f_readdir(&dir, &fno);
    
    if(status != FR_OK){
    
      // Error... Cannot read directory item
      
      while(1);
      
    }
    else if(fno.lfname[0] == '.'){
      
      continue;
    }
    else if(fno.lfname[0] == 0){
    
      // End of directory.. break
      
      break;
      
    }
    else{
      
      // Get last three letters
      
      strncpy(extBuffer, &fno.lfname[strlen(fno.lfname) - 3], 3);
      
      // Add terminating char
      
      extBuffer[3] = '\0';
      
      // Convert to lower case
      
      i = 0;
      
      while(extBuffer[i] != '\0'){
      
        extBuffer[i] = tolower(extBuffer[i]);
        
        i++;
        
      }
      
      // Check if file has wav extention
      
      if(strcmp(extBuffer, "wav") == 0){
        
        // Malloc
      
        songs[j] = malloc(60 * sizeof(char));
        
        // Copy filename into array
        
        strcpy(songs[j], fno.lfname);
      
        // Increase counter
        
        j++;
        
      }
      
    }
            
  }
  
  // Close dir
  
  f_closedir(&dir);

}

void setNextSong(void){

  if(numberOfSongs > 0){
  
    currentSongIndex = (currentSongIndex + 1) % numberOfSongs;
    
  }
  
}

void setPreviousSong(void){

  if(numberOfSongs > 0){
    
    // Check if *currentSongIndex - 1 is negative
    
    if(currentSongIndex - 1 < 0){
    
      currentSongIndex = numberOfSongs - 1;
      
    }
    else{
  
      currentSongIndex = (currentSongIndex - 1) % numberOfSongs;
      
    }
    
  }
  
}

short getNextChunk(void){

  uint8_t bytesBuffer[2]; // Buffer for sample
  
  short i; // Num of samples
  
  for(i = 0; i < NUMOFSAMPLES; i++){
  
    f_read(&fil, bytesBuffer, sizeof(bytesBuffer), NULL);
    
    if(f_eof(&fil) == 1){
    
      // End of file
      
      break;
      
    }
    
    // Add to the array of samples
    
    if(selectedBuffer == 1){
    
      audio_sample_buffer_one[i] = (bytesBuffer[1] << 8) | bytesBuffer[0];
    }
    else{
    
      audio_sample_buffer_two[i] = (bytesBuffer[1] << 8) | bytesBuffer[0];
      
    }
    
    
  }
  
  return i;
  
}

void playAudio(uint8_t start){
  
  // Reset needAnotherChunk
    
  needAnotherChunk = 0;
  
  // If start is set
  
  if(start){
    
    // Select buffer 1
    
    selectedBuffer = 1;
  
    buffer1Length = getNextChunk();
      
    play(audio_sample_buffer_one, buffer1Length);
    
    // Select buffer 2
    
    selectedBuffer = 2;
        
    buffer2Length = getNextChunk();
    
    
  }
  else if(selectedBuffer == 1){
  
    play(audio_sample_buffer_two, buffer2Length);
    
    // Select buffer 1
    
    selectedBuffer = 1;
        
    buffer1Length = getNextChunk();
        
    // Select buffer 2
    
    selectedBuffer = 2;
    
  }
  else{
  
    play(audio_sample_buffer_one, buffer1Length);
    
    // Select buffer 2
    
    selectedBuffer = 2;
        
    buffer1Length = getNextChunk();
    
    // Select buffer 1
        
    selectedBuffer = 1;
    
  }


}

void openFile(void){

  uint8_t status;
  
  // Open current file
  
  status = f_open(&fil, songs[currentSongIndex], FA_READ);
  
  if(status != FR_OK){
  
    // Error... Cannot open file!
    
    while(1);

  }
  
  // Skip first 44 bytes

  status = f_lseek(&fil, 44);
  
}