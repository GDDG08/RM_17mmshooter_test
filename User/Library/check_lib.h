/**
 * DreamChaser Frame Header File
 * 
 * @File:        check_lib.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __CHECK_LIB_H
#define __CHECK_LIB_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/
typedef struct
{
	uint32_t time;
	uint32_t cnt;
	float freq;
	
}check_t;
/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/	 
float check_commFreq( check_t * check);

#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
