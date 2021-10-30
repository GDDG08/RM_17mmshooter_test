/**
 * DreamChaser Frame Header File
 * 
 * @File:        mathtools_lib.h
 * @Brief:       包含一些常用的数学工具
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __MATHTOOLS_UTIL_H
#define __MATHTOOLS_UTIL_H
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "string.h"	 
/* Typedef --------------------------------------------------------------------*/
typedef union{
	uint8_t     u8[4];
	uint32_t    u32;
	int32_t		i32;
	float       fl;
}
union32t;   		//32位数据的联合

typedef union{
	uint8_t     u8[2];
	uint16_t    u16;
	int16_t     i16;
}
union16t; 			//16位数据的联合

/* Functions ------------------------------------------------------------------*/	 
float limit( float input , float limit_up , float limit_down );
float upperlimit( float input , float upper_limit_value  );
float lowerlimit( float input , float lower_limit_value  );
int16_t dec_int16(int16_t input , int16_t aim , uint16_t step);
int16_t approach_is(float input);
uint8_t approach_judge(float input,float errer_max);
	
float sign(float x);
extern float fabsf(float _x);
extern double fabs(double _x);	
extern double sqrt(double _x);		

float 		buff2float	(uint8_t* buff);
uint16_t 	buff2ui16	(uint8_t* buff) ;
int16_t 	buff2i16	(uint8_t* buff) ;

void i162buff	(int16_t int16, uint8_t* buff) ;
void ui162buff	(uint16_t u, 	uint8_t* buff) ;	 
void float2buff	(float f, 		uint8_t* buff) ; 

float Slope_Ctrl(float rawref, float targetref, float acc, float dec);
float remove_micro( float input , float upper_limit_value , float lower_limit_value );

#ifdef __cplusplus
}
#endif
#endif

/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/

