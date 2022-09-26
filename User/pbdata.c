#include "pbdata.h"
//#include <stdbool.h>
u8 F_Fail;//测试失败标志
u8 F_100ms;//100ms定时标志
u8 F_set;//设置命令定时标志
u8 F_1s;//1s定时标志
vu8 SystemStatus;//系统状态
vu8 SystemMessage;//系统信息

uint8_t softswitch;
//NumBox_TypeDef NumBox;//数值框

SaveData_Typedef LoadSave;
u8 USB_Openflag,Disp_usbflag,Disp_RTCflag;
DispValues_TypeDef DispValue;
SlaveData_TypeDef SlaveValue;
SlavePara_TypeDef SlaveSetPara;
SaveData_Typedef LoadSave;

Com_TypeDef ComBuf;
u8 SoftTimer[MAXTSOFTTIMER];//软定时器

u32  g_LcdDirection;
u8 Int_Pe3flag;



void delay(u32 nCount)
{
	for(;nCount!=0;nCount--);
}

/****************************************************************************
* 名    称：delay_us(u32 nus)
* 功    能：微秒延时函数
* 入口参数：u32  nus
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
//void delay_us(u32 nus)
//{
//	 u32 temp;
//	 SysTick->LOAD = 9*nus;
//	 SysTick->VAL=0X00;//清空计数器
//	 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
//	 do
//	 {
//	  temp=SysTick->CTRL;//读取当前倒计数值
//	 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
//	 
//	 SysTick->CTRL=0x00; //关闭计数器
//	 SysTick->VAL =0X00; //清空计数器
//}

///****************************************************************************
//* 名    称：delay_ms(u16 nms)
//* 功    能：毫秒延时函数
//* 入口参数：u16 nms
//* 出口参数：无
//* 说    明：
//* 调用方法：无 
//****************************************************************************/ 
//void delay_ms(u16 nms)
//{
//     //注意 delay_ms函数输入范围是1-1864
//	 //所以最大延时为1.8秒

//	 u32 temp;
//	 SysTick->LOAD = 9000*nms;
//	 SysTick->VAL=0X00;//清空计数器
//	 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
//	 do
//	 {
//	  temp=SysTick->CTRL;//读取当前倒计数值
//	 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
//	 SysTick->CTRL=0x00; //关闭计数器
//	 SysTick->VAL =0X00; //清空计数器
//}

//==========================================================
//函数名称：SetSoftTimer
//函数功能：软定时器设置
//入口参数：软定时器id,定时节拍
//出口参数：无
//创建日期：2014.09.29
//修改日期：2014.10.21 15:08
//备注说明：无
//==========================================================
void SetSoftTimer(u8 id, u8 ticks)
{
	SoftTimer[id]=ticks;
}
//==========================================================
//函数名称：GetSoftTimerOut
//函数功能：软定时器溢出
//入口参数：软定时器id
//出口参数：True(定时到)/False
//创建日期：2014.09.29
//修改日期：2014.09.29 11:18
//备注说明：无
//==========================================================
bool GetSoftTimerOut(u8 id)
{
	return (SoftTimer[id]?FALSE:TRUE);
}
//==========================================================
//函数名称：Beep_On
//函数功能：开蜂鸣器
//入口参数：无
//出口参数：无
//创建日期：2015.01.12
//修改日期：2015.09.04 14:34
//备注说明：无
//==========================================================

