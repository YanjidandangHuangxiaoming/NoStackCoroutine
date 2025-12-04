#include "task_port.h"
void task_systick_init(void)
{
}
void task_systick_irq(void)
{
    task_systick_handler();
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
        //HAL_GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);
        //GPIO_ClrBit(GPIO_LED, PORT_LED);	// 
        timer_set(&input_timer, 100);
        //timer_set(&input_timer,10);  
        PT_WAIT_UNTIL(pt1, timer_expired(&input_timer));


 
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
       // printf("LedOn! \n");
    }
    else
    {
        //printf("LedOff! \n");
    }
    return 0;
}
#endif