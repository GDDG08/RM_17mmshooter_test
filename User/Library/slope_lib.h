/**
 * DreamChaser Frame Header File
 * 
 * @File:        slope_lib.h
 * @Brief:       斜坡控制
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __SLOPE_ALGO_H
#define __SLOPE_ALGO_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/

/* define ----------------------------------------------------------------------*/
	 
/* Typedef --------------------------------------------------------------------*/
typedef struct 
{
    float acc;
    float dec;
    
} Slope_Param_t;

/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/	 


void Slope_InitSlopeParam(Slope_Param_t* pparam, float acc, float dec);
float Slope_CalcSlopeRef(float rawref, float targetref, Slope_Param_t* pparam);


#ifdef __cplusplus
}
#endif

#endif
