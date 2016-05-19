/**
  ******************************************************************************
  * File Name          : aos.c
  * Created            : 08/27/2015 
  * Version            : 1.0 
  * Author             : Asaad Kaadan
  * Description        : Array Operating System (AOS) main source file.
  * License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "aos.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;
uint16_t myID = _module;
uint8_t myTrig = 0;
uint8_t portStatus[7] = {0};

/* Private function prototypes -----------------------------------------------*/
void MX_TIM2_Init(void);
void MX_TIM7_Init(void);

/*-----------------------------------------------------------*/

/* --- AOS Initialization --- */
void AOS_init(void)
{		
	/* MCU Configuration */

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Unlock the Flash Program Erase controller */
	//HAL_FLASH_Unlock();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();	
	MX_UART_Init();
	MX_TIM2_Init();
	MX_TIM7_Init();
	/* PWM Timers will override P1, P3 & P4 */
	MX_TIM3_Init();
  MX_TIM15_Init();
	
	#ifndef TX
		MX_ADC_Init();
	#endif

	/* AOS Initialization */
	
	/* Startup indicator sequence */
	blink(500);
	HAL_Delay(100);
	blink(100);
	HAL_Delay(100);
	blink(100);	

	/* Initialize random seed for PRNG */
	srand(atoi(_firmTime));
	
}

/*-----------------------------------------------------------*/

/* TIM2 init function - periodic event timebase 32-bit */
void MX_TIM2_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
	
	/* Peripheral clock enable */
  __TIM2_CLK_ENABLE();

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 48;
  HAL_TIM_Base_Init(&htim2);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	/* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

}

/*-----------------------------------------------------------*/

/* TIM7 init function - 1 usec timebase 16-bit */
void MX_TIM7_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;
	
	/* Peripheral clock enable */
	__TIM7_CLK_ENABLE();

	/* Peripheral configuration */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 48;
  htim7.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  htim7.Init.Period = 1;
  HAL_TIM_Base_Init(&htim7);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig);

}

/*-----------------------------------------------------------*/

/* --- Load and start micro-second delay counter --- */
void startMicroDelay(uint16_t Delay)
{
	portENTER_CRITICAL();
	
	if (Delay)
	{
		htim7.Instance->ARR = Delay;
	
		HAL_TIM_Base_Start(&htim7);	

		while(htim7.Instance->CNT != 0)
		{
		}
		
		HAL_TIM_Base_Stop(&htim7);
	}
	
	portEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/

/* --- Load and start periodic event counter --- */
void startPerEvent(uint32_t rateFactor)
{
	htim2.Init.Period = rateFactor;
	HAL_TIM_Base_Init(&htim2);

	HAL_TIM_Base_Start_IT(&htim2);		
}

/*-----------------------------------------------------------*/

/* --- Get the UART for a given port --- */
UART_HandleTypeDef* GetUart(uint8_t port)
{
	switch (port)
	{
		case P1 : 
			return P1uart;		
		case P2 :
			return P2uart;
		case P3 :
			return P3uart;
		case P4 :
			return P4uart;
		case P5 :
			return P5uart;
		#if (HO01R1 || HO02R0 || HO01R2 || HO02R1 || HO01R3 || HO02R2)
		case P6 :
			return P6uart;
		#endif
		default:
			return 0;
	}		
}

/*-----------------------------------------------------------*/

/* --- Get the port for a given UART --- */
int GetPort(UART_HandleTypeDef *huart)
{
#if (HO01R1 || HO02R0)
	if (huart->Instance == USART2)
		return P1;
	else if (huart->Instance == USART3)
		return P2;
	else if (huart->Instance == USART7)
		return P3;
	else if (huart->Instance == USART8)
		return P4;
	else if (huart->Instance == USART4)
		return P5;
	else if (huart->Instance == USART5)
		return P6;
#endif
#if (HO01R2 || HO02R1)
	if (huart->Instance == USART2)
		return P1;
	else if (huart->Instance == USART6)
		return P2;
	else if (huart->Instance == USART3)
		return P3;
	else if (huart->Instance == USART5)
		return P4;
	else if (huart->Instance == USART1)
		return P5;
	else if (huart->Instance == USART4)
		return P6;
#endif
#if (HO01R3 || HO02R2)
	if (huart->Instance == USART2)
		return P1;
	else if (huart->Instance == USART3)
		return P2;
	else if (huart->Instance == USART7)
		return P3;
	else if (huart->Instance == USART8)
		return P4;
	else if (huart->Instance == USART4)
		return P5;
	else if (huart->Instance == USART1)
		return P6;
#endif
#if (PO01R0 || PO02R0)
	if (huart->Instance == USART5)
		return P1;
	else if (huart->Instance == USART2)
		return P2;
	else if (huart->Instance == USART3)
		return P3;
	else if (huart->Instance == USART8)
		return P4;
	else if (huart->Instance == USART4)
		return P5;
#endif
		
	return 0;
}

/*-----------------------------------------------------------*/

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;
	
	// Initialize the RCC Oscillators
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;				// HSI = 8 MHz
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;								// PLL = 8x6 = 48 MHz
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	// Initialize the CPU, AHB and APB busses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;		// 48 MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;					// 48 MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;						// 48 MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);		 

	// Initialize the RCC extended peripherals clocks
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  __SYSCFG_CLK_ENABLE();
	
	/* System interrupt init*/
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/*-----------------------------------------------------------*/

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

}

#endif	
		
/************************ (C) COPYRIGHT OU *****END OF FILE****/
