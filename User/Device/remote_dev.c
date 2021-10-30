/**
 * DreamChaser Frame Source File
 * 
 * @File:        DR16_dev.c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "remote_dev.h"
const uint16_t Const_Remote_RX_BUFF_LEN             = 54;
const uint16_t Const_Remote_RX_FRAME_LEN            = 18;
const uint16_t Const_Remote_CHANNEL_VALUE_LIMIT     = 640;
const uint16_t Const_Remote_CHANNEL_VALUE_OFFSET    = 1024;
const uint16_t Const_Remote_CHANNEL_ERROR_LIMIT     = 700;
const uint16_t Const_Remote_REMOTE_OFFLINE_TIME     = 1000;
UART_HandleTypeDef* Const_Remote_UART_HANDLER ;
uint8_t Remote_RxData[Const_Remote_RX_BUFF_LEN];
Remote_RemoteDataTypeDef Remote_RemoteData;

/**
  * @brief      获取遥控器对象的指针
  * @param      无
  * @retval     遥控器对象的指针
  */
Remote_RemoteDataTypeDef* Remote_GetRemoteDataPtr() {
    return &Remote_RemoteData;
}

/**
  * @brief      移除遥控器偏移
  * @param      ch: 原始通道值
  * @retval     真实值
  */
int16_t Remote_CancelChannelOffset(uint16_t ch) {
    return (int16_t) ch - Const_Remote_CHANNEL_VALUE_OFFSET;
}
/**
  * @brief      转换为开关位置状态
  * @param      sw: 原始开关值
  * @retval     开关位置状态
  */
Remote_SwitchStateEnum Remote_ToSwitchState(uint8_t sw) {
    return (Remote_SwitchStateEnum) sw;
}

/**
  * @brief      遥控器键盘数据解码
  * @param      key: 遥控器键盘数据对象
  * @param      v: 原始遥控器键盘数据值
  * @retval     无
  */
void Remote_DecodeKeyboardData(Remote_KeyboardTypeDef* key, uint16_t v) {
    const uint16_t KEY_MASK_W       = 1 << 0;
    const uint16_t KEY_MASK_S       = 1 << 1;
    const uint16_t KEY_MASK_A       = 1 << 2;
    const uint16_t KEY_MASK_D       = 1 << 3;
    const uint16_t KEY_MASK_SHIFT   = 1 << 4;
    const uint16_t KEY_MASK_CTRL    = 1 << 5;
    const uint16_t KEY_MASK_Q       = 1 << 6;
    const uint16_t KEY_MASK_E       = 1 << 7;
    const uint16_t KEY_MASK_R       = 1 << 8;
    const uint16_t KEY_MASK_F       = 1 << 9;
    const uint16_t KEY_MASK_G       = 1 << 10;
    const uint16_t KEY_MASK_Z       = 1 << 11;
    const uint16_t KEY_MASK_X       = 1 << 12;
    const uint16_t KEY_MASK_C       = 1 << 13;
    const uint16_t KEY_MASK_V       = 1 << 14;
    const uint16_t KEY_MASK_B       = 1 << 15;

    key->w      = (v & KEY_MASK_W    ) > 0;
    key->s      = (v & KEY_MASK_S    ) > 0;
    key->a      = (v & KEY_MASK_A    ) > 0;
    key->d      = (v & KEY_MASK_D    ) > 0;
    key->shift  = (v & KEY_MASK_SHIFT) > 0;
    key->ctrl   = (v & KEY_MASK_CTRL ) > 0;
    key->q      = (v & KEY_MASK_Q    ) > 0;
    key->e      = (v & KEY_MASK_E    ) > 0;
    key->r      = (v & KEY_MASK_R    ) > 0;
    key->f      = (v & KEY_MASK_F    ) > 0;
    key->g      = (v & KEY_MASK_G    ) > 0;
    key->z      = (v & KEY_MASK_Z    ) > 0;
    key->x      = (v & KEY_MASK_X    ) > 0;
    key->c      = (v & KEY_MASK_C    ) > 0;
    key->v      = (v & KEY_MASK_V    ) > 0;
    key->b      = (v & KEY_MASK_B    ) > 0;
}

/**
  * @brief      初始化遥控器数据量
  * @param      rc: 指向遥控器对象的指针
  * @retval     无
  */
void Remote_ResetRemoteData(Remote_RemoteDataTypeDef* rc) {
    for (int i = 0; i < 5; ++i)
        rc->remote.ch[i] = 0;
    rc->remote.sl = Remote_ToSwitchState(0);
	rc->remote.sr = Remote_ToSwitchState(0);
    rc->mouse.x = 0;
    rc->mouse.y = 0;
    rc->mouse.z = 0;
    rc->mouse.l = 0;
    rc->mouse.r = 0;
    Remote_DecodeKeyboardData(&(rc->key), 0);
}

/**
  * @brief      初始化遥控器
  * @param      无
  * @retval     无
  */
void Remote_InitRemote(UART_HandleTypeDef* huart) {
	Const_Remote_UART_HANDLER  = huart;
    Remote_ResetRemoteData(&Remote_RemoteData);
    Remote_RemoteData.last_update_time = HAL_GetTick();
    Uart_InitUartDMA(Const_Remote_UART_HANDLER);
    Uart_ReceiveDMA(Const_Remote_UART_HANDLER, Remote_RxData, Const_Remote_RX_BUFF_LEN);
}

/**
  * @brief      判断遥控器是否离线
  * @param      rc: 指向遥控器对象的指针
  * @retval     是否离线（1为是，0为否）
  */
uint8_t Remote_IsRemoteOffline(Remote_RemoteDataTypeDef* rc) {
    uint32_t now = HAL_GetTick();
    if ((now - rc->last_update_time) > Const_Remote_REMOTE_OFFLINE_TIME)
        rc->state = Remote_STATE_LOST;
    return rc->state == Remote_STATE_LOST;
}

/**
  * @brief      判断遥控器数据是否出错
  * @param      rc: 指向遥控器对象的指针
  * @retval     是否出错（1为是，0为否）
  */
uint8_t Remote_IsRemoteError(Remote_RemoteDataTypeDef* rc) {
	const uint8_t REMOTE_OK      = 0;
	const uint8_t REMOTE_ERROR   = 1;

	for (int i = 0; i < 5; ++i)
		if (abs(rc->remote.ch[i]) > Const_Remote_CHANNEL_ERROR_LIMIT) {
			return REMOTE_ERROR;
		}
	if (rc->remote.sl == Remote_SWITCH_NULL ||rc->remote.sr == Remote_SWITCH_NULL) {
		return REMOTE_ERROR;
	}
	return REMOTE_OK;
}

/**
  * @brief      遥控器解码函数
  * @param      rc: 指针指向遥控器数据对象
  * @param      buff: 数据缓冲区
  * @param      rxdatalen: 数据长度
  * @retval     无
  */
void Remote_DecodeRemoteData(Remote_RemoteDataTypeDef* rc, uint8_t* buff, int rxdatalen) {

    if (rxdatalen != Const_Remote_RX_FRAME_LEN) {
        return;                                     // 数据长度错误
    }
    
    rc->state           = Remote_STATE_PENDING;     // 加锁防止互斥读写
    rc->last_update_time = HAL_GetTick();           // 时间戳
    
    /*buff[0]为ch0的低8位，buff[1]的低3位ch0的高3位*/
    rc->remote.ch[0]    = Remote_CancelChannelOffset(((uint16_t)buff[0] | (uint16_t)buff[1] << 8) & 0x07FF);
    /*buff[1]的高5位为ch1的低5位，buff[2]的低6位为ch1的高6位*/
    rc->remote.ch[1]    = Remote_CancelChannelOffset(((uint16_t)buff[1] >> 3 | (uint16_t)buff[2] << 5) & 0x07FF);
    /*buff[2]的高2位为ch2的低2位, buff[3]为ch2的中8位，buff[4]的低1位为ch2的高1位*/
    rc->remote.ch[2]    = Remote_CancelChannelOffset(((uint16_t)buff[2] >> 6 | (uint16_t)buff[3] << 2 | (uint16_t)buff[4] << 10) & 0x07FF);
    /*buff[4]的高7位为ch3的低7位，buff[5]的低4位为ch3的高4位*/
    rc->remote.ch[3]    = Remote_CancelChannelOffset(((uint16_t)buff[4] >> 1 | (uint16_t)buff[5] << 7) & 0x07FF);
    /*buff[5]的高2位为s1*/
    rc->remote.sl     = Remote_ToSwitchState((buff[5] >> 6) & 0x03);
    /*buff[6]的6，7位为s2*/
    rc->remote.sr     = Remote_ToSwitchState((buff[5] >> 4) & 0x03);
    /*buff[6],buff[7]为x*/
    rc->mouse.x         = ((int16_t)buff[6] | (int16_t)buff[7] << 8);
    /*buff[8],buff[9]为y*/
    rc->mouse.y         = ((int16_t)buff[8] | (int16_t)buff[9] << 8);
    /*buff[10],buff[11]为z*/
    rc->mouse.z         = ((int16_t)buff[10] | (int16_t)buff[11] << 8);
    /*buff[12]为左键*/
    rc->mouse.l         = buff[12];
    /*buff[13]为右键*/
    rc->mouse.r         = buff[13];
    /*buff[14],buff[15]为键盘值*/
    Remote_DecodeKeyboardData(&(rc->key), ((int16_t)buff[14]) | ((int16_t)buff[15] << 8));
    /*buff[16],buff[17]为拨轮*/
    rc->remote.ch[4]    = Remote_CancelChannelOffset(((uint16_t)buff[16] | (uint16_t)buff[17] << 8) & 0x07FF);

    if (rc->remote.ch[4] == -1024) rc->remote.ch[4] = 0;   // 部分遥控器拨轮有问题

    if (Remote_IsRemoteError(rc)) {
        rc->state       = Remote_STATE_ERROR;
        Remote_ResetRemoteData(rc);
        return;
    }
    rc->state           = Remote_STATE_CONNECTED;   // 解锁
}

/**
  * @brief      遥控器接收回调函数
  * @param      huart: 指向UART句柄的指针
  * @retval     无
  */
void Remote_RXCallback(UART_HandleTypeDef* huart) {
	/* clear DMA transfer complete flag */
	__HAL_DMA_DISABLE(huart->hdmarx);

	/* handle uart data from DMA */
	int rxdatalen = Const_Remote_RX_BUFF_LEN - Uart_DMACurrentDataCounter(huart->hdmarx->Instance);
	Remote_DecodeRemoteData(&Remote_RemoteData, Remote_RxData, rxdatalen);

	/* restart dma transmission */
	__HAL_DMA_SET_COUNTER(huart->hdmarx, Const_Remote_RX_BUFF_LEN);
	__HAL_DMA_ENABLE(huart->hdmarx);
}
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
