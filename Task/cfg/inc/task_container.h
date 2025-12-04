    {
        1, TASK_MODE_PERIOD, TASK_ID(TaskBlinLed), 0,
#if TASK_WITH_NAME 
        TASK_NAME(TaskBlinLed),
#endif // TASK_WITH_NAME
        100UL, 100UL, TaskBlinLed,
#if TASK_WITH_PT
        { 0 }
#endif 
    },
    //{1, TASK_MODE_PERIOD,   TASK_ID(TaskWDT),     0,TASK_NAME(TaskWDT)      ,   100L,   100L,   TaskWDT,        },   // 
    //{1, TASK_MODE_PERIOD,   TASK_ID(TaskScanKey), 0,TASK_NAME(TaskWDT)      ,   50L,    50L,    TaskScanKey,    {0}},   // 
    


