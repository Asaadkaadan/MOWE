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
uint8_t mode = 0, rate = 0, scale = 0;
float Yaw = 0, Pitch = 0;


/* Private function prototypes -----------------------------------------------*/


/* Main ----------------------------------------------------------------------*/
int main(void)
{
	/* --- AOS Initialization Section --- */	
	AOS_init();	

	/* --- User Initialization Section --- */		
	// Do something
	StartMotorDegree(P3,90);
	StartMotorDegree(P4,90);
	

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

	//mode = 7;
//	scale = 15;
//	rate = 4;
	
	//osDelay(3000);
	
	
  /* Infinite loop */
  for(;;)
  {
		
		switch (mode)
		{
			case 1 :
				SweepTest();
				break;
			case 2 :
				if (scale && rate)
					RandomDist(scale, rate);
				break;
			case 3 :
				MotorMiddle(P3);
				MotorMiddle(P4);
				break;
			case 4 :
				MotorDegree(P3, 0);
				MotorDegree(P4, 0);
				break;
			case 5 :
				MotorDegree(P3, FULLSCALE);
				MotorDegree(P4, FULLSCALE);
				break;
			case 6 :
				if (Yaw) {
					MotorDegree(P3, Yaw);
				}
				if (Pitch) {
					MotorDegree(P4, Pitch);
				}
				break;
			case 7 :		// Hold Array straight
				MotorDegree(P3, 90);		// 90
				MotorDegree(P4, 140);		//140
				break;
			case 12 :		// Start from straight position
				if (scale && rate)
					RandomDistStraight(scale, rate);
				break;
			case 15 : 	// Stop both motors
				StopMotor(P3);
				StopMotor(P4);
				break;
			default:
				break;
		}

		//taskYIELD();
		osDelay(10);
  }
	
}



/************************ (C) COPYRIGHT OU *****END OF FILE****/
