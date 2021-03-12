/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器PWM输出范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./tim_pwm/bsp_general_tim.h"

/**
  * @brief  通用定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
	
 uint32_t testCNT;	
//static void TIM2_NVIC_Configuration(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // 设置中断组为0
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
//		// 设置中断来源
//    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 	
//		// 设置抢占优先级
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
//	  // 设置子优先级
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
//static void TIM2_Mode_Config(u32 TIM_Period,u32 TIM_Prescaler)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

//	// 开启TIMx_CLK,x[6,7] 
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

//  /* 累计 TIM_Period个后产生一个更新或者中断*/		
//  //当定时器从0计数到4999，即为5000次，为一个定时周期
//  TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;     //100  
//	
//	// 高级控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
//	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
//  TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler-1;	//9000
//  // 采样时钟分频
//  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//  // 计数方式
//  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	
//	// 初始化定时器TIMx, x[1,8]
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	
//	
//	// 清除定时器更新中断标志位
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//	
//	// 开启定时器更新中断
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	
//	// 使能定时器
//	TIM_Cmd(TIM2, ENABLE);	
//}

/**
  * @brief  初始化高级控制定时器定时，10ms产生一次中断
  * @param  无
  * @retval 无
  */
//void TIM2_Configuration(u32 Period,u32 Prescaler)
//{
//	TIM2_NVIC_Configuration();	
//  
//  TIM2_Mode_Config(Period,Prescaler);
//}

/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIM3_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 
  /* 定时器通道引脚复用 */
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 
  
	/* 定时器通道引脚配置 */															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/* PA不能再定时器2PWM输出   最后再查询定时器3的PWM输出通道
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM3_PWMOUTPUT_Config(u32 TIM_Period,u32 TIM_Prescaler )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到8999，即为9000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = TIM_Period-1;       
	
	// 高级控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = TIM_Prescaler-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 3000-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1
  
	/*使能通道1重载*/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	// 使能定时器
	TIM_Cmd(TIM3, ENABLE);	
}

/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIM3_Configuration(u32 TIM_Period,u32 TIM_Prescaler )
{
	TIM3_GPIO_Config();
  
  TIM3_PWMOUTPUT_Config(TIM_Period,TIM_Prescaler);
}


/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIM14_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd (GENERAL_OCPWM_GPIO_CLK, ENABLE); 
  /* 定时器通道引脚复用 */
	GPIO_PinAFConfig(GENERAL_OCPWM_GPIO_PORT,GENERAL_OCPWM_PINSOURCE,GENERAL_OCPWM_AF); 
  
	/* 定时器通道引脚配置 */															   
	GPIO_InitStructure.GPIO_Pin = GENERAL_OCPWM_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(GENERAL_OCPWM_GPIO_PORT, &GPIO_InitStructure);
}


/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_PWMOUTPUT_Config(u32 cycle,u32 pre)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[2,3,4,5,12,13,14] 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到8999，即为9000次，为一个定时周期  100K是0。01ms 200Hz是 5ms
    
  TIM_TimeBaseStructure.TIM_Period = cycle-1;  //     
	
	// 高级控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
  TIM_TimeBaseStructure.TIM_Prescaler = 900-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 初始化定时器TIMx, x[2,3,4,5,12,13,14] 
	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
	
	/*PWM模式配置*/
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = pre-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平
  TIM_OC1Init(TIM14, &TIM_OCInitStructure);	 //使能通道1
  
	/*使能通道1重载*/
	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);
	
	// 使能定时器
	TIM_Cmd(TIM14, ENABLE);	
}

/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIM14_Configuration(u32 cycle,u32 pre)
{
	TIM14_GPIO_Config();
    
  
  TIM_PWMOUTPUT_Config( cycle, pre);
}

/*********************************************END OF FILE**********************/
