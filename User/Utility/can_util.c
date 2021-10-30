/**
 * DreamChaser Frame Source File
 * 
 * @File:        can_util.c
 * @Brief:       本文件包含CAN总线数据收发的相关辅助函数
 * @Author:      Ju	Chaowen
 * @Modified:    2021年2月8日 17点13分
 *
 */
#include "can_util.h"
/**
  * @brief      CAN错误处理程序
  * @param      ret: 错误代码
  */
void Can_ErrorHandler(uint32_t ret) {
    while (1) {
        //return;
    }
}

/**
  * @brief      初始化CAN发送头
  * @param      pheader: 指向被初始化的发送头的指针
  * @param      stdid: CAN设备号  0x1ff 0x200 0x2ff
  * @retval     无
  */
void Can_InitTxHeader(CAN_TxHeaderTypeDef *pheader, uint16_t stdid) {
    pheader->StdId = stdid;
    pheader->RTR = CAN_RTR_DATA;
    pheader->IDE = CAN_ID_STD;
    pheader->DLC = 8;
    pheader->TransmitGlobalTime = DISABLE;
}

/**
  * @brief      初始化CAN过滤器并启用CAN总线收发
  * @param      phcan: 指向CAN句柄的指针 &hcan1 &hcan2
  * @retval     无
  */
void Can_InitFilterAndStart(CAN_HandleTypeDef* phcan) {
    CAN_FilterTypeDef sFilterConfig;

    if(phcan == &hcan1)
        sFilterConfig.FilterBank = 0;
    else
        sFilterConfig.FilterBank = 14;
    
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    uint32_t ret = HAL_CAN_ConfigFilter(phcan, &sFilterConfig);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }
    ret = HAL_CAN_Start(phcan);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }
    ret = HAL_CAN_ActivateNotification(phcan, CAN_IT_RX_FIFO0_MSG_PENDING);
    if (ret != HAL_OK) {
        Can_ErrorHandler(ret);
    }
    
}

/**
  * @brief      向CAN总线发送信息
  * @param      phcan: 指向CAN句柄的指针
  * @param      txdata: 要发送的信息
  * @retval     无
  */
void Can_SendMessage(CAN_HandleTypeDef* phcan, CAN_TxHeaderTypeDef* pheader, uint8_t txdata[]) {
    uint32_t mailbox;
    /* Start the Transmission process */
    uint32_t ret = HAL_CAN_AddTxMessage(phcan, pheader, txdata, &mailbox);
    if (ret != HAL_OK) {
        /* Transmission request Error */
        Can_ErrorHandler(ret);
    }
}
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
