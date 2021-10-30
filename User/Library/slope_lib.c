/**
 * BattleSpirit Framework Source File
 * 
 * File:        slope_algo.c
 * Brief:       本文件包含斜坡函数控制的相关算法函数
 * Author:      Chen Kangbing
 * Modified:    2019/12/21 16:25:24
 *
 */
#include "slope_lib.h"
/**
  * @brief      初始化斜坡函数控制参数
  * @param      pparam: 指针指向斜坡函数控制参数
  * @param      kp: P系数
  * @param      ki: I系数
  * @param      kd: D系数
  * @param      sum_max: 积分限幅
  * @param      output_max: 输出限幅
  * @retval     无
  */
void Slope_InitSlopeParam(Slope_Param_t* pparam, float acc, float dec) {
    pparam->acc = acc;
    pparam->dec = dec;
}


/**
  * @brief      计算斜坡函数设定值
  * @param      rawref: 当前设定值
  * @param      targetref: 目标设定值
  * @param      pparam: 指针指向斜坡函数控制参数
  * @retval     斜坡函数设定值，若未启用斜坡函数（参数为0），则返回目标设定值
  */
float Slope_CalcSlopeRef(float rawref, float targetref, Slope_Param_t* pparam) {
    float newref;
    if (pparam->acc == 0 | pparam->dec == 0) 
        return targetref;
    
    if (rawref < targetref - pparam->acc) {
        newref = rawref + pparam->acc;
    }
    else if (rawref > targetref + pparam->dec) {
        newref = rawref - pparam->dec;
    }
    else {
        newref = targetref;
    }
    
    return newref;
}


/**
  * @brief      计算绝对值斜坡函数设定值
  * @param      rawref: 当前设定值
  * @param      targetref: 目标设定值
  * @param      pparam: 指针指向斜坡函数控制参数
  * @retval     绝对值斜坡函数设定值，若未启用斜坡函数，则返回目标设定值
  */
float Slope_CalcAbsSlopeRef(float rawref, float targetref, Slope_Param_t* pparam) {
    float newref;
    if (pparam->acc == 0 | pparam->dec == 0) 
        return targetref;
    
    if (rawref > 0) {
        if (rawref < targetref - pparam->acc) {
            newref = rawref + pparam->acc;
        }
        else if (rawref > targetref + pparam->dec) {
            newref = rawref - pparam->dec;
        }
        else {
            newref = targetref;
        }
    }
    else {
        if (rawref > targetref + pparam->acc) {
            newref = rawref - pparam->acc;
        }
        else if (rawref < targetref - pparam->dec) {
            newref = rawref + pparam->dec;
        }
        else {
            newref = targetref;
        }
    }
    
    return newref;
}
