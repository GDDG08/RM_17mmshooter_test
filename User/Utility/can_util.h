/**
 * DreamChaser Frame Header File
 * 
 * @File:        can_util.h
 * @Brief:       本文件包含CAN总线数据收发的相关辅助函数
 * @Author:      Ju	Chaowen
 * @Modified:    2021年2月8日 17点14分
 *
 */
#ifndef __CAN_UTIL_H
#define __CAN_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif 
/* Includes -------------------------------------------------------------------*/
#include "can.h"
/* define ----------------------------------------------------------------------*/
	 
/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/
void Can_InitTxHeader(CAN_TxHeaderTypeDef *header, uint16_t stdid);
void Can_InitFilterAndStart(CAN_HandleTypeDef* phcan);
void Can_SendMessage(CAN_HandleTypeDef* phcan, CAN_TxHeaderTypeDef* pheader, uint8_t txdata[]);

#endif
#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
