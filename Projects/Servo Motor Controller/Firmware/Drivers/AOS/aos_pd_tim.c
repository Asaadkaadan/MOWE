/**
  ******************************************************************************
  * File Name          : aos_pd_tim.c
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
	
/* Includes ------------------------------------------------------------------*/
#include "aos.h"


TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim15;

/* TIM3 init function */
void MX_TIM3_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 48;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 20000;			// 50 Hz PWM
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htim3);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3);

}
/* TIM15 init function */
void MX_TIM15_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim15.Instance = TIM15;
  htim15.Init.Prescaler = 48;
  htim15.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim15.Init.Period = 20000;			// 50 Hz PWM
  htim15.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim15.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim15);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim15, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&htim15);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim15, &sMasterConfig);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htim15, &sBreakDeadTimeConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  HAL_TIM_PWM_ConfigChannel(&htim15, &sConfigOC, TIM_CHANNEL_1);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM3)
  {
    /* Peripheral clock enable */
    __TIM3_CLK_ENABLE();
  
    /**TIM3 GPIO Configuration    
    PC6     ------> TIM3_CH1
    PC8     ------> TIM3_CH3 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_TIM3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else if(htim_base->Instance==TIM15)
  {
    /* Peripheral clock enable */
    __TIM15_CLK_ENABLE();
  
    /**TIM15 GPIO Configuration    
    PA2     ------> TIM15_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_TIM15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_pwm->Instance==TIM3)
  {
    /* Peripheral clock enable */
    __TIM3_CLK_ENABLE();
  
    /**TIM3 GPIO Configuration    
    PC6     ------> TIM3_CH1
    PC8     ------> TIM3_CH3 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_TIM3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  }
  else if(htim_pwm->Instance==TIM15)
  {
    /* Peripheral clock enable */
    __TIM15_CLK_ENABLE();
  
    /**TIM15 GPIO Configuration    
    PA2     ------> TIM15_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_TIM15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

/* --- Control motor angle (Load PWM timer) --- */
void MotorDegree(uint8_t motorPort, float deg)
{
	
	switch (motorPort)
	{
		case P1 :
			htim15.Instance->CCR1 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			break;
		case P3 :
			htim3.Instance->CCR1 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			break;
		case P4 :
			htim3.Instance->CCR3 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			break;
		default:
			break;
	}		
}

/* --- Control motor speed for continuous servo (Load PWM timer) --- */
void MotorSpeed(uint8_t motorPort, float spd)
{
	
	switch (motorPort)
	{
		case P1 :
			htim15.Instance->CCR1 = (((spd+FULLSCALE)/FULLSCALE) * (MAXWIDTH - MINWIDTH)/2) + MINWIDTH;
			break;
		case P3 :
			htim3.Instance->CCR1 = (((spd+FULLSCALE)/FULLSCALE) * (MAXWIDTH - MINWIDTH)/2) + MINWIDTH;
			break;
		case P4 :
			htim3.Instance->CCR3 = (((spd+FULLSCALE)/FULLSCALE) * (MAXWIDTH - MINWIDTH)/2) + MINWIDTH;
			break;
		default:
			break;
	}		
}

/* --- Control motor angle (Load PWM timer) --- */
void StartMotorDegree(uint8_t motorPort, float deg)
{

	switch (motorPort)
	{
		case P1 :
			htim15.Instance->CCR1 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
			break;
		case P3 :
			htim3.Instance->CCR1 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			break;
		case P4 :
			htim3.Instance->CCR3 = ((deg/FULLSCALE) * (MAXWIDTH - MINWIDTH)) + MINWIDTH;
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
			break;
		default:
			break;
	}		
}

/* --- Start motor PWM and put angle to zero --- */
void StartMotor(uint8_t motorPort)
{
	switch (motorPort)
	{
		case P1 :
			htim15.Instance->CCR1 = MINWIDTH;
			HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
			break;
		case P3 :
			htim3.Instance->CCR1 = MINWIDTH;
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
			break;
		case P4 :
			htim3.Instance->CCR3 = MINWIDTH;
			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
			break;
		default:
			break;
	}	
}

/* --- Stop motor PWM --- */
void StopMotor(uint8_t motorPort)
{
	switch (motorPort)
	{
		case P1 :
			HAL_TIM_PWM_Stop(&htim15, TIM_CHANNEL_1);
			break;
		case P3 :
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
			break;
		case P4 :
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
			break;
		default:
			break;
	}	
}

/* --- Run a full sweep test --- */
void SweepTest(uint8_t port)
{
	static float i = FULLSCALE/2; 
	static uint8_t phase = 0;
	
	if (phase == 0 || phase == 1) {
		if (port == 3)
			MotorDegree(P3, 90);
		if (port == 4)
			MotorDegree(P4, i);
	} else if (phase == 2 || phase == 3) {
		if (port == 4)
			MotorDegree(P4, 90);
		if (port == 3)
			MotorDegree(P3, i);			
	}
	osDelay(50);
	
	if (phase == 0)
	{
		i = i + 0.5;
	
		if (i == FULLSCALE)
			phase = 1;
	}
	else if (phase == 1)
	{
		i = i - 0.5;
		
		if (i == 0) {
			phase = 2;
			if (port == 3)
				MotorDegree(P3, 70);
			if (port == 4)
				MotorDegree(P4, 30);
			osDelay(200);
			if (port == 3)
				MotorDegree(P3, 50);
			if (port == 4)
				MotorDegree(P4, 50);
			osDelay(200);
			if (port == 3)
				MotorDegree(P3, 30);
			if (port == 4)
				MotorDegree(P4, 70);
			osDelay(100);
		}
	}
	else if (phase == 2)
	{
		i = i + 0.5;
	
		if (i == FULLSCALE)
			phase = 3;
	}
	else if (phase == 3)
	{
		i = i - 0.5;
		
		if (i == 0) {
			phase = 0;
			if (port == 4)
				MotorDegree(P4, 70);
			if (port == 3)
				MotorDegree(P3, 30);
			osDelay(200);
			if (port == 4)
				MotorDegree(P4, 50);
			if (port == 3)
				MotorDegree(P3, 50);
			osDelay(200);
			if (port == 4)
				MotorDegree(P4, 30);
			if (port == 3)
				MotorDegree(P3, 70);
			osDelay(100);
		}
	}	
}

/* --- Random disturbance (uniform): rate Hz, +- scale deg --- */
void RandomDist(uint8_t scale, uint8_t rate) 
{
	static int8_t ang = 0;
	
	ang = rand()%(2*scale) - scale;	
	MotorDegree(P3, FULLSCALE/2 + ang);
	ang = rand()%(2*scale) - scale;	
	MotorDegree(P4, FULLSCALE/2 + ang);
	osDelay(1000/rate);
}

/* --- Random disturbance (uniform) from straight position: rate Hz, straight +- scale deg --- */
void RandomDistStraight(uint8_t scale, uint8_t rate) 
{
	static int8_t ang = 0;
	
	ang = rand()%(2*scale) - scale;	
	MotorDegree(P3, FULLSCALE/2 + ang);
	ang = rand()%(2*scale) - scale;	
	MotorDegree(P4, 140 + ang);
	osDelay(1000/rate);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
