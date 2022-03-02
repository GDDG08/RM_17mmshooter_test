/*
 * @Project      : RM_Infantry_Neptune_frame
 * @FilePath     : \RM_17mmshooter_test\User\Task\debug_BTlog.h
 * @Descripttion : 
 * @Author       : GDDG08
 * @Date         : 2021-10-31 09:17:07
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-01-12 15:28:35
 */
#ifndef DEBUG_BTLOG_H
#define DEBUG_BTLOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "uart_util.h"
#include "shooter_task.h"
#include "remote_task.h"

typedef enum {
    BYTE = 0u,
    uInt8 = 1u,
    uInt16 = 2u,
    uInt32 = 3u,
    Float = 4u,
    Char = 5u
} BTlog_TypeEnum;

#define BTlog_tagSize 19

typedef struct {
    void* ptr;
    uint8_t size;
    uint8_t type;
    char tag[BTlog_tagSize];
    // void (*log_func)(uint8_t buff[]);
} BTlog_TableEntry;

extern UART_HandleTypeDef* Const_BTlog_UART_HANDLER;

void BTlog_Init(void);
void BTlog_Send(void);
void BTlog_RXCallback(UART_HandleTypeDef*);
uint8_t BTLog_VerifyData(uint8_t*, uint16_t);

#ifdef __cplusplus
}
#endif

#endif
