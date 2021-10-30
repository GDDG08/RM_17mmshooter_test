/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Config\init_config.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-10-30 17:10:57
 */
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
#include "init_config.h"	 

/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/	 
/**
 * @brief 	初始化
 * @param 	None
 * @retval	None
 * @note	None
 */

void frameInit(void){
	// 启动外设
	Remote_InitRemote(&huart1);
	Motor_GroupConfig();
	
	// 初始化任务数据
	Shooter_TaskInit();
	Debug_TaskInit();
	
	// 开启任务
	Shooter_TaskStart();
	Debug_TaskStart();
	// 
}

/**
 * @brief 	轮询任务
 * @param 	None
 * @retval	None
 * @note	None
 */
void mainloop(void){
	HAL_Delay(10);
	
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
