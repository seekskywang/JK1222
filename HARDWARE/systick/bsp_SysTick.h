#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void SysTick_Init(void);
void TimingDelay_Decrement(void);
void delay_us(__IO u32 nTime);
#define delay_ms(x) delay_us(100*x)	 //µ¥Î»ms
void delay_1us(u32 nus);

#endif /* __SYSTICK_H */
