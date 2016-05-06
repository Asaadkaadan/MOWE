/**
  ******************************************************************************
  * File Name          : aos_md_fe.h
  * Created            : 07/22/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Module Drivers - Front-end Drivers (FED) header file.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_md_fe_H
#define __aos_md_fe_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "aos.h"
	 
/* External variables ---------------------------------------------------------*/
extern uint8_t streamOutFE;
extern uint8_t pdMode;
extern uint8_t ledMode;
extern uint16_t onelevel;
extern uint16_t zerolevel;
extern float rwComRate; 
extern uint8_t rwComPort;
extern uint16_t readComSrc;
extern uint16_t rwComDst;
extern uint8_t rwComLED;	
extern uint8_t streamMode;
extern uint8_t stream2FE;
extern uint8_t stream2FEport;
	 
/* External macros ---------------------------------------------------------*/	 

// Check ports - non-blocking (interrupt-based) read
#define checkP1() if(HAL_UART_Receive_IT(P1uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkP2() if(HAL_UART_Receive_IT(P2uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkP3() if(HAL_UART_Receive_IT(P3uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkP4() if(HAL_UART_Receive_IT(P4uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkP5() if(HAL_UART_Receive_IT(P5uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkP6() if(HAL_UART_Receive_IT(P6uart, (uint8_t *)RXcom, 1) != HAL_OK) {}
#define checkPx(uart) if(HAL_UART_Receive_IT(uart, (uint8_t *)RXcom, 1) != HAL_OK) {}

#define checkAllPorts() checkP1();checkP2();checkP3();checkP4();checkP5();checkP6()

// Read ports - blocking read
#define readP1(n) while(HAL_UART_Receive(P1uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}
#define readP2(n) while(HAL_UART_Receive(P2uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}
#define readP3(n) while(HAL_UART_Receive(P3uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}
#define readP4(n) while(HAL_UART_Receive(P4uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}
#define readP5(n) while(HAL_UART_Receive(P5uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}
#define readP6(n) while(HAL_UART_Receive(P6uart, (uint8_t *)RXcom, n, 1000) != HAL_OK) {}

	
/* External function prototypes -----------------------------------------------*/
extern uint16_t samplePD(void);
extern void pingAllDisp(void);
extern void pingOK(uint16_t originID);	
extern void samOK(uint16_t originID);
extern void sampleAllDisp(void);
extern void readOK(uint16_t originID, uint8_t led);
extern void readAllDisp(void);
extern void startPWM_FE(uint16_t period, uint16_t width);
extern void stream(void);	
extern void streamToFE(uint8_t port);
extern void writeToFE(uint8_t port);
extern uint8_t stream_FE_Port(uint8_t src);
extern uint8_t stream_Port_Port(uint8_t src, uint8_t dst);
	

#ifdef __cplusplus
}
#endif
#endif /*__aos_md_fe_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
