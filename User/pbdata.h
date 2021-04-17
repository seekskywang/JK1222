#ifndef _pbdata_H
#define _pbdata_H

#define _4094_STBL GPIO_ResetBits(GPIOE,GPIO_Pin_6)
#define _4094_STBH GPIO_SetBits(GPIOE,GPIO_Pin_6)

#define _5618_CSL GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define _5618_CSH GPIO_SetBits(GPIOC,GPIO_Pin_3)

//#define _DS1302_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_4)
//#define _DS1302_CSH GPIO_SetBits(GPIOB,GPIO_Pin_4)

//#define _CPLD_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_8)
//#define _CPLD_CSH GPIO_SetBits(GPIOB,GPIO_Pin_8)

//hc595接口定义
#define _SPI_595_CSL    GPIO_ResetBits(GPIOA,GPIO_Pin_11)
#define _SPI_595_CSH    GPIO_SetBits(GPIOA,GPIO_Pin_11)

#define _SPI_595_DSL    GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define _SPI_595_DSH    GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define _SPI_595_SHCPL  GPIO_ResetBits(GPIOA,GPIO_Pin_9)
#define _SPI_595_SHCPH  GPIO_SetBits(GPIOA,GPIO_Pin_9)

#define _SPI_595_STCPL  GPIO_ResetBits(GPIOA,GPIO_Pin_10)
#define _SPI_595_STCPH  GPIO_SetBits(GPIOA,GPIO_Pin_10)


#define _7706_CSL GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define _7706_CSH GPIO_SetBits(GPIOD,GPIO_Pin_14)

#define _ADS1216_CS_L GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define _ADS1216_CS_H GPIO_SetBits(GPIOB,GPIO_Pin_10)

#define _SPI_4094_SCLKL GPIO_ResetBits(GPIOC,GPIO_Pin_3)
#define _SPI_4094_SCLKH GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define _SPI_4094_DATAL GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define _SPI_4094_DATAH GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define _SPI_4094_STROL GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define _SPI_4094_STROH GPIO_SetBits(GPIOC,GPIO_Pin_1)


//#define _SPI_SCLKL GPIO_ResetBits(GPIOB,GPIO_Pin_11)
//#define _SPI_SCLKH GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define _SPI_SCLKL GPIO_ResetBits(GPIOI,GPIO_Pin_5)
#define _SPI_SCLKH GPIO_SetBits(GPIOI,GPIO_Pin_5)


#define _SPI_DINL GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define _SPI_DINH GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define _SPI_DOUT GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
//#define _SPI_DOUTH GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define _FMQ_CSL GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define _FMQ_CSH GPIO_SetBits(GPIOB,GPIO_Pin_5)

//#define _TESTLED_CSL GPIO_ResetBits(GPIOE,GPIO_Pin_2)
//#define _TESTLED_CSH GPIO_SetBits(GPIOE,GPIO_Pin_2)


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






//定义函数

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
//void delay_us(u32 nus);
//void delay_ms(u16 nms);
void SetSoftTimer(u8 id, u8 ticks);
u8 GetSoftTimerOut(u8 id);
void Para_Set_Comp(void);
//16bit数据的位操作定义
#define	SetD0	(0x0001)
#define	SetD1	(0x0002)
#define	SetD2	(0x0004)
#define	SetD3	(0x0008)
#define	SetD4	(0x0010)
#define	SetD5	(0x0020)
#define	SetD6	(0x0040)
#define	SetD7	(0x0080)
#define	SetD8	(0x0100)
#define	SetD9	(0x0200)
#define	SetD10	(0x0400)
#define	SetD11	(0x0800)
#define	SetD12	(0x1000)
#define	SetD13	(0x2000)
#define	SetD14	(0x4000)
#define	SetD15	(0x8000)

#define	ClrD0	(0xfffe)
#define	ClrD1	(0xfffd)
#define	ClrD2	(0xfffb)
#define	ClrD3	(0xfff7)
#define	ClrD4	(0xffef)
#define	ClrD5	(0xffdf)
#define	ClrD6	(0xffbf)
#define	ClrD7	(0xff7f)
#define	ClrD8	(0xfeff)
#define	ClrD9	(0xfdff)
#define	ClrD10	(0xfbff)
#define	ClrD11	(0xf7ff)
#define	ClrD12	(0xefff)
#define	ClrD13	(0xdfff)
#define	ClrD14	(0xbfff)
#define	ClrD15	(0x7fff)
#define PASSWORD  "20185260"

#define HW_HW_PGA_MASK		((u8)((ClrD1&ClrD2&ClrD3&ClrD4&ClrD6&ClrD7)))
#define HW_PGA_1K 			((u8)(SetD4))
#define HW_PGA_10K 			((u8)(SetD7))
#define HW_PGA_100K 		((u8)(SetD3))
#define HW_PGA_103K			((u8)(SetD6))
#define HW_PGA_104K			((u8)(SetD2))
#define HW_PGA_105K			((u8)(SetD1))

#define HW_PRO_TEST			((u8)(SetD1))//测试中信号
#define HW_PRO_UNTEST		((u8)(ClrD1))

#define HW_FD_TEST			((u8)(SetD5))//测试
#define HW_FD_NOTEST		((u8)(~SetD5))//放电


#define HW_GYB_100V 		((u8)(ClrD3))
#define HW_GYB_1000V 		((u8)(SetD3))

//#define HW_GYB_1000VFEEDMASK ((u8)(ClrD2&ClrD1))
#define HW_GYB_1000VFEED	((u8)(SetD0))
#define HW_GYB_100VFEED		((u8)(ClrD0))


#define HW_GYBFD_ON			((u8)(SetD1))
#define HW_GYBFD_OFF		((u8)(ClrD1))
//#define HW_HW_GYBDZ_MASK	((u8)((ClrD5&ClrD4)))

//#define HW_GYB_0R			((u8)(SetD4|SetD5))//测试
//#define HW_GYB_10KR			((u8)(ClrD4&ClrD5))//测试
//#define HW_GYB_100KR		((u8)(SetD4))//测试

//==========================================================

#define FIT_INUM    12  //滤波次数
#define STEP_MAX			(4)//最大步骤(0-3)

#define IDLE_MENU_MAX  		(4)//最大待机菜单
#define IDLE_MENU_PAGE 		(4)//每页待机菜单

#define PARAMETER_MENU_MAX	(6)//最大参数菜单
#define PARAMETER_MENU_PAGE	(4)//每页参数菜单

#define PARAMETER_ACW_MAX	(8)//ACW参数项(0-7)
#define PARAMETER_DCW_MAX	(8)//DCW参数项(0-7)
#define PARAMETER_IR_MAX	(6)//IR参数项(0-5)
#define PARAMETER_GND_MAX	(7)//ACW参数项(0-6)

#define SYSTEM_MENU_MAX		(4)//最大系统菜单(0-5)
#define SYSTEM_MENU_PAGE	(4)//每页系统菜单

#define CONFIG_MENU_MAX		(3)//最大配置项(0-2)

#define PW_LENGTH 			(4)//最大密码位数

#define AD_BUF_LENGTH		(8)//A/D值缓冲区长度


#define MAXTSOFTTIMER (2)		//软定时器数目

#define UART_BUFF_LENTH         (REC_LEN_MAX)//串口接收数据长度

#define FIT_NUM                 140

#define MAXFITNUM 0xff

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
#define OHM 				(0xF4)	//CGROM中有Ω字符，编码为0xF4

//==========================================================
//显示分区的X坐标
#define MSG_ADDR_X 			(0)
#define AUTO_ADDR_X 		(7)
#define RANGE_ADDR_X 		(9)

//#define FALSE 0

//#define TRUE 0

#define RES_ADDR_X 			(12)
#define VOL_ADDR_X 			(1)
#define TIM_ADDR_X 			(12)
#define RAN_ADDR_X			(7)

//电流量程上下限
//电流量程上下限
#define RANGR_LIMIT_HIGH (5600000)
#define RANGR_LIMIT_LOW  (560000)

// 电压量程上下限
#define RANGE_LIMIT_VHIGH (120000*45)
#define RANGE_LIMIT_VLOW (11000*45)


#define DEBUG_RANGE 10
//==========================================================
//标准电压DA   50V   500V
//==========================================================
#define  CAL_50V		(3600)
#define  CAL_500V		(3600)

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
#define		MAX_SET_POW           6000000    //120.000W  最大功率
#define		MAX_SET_RES           50000.0   //50000.0R  最大电阻


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

#define SWITCH_DELAY          (3)//开关之后的读取延时
//定义变量
extern uint8_t softswitch;
extern u8 dt;
extern vu8 SystemStatus;//系统状态
extern vu8 SystemMessage;//系统信息
extern vu8 MenuIndex;//待机菜单项
extern vu8 Range;//当前量程
extern vu8 ffit_data1;
extern vu16	count_ffit;
extern u8 fft_flag;
extern u32  g_LcdDirection;
extern u8 polarity_v,polarity_r;
extern u8 Readfirst_cpld,Readsecond_cpld;
extern u8 RangeChange_Flag;
extern u32 fit_i_buff[MAXFITNUM],fit_v_buff[MAXFITNUM];
extern u32 scan_V[130],scan_I[800];
extern u8 range_over;
extern u8 Int_Pe3flag;
extern const u8 fit_allnum[4];
extern u32 last_R_disp,last_V_disp;
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
//系统消息
enum SysMessageEnum
{
	MSG_CLEAR , 
	MSG_IDLE , 
	MSG_TEST , 
	MSG_ABORT , 
	MSG_PASS ,
	MSG_FAIL ,
	MSG_HIGH , 
	MSG_LOW , 
};



//数值框属性定义
typedef struct
{
	vu8 *Title;//标题
	vu16 Num;//数值
	vu8  Dot;//小数点(0-3: xxxx xxx.x xx.xx x.xxx)
	//vu8  Unit;//单位(0-2: MΩ  GΩ  TΩ)
	vu16 Max;//最大值
	vu16 Min;//最小值
}NumBox_TypeDef;
extern NumBox_TypeDef NumBox;//数值框

//项目值																					   
enum ItemEnum
{	
	ITEM_ACW ,
	ITEM_DCW ,
	ITEM_IR ,
	ITEM_GND ,
	ITEM_NONE ,
};
typedef struct
{
    vu8 xpos;
    vu8 ypos;

}Channel_Pos_Typedef;
typedef struct
{
    vu8 Range;//量程  0 自动
    vu8 Speed;//速度 0 慢速  1 中速  2 快速
    vu8 Avg;//平均次数  1-255
    vu8 Scan;//单次或者扫描 0 单路  1 扫描
    vu8 Signel;//0 连续测试  1  单次测试
    vu8 Mode;//测试模式 0 pt100,1 pt500 ,2 0-2v  3 电阻测试
    vu8 Temp_Channel;//选择温度测试的通道

}UINT8_rec_Typedef;
  
typedef struct 
{
	vu16 value;	//数值
	vu8 dot;	//小数点 0 1 2 3    xxxx   xxx.x  xx.xx x.xxx
	vu8 unit;	//单位 0 1 2 3   MΩ    GΩ   TΩ  
	

}SystemRes_TypeDef;
typedef struct
{
    UINT8_rec_Typedef   UINT8_rec;
    Channel_Pos_Typedef Channel[CHANNEL_NUM];//测试输入口坐标
    float Debug_vlaue[RANGE_MAX];//校正值
    float Clear[CHANNEL_NUM];//清零值
}Savedata_2516_Typedef;
extern Savedata_2516_Typedef Savedata_2516;
//==========================================================
//浮点数转换  
typedef struct
{
	u8 Sign;//正负号
	u32 Digit;//尾数
	s8 Exponent;//阶码

}Float_TypeDef;
extern Float_TypeDef Float;
//==========================================================
//校正值
typedef struct 
{
	vu16 Num;  //输出值
	vu16 NumAd;//输出A/D值
}Cal_TypeDef;

//校正项
typedef struct 
{
	Cal_TypeDef IRVol[2];//绝缘电压档  0  50V  1  500V
	Cal_TypeDef IRCur[8];// 0 1K采样电阻档 1  10K  2  100K  3  1000K   4 10M   5  80M 6 64M   7  128M
	
}Calibrate_TypeDef;
extern Calibrate_TypeDef Calibrate;
//==========================================================
//配置项(3项)
typedef struct 
{
	vu16 BootNum;//开机次数(0-9999，默认0)
	vu16 ProductId;//出厂编号(0-9999，默认0)
	vu16 Password;//设置密码(0-9999，默认0)
}Factory_TypeDef;
//==========================================================
//系统项(2项)

typedef struct
{
    vu8 comp;
    vu8 mode;//绝对值  百分比  直读SEQ
    SystemRes_TypeDef Low_limit;//下限
    
    vu8 beep;
    SystemRes_TypeDef criterion;//标称值
    SystemRes_TypeDef Hi_limit;//上限        

}Compset_TypeDef;
typedef struct
{
    vu8 Temp_equalize;//温度补偿
    u32 temp_coefficient;//温度系数
    u32 temp_reference;//参考温度
    u32 Range_Delay;//量程延时
    u8 compute;//计算
    u32 pole_constant;//电极常数

}Setup_TypeDef;
typedef struct
{
    u8 Trip;
    u8 mode;
    u8 Range;
    u8 speed;
    

}Testset_TypeDef;
typedef struct
{
    u8 name;
    u32 password;

}accountnum_TypeDef;
typedef struct
{
    u8 language;
    u8 blacklight;
    u8 turnoff;
    u8 buard;
    accountnum_TypeDef accountnum;//管理员
    

}Sysconfig_TypeDef;

typedef struct
{
	vu8 trip;//触发
	vu8 speed;//测试速度
	vu8 Res_comp;
	Sort_TypeDef Res_low;//电阻下限
	vu8 V_comp;
	Sort_TypeDef V_low;
	vu8 Range;
    vu8 Range_Set;
	vu8 beep;
	vu8 openbeep;
	Sort_TypeDef Nominal_Res;
	Sort_TypeDef High_Res;
	Sort_TypeDef Nominal_V;
	Sort_TypeDef	V_high;
	


}Set_Data_Typedef;

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
	vu8 listmode[15];//列表负载模式
	vu32 listvalue[15];//列表负载值
	vu16 delay[15];//列表负载延时
	vu8 listcomp[15];//列表负载比较类型
	vu32 listhigh[15];//列表负载上限
	vu32 listlow[15];//列表负载下限
}SaveData_Typedef;
extern SaveData_Typedef LoadSave;
typedef struct
{
	vu32 standard;//标准显示值
	float ad_value;

}Debug_Value_Typedef;
typedef struct
{
	vu8 Res_dot;
	vu8 V_dot;
	vu8 V_Neg;
}Test_UnitTypedef;
extern Test_UnitTypedef Test_Unit;
typedef struct
{
	vu8 Hour;
	vu8 Min;
	vu8 Sec;

}Timer_Value_Typedef;
typedef struct
{
	vu8 Year;
	vu8 Mon;
	vu8 data;

}Data_Value_Typedef;
typedef struct
{
	vu8 uart;
	vu8 buard;
    vu8 u_flag;
    vu8 plc;
	vu8 lanage;
    vu8 textname[20];
	//vu8 
	

}Sys_Setvalue_Typedef;

typedef struct
{
//    Compset_TypeDef Compset;//比较设置
//    Setup_TypeDef   Setup;//设置
//    Testset_TypeDef Testset;//测试时的设置数据
//    Sysconfig_TypeDef Sysconfig;//系统设置
    Set_Data_Typedef Set_Data;
	Debug_Value_Typedef Debug_Value[DEBUG_RANGE];
	Debug_Value_Typedef Debug_Value1[DEBUG_RANGE];
	Debug_Value_Typedef Debug_Value2[DEBUG_RANGE];
	Debug_Value_Typedef Debug_Value3[DEBUG_RANGE];
	Sys_Setvalue_Typedef Sys_Setvalue;
//	float clear;
//	float V_Clear;
    float Clear[RANGE_MAX+1];
     float Clear_V[2];
    vu8 fac_num[10];
    vu8 open;
    
    

}Jk516save_TypeDef;



extern Jk516save_TypeDef    Jk516save;

typedef struct//2516上面的测试完的数据存储
{
    SystemRes_TypeDef newvalue[16];
    SystemRes_TypeDef oldvalue[16];


}Test_value_buff_Typedef;
//==========================================================
//系统项(4项)
typedef struct 
{
	vu16 Vol_Out;	//输出电压(10-1000 V)
	vu16 Set_Time;	//测试时间(0-999.9s)
	vu16 Res_Hi;	//上限(xxxx MΩ)
	vu16 Res_Lo;	//下限(xxxx MΩ)

}System_TypeDef;
typedef struct 
{
	vu32 Test_v;	//测试电压(10-1000 V)
	vu8 dot;	//小数点位

}Test_VTypeDef;
typedef struct
{
	float v;//温度
	float i;
	float r;//电阻值
	u32 rate;

}Res_countTypeDef;
extern Res_countTypeDef Res_count;
typedef struct 
{
	vu16 Test_Res;	//测试电压(10-1000 V)
	vu8 dot;	//小数点位
	vu8 unit;

}Test_RESTypeDef;
//==========================================================
//系统项(2项)
typedef struct 
{
	
	Test_VTypeDef Test_v;	//测试电压(10-1000 V)
	vu16 Test_Time;	//测试时间(0-999.9s)
	Test_RESTypeDef Test_Res;	//测试电阻(xxxx MΩ)
	vu16 Test_I;

}Test_TypeDef;

typedef struct 
{	
//显示数据
	u32 Voltage;
	u32 Current;
	u32 Rdata;
	u32 Power;
	u32 CalValue[10];
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

extern DispValues_TypeDef DispValue;
extern Test_TypeDef Test_value;
//==========================================================
//
//==========================================================
//配置项(3项)
typedef struct 
{
	vu16 Password;//设置密码(0-9999，默认0)
	vu16 BootNum;//开机次数(0-9999，默认0)
	vu16 ProductId;//出厂编号(0-9999，默认0)
}Config_TypeDef;
//==========================================================
//参数项(5项)
typedef struct 
{ 
	vu8 Range;//量程模式(0-7，默认0自动)
	vu8 Beep;//讯响模式(0-2，默认开1-PassOn)
	vu8 ClrOn;//清零开关(0-1，默认关0)
	
	vu8 ExTrig;//外部触发开关(0-1，默认关0)
	vu8 UartOn;//串口开关(0-1，默认关0)
//	vu8 FastOn;//快速测试(0-1，默认关0)
}Parameter_TypeDef;


typedef struct
{
    vu8 send_head;
    vu8 king;
    vu8 Range;
    vu8 send_res[7];
    vu8 send_V[8];
    vu8 comp;
    vu8 sendend;
    vu8 over;

}Send_ComBuff_Typedef;
typedef struct
{
    vu8 No[20];
    vu8 Send_res[9];
    vu8 back;
    vu8 Send_V[9];
    vu8 back1;
    vu8 comp[5];
    vu8 ret[2];

}Send_To_UTypedef;
extern Send_To_UTypedef Send_To_U;
typedef struct 
{
   // vu8 Rev_head;
    vu8 Trip;
    vu8 Speed;
    vu8 R_comp;
    
    vu8 Rev_LoRes[6];//4字节数据  一个小数点  一个单位
    vu8 Rev_V_comp;
    vu8 Rev_V_Lo[6];
    vu8 Range;
    vu8 Beep;
    vu8 Norm_Res[6];
    vu8 Rev_Hires[6];
    vu8 Norm_V[6];
    vu8 Rev_VHi[6];


} Rev_Combuff_Typedef;
extern Rev_Combuff_Typedef Rev_Combuff;
extern Send_ComBuff_Typedef Send_ComBuff;

//==========================================================
//保存信息
typedef struct 
{
	u8 group;
	Parameter_TypeDef Parameter;//参数项
	//System_TypeDef System;//系统项
	Config_TypeDef Config;//配置项
	Calibrate_TypeDef Calibrate;//校准项
	vu16 Crc16;//校验字
}Save_TypeDef;
extern System_TypeDef System;
extern u8 F_Fail;//测试失败标志
extern u8 F_100ms;//100ms定时标志
extern u8 F_set;//设置命令定时标志
extern u8 F_1s;//1s定时标志
extern u8 F_Password;//密码有效标志
extern Save_TypeDef SaveData;//保存值
extern vu32 Voltage;//电压
extern vu32 Test_Time;//电流
extern vu32 Resistance;//电阻
extern u8 SoftTimer[MAXTSOFTTIMER];
extern float i_buff[FIT_INUM];		   //滤波值
extern float V_ad,I_ad;
extern vu32 Ad_value;
extern vu8 Ad_over;
extern vu8 Relay_c;
extern vu8 debug_buff[UART_BUFF_LENTH];
extern vu8 test_start;
extern vu8 Set_flag;
extern vu8 debug_flag;
extern vu32 Range_value;
extern vu32 Range_Value_V;
extern  u8 cpld_buff[2];
extern u8 open_flag;//开路标志位
extern vu8 V_Range;
extern u8 USB_Openflag,Disp_usbflag,Disp_RTCflag;
extern u8 UART_Buffer_Rece[200];
extern u8 flag_Tim_USART;
extern u16 t_USART;
extern u8 UART_Buffer_Size;
extern u8 UART_Buffer_Rece_flag;
extern u8 spinbit;
extern u8 spinbitmax;
extern u16 spintest,spinnum;
extern u8 spinflag;
extern u32 dacvalue[10];
extern u8 setflag;
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
#endif
