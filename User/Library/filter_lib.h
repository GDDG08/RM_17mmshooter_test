/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Library\filter_lib.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-15 11:53:32
 */
/**
 * DreamChaser Frame Header File
 * 
 * @File:        filter_lib.h
 * @Brief:       数字滤波器
 * @Author:      Ju	Chaowen
 * @Modified:    2021年3月21日 16点17分
 *
 */
#ifndef __FILTER_LIB_H
#define __FILTER_LIB_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "arm_math.h"
/* Typedef --------------------------------------------------------------------*/
typedef struct {
    float cutoff_frq;
    float filted_val;
    float filted_last_val;
} low_pass_t;
typedef struct {
    float cut_off_frq;
    float filt_para;
    float last_tick;
    float calc_frq;
} low_pass_param_t;

typedef struct {
    uint8_t length;
    float sum;
    float* buff;
} ave_filter_t;

/* Functions ------------------------------------------------------------------*/
float low_pass_filter(float val, low_pass_t* filt);
void low_pass_filter_init(low_pass_t* filt, float para);

float ave_slide_filter(float input, ave_filter_t* filter);
void ave_slide_filter_init(ave_filter_t* filter, uint8_t length, float* buff);
#ifdef __cplusplus
}
#endif
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
