/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\debug_task.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-30 17:19:54
 */

/* Includes -------------------------------------------------------------------*/
#include "remote_dev.h"
#include "debug_task.h"
/* Variables ------------------------------------------------------------------*/
TIM_HandleTypeDef* Const_debug_TIMER_HANDLER = &htim3;

/* Functions ------------------------------------------------------------------*/

void Debug_TaskInit(void) {
}

void Debug_TaskStart(void) {
    HAL_TIM_Base_Start_IT(Const_debug_TIMER_HANDLER);
}

void Debug_task(void) {
    uint8_t msg[] = "\r\n******UART commucition using IT******\r\nPlease enter 10 characters:\r\n";\

    Remote_RemoteDataTypeDef* rc = &Remote_RemoteData;
    ShooterMode_Typedef* shooter = &Shooter_Mode;
    // Motor_Shooter_l;

    HAL_UART_Transmit_IT(&huart6 ,(uint8_t*)msg,sizeof(msg));
}

void Debug_TimerCallback() {
    Debug_task();
}
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
