#ifndef _TASK_PORT_H_
#define _TASK_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "task_types.h"


void Idle(void);
void task_systick_init(void);

//初始化systick定时器
int InitSystick(void); 
//空闲任务函数声明
void Idle(void); 
void Suspend_Interrupt(void);
void Resume_Interrupt(void);

//systick中断服务函数声明
void task_systick_handler(void);
int8_t timert2(void *Param1);
int8_t timert3(void *Param1);
 
#if(TASK_WITH_PT)
PT_THREAD(TaskBlinLed(struct pt* pt1));
#else
int8_t TaskBlinLed(void);
#endif
#ifdef __cplusplus
}
#endif

#endif
