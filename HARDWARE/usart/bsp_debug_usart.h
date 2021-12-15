#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>


#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE                    115200

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9
#define DEBUG_USART_IRQ                 		USART1_IRQn

#define __DEBUG 	//¿ªÆô´®¿Úµ÷ÊÔ

#ifdef  __DEBUG
#define DEBUG(format,...)	printf("File:"__FILE__",Line:%03d:"format"\n",__LINE__,##__VA_ARGS__) 
#else

#define DEBUG(format,...)

#endif


void Debug_USART_Config(u32 baud);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Send_Uart3str(char *str);
void Usart1_Send(char *str,u16 num);
void Set_Addr(void);
void Set_Para(void);
void I_Gear_SW(void);
void V_Gear_SW(void);
void OnOff_SW(u8 value);
void Mode_SW(void);
void List_Mode_SW(void);
void BatMode_SW(void);
void Sence_SW(void);
void ProtectRst(void);
void Set_Baudrate(void);
void CalHandle(u8 step);
void ReadData(void);
void Rec_Handle(void);
void Set_Dac(u8 step);
void USART1_IRQHandler(void);
#endif /* __USART1_H */
