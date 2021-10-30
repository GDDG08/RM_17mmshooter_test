/**
 * DreamChaser Frame Source File
 * 
 * @File:        uart_util.c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "uart_util.h"

/**
  * @brief      初始化UART DMA
  * @param      huart: UART句柄
  * @retval     无
  */
void Uart_InitUartDMA(UART_HandleTypeDef* huart) {
    /* open uart idle it */
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
}

/**
  * @brief      UART错误处理程序
  * @param      ret: 错误代码
  * @retval     无
  */
void Uart_ErrorHandler(uint32_t ret) {
    while (1) {
        return;
    }
}

/**
  * @brief      向UART串口发送信息（阻塞模式）
  * @param      huart: UART句柄
  * @param      txdata: 要发送的信息
  * @param      size: 要发送信息的长度
  * @param      timeout: 超时时间
  * @retval     无
  */
void Uart_SendMessage(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout) {
    /* Start the Transmission process */
    uint32_t ret = HAL_UART_Transmit(huart, txdata, size, timeout);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Uart_ErrorHandler(ret);
    }
}

/**
  * @brief      向UART串口发送信息（非阻塞模式）
  * @param      huart: UART句柄
  * @param      txdata: 要发送的信息
  * @param      size: 要发送信息的长度
  * @retval     无
  */
void Uart_SendMessage_IT(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size) {
    /* Start the Transmission process */
    uint32_t ret = HAL_UART_Transmit_IT(huart, txdata, size);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Uart_ErrorHandler(ret);
    }
}

/**
  * @brief      向UART串口发送信息（非阻塞模式），强制等待，可能造成延时
  * @param      huart: UART句柄
  * @param      txdata: 要发送的信息
  * @param      size: 要发送信息的长度
  * @param      timeout: 超时时间
  * @retval     无
  */
void Uart_SendMessage_IT_Force(UART_HandleTypeDef* huart, uint8_t txdata[], uint16_t size, uint32_t timeout) {
    __HAL_UNLOCK(huart);
    uint32_t ret = HAL_UART_Transmit_IT(huart, txdata, size);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Uart_ErrorHandler(ret);
    }
}

/**
  * @brief      returns the number of remaining data units in the current DMAy Streamx transfer.
  * @param      dma_stream: where y can be 1 or 2 to select the DMA and x can be 0
  *             to 7 to select the DMA Stream.
  * @retval     The number of remaining data units in the current DMAy Streamx transfer.
  */
uint16_t Uart_DMACurrentDataCounter(DMA_Stream_TypeDef *dma_stream) {
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}

/**
  * @brief      enable global uart it and do not use DMA transfer done it
  * @param      huart: uart IRQHandler id
  * @param      pData: receive buff 
  * @param      Size:  buff size
  * @retval     set success or fail
  */
void Uart_ReceiveDMA(UART_HandleTypeDef* huart, uint8_t rxdata[], uint32_t size) {
    uint32_t tmp1 = 0;
    tmp1 = huart->RxState;
    if (tmp1 == HAL_UART_STATE_READY) {
        if ((rxdata == NULL) || (size == 0)) {
            return;
        }
        huart->pRxBuffPtr = rxdata;
        huart->RxXferSize = size;
        huart->ErrorCode  = HAL_UART_ERROR_NONE;
        /* Enable the DMA Stream */
        HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)rxdata, size);
        /* 
         * Enable the DMA transfer for the receiver request by setting the DMAR bit
         * in the UART CR3 register 
         */
        SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);
    }
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
