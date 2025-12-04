#ifndef _TASK_PORT_H_
#define _TASK_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "task_types.h"
     
// Initialize systick timer
void task_systick_init(void);

// Idle task function declaration
void Idle(void);

// Systick interrupt service function declaration
void task_systick_handler(void); 

 
#if(TASK_WITH_PT)
PT_THREAD(TaskBlinLed(struct pt* pt1));
#else
int8_t TaskBlinLed(void);
#endif

#ifdef __cplusplus
}
#endif

#endif