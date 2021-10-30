/**
 * DreamChaser Frame Source File
 * 
 * @File:        .c
 * @Brief:       
 * @Author:      Ju	Chaowen
 * @Modified:    
 *
 */
#include "motor_dev.h"
const float CONST_REDUCTION_RATIO_3508 = 19.2;	// 官方数据：3591/187
const float CONST_REDUCTION_RATIO_2006 = 36;	
const float CONST_REDUCTION_RATIO_6020 = 1;

const uint32_t Const_Motor_MOTOR_OFFLINE_TIME = 1000;

const uint16_t CONST_MOTOR_FDB_ID[12]={0x0000,0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,0x0010,0x0200,0x0400};
uint16_t Motor_Can1IDList = 0x0000;
uint16_t Motor_Can2IDList = 0x0000;

#if FRAME_CAN1
Motor_Group_t Motors_can1_200;	// 1234
Motor_Group_t Motors_can1_1ff;	// 5678
Motor_Group_t Motors_can1_2ff;	// 9AB
#endif

#if FRAME_CAN2
Motor_Group_t Motors_can2_200;	// 1234
Motor_Group_t Motors_can2_1ff;	// 5678
Motor_Group_t Motors_can2_2ff;	// 9AB
#endif

/**
  * @brief      清空电机PID
  * @param      pmotor: 指向电机对象的指针
  * @retval     无
  */
void Motor_ResetData(Motor_t* pmotor) {
    if (pmotor->pid_type == MOTOR_PID_NONE) return;
		else if (pmotor->pid_type == MOTOR_PID_SPEED){
			PID_ClearData(&(pmotor->pid[0]));
		}
		else if (pmotor->pid_type == MOTOR_PID_NONE){
					PID_ClearData(&(pmotor->pid[0]));
			PID_ClearData(&(pmotor->pid[1]));
		}
}

void Motor_EncoderDataInit(Motor_t* pmotor){
	pmotor->encoder.last_angle = 0;
	pmotor->encoder.has_init = 0;
	pmotor->encoder.round_count = 0;
	pmotor->encoder.last_update_time = 0;
}
/**
  * @brief      清空电机组PID
  * @param      pmotor_group: 指针指向电机组对象
  * @retval     无
  */
void Motor_ResetGroupData(Motor_Group_t* pmotor_group) {
	for (int i = 0; i < 4; ++i) {
		Motor_ResetData(pmotor_group->motor[i]);
	}
}



void Motor_Config( Motor_t* pmotor, CAN_HandleTypeDef* hcan ,Motor_ModelType_e modeltype , 
	uint8_t ID,Motor_FuncType_e functype , Motor_PidType_e pidtype,Motor_Dir_e dir){
	// ID检验
	uint8_t fdbID;
	if(modeltype == MOTOR_6020){
		fdbID = ID + 4;
		if (fdbID > 11 || fdbID < 5){
			pmotor->state = MOTOR_ID_ERROR;
			return ;
		}
	}
	else{
		fdbID = ID;
		if (fdbID > 8){
			pmotor->state = MOTOR_ID_ERROR;
			return ;
		}
	}
	
	// ID冲突检测 与 ID登记
#if FRAME_CAN1
	if (hcan == &hcan1){
		if(Motor_Can1IDList & CONST_MOTOR_FDB_ID[fdbID]){
			pmotor->state = MOTOR_ID_CONFLICT;
			return ;
		}
		Motor_Can1IDList	|= CONST_MOTOR_FDB_ID[fdbID];
		if (fdbID <= 4){
			Motors_can1_200.motor[fdbID - 1] = pmotor;
			pmotor->txid = MOTOR_TXID_200;
		}
		else if(fdbID <= 8){
			Motors_can1_1ff.motor[fdbID - 5] = pmotor;
			pmotor->txid = MOTOR_TXID_1FF;
		}
		else{
			Motors_can1_2ff.motor[fdbID - 9] = pmotor;
			pmotor->txid = MOTOR_TXID_2FF;
		}
	}
#endif
#if FRAME_CAN2
	if (hcan == &hcan2){
		if(Motor_Can2IDList & CONST_MOTOR_FDB_ID[fdbID]){
			pmotor->state = MOTOR_ID_CONFLICT;
			return ;
		}
		Motor_Can2IDList	|= CONST_MOTOR_FDB_ID[fdbID];
		if (fdbID <= 4){
			Motors_can2_200.motor[fdbID - 1 ] = pmotor;
			pmotor->txid = MOTOR_TXID_200;
		}
		else if(fdbID <= 8){
			Motors_can2_1ff.motor[fdbID - 5]  = pmotor;
			pmotor->txid = MOTOR_TXID_1FF;
		}
		else{
			Motors_can2_2ff.motor[fdbID - 9]  = pmotor;
			pmotor->txid = MOTOR_TXID_2FF;
		}
	}
#endif
	// 配置信息
	pmotor->state		= MOTOR_INIT_DONE;	// 电机状态
	pmotor->model_type 	= modeltype;		// 电机型号
	pmotor->func_type 	= functype;			// 电机功能类型
	pmotor->pid_type 	= pidtype;			// PID_类型
	pmotor->ID 			= ID;				// ID
	pmotor->fdb_ID 		= 0x200 + fdbID;	// 反馈ID
	pmotor->dir			= dir;				// 控制方向
	Motor_EncoderDataInit( pmotor);
	Motor_ResetData( pmotor );
}

/**
  * @brief      初始化电机组
  * @param      pgroup: 指向电机组的指针
  * @retval     None
  */
void Motor_GroupInit(Motor_Group_t* pgroup, CAN_HandleTypeDef* phcan, uint16_t stdid) {
	pgroup->can_handle = phcan;
	Can_InitTxHeader(&(pgroup->can_header), stdid);
	for (int i = 0 ; i < 4 ; i++){
		pgroup->motor[i] = NULL;
	}
}

/**
  * @brief      初始化所有电机组
  * @param      None
  * @retval     None
  */
void Motor_GroupConfig(void){
#if FRAME_CAN1
	Motor_GroupInit(&Motors_can1_200,&hcan1,0x200);
	Motor_GroupInit(&Motors_can1_1ff,&hcan1,0x1ff);
	Motor_GroupInit(&Motors_can1_2ff,&hcan1,0x2ff);
	Can_InitFilterAndStart(&hcan1);
#endif
#if FRAME_CAN2
	Motor_GroupInit(&Motors_can2_200,&hcan2,0x200);
	Motor_GroupInit(&Motors_can2_1ff,&hcan2,0x1ff);
	Motor_GroupInit(&Motors_can2_2ff,&hcan2,0x2ff);
	Can_InitFilterAndStart(&hcan2);
#endif
}

/**
  * @brief      解码电机编码器反馈的数据
  * @param      rxdata: CAN接受到的信息
  * @param      pmotor: 指向电机对象的指针
  * @retval     无
  */
void Motor_Decode(uint8_t rxdata[], Motor_t* pmotor) {
	if (pmotor == NULL) return;
	
	pmotor->encoder.last_angle = pmotor->encoder.angle;
	pmotor->encoder.angle   = rxdata[0] << 8 | rxdata[1];
	pmotor->encoder.speed   = rxdata[2] << 8 | rxdata[3];
	pmotor->encoder.current = rxdata[4] << 8 | rxdata[5];
	pmotor->encoder.temp    = rxdata[6];
	
	//底盘电机
	if (pmotor->func_type == MOTOR_TYPE_CHASSIS) {
		if(pmotor->model_type == MOTOR_3508)
			PID_SetFdb(&(pmotor->pid[0]), (float)pmotor->encoder.speed / CONST_REDUCTION_RATIO_3508);
		}
	
		else if (pmotor->func_type == MOTOR_TYPE_GIMBAL_PITCH || pmotor->func_type == MOTOR_TYPE_FEEDER|| pmotor->func_type == MOTOR_TYPE_GIMBAL_YAW) 
		{
		// 计算角度差和圈数
		int diff = pmotor->encoder.angle - pmotor->encoder.last_angle;      //以机械角度增加为正
		if (diff < -5500)           // 正向转过一圈
			pmotor->encoder.round_count++;
		else if (diff > 5500)       // 反向转过一圈
			pmotor->encoder.round_count--;
		
		if (pmotor->func_type == MOTOR_TYPE_GIMBAL_YAW || pmotor->func_type == MOTOR_TYPE_GIMBAL_PITCH) {
			// 计算连续角度
			pmotor->encoder.consequent_angle = (float)pmotor->encoder.round_count * 360.0f + (float)pmotor->encoder.angle / 8192.0f * 360.0f;
			pmotor->encoder.limited_angle = (float)pmotor->encoder.angle / 8192.0f * 360.0f;
		}
		else if (pmotor->func_type == MOTOR_TYPE_FEEDER) {
			// 计算转轴角度 由于减速比 需要除以36
			pmotor->encoder.consequent_angle = (float)pmotor->encoder.round_count * 10.0f + 
											   (float)pmotor->encoder.angle / 8192.0f * 10.0f;
			// 处理count溢出问题
			if (pmotor->encoder.round_count > 10000) {
				pmotor->encoder.consequent_angle -= 10 * pmotor->encoder.round_count; 
				PID_AddRef(&(pmotor->pid[1]), -10 * pmotor->encoder.round_count);
				pmotor->encoder.round_count = 0;
			}
			if (pmotor->encoder.round_count < -10000) {
				pmotor->encoder.consequent_angle += 10 * pmotor->encoder.round_count; 
				PID_AddRef(&(pmotor->pid[1]), 10 * pmotor->encoder.round_count);
				pmotor->encoder.round_count = 0;
			}
			// 拨弹电机初始化
			if (!pmotor->encoder.has_init) {
				pmotor->encoder.init_offset = pmotor->encoder.consequent_angle;
				pmotor->encoder.has_init = 1;
			}
		}
		}
	pmotor->encoder.last_update_time = HAL_GetTick(); //时间戳
}
/**
  * @brief      电机编码器解码回调函数
  * @param      canid: CAN 句柄号
  * @param      stdid: CAN 标识符
  * @retval     无
  */
void Motor_EncoderDecodeCallback(CAN_HandleTypeDef* phcan, uint16_t stdid, uint8_t rxdata[]) {
#if FRAME_CAN1
	if (phcan == &hcan1){
		switch(stdid){
			case 0x201:Motor_Decode(rxdata,Motors_can1_200.motor[0]);break;
			case 0x202:Motor_Decode(rxdata,Motors_can1_200.motor[1]);break;
			case 0x203:Motor_Decode(rxdata,Motors_can1_200.motor[2]);break;
			case 0x204:Motor_Decode(rxdata,Motors_can1_200.motor[3]);break;
			case 0x205:Motor_Decode(rxdata,Motors_can1_1ff.motor[0]);break;
			case 0x206:Motor_Decode(rxdata,Motors_can1_1ff.motor[1]);break;
			case 0x207:Motor_Decode(rxdata,Motors_can1_1ff.motor[2]);break;
			case 0x208:Motor_Decode(rxdata,Motors_can1_1ff.motor[3]);break;
			case 0x209:Motor_Decode(rxdata,Motors_can1_2ff.motor[0]);break;
			case 0x20A:Motor_Decode(rxdata,Motors_can1_2ff.motor[1]);break;
			case 0x20B:Motor_Decode(rxdata,Motors_can1_2ff.motor[2]);break;
		}
	}
#endif
#if FRAME_CAN2
	if (phcan == &hcan2){
		switch(stdid){
			case 0x201:Motor_Decode(rxdata,Motors_can2_200.motor[0]);break;
			case 0x202:Motor_Decode(rxdata,Motors_can2_200.motor[1]);break;	
			case 0x203:Motor_Decode(rxdata,Motors_can2_200.motor[2]);break;
			case 0x204:Motor_Decode(rxdata,Motors_can2_200.motor[3]);break;
			case 0x205:Motor_Decode(rxdata,Motors_can2_1ff.motor[0]);break;
			case 0x206:Motor_Decode(rxdata,Motors_can2_1ff.motor[1]);break;
			case 0x207:Motor_Decode(rxdata,Motors_can2_1ff.motor[2]);break;
			case 0x208:Motor_Decode(rxdata,Motors_can2_1ff.motor[3]);break;
			case 0x209:Motor_Decode(rxdata,Motors_can2_2ff.motor[0]);break;
			case 0x20A:Motor_Decode(rxdata,Motors_can2_2ff.motor[1]);break;
			case 0x20B:Motor_Decode(rxdata,Motors_can2_2ff.motor[2]);break;
		}
	}
#endif
}

/**
  * @brief      初始化电机参数
  * @param      pparam: 指向电机参数对象的指针
  * @retval     无
  */
void Motor_ParamInit(Motor_Param_t* pparam, Motor_PidType_e pidtype, float pidpara[][5]) {
    pparam->pid_type = pidtype;
	if (pidtype == MOTOR_PID_NONE ) return;
	if (pidtype == MOTOR_PID_SPEED){
		PID_ParamInit(&(pparam->pid_param[0]), pidpara[0][0], pidpara[0][1], pidpara[0][2], pidpara[0][3], pidpara[0][4]);
	}
	else{
		PID_ParamInit(&(pparam->pid_param[0]), pidpara[0][0], pidpara[0][1], pidpara[0][2], pidpara[0][3], pidpara[0][4]);
		PID_ParamInit(&(pparam->pid_param[1]), pidpara[1][0], pidpara[1][1], pidpara[1][2], pidpara[1][3], pidpara[1][4]);
	}
}

/**
  * @brief      获取电机PID目标值
  * @param      pmotor: 指向电机对象的指针
  * @retval     目标值
  */
float Motor_GetMotorRef(Motor_t* pmotor) {
    if ( pmotor->pid_type == MOTOR_PID_NONE) return 0.0f;
    if ( pmotor->pid_type == MOTOR_PID_SPEED) {
        return PID_GetRef(&(pmotor->pid[0]));
    }
    else if (pmotor->pid_type == MOTOR_PID_ANGLE) {
        return PID_GetRef(&(pmotor->pid[1]));
    }
    return 0;
}
/**
  * @brief      设置电机PID目标值
  * @param      pmotor: 指向电机对象的指针
  * @param      ref: 目标值
  * @retval     无
  */
void Motor_SetMotorRef(Motor_t* pmotor, float ref) {
	if ( pmotor->pid_type == MOTOR_PID_NONE) return ;
	if ( pmotor->pid_type == MOTOR_PID_SPEED) {
		PID_SetRef(&(pmotor->pid[0]), ref);
	}
	else if (pmotor->pid_type == MOTOR_PID_ANGLE) {
	   PID_SetRef(&(pmotor->pid[1]), ref);
	}
}
/**
  * @brief      获取电机PID速度反馈值
  * @param      pmotor: 指向电机对象的指针
  * @retval     反馈值
  */
float Motor_GetSpeedFdb(Motor_t* pmotor) {
    if (pmotor->pid_type == MOTOR_PID_NONE) return 0.0f;
    return PID_GetFdb(&(pmotor->pid[0]));
}
/**
  * @brief      获取电机PID角度反馈值
  * @param      pmotor: 指向电机对象的指针
  * @retval     反馈值
  */
float Motor_GetAngleFdb(Motor_t* pmotor) {
    if (pmotor->pid_type == MOTOR_PID_NONE) return 0.0f;
    return PID_GetFdb(&(pmotor->pid[1]));
}
/**
  * @brief      设置电机PID速度反馈值
  * @param      pmotor: 指向电机对象的指针
  * @param      fdb: 反馈值
  * @retval     无
  */
void Motor_SetSpeedFdb(Motor_t* pmotor, float fdb) {
	if (pmotor->pid_type == MOTOR_PID_NONE) return;
	PID_SetFdb(&(pmotor->pid[0]), fdb);
}

/**
  * @brief      设置电机PID角度反馈值
  * @param      pmotor: 指向电机对象的指针
  * @param      fdb: 反馈值
  * @retval     无
  */
void Motor_SetAngleFdb(Motor_t* pmotor, float fdb) {
	if (pmotor->pid_type == MOTOR_PID_NONE) return;
	PID_SetFdb(&(pmotor->pid[1]), fdb);
}


Motor_TXID_e Motor_GetMotorTXID(Motor_t * pmotor){
	return pmotor->txid;
}
/**
  * @brief      计算电机PID输出
  * @param      pmotor: 指向电机对象的指针
  * @param      pparam: 指向电机参数对象的指针
  * @retval     无
  */
void Motor_CalcOutput(Motor_t* pmotor, Motor_Param_t* pparam) {
    if (pmotor->pid_type == MOTOR_PID_NONE) return;
    if ( pmotor->pid_type == MOTOR_PID_SPEED) {
		PID_Calc(&(pmotor->pid[0]), &(pparam->pid_param[0]));
    }else if (pmotor->pid_type == MOTOR_PID_ANGLE) {
        PID_Calc(&(pmotor->pid[1]), &(pparam->pid_param[1]));
        PID_SetRef(&(pmotor->pid[0]), PID_GetOutput(&(pmotor->pid[1])));
        PID_Calc(&(pmotor->pid[0]), &(pparam->pid_param[0]));
    }
}

Motor_Group_t* Motor_GetGroupPtr(CAN_HandleTypeDef* can_handle,Motor_TXID_e txid){
#if FRAME_CAN1
	if(can_handle == &hcan1){
		switch (txid){
			case MOTOR_TXID_200: return &Motors_can1_200;
			case MOTOR_TXID_1FF: return &Motors_can1_1ff;
			case MOTOR_TXID_2FF: return &Motors_can1_2ff;
		}
	}
#endif
#if FRAME_CAN2
	if(can_handle == &hcan2){
		switch (txid){
			case MOTOR_TXID_200: return &Motors_can2_200;
			case MOTOR_TXID_1FF: return &Motors_can2_1ff;
			case MOTOR_TXID_2FF: return &Motors_can2_2ff;
		}
	}
#endif
	return NULL;
}
/**
  * @brief      计算电机组PID输出
  * @param      pmotor_group: 指针指向电机组对象
  * @param      pparam: 指向电机参数对象的指针
  * @retval     无
  */
void Motor_CalcGroupOutput(Motor_Group_t* pmotor_group, Motor_Param_t* pparam) {
    for (int i = 0; i < 4; ++i) {
        Motor_CalcOutput(pmotor_group->motor[i], pparam);
    }
}

/**
  * @brief      计算电机PID输出（用结构体自身参数）
  * @param      pmotor: 指向电机对象的指针
  */
void Motor_CalcSelfMotorOutput(Motor_t* pmotor) {
    if (pmotor->pid_type == MOTOR_PID_NONE) return;
    if ( pmotor->pid_type == MOTOR_PID_SPEED) {
		PID_Calc(&(pmotor->pid[0]), &(pmotor->pid_param[0]));
	}
	else if (pmotor->pid_type == MOTOR_PID_ANGLE) {
		PID_Calc(&(pmotor->pid[1]), &(pmotor->pid_param[1]));
		PID_SetRef(&(pmotor->pid[0]), PID_GetOutput(&(pmotor->pid[1])));
		PID_Calc(&(pmotor->pid[0]), &(pmotor->pid_param[0]));
	}
}
void Motor_SetMotorOutput(Motor_t* pmotor,float output){
	pmotor->output = output;
}

void Motor_AddMotorOutput(Motor_t* pmotor,float addoutput){
	pmotor->output = pmotor->pid[0].output + addoutput;
}

/**
  * @brief      电机输出置零
  * @param      pmotor_group: 指针指向电机组对象
  * @retval     无
  */
void Motor_Setoutput0(Motor_t* pmotor){
	if (pmotor == NULL) return;
	pmotor->output = 0;
}

/**
  * @brief      电机组输出置零
  * @param      pmotor_group: 指针指向电机组对象
  * @retval     无
  */
void Motor_GroupSetoutput0(Motor_Group_t* pmotor_group){
	for (int i = 0; i < 4; ++i) {
		Motor_Setoutput0(pmotor_group->motor[i]);
    }
}

void Motor_SendMotorOutput(Motor_t *  pmotor,uint8_t * txdata){
	if (pmotor == NULL){
		txdata[0] = 0;
		txdata[1] = 0;
	}
	else{
		txdata[0] = (uint8_t)((int16_t)(pmotor->output) >> 8);
		txdata[1] = (uint8_t)((int16_t) pmotor->output) ;
	}
}
/**
  * @brief      发送电机组输出
  * @param      pmotor_group: 指针指向要发送的电机组
  * @retval     无
  */
void Motor_SendMotorGroupOutput(Motor_Group_t *pmotor_group) {
    if (pmotor_group == NULL) return; 
	uint8_t txdata[8];

	Motor_SendMotorOutput(pmotor_group->motor[0], txdata );
	Motor_SendMotorOutput(pmotor_group->motor[1], txdata+2 );
	Motor_SendMotorOutput(pmotor_group->motor[2], txdata+4 );
	Motor_SendMotorOutput(pmotor_group->motor[3], txdata+6 );
	
    Can_SendMessage(pmotor_group->can_handle, &(pmotor_group->can_header), txdata);
}

/************************ COPYRIGHT BIT DreamChaser *****END OF FILE****/
