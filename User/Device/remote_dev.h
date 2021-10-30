/**
 * DreamChaser Frame Header File
 * 
 * @File:        DR16_dev.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __DR16_DEV_H
#define __DR16_DEV_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "usart.h"
#include "uart_util.h"
#include "stdlib.h"
/* const ----------------------------------------------------------------------*/
extern const uint16_t Const_Remote_RX_BUFF_LEN;
extern const uint16_t Const_Remote_RX_FRAME_LEN;
extern const uint16_t Const_Remote_CHANNEL_VALUE_LIMIT;
extern const uint16_t Const_Remote_CHANNEL_VALUE_OFFSET;
extern const uint16_t Const_Remote_CHANNEL_ERROR_LIMIT;
extern const uint16_t Const_Remote_REMOTE_OFFLINE_TIME;
	
/* Typedef --------------------------------------------------------------------*/
typedef enum {
    Remote_STATE_NULL       = 0,
    Remote_STATE_CONNECTED  = 1,
    Remote_STATE_LOST       = 2,
    Remote_STATE_ERROR      = 3,
    Remote_STATE_PENDING    = 4
}
Remote_RemoteStateEnum;

typedef enum {
    Remote_SWITCH_NULL      = 0,
    Remote_SWITCH_UP        = 1,
    Remote_SWITCH_DOWN      = 2,
    Remote_SWITCH_MIDDLE    = 3
} 
Remote_SwitchStateEnum;

typedef struct {
    uint8_t w, a, s, d, shift, ctrl, q, e, r, f, g, z, x, c, v, b;
} 
Remote_KeyboardTypeDef;

typedef struct {
    struct {
        int16_t                 ch[5]; //通道0 通道1 通道2 通道3 拨轮
        Remote_SwitchStateEnum  sl;  //开关0 开关1
		Remote_SwitchStateEnum  sr;  //开关0 开关1
    } remote;

    struct {
        int16_t  x;//鼠标x
        int16_t  y;//鼠标y
        int16_t  z;//鼠标z
        uint8_t  l;//鼠标左键
        uint8_t  r;//鼠标右键
    } mouse;

    Remote_KeyboardTypeDef key;//键盘
    Remote_RemoteStateEnum state;
	uint8_t pengding;
    uint32_t last_update_time;
} 
Remote_RemoteDataTypeDef;

/* Variables ------------------------------------------------------------------*/
extern UART_HandleTypeDef* Const_Remote_UART_HANDLER;

extern Remote_RemoteDataTypeDef Remote_RemoteData;

/* Functions ------------------------------------------------------------------*/	 
Remote_RemoteDataTypeDef* Remote_GetRemoteDataPtr(void);

void Remote_InitRemote(UART_HandleTypeDef* huart);
void Remote_RXCallback(UART_HandleTypeDef* huart);

uint8_t Remote_IsRemoteOffline(Remote_RemoteDataTypeDef* rc);


#endif
#ifdef __cplusplus
}
#endif
/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
