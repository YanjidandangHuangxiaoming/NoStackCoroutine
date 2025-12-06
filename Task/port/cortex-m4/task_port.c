#include "task_port.h"
#include "device_registers.h" 
#include "pins_driver.h"

#include "Board_LED.h"                  // ::Board Support:LED

//lint -emacro((923,9078),SCB,SysTick) "cast from unsigned long to pointer"
#include "RTE_Components.h"
#include CMSIS_device_header

#ifndef SYSTICK_IRQ_PRIORITY
#define SYSTICK_IRQ_PRIORITY    0xFFU
#endif


/*----------------------------------------------------------------------------
  configure SystemCoreClock:
     System Clock source         PLL (OSC)
     OSC Frequency(Hz)           8000000
     SYSCLK(Hz)                  96000000
     PLLMUL                      24
     PREDIV                      2
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {

  SCG->SOSCCSR = SCG_SOSCCSR_SOSCERR(1U);

  /* set up OSC clock. */
  /* Setup dividers. */
  SCG->SOSCDIV = (SCG_SOSCDIV_SOSCDIV1(1U) |
                  SCG_SOSCDIV_SOSCDIV2(1U)  );

  /* Set OSC configuration. */
  SCG->SOSCCFG = (SCG_SOSCCFG_EREFS(1U) |
                  SCG_SOSCCFG_RANGE(2U)  );

  /* Enable clock. */
  SCG->SOSCCSR = (SCG_SOSCCSR_SOSCEN(1U) );

  /* Wait for OSC clock to be valid. */
  while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD(1U))) __NOP();

 /* Init SIRC */
  SCG->SIRCCSR = 0U;

  /* Setup dividers. */
  SCG->SIRCDIV = (SCG_SIRCDIV_SIRCDIV1(1U) |
                  SCG_SIRCDIV_SIRCDIV2(2U)  );

  /* Set SIRC configuration. */
  SCG->SIRCCFG = SCG_SIRCCFG_RANGE(1U);

  /* Enable clock. */
  SCG->SIRCCSR = SCG_SIRCCSR_SIRCEN(1U);

  /* Wait for SIRC clock to be valid. */
  while (!(SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD(1U))) __NOP();

  /* Change to SIRC as system clock source */
  SCG->RCCR = (SCG_RCCR_DIVSLOW(3U) |
               SCG_RCCR_DIVBUS(0U)  |
               SCG_RCCR_DIVCORE(0U) |
               SCG_RCCR_SCS(2U)     );

  /* Wait for clock source switch finished. */
  while ((SCG->RCCR & 0x0F000000) != 0x02000000) __NOP();;

  /* Init Firc */
  SCG->FIRCCSR = 0U;

  /* Setup dividers. */
  SCG->FIRCDIV = (SCG_FIRCDIV_FIRCDIV1(1U) |
                  SCG_FIRCDIV_FIRCDIV2(1U)  );

  /* Set FIRC configuration. */
  SCG->FIRCCFG = SCG_FIRCCFG_RANGE(0U);

  /* Enable clock. */
  SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN(1U);

  /* Wait for FIRC clock to be valid. */
  while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD(1U))) __NOP();;

  /* Init SysPll */
  SCG->SPLLCSR = SCG_SPLLCSR_SPLLERR(1U);

  /* Setup dividers. */
  SCG->SPLLDIV = (SCG_SPLLDIV_SPLLDIV1(1U) |
                  SCG_SPLLDIV_SPLLDIV2(2U)  );

  /* Set PLL configuration. */
  SCG->SPLLCFG = (SCG_SPLLCFG_PREDIV(0U) |                     /* prediv 1 */
                  SCG_SPLLCFG_MULT(8U)   );                    /* pll multiply 24 */

  /* Enable clock. */
  SCG->SPLLCSR = SCG_SPLLCSR_SPLLEN(1U);

  /* Wait for PLL clock to be valid. */
  while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD(1U))) __NOP();

  /* Change to use PLL as system clock source */
  SCG->RCCR = (SCG_RCCR_DIVSLOW(4U) |
               SCG_RCCR_DIVBUS(1U)  |
               SCG_RCCR_DIVCORE(0U) |
               SCG_RCCR_SCS(6U)     );

  /* Wait for clock source switch finished. */
  while ((SCG->RCCR & 0x0F000000) != 0x06000000) __NOP();
}

int InitSystick(void)
{  
    uint32_t load = 0;
    if (TICK_PER_S == 0U) 
    {
        //lint -e{904} "Return statement before end of function"
        return (-1);
    }
    load = (SystemCoreClock / TICK_PER_S) - 1U;
    if (load > 0x00FFFFFFU)
    {
        //lint -e{904} "Return statement before end of function"
        return (-1);
    }

    // Set SysTick Interrupt Priority    
    SCB->SHP[11]  = SYSTICK_IRQ_PRIORITY; 
    SysTick->CTRL =  SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk| SysTick_CTRL_ENABLE_Msk;    
    SCB->ICSR = SCB_ICSR_PENDSTSET_Msk;
    SysTick->LOAD =  load;
    SysTick->VAL  =  0U;
    return 0;
}
    
void task_systick_init(void)
{
    SystemCoreClockConfigure();                                  /* configure system core clock */
    SystemCoreClockUpdate();                                     /* update System Core Clock info */
    InitSystick();
}

void SysTick_Handler(void)
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
        LED_On(0);
        //HAL_GPIO_WritePin(GPIOB, 1, GPIO_PIN_RESET);
        //GPIO_ClrBit(GPIO_LED, PORT_LED);	// 
        timer_set(&input_timer, 100);
        //timer_set(&input_timer,10);  
        PT_WAIT_UNTIL(pt1, timer_expired(&input_timer));


        
        LED_Off(0);
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

int8_t timert2(void *Param1)
{
    static int i1 = 0;
    ++i1 ;
    i1%=2;
    if(i1==0)
        LED_Off(1);
    else
        LED_On(1);
    return 0;
}


int8_t timert3(void *Param1)
{
    static int i1 = 0;
    ++i1 ;
    i1%=2;
    if(i1==0)
        LED_Off(2);
    else
        LED_On(2);
    return 0;
}
    
void Suspend_Interrupt(void)
{
}
void Resume_Interrupt(void)
{
}