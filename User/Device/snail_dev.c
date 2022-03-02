/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Device\snail_dev.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:30:27
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-15 15:13:18
 */

#include "snail_dev.h"

/**
  * @brief      ��ʼ��Snail���
  * @param      
  * @retval     ��
  */
void MotorSnailConfig(MotorSnail_t* snail, TIM_HandleTypeDef* PwmTIMHandle, int PWMchannel, TIM_HandleTypeDef* EncoderTIMHandle, uint8_t bufflen, float *buff) {
    snail->PWMHandle.htim = PwmTIMHandle;
    snail->PWMHandle.ch = PWMchannel;
    snail->PWMHandle.state = PWM_ON;
    snail->EncoderHandle = EncoderTIMHandle;

    // float* buff = (float*)malloc(sizeof(float) * bufflen);
    // memset(buff, 0, sizeof(buff));
    ave_slide_filter_init(&snail->fdb_fil, bufflen, buff);

    HAL_TIM_PWM_Start(PwmTIMHandle, PWMchannel);
    HAL_TIM_Encoder_Start(EncoderTIMHandle, TIM_CHANNEL_ALL);

    SetSnailDuty(snail, 0.2f);
    SetSnailOutput(snail);
}
/**
  * @brief      ��snail�������pwm�ź�
  * @param      snail����ṹ��     
  * @retval     
  */
void SetSnailDuty(MotorSnail_t* snail, float duty) {
    snail->PWMHandle.duty = duty;
    snail->compare = (int)(duty * snail->PWMHandle.htim->Init.Period);
}

void SetSnailOutput(MotorSnail_t* snail) {
    __HAL_TIM_SetCompare(snail->PWMHandle.htim, snail->PWMHandle.ch, snail->compare);
}

float PID_duty_offset[2] = {0.00025635f, 0.05f};
void SnailPIDOutput(MotorSnail_t* snail) {
    float duty = 0.5f;
    if (snail->PID.ref > 0) {
        PID_Calc(&snail->PID, &snail->PIDpara);
        // PID_SetOutput(&snail->PID, snail->PID.ref);
        duty = PID_GetOutput(&snail->PID) * PID_duty_offset[0] + 0.53f + PID_duty_offset[1];
        // float duty = PID_GetOutput(&snail->PID) * 0.00011136f + 0.47522f;
    }
    if (duty < 0.5f)
        duty = 0.5f;
    if (duty > 0.98f)
        duty = 0.98f;
    SetSnailDuty(snail, duty);
    SetSnailOutput(snail);
}

void SnailNoPIDOutput(MotorSnail_t* snail) {
    // y = 0.0002563506 x + 0.4625790958
    float duty = snail->PID.ref * 0.00025635f + 0.462579f;
    if (duty < 0.5f)
        duty = 0.5f;
    if (duty > 0.98f)
        duty = 0.98f;
    SetSnailDuty(snail, duty);
    SetSnailOutput(snail);
}

void SnailDutyOutput(MotorSnail_t* snail, float duty) {
    if (duty < 0.5f)
        duty = 0.5f;
    if (duty > 0.98f)
        duty = 0.98f;
    SetSnailDuty(snail, duty);
    SetSnailOutput(snail);
}

void MotorSnail_SetRef(MotorSnail_t* snail, float ref) {
    snail->PID.ref = ref;
}
/**
  * @brief      ����FeedBack
  * @param      snail����ṹ��     
  * @retval     
  */
void MotorSnail_SetFdb(MotorSnail_t* snail) {
    static int fdb = 0;
    // snail->Data.direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(snail->EncoderHandle);
    snail->Data.counter = __HAL_TIM_GET_COUNTER(snail->EncoderHandle);

    if (snail->Data.direction == 0 /* 1? */)
        fdb = snail->Data.counter;
    else
        fdb = 65535 - snail->Data.counter;
    if (fdb >= 65000)
        fdb = 0;
    __HAL_TIM_SET_COUNTER(snail->EncoderHandle, 0);

    snail->PID.fdb = ave_slide_filter((float)fdb / 2.0f, &snail->fdb_fil);
}
/**
 * @brief 	����PWM
 * @param 	
 * @retval	None
 */
void Snail_Start(MotorSnail_t* snail) {
    if (snail->PWMHandle.state == PWM_OFF)
        HAL_TIM_PWM_Start(snail->PWMHandle.htim, snail->PWMHandle.ch);
}

/**
 * @brief 	�ر�Snail
 * @param 	
 * @retval	None
 */
void Snail_Stop(MotorSnail_t* snail) {
    if (snail->PWMHandle.state == PWM_ON)
        HAL_TIM_PWM_Stop(snail->PWMHandle.htim, snail->PWMHandle.ch);
}
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
