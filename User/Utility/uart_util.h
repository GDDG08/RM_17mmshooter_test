/**
 * DreamChaser Frame Header File
 * 
 * @File:        uart_util.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __UART_UTIL_H
#define __UART_UTIL_H
#ifdef __cplusplus
extern "C" {
#endif 
/* Includes -------------------------------------------------------------------*/
#include "usart.h"
#include "remote_dev.h"

/* define ----------------------------------------------------------------------*/
	 
/* Typedef --------------------------------------------------------------------*/

/* Variables ------------------------------------------------------------------*/
	 
/* Functions ------------------------------------------------------------------*/	
void Uart_ReceiveDMA(UART_HandleTypeDef* huart, uint8_t rxdata[], uint32_t size);
void Uart_SendMessage_IT_Force(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
void Uart_SendMessage_IT(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size);
void Uart_SendMessage(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout);
void Uart_InitUartDMA(UART_HandleTypeDef* huart);
uint16_t Uart_DMACurrentDataCounter(DMA_Stream_TypeDef *dma_stream);

#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
