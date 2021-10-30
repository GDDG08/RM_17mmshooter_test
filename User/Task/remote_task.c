/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_frame\User\Task\remote_task.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-09 16:42:18
 */
/**
 * DreamChaser Frame Source File
 * 
 * @File:        remote_task.c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "remote_task.h"

/**
 * @brief 	遥控器控制任务
 * @param 	None
 * @retval	None
 * @note	None
 */
void Remote_task(void) {
    Remote_RemoteDataTypeDef* rc = &Remote_RemoteData;
    rc->pengding = 1;

    ShooterMode_Typedef* shooter = &Shooter_Mode;

    switch (rc->remote.sr) {
        case Remote_SWITCH_UP:
            shooter->shooter_speed = Shooter_15mpers;
            shooter->feeder_speed = Feeder_SlowSpeed;
            break;
        case Remote_SWITCH_MIDDLE:
            shooter->shooter_speed = Shooter_18mpers;
            shooter->feeder_speed = Feeder_SlowSpeed;
            break;
        case Remote_SWITCH_DOWN:
            shooter->shooter_speed = Shooter_30mpers;
            shooter->feeder_speed = Feeder_SlowSpeed;
            break;
        default:
            shooter->shooter_speed = 0.0f;
            shooter->feeder_speed = 0.0f;
            break;
    }
    switch (rc->remote.sl) {
        case Remote_SWITCH_UP:
            shooter->shooter_mode = Shooter_STOP;
            shooter->shooter_speed = 0.0f;
            shooter->feeder_speed = 0.0f;
            break;
        case Remote_SWITCH_MIDDLE:
            shooter->shooter_mode = Shooter_IDLE;
            shooter->feeder_speed = 0.0f;
            break;
        case Remote_SWITCH_DOWN:
            shooter->shooter_mode = Shooter_NORMAL;
            break;
        default:
            shooter->shooter_mode = Shooter_NULL;
            shooter->shooter_speed = 0.0f;
            shooter->feeder_speed = 0.0f;
            break;
    }
    if (-rc->remote.ch[2] > 500.0f && shooter->single_shoot_done == 2) {
        shooter->single_shoot_done = 0;
    } else if (shooter->single_shoot_done == 1 && -rc->remote.ch[2] < 400.0f) {
        shooter->single_shoot_done = 2;
    }

    if (-rc->remote.ch[2] < -500.0f && shooter->openloop_set_done == 2) {
        shooter->openloop_set_done = 0;
    } else if (shooter->openloop_set_done == 1 && -rc->remote.ch[2] > -400.0f) {
        shooter->openloop_set_done = 2;
    }
    rc->pengding = 0;
}
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
