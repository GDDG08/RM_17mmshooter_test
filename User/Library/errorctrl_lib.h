/**
 * DreamChaser Frame Header File
 * 
 * @File:        errorctrl_lib.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __ERRORCTRL_LIB_H
#define __ERRORCTRL_LIB_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "mathtools_lib.h"
/* define ----------------------------------------------------------------------*/
/* Typedef --------------------------------------------------------------------*/
/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/	 
uint16_t checksum_16( uint8_t *buff , uint16_t len);
#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
