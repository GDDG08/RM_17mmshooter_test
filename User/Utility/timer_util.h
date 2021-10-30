/**
 * DreamChaser Frame Header File
 * 
 * @File:        timer_util.h
 * @Brief:       定时器相关函数
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __TIMER_UTIL_H
#define __TIMER_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif 
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "tim.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/	 
void Timer_StartTimer(TIM_HandleTypeDef* htim);
    
#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
