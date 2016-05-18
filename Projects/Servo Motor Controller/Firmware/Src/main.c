/**
  ******************************************************************************
  * File Name          : main.c
  * Created            : 01/05/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : MOWE main source file. Gimbal control module on HO01R1.
													Three BLDC motors (via RC servo ESCs) on P1, P3 & P4.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	

/* Includes ------------------------------------------------------------------*/
#include "aos.h"				// AOS

/* Private variables ---------------------------------------------------------*/
extern TaskHandle_t FrontEndTaskHandle;
//extern TaskHandle_t ContRotationHandle;
uint8_t mode = 0, motor = 0;
float rate = 1;
int16_t angle = 0;

/* Private function prototypes -----------------------------------------------*/


/* Main ----------------------------------------------------------------------*/
int main(void)
{
	/* --- AOS Initialization Section --- */	
	AOS_init();	

	/* --- User Initialization Section --- */		
	// Do something
	StartMotorDegree(P3,0);
	StartMotorDegree(P4,0);
	MotorSpeed(P3, 20);
	MotorSpeed(P4, 20);
	

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
		if (mode != 0 && (motor == 3 || motor == 4))
		{
		switch (mode)
			{
				case 1 :
					StartMotor(motor);
					mode = 0;
					break;
				case 2 : 	
					StopMotor(motor);
					mode = 0;
					break;
//				case 3 :
//					MotorDegree(motor, angle);
//					mode = 0;
//					break;
//				case 4 :
//					MotorMiddle(motor);
//					mode = 0;
//					break;
				case 5 :
					MotorSpeed(motor, 0);
					mode = 0;
					break;
				case 6 :
					MotorSpeed(motor, 86);		// CW
					mode = 0;
					break;
				case 7 :
					MotorSpeed(motor, -86);		// CCW
					mode = 0;
					break;
				case 8 :
					MotorSpeed(motor, rate);
					mode = 0;
					break;
//				case 7 :
//					SweepTest(motor);
//					break;
				default:
					break;
			}
		}

		osDelay(10);
  }
	
}

/*-----------------------------------------------------------*/

///* ContRotationTask function */
//void ContRotationTask(void * argument)
//{
//	uint16_t deg = 0;
//	
//  /* Infinite loop */
//  for(;;)
//  {
//		
//		if (mode == 8 && (motor == 3 || motor == 4) )
//		{
//			
//			MotorDegree(motor, deg++);
//			if (deg == 360)
//				deg = 0;
//			
//			osDelay(1000/(6*rate));		//	(1/360/60) * 1000

//		}
//		else
//			osDelay(10);
//		
//  }
//	
//}

/************************ (C) COPYRIGHT OU *****END OF FILE****/
