/**
 * DreamChaser Frame Source File
 * 
 * @File:       imu_dev.c
 * @Brief:      IMU相关
 * @Author:     Ju	Chaowen
 * @Modified:   2021年6月11日 17点03分 
 *
 */

#include "imu_dev.h"
#if __IF_ENABLE( __DEV_IMU )

UART_HandleTypeDef* Const_IMU_UART_HANDLER ;
const uint16_t Const_IMU_RX_BUFF_LEN = 100;			//IMU接收缓冲区长度
uint8_t IMU_RxData[Const_IMU_RX_BUFF_LEN];			//IMU接收数组

IMU_Data_t IMU_Data;	// IMU数据

uint16_t payload_len;	// IMU数据包长度
uint16_t crc,tmp;		// CRC计算结果、数据包中的CRC

//******************** Begin of IMU Init ********************//
/**
  * @brief      获取IMU数据对象的指针
  * @param      无
  * @retval     IMU数据对象的指针
  */
IMU_Data_t* IMU_GetIMUDataPtr() {
    return &IMU_Data;
}

/**
  * @brief      重置IMU单个轴向的数据
  * @param      axis: 指向IMU单个轴向的指针
  * @retval     无
  */
void IMU_Axis_Reset(IMU_Axis_t * axis){
	axis->angle = 0;
	axis->speed = 0;
	axis->angle_offset = 0;
	axis->offset_state = IMU_AXIS_OFFSET_NOT;
}

/**
  * @brief      重置IMU数据对象
  * @param      imu: 指针指向IMU数据对象
  * @retval     无
  */
void IMU_ResetIMUData(IMU_Data_t* imu) {
	for (int i = 0; i < 3; ++i) {
        imu->speed[i]       		= 0;
        imu->angle[i]    = 0;
        imu->angle_raw[i]  = 0;
        imu->angle_raw_last[i]   = 0;
        imu->count[i]       = 0;
	}
	IMU_Axis_Reset(&imu->yaw);
	IMU_Axis_Reset(&imu->pitch);
	IMU_Axis_Reset(&imu->roll);
	imu->state = IMU_STATE_NULL;
}

/**
  * @brief      初始化IMU
  * @param      无
  * @retval     无
  */
void IMU_DevInit(IMU_Type_e type,IMU_Place_e place,UART_HandleTypeDef* huart) {
	Const_IMU_UART_HANDLER = huart;
	IMU_Data.type 	= type;
	IMU_Data.place 	= place;
	IMU_ResetIMUData(&IMU_Data);
	HAL_Delay(1000);
    Uart_InitUartDMA(Const_IMU_UART_HANDLER);
    Uart_ReceiveDMA(Const_IMU_UART_HANDLER, IMU_RxData, Const_IMU_RX_BUFF_LEN);
}
//******************** End of IMU Init ********************//

//******************** HI 系列解码相关 开始 ********************//
/**
  * @brief      获取HI_IMU的速度
  * @param      buff：	缓冲区
  * @retval     是否离线（1为是，0为否）
  */
void Hi_get_speed(IMU_Data_t * imu,uint8_t *buff){
	int16_t temp[3];
	
	// 拼接
	temp[0] = (int16_t)((buff[2] << 8) | buff[1]);	//pitch
	temp[1] = (int16_t)((buff[4] << 8) | buff[3]);	//roll
	temp[2] = (int16_t)((buff[6] << 8) | buff[5]);	//yaw
	
	// 精度和类型转换
	for(int i = 0;i < 3;i++){ imu->speed[i] = (float)temp[i] * 0.1f;}
	
	// 坐标转换
	switch(imu->place){
		case IMU_X2FRONT: 	imu->yaw.speed = - imu->speed[2] ; imu->pitch.speed =  - imu->speed[1];imu->roll.speed =  + imu->speed[0];break;
		case IMU_X2BACK:	imu->yaw.speed = - imu->speed[2] ; imu->pitch.speed =  + imu->speed[1];imu->roll.speed =  - imu->speed[0];break;
		case IMU_X2LEFT:	imu->yaw.speed = - imu->speed[2] ; imu->pitch.speed =  - imu->speed[0];imu->roll.speed =  - imu->speed[1];break;
		case IMU_X2RIGHT:	imu->yaw.speed = - imu->speed[2] ; imu->pitch.speed =  + imu->speed[0];imu->roll.speed =  + imu->speed[1];break;
	}
}

/**
  * @brief      获取HI_IMU的角度
  * @param      buff：	缓冲区
  * @retval     是否离线（1为是，0为否）
  */
void Hi_get_angle(IMU_Data_t * imu,uint8_t *buff){
	int16_t temp[3];
	
	// 历史数据更新
	for(int i=0;i<3;i++){ imu->angle_raw_last[i] = imu->angle_raw[i];}
	
	// 拼接
	temp[0] = (int16_t)((buff[2] << 8) | buff[1]);	
	temp[1] = (int16_t)((buff[4] << 8) | buff[3]);	
	temp[2] = (int16_t)((buff[6] << 8) | buff[5]);	
	
	// 精度和类型转换
	imu->angle_raw[0] = (float)(temp[0] / 100.0f);	
	imu->angle_raw[1] = (float)(temp[1] / 100.0f);  
	imu->angle_raw[2] = (float)(temp[2] / 10.0f);	
	
	// 连续角计算
	for(int i=0;i<3;i++){
		if(imu->angle_raw[i] - imu->angle_raw_last[i]<-181)	imu->count[i]++;
		if(imu->angle_raw[i] - imu->angle_raw_last[i]> 181)	imu->count[i]--;
		imu->angle[i] = (float)imu->count[i]*360 + imu->angle_raw[i];
	}
	
	// 坐标转换
	switch(imu->place){
		case IMU_X2FRONT: 	{
			imu->yaw.angle 		=  -(imu->angle[2] - imu->yaw.angle_offset	);
			imu->pitch.angle 	=  -(imu->angle[0] - imu->pitch.angle_offset);
			imu->roll.angle 	=   (imu->angle[1] - imu->roll.angle_offset	);
			break;
		}
		case IMU_X2BACK:	{
			imu->yaw.angle 		=  -(imu->angle[2] - imu->yaw.angle_offset	);
			imu->pitch.angle 	=   (imu->angle[0] - imu->pitch.angle_offset);
			imu->roll.angle 	=  -(imu->angle[1] - imu->roll.angle_offset	);
			break;
		}
		case IMU_X2LEFT:	{
			imu->yaw.angle 		=  -(imu->angle[2] - imu->yaw.angle_offset	);
			imu->pitch.angle 	=  -(imu->angle[1] - imu->pitch.angle_offset);
			imu->roll.angle 	=  -(imu->angle[0] - imu->roll.angle_offset	);
			break;
		}
		case IMU_X2RIGHT:	{
			imu->yaw.angle 		=  -(imu->angle[2] - imu->yaw.angle_offset	);
			imu->pitch.angle 	=   (imu->angle[1] - imu->pitch.angle_offset);
			imu->roll.angle 	=   (imu->angle[0] - imu->roll.angle_offset	);
			break;
		}
	}
}

/**
  * @brief   	crc校验
  * @param  	currectCrc
  * @param    	src
  * @param    	lengthInBytes
  * @retval 	
  */
static void HI_crc16_update(uint16_t *currectCrc, const uint8_t *src, uint32_t lengthInBytes) {
	uint32_t crc = *currectCrc; 
	uint32_t j;
	for (j=0; j < lengthInBytes; ++j) { 
		uint32_t i; 
		uint32_t byte = src[j]; 
		crc ^= byte << 8; 
		for (i = 0; i < 8; ++i) { 
			uint32_t temp = crc << 1; 
			if (crc & 0x8000) { 
				temp ^= 0x1021; 
			}
			crc = temp; 
		}
	}
	*currectCrc = crc; 
}

/**
  * @brief   	HI系列IMU解码
  * @param  	currectCrc
  * @param    	src
  * @param    	lengthInBytes
  * @retval 	
  */
void HI_Decode(IMU_Data_t* imu, uint8_t* buff){
	// 帧头校验	
	if (buff[0] == 0X5A && buff[1] == 0XA5) {
		// PAYLOAD长度 规定为20
		payload_len = buff[2] + (buff[3] << 8);		
		// HI CRC校验
		crc = 0;
		HI_crc16_update(&crc, buff, 4);
		HI_crc16_update(&crc, buff+6, payload_len);
		tmp = (buff[5] << 8) | buff[4];
		
		// CRC校验通过
		if(tmp == crc){
			for(int i = 0;i < payload_len;i++){
				switch(buff[i+6]){
					case 0x90:								i += 1;break;
					case 0xA0:								i += 6;break;
					case 0xB0:Hi_get_speed(imu,buff +i+6);	i += 6;break;
					case 0xC0:								i += 6;break;
					case 0xD0:Hi_get_angle(imu,buff +i+6);	i += 6;break;
					case 0xD1:								i += 16;break;
					case 0xF0:								i += 4;break;
					case 0x91:								i += 75;break;
					default:break;
				}
			}
		}
		// CRC校验错误
		else{	imu->state = IMU_STATE_CRC_ERROR;	return;}
	}
	
	// 帧头错误
	else{	imu->state = IMU_STATE_SOF_ERROR;	return;}
}
//******************** END of HI IMU ********************//

//******************** BEGIN of IMU Decode ********************//
/**
  * @brief      IMU数据解码函数
  * @param      referee: 指针指向IMU数据对象
  * @param      buff: 数据缓冲区
  * @retval     无
  */
void IMU_Decode(IMU_Data_t* imu, uint8_t* buff) 
{
	// 加锁防止互斥读写
	imu->state              = IMU_STATE_PENDING; 
	imu->last_update_time   = HAL_GetTick(); 		//更新时间戳
	
	// 超核电子HI系列IMU，本质是BMI055外接单片机
	if(imu->type == HI226 || imu->type == HI229 ||imu->type == HI220){
		HI_Decode(imu,buff);
	}
	// 其他系列IMU
	else{}
	
	imu->state = IMU_STATE_CONNECTED; // 解锁
}

/**
  * @brief      IMU串口回调函数
  * @param      huart: 指针指向串口句柄
  * @retval     无
  */
void IMU_RXCallback(UART_HandleTypeDef* huart) {
	__HAL_DMA_DISABLE(huart->hdmarx);
	IMU_Decode(&IMU_Data, IMU_RxData);
	__HAL_DMA_SET_COUNTER(huart->hdmarx, Const_IMU_RX_BUFF_LEN);
	__HAL_DMA_ENABLE(huart->hdmarx);
}
//******************** END of IMU Decode ********************//

//******************** BEGIN of IMU API ********************//
/**
  * @brief      获取IMU轴向数据
  * @param      axis：	IMU轴向数据结构体指针
  * @param      data：	IMU轴向数据类型枚举
  * @retval     value：	返回数据
  */
float IMU_GetIMUAxisData(IMU_Axis_t * axis,IMU_AxisData_e data){
	float value;
	switch(data){
		case IMU_AXIS_ANGLE:	value = axis->angle;break;
		case IMU_AXIS_SPEED:	value = axis->speed;break;
		case IMU_AXIS_OFFSET:	value = axis->angle_offset;break;
	}
	return value;
}

/**
  * @brief      获取IMU轴向数据
  * @param      axis：	IMU轴向枚举
  * @param      data：	IMU轴向数据类型枚举
  * @retval     value：	返回数据
  */
float IMU_GetIMUData(IMU_Axis_e axis,IMU_AxisData_e data){
	float value;
	switch(axis){
		case IMU_AXIS_YAW:	{value = IMU_GetIMUAxisData(&IMU_Data.yaw,		data);break;}
		case IMU_AXIS_PITCH:{value = IMU_GetIMUAxisData(&IMU_Data.pitch,	data);break;}
		case IMU_AXIS_ROLL:	{value = IMU_GetIMUAxisData(&IMU_Data.roll,		data);break;}
	}
	return value;
}

/**
  * @brief      获取IMU轴向偏置状态
  * @param      axis：			IMU轴向枚举
  * @retval     offset_state:	IMU轴向偏置初始化状态
  */
IMU_AxisOffsetState_e IMU_GetAxisOffsetState(IMU_Axis_e axis){
	IMU_AxisOffsetState_e offset_state;
	switch(axis){
		case IMU_AXIS_YAW: 	offset_state = IMU_Data.yaw.offset_state;break;
		case IMU_AXIS_PITCH:offset_state = IMU_Data.pitch.offset_state;break;
		case IMU_AXIS_ROLL:	offset_state = IMU_Data.roll.offset_state;break;
	}
	return offset_state;
}

/**
  * @brief      设置IMU轴向偏置
  * @param      axis：			IMU轴向枚举
  * @param      offset：		IMU轴向偏置
  * @retval     None
  */
void IMU_SetAxisOffset(IMU_Axis_e axis,float offset){
	switch(axis){
		case IMU_AXIS_YAW:	IMU_Data.yaw.angle_offset 	= offset;IMU_Data.yaw.offset_state 	 = IMU_AXIS_OFFSET_DONE;break;
		case IMU_AXIS_PITCH:IMU_Data.pitch.angle_offset = offset;IMU_Data.pitch.offset_state = IMU_AXIS_OFFSET_DONE;break;
		case IMU_AXIS_ROLL:	IMU_Data.roll.angle_offset 	= offset;IMU_Data.roll.offset_state  = IMU_AXIS_OFFSET_DONE;break;
	}
}
//******************** END of IMU API ********************//
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
