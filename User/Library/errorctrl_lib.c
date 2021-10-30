/**
 * DreamChaser Frame Source File
 * 
 * @File:        errorctrl_lib.c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    2021年4月3日 13点47分
 *
 */
#include "errorctrl_lib.h"	 
 
/**
 * @brief 	xx
 * @param 	None
 * @retval	None
 * @note	None
 */
uint16_t checksum_16( uint8_t *buff , uint16_t len){
	uint16_t checksum ;
	uint16_t u16 = 0;
	uint8_t  end;
	
	// 判断数据段
	if (len % 2 == 1){
		end = len -1;
		checksum = (uint16_t )buff[8 + len-1] << 8;
	}
	else{
		end = len;
		checksum 	= 0;
	}
	
	for (int i = 0 ; i < 7 ; i+=2 ){
		u16 = buff2ui16(buff + i);
		checksum += u16;
	}
	
	for (int i = 0 ; i < end ; i+=2){
		u16 = buff2ui16(buff + 8 + i);
		checksum += u16;
	}
	return checksum;
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
