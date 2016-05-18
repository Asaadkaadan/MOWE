/**
  ******************************************************************************
  * File Name          : line_10_rx.h
  * Date               : 11-Jan-2016
  *	Author 			   : Asaad Kaadan
  * Description        : This file is automatically generated by the 'topogen' MATLAB tool.
  *                      This version is compiled for MCU firmware use.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __line_10_rx_H
#define __line_10_rx_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

// Module Part Numbers:
// HO01R1: Hexagon receiver with VEMT3700 - 30mm optical resolution.
// HO01R2: Hexagon receiver with VEMT3700 - 20mm optical resolution.
// HO02R0: Hexagon transmitter  with VSMY2850G - 30mm optical resolution.
// HO02R1: Hexagon transmitter  with VSMY2850G - 20mm optical resolution.
// PO01R0: Pentagon receiver with VEMT3700 compatible with hexagons 30mm optical resolution.
// PO02R0: Pentagon transmitter  with VSMY2850G compatible with hexagons 30mm optical resolution.

// Enumerations
enum	modulePartNumbers{_HO01R1, _HO01R2, _HO02R0, _HO02R1, _PO01R0, _PO02R0};
enum	portPol{_normal, _reversed};
enum	modulePorts{_P1=1, _P2, _P3, _P4, _P5, _P6};

#define _N	10					// Number of array modules

// Array modules
#define _mod1	1<<3
#define _mod2	2<<3
#define _mod3	3<<3
#define _mod4	4<<3
#define _mod5	5<<3
#define _mod6	6<<3
#define _mod7	7<<3
#define _mod8	8<<3
#define _mod9	9<<3
#define _mod10	10<<3

// Topology
static uint16_t array[_N][7] = {
{ _HO01R1, 0, 0, _mod2|_P6, 0, 0, 0},	// Module 1
{ _HO01R1, 0, 0, _mod3|_P6, 0, 0, _mod1|_P3},	// Module 2
{ _HO01R1, 0, 0, _mod4|_P6, 0, 0, _mod2|_P3},	// Module 3
{ _HO01R1, 0, 0, _mod5|_P6, 0, 0, _mod3|_P3},	// Module 4
{ _HO01R1, 0, 0, _mod6|_P6, 0, 0, _mod4|_P3},	// Module 5
{ _HO01R1, 0, 0, _mod7|_P6, 0, 0, _mod5|_P3},	// Module 6
{ _HO01R1, 0, 0, _mod8|_P6, 0, 0, _mod6|_P3},	// Module 7
{ _HO01R1, 0, 0, _mod9|_P6, 0, 0, _mod7|_P3},	// Module 8
{ _HO01R1, 0, 0, _mod10|_P6, 0, 0, _mod8|_P3},	// Module 9
{ _HO01R1, 0, 0, 0, 0, 0, _mod9|_P3}    // Module 10
};

// Configurations for duplex serial ports
#ifdef _module_1
	#define	_module	1
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_normal	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_2
	#define	_module	2
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_reversed	1
#endif
#ifdef _module_3
	#define	_module	3
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_4
	#define	_module	4
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_5
	#define	_module	5
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_6
	#define	_module	6
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_7
	#define	_module	7
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_8
	#define	_module	8
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_9
	#define	_module	9
	#define	HO01R1	1
	#define	_P1pol_normal	1
	#define	_P2pol_normal	1
	#define	_P3pol_reversed	1
	#define	_P4pol_normal	1
	#define	_P5pol_normal	1
	#define	_P6pol_normal	1
#endif
#ifdef _module_10
	#define	_module	10
	#define	HO01R1	1
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
#endif /*__line_10_rx_H */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT OU *****END OF FILE****/