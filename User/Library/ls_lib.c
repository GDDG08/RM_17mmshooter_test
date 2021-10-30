/**
 * DreamChaser Frame Source File
 * 
 * @File:        .c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */

#include "ls_lib.h"	 
 
/**
 * @brief 	xx
 * @param 	None
 * @retval	None
 * @note	None
 */
float den;
float a,b;
float sumx ;
float sumxx ;
float sumy ;
float sumxy ;
float LS_OneClac(float * buff,int len){	
	

	sumx 	= len * (len - 1)/2;
	sumxx 	= len * (len - 1) * (2 * len - 1)/6;
	sumy 	= 0;
	sumxy 	= 0;
	
	for (int i = 0 ; i < len -1 ;i++){
		sumy += buff[i];
		sumxy +=((i+1)* buff[i]);
	}
	
	den = (len - 1)*sumxx - sumx * sumx;
	a = (len * sumxy - sumx * sumy)/(float)den;
	b = (sumxx * sumy - sumx * sumxy)/(float)den;
	
	return (a *(len)+ b);
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
