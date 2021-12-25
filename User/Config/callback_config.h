/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Config\callback_config.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-12-25 15:37:57
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
#ifndef __CALLBACK_CONFIG_H
#define __CALLBACK_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "tim.h"
#include "frame_config.h"
#include "remote_dev.h"
#include "motor_dev.h"
#include "remote_task.h"
#include "shooter_task.h"
#include "debug_task.h"
#include "debug_BTlog.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/
void Uart_ReceiveHandler(UART_HandleTypeDef* huart);
#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
