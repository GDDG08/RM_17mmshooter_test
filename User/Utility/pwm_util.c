/**
 * DreamChaser Frame Source File
 * 
 * @File:        .c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "pwm_util.h"
/**
 * @brief 	开启PWM
 * @param 	pwm:pwm句柄
 * @retval	None
 */
void PWM_StartPWM(PWM_PWMHandleTypeDef* pwm) {
	if (pwm->state == PWM_OFF) {
		HAL_TIM_PWM_Start(pwm->htim, pwm->ch); 
		pwm->state = PWM_ON;
	}
}

/**
 * @brief 	关闭PWM
 * @param 	pwm:pwm句柄
 * @retval	None
 */
void PWM_StopPWM(PWM_PWMHandleTypeDef* pwm) {
	if (pwm->state == PWM_ON) {
		HAL_TIM_PWM_Stop(pwm->htim, pwm->ch);
		pwm->state = PWM_OFF;
	}
}

/**
 * @brief 	设置PWM占空比
 * @param 	pwm:pwm句柄
 * @retval	None
 */
void PWM_SetPWMDuty(PWM_PWMHandleTypeDef* pwm, float duty) {
    PWM_PWMStateEnum last_state = pwm->state;
    PWM_StopPWM(pwm);
    pwm->duty = duty;
    
    pwm->conf.Pulse = pwm->duty * (pwm->htim->Init.Period + 1);
    HAL_TIM_PWM_ConfigChannel(pwm->htim, &(pwm->conf), pwm->ch);
    
    if (last_state == PWM_ON) {
        PWM_StartPWM(pwm);
    }
}

void PWM_InitPWM(PWM_PWMHandleTypeDef* pwm, TIM_HandleTypeDef* htim, uint32_t ch ,PWM_APB_e apb) {
	pwm->htim   = htim;
	pwm->ch     = ch;
	pwm->state  = PWM_OFF;
	pwm->duty   = 0.0;

	pwm->conf.OCMode        = TIM_OCMODE_PWM1;
	pwm->conf.OCPolarity    = TIM_OCPOLARITY_HIGH;
	pwm->conf.OCFastMode    = TIM_OCFAST_DISABLE;

	PWM_SetPWMFreq(pwm, 1000);
	}

void PWM_SetPWMFreq(PWM_PWMHandleTypeDef* pwm, uint32_t freq ) {
	PWM_PWMStateEnum last_state = pwm->state;
	PWM_StopPWM(pwm);
	pwm->freq = freq;
	APB1PERIPH_BASE;

	pwm->htim->Init.Period = 99;

	pwm->htim->Init.Prescaler = 84000000 / (pwm->htim->Init.Period + 1) / freq - 1;
	HAL_TIM_PWM_Init(pwm->htim);

	PWM_SetPWMDuty(pwm, pwm->duty);

	if (last_state == PWM_ON) {
		PWM_StartPWM(pwm);
	}
}

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
