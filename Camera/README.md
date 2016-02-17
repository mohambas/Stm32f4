Camera is comprised of Stm32f4 discovery board, hy32d TFT LCD and VC0706 camera module.

This procedure is executed in a loop:

Special command is sent the camera module to begin picture capturing sequence.
After this sequence has been completed, data is written into internal buffer which can be read by a microcontroller(commands and data
are transfered via SPI).
Microcontroller reads the image data and displays them on the screen.


