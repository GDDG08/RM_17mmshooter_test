/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_frame\User\Config\frame_config.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-04 16:39:08
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
#ifndef __FRAME_CONFIG_H
#define __FRAME_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* define ----------------------------------------------------------------------*/
#define ON 1
#define OFF 0
#define __IF_ENABLE(x) (x == ON)

/* Robot Configuration */  //可选 HERO ENGINEER INFANTRY SENTRY UAV DART RADAR
#define ROBOT INFANTRY

#define FRAME_CAN1 ON
#define FRAME_CAN2 OFF

/* Base Utility Configuration */
#define __UTIL_UART ON
#define __UTIL_CAN ON
#define __UTIL_PWM ON

/* Device Configuration */
#if __IF_ENABLE(__UTIL_UART)
#define __DEV_IMU ON
#define __DEV_MINIPC ON
#define __DEV_DR16 ON
#define __DEV_BOARDCOM ON
#define __DEV_REFREE ON
#define __DEV_TOF ON
#endif

#if __IF_ENABLE(__UTIL_CAN)
#define __DEV_RMMOTOR ON
#endif

#if __IF_ENABLE(__UTIL_PWM)
#define __DEV_BEEPER ON
#define __DEV_SERVO ON
#define __DEV_SNAIL ON
#endif

#define __DEV_LED ON
#define __DEV_ENCODER ON

/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
