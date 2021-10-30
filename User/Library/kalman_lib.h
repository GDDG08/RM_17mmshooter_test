/**
 * DreamChaser Frame Header File
 * 
 * @File:        kalman_lib.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __KALMAN_LIB_H
#define __KALMAN_LIB_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/
typedef struct{
	float p_last;	// 上次估计协方差
	float p_now;	// 当前估计协方差
	float out;	// 输出值
	float kg;	// 卡尔曼增益
	float q;	// 过程噪声协方差
	float r;	// 观测噪声协方差
}kalman_t;

/* Functions ------------------------------------------------------------------*/	
void kalman_FilterInit(kalman_t *kfp ,float r);
float kalmanFilter(kalman_t *kfp,float input);

#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
