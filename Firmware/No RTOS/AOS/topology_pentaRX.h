/**
  ******************************************************************************
  * File Name          : topology_pentaRX.h
  * Date               : 09/17/2015
	*	Author 						 : Asaad Kaadan
  * Description        : Array topology definition. To be filled out manually or
	*												using the MATLAB topology generation tool.
  *
  ******************************************************************************
  */
	
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __topology_H
#define __topology_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
	 
// Module Part Numbers: 
// HO01R1: Hexagon receiver with VEMT3700 - 30mm height
// HO01R2: Hexagon receiver with VEMT3700 - 20mm height
// HO02R0: Hexagon transmiter with VSMY2850G - 30mm height
// HO02R1: Hexagon transmiter with VSMY2850G - 20mm height
// PO01R0: Pentagon receiver with VEMT3700 compatible with hexagons 30mm height
// PO02R0: Pentagon transmiter with VSMY2850G compatible with hexagons 30mm height
	
// Enumerations
enum	modulePartNumbers{_HO01R1, _HO01R2, _HO01R3, _HO02R0, _HO02R1, _HO02R2, _PO01R0, _PO02R0};
enum	portPol{_normal, _reversed};
enum	modulePorts{_P1=1, _P2, _P3, _P4, _P5, _P6};

#define _N	1					// Number of array modules

// Array modules
#define _mod1		1<<3


// Topology
static uint16_t array[_N][7] = {
{ _PO01R0, 0, 0, 0, 0, 0, 0}		// Module 1
};

// Configurations for duplex serial ports
#ifdef _module_1
	#define	_module	1
	#define	PO01R0	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1	
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif


#ifdef __cplusplus
}
#endif
#endif /*__ topology_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT OU *****END OF FILE****/
