#include "main.h"

CAN_HandleTypeDef hcan1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);

CAN_FilterTypeDef CAN1_Filtre;
CAN_TxHeaderTypeDef CAN1_TxHeader;
CAN_RxHeaderTypeDef CAN1_RxHeader;

uint8_t CAN1TxBuffer[8] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
uint8_t CAN1RxBuffer[8];

uint32_t FreeTxLevel;

void TM_Delay_Init(void);
void TM_DelayMicros(uint32_t micros);
uint32_t multiplier;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN1_Init();
	TM_Delay_Init();
	
	CAN1_Filtre.FilterMode = CAN_FILTERMODE_IDLIST;
	CAN1_Filtre.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN1_Filtre.SlaveStartFilterBank = 14;
	CAN1_Filtre.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN1_Filtre.FilterIdHigh = 0x5640;
	CAN1_Filtre.FilterIdLow = 0x0000;
	CAN1_Filtre.FilterMaskIdHigh = 0xFFFF;
	CAN1_Filtre.FilterMaskIdLow = 0xFFFF;
	CAN1_Filtre.FilterActivation = CAN_FILTER_ENABLE;
	CAN1_Filtre.FilterBank = 0;
	HAL_CAN_ConfigFilter(&hcan1, &CAN1_Filtre);
	
	CAN1_TxHeader.RTR = 0;
	CAN1_TxHeader.IDE = 0;
	CAN1_TxHeader.StdId = 0x2A2;
	CAN1_TxHeader.DLC = 8;
	CAN1_TxHeader.TransmitGlobalTime = DISABLE;
	CAN1_TxHeader.ExtId = 0x0000;
	
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	HAL_CAN_Start(&hcan1);
	
  while (1)
  {
			FreeTxLevel = HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);
			HAL_CAN_AddTxMessage(&hcan1, &CAN1_TxHeader, CAN1TxBuffer, &FreeTxLevel);
			while(HAL_CAN_IsTxMessagePending(&hcan1, FreeTxLevel) == 1);
			TM_DelayMicros(1200);		
  }
}

void TM_Delay_Init(void) {
	uint32_t HCLK_Frequency; 
    	HCLK_Frequency = HAL_RCC_GetSysClockFreq(); 	//  Get system clocks 
   	 multiplier = HCLK_Frequency / 8000000;		//  While loop takes 4 cycles 
								// For 1 us delay, we need to divide with 8M 
}

void TM_DelayMicros(uint32_t micros) {
    	micros = micros * multiplier - 10;			// Multiply micros with multipler and Substract 10 
   	 while (micros--);					// 4 cycles for one loop 
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 128;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_14TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

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

