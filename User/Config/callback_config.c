/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Config\callback_config.c
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-04 15:28:43
 * @LastEditors  : GDDG08
 * @LastEditTime : 2021-12-25 15:39:43
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
#include "callback_config.h"

/* Functions ------------------------------------------------------------------*/
/**
  * @brief      定时器中断回调函数
  * @param      htim: 指针指向定时器句柄
  * @retval     无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
    if (htim == Const_shooter_TIMER_HANDLER) {
        Shooter_TimerCallback();
    } else if (htim == Const_debug_TIMER_HANDLER) {
        Debug_TimerCallback();
    }
}

/**
  * @brief      UART RX 回调分配函数
  * @param      huart: uart IRQHandler id
  * @retval     无
  */
void Uart_RxIdleCallback(UART_HandleTypeDef* huart) {
    if (huart == Const_Remote_UART_HANDLER) {
        Remote_RXCallback(huart);
        Remote_task();
    } else if (huart == Const_BTlog_UART_HANDLER) {
        BTlog_RXCallback(huart);
    }
}

/**
  * @brief      CAN总线数据接收回调函数
  * @param      phcan: 指向CAN句柄的指针
  * @retval     无
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* phcan) {
    CAN_RxHeaderTypeDef Can_rxHeader;
    uint8_t Can_rxData[8];
    /* Get RX message */
    uint32_t ret = HAL_CAN_GetRxMessage(phcan, CAN_RX_FIFO0, &Can_rxHeader, Can_rxData);
    if (ret != HAL_OK) {
        /* Reception Error */
    }
    Motor_EncoderDecodeCallback(phcan, Can_rxHeader.StdId, Can_rxData);
}

void Uart_ReceiveHandler(UART_HandleTypeDef* huart) {
    // clear idle it flag after uart receive a frame data
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) && __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE)) {
        /* clear idle it flag avoid idle interrupt all the time */
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        /* handle received data in idle interrupt */
        Uart_RxIdleCallback(huart);
    }
}

/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
