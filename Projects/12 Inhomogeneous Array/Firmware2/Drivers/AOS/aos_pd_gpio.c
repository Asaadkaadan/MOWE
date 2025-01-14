/**
  ******************************************************************************
  * File Name          : aos_pd_gpio.c
  * Date               : 05/05/2015 12:12:27
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
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
  * Modified           : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "aos.h"


/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/

void IND_LED_Init(void);

#ifdef TX 
GPIO_InitTypeDef GPIO_IR_InitStruct;
#endif


/** Pinout Configuration
*/
void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();
  __GPIOB_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	
	IND_LED_Init();
	
	#ifdef TX
	__GPIOB_CLK_ENABLE();
	IR_LED_Init();
	#endif

}

//-- Configure indicator LED
void IND_LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

#ifdef TX 

//-- Configure front-end LED
void IR_LED_Init(void)
{	
	GPIO_IR_InitStruct.Pin = GPIO_PIN_0;
	GPIO_IR_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_IR_InitStruct.Pull = GPIO_NOPULL;
	GPIO_IR_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_IR_InitStruct);
}

#endif


#if (_module == 2)

//-- Configure output signals
void Output_Init(void)
{	
	GPIO_InitTypeDef GPIO_OUT_InitStruct;

	/* P1 TX >> PA2 */
	GPIO_OUT_InitStruct.Pin = GPIO_PIN_2;
	GPIO_OUT_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_OUT_InitStruct.Pull = GPIO_NOPULL;
	GPIO_OUT_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_OUT_InitStruct);
	
	/* P2 TX >> PC4 */
	GPIO_OUT_InitStruct.Pin = GPIO_PIN_4;
	GPIO_OUT_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_OUT_InitStruct.Pull = GPIO_NOPULL;
	GPIO_OUT_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_OUT_InitStruct);
	
	/* P3 TX >> PC6 */
	GPIO_OUT_InitStruct.Pin = GPIO_PIN_6;
	GPIO_OUT_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_OUT_InitStruct.Pull = GPIO_NOPULL;
	GPIO_OUT_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_OUT_InitStruct);
}

#endif


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
