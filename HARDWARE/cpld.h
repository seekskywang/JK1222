#ifndef _CPLD_H
#define _CPLD_H

#include "stm32f4xx.h"
#define CPLD_SCLK_H GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define CPLD_SCLK_L GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define CPLD_DIN_H  GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define CPLD_DIN_L  GPIO_ResetBits(GPIOA,GPIO_Pin_5)

void Send_To_Cpld(u8 *buff,u8 num);

void CPLD_GPIO_Configuration(void);
#endif
