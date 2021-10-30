/**
 * DreamChaser Frame Header File
 * 
 * @File:        pwm_util.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __PWM_UTIL_H
#define __PWM_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif 

#include "tim.h"
typedef enum{
	APB1_TIMER = 1,
	APB2_TIMER = 2

}PWM_APB_e;

typedef enum {
	PWM_OFF = 0,
	PWM_ON  = 1

} PWM_PWMStateEnum;

typedef struct {
	TIM_HandleTypeDef* htim;	//定时器句柄
	uint32_t ch;				//通道
	TIM_OC_InitTypeDef conf;	//配置参数
	float duty;					//占空比
	uint32_t freq;				//频率
	PWM_PWMStateEnum state;		//开关
	PWM_APB_e APB;				//总线
	
} PWM_PWMHandleTypeDef;

void PWM_InitPWM(PWM_PWMHandleTypeDef* pwm, TIM_HandleTypeDef* htim, uint32_t ch ,PWM_APB_e apb) ;
void PWM_StartPWM(PWM_PWMHandleTypeDef* pwm);
void PWM_StopPWM(PWM_PWMHandleTypeDef* pwm);
void PWM_SetPWMDuty(PWM_PWMHandleTypeDef* pwm, float duty);
void PWM_SetPWMFreq(PWM_PWMHandleTypeDef* pwm, uint32_t freq);

#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
