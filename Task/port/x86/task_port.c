#include "task.h"
#include "task_port.h"
#include "windows.h"


#pragma comment(lib,"winmm.lib")
 
// 定时器回调函数，相当于“中断服务程序”
void CALLBACK task_systick_irq(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{  
    task_systick_handler();  

}
 

void task_systick_init(void)
{
    // Start windows timer, 1ms period, callback function is 
    TIMECAPS tc;
    MMRESULT result;

    // 查询系统支持的定时器分辨率范围
    result = timeGetDevCaps(&tc, sizeof(TIMECAPS));
    if (result != MMSYSERR_NOERROR) {
        printf("timeGetDevCaps failed!\n");
        return ;
    }

    printf("System timer resolution: min=%u ms, max=%u ms\n", tc.wPeriodMin, tc.wPeriodMax);

    // 设置全局定时器分辨率为 1ms
    timeBeginPeriod(1);

    // 创建一个周期性定时器，每隔 1ms 调用一次 TimerProc
    UINT timerId = timeSetEvent(
        1,              // 周期：1ms
        1,              // 精度：1ms
        task_systick_irq,      // 回调函数
        0,              // 用户数据
        TIME_PERIODIC   // 周期性触发
    );

    if (timerId == 0) {
        printf("Failed to create timer!\n");
        timeEndPeriod(1);
        return  ;
    }

    printf("Timer started with ID=%u\n", timerId);
}

void Idle(void)
{

    /*
    __WFI();
    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    */
}
 
 

#if(TASK_WITH_PT)
PT_THREAD(TaskBlinLed(struct pt* pt1))
{
    static struct timer  input_timer;
    PT_BEGIN(pt1);
    while (1)
    {
        printf("LedOn! \n");
        //HAL_GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);
        //GPIO_ClrBit(GPIO_LED, PORT_LED);	// 
        timer_set(&input_timer, 100);
        //timer_set(&input_timer,10);  
        PT_WAIT_UNTIL(pt1, timer_expired(&input_timer));



        printf("LedOff! \n");
        //HAL_GPIO_WritePin(GPIOB, 1, GPIO_PIN_SET);
        //GPIO_SetBit(GPIO_LED, PORT_LED);	// 
        timer_set(&input_timer, 900);
        //timer_set(&input_timer,10);  
        PT_WAIT_UNTIL(pt1, timer_expired(&input_timer));
    }
    PT_END(pt1);
}  
#else
int8_t TaskBlinLed(void)
{
    static uint32_t i = 0;
    
    i = (++i)%10;
    if (i == 0)
    {
        printf("LedOn! \n");
    }
    else
    {
        printf("LedOff! \n");
    }
    return 0;
}
#endif
    
void Suspend_Interrupt(void)
{
}
void Resume_Interrupt(void)
{
}