/**
 * DreamChaser Frame Header File
 * 
 * @File:        rmmotor.h
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#ifndef __MOTOR_DEV_H
#define __MOTOR_DEV_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes -------------------------------------------------------------------*/
#include "frame_config.h"
#include "can.h"
#include "pid_lib.h"
#include "can_util.h"
/* define ----------------------------------------------------------------------*/

/* Typedef --------------------------------------------------------------------*/
typedef enum{
	MOTOR_CNNECTED 	= 0,
	MOTOR_LOST		= 1,
	MOTOR_ID_ERROR 	= 2,
	MOTOR_ID_CONFLICT = 3,
	MOTOR_INIT_DONE = 4
} Motor_State_e;

typedef enum {
	MOTOR_NONE = 0,
	MOTOR_3508 = 1,
	MOTOR_2006 = 2,
	MOTOR_6020 = 3
}
Motor_ModelType_e;	// 电机型号枚举

typedef enum {
	MOTOR_TYPE_NOT_CONNECTED 	= 0,
	MOTOR_TYPE_CHASSIS  		= 1,
	MOTOR_TYPE_SHOOTER  		= 2,
	MOTOR_TYPE_FEEDER   		= 3,
	MOTOR_TYPE_GIMBAL_PITCH  	= 4,
	MOTOR_TYPE_GIMBAL_YAW		= 5
}
Motor_FuncType_e; 	// 电机功能类型枚举

typedef enum {
	MOTOR_PID_NONE			= 0,
	MOTOR_PID_SPEED 		= 1,
	MOTOR_PID_ANGLE 		= 2
}
Motor_PidType_e;	// 电机PID类型枚举

typedef enum{
	MOTOR_TXID_200 = 1,
	MOTOR_TXID_1FF = 2,
	MOTOR_TXID_2FF = 3
}
Motor_TXID_e;		// 电机发送ID枚举

typedef enum{
	MOTOR_DIR_CCW = 0,
	MOTOR_DIR_CW =1
}Motor_Dir_e;

typedef struct {
	// 电机反馈
	int16_t angle;
	int16_t speed;
	int16_t current;
	int8_t  temp;
	
	// 用于计算连续角度
	int16_t last_angle;
	int16_t round_count;
	
	uint8_t  has_init;			// 初始化标志
	float   init_offset;		// 初始化偏移量
	
	float   limited_angle;		// 限制角
	float   consequent_angle;	// 连续角
	
	uint32_t last_update_time;	// 编码器时间戳
}
Motor_Encoder_t;		//电机编码器结构体

typedef struct {
	Motor_State_e		state;		// 状态
	Motor_ModelType_e 	model_type;	// 商品类型 		
	Motor_FuncType_e 	func_type;	// 功能类型
	Motor_PidType_e		pid_type;	// PID类型 2:位置闭环 1：速度闭环 0：未连接
	Motor_Dir_e			dir;		// 安装方向
	uint8_t 			ID;			// 设置ID
	uint16_t			fdb_ID;		// 反馈ID
	Motor_TXID_e		txid;		//
	PID_PIDTypeDef pid[2];			// 1：位置环	0:速度环
	PID_PIDParamTypeDef pid_param[2];
	Motor_Encoder_t encoder;		// 编码器
	float output;
	
}
Motor_t;			// 电机结构体

typedef struct {
	Motor_t* motor[4];					//电机指针
	CAN_HandleTypeDef* 	can_handle;		//句柄
	CAN_TxHeaderTypeDef can_header;		//帧头
}
Motor_Group_t;	// 电机组结构体

typedef struct {
	Motor_PidType_e pid_type;
	PID_PIDParamTypeDef pid_param[2];
}
Motor_Param_t;	//电机参数结构体

/* Variables ------------------------------------------------------------------*/

/* Functions ------------------------------------------------------------------*/	
void Motor_EncoderDecodeCallback(CAN_HandleTypeDef* phcan, uint16_t stdid, uint8_t rxdata[]);
void Motor_GroupConfig(void);
void Motor_Config( Motor_t* pmotor, CAN_HandleTypeDef* hcan ,Motor_ModelType_e modeltype , 
	uint8_t ID,Motor_FuncType_e functype , Motor_PidType_e pidtype,Motor_Dir_e dir);

void Motor_Setoutput0(Motor_t* pmotor);
void Motor_GroupSetoutput0(Motor_Group_t* pmotor_group);

Motor_TXID_e Motor_GetMotorTXID(Motor_t * pmotor);
Motor_Group_t* Motor_GetGroupPtr(CAN_HandleTypeDef* can_handle,Motor_TXID_e txid);
void Motor_ResetData(Motor_t* pmotor);

void Motor_AddMotorOutput(Motor_t* pmotor,float addoutput);
void Motor_SetMotorOutput(Motor_t* pmotor,float output);
	
void Motor_CalcOutput(Motor_t* pmotor, Motor_Param_t* pparam);
void Motor_CalcSelfMotorOutput(Motor_t* pmotor) ;

void Motor_SendMotorGroupOutput(Motor_Group_t *pmotor_group);
#endif
#ifdef __cplusplus
}
#endif
/************************ (C) COPYRIGHT BIT DreamChaser *****END OF FILE****/
