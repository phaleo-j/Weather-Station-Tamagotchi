 void SPI_sendbyte_noparam(uint8_t tx_buffer){  // send a single byte, command with no parameters

	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
	  HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);       // send reset command
	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);   //TCS 1
  }

  void SPI_sendarraydata(uint8_t tx_buffer[], uint8_t number_of_bytes){  //send multiple bytes
	  HAL_SPI_Transmit(&hspi2, tx_buffer, number_of_bytes, 100);       // send reset command
  }


  HAL_GPIO_WritePin(GPIOB, RST_Pin, GPIO_PIN_RESET);  //Hardware Pin
  HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOB, RST_Pin, GPIO_PIN_SET);  //Hardware Pin
    HAL_Delay(150);

    uint8_t quantity_elements = 4;
    uint8_t tx_buffer = SOFTWARE_RST;
    //SPI_sendbyte_noparam(tx_buffer);
    HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET); // set back to command mode
    HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
    HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);       // send reset command
    HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);   //TCS 1

    HAL_Delay(150);
                   tx_buffer = ST77_SLPOUT;
   	               SPI_sendbyte_noparam(tx_buffer);

   	                HAL_Delay(150);

   	    	        tx_buffer = ST77_GAMSET;
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        tx_buffer = ST77_GC2;
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET); // set data command parameter
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);  //TCS 1
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET); // set back to command mode

   	    	        tx_buffer = ST77_COLMOD;
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        tx_buffer = ST77_COLMOD_WRITE_16BPP;
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);  //TCS 1
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);

   	    	        tx_buffer = ST77_MADCTL;
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        tx_buffer = ST77_MADCTL_MY | ST77_MADCTL_MV | ST77_MADCTL_RGB;
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);

   	    	        tx_buffer = ST77_CASET;
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);
   	    	        SPI_sendarraydata(caset_command_param, quantity_elements);
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);

   	    	        tx_buffer = ST77_RASET;
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
   	    	        HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);
   	    	        SPI_sendarraydata(raset_command_param, quantity_elements);
   	    	        HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
   	    	        HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);


   	    	        tx_buffer = ST77_NORON;
   	    	        SPI_sendbyte_noparam(tx_buffer);

   	    	        tx_buffer = ST77_DISPON;
   	    	        SPI_sendbyte_noparam(tx_buffer);



   	    	     uint8_t hi = 0x07;
   	    	     uint8_t lo = 0xE0;

                   tx_buffer = ST77_RAMWR;
                   HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
                   HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);
                   HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);

                   for (int i = 0; i < 128*128; i++) {
                	   HAL_SPI_Transmit(&hspi2, &hi,1, 100);
                	   HAL_SPI_Transmit(&hspi2, &lo,1, 100);
                   }

                   HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET); //TCS 1

