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
	
	// 开启任务
	Shooter_TaskStart();
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
