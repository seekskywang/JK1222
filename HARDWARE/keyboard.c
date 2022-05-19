#include "pbdata.h"
#include <math.h>
//==========================================================
//全局变量
Key_TypeDef Keyboard;//键码结构体

static u8 LastKeyVal;//上次键值
static u8 LastKeyTicks;//按键节拍
static u8 ContinueKeyTicks;//持续按键节拍
const u8 Scan_Value[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};
u8 KekValue;
u16 g_enc_fast_flag;
u8 spinflag;
u16 spintest,spinnum;
void Key_Delay(u32 num)
{
    while(num)
    {
        num--;
    }
    

}
void Spin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	 //PB6 ch1  A,PB7 ch2 
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能TIM1时钟	
	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);
	 
	 GPIO_StructInit(&GPIO_InitStructure);//将GPIO_InitStruct中的参数按缺省值输入
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;         
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA6 PA7浮空输入	
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOC, &GPIO_InitStructure);   

	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;         
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//PA6 PA7浮空输入	
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);  
	 
	 GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	 GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);	
	 
	 TIM_DeInit(TIM3);
	 TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	 TIM_TimeBaseStructure.TIM_Period = 359*4;  //设定计数器重装值   TIMx_ARR = 359*4
	 TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3时钟预分频值
	 TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//设置时钟分割 T_dts = T_ck_int	
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数 
	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              
	 
	 TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//使用编码器模式3，上升下降都计数
	 TIM_ICStructInit(&TIM_ICInitStructure);//将结构体中的内容缺省输入
	 TIM_ICInitStructure.TIM_ICFilter = 6;  //选择输入比较滤波器 
	 TIM_ICInit(TIM3, &TIM_ICInitStructure);//将TIM_ICInitStructure中的指定参数初始化TIM3
	 
	 //Reset counter
	 TIM3->CNT = 0;//
	 TIM_Cmd(TIM3, ENABLE);   //启动TIM1定时器
}



uint16_t Encoder_Process(u8 list)
{
	u8 key;
	if(TIM3->CNT < 100 || TIM3->CNT > 800)
	{
		TIM3->CNT = 400;
		spintest = TIM3->CNT/2;
		spinnum = spintest;
	}
	spintest = TIM3->CNT/2;
	if(spinnum < spintest)//增加
	{
		Key_beep();
		spinflag = 1;
		spinnum = spintest;
		switch(GetSystemStatus())
		{
			case SYS_STATUS_TEST:
			{
				if(coder_flag == 1)
				{
					flag_spin = 1;
					if(list == 2)
					{
						switch(LoadSave.mode)
						{
							case 0:
							{
								LoadSave.current += pow(10,spinbit-1);
							}break;
							case 1:
							{
								LoadSave.voltage += pow(10,spinbit-1);
								if(LoadSave.voltage > MAX_SET_VOLT)
								{
									LoadSave.voltage = MAX_SET_VOLT;
								}
							}break;
							case 2:
							{
								LoadSave.risistence += pow(10,spinbit-1);
							}break;
							case 3:
							{
								LoadSave.power += pow(10,spinbit-1);
							}break;
						}
						Para_Set_Comp();
						return list;
					}
				}else{
					key = list;
					if(key<2)
					{
						key++;
					}else{
						key = 0;
					}
				}
			}break;
			case SYS_STATUS_DEBUG:
			{
				if(calpage == 0)
				{
					dacvalue[list-1-8] += dacctrl;
				}else if(calpage == 1){
					dacvalue[list+7] += dacctrl;
				}
				Set_Dac(list);
			}break;
			case SYS_STATUS_BATTERY:
			{
				key = list;
				if(LoadSave.loadmode == 0)
				{
					if(key>7)
					{
						key=0;
					}else if(key == 4){
						key+=2;
					}
					else{
						key++;
					}
				}else if(LoadSave.loadmode == 2){
					if(key==6)
					{
						key=0;
					}else if(key == 2){
						key = 6;
					}else if(key == 2){
						key=6;
					}else if(key == 1){
						key=2;
					}else if(key == 0){
						key=1;
					}
				}
				return key;
			}break;
			case SYS_STATUS_SETUP:
			{
				key = list;
				if(key<12)
				{
					key++;
				}else{
					key = 0;
				}
			}break;
			case SYS_STATUS_LIMITSET:
			{
				key = list;
				if(key<11)
				{
					key++;
				}else{
					key = 0;
				}
			}break;
			case SYS_STATUS_LIST:
			{
				key = list;
				if(key>11)
				{
					key=0;
				}
				else
				{
//					if(key == 2)
//					{
//						key+=2;
//					}else{
						key++;
//					}
				}
				return key;
			}break;
			case SYS_STATUS_SYSSET:
			{
				key = list;
				if(key<11)
				{
					key++;
				}else{
					key = 0;
				}
			}break;
			case SYS_STATUS_DYNAMIC:
			{
				key = list;
				if(key<8)
				{
					key++;
				}else{
					key = 0;
				}
			}break;
			default:
			break;
		}
	}else if(spinnum > spintest){//减少
		Key_beep();
		spinflag = 1;
		spinnum = spintest;
		switch(GetSystemStatus())
		{
			case SYS_STATUS_TEST:
				switch(GetSystemStatus())
				{
					case SYS_STATUS_TEST:
						if(coder_flag == 1)
						{
							flag_spin = 1;
							if(list == 2)
							{
								switch(LoadSave.mode)
								{
									case 0:
									{
										if(LoadSave.current < pow(10,spinbit-1))
										{
											LoadSave.current = 0;
										}else{
											LoadSave.current -= pow(10,spinbit-1);
											if(LoadSave.current < pow(10,spinbit-1))
											{
												if(spinbit > 1)
												spinbit --;
											}
										}
									}break;
									case 1:
									{
										if(LoadSave.voltage < pow(10,spinbit-1))
										{
											LoadSave.voltage = 0;
										}else{
											LoadSave.voltage -= pow(10,spinbit-1);
											if(LoadSave.voltage < pow(10,spinbit-1))
											{
												if(spinbit > 1)
												spinbit --;
											}
										}

									}break;
									case 2:
									{
										if(LoadSave.risistence < pow(10,spinbit-1))
										{
											LoadSave.risistence = 0;
										}else{
											LoadSave.risistence -= pow(10,spinbit-1);
											if(LoadSave.risistence < pow(10,spinbit-1))
											{
												if(spinbit > 1)
												spinbit --;
											}
										}

									}break;
									case 3:
									{
										if(LoadSave.power < pow(10,spinbit-1))
										{
											LoadSave.power = 0;
										}else{
											LoadSave.power -= pow(10,spinbit-1);
											if(LoadSave.power < pow(10,spinbit-1))
											{
												if(spinbit > 1)
												spinbit --;
											}
										}
										
									}break;
								}
								return list;
							}
						}else{
							key = list;
							if(key>0)
							{
								key--;
							}else{
								key = 2;
							}
						}
					break;
				}
			break;
			case SYS_STATUS_DEBUG:
			{
				if(calpage == 0)
				{
					dacvalue[list-1-8] -= dacctrl;
				}else if(calpage == 1){
					dacvalue[list+7] -= dacctrl;
				}
//				dacvalue[list-1] -= 100;
				Set_Dac(list);
			}break;
			case SYS_STATUS_BATTERY:
			{
				key = list;
				if(LoadSave.loadmode == 0)
				{
					if(key<1)
					{
						key=8;
					}else if(key == 6){
						key-=2;
					}
					else{
						key--;
					}
				}else if(LoadSave.loadmode == 2){
					if(key==6)
					{
						key=2;
					}else if(key == 5){
						key = 2;
					}else if(key == 2){
						key--;
					}else if(key == 1){
						key--;
					}else if(key == 0){
						key=6;
					}
				}
				return key;
			}break;
			case SYS_STATUS_SETUP:
			{
				key = list;
				if(key>0)
				{
					key--; 
				}else{
					key = 12;
				}
			}break;
			case SYS_STATUS_LIMITSET:
			{
				key = list;
				if(key>0)
				{
					key--; 
				}else{
					key = 11;
				}
			}break;
			case SYS_STATUS_LIST:
			{
				key = list;
				if(key<1)
				{
					key=12;
				}
				else{
//					if(key ==4)
//					{
//						key-=2;
//					}else{
						key--;
//					}
				}
				return key;
			}break;
			case SYS_STATUS_SYSSET:
			{
				key = list;
				if(key>0)
				{
					key--;
				}else{
					key = 11;
				}
			}break;
			case SYS_STATUS_DYNAMIC:
			{
				key = list;
				if(key>0)
				{
					key--;
				}else{
					key = 8;
				}
			}break;
			default:
			break;
		}
	}else{
		spinflag = 0;
	}
	
}
 

void Keyboard_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|
	RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOH|RCC_AHB1Periph_GPIOI,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=DIG1_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DIG1_GPIO_PORT,&GPIO_InitStructure);
    GPIO_SetBits(DIG1_GPIO_PORT,DIG1_PIN);
	
	GPIO_InitStructure.GPIO_Pin=DIG2_PIN;
	GPIO_Init(DIG2_GPIO_PORT,&GPIO_InitStructure);
    GPIO_SetBits(DIG2_GPIO_PORT,DIG2_PIN);
	
	GPIO_InitStructure.GPIO_Pin=DIG3_PIN;
	GPIO_Init(DIG3_GPIO_PORT,&GPIO_InitStructure);
    GPIO_SetBits(DIG3_GPIO_PORT,DIG3_PIN);
	
	GPIO_InitStructure.GPIO_Pin=DIG4_PIN;
	GPIO_Init(DIG4_GPIO_PORT,&GPIO_InitStructure);
    GPIO_SetBits(DIG4_GPIO_PORT,DIG4_PIN);
	
	GPIO_InitStructure.GPIO_Pin=KI1_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI1_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI2_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI2_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI3_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI3_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI4_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI4_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI5_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI5_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI6_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI6_GPIO_PORT,&GPIO_InitStructure);

	
	GPIO_InitStructure.GPIO_Pin=KI7_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_Init(KI7_GPIO_PORT,&GPIO_InitStructure);

	
}
u8 Key_Identiry(void)
{
	KekValue = KEY_NONE;
	
	DIG1L;
	DIG2H;
	DIG3H;
	DIG4H;	
	delay(10);
	if(READKI1 == 0)
	{
		KekValue = 1;
	}else if(READKI2 == 0){
		KekValue = 2;
	}else if(READKI3 == 0){
		KekValue = 3;
	}else if(READKI4 == 0){
		KekValue = 4;
	}else if(READKI5 == 0){
		KekValue = 5;
	}else if(READKI6 == 0){
		KekValue = 6;
	}else if(READKI7 == 0){
		KekValue = 7;
	}
	
	DIG1H;
	DIG2L;
	DIG3H;
	DIG4H;
	delay(10);
	if(READKI1 == 0)
	{
		KekValue = 8;
	}else if(READKI2 == 0){
		KekValue = 9;
	}else if(READKI3 == 0){
		KekValue = 10;
	}else if(READKI4 == 0){
		KekValue = 11;
	}else if(READKI5 == 0){
		KekValue = 12;
	}else if(READKI6 == 0){
		KekValue = 13;
	}else if(READKI7 == 0){
		KekValue = 14;
	}
	
	DIG1H;
	DIG2H;
	DIG3L;
	DIG4H;	
	delay(10);
	if(READKI1 == 0)
	{
		KekValue = 15;
	}else if(READKI2 == 0){
		KekValue = 16;
	}else if(READKI3 == 0){
		KekValue = 17;
	}else if(READKI4 == 0){
		KekValue = 18;
	}else if(READKI5 == 0){
		KekValue = 19;
	}else if(READKI6 == 0){
		KekValue = 20;
	}else if(READKI7 == 0){
		KekValue = 21;
	}
	
	DIG1H;
	DIG2H;
	DIG3H;
	DIG4L;	
	delay(10);
	if(READKI1 == 0)
	{
		KekValue = 22;
	}else if(READKI2 == 0){
		KekValue = 23;
	}else if(READKI3 == 0){
		KekValue = 24;
	}else if(READKI4 == 0){
		KekValue = 25;
	}else if(READKI5 == 0){
		KekValue = 26;
	}else if(READKI6 == 0){
		KekValue = 27;
	}else if(READKI7 == 0){
		KekValue = 28;
	}
	return (KekValue);
}


//void Keyboard_Init(void)
//{
//	
//    
//	GPIO_InitTypeDef GPIO_InitStructure;
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOD,ENABLE);
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//    GPIO_SetBits(GPIOC,GPIO_Pin_9);
//    
//    
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
//	//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//	GPIO_Init(GPIOI,&GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
//	///GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//	GPIO_Init(GPIOD,&GPIO_InitStructure);
//	
//	Keyboard.value=KEY_NONE;//键值为空
//	Keyboard.state=FALSE;	//按键无效
//	Keyboard.continuance=FALSE;//持续按键无效
//	
//	LastKeyVal=KEY_NONE;//上次键值
//	LastKeyTicks=0;//按键节拍
//	ContinueKeyTicks=0;//持续按键节拍
//}

void _164Delaay(void)
{
	u16 i;
	for(i=0;i<10;i--)
	;

};
void HW_Sendvalueto164(u8 value)
{
	uint8_t i;
	_Hc164CpL;
	for(i=0;i<8;i++)
	{
		if((value&0x80)==0x80)
			_Hc164DH;
		else
			_Hc164DL;
		_Hc164CpH;
		_164Delaay();
		_Hc164CpL;
		value<<=1;
        //_164Delaay();
	}

}

//==========================================================
//函数名称：Key_Identiry
//函数功能：按键识别
//入口参数：无
//出口参数：U8 键值
//创建日期：2014.09.25 
//修改日期：2014.09.29 14:45
//备注说明：按键输入信号，0有效
//==========================================================
//u8 Key_Identiry(void)
//{
//	u16 key1;
//	u8 key;
//	
//	key1=GPIO_ReadInputData(GPIOG);//GPIOG->ODR;//
//	key1|=0x01ff;
//	if(key1!=0xFFFF)//按键输入信号有效
//	{
//		switch(key1)
//		{
//			case HW_KEY_SET:
//				key=KEY_SET;
//				break;
//			case HW_KEY_UP:
//				key=KEY_UP;
//				break;
//			case HW_KEY_DOWN:
//				key=KEY_DOWN;
//				break;
////			case HW_KEY_LEFT:
////				key=KEY_LEFT;
////				break;
////			case HW_KEY_RIGHT:
////				key=KEY_RIGHT;
////				break;
//			case HW_KEY_ENTER:
//				key=KEY_ENTER;
//				break;
//			case HW_KEY_RESET:
//				key=KEY_RESET;
//				break;
//			case HW_KEY_START:
//				key=KEY_START;
//				break;
//			default:
//				key=KEY_NONE;
//				break;
//		}
//	}
//	else
//	{
//		key=KEY_NONE;
//	}
////	if(key==KEY_NONE)
////	{
////		key2=GPIO_ReadInputData(GPIOC);
////		key2|=0XFFCF;
////		if(key2!=0xFFFF)//按键输入信号有效
////		{
////			switch(key2)
////			{
//////				case HW_KEY_SET:
//////					key=KEY_SET;
//////					break;
//////				case HW_KEY_UP:
//////					key=KEY_UP;
//////					break;
////				case HW_KEY_DOWN:
////					key=KEY_DOWN;
////					break;
//////				case HW_KEY_LEFT:
//////					key=KEY_LEFT;
//////					break;
//////				case HW_KEY_RIGHT:
//////					key=KEY_RIGHT;
//////					break;
//////				case HW_KEY_ENTER:
//////					key=KEY_ENTER;
//////					break;
////				case HW_KEY_RESET:
////					key=KEY_RESET;
////					break;
//////				case HW_KEY_START:
//////					key=KEY_START;
//////					break;
////				default:
////					key=KEY_NONE;
////					break;
////			}
////		}
////		else
////		{
////			key=KEY_NONE;
////		}
////	}
//	return (key);
//}
//==========================================================
//函数名称：Key_Init
//函数功能：按键结构体初始化
//入口参数：无
//出口参数：无
//创建日期：2014.09.29
//修改日期：2014.09.29 10:49
//备注说明：无
//==========================================================
//void Key_Init(void)
//{
//    Keyboard_Init();//按键端口初始化
//	Keyboard.value=KEY_NONE;//键值为空
//	Keyboard.state=FALSE;	//按键无效
//	Keyboard.continuance=FALSE;//持续按键无效
//	
//	LastKeyVal=KEY_NONE;//上次键值
//	LastKeyTicks=0;//按键节拍
//	ContinueKeyTicks=0;//持续按键节拍
//}

//==========================================================
//函数名称：Key_Read
//函数功能：读取按键
//入口参数：无
//出口参数：无
//创建日期：2014.09.29
//修改日期：2014.09.29 10:50
//备注说明：无
//==========================================================
u8 Key_Read(void)
{
	if (Keyboard.state)	//有键按下
	{
		Keyboard.state=FALSE;
		//Beep_One();	//蜂鸣器响一声
        Key_beep();
        
		return (Keyboard.value);
	}
	//可直接处理电源等通用按键
	return (KEY_NONE);	
}

//==========================================================
//函数名称：Key_Read_WithTimeOut
//函数功能：阻塞式读取按键
//入口参数：ticks:等待节拍数(50ms周期)
//出口参数：按键值
//创建日期：2014.09.29
//修改日期：2014.09.29 10:56
//备注说明：直到按键响应或限时到
//==========================================================
u8 Key_Read_WithTimeOut(u8 ticks)
{
   // u8 i=1;
//	if (ticks)//定时等待按键
//		{
			SetSoftTimer(KEYBOARD_SOFTTIMER,ticks);//设置按键延时周期(即软定时器设置,50ms周期)
				
			while ((!GetSoftTimerOut(KEYBOARD_SOFTTIMER))&&(!Keyboard.state)) //计时未到并且无按键按下
				{
//					RunOnKeyFree();//在按键等待过程中处理其他业务
                    //i=GetSoftTimerOut(KEYBOARD_SOFTTIMER);
				}
//		}
//	else//无限等待按键
//		{
//			while (!Keyboard.state) 
//				{
//					RunOnKeyFree();//在按键等待过程中处理其他业务
//				}			
//		}
	return Key_Read();//按键更新
}
////==========================================================
////函数名称：Key_Scan
////函数功能：按键扫描
////入口参数：无
////出口参数：无
////创建日期：2014.09.28 
////修改日期：2014.09.28 11:01
////备注说明：由10mS中断调用
////==========================================================
// u8 Key_Identiry(void)
//{
//    u8 row,i;
////	u8 scanvalue=0;
//	u8 key_value=0xff;
//	u8 value=0xff;
//	HW_Sendvalueto164(0);
//	if(GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_11)==0)
//		row=0;
//	else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)==0)
//		row=1;
//	else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)==0)
//		row=2;
//	else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)==0)
//		row=3;
//	else 
//        row=4;
//    if(row<4)
//	{
//		
//		for(i=0;i<8;i++)
//		{
//			HW_Sendvalueto164(Scan_Value[i]);
//            _164Delaay();
//			switch(row)
//			{
//				case 0:
//						
//						if(GPIO_ReadInputDataBit(GPIOI, GPIO_Pin_11)==0)
//						{
//							
//							value=i;
//						}
//						
//					break;
//				case 1:
//					if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)==0)
//						value=i;
//					break;
//				case 2:
//					if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)==0)
//						value=i;
//					break;
//				case 3:
//					if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2)==0)
//						value=i;
//					break;
//				default:
//					break;
//			}
//            if(value!=0xff)
//			break;
//		}
//        if(value==0xff)
//            return  KEY_NONE;
//		key_value=row*8+value;
//        return key_value;
//	
//	
//	}
//    else
//        if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13)==0)
//        {
////            return 116;
//        
//        
//        }
//    return  KEY_NONE;
//    
//	
//    
//}

void Key_Scan(void)
{
	u8 keyval;
	
	keyval = Key_Identiry();//按键识别，返回键值

	//按键长短识别
	if (keyval==KEY_NONE)//无键
	{
		if (LastKeyVal)	//上次有键，短按键判别
		{
			if ( (LastKeyTicks>SHORTKEYCOUNT) && (LastKeyTicks<LONGKEYCOUNT) )
			{
				Keyboard.value=LastKeyVal;	//键值
				Keyboard.state=TRUE;		//有按键
			}
		}
		LastKeyVal=KEY_NONE;
		LastKeyTicks=0;
		Keyboard.continuance=FALSE; //持续按键	
	}
	else	//有键
	{
		if (LastKeyVal==keyval)//上次按键和本次按键相同
		{
			if (LastKeyTicks<LONGKEYCOUNT+1)//长按键节拍数100x10mS
				LastKeyTicks++;
			
			if (LastKeyTicks==LONGKEYCOUNT)//等于长按键节拍数
			{
				ContinueKeyTicks=0;//持续按键节拍
				//keyval|=LONG_PRESS;	//长按键
				Keyboard.value=keyval;	//键值
				Keyboard.state=TRUE;	//有按键
//				Keyboard.continuance=FALSE; //持续按键
				Keyboard.continuance=TRUE; //持续按键
			}
			else if (LastKeyTicks>LONGKEYCOUNT)//大于长按键节拍数
			{
				if(HW_KEYBOARD_CONTINUE_SUPPORT)//持续按键有效判别
				{
					//keyval|=LONG_PRESS;	//长按键
					Keyboard.value=keyval;//键值
//					Keyboard.state=TRUE;//有按键
					Keyboard.continuance=TRUE; //持续按键
					ContinueKeyTicks++;
					if(ContinueKeyTicks>CONTINUEKEYCOUNT)//持续按键节拍数
					{
						ContinueKeyTicks=0;//持续按键节拍
						if(Keyboard.state==FALSE)//按键已读取
							Keyboard.state=TRUE;//有按键
					}
				}
			}					
		}
		else//上次按键和本次按键不同
		{
			LastKeyVal=keyval;
			LastKeyTicks=0;
		}
	}
}

