#ifndef _TASK_H_
#define _TASK_H_


#include "task_types.h"
#include "task_cfg.h"
#include "task_port.h"

//周期任务处理函数声明
void TaskProc(void);

//激活任务 
int8_t ActiveTask(Task_id taskid,TASK_MODE mode, int32_t lPeriod) ;
//挂起任务
int8_t SuspendTask(Task_id taskid);
//启动定时器
int8_t StartTimer2(int8_t *TimerID, TIMER_MODE cMode, int32_t lPeriod, timerProc timerProc, void *Param1);
//启动定时器
int8_t StartTimer(TIMER_MODE cMode, int32_t lPeriod, timerProc timerProc, void *Param1);
//停止定时器
int8_t StopTimer(int8_t *pcTimerNumber );
//停止所有定时器
void StopAllTimer(void);

//初始化任务模块
void InitTask(void);
//获取系统时间(毫秒)
int32_t get_system_ms(void);
 
#endif

