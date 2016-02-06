/**
  ******************************************************************************
  * File Name          : main.c
  * Created            : 01/05/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : MOWE main source file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	

/* Includes ------------------------------------------------------------------*/
#include "aos.h"				// AOS

/* Private variables ---------------------------------------------------------*/
extern TaskHandle_t FrontEndTaskHandle;


/* Private function prototypes -----------------------------------------------*/


/* Main ----------------------------------------------------------------------*/
int main(void)
{
	/* --- AOS Initialization Section --- */	
	AOS_init();	

	/* --- User Initialization Section --- */		
	// Do something

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();
			
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  while (1)
  {
  }
	
}

/*-----------------------------------------------------------*/

/* FrontEndTask function */
void FrontEndTask(void * argument)
{

  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
	
}



/************************ (C) COPYRIGHT OU *****END OF FILE****/
