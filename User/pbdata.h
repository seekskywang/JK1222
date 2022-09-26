#ifndef _pbdata_H
#define _pbdata_H


#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./flash/bsp_spi_flash.h"
#include "./tim/bsp_basic_tim.h"
#include "./usart/bsp_debug_usart.h"
#include "./tim_pwm/bsp_general_tim.h"
#include "./systick/bsp_SysTick.h"
#include "./adc/bsp_adc.h"

//#include <stdbool.h>
#include <string.h>

#include "misc.h"

#include "stdio.h"
//#include "Lcd2002.h"
#include "gpio.h"
//#include "cpld.h"
#include "4094.h"
//#include "24c256.h"
//#include "dac.h"
//#include "adc.h"
//#include "ds1302.h"
#include "ad7706.h"
//#include "tlc5618.h"
//#include "time.h"



#include "disp.h"
#include "keyboard.h"
#include "user.h"
#include  "test.h"
#include  <string.h>
#include "flash_eerom.h"
#include "uart.h"  	
#include "flash.h" 
#include "spi.h"
#include "bsp_exti.h"
#include "ff.h"





//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
//void delay_us(u32 nus);
//void delay_ms(u16 nms);
void SetSoftTimer(u8 id, u8 ticks);
u8 GetSoftTimerOut(u8 id);
void Para_Set_Comp(void);



#define PASSWORD  "20185260"

#define BMP_SWITCH  0      //截图功能开关

//==========================================================

#define MAXTSOFTTIMER (2)		//软定时器数目


//#define BASTARDLY  //定义了以后 要跳动正负三个字才变化的
//==========================================================
//软件版本：10
#define SOFTWARE_VERSION		(0x10)

//==========================================================
//默认出厂编号(0)
#define DEFAULT_MACHINE_ID 		(0)

//默认出厂密码(0)
#define DEFAULT_PASSWORD 		(0)

//默认开机次数(0)
#define SET_BOOT_DEFAULT		(0)

#define CHANNEL_NUM             (15)

//AD值滤波配置，增加数据稳定度
#define AD_FILTER_SUPPORT		(_DEBUG_CONFIG_TRUE)

//==========================================================
#define SETUP_MENU_MAX  	(2)//最大设置菜单

//#define PARAMETER_MENU_MAX	(5)//最大参数菜单

//#define SYSTEM_MENU_MAX		(2)//最大系统菜单

#define CONFIG_MENU_MAX		(3)//最大配置项(0-2)

#define PW_LENGTH 			(4)//最大密码位数

#define RANGE_MAX 			(6)//最大量程

#define RANGE_V_MAX         (1)

#define AD_BUF_LENGTH		(8)//A/D值缓冲区长度

#define KEY_NUM				(3)	//按键灵敏度

#define SYS_MAX				(12)

#define PASSWORD_LENTH  8

#define NUM_LENTH 			(6)//显示长度
#define NUM_LENTH_V 			(8)//显示长度
#define MAX_DOTNUM          (5)//最长的小数点

//==========================================================


#define DEBUG_RANGE 16


//软定时器定义
#define KEYBOARD_SOFTTIMER	(0) //按键延时软定时器
#define DISP_SOFTTIMER 		(1) //显示延时软定时器

//==========================================================
//标题长度
#define LIST_TITLE_LEN		(8)
#define TITLE_LEN_MAX		(8)
#define SetSystemStatus(status) (SystemStatus=status)
#define GetSystemStatus() (SystemStatus)
#define GetSystemMessage() (SystemMessage)
#define SetSystemMessage(msg) (SystemMessage=msg)


#define		MAX_SET_CURRENT       600000    //60.0000A  最大设置电流 (低档 2.00000A  高档 20.0000)
#define		MAX_SET_VOLT          1200000    //120.000V  最大设置电压 (低档 15.0000V  高档 150.000)
//#ifdef LOADVER1200
//#define		powermax           12000000    //1200.0000W  最大功率
//#elif define LOADVER800
//#define		powermax           8000000    //800.0000W  最大功率
//#elif define LOADVER600
//#define		powermax           6000000    //600.0000W  最大功率
//#endif
#define		MAX_SET_RES           2000000   //200000.0R  最大电阻


#define		MAX_CURRENT    				20000			//最大电流 ( 2A )不同档位电流小数点不同 ( 0- 2.0000, 0-20.000)
#define		MIN_CURRENT    				1000			//最小电流  用于判断档位区间


#define		MAX_CURRENTUP    			30000			//最大电流上升率
#define		MIN_CURRENTUP   			6			    //最小电流上升率
#define		MAX_CURRENTDOWN    		30000			//最大电流下降率
#define		MIN_CURRENTDOWN  			6			    //最小电流下降率
#define		MAX_CURRENTPOOTE  		210000		//最大电流保护值 （21.0000）

#define		MAX_POWERPOOTE  		 	15000			//最大功率保护值 （150.00）

#define		MAX_VOLOTE    				150000		//最大电压  (0-15.0000， 0-150.000)
#define		MIN_VOLOTE    				100			  //最小电压

#define		MAX_LoadVOLOTE    		1200000		//最大开始带载电压值 150.000
#define		MIN_LoadVOLOTE    		0			    //最小开始带载电压值

#define		MAX_FreeLoadVOLOTE    1200000		//最大开始卸载电压值
#define		MIN_FreeLoadVOLOTE    0			    //最小开始卸载电压值


#define		DYN_HIGH_MAX          210000    //动态模式高准位拉载电流 21.0000  MAX
#define		DYN_HIGH_MIN          0         //动态模式高准位拉载电流 0        MIN
#define		DYN_LOW_MAX           210000    //动态模式低准位拉载电流 21.0000  MAX
#define		DYN_LOW_MIN           0         //动态模式低准位拉载电流 0        MIN


#define		DYN_HIGH_DELLMAX      50000000    //动态模式高准位拉载电流时间 50.00000S  MAX  >10s最后一位无效
#define		DYN_HIGH_DELLMIN      10         //动态模式高准位拉载电流时间 0.000010S  MIN

#define		DYN_LOW_DELLMAX      50000000    //动态模式低准位拉载电流时间 50.00000S  MAX >10s最后一位无效
#define		DYN_LOW_DELLMIN      10         //动态模式低准位拉载电流时间 0.000010S  MIN

#define		DYN_SLEW_RISEMAX     30000      //动态模式电流上升率 MAX  3.0000
#define		DYN_SLEW_RISEMIN     6          //动态模式电流上升率 MAX  0.0006

#define		DYN_SLEW_FALLMAX     30000      //动态模式电流下降率 MAX  3.0000
#define		DYN_SLEW_FALLMIN     6          //动态模式电流下降率 MAX  0.0006

//显示刷新滤波次数
#define DISP_FILTER		10

#define STARTDELAY		80

#define SWITCH_DELAY          (3)//开关之后的读取延时
//定义变量
extern uint8_t softswitch;
extern vu8 SystemStatus;//系统状态
extern vu8 SystemMessage;//系统信息
extern u32  g_LcdDirection;
extern u8 Int_Pe3flag;
extern u8 buttonpage1;
extern u8 calpage;
extern FRESULT result;
extern FATFS fs;
extern FIL file;
extern u8 bmpname[30];
//==========================================================
//系统状态
enum SysStatusEnum
{	
	SYS_STATUS_ERROR ,			//异常
	SYS_STATUS_RESET ,			//复位
	SYS_STATUS_POWER ,			//开机
//	SYS_STATUS_POWER ,
	SYS_STATUS_DATASTORE ,		//数据保存
	SYS_STATUS_FILE ,			//文件管理
	SYS_STATUS_SETUPTEST ,			//测量设置
	SYS_STATUS_TEST ,			//测试
	SYS_STATUS_RANGE ,		//档号显示
	SYS_STATUS_RANGECOUNT,	//档计数显示
	SYS_STATUS_ITEM ,			//列表扫描显示
	SYS_STATUS_USERDEBUG,//用户校正 
	SYS_STATUS_FACRDEBUG, //工厂校正
	SYS_STATUS_SYSSET,     //系统设置
    SYS_STATUS_LIMITSET,     //极限设置
    SYS_STATUS_ITEMSET,//列表扫描设置
	SYS_STATUS_TOOL,//工具
	SYS_STATUS_SYS,
    SYS_STATUS_CLEAR,
    SYS_STATUS_SETUP,
    SYS_STATUS_DEBUG,
	SYS_STATUS_LED,
	SYS_STATUS_BATTERY,
	SYS_STATUS_DYNAMIC,
	SYS_STATUS_LIST,		//列表测试
	SYS_STATUS_LOCK
};

//==========================================================




  

typedef struct
{
	vu8 sence;//远端测量
	vu8 mode;//模式
	vu32 voltage;//设置电压
	vu32 current;//设置电流
	vu32 risistence;//设置电阻
	vu32 power;//设置功率
	vu32 ovp;//过电压保护
	vu32 ocp;//过电流保护
	vu32 opp;//过功率保护
//	vu8 remote;//远端测量
	vu8 crange;//电流量程
	vu8 vrange;//电压量程
	vu32 onvol;//启动电压
	vu32 offvol;//关断电压
	vu8 crmode;//恒阻模式
	vu32 autooff;//延时关断
	vu32 maxc;//最大电流
	vu32 maxv;//最大电压
	vu32 maxp;//最大功率
	vu32 crise;//电流上升率
	vu32 cdrop;//电流下降率
	vu32 cvdowntime; //CV模式电压下降时间
	
	vu32 ledvo;//ledvo
	vu32 ledio;//ledio
	vu32 ledrd;//ledrd
	
	
//电池测试
	vu8 loadmode;//电池测试放电模式
	vu32 loadc1;//放电电流1
	vu32 loadc2;//放电电流2
	vu32 loadc3;//放电电流3
	vu8 curvetime;//曲线采样间隔
	vu32 coffv1;//截止电压1
	vu32 coffv2;//截止电压2
	vu32 coffv3;//截止电压3
	vu32 loadr;//放电电阻
	vu32 coffvr;//恒阻模式截止电压
//动态测试
	vu8 testdis;//测量显示
	vu32 valA;//动态模式拉载电流A
	vu32 valB;//动态模式拉载电流B
	vu32 timeA;//动态模式拉载持续时间
	vu8 dynamode;//动态模式触发模式选择
	vu32 dynaIRise;//动态模式电流上升率
	vu32 dynaIDrop;//动态模式电流下降率
	vu32 timeB;//动态模式拉载持续时间B
	
	vu8 qcmode;//快充模式选择开关
	vu32 Class_5;//5V  
	vu32 Class_9;//9V
	vu32 Class_12;//12V
	vu32 Class_15;//15V
	vu32 Class_20;//20V
//极限设置
	u32 vhigh;//电压上限
	u32 vlow;//电压下限
	u32 chigh;//电流上限
	u32 clow;//电流下限
	u32 phigh;//功率上限
	u32 plow;//功率下限
	u8 limitdisp;//极限显示
	u8 limitbeep;//极限讯响
	u8 vcomp;//电压判定
	u8 ccomp;//电流判定
	u8 pcomp;//功率判定
//系统设置
	u8 language;
//列表测试
	u8 ListNum;//列表步数
	u8 StepMode;//步进模式
	u8 LoopTest;//循环测试
	u8 ListBeep;//列表讯响
	vu32 listonvol;//启动电压
	vu32 gatev;//门槛电压
	vu8 listmode[15];//列表负载模式
	vu32 listvalue[15];//列表负载值
	vu16 delay[15];//列表负载延时
	vu8 listcomp[15];//列表负载比较类型
	vu32 listhigh[15];//列表负载上限
	vu32 listlow[15];//列表负载下限
//系统设置
	u16 Baudrate;//波特率
	u16 Addr;//本机地址
//反接记录
	u16 ErrCnt[4];
//版本区分
	u8 Version;//0-1200W;1-800W;2-600W;
//通讯接口
	u8 COMM;//0-232;1-485;
//logo
	u8 jkflag;
//仪器模式 0-主机 1-从机
	u8 devmode;
//从机数量
	u8 devnum;
//从机编号
	u8 slaveNo;
}SaveData_Typedef;
extern SaveData_Typedef LoadSave;



//==========================================================


//==========================================================
//系统项(2项)


typedef struct 
{	
//显示数据
	u32 Voltage;
	u32 Current;
	u32 Rdata;
	u32 Power;
	u32 CalValue[25];
	float Capraw;
	u32 Capacity;
	u8 liststep;
	u8 listrunstep;
	u32 listdelay;
	u8 listcompres[15];
	u32 listv[15];
	u32 listc[15];
	u32 listp[15];
	u8 listvrange[15];
	u8 listcrange[15];
	u8 testcomp[3];
	u8 testbeep;
	u8 Operation_MODE;
	u8 respage;
	u8 protectflag;
	u8 alertdisp;
////校准测量控制电压低档
//	u32 VLOW1;
//	u32 SETVLOW1;
//	u32 VLOW2;
//	u32 SETVLOW2;
////校准测量控制电压高档
//	u32 VHIGH1;
//	u32 SETVHIGH1;
//	u32 VHIGH2;
//	u32 SETVHIGH2;
//	u32 VHIGH3;
//	u32 SETVHIGH3;
////校准测量控制电流低档
//	u32 ILOW1;
//	u32 SETILOW1;
//	u32 ILOW2;
//	u32 SETILOW2;
////校准测量控制电流高档
//	u32 IHIGH1;
//	u32 SETIHIGH1;
//	u32 IHIGH2;
//	u32 SETIHIGH2;
//	u32 IHIGH3;
//	u32 SETIHIGH3;	
	
}DispValues_TypeDef;

typedef struct 
{	
//从机数据
	u32 Voltage[15];
	u32 Current[15];
	u32 Rdata[15];
	u32 Power[15];
	u32 vrange[15];
	u32 crange[15];
}SlaveData_TypeDef;

typedef struct 
{	
//从机参数
	u32 SetCurrent;
	u32 SetRdata;
	u32 SetPower;
}SlavePara_TypeDef;

extern SlavePara_TypeDef SlaveSetPara;
extern SlaveData_TypeDef SlaveValue;
extern DispValues_TypeDef DispValue;
//==========================================================
//
//==========================================================

//==========================================================


//==========================================================

extern u8 F_Fail;//测试失败标志
extern u8 F_100ms;//100ms定时标志
extern u8 F_set;//设置命令定时标志
extern u8 F_1s;//1s定时标志
extern u8 SoftTimer[MAXTSOFTTIMER];

extern u8 USB_Openflag,Disp_usbflag,Disp_RTCflag;
extern u8 UART_Buffer_Rece[BUFFSIZEMAX];
extern u8 UART3_Buffer_Rece[BUFFSIZEMAX];
extern u8 flag_Tim_USART;
extern u16 t_USART;
extern u8 UART_Buffer_Size;
extern u8 UART_Buffer_Rece_flag;
extern u8 UART3_Buffer_Rece_flag;
extern u8 spinbit;
extern u8 spinbitmax;
extern u16 spintest,spinnum;
extern u8 spinflag;
extern u32 dacvalue[17];
extern u8 setflag;
extern u8 setslaveflag;
extern u8 slaveonoffflag;
extern u8 mainswitch;
extern u8 jumpflag;
extern u8 flag_spin;
extern u16 t_SPIN;
extern u8 spinsend; 
extern u8 resflag;
extern u8 resdisp;
extern u8 batstep;
extern u8 listbeep;
extern u8 switchdelay;
extern u8 inputtrans;
extern u8 coder_flag;
extern u16 dacctrl;
extern u8 usart1rxbuff[BUFFSIZEMAX];
extern u8 usart1txbuff[BUFFSIZEMAX];
extern u8 usart3rxbuff[BUFFSIZEMAX];
extern u8 usart3txbuff[BUFFSIZEMAX];
extern u16 Uart1RXbuff_len;
extern u16 Uart3RXbuff_len;
extern u8 spinsetflag;
extern u8 slaveID;
#endif
