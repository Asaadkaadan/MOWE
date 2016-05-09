/**
  ******************************************************************************
  * File Name          : main.c
  * Created            : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : MOWE main source file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

#include "aos.h"

/* Private variables ---------------------------------------------------------*/




/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);



int main(void)
{



		/* MCU Configuration----------------------------------------------------------*/

		/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
		HAL_Init();

		/* Configure the system clock */
		SystemClock_Config();

		/* Initialize all configured peripherals */
		MX_GPIO_Init();	
		MX_UART_Init();
		MX_TIM2_Init();
		MX_TIM7_Init();
		MX_DMA_Init();
		#ifndef TX
				MX_ADC_Init();
		#endif


		/* Initialization -----------------------------------------------------------*/
		
		// Startup indicator sequence
		blink(500);
		HAL_Delay(100);
		blink(100);
		HAL_Delay(100);
		blink(100);	

		/* initialize random seed for PRNG */
		srand(atoi(_firmTime));
		
		// Check myID and myName
		checkMyName();
		// Check number of receivers in the array
		checkNumOfReceivers();		
		
		
		
		
		/* Infinite loop */
		while (1)
		{
				/* --- AOS Section --- */
			
				// Check user commands
				checkUser();
			
				// Dispatch pending messages
				mesDispatch();
	
			
				/* --- User Section --- */
			
				// Do something
				

		}
	

}

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

}

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

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
