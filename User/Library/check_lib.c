/**
 * DreamChaser Frame Source File
 * 
 * @File:        .c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
/* Includes -------------------------------------------------------------------*/
#include "check_lib.h"	 
/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/	 
/**
 * @brief 	xx
 * @param 	None
 * @retval	None
 * @note	None
 */
float check_commFreq( check_t * check)
{
	uint32_t now_time = HAL_GetTick();
	check->cnt ++ ;
	if (now_time - check->time >1000)
	{
		check->freq = check->cnt;
		check->cnt = 0;
		check->time += 1000;
	}
	return check->freq;
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
