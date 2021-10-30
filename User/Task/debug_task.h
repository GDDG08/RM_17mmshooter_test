/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\debug_task.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-30 10:47:48
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-30 17:26:08
 */

#ifndef __REMOTE_TASK_H
#define __REMOTE_TASK_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "remote_dev.h"
#include "include_lib.h"
#include "shooter_task.h"
/* Const ----------------------------------------------------------------------*/

/* define ---------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/
extern TIM_HandleTypeDef* Const_debug_TIMER_HANDLER;
/* Functions ------------------------------------------------------------------*/
// init
void Debug_TaskInit(void);
void Debug_TaskStart(void);
// callback
void Debug_TimerCallback(void);
#ifdef __cplusplus
}
#endif
#endif
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
