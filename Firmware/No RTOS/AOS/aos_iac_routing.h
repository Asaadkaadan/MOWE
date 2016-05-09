/**
  ******************************************************************************
  * File Name          : aos_iac_routing.h
  * Created            : 07/23/2015 
  * Version            : 1.0 
 	* Author             : Asaad Kaadan
  * Description        : AOS Inter-array Communication - Routing routines header files.
 	* License            : The MIT Open-source License (MIT). 
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __aos_iac_routing_H
#define __aos_iac_routing_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "aos.h"
	 
/* External variables ---------------------------------------------------------*/
extern uint8_t outPort;
extern uint8_t bcastIn;
extern uint8_t bcastOut[6];
extern uint8_t message[8];
extern ITStatus P1Ready;
extern ITStatus P2Ready;
extern ITStatus P3Ready;
extern ITStatus P4Ready;
extern ITStatus P5Ready;
extern ITStatus P6Ready;
extern ITStatus PCReady;
extern uint8_t portStatus[7];
extern uint8_t mesStack[6][6];
extern uint16_t routeDist[_N]; 
extern uint16_t routePrev[_N]; 
extern uint16_t route[_N];
	 
/* External macros ---------------------------------------------------------*/	 

// Message IDs

#define	_MesID_reroute						1
#define	_MesID_FormLong						2
#define	_MesID_FormShort					3
#define	_MesID_status							4
#define	_MesID_reset							5

#define	_MesID_ping								10
#define	_MesID_pingOK							11
#define	_MesID_singlePingDisp			12
#define	_MesID_pingAllDisp				13

#define	_MesID_sample							15
#define	_MesID_samOK							16
#define	_MesID_singleSamDisp			17
#define	_MesID_sampleAllDisp			18

#define	_MesID_LEDoffFE						20
#define	_MesID_LEDonFE						21
#define	_MesID_LEDtoggleFE				22
#define	_MesID_LEDpulseFE					23
#define	_MesID_LEDpwmFE						24

#define	_MesID_set								30
#define	_MesID_setzerolevel				31
#define	_MesID_setonelevel				32
#define	_MesID_SetPdMode					33
#define	_MesID_SetLedMode					34

#define	_MesID_read								40
#define	_MesID_readOK							41
#define	_MesID_singleReadDisp			42
#define	_MesID_readAllDisp				43

#define	_MesID_write							45

#define	_MesID_stop								50
#define	_MesID_stopPWM						51
#define	_MesID_pause							52
#define	_MesID_resume							53

#define	_MesID_stream							100

#define	_MesID_streamDMA					105
#define	_MesID_streamSISO					106
#define	_MesID_streamSIMO					107

#define	_MesID_broadcast					111


	
/* External function prototypes -----------------------------------------------*/
extern void sendPacket(uint8_t port, const char* text);
extern void send(uint8_t port, const char* text);
extern void readPort(uint8_t port, uint16_t size);
extern uint8_t findRoute(uint16_t sourceID, uint16_t desID);
extern UART_HandleTypeDef* getUart(uint8_t port);
extern int getPort(UART_HandleTypeDef *huart);
extern void parseMessage(uint8_t port);
extern void mesSend(uint16_t src, uint16_t dst);
extern void mesDispatch(void);
extern void pushMessage(uint8_t n_args, ...);



#ifdef __cplusplus
}
#endif
#endif /*__aos_iac_routing_H */


/************************ (C) COPYRIGHT OU *****END OF FILE****/
