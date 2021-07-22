/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name           : main.c
  * Description         : STATE 0: Ask for user GPS input
  * 					  STATE 1: Parse the GPS input using strtok
  * 					  STATE 2: output TIME information
  * 					  STATE 3: output LATITUDE information
  * 					  STATE 4: output LONGITUDE information
  * 					  STATE 5: output LATITUDE information
  *
  ******************************************************************************
  *
  *Author 				: Vaibhav Ariyur
  *Date					: April 9th 2021
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "debounce.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  HD44780_Init();
  deBounceInit();
  /* USER CODE BEGIN 2 */
  uint8_t STATE=0;
  uint8_t pinstate1;
  uint8_t pinstate2;
  uint8_t pinstate3;
  uint8_t pinstate4;
  char GPSdata[73]={0};
  const char *delim=",";

  char *timeRaw=NULL;
  char timeProc[15]={0};
  char *latRaw=NULL;
  char latProc[16]={0};
  char *longRaw=NULL;
  char longProc[16]={0};
  char *altRaw=NULL;
  char altProc[14]={0};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
  {
    /* USER CODE END WHILE */
	  switch(STATE){
	  	  //RETRIEVE DATA
	  	  case 0:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Input GPS");
				HD44780_GotoXY(0,1);
				HD44780_PutStr("data");
	  		    printf("Input GPS data:\r\n");
	  		    scanf("%s",&GPSdata);
	  		    //printf("Inputted GPS data: %s\r\n", GPSdata);
	  		    STATE=1;
	  		    break;
	  	  //PROCESSING DATA
	  	  case 1:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Processing Data");
				HD44780_GotoXY(0,1);
				HD44780_PutStr("...");

				printf("Processing Data... \r\n");
				strtok(GPSdata,delim);

				timeRaw=strtok(NULL,delim);
				char timeTemp[15]={timeRaw[0],timeRaw[1],'h',' ',timeRaw[2],timeRaw[3],'m',' ',timeRaw[4],timeRaw[5],timeRaw[6],timeRaw[7],timeRaw[8],'s','\0'};
				sprintf(timeProc,"%s",timeTemp);

				latRaw=strtok(NULL,delim);
				char latTemp[16]={'0',latRaw[0],latRaw[1],'d',' ',latRaw[2],
						latRaw[3],'m',' ',latRaw[5],latRaw[6],
						latRaw[7],latRaw[8],'d','d','\0'};
				sprintf(latProc,"%s",latTemp);

				strtok(NULL,delim);

				longRaw=strtok(NULL,delim);
				char longTemp[16]={longRaw[0],longRaw[1],longRaw[2],'d',' ',
						longRaw[3],longRaw[4],'m',' ',longRaw[6],longRaw[7],
						longRaw[8],longRaw[9],'d','d','\0'};
				sprintf(longProc,"%s",longTemp);

				strtok(NULL,delim);
				strtok(NULL,delim);
				strtok(NULL,delim);
				strtok(NULL,delim);

				altRaw=strtok(NULL,delim);
				sprintf(altProc,"%s metres",altRaw);
				STATE=2;
				HAL_Delay(5000);
				break;
		  //TASK SELECTION
	  	  case 2:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("1)Time 2)Latitu");
				HD44780_GotoXY(0,1);
				HD44780_PutStr("3)Longi 4)Altitu");

				printf("1)Time 2)Latitude 3)Longitude 4)Altitude \r\n");

				while(1){
					pinstate1=deBounceReadPin(GPIO_PIN_3);
					pinstate2=deBounceReadPin(GPIO_PIN_4);
					pinstate3=deBounceReadPin(GPIO_PIN_7);
					pinstate4=deBounceReadPin(GPIO_PIN_8);

					if(pinstate1==0){
						while(pinstate1==0){pinstate1=deBounceReadPin(GPIO_PIN_3);}
						STATE=3;
						break;
					}

					if(pinstate2==0){
						while(pinstate2==0){pinstate2=deBounceReadPin(GPIO_PIN_4);}
						STATE=4;
						break;
					}

					if(pinstate3==0){
						while(pinstate3==0){pinstate3=deBounceReadPin(GPIO_PIN_7);}
						STATE=5;
						break;
					}

					if(pinstate4==0){
						while(pinstate4==0){pinstate4=deBounceReadPin(GPIO_PIN_8);}
						STATE=6;
						break;
					}
				}
				break;
		  //TIME
	  	  case 3:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Time:");
				HD44780_GotoXY(0,1);
				HD44780_PutStr(timeProc);

				printf("Time: %s\r\n",timeProc);

				HAL_Delay(5000);
				STATE=2;
				break;
		  //LATITUDE
	  	  case 4:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Latitu:");
				HD44780_GotoXY(0,1);
				HD44780_PutStr(latProc);

				printf("Latitude: %s\r\n",latProc);

				HAL_Delay(5000);
				STATE=2;
				break;
		  //LONGITUDE
	  	  case 5:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Longi:");
				HD44780_GotoXY(0,1);
				HD44780_PutStr(longProc);

				printf("Longitude: %s\r\n",longProc);

				HAL_Delay(5000);
				STATE=2;
				break;
		  //ALTITUDE
	  	  case 6:
				HD44780_ClrScr();
				HD44780_GotoXY(0,0);
				HD44780_PutStr("Altitu:");
				HD44780_GotoXY(0,1);
				HD44780_PutStr(altProc);

				printf("Altitude: %s\r\n",altProc);

				HAL_Delay(5000);
				STATE=2;
				break;


	  }

	  }

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA3 PA4 PA7 PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
