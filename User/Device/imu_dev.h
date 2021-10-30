/**
 * DreamChaser Frame Header File
 * 
 * @File:		imu_dev.h
 * @Brief:      IMU相关 
 * @Author:     Ju	Chaowen
 * @Modified:   2021年6月11日 17点04分
 *

安装要求
	1、IMU必须在云台复位状态下水平放置，因为Z轴上的传感器与其他轴不相同，否则yaw周角度零飘极大，暂时不支持倒置（z轴指向地）
	2、框架以IMU的x轴指向对应的云台指向来确定
	3、FRONT、BACK、LEFT、RIGHT

HI系列IMU发送通信协议
	帧头 2byte 0x5A 0xA5 
	包长 2byte 1-512 
	CRC  2byte LSB低字节在前
	数据包
	0x90 2byte 	用户ID	OFF
	0xA0 7byte 	加速度	OFF
	0xB0 7byte 	角速度	ON
	0xC0 7byte 	磁场强度OFF
	0xD0 7byte 	欧拉角 	ON
	0xD1 17byte 四元数	OFF
	0xF0 5byte	气压 	OFF
 */

#ifndef IMU_DEV_H
#define IMU_DEV_H
#ifdef __cplusplus
extern "C" {
#endif

#include "frame_config.h"
#if __IF_ENABLE( __DEV_IMU )

#include "stm32f4xx_hal.h"
#include "usart.h"
#include "uart_util.h"
/* Typedef --------------------------------------------------------------------*/	

typedef enum{
	HI226 = 0u,
	HI229 = 1u,
	HI220 = 2u,

} IMU_Type_e;

typedef enum{
	IMU_X2FRONT = 0u,	// x轴指向前
	IMU_X2BACK 	= 1u,	// x轴指向后
	IMU_X2LEFT 	= 2u,	// x轴指向左
	IMU_X2RIGHT = 3u	// x轴指向右
	
}IMU_Place_e;

typedef enum {
    IMU_STATE_NULL      = 0,
    IMU_STATE_CONNECTED = 1,
    IMU_STATE_LOST      = 2,
    IMU_STATE_PENDING   = 3,
	IMU_STATE_SOF_ERROR     = 4,
	IMU_STATE_CRC_ERROR     = 5

} IMU_State_e;

typedef enum{
	IMU_AXIS_OFFSET_NOT	 =	0,
	IMU_AXIS_OFFSET_DONE =	1
	
}IMU_AxisOffsetState_e;

typedef enum{
	IMU_AXIS_YAW 	= 0,
	IMU_AXIS_PITCH	= 1,
	IMU_AXIS_ROLL 	= 2
	
}IMU_Axis_e;

typedef enum{
	IMU_AXIS_ANGLE = 0,
	IMU_AXIS_SPEED = 1,
	IMU_AXIS_OFFSET = 2

}IMU_AxisData_e;

typedef __packed struct{
	float speed;
	float angle;
	float angle_offset;
	IMU_AxisOffsetState_e offset_state;
	
} IMU_Axis_t;
	
typedef __packed struct {
	// 原始数据
	float speed[3];				// 角速度 				单位	°/s
    float angle[3];				// 连续角度【-∞，＋∞】	单位	°
    float angle_raw[3];			// 原始角度【0，360】	单位	°
	float angle_raw_last[3];	// 上一次的原始角度		单位	°
    int   count[3];				// 圈数
    
	IMU_Axis_t yaw;
	IMU_Axis_t pitch;
	IMU_Axis_t roll;
	
	IMU_Type_e type;	// 类型
	IMU_Place_e	place;	// 放置方式
    IMU_State_e state;
	
    uint32_t last_update_time;
	
} IMU_Data_t;

/* Variables ------------------------------------------------------------------*/
//extern const uint16_t Const_IMU_RX_BUFF_LEN;
extern UART_HandleTypeDef* Const_IMU_UART_HANDLER;
extern IMU_Data_t IMU_Data;

/* Functions ------------------------------------------------------------------*/	
IMU_Data_t* IMU_GetIMUDataPtr(void);
void IMU_DevInit(IMU_Type_e type,IMU_Place_e place,UART_HandleTypeDef* huart) ;
void IMU_RXCallback(UART_HandleTypeDef* huart);

// 对外接口
float IMU_GetIMUData(IMU_Axis_e axis,IMU_AxisData_e data);
IMU_AxisOffsetState_e IMU_GetAxisOffsetState(IMU_Axis_e axis);
void IMU_SetAxisOffset(IMU_Axis_e axis,float offset);
#endif

#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
