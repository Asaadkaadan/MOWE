/**
  ******************************************************************************
  * File Name          : aos_pd_tim.h
  * Date               : 29/03/2016 22:14:01
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/**
  ******************************************************************************
  * Modified           : 03/29/2016 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */	

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_pd_tim_H
#define __aos_pd_tim_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

	 	 
#define	MINWIDTH	1000	// msec
#define	MAXWIDTH	2000 	// msec
	 
//#define	FULLSCALE		360		// deg
#define	FULLSCALE		86		// rpm at 6V
	 
#define MotorMiddle(p)	MotorDegree(p, FULLSCALE/2)
#define YawMotor	P3
#define PitchMotor	P4	 
	 
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;

void MX_TIM3_Init(void);
void MX_TIM15_Init(void);
void MotorDegree(uint8_t motorPort, float deg);
void MotorSpeed(uint8_t motorPort, float spd);
void StartMotor(uint8_t motorPort);
void StopMotor(uint8_t motorPort);
void StartMotorDegree(uint8_t motorPort, float deg);
void SweepTest(uint8_t port);
void RandomDist(uint8_t scale, uint8_t rate);
void RandomDistStraight(uint8_t scale, uint8_t rate);


#ifdef __cplusplus
}
#endif
#endif /*__ aos_pd_tim_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
