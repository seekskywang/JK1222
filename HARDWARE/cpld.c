#include "cpld.h"
#include "stm32f4xx.h"
#include "./systick/bsp_SysTick.h"


void CPLD_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_7;     //
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
    
    //GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//10 FAIL  11  LOCK  12 TRIP  14 ON
   // GPIO_Init(GPIOH,&GPIO_InitStructure); 
   // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//
   // GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//OFF
    GPIO_Init(GPIOA,&GPIO_InitStructure); 


}
void Send_one_buff(u8 dat)
{
    u8 i;
    CPLD_SCLK_L;
    delay_us(10);
    for(i=0;i<8;i++)
    {
        if(dat&0x80)
            CPLD_DIN_H;
        else
            CPLD_DIN_L;
        dat<<=1;
        delay_us(10);
        CPLD_SCLK_H;
        delay_us(10);
        
        CPLD_SCLK_L;
        
                
    }
//    delay_us(10);
//    CPLD_SCLK_L;
//    CPLD_DIN_L;


}
void Send_To_Cpld(u8 *buff,u8 num)
{
    u8 i; 
    
    if(num<1)
        num=1;
    for(i=0;i<num;i++)
    {
        Send_one_buff(*(buff+i));
       
    }
    //delay_us(2);
    CPLD_SCLK_L;
    delay_us(10);
    CPLD_DIN_H;
    delay_us(10);
    CPLD_SCLK_H;//特殊协议改造
//    delay_us(1);
//    CPLD_SCLK_L;


}
