/**
 * DreamChaser Frame Source File
 * 
 * @File:        mathtools_lib.c
 * @Brief:       包含一些常用的数学工具
 * @Author:      Ju	Chaowen
 * @Modified:    2021年2月8日 16点46分
 *
 */
/* Includes -------------------------------------------------------------------*/
#include "mathtools_lib.h"	  
/* Functions ------------------------------------------------------------------*/	 
/**
* @brief 	上下限函数
* @param 	input：		输入
* @param 	limit_up：	上限
* @param 	limit_down：下限
* @retval	input		在限制内
			limit_up	超过上限
			limit_down	超过下限
* @note		None
*/
float limit( float input , float upper_limit_value , float lower_limit_value )
{
	if(input > upper_limit_value)
		return upper_limit_value ;
	else if(input < lower_limit_value)
		return lower_limit_value ;
	else
		return input ;
}

/**
* @brief 	上限函数
* @param 	input：		输入
* @param 	limit_up：	上限
* @retval	input		在限制内
			limit_up	超过上限
* @note		None
*/
float upperlimit( float input , float upper_limit_value  )
{
	if(input > upper_limit_value)
		return upper_limit_value ;
	else
		return input ;
}

/**
* @brief 	下限函数
* @param 	input：		输入
* @param 	limit_up：	上限
* @retval	input		在限制内
			limit_up	超过上限
* @note		None
*/
float lowerlimit( float input , float lower_limit_value  )
{
	if(input > lower_limit_value)
		return lower_limit_value ;
	else
		return input ;
}

/**
* @brief 	符号判断函数
* @param 	x:		输入值
* @retval	正数：	1
			负数：	-1
			0：		0	
* @note		None
*/
float sign(float x) 
{
	if(x > 0)
		return (1.0f);
	else if(x < 0)
		return (-1.0f);
	return 0;
}

/**
* @brief 	衰减函数
* @param 	input：		输入
* @param 	limit_up：	上限
* @retval	衰减后的值
* @note		None
*/
int16_t dec_int16(int16_t input , int16_t aim , uint16_t step){
	if(  ((input - aim) <= step)  &&  ((input - aim) >= -step)  )
		return aim;
	else if(input > aim)
		return input - step;
	else //(input < aim )
		return input + step;
}

/**
* @brief 	四舍五入函数
* @param 	input：		输入
* @param 	limit_up：	上限
* @retval	衰减后的值
* @note		None
*/
int16_t approach_is(float input){
	if( input - (int16_t)input > (  (int16_t)input - input  + 1 ) )
		return (int16_t)input+1;
	else
		return (int16_t)input;
}

/**
* @brief 	数据近似相等判断
* @param 	input：		输入
* @param 	errer_max：	上限
* @retval	是否认为近似相等
* @note		None
*/
uint8_t approach_judge(float input,float errer_max){
	if(input > errer_max)
		return 0 ;
	else if(input < -errer_max)
		return 0 ;
	else
		return 1 ;
}

/**
* @brief 	8位缓冲buff转换为浮点数
* @param 	buff	：8位缓冲
* @retval	浮点数
* @note		None
*/
float buff2float(uint8_t* buff) { 
    uint8_t tmpbuff[4];
    memcpy(tmpbuff, buff, 4);
    return *((float *)tmpbuff); 
}

/**
* @brief 	浮点数转换为8位
* @param 	buff	：8位缓冲
* @param 	f		：浮点数
* @note		None
*/
void float2buff(float f, uint8_t* buff) { 
    uint8_t tmpbuff[4];
    *((float *)tmpbuff) = f;
    memcpy(buff, tmpbuff, 4);
}

/**
* @brief 	8位缓冲转换为16位无符号
* @param 	buff	：8位缓冲
* @retval	16位无符号数
* @note		None
*/
uint16_t buff2ui16(uint8_t* buff) {
    return buff[0] | (buff[1] >> 8);
}

/**
* @brief 	8位缓冲转换为16位有符号
* @param 	buff	：8位缓冲
* @retval	16位有符号数
* @note		None
*/
int16_t buff2i16(uint8_t* buff) {
    return buff[0] | (buff[1] >> 8);
}

/**
* @brief 	16位无符号数转换为8位
* @param 	buff	：8位缓冲
* @param 	u		：16位无符号数
* @note		None
*/
void ui162buff(uint16_t u, uint8_t* buff) 
{
    buff[0] = u & 0xff;
    buff[1] = u >> 8;
}

/**
* @brief 	16位有符号数转换为8位
* @param 	buff	：8位缓冲
* @param 	int16		：16位有符号数
* @note		None
*/
void i162buff(int16_t int16, uint8_t* buff) 
{
    buff[0] = int16 & 0xff;
    buff[1] = int16 >> 8;
}

/**
* @brief      斜坡控制
* @param      rawref	:当前期望
* @param      targetref	:目标期望
* @param      acc		:增加步长
* @param      float dec	:衰减步长
* @retval     newref	：斜坡控制后的输出期望	
*/
float Slope_Ctrl(float rawref, float targetref, float acc, float dec)
{    
    float newref;
    if (rawref > 0) {
        if (rawref < targetref - acc) 
            newref = rawref + acc;
        else if (rawref > targetref + dec) 
            newref = rawref - dec;
        else 
            newref = targetref;
    }
    else {
        if (rawref > targetref + acc) 
            newref = rawref - acc;
        else if (rawref < targetref - dec) 
            newref = rawref + dec;
        else 
            newref = targetref;
    }
    return newref;
}

/**
* @brief 	去除小范围扰动函数
* @param 	input：				输入
* @param 	upper_limit_value：	上界阈值
* @param 	lower_limit_value：	下界阈值
* @retval	在上下界之内输出为0 否则输出输入值
* @note		None
*/
float remove_micro( float input , float upper_limit_value , float lower_limit_value )
{
	if( (input < upper_limit_value) && (input > lower_limit_value) )
		return 0 ;
	else
		return input ;
}

/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
