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
