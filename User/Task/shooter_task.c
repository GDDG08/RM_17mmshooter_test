/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\shooter_task.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-15 15:53:51
 */

/* Includes -------------------------------------------------------------------*/
#include "remote_dev.h"
#include "shooter_task.h"
/* Variables ------------------------------------------------------------------*/
TIM_HandleTypeDef* Const_shooter_TIMER_HANDLER = &htim2;
ShooterMode_Typedef Shooter_Mode;

Motor_t Motor_Feeder;
Motor_Group_t* Motor_feederMotors;
MotorSnail_t Motor_Shooter_l;
MotorSnail_t Motor_Shooter_r;

PID_PIDTypeDef ShooterDiff_pid;
PID_PIDParamTypeDef ShooterDiff_param;
ave_filter_t ShooterDiff_fdb_fil;

float Shooter_speed_offset_remote[2] = {0.0f, 0.0f};

/* Functions ------------------------------------------------------------------*/
/**
 * @brief 	
 * @param 	None
 * @retval	None
 * @note	None
 */
float buff0[2][20];
float buff1[500];
void Shooter_TaskInit(void) {
    Shooter_Mode.single_shoot_done = 2;
    Shooter_Mode.openloop_set_done = 2;
    Shooter_Mode.openloop_mode = OpenLoop_closed;

    // MotorSnailConfig(&Motor_Shooter_l, &htim8, TIM_CHANNEL_1, &htim4, 100);
    // MotorSnailConfig(&Motor_Shooter_r, &htim8, TIM_CHANNEL_2, &htim5, 15.9);
    MotorSnailConfig(&Motor_Shooter_l, &htim8, TIM_CHANNEL_1, &htim5, 10, buff0[0]);
    MotorSnailConfig(&Motor_Shooter_r, &htim8, TIM_CHANNEL_2, &htim4, 10, buff0[1]);
    Motor_Shooter_l.Data.direction = 1;
    Motor_Shooter_r.Data.direction = 0;
    Snail_Start(&Motor_Shooter_l);
    Snail_Start(&Motor_Shooter_r);

    Motor_Config(&Motor_Feeder, &hcan1, MOTOR_2006, 4, MOTOR_TYPE_FEEDER, MOTOR_PID_SPEED, MOTOR_DIR_CCW);
    Motor_feederMotors = Motor_GetGroupPtr(&hcan1, Motor_GetMotorTXID(&Motor_Feeder));

    // PID_ParamInit(&Motor_Shooter_l.PIDpara, 0.7, 0.01, 6, 10000, 20000);
    // PID_ParamInit(&Motor_Shooter_r.PIDpara, 0.7, 0.01, 6, 10000, 20000);
    // PID_ParamInit(&Motor_Shooter_l.PIDpara, 0.9, 0.005, 3, 10000, 20000);
    // PID_ParamInit(&Motor_Shooter_r.PIDpara, 0.9, 0.005, 3, 10000, 20000);
    PID_ParamInit(&Motor_Shooter_l.PIDpara, 1.2, 0.003, 3, 10000, 20000);
    PID_ParamInit(&Motor_Shooter_r.PIDpara, 1.2, 0.003, 3, 10000, 20000);
    //TODO:测试DIFF PID，偏置，多pidPara

    PID_ParamInit(&Motor_Feeder.pid_param[0], 750, 0.01, 0, 10000, 20000);
    PID_ParamInit(&Motor_Feeder.pid_param[1], 8.35, 0, 0.11, 10000, 20000);

    PID_ParamInit(&ShooterDiff_param, 14, 0.3, 0, 5, 12);
    PID_SetRef(&ShooterDiff_pid, 0.0f);

    ave_slide_filter_init(&ShooterDiff_fdb_fil, 500, buff1);
    // low_pass_filter_init(&FilterParam_Feeder, -1);
}

void Shooter_TaskStart(void) {
    HAL_TIM_Base_Start_IT(Const_shooter_TIMER_HANDLER);
}

/**
  * @brief      
  * @param      
  * @retval     
  */
float Shooter_speed_offset[3][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}};  //{{0.71f, 0.208f}, {0.0f, 1.308f}, {-1.2119f, 5.652f}};
void Shooter_TimerCallback() {
    if (Shooter_Mode.openloop_set_done == 0) {
        Shooter_Mode.openloop_mode = Shooter_Mode.openloop_mode == OpenLoop_open ? OpenLoop_closed : OpenLoop_open;
        Shooter_Mode.openloop_set_done = 1;
    }
    MotorSnail_SetFdb(&Motor_Shooter_l);
    MotorSnail_SetFdb(&Motor_Shooter_r);
    //for encoder test
    Motor_Shooter_r.PID.fdb *= 2;

    PID_SetFdb(&ShooterDiff_pid, ave_slide_filter(Motor_Shooter_l.PID.fdb - Motor_Shooter_r.PID.fdb, &ShooterDiff_fdb_fil));  // Motor_Shooter_l.PID.fdb - Motor_Shooter_r.PID.fdb);
    PID_Calc(&ShooterDiff_pid, &ShooterDiff_param);
    if (Shooter_Mode.openloop_mode == OpenLoop_closed && Shooter_Mode.shooter_speed_l > 0 && Shooter_Mode.shooter_speed_r > 0) {
        MotorSnail_SetRef(&Motor_Shooter_l, Shooter_Mode.shooter_speed_l + PID_GetOutput(&ShooterDiff_pid) + Shooter_speed_offset_remote[0]);
        MotorSnail_SetRef(&Motor_Shooter_r, Shooter_Mode.shooter_speed_r - PID_GetOutput(&ShooterDiff_pid) + Shooter_speed_offset_remote[1]);
    } else {
        MotorSnail_SetRef(&Motor_Shooter_l, Shooter_Mode.shooter_speed_l);
        MotorSnail_SetRef(&Motor_Shooter_r, Shooter_Mode.shooter_speed_r);
    }
    // MotorSnail_SetRef(&Motor_Shooter_l, Shooter_Mode.shooter_speed);
    // MotorSnail_SetRef(&Motor_Shooter_r, Shooter_Mode.shooter_speed);
    if (Shooter_Mode.openloop_mode == OpenLoop_closed) {
        SnailPIDOutput(&Motor_Shooter_l);
        SnailPIDOutput(&Motor_Shooter_r);
    } else {
        SnailNoPIDOutput(&Motor_Shooter_l);
        SnailNoPIDOutput(&Motor_Shooter_r);
        // SnailDutyOutput(&Motor_Shooter_l, Shooter_speed_offset_remote[0]);
        // SnailDutyOutput(&Motor_Shooter_r, Shooter_speed_offset_remote[1]);
    }

    //Feeder 2006
    PID_SetFdb(&Motor_Feeder.pid[0], (float)Motor_Feeder.encoder.speed / 36.0f);
    PID_SetFdb(&Motor_Feeder.pid[1], (float)Motor_Feeder.encoder.consequent_angle);

    PID_SetRef(&Motor_Feeder.pid[0], Shooter_Mode.feeder_speed);

    if (Shooter_Mode.shooter_mode == Shooter_IDLE) {
        if (Shooter_Mode.single_shoot_done == 0) {  // not shoot yet
            PID_SetRef(&Motor_Feeder.pid[1], (float)Motor_Feeder.encoder.consequent_angle + 45.0f);
            Shooter_Mode.single_shoot_done = 3;

        } else if (Shooter_Mode.single_shoot_done == 3 && fabs(Motor_Feeder.pid[1].fdb - Motor_Feeder.pid[1].ref) < 1.0f)
            Shooter_Mode.single_shoot_done = 1;
        else if (Shooter_Mode.single_shoot_done == 2)
            PID_SetRef(&Motor_Feeder.pid[1], (float)Motor_Feeder.encoder.consequent_angle);
        Motor_Feeder.pid_type = MOTOR_PID_ANGLE;

    } else {
        Motor_Feeder.pid_type = MOTOR_PID_SPEED;
    }

    Motor_CalcSelfMotorOutput(&Motor_Feeder);
    Motor_SetMotorOutput(&Motor_Feeder, Motor_Feeder.pid[0].output);
    Motor_SendMotorGroupOutput(Motor_feederMotors);
}

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
