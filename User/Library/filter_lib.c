/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Library\filter_lib.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-15 12:01:23
 */
/**
 * DreamChaser Frame Source File
 * 
 * @File:        filter_lib.c
 * @Brief:       数字滤波器
 * @Author:      Ju	Chaowen
 * @Modified:    2021年3月21日 16点16分
 *
 */
#include "filter_lib.h"

/**
 * @brief 	一阶低通滤波
 * @param 	None
 * @retval	None
 * @note	None
 */

void low_pass_filter_init(low_pass_t* filt, float cutoff_frq) {
    filt->cutoff_frq = cutoff_frq;
    filt->filted_last_val = 0;
}

float low_pass_filter(float val, low_pass_t* filt) {
    float para;
    para = 2 * PI * 0.001f * (filt->cutoff_frq);
    filt->filted_val = para * val + (1 - para) * filt->filted_last_val;
    filt->filted_last_val = filt->filted_val;
    return filt->filted_val;
}

//float low_pass_filter(float val, low_pass_param_t* pparam, low_pass_t* filt) {
//    // calculate cut off frequence
//    uint32_t period = HAL_GetTick() - pparam->last_tick;
//    pparam->last_tick = HAL_GetTick();
//    if ((pparam->filt_para > 0) && (pparam->filt_para <= 1)) {
//        filt->filted_val = pparam->filt_para * val + (1 - pparam->filt_para) * filt->filted_last_val;
//        filt->filted_last_val = filt->filted_val;
//        if (period > 0)
//            pparam->cut_off_frq = pparam->filt_para / (2 * PI * (float)period * 0.001f);
//        pparam->calc_frq = 1000 / (float)period;
//        return filt->filted_val;
//    } else
//        return val;
//}

/**
 * @brief 	滑动均值滤波
 * @param 	None
 * @retval	None
 * @note	None
 */
void ave_slide_filter_init(ave_filter_t* filter, uint8_t length, float* buff) {
    filter->buff = buff;
    filter->length = length;
    filter->sum = 0;
}

float ave_slide_filter(float input, ave_filter_t* filter) {
    filter->sum = filter->sum - filter->buff[0] + input;

    for (int i = 0; i < (filter->length - 1); i++) {
        filter->buff[i] = filter->buff[i + 1];
    }
    filter->buff[filter->length - 1] = input;

    return (filter->sum) / (filter->length);
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
