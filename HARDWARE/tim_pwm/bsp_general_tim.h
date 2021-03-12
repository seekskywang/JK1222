#ifndef __GENERAL_TIM_H
#define	__GENERAL_TIM_H

#include "stm32f4xx.h"

//#define GENERAL_TIM           		TIM2
//#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

//#define GENERAL_TIM_IRQn					TIM2_IRQn
//#define GENERAL_TIM_IRQHandler    TIM2_IRQHandler


#define GENERAL_OCPWM_PIN             GPIO_Pin_7              
#define GENERAL_OCPWM_GPIO_PORT       GPIOA                      
#define GENERAL_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define GENERAL_OCPWM_PINSOURCE				GPIO_PinSource7
#define GENERAL_OCPWM_AF							GPIO_AF_TIM14


//#define GENERAL_TIM           		    TIM14
//#define GENERAL_TIM_CLK       		    RCC_APB1Periph_TIM14

//#define GENERAL_TIM_IRQn					    TIM14_IRQn
//#define GENERAL_TIM_IRQHandler        TIM14_IRQHandler


extern uint32_t testCNT;	

void TIM2_Configuration(u32 Period,u32 Prescaler);
void TIM3_Configuration(u32 TIM_Period,u32 TIM_Prescaler );
void TIM14_Configuration(u32 cycle,u32 pre);
void MODS_SendWithCRC(uint8_t *_pBuf, uint8_t _ucLen);



//void TIM2_Configuration(void);

#endif /* __GENERAL_TIM_H */

