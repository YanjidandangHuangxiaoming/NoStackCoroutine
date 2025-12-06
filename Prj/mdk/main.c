
#include "main.h"
int8_t TimerId = -1;
void Init(void)
{
    InitTask();
    LED_Initialize();
    StartTimer(TIMER_MODE_CYCROUTINE,1000,timert2,NULL);
    StartTimer2(&TimerId,TIMER_MODE_ONCEROUTINE,1000,timert3,NULL);
    SuspendTask(TASK_ID(TaskBlinLed));
    ActiveTask(TASK_ID(TaskBlinLed),TASK_MODE_PERIOD,20);
}

int main(void)
{
    Init();
    TaskProc();
    
    
    return 0;
}
