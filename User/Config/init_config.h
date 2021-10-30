/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Config\init_config.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-30 17:27:36
 */
/**
 * DreamChaser Frame Header File
 * 
 * @File:        init_config.h
 * @Brief:       初始化
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __INIT_CONFIG_H
#define __INIT_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "frame_config.h"
#include "remote_dev.h"
#include "motor_dev.h"
#include "tim.h"
#include "shooter_task.h"
#include "debug_task.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/
void frameInit(void);
void mainloop(void);

#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
