# Weather-Station-Tamagotchi

Microcontroller: STM32-F446RE
-512KB Flash
-128KB SRAM
-180 MHz Max Clock Speed

LCD Screen: 
TFT LCD Display by ADA Fruit, 128x128



So far in this project, upload of updated ST77_Driver.c is needed
Current Updates: 


9/9/2026 - Working on  drawing function for driver, managed to get pixels on screen with proper initialization, next step is create simple animations. 


9/13/26 - Managed to get a full 128 by 128 pixel image on the screen, the format is, Big-endian for each 16-bit color, in BGR format. I am now able to upload many different images, I have plans of wanting to make this faster perhaps via DMA? We will see, soon I need to start the RTOS integration once sprite animation starts coming into play. "FRAMEDATA_LUT.c" contains the memory of the images in array, an image that covers the entire screen takes 32KB total, my STM32 microcontroller running the LCD screen has 512KB of flash memory, so I can utilize up to most 16 full images. As for the application of a Tamagotchi, I will not need nearly as much of this memory. 
