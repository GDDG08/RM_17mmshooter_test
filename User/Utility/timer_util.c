/**
 * DreamChaser Frame Source File
 * 
 * @File:        .c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "timer_util.h" 



void Timer_StartTimer(TIM_HandleTypeDef* htim) {
    HAL_TIM_Base_Start_IT(htim);
}

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
