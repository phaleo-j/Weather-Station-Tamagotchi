/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * Digital Logic Analyzer Channels:
  *
  * CH1 - SCK
  * CH2 - SI
  * CH3 - TCS
  * CH4 - D/C
  *
  * In Data Sheet:
  * Page 51 - Color Encoding
  *-D/CX=’1’: display data or command parameter.
  *-D/CX=’0’: command data.
  *GM = 11 128x160 memory
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "framedata_LUT.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint8_t x_start_hibyte;
	uint8_t x_start_lobyte;
	uint8_t x_end_hibyte;
	uint8_t x_end_lobyte;

	uint8_t y_start_hibyte;
	uint8_t y_start_lobyte;
	uint8_t y_end_hibyte;
	uint8_t y_end_lobyte;

} ADDRESS_SET;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SOFTWARE_RST 0x01   //ST7735R software reset
#define ST77_NOP     0x00 // No operation
#define ST77_SLPOUT 0x11  // Sleep Out Mode, good after reset

#define ST77_GAMSET 0x26  // Set Gamma Curve
#define ST77_GC2    0x04  // Gamma Curve 2.2 command param

#define ST77_COLMOD 0x3A  // Interface Pixel Format
#define ST77_COLMOD_WRITE_16BPP 0x55 // 16 Bit per pixel frame format param

#define ST77_MADCTL 0x36 // Memory something
#define ST77_MADCTL_MY 0x00  // MADCTL command param
#define ST77_MADCTL_MV 0x00  //MADCTL command param
#define ST77_MADCTL_RGB 0x00  //MADCTL command param

#define ST77_CASET 0x2A // Column Address Set
#define ST77_RASET 0x2B // Row Address Set
#define ST77_NORON 0x13 // Normal Display Mode On
#define ST77_DISPON 0x29 // Display On
#define ST77_DISPOFF 0x28 // Display Off
#define ST77_INVON 0x21 // Inversion Mode On
#define ST77_RAMWR 0x2C  //Memory Write
#define ST77_RGBSET 0x2D //Color Setting for 4K, 65K and 262K

#define FIXED_SIZE_COLUMN_SET 4  //Caset Bytes
#define FIXED_SIZE_ROW_SET 4     //Raset Bytes

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */                                    // Full Ranges  XS,XE = {2, 129}    YS,YE = {1,128}
uint8_t caset_command_param[FIXED_SIZE_COLUMN_SET] = {0x00, 0x02, 0x00, 0x81};  // XS=2, XE=129
uint8_t raset_command_param[FIXED_SIZE_ROW_SET] = {0x00, 0x01, 0x00, 0x80}; //  YS=1 ,YE =128
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */





void SPI_TCS_LO(){
	 HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
}

void SPI_TCS_HI(){
	HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
}


void SPI_sendarraydata(uint8_t tx_buffer[], uint8_t number_of_bytes){  //send multiple bytes
	  HAL_SPI_Transmit(&hspi2, tx_buffer, number_of_bytes, 100);       // send reset command
  }



 void ST77_DrawImage(tImage Image,ADDRESS_SET *address,uint16_t x_start, uint16_t x_end, uint16_t y_start, uint16_t y_end){


      uint8_t spi_data = ST77_CASET;
      uint8_t color_hi;
      uint8_t color_lo;

	  address->x_start_hibyte = (x_start & 0xFF00) >> 8;
	  address->x_start_lobyte = (x_start & 0x00FF);

	  address->x_end_hibyte = (x_end & 0xFF00) >> 8;
	  address->x_end_lobyte = (x_end & 0x00FF);

	  uint8_t caset_parameters[FIXED_SIZE_COLUMN_SET] = {address->x_start_hibyte, address->x_start_lobyte,
			  address->x_end_hibyte,address->x_end_lobyte};

	  address->y_start_hibyte = (y_start & 0xFF00) >> 8;
	  address->y_start_lobyte = (y_start & 0x00FF);

	  address->y_end_hibyte = (y_end & 0xFF00) >> 8;
	  address->y_end_lobyte = (y_end & 0x00FF);

	  uint8_t raset_parameters[FIXED_SIZE_ROW_SET] = {address->y_start_hibyte, address->y_start_lobyte,
			  address->y_end_hibyte,address->y_end_lobyte};


	     SPI_TCS_LO();
	   //HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
	  	  HAL_SPI_Transmit(&hspi2, &spi_data,1, 100);
	  	  HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);

	  	  SPI_sendarraydata(caset_parameters, 4);

	  	  SPI_TCS_HI();
	  //  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
	  	  HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);


	  	  spi_data = ST77_RASET;
	  	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
	  	  HAL_SPI_Transmit(&hspi2, &spi_data,1, 100);
	  	  HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);

	  	  SPI_sendarraydata(raset_parameters, 4);

	  	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);
	  	  HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);

// maybe split up right here?

	  	  uint16_t columns_to_fill = (x_end - x_start)+1;
		  uint16_t rows_to_fill = (y_end - y_start)+1;


		  if (columns_to_fill == 0){
			  columns_to_fill++;
		  }

		  if (rows_to_fill == 0){
		 			  rows_to_fill++;
		 		  }


		  spi_data = ST77_RAMWR;
		 		                   HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
		 		                   HAL_SPI_Transmit(&hspi2, &spi_data,1, 100);
		 		                   HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_SET);


         //were doing 4 bytes at a time now
		  for (int i = 0; i < (columns_to_fill * rows_to_fill)/2; i++) {    //129 columns by 130 rows, +1,+2 offset, fill entire screen black
                //do first pixel, so 16 bits

			  color_hi = (Image.data[i] >> 8) & 0x000000FF;
			  color_lo = (Image.data[i]) & 0x000000FF;
			  HAL_SPI_Transmit(&hspi2,&color_hi ,1, 100);
			  HAL_SPI_Transmit(&hspi2,&color_lo ,1, 100);

			  color_hi = (Image.data[i] >> 24) & 0x000000FF;    //extract highest byte
			  color_lo = (Image.data[i] >> 16) & 0x000000FF;   //extract 2nd highest byte
			  HAL_SPI_Transmit(&hspi2,&color_hi ,1, 100);
			  HAL_SPI_Transmit(&hspi2,&color_lo ,1, 100);

	  }

		  HAL_GPIO_WritePin(GPIOA, D_C_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET); //TCS 1

 }


 void SPI_sendbyte_noparam(uint8_t tx_buffer){  // send a single byte, command with no parameters

	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_RESET);   //TCS 0
	  HAL_SPI_Transmit(&hspi2, &tx_buffer,1, 100);       // send reset command
	  HAL_GPIO_WritePin(GPIOA, TCS_Pin, GPIO_PIN_SET);   //TCS 1
  }





/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
 //  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);


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



                   //Init sequence or ST77 Driver Display
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


   	    	        HAL_Delay(120);  //necessary delay

                   ADDRESS_SET address_test;

                   ST77_DrawImage(Image,&address_test,2,129,1,128);  //blank black screen




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 251;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 49999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|TCS_Pin|D_C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin TCS_Pin D_C_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|TCS_Pin|D_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
