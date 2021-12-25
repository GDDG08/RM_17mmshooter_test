/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\shooter_task.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-12-25 16:07:59
 */
/**
 * DreamChaser Frame Header File
 * 
 * @File:        .h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __GIMBAL_TASK_H
#define __GIMBAL_TASK_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "tim.h"
#include "pid_lib.h"
#include "motor_dev.h"
#include "snail_dev.h"
#include "filter_lib.h"
/* Define ----------------------------------------------------------------------*/
#define Shooter_15mpers 215.0f
#define Shooter_18mpers 230.0f
#define Shooter_30mpers 280.0f

#define Feeder_SlowSpeed 50.0f
#define Feeder_FastSpeed 100.0f
#define Feeder_WaitSpeed 10.0f

/* Typedef --------------------------------------------------------------------*/
typedef enum {
    Shooter_NULL = 0u,
    Shooter_STOP = 1u,
    Shooter_IDLE = 2u,
    Shooter_NORMAL = 3u
} Shooter_StateEnum;

typedef enum {
    OpenLoop_closed = 0u,
    OpenLoop_open = 1u
} OpenLoop_StateEnum;

typedef struct {
    Shooter_StateEnum shooter_mode;
    OpenLoop_StateEnum openloop_mode;
    uint8_t single_shoot_done;
    uint8_t openloop_set_done;
    float shooter_speed;
    float feeder_speed;
} ShooterMode_Typedef;  //云台结构体
/* Variables ------------------------------------------------------------------*/
extern TIM_HandleTypeDef* Const_shooter_TIMER_HANDLER;
extern ShooterMode_Typedef Shooter_Mode;
extern Motor_t Motor_Feeder;
extern MotorSnail_t Motor_Shooter_l;
extern MotorSnail_t Motor_Shooter_r;

/* Functions ------------------------------------------------------------------*/
// init
void Shooter_TaskInit(void);
void Shooter_TaskStart(void);
// callback
void Shooter_TimerCallback(void);

#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
