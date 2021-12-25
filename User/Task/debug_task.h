/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\debug_task.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-30 10:47:48
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-12-25 16:02:57
 */

#ifndef __DEBUG_TASK_H
#define __DEBUG_TASK_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "include_lib.h"
#include "debug_BTlog.h"

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
