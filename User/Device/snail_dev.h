/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Device\snail_dev.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:30:27
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-11-20 16:09:43
 */
/**
 * DreamChaser Frame Header File
 * 
 * @File:        rmmotor.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __SNAIL_DEV_H
#define __SNAIL_DEV_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "frame_config.h"
#include "pid_lib.h"
#include "pwm_util.h"
#include "filter_lib.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint8_t direction;
    uint16_t counter;
    uint32_t last_update_time;
} SnailData;

typedef struct
{
    SnailData Data;
    PID_PIDTypeDef PID;
    PID_PIDParamTypeDef PIDpara;
    PWM_PWMHandleTypeDef PWMHandle;
    TIM_HandleTypeDef* EncoderHandle;
    int compare;
    ave_filter_t fdb_fil;
} MotorSnail_t;

/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/
void MotorSnailConfig(MotorSnail_t* snail, TIM_HandleTypeDef* PwmTIMHandle, int PWMchannel, TIM_HandleTypeDef* EncoderTIMHandle, uint8_t bufflen);
void SetSnailDuty(MotorSnail_t* snail, float duty);
void SetSnailOutput(MotorSnail_t* snail);
void SnailPIDOutput(MotorSnail_t* snail);
void SnailNoPIDOutput(MotorSnail_t* snail);
void MotorSnail_SetRef(MotorSnail_t* snail, float ref);
void MotorSnail_SetFdb(MotorSnail_t* snail);
void Snail_Start(MotorSnail_t* snail);
void Snail_Stop(MotorSnail_t* snail);
#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
