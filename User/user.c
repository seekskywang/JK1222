//==========================================================
//文件名称：User.c
//文件描述：菜单设置子程序
//文件版本：Ver1.0
//创建日期：2014.12.15
//修改日期：2015.09.06 11:15
//文件作者：
//备注说明：无
//注意事项：无
//==========================================================
#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include <math.h>
////#include <absacc.h>
////#include <intrins.h>
#include "pbdata.h"
//#include <absacc.h>
#include "./APP/usbh_bsp.h"
#include "cpld.h"
//#include "GlobalValue.h"
//#include "Hardware.h"
#include "gpio.h"
#include "./RTC/bsp_rtc.h"
//#include "Interrupt.h"
//#include "Lcd240128.h"
//#include "Disp.h"
//#include "Keyboard.h"
//#include "Beep.h"
//#include "Isp.h"
//#include "Uart0.h"
//#include "Com.h"
//#include "User.h"
u8 DispBuf[DISP_MAX_LEN];//显示缓冲区定义
extern char scpinum[20],scpinum1[20];
extern u8 scpidot,scpiunit,scpidot1,scpiunit1;
extern u8 scpnum,scpnum1;

//==========================================================
const u32 Port_Select[16][16]=//pos(x,y)x0-15 y0-15
{
    {0x00000000,0X10800000,0X10008000,0X01000080,0X14000000,0X10400000,0X10004000,0X10000040,
     0X12000000,0X10200000,0X10002000,0X10000020,0X11000000,0X10010000,0X10000100,0X10000001},
    {0X08020000,0x00000000,0X00028000,0X00020080,0X04020000,0X00420000,0X00024000,0X10000040,
     0X02020000,0X00220000,0X00022000,0X00020020,0X01020000,0X00030000,0X00020100,0X00020001},
    {0X08000200,0X00800200,0x00000000,0X00000280,0X04000200,0X00400200,0X00004200,0X00000240,
     0X02000200,0X00200200,0X00002200,0X00000220,0X01000200,0X00010200,0X00000300,0X00000201},
    {0X08000002,0X00800002,0X00008002,0x00000000,0X04000002,0X00400002,0X00004002,0X00000042,
     0X02000002,0X00200002,0X00002002,0X00000022,0X01000002,0X00010002,0X00000102,0X00000003},
    {0X28000000,0X20800000,0X20008000,0X20000080,0x00000000,0X20400000,0X20004000,0X20000040,
     0X22000000,0X20200000,0X20002000,0X20000020,0X21000000,0X20010000,0X20000100,0X20000001},
    {0X08040000,0X00840000,0X00048000,0X00040080,0X04040000,0x00000000,0X00044000,0X00040040,
     0X02040000,0X00240000,0X00042000,0X00040020,0X01040000,0X00050000,0X00040100,0X00040001},
    {0X08000400,0X00800400,0X00008400,0X00000480,0X04000400,0X00400400,0x00000000,0X00000440,
     0X02000400,0X00200400,0X00002400,0X00000420,0X01000400,0X00010400,0X00000500,0X00000401},
    {0X08000004,0X00800004,0X00008004,0X00000084,0X04000004,0X00400004,0X00004004,0x00000000,
     0X02000004,0X00200004,0X00002004,0X00000024,0X01000004,0X00010004,0X00000104,0X00000005},
    {0X48000000,0X40800000,0X40008000,0X40000080,0X44000000,0X40400000,0X40004000,0X40000040,
     0x00000000,0X40200000,0X40002000,0X40000020,0X41000000,0X40010000,0X40000100,0X40000001},
    {0X08080000,0X00880000,0X00088000,0X00080080,0X04080000,0X00480000,0X00084000,0X00080040,
     0X02080000,0X00000000,0X00082000,0X00080020,0X01080000,0X00090000,0X00080100,0X00080001},
    {0X08000800,0X00800800,0X00008800,0X00000880,0X04000800,0X00400800,0X00004800,0X00000840,
     0X02000800,0X00200800,0X00000000,0X00000820,0X01000800,0X00010800,0X00000900,0X00000801},
    {0X08000008,0X00800008,0X00008008,0X00000088,0X04000008,0X00400008,0X00004008,0X00000048,
     0X02000008,0X00200008,0X00002008,0X00000000,0X01000008,0X00010008,0X00000108,0X00000009},
    {0X88000000,0X80800000,0X80008000,0X80000080,0X84000000,0X80400000,0X80004000,0X80000040,
     0X82000000,0X80200000,0X80002000,0X80000020,0X00000000,0X80010000,0X80000100,0X80000001},
    {0X08100000,0X00900000,0X00108000,0X00100080,0X04100000,0X00500000,0X00104000,0X00100040,
     0X02100000,0X00300000,0X00102000,0X00100020,0X01100000,0X00000000,0X00100100,0X00100001},
    {0X08001000,0X00801000,0X00009000,0X00001080,0X04001000,0X00401000,0X00005000,0X00001040,
     0X02001000,0X00201000,0X00003000,0X00001020,0X01001000,0X00011000,0X00000000,0X00001001},
    {0X08000010,0X00800010,0X00008010,0X00000090,0X04000010,0X00400010,0X00004010,0X00000050,
     0X02000010,0X00200010,0X00002010,0X00000030,0X01000010,0X00010010,0X00000110,0X00000000}

};
const u32 compCcolor[4]={White,Red,White,White};
const u32 compVcolor[4]={White,White,Red,White};
const u32 compPcolor[4]={White,White,White,Red};

const u32 testcompcolor[2]={White,Red};

const uint8_t User_Check_main[][12+1]=
{
	{"档号    "},
	{"标称值  "},
	{"参考值  "},

};
const uint8_t User_Check_Item[][12+1]=
{
	{"电压低档1"},
	{"电压低档2"},	
	{"电压高档1"},
	{"电压高档2"},
    {"电压高档3"},	
	{"电压高档4"},
	{"电压高档5"},
	{"电压高档6"},
	
	{"CV低档1"},
	{"CV低档2"},	
	{"CV高档1"},
	{"CV高档2"},
    {"CV高档3"},
	{"CV高档4"},
	{"CV高档5"},
    {"CV高档6"}
//	
//	{"电流低档4"},
//    {"电流高档1"},
//	
//	{"电流高档2"},
//    {"电流高档3"},
//	{"电流高档4"},
//	{"电流高档5"},
	
	

};

const uint8_t User_Check_Item1[][12+1]=
{
	{"电流低档1"},
	{"电流低档2"},	
	{"电流低档3"},
	{"电流低档4"},
    {"电流高档1"},	
	{"电流高档2"},
	{"电流高档3"},
	{"电流高档4"},
	{"电流高档5"},
	
//	
//	{"电流低档4"},
//    {"电流高档1"},
//	
//	{"电流高档2"},
//    {"电流高档3"},
//	{"电流高档4"},
//	{"电流高档5"},
	
	

};
const uint8_t Test_Setitem[][9+1]=
{
	{"模式    :"},
	{":"},
//	{"电阻下限:"},
//	{"量程    :"},
//	{"电压上限:"},
//	{"电压下限:"},
};

const uint8_t Led_Setitem[][9+1]=
{
	{"LED Vo  :"},
	{"LED Io  :"},
	{"LED Coff:"},
//	{"电阻下限:"},
//	{"量程    :"},
//	{"电压上限:"},
//	{"电压下限:"},
};

const uint8_t Bat_Setitem[2][8][18+1]=
{
	{
	{"放电模式    :"},
	{"放电电流1   :"},
	{"放电电流2   :"},
	{"放电电流3   :"},
	{""},
	{"截止电压1   :"},
	{"截止电压2   :"},
	{"截止电压3   :"},
	},
	{
	{"LOADMODE"},
	{"LOADI1"},
	{"LOADI2"},
	{"LOADI3"},
	{""},
	{"CUTOFFV1"},
	{"CUTOFFV2"},
	{"CUTOFFV3"},
	},
};

const uint8_t Bat_SetitemR[2][9][15+1]=
{
	{
	{"放电模式    :"},
	{"放电电阻    :"},
	{""},
	{""},
	{""},
	{"截止电压    :"},
	{""},
	{""},
	},
	{
	{"LOADMODE   :"},
	{"LOADR      :"},
	{""},
	{""},
	{""},
	{"CUTOFFV    :"},
	{""},
	{""},
	},
};

const uint8_t Dyn_Setitem[][15+1]=
{
	{"测量显示     "},
	{"拉载电流A    "},
	{"拉载电流B    "},
	{"电流上升率   "},
	{"动态模式     "},
	{"拉载持续时间A"},
	{"拉载持续时间B"},
	{"电流下降率   "},
};

const uint8_t Dyn_SetitemE[][15+1]=
{
	{"TESTDISP"},
	{"LOADA"},
	{"LOADB"},
	{"IRISE"},
	{"DYNAMODE"},
	{"TIMEA"},
	{"TIMEB"},
	{"IDROP"},
};

const uint8_t Unit_Setitem[][5+1]=
{
	{"A"},
	{"V"},
	{"Ω"},
	{"W"},
	{"s"},
	{"A/ms"},
	{"步"},
	{"ms"},
};

const uint8_t Unit_Compitem[][5+1]=
{
	{" "},
	{"A"},
	{"V"},
	{"W"},
};

const uint8_t Test_Setitem_E[][9+1]=
{
	{"MODE    :"},
	{":"},

};

const uint8_t Mode_Setitem[2][4][9+1]=
{
	{
	{"电流   :"},
	{"电压   :"},
	{"电阻   :"},
	{"功率   :"},},
	
	{
	{"I      :"},
	{"V      :"},
	{"R      :"},
	{"P      :"},}
};

const uint8_t All_TopName[][21+1]=
{
	{"< 测量显示 >    "},
	{"< LED测试  >    "},
	{"< 电池测试 >  "},
	{"< 动态测试 >"},
	{"< 测量设置 >    "},
	{"< 列表测试 >    "},
	{"< 极限设置 >"},
	{"< 列表扫描设置 >"},
	{"< 系统设置 >    "},
	{"[ LCR文件列表 ] "},
	{"< 系统显示 >    "},

};

const uint8_t All_TopName_E[][21+1]=
{
	{"< MEAS DISP >    "},
	{"< 档号显示 >    "},
	{"< BAT DISP >  "},
	{"< DYNA DISP >"},
	{"< MEAS SETUP >  "},
	{"< LIST DISP >    "},
	{"< LIMIT DISP >"},
	{"< 列表扫描设置 >"},
	{"< SYS SETUP >   "},
	{"[ LCR文件列表 ] "},
	{"< SYS DISP >    "},

};

const uint8_t	Set_testitem[][9+1]=
{
	{"远端测量:"},
	{"电流量程:"},
	{"电压量程:"},
	{"启动电压:"},
	{"关断电压:"},
	{"电压下降:"},
	
	{"延时关断:"},
	{"最大电流:"},
	{"最大电压:"},	
	{"最大功率:"},
	{"上升速率:"},
	{"下降速率:"},
//	{"         "},
//	{"         "},
//	
//	{"         "},
//	{"         "},
//	{"         "} ,
//    {"         "},
};

const uint8_t	Set_testitem_E[][9+1]=
{
	{"REMOTE  :"},
	{"IRANGE  :"},
	{"VRANGE  :"},
	{"STARTV  :"},
	{"TURNOFFV:"},
	{"VDROP   :"},
	
	{"OFFDELAY:"},
	{"MAXI    :"},
	{"MAXV    :"},
	{"MAXP    :"},	
	{"IRISE   :"},
	{"IDROP   :"},

};

const uint8_t	Limit_item[][9+1]=
{
	{"电压上限:"},
	{"电压下限:"},
	{"电流上限:"},
	{"电流下限:"},
	{"功率上限:"},
	{"功率下限:"},
	
	{"极限显示:"},
	{"极限讯响:"},
	{"电压判定:"},	
	{"电流判定:"},
	{"功率判定:"},
	
//	{"         "},
//	{"         "},
//	{"         "},
//	
//	{"         "},
//	{"         "},
//	{"         "} ,
//    {"         "},
};

const uint8_t	Limit_itemE[][9+1]=
{
	{"VHIGH"},
	{"VLOW"},
	{"IHIGH"},
	{"ILOW"},
	{"PHIGH"},
	{"PLOW"},
	
	{"DISP"},
	{"BEEP"},
	{"VCOMP"},	
	{"ICOMP"},
	{"PCOMP"},
	
//	{"         "},
//	{"         "},
//	{"         "},
//	
//	{"         "},
//	{"         "},
//	{"         "} ,
//    {"         "},
};

const uint8_t Test_Modevalue1[][6+1]=
{
	{"定电流"},
	{"定电压"},
	{"定电阻"},
	{"定功率"},
	{"动态"},
	{"LED"},
	{"短路"},
	{"更多"},

};

const uint8_t List_Setitem[][14+1]=
{
	{"列表步数  :"},
	{"步进模式  :"},
	{"门槛电压  :"},
	{"列表讯响  :"},
	{"启动电压  :"},
};

const uint8_t List_Setitem_E[][14+1]=
{
	{"List Steps :"},
	{"Step Mode  :"},
	{"THRESHOLD  :"},
	{"LIST BEEP  :"},
	{"START V    :"},
	
};

const uint8_t Test_Modevalue_E1[][6+1]=
{
	{"CC"},
	{"CV"}, 
	{"CR"},
	{"CW"},
	{"DYNA"},
	{"LET"}, 
	{"SHORT"},
	{"MORE"},
};

const uint8_t Bat_Modevalue[][6+1]=
{
	{"恒流"},
	{"恒阻"},
	{"恒阻"}, 
};

const uint8_t Bat_ModevalueE[][6+1]=
{
	{"CC"},
	{"CR"},
	{"CR"}, 
};

const uint8_t Calvalue[][6+1]=
{
	{"电压"},
	{"电流"},
	{"档位"},
	{"设置"},
	{"校准"},
};


const uint8_t Test_Modevalue2[][6+1]=
{
	{"短路"},
	{"LED"},
	{"短路"},
	{"更多"},
};

const uint8_t Test_Modevalue_E2[][6+1]=
{
	{"CV"},
	{"CC"},
	{"定电阻"},
	{"定功率"},
};

const uint8_t Bat_LoadMod[][6+1]=
{
	{"恒流"},
	{"恒阻"},
	{"恒阻"},
};

const uint8_t Test_Tirpvalue[][6+1]=
{
	{"自动"},
	{"手动"},
	{"外部"},
	{"远程"}

};
const uint8_t Test_Tirpvalue_E[][6+1]=
{
	{"AUTO"},
	{"MAN"},
	{"EXT"},
	{"REMOTE"}

};



const uint8_t Test_Speedvalue[][6+1]=
{
	{"慢速"},
	{"中速"},
    {"快速"},
    {"极速"}

};
const uint8_t Test_Speedvalue_E[][6+1]=
{
	{"SLOW"},
	{"MED"},
    {"FAST"},
    {"TOP"}

};
const uint8_t Test_Compvalue[][6+1]=
{
	{"关闭"},
	{"打开"}

};

const uint8_t List_Beep[][6+1]=
{
	{"合格"},
	{"不合格"}

};

const uint8_t List_BeepE[][6+1]=
{
	{"PASS"},
	{"FAIL"}

};

const uint8_t Tran_Mode[][6+1]=
{
	{"连续"},
	{"脉动"},
	{"触发"}

};

const uint8_t Tran_ModeE[][6+1]=
{
	{"CONST"},
	{"PULSE"},
	{"TIRG "}

};

const uint8_t Test_Compvalue_E[][6+1]=
{
	{"OFF"},
	{"ON "}

};

const uint8_t Test_Commvalue_E[][6+1]=
{
	{"RS232"},
	{"RS485"},
  {"RS232"},
};

const uint8_t Test_PCTLvalue_E[][6+1]=
{
	{"MODBUS"},
	{"SCPI"},
	{"SCPI"}

};
const uint8_t Test_Rangevalue[][6+1]=
{
	{"低"},
	{"高"}

};
const uint8_t Test_Rangevalue_E[][6+1]=
{
	{"LOW"},
	{"HIGH"}

};

const uint8_t Setup_Beep[][6+1]=
{
	"关闭  ",
	"合格  ",
	"不合格",
	"开路ON"
};
const uint8_t Setup_Beep1[][7+1]=
{
	"关 闭  ",
	"合格  ",
	"不合格",
	"开路OFF"
};
const uint8_t Setup_Beep_E[][6+1]=
{
	"CLOSE ",
	"PASS  ",
	"FAIL ",
	"OPENON"
};
const uint8_t Setup_Beep_E1[][7+1]=
{
	"CLOSE ",
	"PASS  ",
	"FAIL ",
	"OPENOFF"
};
const uint8_t INPUT[][7]={"输入"};
const uint8_t INPUT_E[][7]={"INPUT"};
    

const uint8_t User_Range[][10+1]=
{
    {"自动  "},
    {"10 mΩ"},
    {"100mΩ"},
    {"1Ω   "},
    {"10Ω  "},
    {"100 Ω"},
    {"1k  Ω"},
    {"10k Ω"},
    

};
const uint8_t User_Range_E[][10+1]=
{
    {"AUTO  "},
    {"10 mΩ"},
    {"100mΩ"},
    {"1Ω   "},
    {"10Ω  "},
    {"100 Ω"},
    {"1k  Ω"},
    {"10k Ω"},
    

};
const u8 DISP_UINT[][4]=
{
    {" V"},
    {" Ω"},
    {" A"},
	{" s"},
	{" W"}

};

const u8 MODE_UINT[][4]=
{
    {" A"},
    {" V"},
    {" Ω"},
	{" W"},
};

const uint8_t User_Rangeset[][11]=
{
    {"AUTO"},
    {"HAND"},
    {"NORMAL"},
    {"INCR +"},
    {"DECR -"},
    
};
const uint8_t Sys_Setitem[][10+1]=
{
	{"本机地址"},
	{"通讯协议"},
	{"U盘开关"},
	{"通讯接口"},
	{"显示语言"},
	
	{"日期"},
    {"时间"},
//    {"文件名称"},
	
};
const uint8_t Sys_Setitem_E[][10+1]=
{
	{"ADDR"},
	{"PROTOCOL"},
	{"U_DISK"},
	{"I/O_PORT"},
	{"LANGUAGE"},
	
	{"DATE"},
  {"TIME"},
//    {"FILE"},
	
};

const uint8_t Sys_Buard_Value[][6+1]=
{
	
	{"4800"},
	{"9600"},
	{"14400"},
	{"19200"},
	{"115200"},
	{"38400"},
	{"56000"},
	{"2400"},

};
const uint8_t Sys_Language_Value[][7+1]=
{
	{"  中文"},
	{"ENGLISH"}

};
const uint8_t BiasButton_Tip[][7+1]=  //频率选择时候的下面的提示符号
{
    {"DECR -"},
    {"INCR +"},

};

const uint8_t Sys_Sys[][20+1]=
{
	{"仪器型号  JK5016"},
	{"软件版本  Ver:1.0"},
	{"硬件版本  Ver:1.0"},
	{"仪器编号"},
//	{"账号    "},



};
const uint8_t Sys_Sys_E[][20+1]=
{
	{"INST MODEL  JK5016"},
	{"SOFT VER   Ver:1.0"},
	{"HARD VER   Ver:1.0"},
	{"SERIALNO"},
//	{"账号    "},



};
const uint8_t Sys_Sys1[][20+1]=
{
	{"仪器型号  2520C"},
	{"软件版本  Ver:2.5"},
	{"硬件版本  Ver:1.1"},
	{"仪器编号"},
//	{"账号    "},



};
const uint8_t Sys_Sys_E1[][20+1]=
{
	{"INST MODEL  2520C"},
	{"SOFT VER   Ver:2.5"},
	{"HARD VER   Ver:1.1"},
	{"SERIALNO"},
//	{"账号    "},



};
const uint8_t Range_Disp_Test[][7][11+1]=
{
    {
        {"AUTO 10mΩ"},
        {"AUTO100mΩ"},
        {"AUTO   1Ω"},
        {"AUTO  10Ω"},
        {"AUTO 100Ω"},
        {"AUTO  1kΩ"},
        {"AUTO 10kΩ"},
       
    },
    {
        {"HAND 10mΩ"},
        {"HAND100mΩ"},
        {"HAND   1Ω"},
        {"HAND  10Ω"},
        {"HAND 100Ω"},
        {"HAND  1kΩ"},
        {"HAND 10kΩ"},
       
    }


};

const uint8_t Step_Mode[][6+1]=
{
	" 连续",
	" 触发",
	" 自动",
};

const uint8_t Step_ModeE[][6+1]=
{
	" CONS",
	" TRIG",
	" AUTO",
};

const uint8_t List_ItemDis[][6+1]=
{
	"定电流",
	"定电压",
	"定电阻",
	"定功率",
//	"开路",
	"短路空",
};

const uint8_t List_Res[][6+1]=
{
	"合格",
	"不合格",
	"不合格",
	"不合格",
	"无",
};

const uint8_t List_ResE[][6+1]=
{
	"PASS",
	"FAIL",
	"FAIL",
	"FAIL",
	"NONE",
};

const uint8_t List_ItemDisE[][6+1]=
{
	"CC   ",
	"CV   ",
	"CP   ",
	"CR   ",
//	"OPEN",
	"SHORT",
};

const uint8_t List_CompType[][6+1]=
{
	"关闭",
	"按电流",
	"按电压",
	"按功率",
};


const uint8_t List_ResBack[][6+1]=
{
	"",
	"",
	"上一页",
	"下一页",
	"返回",
};

const uint8_t List_ResBackE[][6+1]=
{
	"",
	"",
	"",
	"",
	"BACK",
};

const uint8_t List_CompTypeE[][8+1]=
{
	"OFF",
	"Current",
	"Voltage",
	"Power",
};
const u8 RANG_4094[8]={0x48,0x48,0x48,0x80,0x01,0x01,0x02,0x04};//换挡的时候的4094的值
//位操作增减数值
const u16 POW_NUM[4]=
{
	1000,
	100,
	10,
	1,
};
const u32 RANGE_RATE[5]=
{
	1000,
	10000,
	100000,
	1000000,
	10000000,
	


};
const vu32 Debug_Compvalue[][2]=
{
	{8000,12000},
	{8000,12000},
	{8000,12000},
	{8000,12000},
  {8000,12000},
	{8000,12000},
	{8000,12000},
	{50000,80000},
	{50000,80000}


};
const vu32 Debug_Compvaluemind[]=
{
	10000,10000,10000,10000,10000,10000,60000,60000


};
#define FACTORY_MENU_MAX (sizeof(FactoryTab)/sizeof(FactoryTab[0])-1)
    Test_ValueTypedef Test_Value;

Test_ValueTypedef Test_Value_V;
//==========================================================
//设置菜单

//==========================================================
//确认提示

//==========================================================
//数值提示

//==========================================================
//选择提示

//==========================================================
//参数菜单

//==========================================================
//编号

//==========================================================
//系统菜单

//==========================================================
//系统项上、下限
const u16 SystemLimit[][2]=
{
	{500, 1},
	{9999, 0},
	{9999, 0},
	{9999, 0},
};
//==========================================================
//系统项上、下限
const u16 SystemLimitDot[5]=
{
	0,1,0,0
	
};
//==========================================================
//参数项上、下限
const u8 ParameterLimit[][2]=
{
	{ RANGE_MAX, 0},//量程模式
	{ 2, 0},//速度
	{ 255, 1},//平均次数
	{ 1, 0},//扫描次数
	{ 3, 0},//测试模式
    { 14, 0},//温度通道选择
};
const float Debug_Limit[2]={1.3,0.7};
//==========================================================
//系统状态


//==========================================================
//出厂信息

//==========================================================

//==========================================================
//参数菜单项
enum ParameterMenuEnum
{	
	PARAMETER_MENU_RANGE ,
	PARAMETER_MENU_BEEP ,
	PARAMETER_MENU_CLEAR ,
	PARAMETER_MENU_TRIG ,
	PARAMETER_MENU_UART ,
};

//==========================================================
//系统菜单项
enum SYSTEMMenuEnum
{	
	SYSTEM_MENU_VOUT ,
	SYSTEM_MENU_Ttime,
	SYSTEM_MENU_RH	 ,
	SYSTEM_MENU_RL	 ,
	
};
//==========================================================
//函数名称：InitGlobalValue
//函数功能：初始化全局变量
//入口参数：无
//出口参数：无
//创建日期：2015.10.28
//修改日期：2015.10.28 15:33
//备注说明：无
//==========================================================
void InitGlobalValue(void)
{
	u16 len;
	u8 * buf;
						 
	buf=(u8 *)&Jk516save;//数据首地址
	len=sizeof(Jk516save_TypeDef);
	while(len--)
	{
		*buf=0;//清空
		buf++;
	}
	F_Password=FALSE;//密码有效标志	
}


void Hex_Format(u32 dat , u8 Dot , u8 len , u8 dispzero)
{
	u8 i,j;

	if(len>8)len=8;//最大长度8
	if(len==0)len=1;
	if(Dot>7)Dot=7;//最大小数点7
	if(Dot>len)Dot=len;

	for(i=0;i<sizeof(DispBuf);i++)	//清空缓冲区
		DispBuf[i]=' ';

	for(i=len;i>0;i--)	//数制转换
	{
		DispBuf[i-1] = dat%10 + '0';//转为ASCII
		dat/=10;
	}
	
	if(Dot)//有小数点
	{
		j=len;
		i=Dot;//取小数点位置
		while(i--)
		{
			DispBuf[j]=DispBuf[j-1];
			j--;
		}
		DispBuf[j]='.';//加入小数点
	}

	if(dispzero==FALSE)//不显示无效零
	{
		for(i=0;i<(len-1);i++)
		{
			if((DispBuf[i]=='0')&&(DispBuf[i+1]!='.'))
				DispBuf[i]=' ';
			else
				break;
		}
	}
	if(Dot)len++;
	for(i=len;i<sizeof(DispBuf);i++)//末尾补结束符'\0'
	{
		DispBuf[i]=0;
	}			
}





const u8 send_dot[]={3,2,4,3,2,4,3,2,4,3,2};
const u8 Send_uint[]={0,0,1,1,1,2,2,2,3,6,6};
Test_ValueTypedef Datacov(s32 value,u8 range)
{
    Test_ValueTypedef midvalue;
    
    midvalue.polar=polarity_r;
    midvalue.res=value;
    midvalue.dot=send_dot[range];
    midvalue.uint=Send_uint[range];
    return midvalue;

}


Test_ValueTypedef V_Datacov(s32 value ,u8 range)
{
    Test_ValueTypedef midvalue;
    
    midvalue.polar=polarity_v;
        
    midvalue.res=value;
    if(range)
        midvalue.dot=3;
    else
        midvalue.dot=4;
    midvalue.uint=0;
    return midvalue;

}

u8 Comp_choice(void)//三种模式分选
{
    u8 comp=0;// 0 合格  1  失败
    u32 testvalue,hi_value,lo_value;
//    testvalue=(u32)Test_Value.res*pow(10,Test_Value.uint-3)/pow(10,Test_Value.dot);//减3以后是m欧姆为单位
//    hi_value=(u32)Jk516save.Compset.Hi_limit.value*pow(10,Jk516save.Compset.Hi_limit.unit-3)/pow(10,Jk516save.Compset.Hi_limit.dot);
//    lo_value=(u32)Jk516save.Compset.Low_limit.value*pow(10,Jk516save.Compset.Low_limit.unit-3)/pow(10,Jk516save.Compset.Low_limit.dot);
    if(testvalue>=hi_value)
        comp=1;//上限失败
    else if(testvalue<=lo_value)
        comp=2;//下限失败
    else
        comp=0;//合格
    return comp;
    
    
}
void Beep_Out(u8 flag)
{
    if(Jk516save.Set_Data.beep==0)
        Beep_Off();
    else
    {
        if(flag==0)//合格
        {
            if(Jk516save.Set_Data.beep==1)//合格响
            {
                Beep_On();
            }
            else
                Beep_Off();
        }
        else//不合格
        {
            if(Jk516save.Set_Data.beep==2)//不合格响
            {
                Beep_On();
            }
            else
                Beep_Off();
        }
    }

}

const u8 Test_Uint[][4]=
{
    {"mΩ"},
    {" Ω"},
    {"kΩ"},
    {"MΩ"},

};

//反接警告
void MissConDisp(void)
{
	Colour.black = LCD_COLOR_TEST_BUTON;
	LCD_DrawFullRect(120, 92,360-120 ,205-92);
	Colour.black = Red;
	LCD_FillTriangle(150,130,170,100,140,140);
	Colour.black = LCD_COLOR_YELLOW;
	LCD_DrawFullRect(148,110,153-148,130-110);
	LCD_DrawFullRect(148,132,153-148,137-132);
	Colour.Fword = Red;
	Colour.black = LCD_COLOR_TEST_BUTON;
	WriteString_16(134,145,"警告",0);
	WriteString_16(175,100,"电源反接",0);
	WriteString_16(175,120,"请先断开电源",0);
	WriteString_16(175,140,"再按确认键恢复测试",0);
	
//	LCD_DrawLine();
}

void Disp_Open(void)
{
    u32 colour;
    colour= Colour.black;
    Colour.black=LCD_COLOR_TEST_MID;
	V_POS();
    if(Jk516save.Set_Data.speed!=3)
    {
        if(Jk516save.Set_Data.speed==0)
        {
            WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y, "--------",0 );
            WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y+TEST_VALUE_YMID, "--------",0 );
        }
        else
        {
            WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+16, "--------",1 );
            WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+TEST_VALUE_YMID+16, "--------",1 );
        
        }
    }
    else
    {
         WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+32, "--------",0);
         WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+TEST_VALUE_YMID+32, "--------",0 ); 
    
    
    }
	Test_Value.res = 0;
	Test_Value_V.res = 0;
	if(Jk516save.Set_Data.openbeep == 0)
	{
		Close_Compled();
		Beep_Off();
	//    Beep_Out(0);
		LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
		LCD_ShowFontCN_40_55(60+40*6,92,40,55, (uint8_t*)Out_Assic+14*40*55/8);
	}
	if(Jk516save.Set_Data.V_comp==1 || Jk516save.Set_Data.Res_comp==1)
	{
		if(Jk516save.Set_Data.openbeep == 0)
		{
			Close_Compled();
			Beep_Off();
		//    Beep_Out(0);
			LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
			LCD_ShowFontCN_40_55(60+40*6,92,40,55, (uint8_t*)Out_Assic+14*40*55/8);
		}else{
			Colour.black=LCD_COLOR_RED;
			Beep_Out(1);
//			Led_Fail_On();
			memcpy(DispBuf,"RV FL",5);
			Send_ComBuff.comp=3;                                   
			memcpy((void *)Send_To_U.comp,DispBuf,5);
			DispBuf[5]=0;
			LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
			WriteString_16(SORTING_XDISP, SORTING_Y_DISP, DispBuf,  0);
		}
	}else{
		Close_Compled();
		Beep_Off();
	//    Beep_Out(0);
		LCD_DrawFullRect(SORTING_XDISP, SORTING_Y_DISP, 60, 22);
		LCD_ShowFontCN_40_55(60+40*6,92,40,55, (uint8_t*)Out_Assic+14*40*55/8);
	}
    //WriteString_16 ( TESTVALUE_X, SORTING_Y_DISP+30, "RV_OPEN",0 ); 
    Colour.black=colour;
    Disp_Range(Jk516save.Set_Data.Range_Set,Range);
    

}
void Disp_Testvalue(Test_ValueTypedef value,Test_ValueTypedef value_v,u8 speed)
{
    u8 i;
//    u32 date;
    u32 Res_disp;
    Res_disp=value.res;
    for(i=0;i<9;i++)
    DispBuf[i]=0;
    #ifdef BASTARDLY
    if(value.res>last_R_disp)
    {
       date= value.res-last_R_disp;
    
    }
    else
    {
        date=last_R_disp-value.res;
    
    
    }
    if(date>=3)
    {
        last_R_disp=value.res;
        Res_disp=value.res;
    
    }
    else
    {
        Res_disp=last_R_disp;
    
    }
    #endif
    if(Res_disp>33000)
    {
        for(i=0;i<6;i++)
        DispBuf[i]='-';
        
    }
    else
    Hex_Format(Res_disp , value.dot , 5, FALSE);  
    memcpy((void *)Send_ComBuff.send_res,DispBuf,6);
    memcpy((void *)Send_To_U.Send_res,DispBuf,6);//电阻
    memcpy((void *)&Send_To_U.Send_res[6],DISP_UINT[Test_Value.uint],3);//单位
   Send_To_U.back=9;
    DispBuf[6]=' ';
    if(Jk516save.Set_Data.speed!=3)
    {
        if(Jk516save.Set_Data.speed==0)
        {
            if(value.polar)
                WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y, " ",0); 
            else
                WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y, "-",0 );
            
              WriteString_Big ( TESTVALUE_X, TESTVALUE_Y, DispBuf ,0); 
        }
        else
        {
            if(value.polar)
                WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+16, " ",1); 
            else
                WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+16, "-",1 );
              WriteString_Big ( TESTVALUE_X, TESTVALUE_Y+16, DispBuf ,1); 
        
        
        }
    }
    else
    {
        if(value.polar)
            WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+32, " ",0 ); 
        else
            WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+32, "-",0 );
        
          WriteString_16 ( TESTVALUE_X+10, TESTVALUE_Y+32, DispBuf,0 ); 
    
    
    }
        
    
        Hex_Format(value_v.res , value_v.dot , 6, FALSE);
        memcpy((void *)&Send_ComBuff.send_V[1],DispBuf,7);//
        memcpy((void *)&Send_To_U.Send_V[1],DispBuf,7);//存U盘
        Send_To_U.Send_V[8]='V';
        Send_To_U.back1=9;
        memcpy((void *)&Send_To_U.ret[0],"\r\n",2);
        //Send_To_U.ret="/r/n";
     if(Jk516save.Set_Data.speed!=3)
     {
         if(Jk516save.Set_Data.speed==0)
         {
        
           if(value_v.polar)
               WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y+TEST_VALUE_YMID, " ",0 ); 
           else
               WriteString_Big ( TESTVALUE_X-32, TESTVALUE_Y+TEST_VALUE_YMID, "-",0 );
          WriteString_Big ( TESTVALUE_X, TESTVALUE_Y+TEST_VALUE_YMID, DispBuf,0 ); 
        }
         else
         {
             if(value_v.polar)
               WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+TEST_VALUE_YMID+16, " ",1 ); 
           else
               WriteString_Big ( TESTVALUE_X-16, TESTVALUE_Y+TEST_VALUE_YMID+16, "-",1 );
          WriteString_Big ( TESTVALUE_X, TESTVALUE_Y+TEST_VALUE_YMID+16, DispBuf,1 ); 
         }
    }
     else
     {
         if(value_v.polar)
            WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+TEST_VALUE_YMID+32, " ",0 ); 
        else
            WriteString_16 ( TESTVALUE_X, TESTVALUE_Y+TEST_VALUE_YMID+32, "-",0 );
          WriteString_16 ( TESTVALUE_X+10, TESTVALUE_Y+TEST_VALUE_YMID+32, DispBuf,0 ); 
     
     }
     if(value_v.polar)
     {
         Send_ComBuff.send_V[0]=' ';
         Send_To_U.Send_V[0]=' ';
         V_POS();
     }
     else
     {
         Send_ComBuff.send_V[0]='-';
         Send_To_U.Send_V[0]='-';
         V_NEG();
     }
     Send_ComBuff.send_res[6]=Test_Value.uint;
         
        
        
   
    
//    if(value_v.polar)
//        WriteString_16( TESTVALUE_X-8, TESTVALUE_Y, "-",1);
//    else
//        WriteString_16( TESTVALUE_X-8, TESTVALUE_Y," ",1);
//        
//    WriteString_16( TESTVALUE_X+100, TESTVALUE_Y, DispBuf,1 );
    
}
void Disp_xxx(u16 data,u16 pos)
{
    Hex_Format(data , 0 , 5, FALSE);
    LCD_DispString_EN_CH( TESTVALUE_X+pos, TESTVALUE_Y, DispBuf );



}
void Disp_Hint(u8 num)
{
	if(LoadSave.language == 0)
	{
		if(num == 1)
		{
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2+40,205,"请用数字键盘或旋钮输入",0);
		}else if(num == 2){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+60,205,"请用数字键盘输入",0);
		}else if(num == 3){
			Colour.black=LCD_COLOR_TEST_MID;
			LCD_DrawFullRect(LIST2+60,205,150,SPACE1-2);
		}else if(num == 4){
			Colour.black=LCD_COLOR_TEST_BACK;
			LCD_DrawFullRect(LIST2+60,205,150, SPACE1-2);
		}else if(num == 5){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+60,205,"按下旋钮切换步骤",0);
		}else if(num == 6){
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2+60,205,"按下旋钮快速选择",0);
		}else if(num == 7){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+60,205,"按下旋钮快速选择",0);
		}else if(num == 8){
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2+60,205,"请用数字键盘输入",0);
		}
	}else{
		if(num == 1)
		{
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2-80,205,"Input with keyboard or encoder.",0);
		}else if(num == 2){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+30,205,"Input with keyboard.",0);
		}else if(num == 3){
			Colour.black=LCD_COLOR_TEST_MID;
			LCD_DrawFullRect(LIST2-80+24+24,205,150+130-24,SPACE1-2);
		}else if(num == 4){
			Colour.black=LCD_COLOR_TEST_BACK;
			LCD_DrawFullRect(LIST2-80+24+24,205,150+130-24, SPACE1-2);
		}else if(num == 5){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+30,205,"Press the encoder.  ",0);
		}else if(num == 6){
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2+30,205,"Press the encoder.  ",0);
		}else if(num == 7){
			Colour.black=LCD_COLOR_TEST_BACK;
			Colour.Fword = Red;
			WriteString_16(LIST2+30,205,"Press the encoder.  ",0);
		}else if(num == 8){
			Colour.black=LCD_COLOR_TEST_MID;
			Colour.Fword = Red;
			WriteString_16(LIST2+30,205,"Input with keyboard.",0);
		}
	}
}

void Test_Disp(u8 vsw,u8 isw)
{
	
	Colour.black=LCD_COLOR_TEST_MID;
	Colour.Fword = testcompcolor[DispValue.testcomp[0]];
	if(vsw == 0)
	{
//		if(DispValue.Voltage<100)
//		{
//			Hex_Format(0,4,7,0);  
//		}else{
			Hex_Format(DispValue.Voltage,4,7,0);    
//		}			
		Disp_EN40(80-60,80,DispBuf);
		Disp_EN40(310-60,80,"V");
	}else if(vsw == 1){
//		if(DispValue.Voltage<100)
//		{
//			Hex_Format(0,3,6,0);       
//		}else{
			Hex_Format(DispValue.Voltage,3,6,0);       
//		}	
		Disp_EN40(80-60+28,80,DispBuf);
		Disp_EN40(310-60,80,"V");
	}
	
	Colour.Fword = testcompcolor[DispValue.testcomp[1]];
	if(isw == 0)
	{
		Hex_Format(DispValue.Current,4,7,0);  
		Disp_EN40(80-60,80+40,DispBuf);
		Disp_EN40(310-60,80+40,"A");
	}else if(isw == 1){
		Hex_Format(DispValue.Current,3,6,0);  
		Disp_EN40(80-60+28,80+40,DispBuf);
		Disp_EN40(310-60,80+40,"A");
	}

	Colour.Fword = testcompcolor[DispValue.testcomp[2]];
	Hex_Format(DispValue.Power,3,7,0);  
	Disp_EN40(80-60,80+80,DispBuf);
	Disp_EN40(310-60,80+80,"W"); 
	
	if(LoadSave.limitdisp == 1)
	{
		Colour.Fword = LCD_COLOR_GREY;
		Hex_Format(LoadSave.vhigh,4,7,0); 
		WriteString_16(LIST2+88,80,DispBuf,0);
		WriteString_16(LIST2+88+82,80,"V",0);
		
		Hex_Format(LoadSave.vlow,4,7,0); 
		WriteString_16(LIST2+88,80+20,DispBuf,0);
		WriteString_16(LIST2+88+82,80+20,"V",0);
		
		Hex_Format(LoadSave.chigh,4,7,0); 
		WriteString_16(LIST2+88,80+40,DispBuf,0);
		WriteString_16(LIST2+88+82,80+40,"A",0);
		
		Hex_Format(LoadSave.clow,4,7,0); 
		WriteString_16(LIST2+88,80+60,DispBuf,0);
		WriteString_16(LIST2+88+82,80+60,"A",0);
		
		Hex_Format(LoadSave.phigh,4,7,0); 
		WriteString_16(LIST2+88,80+80,DispBuf,0);
		WriteString_16(LIST2+88+82,80+80,"W",0);
		
		Hex_Format(LoadSave.plow,4,7,0); 
		WriteString_16(LIST2+88,80+100,DispBuf,0);
		WriteString_16(LIST2+88+82,80+100,"W",0);
	}
}


void Dynamic_Disp(u8 vsw,u8 isw)
{
	
	Colour.black=LCD_COLOR_TEST_MID;
	if(LoadSave.testdis == 0)
	{
//		Hex_Format(DispValue.Voltage,4,7,0);       
		Disp_EN40(80,120,"       ");
		Disp_EN40(310,120," ");
	
//		Hex_Format(DispValue.Current,4,7,0);  
		Disp_EN40(80,120+60,"       ");
		Disp_EN40(310,120+60," ");
	}else if(LoadSave.testdis == 1){
		if(vsw == 0)
		{
			if(DispValue.Voltage<100)
			{
				Hex_Format(0,4,7,0);  
			}else{
				Hex_Format(DispValue.Voltage,4,7,0);    
			}	      
			Disp_EN40(80,120,DispBuf);
			Disp_EN40(310,120,"V");
		}else if(vsw == 1){
			if(DispValue.Voltage<100)
			{
				Hex_Format(0,3,6,0);       
			}else{
				Hex_Format(DispValue.Voltage,3,6,0);       
			}       
			Disp_EN40(80,120,DispBuf);
			Disp_EN40(310,120,"V");
		}
		if(isw == 0)
		{
			Hex_Format(DispValue.Current,4,7,0);  
			Disp_EN40(80,120+60,DispBuf);
			Disp_EN40(310,120+60,"A");
		}else if(isw == 1){
			Hex_Format(DispValue.Current,3,6,0);  
			Disp_EN40(80,120+60,DispBuf);
			Disp_EN40(310,120+60,"A");
		}	
	}
	
//	Hex_Format(DispValue.Power,3,6,0);  
//	Disp_EN40(80,80+80,DispBuf);
//	Disp_EN40(310,80+80,"W"); 
}

void Bat_Disp(u8 vsw,u8 isw)
{
	Colour.black=LCD_COLOR_TEST_MID;
	if(vsw == 0)
	{
		if(DispValue.Voltage<100)
		{
			Hex_Format(0,4,7,0);  
		}else{
			Hex_Format(DispValue.Voltage,4,7,0);    
		}
		Hex_Format(DispValue.Voltage,4,7,0);       
		WriteString_16(80,100+30,DispBuf,0);
		WriteString_16(160,100+30,"V",0);
	}else if(vsw == 1){
		if(DispValue.Voltage<100)
		{
			Hex_Format(0,3,6,0);  
		}else{
			Hex_Format(DispValue.Voltage,3,6,0);    
		}
//		Hex_Format(DispValue.Voltage,3,6,0);       
		WriteString_16(80,100+30,DispBuf,0);
		WriteString_16(160,100+30,"V",0);
	}
	if(isw == 0)
	{
		Hex_Format(DispValue.Current,4,7,0);  
		WriteString_16(80,100+60,DispBuf,0);
		WriteString_16(160,100+60,"A",0);
	}else if(isw == 1){
		Hex_Format(DispValue.Current,3,6,0);  
		WriteString_16(80,100+60,DispBuf,0);
		WriteString_16(160,100+60,"A",0);
	}
	Hex_Format(DispValue.Power,3,6,0);  
	WriteString_16(80,100+90,DispBuf,0);
	WriteString_16(160,100+90,"W",0);
	Hex_Format(DispValue.Rdata,4,7,0);  
	WriteString_16(80+200,100+30,DispBuf,0);
	WriteString_16(160+200,100+30,"Ω",0);
	Hex_Format(DispValue.Capacity,0,6,0);  
	WriteString_16(80+200,100+60,DispBuf,0);
	WriteString_16(160+200,100+60,"mAH",0);
}


//显示 下面的快捷键按键  画矩形的底层函数要移植
void Disp_Fastbutton(void)
{
	uint32_t i;
	for(i=0;i<5;i++)
	{
        Colour.black=LCD_COLOR_TEST_BUTON;
		LCD_DrawFullRect( 10+4*i+i*90, 228, 90, 43   ) ;
        Colour.black=LCD_COLOR_TEST_LINE;
        LCD_DrawFullRect( 10+94*i+1, 227, 90, 1   );
        LCD_DrawFullRect( 10+90+i*94, 227, 1, 44   );
        Colour.black=Black;
        LCD_DrawFullRect( 10+90+i*94+1, 227, 3, 44   );

	}

}//118




void Disp_Button_value1(uint32_t value)
{
	Disp_Fastbutton();
	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
        if(LoadSave.language)
        {
            WriteString_16(10+18, 271-40, "MEAS",  0);
            WriteString_16(10+18, 271-20, "DISP",  0);
            WriteString_16(10+98+18, 271-40, "BAT",  0);
            WriteString_16(10+98+18, 271-20, "TEST",  0);
            WriteString_16(10+98+94+18, 271-40, "DYNA",  0);
            WriteString_16(10+98+94+18, 271-20, "TEST",  0);
            WriteString_16(10+98+94+94+18, 271-40, "LIST",  0);
            WriteString_16(10+98+94+94+18, 271-20, "TEST",  0);
			WriteString_16(10+98+94+94+94+18, 271-40, "LIMIT",  0);
            WriteString_16(10+98+94+94+94+18, 271-20, "SET",  0);
        
        }
        else
        {
            WriteString_16(10+18, 271-40, "测量",  0);
            WriteString_16(10+18, 271-20, "显示",  0);
            WriteString_16(10+98+18, 271-40, "电池",  0);
            WriteString_16(10+98+18, 271-20, "测试",  0);
            WriteString_16(10+98+94+18, 271-40, "动态",  0);
            WriteString_16(10+98+94+18, 271-20, "测试",  0);
            WriteString_16(10+98+94+94+18, 271-40, "列表",  0);
            WriteString_16(10+98+94+94+18, 271-20, "测试",  0);
			WriteString_16(10+98+94+94+94+18, 271-40, "极限",  0);
			WriteString_16(10+98+94+94+94+18, 271-20, "设置",  0);
        }
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84+18, 271-40, "文件",  0);
		WriteString_16(84+18, 271-20, "管理",  0);
		WriteString_16(84+80+18, 271-40, "保存",  0);
		WriteString_16(84+80+18, 271-20, "数据",  0);
        WriteString_16(84+80+80+80+80+18, 271-40, "更多",  0);
		WriteString_16(84+80+80+80+80+18, 271-20, " 2/2",  0);
    
    }else if(value==2){
		if(LoadSave.language)
        {
            WriteString_16(10+18, 271-40, "MEAS",  0);
            WriteString_16(10+18, 271-20, "DISP",  0);
            WriteString_16(10+98+18, 271-40, "LIMIT",  0);
            WriteString_16(10+98+18, 271-20, "SETUP",  0);
            WriteString_16(10+98+94+18, 271-40, " SYS",  0);
            WriteString_16(10+98+94+18, 271-20, "SETUP",  0);
//            WriteString_16(10+98+94+94+94+18, 271-40, "SYS",  0);
//            WriteString_16(10+98+94+94+94+18, 271-20, "INFO",  0);
        
        }
        else
        {
						WriteString_16(10+18, 271-40, "测量",  0);
            WriteString_16(10+18, 271-20, "显示",  0);
            WriteString_16(10+98+18, 271-40, "极限",  0);
            WriteString_16(10+98+18, 271-20, "设置",  0);
            WriteString_16(10+98+94+18, 271-40, "系统",  0);
            WriteString_16(10+98+94+18, 271-20, "设置",  0);
//            WriteString_16(10+98+94+94+18, 271-40, "系统",  0);
//            WriteString_16(10+98+94+94+18, 271-20, "信息",  0);
//			WriteString_16(10+98+94+94+94+94+18, 271-40, "固件",  0);
//			WriteString_16(10+98+94+94+94+94+18, 271-20, "升级",  0);
        }
	}else if(value==3){
		if(LoadSave.language)
        {
            WriteString_16(10+18, 271-40, "MEAS",  0);
            WriteString_16(10+18, 271-20, "DISP",  0);
            WriteString_16(10+98+18, 271-40, "MEAS",  0);
            WriteString_16(10+98+18, 271-20, "SETUP",  0);
            WriteString_16(10+98+94+18, 271-40, " SYS",  0);
            WriteString_16(10+98+94+18, 271-20, "SETUP",  0);
//            WriteString_16(10+98+94+94+94+18, 271-40, "SYS",  0);
//            WriteString_16(10+98+94+94+94+18, 271-20, "INFO",  0);
        
        }
        else
        {
			WriteString_16(10+18, 271-40, "测量",  0);
            WriteString_16(10+18, 271-20, "显示",  0);
            WriteString_16(10+98+18, 271-40, "测量",  0);
            WriteString_16(10+98+18, 271-20, "设置",  0);
            WriteString_16(10+98+94+18, 271-40, "系统",  0);
            WriteString_16(10+98+94+18, 271-20, "设置",  0);
//            WriteString_16(10+98+94+94+18, 271-40, "系统",  0);
//            WriteString_16(10+98+94+94+18, 271-20, "信息",  0);
//			WriteString_16(10+98+94+94+94+94+18, 271-40, "固件",  0);
//			WriteString_16(10+98+94+94+94+94+18, 271-20, "升级",  0);
        }
	}

}
void Disp_Button_TestSet(uint32_t value)
{
//	Disp_Fastbutton();
 	if(value==0)
	{
		Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84+18, 271-40, "测量",  0);
		WriteString_16(84+18, 271-20, "显示",  0);
		WriteString_16(84+80+18, 271-40, "测量",  0);
		WriteString_16(84+80+18, 271-20, "设置",  0);
		WriteString_16(84+80+80+18, 271-40, "系统",  0);
		WriteString_16(84+80+80+18, 271-20, "设置",  0);
		WriteString_16(84+80+80+80+18, 271-40, "系统",  0);
		WriteString_16(84+80+80+80+18, 271-20, "信息",  0);
//		WriteString_16(84+80+80+80+80, 271-40, "更多",  0);
//		WriteString_16(84+80+80+80+80, 271-20, " 1/2",  0);
	}
    else if(value==1)
    {
        Colour.Fword=White;
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(84+18, 271-40, "文件",  0);
		WriteString_16(84+18, 271-20, "管理",  0);
		WriteString_16(84+8+180, 271-40, "系统",  0);
		WriteString_16(84+80+18, 271-20, "设置",  0);
        WriteString_16(84+80+80+18, 271-30, "工具",  0);
//		WriteString_16(84+80+80, 271-20, "设置",  0);
        WriteString_16(84+80+80+80+80+18, 271-40, "更多",  0);
		WriteString_16(84+80+80+80+80+18, 271-20, " 2/2",  0);
    
    
    }


}
void Disp_Button_SysSet(void)
{

		Disp_Button_value1(3);


}
void Disp_TopBar_Color(void)
{
    Colour.black=LCD_COLOR_TEST_BAR;
    LCD_DrawFullRect(0, 0, 170, 22);
	//LCD_DrawFullRect( 0, 0, 240, 22 );


}
void Disp_MidRect(void)
{
    Colour.black=LCD_COLOR_TEST_MID;
	LCD_DrawFullRect( 0, 92-16,479 , 202-92+40 ) ;
}

void Disp_MidRect1(void)
{
    Colour.black=LCD_COLOR_TEST_MID;
	LCD_DrawFullRect( 0, 92+18,479 , 202-92+40 ) ;
}

void Disp_MidRectBat(void)
{
    Colour.black=LCD_COLOR_TEST_MID;
	LCD_DrawFullRect( 0, 92+18,479 , 202-92+40-34 ) ;
}
void Disp_TestScreen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键
	Disp_MidRect();//显示中间色块
}

void Disp_BatScreen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键
	Disp_MidRectBat();//显示中间色块
}

void Disp_ListScreen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键
//	Disp_MidRect();//显示中间色块
}

//
//测量显示主菜单的项目显示
//
void Disp_Test_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Test_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_TestScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[0],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
    if(LoadSave.language)
        pt=Test_Setitem_E;
    else
        pt=Test_Setitem;
	for(i=0;i<1;i++)
	{
		if(i<2)
			WriteString_16(0, 26+i*22, pt[i],  0);
		else
			WriteString_16(250, 26+(i-3)*22, pt[i],  0);

	
	}
	Colour.Fword=LCD_COLOR_GREY;
	Colour.black=LCD_COLOR_TEST_MID;
	if(LoadSave.limitdisp == 1)
	{
		WriteString_16(LIST2+88-28,80,"VH:",0);
		WriteString_16(LIST2+88-28,80+20,"VL:",0);
		
		WriteString_16(LIST2+88-28,80+40,"CH:",0);
		WriteString_16(LIST2+88-28,80+60,"CL:",0);
		
		WriteString_16(LIST2+88-28,80+80,"PH:",0);
		WriteString_16(LIST2+88-28,80+100,"PL:",0);
	}
//	Disp_Button_page1(0);
	

}

//测量显示主菜单的项目显示
//
void Disp_Led_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Test_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_TestScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[1],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
    pt=Led_Setitem;
	for(i=0;i<3;i++)
	{
		if(i<2)
			WriteString_16(0, 26+i*22, pt[i],  0);
		else
			WriteString_16(250, 26+(i-2)*22, pt[i],  0);

	
	}
	
	Disp_Button_value1(0);
	

}


void Disp_Battery_Item(u8 type)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Bat_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_BatScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[2],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	if(LoadSave.loadmode == 0)
	{
		for(i=0;i<8;i++)
		{
			if(i<4)
				WriteString_16(0, 26+i*22, Bat_Setitem[LoadSave.language][i],  0);
			else
				WriteString_16(250, 26+(i-4)*22, Bat_Setitem[LoadSave.language][i],  0);
		
		}
	}else if(LoadSave.loadmode == 2){
		for(i=0;i<8;i++)
		{
			if(i<4)
				WriteString_16(0, 26+i*22, Bat_SetitemR[LoadSave.language][i],  0);
			else
				WriteString_16(250, 26+(i-4)*22, Bat_SetitemR[LoadSave.language][i],  0);
		
		}
	}
	if(type == 0)
	{
		Disp_Button_value1(0);
	}
	

}



//列表测试主菜单的项目显示
void Disp_List_Item(void)
{
	uint32_t i;
	const u8 (*pt)[sizeof(List_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
	
	Disp_ListScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[5],  0);
	
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	for(i=0;i<5;i++)
	{
		if(i<3)
		{
			if(LoadSave.language == 1)
			{
				WriteString_16(0, 26+i*22, List_Setitem_E[i],  0);
				
			}else if(LoadSave.language == 0){
				WriteString_16(0, 26+i*22, List_Setitem[i],  0);
			}
		}else{
			if(LoadSave.language == 1)
			{
				WriteString_16(250,26+(i-3)*22, List_Setitem_E[i],  0);
				
			}else if(LoadSave.language == 0){
				WriteString_16(250,26+(i-3)*22, List_Setitem[i],  0);
			}
			
		}

	}
	//	LCD_DrawRect(0,26+2*22,479,16,LCD_COLOR_TEST_BACK);
	Disp_Button_value1(0);

}

void Disp_Dynamic_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Dyn_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_BatScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[3],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	if(LoadSave.language==0)
    pt=Dyn_Setitem;
	else
		pt=Dyn_SetitemE;
	for(i=0;i<8;i++)
	{
		if(i<4)
			WriteString_16(0, 26+i*22, pt[i],  0);
		else
			WriteString_16(250, 26+(i-4)*22, pt[i],  0);

	
	}
	
	Disp_Button_value1(0);
	

}


//测试的时候显示的设置值
void Disp_Test_value(u8 num)
{
//	vu32 flag;
	vu32 i;
    const u8 (*ppt)[11];
    const u8 (*pt)[7];
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
	if(LoadSave.language)
	{
		pt=Test_Modevalue_E1;
	}
	else
	{
		pt=Test_Modevalue1;
	
	}
	if(LoadSave.mode <4)
	{
		Colour.black=LCD_COLOR_TEST_BACK;
		WriteString_16(250, 26+(3-3)*22, Mode_Setitem[LoadSave.language][LoadSave.mode],0);
	}
	

	Black_Select=(num==1)?1:0;//模式
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+88, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	WriteString_16(LIST1+88, FIRSTLINE, pt[LoadSave.mode],  0);//增加算法  把顺序改过来
	
	
	Black_Select=(num==2)?1:0;//参数
	
	
	
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
//	if(LoadSave.mode == 3)
//	{
		LCD_DrawFullRect( LIST2+88-2-10, FIRSTLINE-2,SELECT_2END-LIST2-86+4+10+10, SPACE1-2  ) ;//SPACE1
//	}else{
//		LCD_DrawFullRect( LIST2+88-2, FIRSTLINE-2,SELECT_2END-LIST2-86+4+10, SPACE1-2  ) ;//SPACE1
//	}
	if(Black_Select && coder_flag == 1)
	{
		Colour.black=LCD_COLOR_YELLOW;
		if(LoadSave.mode == 2)//CR
		{
			if(spinbit < 2)
			{
				LCD_DrawFullRect(LIST2+88+8*10-spinbit*10, FIRSTLINE+16,8, 2);
			}else{
				LCD_DrawFullRect(LIST2+88+7*10-spinbit*10, FIRSTLINE+16,8, 2);
			}
		}else{
			if(spinbit < 5)
			{
				LCD_DrawFullRect(LIST2+88+8*10-spinbit*10, FIRSTLINE+16,8, 2);
			}else{
				LCD_DrawFullRect(LIST2+88+7*10-spinbit*10, FIRSTLINE+16,8, 2);
			}
		}
		Colour.black=LCD_COLOR_SELECT;	
	}		
	
    Colour.Fword=White;
	if(LoadSave.mode == 0)//CC
	{
		Hex_Format(LoadSave.current,4,7,0);
	}else if(LoadSave.mode == 1){//CV
		Hex_Format(LoadSave.voltage,4,7,0);
	}else if(LoadSave.mode == 2){//CR
		Hex_Format(LoadSave.risistence,1,7,0);
	}else if(LoadSave.mode == 3){//CP
		if(LoadSave.Version == 0 || LoadSave.Version == 4 || LoadSave.Version == 5)
		{
			Hex_Format(LoadSave.power,4,8,0);
		}else{
			Hex_Format(LoadSave.power,4,7,0);
		}
	}
	if(LoadSave.mode == 3){//CP
		if(LoadSave.Version == 0 || LoadSave.Version == 4 || LoadSave.Version == 5)
		{
			WriteString_16(LIST2+88-10, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
		}else{
			WriteString_16(LIST2+88, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
		}
		
	}else{
		WriteString_16(LIST2+88, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
	}
	WriteString_16(LIST2+90+80+10, FIRSTLINE, MODE_UINT[LoadSave.mode],  0);
	if(LoadSave.mode == 3){//CP
		if(DispBuf[0] == 0x20 && DispBuf[1] == 0x20 && DispBuf[2] == 0x20)
		{
			spinbitmax = 5;
		}else if(DispBuf[0] == 0x20 && DispBuf[1] == 0x20 && DispBuf[2] != 0x20){
			spinbitmax = 6;
		}else if(DispBuf[0] == 0x20 && DispBuf[1] != 0x20 && DispBuf[2] != 0x20){
			spinbitmax = 7;
		}else if(DispBuf[0] != 0x20 && DispBuf[1] != 0x20 && DispBuf[2] != 0x20){
			spinbitmax = 8;
		}
	}else{
		if(DispBuf[0] == 0x20 && DispBuf[1] == 0x20)
		{
			spinbitmax = 5;
		}else if(DispBuf[0] == 0x20 && DispBuf[1] != 0x20){
			spinbitmax = 6;
		}else if(DispBuf[0] != 0x20 && DispBuf[1] != 0x20){
			spinbitmax = 7;
		}
	}
	if(num == 2)
	{
		Disp_Hint(1);
	}else if(num == 1){
		Disp_Hint(6);
	}else{
		Disp_Hint(3);
	}
//	switch(num)
//	{
//		case 0:
//				Disp_Button_value1(0);
//			break;
//		case 1:
	if(DispValue.alertdisp == 0)
	{
		Disp_Fastbutton();
		Colour.Fword=White;//
		Colour.black=LCD_COLOR_TEST_BUTON;
		if(LoadSave.language)
		{
			pt=Test_Modevalue_E1;
		}
		else
		{
			pt=Test_Modevalue1;
		
		}
		if(buttonpage1 == 1)
		{
			
			for(i=0;i<4;i++)
			{
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[7],  0);
		}else if(buttonpage1 == 2){
			WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE, pt[6],  0);
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[7],  0);
			if(LoadSave.language == 0)
			{
				WriteString_16(10+98+18, 271-40, "电池",  0);
				WriteString_16(10+98+18, 271-20, "测试",  0);
				WriteString_16(10+98+94+18, 271-40, "动态",  0);
				WriteString_16(10+98+94+18, 271-20, "测试",  0);
				WriteString_16(10+98+94+94+18, 271-40, "列表",  0);
				WriteString_16(10+98+94+94+18, 271-20, "测试",  0);
			}else{
				WriteString_16(10+98+18, 271-40, "BAT",  0);
				WriteString_16(10+98+18, 271-20, "TEST",  0);
				WriteString_16(10+98+94+18, 271-40, "DYNA",  0);
				WriteString_16(10+98+94+18, 271-20, "TEST",  0);
				WriteString_16(10+98+94+94+18, 271-40, "LIST",  0);
				WriteString_16(10+98+94+94+18, 271-20, "TEST",  0);
			}
			
		}
	}else{
		Disp_Fastbutton();
		Colour.Fword=White;//
		Colour.black=LCD_COLOR_TEST_BUTON;
		if(LoadSave.language == 0)
		{
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
		}else{
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "OK",  0);
		}
	}
//		break;
//		case 2:
//			Disp_Fastbutton();


//		default:
//			
//		break;	
//	}
}


//测试的时候显示的设置值
void Disp_Led_value(u8 num)
{
//	vu32 flag;
	vu32 i;
    const u8 (*ppt)[11];
    const u8 (*pt)[7];
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
	

	Black_Select=(num==1)?1:0;//LED Vo
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawFullRect( LIST1+88, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.ledvo,3,5,0);
	WriteString_16(LIST1+88, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
	
	Black_Select=(num==2)?1:0;//参数
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST2+88, FIRSTLINE-2,SELECT_2END-LIST2-88 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.ledrd,3,5,0);
	WriteString_16(LIST2+88, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
	
	Black_Select=(num==3)?1:0;//参数
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+88, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.ledio,3,5,0);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来

	
	switch(num)
	{
		case 0:
				Disp_Button_value1(0);
			break;
		case 1:
				Disp_Fastbutton();
				Colour.Fword=White;//
				Colour.black=LCD_COLOR_TEST_BUTON;
				
				if(LoadSave.language)
                {
                    pt=Test_Modevalue_E1;
                }
                else
                {
                    pt=Test_Modevalue1;
                
                }
				for(i=0;i<5;i++)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
		
		break;
		case 2:
			Disp_Fastbutton();
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
            if(LoadSave.language)
            {
                pt=INPUT_E;
            }
            else
            {
                pt=INPUT;
            
            }
			for(i=0;i<1;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
		break;

		default:
			
		break;	
	}
}

//测试的时候显示的设置值
void Disp_Bat_value(u8 num)
{
//	vu32 flag;
	vu32 i;
    const u8 (*ppt)[11];
    const u8 (*pt)[7];
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
	
	if(LoadSave.loadmode == 0)
	{
		Black_Select=(num==1)?1:0;//放电模式
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}	
		LCD_DrawFullRect( LIST1+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		if(LoadSave.language == 0)
			WriteString_16(LIST1+118, FIRSTLINE, Bat_LoadMod[LoadSave.loadmode],  0);
		else
			WriteString_16(LIST1+118, FIRSTLINE, Bat_ModevalueE[LoadSave.loadmode],  0);
		
		
		Black_Select=(num==2)?1:0;//放电电流1
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.loadc1,4,7,0);
		WriteString_16(LIST1+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1, Unit_Setitem[0],  0);
		
		Black_Select=(num==3)?1:0;//放电电流2
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1*2-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.loadc2,4,7,0);
		WriteString_16(LIST1+118, FIRSTLINE+SPACE1*2, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1*2, Unit_Setitem[0],  0);

		Black_Select=(num==4)?1:0;//放电电流3
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1*3-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.loadc3,4,7,0);
		WriteString_16(LIST1+118, FIRSTLINE+SPACE1*3, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1*3, Unit_Setitem[0],  0);
		
//		Black_Select=(num==5)?1:0;//曲线采样间隔
//		if(Black_Select)
//		{
//			Colour.black=LCD_COLOR_SELECT;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_TEST_BACK;
//		}	
//		LCD_DrawFullRect( LIST2+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
//		Colour.Fword=White;
//		Hex_Format(LoadSave.curvetime,0,2,0);
//		WriteString_16(LIST2+118, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
//		WriteString_16(LIST2+118+60, FIRSTLINE, Unit_Setitem[4],  0);
		
		Black_Select=(num==6)?1:0;//截止电压1
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.coffv1,4,7,0);
		WriteString_16(LIST2+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST2+118+82, FIRSTLINE+SPACE1, Unit_Setitem[1],  0);
		
		Black_Select=(num==7)?1:0;//截止电压2
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1*2-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.coffv2,4,7,0);
		WriteString_16(LIST2+118, FIRSTLINE+SPACE1*2, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST2+118+82, FIRSTLINE+SPACE1*2, Unit_Setitem[1],  0);

		Black_Select=(num==8)?1:0;//截止电压3
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1*3-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.coffv3,4,7,0);
		WriteString_16(LIST2+118, FIRSTLINE+SPACE1*3, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST2+118+82, FIRSTLINE+SPACE1*3, Unit_Setitem[1],  0);
		
		if(num == 1)
		{
			Disp_Hint(6);
		}else if(num == 0){
			Disp_Hint(3);
		}else{
			Disp_Hint(8);
		}
	}else if(LoadSave.loadmode == 2){
		Black_Select=(num==1)?1:0;//放电模式
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}	
		LCD_DrawFullRect( LIST1+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		if(LoadSave.language == 0)
			WriteString_16(LIST1+118, FIRSTLINE, Bat_LoadMod[LoadSave.loadmode],  0);
		else
			WriteString_16(LIST1+118, FIRSTLINE, Bat_ModevalueE[LoadSave.loadmode],  0);
		
		
		Black_Select=(num==2)?1:0;//放电电阻
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1-2,88+16 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.loadr,1,7,0);
		WriteString_16(LIST1+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1, Unit_Setitem[2],  0);
		
//		Black_Select=(num==5)?1:0;//曲线采样间隔
//		if(Black_Select)
//		{
//			Colour.black=LCD_COLOR_SELECT;
//		
//		}
//		else
//		{
//			Colour.black=LCD_COLOR_TEST_BACK;
//		}	
//		LCD_DrawFullRect( LIST2+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
//		Colour.Fword=White;
//		Hex_Format(LoadSave.curvetime,0,2,0);
//		WriteString_16(LIST2+118, FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
//		WriteString_16(LIST2+118+60, FIRSTLINE, Unit_Setitem[4],  0);
		
		Black_Select=(num==6)?1:0;//截止电压
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
		Colour.Fword=White;
		Hex_Format(LoadSave.coffvr,4,7,0);
		WriteString_16(LIST2+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
		WriteString_16(LIST2+118+82, FIRSTLINE+SPACE1, Unit_Setitem[1],  0);
		
		if(num == 1)
		{
			Disp_Hint(6);
		}else if(num == 0){
			Disp_Hint(3);
		}else{
			Disp_Hint(8);
		}
	}
	if(DispValue.alertdisp == 0)
	{
		switch(num)
		{
			case 0:
					Disp_Button_value1(0);
				break;
			case 1:
					Disp_Fastbutton();
					Colour.Fword=White;//
					Colour.black=LCD_COLOR_TEST_BUTON;
					
					if(LoadSave.language)
					{
						pt=Bat_ModevalueE;
					}
					else
					{
						pt=Bat_Modevalue;
					
					}
					for(i=0;i<2;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			
			break;
			case 2:
			case 3:
			case 4:
			case 6:
			case 7:
			case 8:
				Disp_Fastbutton();
	//			Colour.Fword=White;
	//			Colour.black=LCD_COLOR_TEST_BUTON;
	//            if(LoadSave.language)
	//            {
	//                pt=INPUT_E;
	//            }
	//            else
	//            {
	//                pt=INPUT;
	//            
	//            }
	//			for(i=0;i<1;i++)
	//			{
	//				
	//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
	//			}
			break;

			default:
				
			break;	
		}
	}else{
		Disp_Fastbutton();
		Colour.Fword=White;//
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
	}
}


void Disp_Dyn_value(u8 num)
{
//	vu32 flag;
	vu32 i;
    const u8 (*ppt)[11];
    const u8 (*pt)[7];
//	vu32 xpose;
	vu32 Black_Select;
//	vu32 Select_color;
	

	Black_Select=(num==1)?1:0;//测量显示
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawFullRect( LIST1+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	if(LoadSave.language == 0)
		WriteString_16(LIST1+118, FIRSTLINE, Test_Compvalue[LoadSave.testdis],  0);//增加算法  把顺序改过来
	else
		WriteString_16(LIST1+118, FIRSTLINE, Test_Compvalue_E[LoadSave.testdis],  0);
		
	 
	
	Black_Select=(num==2)?1:0;//动态模式拉载电流A
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.valA,4,7,0);
	WriteString_16(LIST1+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1, Unit_Setitem[0],  0);
	
	Black_Select=(num==3)?1:0;///动态模式拉载电流B
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1*2-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.valB,4,7,0);
	WriteString_16(LIST1+118, FIRSTLINE+SPACE1*2, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1*2, Unit_Setitem[0],  0);

	Black_Select=(num==4)?1:0;//边沿AB
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1*3-2,88+4+8 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.dynaIRise,4,5,0);
	WriteString_16(LIST1+118, FIRSTLINE+SPACE1*3, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST1+118+60, FIRSTLINE+SPACE1*3, Unit_Setitem[5],  0);
	
	Black_Select=(num==5)?1:0;//动态模式
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawFullRect( LIST2+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
//	Hex_Format(LoadSave.dynamode,0,2,0);
	if(LoadSave.language == 0)
		WriteString_16(LIST2+118, FIRSTLINE, Tran_Mode[LoadSave.dynamode],  0);//增加算法  把顺序改过来
	else
		WriteString_16(LIST2+118, FIRSTLINE, Tran_ModeE[LoadSave.dynamode],  0);
	
	Black_Select=(num==6)?1:0;//脉宽A
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1-2,88+4+20 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.timeA,1,8,0);
	WriteString_16(LIST2+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST2+118+60+30, FIRSTLINE+SPACE1, Unit_Setitem[7],  0);
	
	Black_Select=(num==7)?1:0;//脉宽B
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1*2-2,88+4+20 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.timeB,1,8,0);
	WriteString_16(LIST2+118, FIRSTLINE+SPACE1*2, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST2+118+60+30, FIRSTLINE+SPACE1*2, Unit_Setitem[7],  0);

	Black_Select=(num==8)?1:0;//边沿BA
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1*3-2,88+4+8 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.dynaIDrop,4,5,0);
	WriteString_16(LIST2+118, FIRSTLINE+SPACE1*3, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST2+118+60, FIRSTLINE+SPACE1*3, Unit_Setitem[5],  0);
	
	if(DispValue.alertdisp == 0)
	{
		switch(num)
		{
			case 0:
					Disp_Button_value1(0);
				break;
			case 1:
					Disp_Fastbutton();
					Colour.Fword=White;//
					Colour.black=LCD_COLOR_TEST_BUTON;
					
					if(LoadSave.language)
					{
						pt=Test_Compvalue_E;
					}
					else
					{
						pt=Test_Compvalue;
					
					}
					for(i=0;i<2;i++)
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			
			break;
			case 5:
				Disp_Fastbutton();
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
				if(LoadSave.language)
				{
					pt=Tran_ModeE;
				}
				else
				{
					pt=Tran_Mode;
				
				}
				for(i=0;i<3;i++)
				{
					
					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
				}
			break;

			default:
				Disp_Fastbutton();
			break;	
		}
	}else{
		Disp_Fastbutton();
		Colour.Fword=White;//
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
	}
}


//列表结果显示
void Disp_Res_Sheet(u8 num,u8 page)
{
	const u8 (*ppt)[11];
    const u8 (*pt)[7];
	u8 dispnum;
	u8 i;
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	
	
	if(DispValue.respage == 0)
	{
		if(LoadSave.ListNum <= 5)
		{
			dispnum = LoadSave.ListNum;
		}else{
			dispnum = 5;
		}
	}else if(DispValue.respage == 1){
		if(LoadSave.ListNum <= 10)
		{
			dispnum = LoadSave.ListNum-5;
		}else{
			dispnum = 5;
		}
	}else if(DispValue.respage == 2){
		dispnum = LoadSave.ListNum-10;
	}
	
	LCD_DrawFullRect(0,94,479,202);
	LCD_SetColors(White,LCD_COLOR_TEST_BACK);
	Colour.black=White;
	LCD_DrawFullRect( 0, 94, 1, 202-(5-dispnum-1)*22-93);
	LCD_DrawFullRect( 40, 94, 1, 202-(5-dispnum-1)*22-93 );
	LCD_DrawFullRect( 40+40+40, 94, 1, 202-(5-dispnum-1)*22-93);
	for(i=1;i<4;i++)
	{
		LCD_DrawFullRect( 40+40+40+100*i, 94, 1, 202-(5-dispnum-1)*22-93);
	}
	
	for(i=0;i<dispnum+2;i++)
	{
		LCD_DrawFullRect( 0, 94+22*i, 479,1 );
	}

	Colour.black=LCD_COLOR_TEST_BACK;
	if(LoadSave.language == 1)
	{
		WriteString_16(3,97,  "NO",  0);
		WriteString_16(3+60,97,  "MODE",  0);
		WriteString_16(3+30+80+40,97,  "V",  0);
		WriteString_16(3+30+80+40+100,97,  "I",  0);
		WriteString_16(3+30+80+40+100+100,97,  "P",  0);
		WriteString_16(3+10+80+40+100+100+100,97,  "RES",  0);
	}else if(LoadSave.language == 0){
		WriteString_16(3,97,  "序号",  0);
		WriteString_16(3+60,97,  "项目",  0);
		WriteString_16(3+30+80+40,97,  "电压",  0);
		WriteString_16(3+30+80+40+100,97,  "电流",  0);
		WriteString_16(3+30+80+40+100+100,97,  "功率",  0);
		WriteString_16(3+10+80+40+100+100+100,97,  "结果",  0);
	}
	
	Disp_Fastbutton();
	Colour.Fword=White;//
	Colour.black=LCD_COLOR_TEST_BUTON;
	
	if(LoadSave.language)
	{
		pt=List_ResBackE;
	}
	else
	{
		pt=List_ResBack;
	
	}
	for(i=0;i<5;i++)
	WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
	
	
	for(i=0;i<dispnum;i++)
	{
		Colour.black=LCD_COLOR_TEST_BACK;
//		LCD_DrawRect(3, 95+22*i,35 ,95+22*i+32 , Colour.black ) ;
		Hex_Format(i+DispValue.respage*5+1,0,2,0);
		WriteString_16(3,97+22+22*i, DispBuf, 0);//序号
		
		WriteString_16(3+50,97+22+22*i, List_ItemDis[LoadSave.listmode[i+DispValue.respage*5]], 0);//项目
		
		Colour.Fword=compVcolor[DispValue.listcompres[i+DispValue.respage*5]];
		if(DispValue.listvrange[i+DispValue.respage*5] == 0)//电压
		{
			Hex_Format(DispValue.listv[i+DispValue.respage*5],4,7,0);
			WriteString_16(3+80+40,97+22+22*i,DispBuf, 0);
		}else if(DispValue.listvrange[i+DispValue.respage*5] == 1){
			Hex_Format(DispValue.listv[i+DispValue.respage*5],3,6,0);
			WriteString_16(3+80+40,97+22+22*i,DispBuf, 0);
		}
		
		Colour.Fword=compCcolor[DispValue.listcompres[i+DispValue.respage*5]];
		if(DispValue.listcrange[i+DispValue.respage*5] == 0)//电流
		{
			Hex_Format(DispValue.listc[i+DispValue.respage*5],4,7,0);
			WriteString_16(3+80+40+100,97+22+22*i,DispBuf, 0);
		}else if(DispValue.listcrange[i+DispValue.respage*5] == 1){
			Hex_Format(DispValue.listc[i+DispValue.respage*5],3,6,0);
			WriteString_16(3+80+40+100,97+22+22*i,DispBuf, 0);
		}
		
		Colour.Fword=compPcolor[DispValue.listcompres[i+DispValue.respage*5]];
		Hex_Format(DispValue.listp[i+DispValue.respage*5],3,7,0);//功率
		WriteString_16(3+80+40+100+100,97+22+22*i,DispBuf, 0);
		
		Colour.Fword = White;
		if(LoadSave.language == 0)
			WriteString_16(3+80+40+100+100+100,97+22+22*i, List_Res[DispValue.listcompres[i+DispValue.respage*5]], 0);//结果
		else
			WriteString_16(3+80+40+100+100+100,97+22+22*i, List_ResE[DispValue.listcompres[i+DispValue.respage*5]], 0);//结果
		
	}
}

//列表流程显示
void Disp_List_Process(void)
{
	Colour.Fword=LCD_COLOR_YELLOW;
	Colour.black=LCD_COLOR_TEST_BACK;
//	LCD_DrawRect(300,26+3*22,250,26+8*22);
	if(LoadSave.language == 0)
	{
		WriteString_16(300,94,"第",0);
		WriteString_16(50+300,94,"步",0);
		Hex_Format(DispValue.listrunstep+1, 0 , 2 , 0);//显示步数
		WriteString_16(22+300,94 ,DispBuf,0);
	}else if(LoadSave.language == 1){
		WriteString_16(300,94,"Step",0);
		Hex_Format(DispValue.listrunstep+1, 0 , 2 , 0);//显示步数
		WriteString_16(22+16+300,94 ,DispBuf,0);
	}
	if(LoadSave.language == 0)
	{
		WriteString_16(400,94,List_ItemDis[LoadSave.listmode[DispValue.listrunstep]],0);	
	}else if(LoadSave.language == 1){
		WriteString_16(400,94,List_ItemDisE[LoadSave.listmode[DispValue.listrunstep]],0);	
	}
	if(LoadSave.language == 0)
	{
		WriteString_16(300,26+4*22,"电压",0);
		WriteString_16(300,26+5*22,"电流",0);
		WriteString_16(300,26+6*22,"功率",0);
		WriteString_16(300,26+7*22,"时间",0);
	}else if(LoadSave.language == 1){
		WriteString_16(300,26+4*22,"V",0);
		WriteString_16(300,26+5*22,"I",0);
		WriteString_16(300,26+6*22,"P",0);
		WriteString_16(300,26+7*22,"TIME",0);
	}
	
	if(LoadSave.vrange == 0)
	{
		if(DispValue.Voltage<100)
		{
			Hex_Format(0,4,7,0);  
		}else{
			Hex_Format(DispValue.Voltage,4,7,0);    
		}
		Hex_Format(DispValue.Voltage,4,7,0);       
		WriteString_16(360,26+4*22,DispBuf,0);
		WriteString_16(360+80,26+4*22,"V",0);
	}else if(LoadSave.vrange == 1){
		if(DispValue.Voltage<100)
		{
			Hex_Format(0,3,6,0);  
		}else{
			Hex_Format(DispValue.Voltage,3,6,0);    
		}
//		Hex_Format(DispValue.Voltage,3,6,0);       
		WriteString_16(360,26+4*22,DispBuf,0);
		WriteString_16(360+80,26+4*22,"V",0);
	}
	if(LoadSave.crange == 0)
	{
		Hex_Format(DispValue.Current,4,7,0);  
		WriteString_16(360,26+5*22,DispBuf,0);
		WriteString_16(360+80,26+5*22,"A",0);
	}else if(LoadSave.crange == 1){
		Hex_Format(DispValue.Current,3,6,0);  
		WriteString_16(360,26+5*22,DispBuf,0);
		WriteString_16(360+80,26+5*22,"A",0);
	}	
	Hex_Format(DispValue.Power,3,7,0);  
	WriteString_16(360,26+6*22,DispBuf,0);
	WriteString_16(360+80,26+6*22,"W",0); 
	
	Hex_Format(DispValue.listdelay,1,4,0);  
	WriteString_16(360,26+7*22,DispBuf,0);
	WriteString_16(360+80,26+7*22,"s",0);
}
//列表模式设置值
void Disp_List_value(u8 num)
{
//	uint32_t flag;
	uint32_t i;
//	uint32_t xpose;
	uint32_t Black_Select;
	const u8 (*ppt)[11];
    const u8 (*pt)[7];
	
//	uint32_t Select_color;
	Black_Select=(num==1)?1:0;//列表步数
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.ListNum,0,3,0);	
	WriteString_16(LIST1+118,FIRSTLINE, DispBuf,  0);//增加算法  把顺序改过来
	
	
	Black_Select=(num==2)?1:0;//步进模式
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	if(LoadSave.language == 0)
		WriteString_16(LIST1+118,FIRSTLINE+SPACE1, Step_Mode[LoadSave.StepMode],  0);
	else
		WriteString_16(LIST1+118,FIRSTLINE+SPACE1, Step_ModeE[LoadSave.StepMode],  0);
	
	Black_Select=(num==3)?1:0;///门槛电压
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+118, FIRSTLINE+SPACE1*2-2,88+4 , SPACE1-2  ) ;//SPACE1
	Hex_Format(LoadSave.gatev,4,7,0);
  Colour.Fword=White;
	WriteString_16(LIST1+118, FIRSTLINE+SPACE1*2, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST1+118+82, FIRSTLINE+SPACE1*2, Unit_Setitem[1],  0);

	
	Black_Select=(num==4)?1:0;//列表讯响
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}	
	LCD_DrawFullRect( LIST2+118, FIRSTLINE-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	if(LoadSave.language == 0)
		WriteString_16(LIST2+118, FIRSTLINE, List_Beep[LoadSave.ListBeep],  0);//增加算法  把顺序改过来
	else
		WriteString_16(LIST2+118, FIRSTLINE, List_BeepE[LoadSave.ListBeep],  0);
	
	Black_Select=(num==5)?1:0;//启动电压
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST2+118, FIRSTLINE+SPACE1-2,88+4 , SPACE1-2  ) ;//SPACE1
    Colour.Fword=White;
	Hex_Format(LoadSave.listonvol,4,7,0);
	WriteString_16(LIST2+118, FIRSTLINE+SPACE1, DispBuf,  0);//增加算法  把顺序改过来
	WriteString_16(LIST2+118+82, FIRSTLINE+SPACE1, Unit_Setitem[1],  0);
	
	//步骤和项目
	Black_Select=(num==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect(0,94,80,16);
	Colour.Fword=White;
	if(LoadSave.language == 0)
	{
		WriteString_16(0,94,"第",0);
		WriteString_16(50,94,"步",0);
		Hex_Format(DispValue.liststep+1, 0 , 2 , 0);//显示步数
		WriteString_16(22,94 ,DispBuf,0);
	}else if(LoadSave.language == 1){
		WriteString_16(0,94,"Step",0);
		Hex_Format(DispValue.liststep+1, 0 , 2 , 0);//显示步数
		WriteString_16(22+16,94 ,DispBuf,0);
	}
	
	
	
	
	Black_Select=(num==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect(100,94,80,16);
	if(LoadSave.language == 0)
	{
		WriteString_16(100,94,List_ItemDis[LoadSave.listmode[DispValue.liststep]],0);	
	}else if(LoadSave.language == 1){
		WriteString_16(100,94,List_ItemDisE[LoadSave.listmode[DispValue.liststep]],0);	
	}
	Colour.black=LCD_COLOR_TEST_BACK;
	if(LoadSave.listmode[DispValue.liststep] != 4)//定电流定电圧定功率定电阻
	{
		if(LoadSave.language == 0)
		{
			WriteString_16(0,26+4*22,"负载大小",0);
			WriteString_16(0,26+5*22,"时间",0);
			WriteString_16(0,26+6*22,"比较类型",0);
			WriteString_16(0,26+7*22,"上限",0);
			WriteString_16(0,26+8*22,"下限",0);
		}else if(LoadSave.language == 1){
			WriteString_16(0,26+4*22,"LOAD",0);
			WriteString_16(0,26+5*22,"Time",0);
			WriteString_16(0,26+6*22,"Comp",0);
			WriteString_16(0,26+7*22,"High",0);
			WriteString_16(0,26+8*22,"Low",0);
		}
		LCD_DrawFullRect(200,26+4*22-2,20,18);
		LCD_DrawFullRect(200,26+7*22-2,20,18);
		LCD_DrawFullRect(200,26+8*22-2,20,18);
		WriteString_16(200,26+4*22,Unit_Setitem[LoadSave.listmode[DispValue.liststep]],0);
		WriteString_16(200,26+7*22,Unit_Compitem[LoadSave.listcomp[DispValue.liststep]],0);
		WriteString_16(200,26+8*22,Unit_Compitem[LoadSave.listcomp[DispValue.liststep]],0);
		WriteString_16(200,26+5*22,"s",0);
		Black_Select=(num==8)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+4*22-2,92,20);
		Colour.Fword=White;
		if(LoadSave.listmode[DispValue.liststep] == 2)
		{
			Hex_Format(LoadSave.listvalue[DispValue.liststep],1,7,0);
		}else{
			Hex_Format(LoadSave.listvalue[DispValue.liststep],4,8,0);
		}
		WriteString_16(100,26+4*22,DispBuf,0);
		
		
		Black_Select=(num==9)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+5*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.delay[DispValue.liststep],1,4,0);
		WriteString_16(100,26+5*22,DispBuf,0);
		
		Black_Select=(num==10)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+6*22-2,92,20);
		Colour.Fword=White;
		if(LoadSave.language==0)
			WriteString_16(100,26+6*22,List_CompType[LoadSave.listcomp[DispValue.liststep]],0);
		else
			WriteString_16(100,26+6*22,List_CompTypeE[LoadSave.listcomp[DispValue.liststep]],0);
		
		Black_Select=(num==11)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+7*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.listhigh[DispValue.liststep],4,7,0);
		WriteString_16(100,26+7*22,DispBuf,0);
		
		
		Black_Select=(num==12)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+8*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.listlow[DispValue.liststep],4,7,0);
		WriteString_16(100,26+8*22,DispBuf,0);
		
		
		
	}else if(LoadSave.listmode[DispValue.liststep] == 4)//短路
	{
		if(LoadSave.language == 0)
		{
			WriteString_16(0,26+4*22,"负载大小",0);
			WriteString_16(0,26+5*22,"时间",0);
			WriteString_16(0,26+6*22,"比较类型",0);
			WriteString_16(0,26+7*22,"上限",0);
			WriteString_16(0,26+8*22,"下限",0);
		}else if(LoadSave.language == 1){
			WriteString_16(0,26+4*22,"LOAD",0);
			WriteString_16(0,26+5*22,"Time",0);
			WriteString_16(0,26+6*22,"Comp",0);
			WriteString_16(0,26+7*22,"High",0);
			WriteString_16(0,26+8*22,"Low",0);
		}
//		WriteString_16(173,26+4*22,"A",0);
		LCD_DrawFullRect(200,26+4*22-2,20,18);
		LCD_DrawFullRect(200,26+7*22-2,20,18);
		LCD_DrawFullRect(200,26+8*22-2,20,18);
		WriteString_16(200,26+5*22,"s",0);
		Black_Select=(num==7)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		
		Hex_Format(LoadSave.listvalue[DispValue.liststep],4,7,0);
		LCD_DrawFullRect(100,26+4*22-2,92,20);
		WriteString_16(100,26+4*22,DispBuf,0);
		Black_Select=(num==8)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+4*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.listvalue[DispValue.liststep],4,7,0);
		WriteString_16(100,26+4*22,DispBuf,0);
		
		
		Black_Select=(num==9)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+5*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.delay[DispValue.liststep],1,4,0);
		WriteString_16(100,26+5*22,DispBuf,0);
		
		Black_Select=(num==10)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+6*22-2,92,20);
		Colour.Fword=White;
		if(LoadSave.language==0)
			WriteString_16(100,26+6*22,List_CompType[LoadSave.listcomp[DispValue.liststep]],0);
		else
			WriteString_16(100,26+6*22,List_CompTypeE[LoadSave.listcomp[DispValue.liststep]],0);
		
		Black_Select=(num==11)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+7*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.listhigh[DispValue.liststep],4,7,0);
		WriteString_16(100,26+7*22,DispBuf,0);
		
		Black_Select=(num==12)?1:0;
		if(Black_Select)
		{
			Colour.black=LCD_COLOR_SELECT;
		
		}
		else
		{
			Colour.black=LCD_COLOR_TEST_BACK;
		}
		LCD_DrawFullRect(100,26+8*22-2,92,20);
		Colour.Fword=White;
		Hex_Format(LoadSave.listlow[DispValue.liststep],4,7,0);
		WriteString_16(100,26+8*22,DispBuf,0);
		
		
	}
	switch(num)
	{
		case 1:
		case 5:
		case 8:
		case 9:
		case 11:
		case 12:
			Disp_Hint(2);
		break;
		case 6:
		{
			Disp_Hint(5);
		}break;
		case 2:
		case 4:
		case 7:
		case 10:
			Disp_Hint(7);
		break;
		default:
			Disp_Hint(4);
		break;
	}
	if(DispValue.alertdisp == 0)
	{
		Disp_Fastbutton();
		if(resdisp == 0)
		{
			switch(num)
			{
				case 0:
						Disp_Button_value1(0);
					break;
	//			case 1:
	//			case 5:
	//			case 8:
	//			case 9:
	//			case 11:
	//			case 12:
	//			{
	//				Disp_Fastbutton();
	//				Colour.Fword=White;
	//				Colour.black=LCD_COLOR_TEST_BUTON;
	//				if(LoadSave.language)
	//				{
	//					pt=INPUT_E;
	//				}
	//				else
	//				{
	//					pt=INPUT;
	//				
	//				}
	//				for(i=0;i<1;i++)
	//				{
	//					
	//					WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
	//				}
	//			}break;
				case 2:
				
						Disp_Fastbutton();
						Colour.Fword=White;//
						Colour.black=LCD_COLOR_TEST_BUTON;
						
						if(LoadSave.language)
						{
							pt=Step_ModeE;
						}
						else
						{
							pt=Step_Mode;
						
						}
						for(i=0;i<3;i++)
						WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
				
				break;
				case 4:
						Disp_Fastbutton();
						Colour.Fword=White;//
						Colour.black=LCD_COLOR_TEST_BUTON;
						
						if(LoadSave.language)
						{
							pt=List_BeepE;
						}
						else
						{
							pt=List_Beep;
						
						}
						for(i=0;i<2;i++)
						WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
				
				break;
				case 7:
						Disp_Fastbutton();
						Colour.Fword=White;//
						Colour.black=LCD_COLOR_TEST_BUTON;
						
						if(LoadSave.language)
						{
							pt=List_ItemDisE;
						}
						else
						{
							pt=List_ItemDis;
						
						}
						for(i=0;i<5;i++)
						WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
				
				break;	
				case 10:
						Disp_Fastbutton();
						Colour.Fword=White;//
						Colour.black=LCD_COLOR_TEST_BUTON;
						
						if(LoadSave.language)
						{
							for(i=0;i<4;i++)
								WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, List_CompTypeE[i],  0);
//							pt=List_CompTypeE;
						}
						else
						{
							for(i=0;i<4;i++)
								WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, List_CompType[i],  0);
//							pt=List_CompType;
						
						}
						
				
				break;	
				default:
					
				break;	
			}
		}else if(resdisp == 1){
			Disp_Fastbutton();
			Colour.Fword=White;//
			Colour.black=LCD_COLOR_TEST_BUTON;
			
			if(LoadSave.language)
			{
				pt=List_ResBackE;
			}
			else
			{
				pt=List_ResBack;
			
			}
			for(i=0;i<5;i++)
			WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
		}
	}else{
		Disp_Fastbutton();
		Colour.Fword=White;//
		Colour.black=LCD_COLOR_TEST_BUTON;
		WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
	}

	

}

void Disp_TestSet_MidRect(void)
{

    Disp_MidRect();
}
void Disp_TestSetScreen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键
//	Disp_TestSet_MidRect();//显示中间色块
}

//设置界面	 
void Disp_Limit_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Limit_item[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_TestSetScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[6],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
    if(LoadSave.language)
        pt=Limit_itemE;
    else
        pt=Limit_item;
        
	for(i=0;i<(sizeof(Limit_item)/(sizeof(Limit_item[0])));i++)
	if(i<6)
	{
//		if(i>2&&i<8)
//			Colour.black=LCD_COLOR_TEST_BACK;
//		else
			Colour.black=LCD_COLOR_TEST_BACK;	
		
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, pt[i],  0);

	}
	else
	{
	  	if((i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)>2&&(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)<8)
			Colour.black=LCD_COLOR_TEST_BACK;
		else
			Colour.black=LCD_COLOR_TEST_BACK;

		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-6), pt[i],  0);
		
		
	}	
	//Disp_Button_TestSet(0);
    Disp_Button_value1(3);

}

//设置界面	 
void Disp_Test_Set_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Set_testitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
    
	Disp_TestSetScreen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[4],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
    if(LoadSave.language)
        pt=Set_testitem_E;
    else
        pt=Set_testitem;
        
	for(i=0;i<(sizeof(Set_testitem)/(sizeof(Set_testitem[0])));i++)
	if(i<sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)
	{
		if(i>2&&i<8)
			Colour.black=LCD_COLOR_TEST_BACK;
		else
			Colour.black=LCD_COLOR_TEST_BACK;	
		
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, pt[i],  0);

	}
	else
	{
	  	if((i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)>2&&(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2)<8)
			Colour.black=LCD_COLOR_TEST_BACK;
		else
			Colour.black=LCD_COLOR_TEST_BACK;

		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(Set_testitem)/(sizeof(Set_testitem[0]))/2), pt[i],  0);
		
		
	}	
	//Disp_Button_TestSet(0);
    Disp_Button_value1(2);

}


//显示设置参数的值Setup_Valueall
void DispSet_value(u8 keynum)
{
	vu32 i;
    const u8 (*pt)[7];
	const u8 (*pt1)[8];
    const u8 (*ppt)[11];
	vu32 Black_Select;
	
	Colour.Fword = White;
//远端测量
	Black_Select=(keynum==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }
	LCD_DrawFullRect(LIST1+88, FIRSTLINE, SELECT_1END-(LIST1+88), SPACE1-4);	
	WriteString_16(LIST1+88, FIRSTLINE, pt[LoadSave.sence],  0);//增加算法  把顺序改过来
	
//电流量程	
	Black_Select=(keynum==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
    if(LoadSave.language)
    {
        pt=Test_Rangevalue_E;
    
    }
    else
    {
        pt=Test_Rangevalue;
    
    }
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1-2, SELECT_1END-(LIST1+88), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, pt[LoadSave.crange],  0);
	
	
//电压量程
	Black_Select=(keynum==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
    if(LoadSave.language)
    {
        pt=Test_Rangevalue_E;
    
    }
    else
    {
        pt=Test_Rangevalue;
    
    }
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*2, SELECT_1END-(LIST1+88), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, pt[LoadSave.vrange],  0);
	
	
//启动电压	
	Black_Select=(keynum==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
		
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	
	Hex_Format(LoadSave.onvol ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*3-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*3, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1*3,DISP_UINT[0],  0);

	
//关断电压
	Black_Select=(keynum==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	Hex_Format(LoadSave.offvol ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*4-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*4, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1*4,DISP_UINT[0],  0);
	
	
	
//电压下降
	Black_Select=(keynum==6)?1:0;
	
	
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	Hex_Format(LoadSave.cvdowntime ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*5-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*5, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1*5, " V/ms",  0);

//延时关断
	Black_Select=(keynum==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.autooff != 0)
	{
		Hex_Format(LoadSave.autooff , 0 , 6 , 0);
		LCD_DrawFullRect(LIST2+88, FIRSTLINE-2, SELECT_1END-(LIST1+70), SPACE1-4);
		WriteString_16(LIST2+88, FIRSTLINE, DispBuf,  0);
		WriteString_16(LIST2+90+8*7+2, FIRSTLINE,Unit_Setitem[4],  0);
	}else{
		LCD_DrawFullRect(LIST2+88, FIRSTLINE-2, SELECT_1END-(LIST1+70), SPACE1-4);
		if(LoadSave.language == 0)
		{
			WriteString_16(LIST2+88, FIRSTLINE, "关闭",  0);
		}else{
			WriteString_16(LIST2+88, FIRSTLINE, "OFF",  0);
		}
	}
    
	//WriteString_16(LIST2+88, FIRSTLINE, User_Range[Jk516save.Set_Data.Range],  0);
	
//最大电流
	Black_Select=(keynum==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
     if(LoadSave.language)
    {
        pt=Setup_Beep_E;
    
    }
    else
    {
        pt=Setup_Beep;
    
    }
	Hex_Format(LoadSave.maxc , 4 , 7 , 0);
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*1-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*1, DispBuf,  0);
	WriteString_16(LIST2+90+80, FIRSTLINE+SPACE1*1,DISP_UINT[2],  0);
	
//最大电压
	Black_Select=(keynum==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	 
	Hex_Format(LoadSave.maxv , 4 ,7 , 0);
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*2-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, DispBuf,  0);
	WriteString_16(LIST2+90+80, FIRSTLINE+SPACE1*2,DISP_UINT[0],  0);
	
		
	

//最大功率
	
	Black_Select=(keynum==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	Hex_Format(LoadSave.maxp , 4 , 8 , 0);
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*3-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, DispBuf,  0);
	WriteString_16(LIST2+90+80+10, FIRSTLINE+SPACE1*3,DISP_UINT[4],  0);
		
	
//上升速率
	
	Black_Select=(keynum==11)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	Hex_Format(LoadSave.crise, 4 ,5 , 0);
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*4-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*4, DispBuf,  0);
	
	WriteString_16(LIST2+90+80, FIRSTLINE+SPACE1*4, " A/ms",  0);
	
	
//下降速率
	
	Black_Select=(keynum==12)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	
	Hex_Format(LoadSave.cdrop, 4 ,5 , 0);
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*5-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*5, DispBuf,  0);
	
	WriteString_16(LIST2+90+80, FIRSTLINE+SPACE1*5, " A/ms",  0);
	
	switch(keynum)
	{
		case 4:
		case 5:
		case 6:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
			Disp_Hint(2);
		break;
		case 1:
		case 2:
		case 3:
		case 7:
			Disp_Hint(7);
		break;
		default:
			Disp_Hint(4);
		break;
	}
	Disp_Fastbutton();
	switch(keynum)
	{
		case 0:
				Disp_Button_value1(2);
			break;
		case 1:
			
				Colour.Fword=White;
				Colour.black=LCD_COLOR_TEST_BUTON;
                if(LoadSave.language)
                {
                    pt=Test_Compvalue_E;
                
                }
                else
                {
                    pt=Test_Compvalue;
                
                }
				for(i=0;i<2;i++)
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
				
			
		break;
		case 2:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
            if(LoadSave.language)
            {
                pt=Test_Rangevalue_E;
            
            }
            else
            {
                pt=Test_Rangevalue;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
		break;
		case 3:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
            if(LoadSave.language)
            {
                pt=Test_Rangevalue_E;
            
            }
            else
            {
                pt=Test_Rangevalue;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:

			break;
		
			
		default:
			break;
		
			
	
	
	}

}




//显示极限设置参数的值
void DispLimit_value(u8 keynum)
{
	vu32 i;
    const u8 (*pt)[7];
	const u8 (*pt1)[8];
    const u8 (*ppt)[11];
	vu32 Black_Select;
	
	Colour.Fword = White;
//电压上限
	Black_Select=(keynum==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	Hex_Format(LoadSave.vhigh ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE, SELECT_1END-(LIST1+88), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE, DispBuf,  0);	
	WriteString_16(LIST1+90+80, FIRSTLINE,DISP_UINT[0],  0);
	
//电压下限
	Black_Select=(keynum==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	Hex_Format(LoadSave.vlow ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1-2, SELECT_1END-(LIST1+88), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1,DISP_UINT[0],  0);
	
//电流上限
	Black_Select=(keynum==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	Hex_Format(LoadSave.chigh ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*2, SELECT_1END-(LIST1+88), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*2, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1*2,DISP_UINT[2],  0);
	
//电流下限	
	Black_Select=(keynum==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
		
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	
	Hex_Format(LoadSave.clow ,4 , 7 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*3-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*3, DispBuf,  0);
	WriteString_16(LIST1+90+80, FIRSTLINE+SPACE1*3,DISP_UINT[2],  0);

	
//功率上限
	Black_Select=(keynum==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	Hex_Format(LoadSave.phigh ,4 , 8 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*4-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*4, DispBuf,  0);
	WriteString_16(LIST1+90+80+10, FIRSTLINE+SPACE1*4,DISP_UINT[4],  0);
	
	
	
//功率下限
	Black_Select=(keynum==6)?1:0;
	
	
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	
	Hex_Format(LoadSave.plow ,4 , 8 , 0);
	LCD_DrawFullRect(LIST1+88, FIRSTLINE+SPACE1*5-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST1+88, FIRSTLINE+SPACE1*5, DispBuf,  0);
	WriteString_16(LIST1+90+80+10, FIRSTLINE+SPACE1*5,DISP_UINT[4],  0);

//极限显示
	Black_Select=(keynum==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }
	LCD_DrawFullRect(LIST2+88, FIRSTLINE-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE, pt[LoadSave.limitdisp],  0);
    
	//WriteString_16(LIST2+88, FIRSTLINE, User_Range[Jk516save.Set_Data.Range],  0);
	
//极限讯响
	Black_Select=(keynum==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
    if(LoadSave.language)
    {
        pt=List_BeepE;
    
    }
    else
    {
        pt=List_Beep;
    
    }
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*1-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*1, pt[LoadSave.limitbeep],  0);
	
//电压判定
	Black_Select=(keynum==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }
	 
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*2-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*2, pt[LoadSave.vcomp],  0);

	
		
	

//电流判定
	
	Black_Select=(keynum==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*3-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*3, pt[LoadSave.ccomp],  0);
		
	
//功率判定
	
	Black_Select=(keynum==11)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }
	LCD_DrawFullRect(LIST2+88, FIRSTLINE+SPACE1*4-2, SELECT_1END-(LIST1+70), SPACE1-4);
	WriteString_16(LIST2+88, FIRSTLINE+SPACE1*4, pt[LoadSave.pcomp],  0);

	switch(keynum)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			Disp_Hint(2);
		break;
		case 0:
			Disp_Hint(4);
		break;
		default:
			Disp_Hint(7);
		break;
	}
	
	Disp_Fastbutton();
	switch(keynum)
	{
		case 0:
				Disp_Button_value1(3);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			break;
		
			
		case 8:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
            if(LoadSave.language)
            {
                pt=List_BeepE;
            
            }
            else
            {
                pt=List_Beep;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			break;
		case 7:
		case 9:
		case 10:
		case 11:
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BUTON;
            if(LoadSave.language)
            {
                pt=Test_Compvalue_E;
            
            }
            else
            {
                pt=Test_Compvalue;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			break;
			
		default:
			break;
		
			
	
	
	}

}

void Disp_Sys_Screen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键	  
//	Disp_ListScan_MidRect();//显示中间色块	 User_LimitList_Item  
}
//系统设置子函数
void Disp_SysLine(void)
{
 	uint32_t i;
	for(i=0;i<7;i++)
	{
		//if(i<=13/2)
			LCD_DrawFullRect( 90, FIRSTLINE+(i+1)*SPACE1-2, 100,1);
//		else
//			LCD_DrawLine( 90+250, FIRSTLINE+(i-13/2+1)*SPACE1-2, 90+100+250,FIRSTLINE+(i-13/2+1)*SPACE1-2, White );
	}

}
void Disp_Sys_Item(void)
{
	uint32_t i;
    const u8 (*pt)[sizeof(Sys_Setitem[0])];
    const u8 (*ppt)[sizeof(All_TopName[0])];
	Disp_Sys_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[8],  0);
	//WriteString_16(0, 4, All_TopName[8],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
    if(LoadSave.language)
		pt=Sys_Setitem_E;
    else
        pt=Sys_Setitem;
//	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);
    
	for(i=0;i<(sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0])));i++)
	//if(i<=sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, pt[i],  0);
        

	}
//	else
//	{
//		WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2), Sys_Setitem[i],  0);
//	}
    Colour.black=White;
	Disp_SysLine();
	Disp_Button_value1(3);
	//Disp_Button_SysSet();
}

//显示设置参数的值Setup_Valueall
void Disp_Sys_value(u8 keynum)
{
//	vu32 flag;
	vu32 i;
    const u8 (*pt)[7];
//	vu32 xpose;
	vu32 Black_Select;
    RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
    RTC_TimeStructure.RTC_H12 = RTC_HourFormat_24;
//	vu32 Select_color;
//串口
//    keynum=1;
	Black_Select=(keynum==1)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
//	if(LoadSave.language)
//    {
//        pt=Test_Compvalue_E;
//    
//    }
//    else
//    {
//        pt=Test_Compvalue;
//    
//    }	
	LCD_DrawFullRect( LIST1+90, FIRSTLINE,SELECT_1END-(LIST1+90) , SPACE1-4 ) ;//SPACE1+
	Hex_Format(LoadSave.Addr,0,3,0);
	WriteString_16(LIST1+90, FIRSTLINE+2, DispBuf,  0);//
	
//通讯协议
	Black_Select=(keynum==2)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1,SELECT_1END-(LIST1+90) , SPACE1-4  ) ;//SPACE1
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1+2, Test_PCTLvalue_E[LoadSave.COMM],  0);
	//U盘开关
    Black_Select=(keynum==3)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Compvalue_E;
    
    }
    else
    {
        pt=Test_Compvalue;
    
    }	
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*2,SELECT_1END-(LIST1+90) , SPACE1-4  ) ;//SPACE1
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*2+2, pt[Jk516save.Sys_Setvalue.u_flag],  0);
	//通讯接口选择
    Black_Select=(keynum==4)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	if(LoadSave.language)
    {
        pt=Test_Commvalue_E;
    
    }
    else
    {
        pt=Test_Commvalue_E;
    
    }	
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*3,SELECT_1END-(LIST1+90) , SPACE1-4  ) ;//SPACE1
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*3+2, pt[LoadSave.COMM],  0);
	
//显示语言
	Black_Select=(keynum==5)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*4,SELECT_1END-(LIST1+90) , SPACE1 -4 ) ;//SPACE1
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*4+2, Sys_Language_Value[LoadSave.language],  0);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
//年
	Black_Select=(keynum==6)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.Year=17;
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*5,LIST1+90+18-(LIST1+90) , SPACE1-4 ) ;//SPACE1

	sprintf((char *)DispBuf,"%2d",RTC_DateStructure.RTC_Year);
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+90+20, FIRSTLINE+SPACE1*5+2, "-",  0);
//月
	
	Black_Select=(keynum==7)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.Mon=17;
	LCD_DrawFullRect( LIST1+90+30, FIRSTLINE+SPACE1*5,18 , SPACE1-4  ) ;//SPACE1

	sprintf((char *)DispBuf,"%2d",RTC_DateStructure.RTC_Month);
	WriteString_16(LIST1+90+30, FIRSTLINE+SPACE1*5+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+90+20+30, FIRSTLINE+SPACE1*5+2, "-",  0);
	
	
	//日
	
	Black_Select=(keynum==8)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Data_Value.data=17;
	LCD_DrawFullRect( LIST1+90+30+32, FIRSTLINE+SPACE1*5,18 ,SPACE1-4 ) ;//SPACE1

	sprintf((char *)DispBuf,"%2d",RTC_DateStructure.RTC_Date);
	WriteString_16(LIST1+90+30+32, FIRSTLINE+SPACE1*5+2, DispBuf,  0);

//时
	Black_Select=(keynum==9)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Timer_Value.Hour=1;
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*6,18 , SPACE1-4 ) ;//SPACE1

	sprintf((char *)DispBuf,"%2d",RTC_TimeStructure.RTC_Hours);
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*6+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+90+20, FIRSTLINE+SPACE1*6+2, ":",  0);
//分
	
	Black_Select=(keynum==10)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	//SaveData.Sys_Setup.Timer_Value.Min=17;
	LCD_DrawFullRect( LIST1+90+30, FIRSTLINE+SPACE1*6,18 , SPACE1-4 ) ;//SPACE1

	sprintf((char *)DispBuf,"%2d",RTC_TimeStructure.RTC_Minutes);
	WriteString_16(LIST1+90+30, FIRSTLINE+SPACE1*6+2, DispBuf,  0);
	Colour.black=LCD_COLOR_TEST_BACK;
	WriteString_16(LIST1+90+20+30, FIRSTLINE+SPACE1*6+2, ":",  0);
	
	
	//秒
	
	Black_Select=(keynum==11)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
	LCD_DrawFullRect( LIST1+90+30+32, FIRSTLINE+SPACE1*6,18 ,SPACE1-4 ) ;//SPACE1
	sprintf((char *)DispBuf,"%2d",RTC_TimeStructure.RTC_Seconds);
	WriteString_16(LIST1+90+30+32, FIRSTLINE+SPACE1*6+2, DispBuf,  0);
	//文件名称
   Black_Select=(keynum==12)?1:0;
	if(Black_Select)
	{
		Colour.black=LCD_COLOR_SELECT;
	
	}
	else
	{
		Colour.black=LCD_COLOR_TEST_BACK;
	}
		
	LCD_DrawFullRect( LIST1+90, FIRSTLINE+SPACE1*7,SELECT_1END-(LIST1+90) ,SPACE1 -4 ) ;//SPACE1
	WriteString_16(LIST1+90, FIRSTLINE+SPACE1*7+2,(const uint8_t *) Jk516save.Sys_Setvalue.textname,  0);

    
	Disp_Fastbutton();
    Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BUTON;
	switch(keynum)
	{
		case 0:
				Disp_Button_SysSet();
			break;
//		case 1:
////            if(LoadSave.language)
////            {
////                pt=Test_Compvalue_E;
////            
////            }
////            else
////            {
////                pt=Test_Compvalue;
////            
////            }
////            for(i=0;i<2;i++)
////            WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);


//		break;
		case 2:
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Test_PCTLvalue_E[i],  0);
			}
		break;
		case 3:
			if(LoadSave.language)
            {
                pt=Test_Compvalue_E;
            
            }
            else
            {
                pt=Test_Compvalue;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			break;
		case 4:
            if(LoadSave.language)
            {
                pt=Test_Commvalue_E;
            
            }
            else
            {
                pt=Test_Commvalue_E;
            
            }
			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
			}
			break;
		case 5:

			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE-16, BUTTOM_Y_VALUE, Sys_Language_Value[i],  0);
			}
			break;
		case 6:
		case 7:
        case 8:
        case 9:
            
        case 10:
            
        case 11:

			for(i=0;i<2;i++)
			{
				
				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
			}
			break;
	
//		case 12:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<5;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, Sys_Addr_value[i],  0);
//			}
//			break;
//		case 13:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 14://
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, CompButton_Tip[i],  0);
//			}
//			break;
//		case 15:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 16:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 17:
//			Colour.Fword=White;
//			Colour.black=LCD_COLOR_TEST_BUTON;
//			for(i=0;i<2;i++)
//			{
//				
//				WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, BiasButton_Tip[i],  0);
//			}
//			break;
//		case 18:
//			break;
//		case 19:
//			break;
//		case 20:
//			break;

		default:
			break;
		
			
	
	
	}

}

//系统设置
void Use_SysSetProcess(void)
{	
	//vu32 keynum=0;
	vu8 key,i;
	Disp_Coordinates_Typedef  Coordinates;
//    uint8_t Disp_buff[12];
    char key_count=0;
	vu8 Disp_flag=1;
//    u8 keynum1=0;
    u8 keynum=0;
	u8 keytrans=0;
    RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	//Button_Page_Typedef Button_Page;
	//keynum=0;
	//Button_Page.page=0;
    //lcd_Clear(LCD_COLOR_TEST_BACK);
	Disp_Sys_Item();
	//Delay_Key();
 	while(GetSystemStatus()==SYS_STATUS_SYSSET)
	{
	  	keytrans=Encoder_Process(keynum);
		if(spinflag == 1)
		{
			Disp_flag = 1;
			keynum = keytrans;
		}
		if(Disp_flag==1)
		{
			Disp_Sys_value(keynum);
			Disp_flag=0;
            
		}
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
        RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		
		key=Key_Read();
		inputtrans = key;
        if(key!=KEY_NONE)
		{	
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:

					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);//
							break;
						case 2:
						{
							LoadSave.COMM = 0;
							Set_Comm();
							Store_set_flash();
						}break;
						case 4:
						{
							LoadSave.COMM = 0;
							Set_Comm();
							Store_set_flash();
						}break;
						case 5:
						{
							LoadSave.language = 0;
							Store_set_flash();
						}break;
						case 6:
							if(RTC_DateStructure.RTC_Year<1)
								RTC_DateStructure.RTC_Year=RTC_YEAR_MAX;
							else
                                if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
                                    RTC_DateStructure.RTC_Year=0;
                                else
								RTC_DateStructure.RTC_Year--;
								// RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, RTC_TIME_DISP.YEAR);
                            
								//RTC_CalibConfig(LPC_RTC, 0, RTC_CALIB_DIR_FORWARD);
								//RTC_CalibCounterCmd(LPC_RTC, ENABLE);
                                RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                                RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month<=1)
								RTC_DateStructure.RTC_Month=RTC_MONTH_MAX;
							else
								RTC_DateStructure.RTC_Month--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date<=1)
								RTC_DateStructure.RTC_Date=RTC_DAYOFMONTH_MAX;
							else
								RTC_DateStructure.RTC_Date--;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
                           
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours<1)
								RTC_TimeStructure.RTC_Hours=RTC_HOUR_MAX;
							else
								RTC_TimeStructure.RTC_Hours--;
//                            
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes<1)
								RTC_TimeStructure.RTC_Minutes=RTC_MINUTE_MAX;
							else
								RTC_TimeStructure.RTC_Minutes--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds<1)
								RTC_TimeStructure.RTC_Seconds=RTC_SECOND_MAX;
							else
								RTC_TimeStructure.RTC_Seconds--;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;	
						default:
						break;
					
					
					}

				break;
				case Key_F2:
					

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_SETUP);
//							else
//								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 2:
						{
							LoadSave.COMM = 2;
							Set_Comm();
							Store_set_flash();
						}break;
						case 4:
						{
							LoadSave.COMM = 1;
							Set_Comm();
							Store_set_flash();
						}break;
						case 5:
						{
							LoadSave.language = 1;
							Store_set_flash();
						}break;
						case 6:
							if(RTC_DateStructure.RTC_Year>=RTC_YEAR_MAX)
								RTC_DateStructure.RTC_Year=0;
							else
								RTC_DateStructure.RTC_Year++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 7:
							if(RTC_DateStructure.RTC_Month>=RTC_MONTH_MAX)
								RTC_DateStructure.RTC_Month=1;
							else
								RTC_DateStructure.RTC_Month++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 8:
							if(RTC_DateStructure.RTC_Date>=RTC_DAYOFMONTH_MAX)
								RTC_DateStructure.RTC_Date=1;
							else
								RTC_DateStructure.RTC_Date++;
                            RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 9:
							if(RTC_TimeStructure.RTC_Hours>=RTC_HOUR_MAX)
								RTC_TimeStructure.RTC_Hours=0;
							else
								RTC_TimeStructure.RTC_Hours++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 10:
							if(RTC_TimeStructure.RTC_Minutes>=RTC_MINUTE_MAX)
								RTC_TimeStructure.RTC_Minutes=0;
							else
								RTC_TimeStructure.RTC_Minutes++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						case 11:
							if(RTC_TimeStructure.RTC_Seconds>=RTC_SECOND_MAX)
								RTC_TimeStructure.RTC_Seconds=0;
							else
								RTC_TimeStructure.RTC_Seconds++;
                            RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
                            RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
							break;
						
						default:
						break;
					}				
			
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
//								SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 1:
						{
						}break;
						case 2:
						{
							LoadSave.Baudrate = 2;
							Set_Baudrate();
							Store_set_flash();
						}break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:

							break;
						case 7:

							break;
						case 8:
							break;

						
						default:
							break;
					
					
					}	
					
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
//								SetSystemStatus(SYS_STATUS_SYS);
							break;
						case 1:
                            
							
									
							break;
						case 2:
						{
							LoadSave.Baudrate = 3;
							Set_Baudrate();
							Store_set_flash();
						}break;
						case 3:
							break;
						case 4:

							break;
						case 5:
							break;
						case 6:
							
							break;
						case 7:
                            
							break;
						case 8:
							
							break;
						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(keynum)
					{
						case 0:
//							JumpBoot(55);
						break;
						case 2:
						{
							LoadSave.Baudrate = 4;
							Set_Baudrate();
							Store_set_flash();
						}break;
						case 4:
						case 9:
							break;
						case 7:	
							
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;

				case Key_DOT:

				case Key_NUM1:
				//break;
				case Key_NUM2:
				//break;
				case Key_NUM3:
				//break;
				case Key_NUM4:
				//break;
				case Key_NUM5:
				//break;
				case Key_NUM6:
				//break;
				case Key_NUM7:
				//break;
				case Key_NUM8:
				//break;
				case Key_NUM9:
				//break;
				case Key_NUM0:
					switch(keynum)
					{
						case 1:
							Coordinates.xpos=LIST1+88+2;
							Coordinates.ypos=FIRSTLINE;
							Coordinates.lenth=76;
							LoadSave.Addr=Disp_Set_Addr(&Coordinates);
							Set_Para();
							Store_set_flash();
						break;
					}
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				case PRESS_SPIN:
				{

				}break;
				case Key_Ent:
				{
				}break;
				case Key_ESC:
				{
					if(mainswitch == 0)
						SetSystemStatus(SYS_STATUS_TEST);
				}break;
				case Key_SAVE:
				{
					
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
				}break;
				default:
				break;
					
			}
		}
		
	
	}
    
}

void Disp_Sys(void)
{
	uint32_t i;
    const u8 (*pt)[(sizeof(Sys_Sys[0]))];
    const u8 (*ppt)[(sizeof(All_TopName[0]))];
	Disp_Sys_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
    if(LoadSave.language)
        ppt=All_TopName_E;
    else
        ppt=All_TopName;
	WriteString_16(0, 4, ppt[10],  0);
	//WriteString_16(0, 4, All_TopName[10],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;
	if(Jk516save.open)
	{
		if(LoadSave.language)
			pt=Sys_Sys_E;
		else
			pt=Sys_Sys;
	}else{
		if(LoadSave.language)
			pt=Sys_Sys_E1;
		else
			pt=Sys_Sys1;
	}
    
//	WriteString_16(LIST1, FIRSTLINE, User_ListScan_Item[0],  0);

	for(i=0;i<(sizeof(Sys_Sys)/(sizeof(Sys_Sys[0])));i++)
	//if(i<=sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2)
	{
		WriteString_16(LIST1, FIRSTLINE+SPACE1*i, pt[i],  0);

	}//Save_Res.fac_num
    WriteString_16(LIST1+82, FIRSTLINE+SPACE1*3, (const uint8_t*)Jk516save.fac_num,  0);
//	else
//	{
//		WriteString_16(LIST2,FIRSTLINE+SPACE1*(i-sizeof(Sys_Setitem)/(sizeof(Sys_Setitem[0]))/2), Sys_Setitem[i],  0);
//	}
//	Disp_SysLine();
	Disp_Button_SysSet();
}

void Sys_Process(void)
{
	vu32 keynum=0;
	vu8 key;
	vu8 Disp_flag=1;
    LCD_Clear(LCD_COLOR_TEST_BACK);

	while(GetSystemStatus()==SYS_STATUS_SYS)
	{
		if(Disp_flag==1)
		{
			Disp_Sys();
			Disp_flag=0;	
		}
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		if(key!=KEY_NONE)
		 {
			Disp_flag=1;
//            Key_Beep();
			switch(key)
			{
				case Key_F1:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);
							break;
						
						default:
							break;
					
					}
                    
				break;
				case Key_F2:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SETUP);
							break;
						
					
					}
                    
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						
						default:
						break;
					
					}
						
				break;
				case Key_F4:
					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_SYS);
							break;
						
							
						
						default:
						break;
					
					}
				break;
				
				

				
				
				default:
				break;
					
			}
		
		
		}
	
	
	
	
	}

}
void Disp_button_Num_Step(void)
{
	const u8 (*pt)[sizeof(Unit_Setitem[0])];
	
	Disp_Fastbutton();
	
	pt = Unit_Setitem;
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE, pt[6],  0);

}

void Disp_button_Num_time(void)
{
	const u8 (*pt)[sizeof(Unit_Setitem[0])];
	
	Disp_Fastbutton();
	
	pt = Unit_Setitem;
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE, pt[LoadSave.mode],  0);

}

void Disp_button_CNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Unit_Setitem[0],  0);

}

void Disp_button_VNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE, Unit_Setitem[1],  0);

}

void Disp_button_RNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Unit_Setitem[2],  0);

}

void Disp_button_PNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Unit_Setitem[3],  0);

}

void Disp_button_SNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Unit_Setitem[4],  0);

}

void Disp_button_MSNum_time(void)
{
	
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(BUTTOM_X_VALUE, BUTTOM_Y_VALUE,Unit_Setitem[7],  0);

}
//数字键输入显示
Sort_TypeDef Disp_NumKeyboard_Set(Disp_Coordinates_Typedef *Coordinates,u8 flag )
{
	u8 initinput;
    u8 lenth;
	vu8 While_flag=1;
	vu8 Disp_buff[10];
	vu8 key=inputtrans,i;
	vu8 dispflag=1;
	vu8 dot_num=0,dot_num1=0;
//	vu8 page=0;
	vu32 keynum=0;
	vu8 key_count=0;
//	vu32 Num[6]={1,10,100,1e3,1e4,1e5};
	Sort_TypeDef   Sort_set;
	Sort_set.Dot=0;
	Sort_set.Num=0;
	Sort_set.Unit=0;
	Sort_set.Num=0;
	for(i=0;i<8;i++)
	Disp_buff[i]=' ';
	Disp_buff[8]=0;
	if(flag)
        lenth=NUM_LENTH_V;
    else
        lenth=NUM_LENTH;
	while(While_flag)
	{
		if(initinput == 1)
		{
			key=Key_Read();
		}else{
			initinput = 1;
		}
        if(key!=KEY_NONE)
		{
			dispflag=1;
//            Key_Beep();
			switch(key)
			{
				case Key_F1:
					
					Sort_set.Unit=0;
					
					While_flag=0;
					if(key_count<lenth)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F2:
					Sort_set.Unit=1;
					
					While_flag=0;
					if(key_count<lenth)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F3:
                    Sort_set.Unit=2;
					
					While_flag=0;
					if(key_count<lenth)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;

				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;

				case Key_NUM1:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<lenth)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<lenth&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_ESC:
				{
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
				}break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				case Key_Ent:
					
					Sort_set.Unit=0;
					
					While_flag=0;
					if(key_count<lenth)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					
					}
						
					
					Sort_set.Updata_flag=1;
					
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            Colour.Fword=White;
			Colour.black=LCD_COLOR_RED;
			LCD_DrawFullRect( Coordinates->xpos, Coordinates->ypos,Coordinates->lenth+16, SPACE1-4);
			
			WriteString_16(Coordinates->xpos, Coordinates->ypos, (const uint8_t *)Disp_buff,  0);
			//dispflag=0;
		}
	
	}
	for(i=key_count;i<dot_num1+5;i++)
		Disp_buff[i]='0';
	for(i=0;i<dot_num1+5;i++)
	{
		if(Disp_buff[0]>='0'&&(Disp_buff[0]<='9'))
		{
			if(Disp_buff[i]>='0'&&(Disp_buff[i]<='9'))
			{
			
				if(dot_num1>i)
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				}
				else
				{
					Sort_set.Num*=10;
					Sort_set.Num+=Disp_buff[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}
	
	
	
	}
	Sort_set.Dot=dot_num1;
//	if(Disp_buff[0]>='0'&&(Disp_buff[0]<'9'))
//		{
//			if(Disp_buff[key_count-1]!='.')		
//			{
//				Sort_set.Num*=Num[key_count-dot_num-1];
//				Sort_set.Num+=(Disp_buff[key_count-1]-'0');//*Num[key_count-dot_num-1];
//				
//			}				
//			//*(Disp_buff[key_count-1]-'0'))+=Num[key_count-dot_num-1];
//			else
//			{
//			
//			}
//		
//		
//		}
//			
//		else 
//			;//(Disp_buff[key_count-1]-'0')*Sort_set.Num+=Num[key_count-dot_num-2];
	return Sort_set;

}
Sort_TypeDef Time_Set_Cov(Sort_TypeDef *Time)
{
	float value;
	if(Time->Num>99999)
	{
		Time->Num/=10;
		Time->Dot=MAX_DOTNUM;
		
	}
	Time->Dot=MAX_DOTNUM-Time->Dot;
	value=Time->Num;
	if(Time->Unit==0)
		value*=(float)1e4;
	else if(Time->Unit==1)
		value*=(float)1e7;
    else if(Time->Unit==2)
		value*=(float)1e10;
        
	value/=pow(10,Time->Dot);
	if(value>(float)3e11)
	{
		//value=3e10;
		Time->Num=30000;
		Time->Dot=3;
		Time->Unit=2;
		
	}else
    if(value>=(float)3e10)
	{
		Time->Num=value/(float)1e7;
		Time->Dot=3;
		Time->Unit=2;
		
	
	
	
	}else if(value>=(float)3e9)
	{
		Time->Num=value/(float)1e6;
		Time->Dot=4;
		Time->Unit=2;
	
	
	}else if(value>=(float)3e8)
	{
		Time->Num=value/(float)1e5;
		Time->Dot=2;
		Time->Unit=1;
	
	
	}
	else if(value>=(float)3e7)
	{
		Time->Num=value/(float)10000;
		Time->Dot=3;
		Time->Unit=1;
	
	}else if(value>=(float)3e6)
	{
		Time->Num=value/(float)1000;
		Time->Dot=4;
		Time->Unit=1;
	
	
	}else if(value>=(float)3e5)
	{
		Time->Num=value/(float)100;
		Time->Dot=2;
		Time->Unit=0;
	
	}
    else if(value>=(float)3e4)
        {
		Time->Num=value/10;
		Time->Dot=3;
		Time->Unit=0;
	
	}
        else
        {
        Time->Num=value;
		Time->Dot=4;
		Time->Unit=0;
        
        }
	Time->mid_data=	value;
	
	

	return *Time;
}
//常规测量设置
vu32 Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

//设置电流
vu32 Disp_Set_CNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_CNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}


//设置地址
vu32 Disp_Set_Addr(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
//	Disp_button_CNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
	Sort_num.Num/=10000;
	if(Sort_num.Num > 255)
	{
		Sort_num.Num = 255;
	}
//	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

//设置电压
vu32 Disp_Set_VNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_VNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

//设置电阻
vu32 Disp_Set_RNum(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_RNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	Sort_num.Num/=1000;
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

vu32 Disp_Set_Step(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_Step();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	Sort_num.Num/=10000;
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

vu32 Disp_Set_TimeS(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_SNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	Sort_num.Num/=10000;
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

vu32 Disp_Set_TimeMS(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_MSNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	Sort_num.Num/=1000;
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}	
		
	return Sort_num.Num;

}

//设置功率
vu32 Disp_Set_TimeP(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_PNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}
vu32 Disp_List_Delay(Disp_Coordinates_Typedef *Coordinates)
{
	Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_SNum_time();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
//	Sort_num1=Time_Set_Cov(&Sort_num);
	Sort_num.Num/=1000;
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num.Num;

}

//电阻设置
Sort_TypeDef SCPI_SET_R(void)
{
	u8 i;
	static Sort_TypeDef Scpi_num,Scpi_num1;
	Scpi_num.Dot=0;
	Scpi_num.Num=0;
	Scpi_num.Unit=0;
	
	Scpi_num1.Dot=0;
	Scpi_num1.Num=0;
	Scpi_num1.Unit=0;
	
	
	Scpi_num.Unit=scpiunit;
	if(scpidot == 0)
	{
		scpinum[scpnum]='.';
		scpidot = scpnum;
		for(i=scpnum+1;i<6;i++)
		scpinum[i]='0';
	}else{
		for(i=scpnum;i<6;i++)
		scpinum[i]='0';
	}
	
	
	for(i=0;i<6;i++)
	{
		if(scpinum[0]>='0'&&(scpinum[0]<='9'))
		{
			if(scpinum[i]>='0'&&(scpinum[i]<='9'))
			{
			
				if(scpidot>i)
				{
					Scpi_num.Num*=10;
					Scpi_num.Num+=scpinum[i]-'0';
				
				}
				else
				{
					Scpi_num.Num*=10;
					Scpi_num.Num+=scpinum[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}			
	}
	Scpi_num.Dot=scpidot;
	Scpi_num1=Time_Set_Cov(&Scpi_num);
	scpidot = 0;	
		
	return Scpi_num1;

}
//电阻设置
Sort_TypeDef SCPI_SET_R1(void)
{
	u8 i;
	static Sort_TypeDef Scpi_num,Scpi_num1;
	Scpi_num.Dot=0;
	Scpi_num.Num=0;
	Scpi_num.Unit=0;
	
	Scpi_num1.Dot=0;
	Scpi_num1.Num=0;
	Scpi_num1.Unit=0;
	
	
	Scpi_num.Unit=scpiunit1;
	if(scpidot1 == 0)
	{
		scpinum1[scpnum1]='.';
		scpidot1 = scpnum1;
		for(i=scpnum1+1;i<6;i++)
		scpinum1[i]='0';
	}else{
		for(i=scpnum1;i<6;i++)
		scpinum1[i]='0';
	}
	
	
	for(i=0;i<6;i++)
	{
		if(scpinum1[0]>='0'&&(scpinum1[0]<='9'))
		{
			if(scpinum1[i]>='0'&&(scpinum1[i]<='9'))
			{
			
				if(scpidot1>i)
				{
					Scpi_num.Num*=10;
					Scpi_num.Num+=scpinum1[i]-'0';
				
				}
				else
				{
					Scpi_num.Num*=10;
					Scpi_num.Num+=scpinum1[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}			
	}
	Scpi_num.Dot=scpidot1;
	Scpi_num1=Time_Set_Cov(&Scpi_num);
	scpidot1 = 0;	
		
	return Scpi_num1;

}


Sort_TypeDef Input_Set_Cov(Sort_TypeDef *Input_Ref)//
{
//	vu8 i;
//	vu32 buff[]={10000,1000,100,10,1};
	float value;
//	if(Input_Ref->Num>120000)
//	{
//		Input_Ref->Num=12000;
//		Input_Ref->Dot=3;
//		
//	}
//	value=(float)Input_Ref->Num*1000000;
//	value/=(pow(10,7-Input_Ref->Dot));
	value=Input_Ref->Num/(pow(10,7-Input_Ref->Dot));
	value*=(float)1000000;
	
	if(value>6000000)
	{
		value=6000000;
	}
	if(value>=(float)1e7)
	{
		Input_Ref->Dot=3;
		Input_Ref->Num=value/100;
	
	
	}else if(value>=(float)1e6)
	{
		Input_Ref->Dot=4;
		Input_Ref->Num=value/10;
	
	
	}
	else
	{
		Input_Ref->Dot=5;
		Input_Ref->Num=value;
	
	
	}
	Input_Ref->mid_data=value;
//	value=pow(10.0,4-(float)Input_Ref->Dot);
//	value=Input_Ref->Num/value;
//	if(value>=1000)
//	{
//		Input_Ref->Unit++;
//		Input_Ref->Dot+=3;
//	
//	
//	}else if(value==0)
//	{
//		if(Input_Ref->Unit>0)
//		{
//			Input_Ref->Unit--;
//			if(Input_Ref->Dot>=3)
//				Input_Ref->Dot-=3;
//		
//		
//		}
//	
//	
//	}
	
	return *Input_Ref;
}
void Disp_button_Num_Freq(void)
{
	Disp_Fastbutton();
	
	Colour.black=LCD_COLOR_TEST_BUTON;
	Colour.Fword=White;
	WriteString_16(84, 271-30, " V ",  0);
	//WriteString_16(84+80, 271-30, " Ω ",  0);


}
//电压设置
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates)
{

        
    Sort_TypeDef Sort_num,Sort_num1;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
	Sort_num1=Input_Set_Cov(&Sort_num);
	if(Sort_num1.Updata_flag==0)
	{
		Sort_num1.Dot=0;
		Sort_num1.Num=0;
		Sort_num1.Unit=0;
	
	}
		
	return Sort_num1;

}

//电压设置
Sort_TypeDef SCPI_SET_V(void)
{
	u8 i;
	static Sort_TypeDef ScpiV_num,ScpiV_num1;
	ScpiV_num.Dot=0;
	ScpiV_num.Num=0;
	ScpiV_num.Unit=0;
	
	ScpiV_num1.Dot=0;
	ScpiV_num1.Num=0;
	ScpiV_num1.Unit=0;
	
	
	ScpiV_num.Unit=0;
	if(scpidot == 0)
	{
		scpinum[scpnum]='.';
		scpidot = scpnum;
		for(i=scpnum+1;i<7;i++)
		scpinum[i]='0';
	}else{
		for(i=scpnum;i<7;i++)
		scpinum[i]='0';
	}
	
	
	for(i=0;i<7;i++)
	{
		if(scpinum[0]>='0'&&(scpinum[0]<='9'))
		{
			if(scpinum[i]>='0'&&(scpinum[i]<='9'))
			{
			
				if(scpidot>i)
				{
					ScpiV_num.Num*=10;
					ScpiV_num.Num+=scpinum[i]-'0';
				
				}
				else
				{
					ScpiV_num.Num*=10;
					ScpiV_num.Num+=scpinum[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}			
	}
	ScpiV_num.Dot=scpidot;
	ScpiV_num1=Input_Set_Cov(&ScpiV_num);
	scpidot = 0;	
		
	return ScpiV_num1;

}

//电压设置
Sort_TypeDef SCPI_SET_V1(void)
{
	u8 i;
	static Sort_TypeDef ScpiV_num,ScpiV_num1;
	ScpiV_num.Dot=0;
	ScpiV_num.Num=0;
	ScpiV_num.Unit=0;
	
	ScpiV_num1.Dot=0;
	ScpiV_num1.Num=0;
	ScpiV_num1.Unit=0;
	
	
	ScpiV_num.Unit=0;
	if(scpidot1 == 0)
	{
		scpinum1[scpnum1]='.';
		scpidot1 = scpnum1;
		for(i=scpnum1+1;i<7;i++)
		scpinum1[i]='0';
	}else{
		for(i=scpnum1;i<7;i++)
		scpinum1[i]='0';
	}
	
	
	for(i=0;i<7;i++)
	{
		if(scpinum1[0]>='0'&&(scpinum1[0]<='9'))
		{
			if(scpinum1[i]>='0'&&(scpinum1[i]<='9'))
			{
			
				if(scpidot1>i)
				{
					ScpiV_num.Num*=10;
					ScpiV_num.Num+=scpinum1[i]-'0';
				
				}
				else
				{
					ScpiV_num.Num*=10;
					ScpiV_num.Num+=scpinum1[i]-'0';
				
				
				}
			}
			
			
			//Sort_set.Num+=(Disp_buff[key_count-1]-'0');
		
		
		}			
	}
	ScpiV_num.Dot=scpidot1;
	ScpiV_num1=Input_Set_Cov(&ScpiV_num);
	scpidot1 = 0;	
		
	return ScpiV_num1;

}
void Disp_Range(u8 hand,u8 range)
{//Range_Disp_Test
//    if(hand==1)
//        hand=0;
//    else
//        hand=1;
    if(hand>1)
        hand=1;
    if(range>6)
        range=6;
    Colour.black =LCD_COLOR_TEST_BACK;
    WriteString_16(SORTING_XDISP-20, SORTING_Y_DISP+30, Range_Disp_Test[hand][range],  0);


}

void Swap(uint32_t A[], uint16_t i, uint16_t j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}
//冒泡排序
void BubbleSort(uint32_t A[], uint16_t n)
{
	int i,j;
    for (j = 0; j < n - 1; j++)         // 每次最大元素就像气泡一样"浮"到数组的最后
    {
        for (i = 0; i < n - 1 - j; i++) // 依次比较相邻的两个元素,使较大的那个向后移
        {
            if (A[i] > A[i + 1])            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
            {
                Swap(A, i, i + 1);
            }
        }
    }
}

uint8_t R_Comp(void)
{
    float testvalue;
    float set_highvalue;
    float set_lowvalue;
    u8 flag;
    testvalue=Test_Value.res*pow(10,3*(Test_Value.uint))/pow(10,Test_Value.dot);
    set_highvalue=Jk516save.Set_Data.High_Res.Num*pow(10,3*(Jk516save.Set_Data.High_Res.Unit))/pow(10,Jk516save.Set_Data.High_Res.Dot);
    set_lowvalue=Jk516save.Set_Data.Res_low.Num*pow(10,3*(Jk516save.Set_Data.Res_low.Unit))/pow(10,Jk516save.Set_Data.Res_low.Dot);
    if(testvalue>set_highvalue)//大于上限
        flag=1;
    else if(testvalue<set_lowvalue)//小于下限
    {
        flag=2;
        
            
    
    
    }
    else
        flag=0;//合格
    return flag;



}
uint8_t V_Comp(void)
{
    u32 testvalue;
    u32 set_highvalue;
    u32 set_lowvalue;
    u8 flag;
    u8 dot=1;
    if(Test_Value_V.dot==3)
        dot=10;
    testvalue=Test_Value_V.res*dot;
    
    if(Jk516save.Set_Data.V_high.Dot==3)
        set_highvalue=Jk516save.Set_Data.V_high.Num*10;
    else if(Jk516save.Set_Data.V_high.Dot==5)
        set_highvalue=Jk516save.Set_Data.V_high.Num/10;
    else
        set_highvalue=Jk516save.Set_Data.V_high.Num;
        
    
    if(Jk516save.Set_Data.V_low.Dot==3)
        set_lowvalue=Jk516save.Set_Data.V_low.Num*10;
    else if(Jk516save.Set_Data.V_low.Dot==5)
        set_lowvalue=Jk516save.Set_Data.V_low.Num/10;
    else
        set_lowvalue=Jk516save.Set_Data.V_low.Num;
    
    if(testvalue>set_highvalue)//大于上限
        flag=1;
    else if(testvalue<set_lowvalue)//小于下限
        flag=2;
    else
        flag=0;
    return flag;



}
uint8_t Jisuan_Range(Sort_TypeDef date)//0  m  1    2 k
{
    uint8_t range;
    if(date.Dot==3&&date.Unit==0)
        range=0;
    if(date.Dot==2&&date.Unit==0)
        range=1;
    if(date.Dot==4&&date.Unit==1)
        range=2;
    if(date.Dot==3&&date.Unit==1)
        range=3;
    if(date.Dot==2&&date.Unit==1)
        range=4;
    if(date.Dot==4&&date.Unit==2)
        range=5;
    if(date.Dot==3&&date.Unit==2)
        range=6;
    
    return range;


}
uint8_t Jisuan_V_Range(Sort_TypeDef date)
{
    uint8_t range;
    if(date.Dot==3&&date .Num>120000)
        range=1;
    else
        range=0;


    return range;
}
void Disp_dateandtime(void)
{
    
    RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
    char LCDTemp[30];
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);//改1秒扫描一次
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    sprintf(LCDTemp,"20%0.2d-%0.2d-%0.2d %0.2d:%0.2d:%0.2d", 
    RTC_DateStructure.RTC_Year,
    RTC_DateStructure.RTC_Month, 
    RTC_DateStructure.RTC_Date,
    RTC_TimeStructure.RTC_Hours, 
    RTC_TimeStructure.RTC_Minutes,
    RTC_TimeStructure.RTC_Seconds);
    
    
    //);
    Colour.black =LCD_COLOR_TEST_BACK;
    WriteString_16(LIST2, LIST1+4, (const uint8_t *)LCDTemp,  0);
//    sprintf(LCDTemp,"The Time :  %0.2d:%0.2d:%0.2d", 
//    RTC_TimeStructure.RTC_Hours, 
//    RTC_TimeStructure.RTC_Minutes, 
//    RTC_TimeStructure.RTC_Seconds);
//    WriteString_16(SORTING_XDISP-20, LIST1+4, Range_Disp_Test[hand][range],  0);
    memcpy((void*)Send_To_U.No,LCDTemp,19);
    Send_To_U.No[19]=9;
		sprintf(bmpname,"0:20%0.2d%0.2d%0.2d%0.2d%0.2d%0.2d.bmp", 
    RTC_DateStructure.RTC_Year,
    RTC_DateStructure.RTC_Month, 
    RTC_DateStructure.RTC_Date,
    RTC_TimeStructure.RTC_Hours, 
    RTC_TimeStructure.RTC_Minutes,
    RTC_TimeStructure.RTC_Seconds);
}

void Disp_Debug_Reference( vu8 list,Test_ValueTypedef eee,Test_ValueTypedef ddd)
{
	if(list)
	if(list>DEBUG_RANGE-2)
	{
		
		//V_BCD_Int(eee);
        Hex_Format(eee.res,eee.dot,6,FALSE);
		
		
	}
		else
		{
			
			 Hex_Format(ddd.res,ddd.dot,7,FALSE);
			
			
		}
        WriteString_16(LIST1+160+160, FIRSTLINE+(SPACE1-2)*(list), DispBuf,  1);


}
const vu8 Debug_Dot[DEBUG_RANGE]={3,2,4,3,2,4,3,4,3};
//显示设置参数的值Setup_Valueall
void Disp_Debug_value(vu8 list)
{
//	vu32 flag;
	vu32 i;
//	vu32 xpose;
	const u8 (*pt)[7];
	vu32 Black_Select;
//	vu32 Select_color;
	if(calpage == 0)
	{
		for(i=0;i<DEBUG_RANGE;i++)
		{
			if(list==(i+1))
			{
				Colour.black=LCD_COLOR_SELECT;
			
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
		
			if(i < 10)
			{
				LCD_DrawFullRect( LIST1+160, FIRSTLINE+(SPACE1-2)*(i),90 , SPACE1-2 ) ;//SPACE1
				Hex_Format(DispValue.CalValue[i], 4 , 7 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
				WriteString_16(LIST1+160, FIRSTLINE+(SPACE1-2)*(i), DispBuf,  1);
			}else{
				LCD_DrawFullRect( LIST2+100, FIRSTLINE+(SPACE1-2)*(i-10),90 , SPACE1-2 ) ;//SPACE1
				Hex_Format(DispValue.CalValue[i], 4 , 7 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
				WriteString_16(LIST2+100, FIRSTLINE+(SPACE1-2)*(i-10), DispBuf,  1);
			}
		}
	}else if(calpage == 1){
		for(i=0;i<9;i++)
		{
			if(list==(i+1))
			{
				Colour.black=LCD_COLOR_SELECT;
			}
			else
			{
				Colour.black=LCD_COLOR_TEST_BACK;
			}
//			if(i < 10)
//			{
				LCD_DrawFullRect( LIST1+160, FIRSTLINE+(SPACE1-2)*(i),90 , SPACE1-2 ) ;//SPACE1
				Hex_Format(DispValue.CalValue[i+16], 4 , 7 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
				WriteString_16(LIST1+160, FIRSTLINE+(SPACE1-2)*(i), DispBuf,  1);
//			}else{
//				LCD_DrawFullRect( LIST2+100, FIRSTLINE+(SPACE1-2)*(i-10),90 , SPACE1-2 ) ;//SPACE1
//				Hex_Format(DispValue.CalValue[i], 4 , 7 , 0);//sprintf((char *)DispBuf,"%4",Save_Res;
//				WriteString_16(LIST2+100, FIRSTLINE+(SPACE1-2)*(i-10), DispBuf,  1);
//			}
		}
	}
	Colour.black=LCD_COLOR_TEST_BACK;
	
	Disp_Fastbutton();
	Colour.Fword=White;//
	Colour.black=LCD_COLOR_TEST_BUTON;
	pt=Calvalue;
	for(i=2;i<5;i++)
	{
		WriteString_16(BUTTOM_X_VALUE+i*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, pt[i],  0);
	}


}
//用户校正界面子函数
void Disp_Usercheck_Screen(void)
{
	Disp_TopBar_Color();
	Disp_Fastbutton();//显示快速按键
//	Disp_TestSet_MidRect();//显示中间色块
}
void Disp_UserCheck_Item(void)
{
	uint32_t i;
	Disp_Usercheck_Screen();
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BAR;
	WriteString_16(0, 4, All_TopName[5],  0);
	Colour.Fword=White;
	Colour.black=LCD_COLOR_TEST_BACK;//User_Check_main
	Hex_Format(LoadSave.ErrCnt[0],0,5,0);
	WriteString_16(200, 4,DispBuf ,  0);
	if(LoadSave.Version == 0)
	{
		WriteString_16(360, 4,"1200W" ,  0);
	}else if(LoadSave.Version == 1){
		WriteString_16(360, 4," 800W" ,  0);
	}else if(LoadSave.Version == 2){
		WriteString_16(360, 4," 600W" ,  0);
	}else if(LoadSave.Version == 3){
		WriteString_16(360, 4," 400W" ,  0);
	}else if(LoadSave.Version == 4){
		WriteString_16(360, 4,"2400W" ,  0);
	}else if(LoadSave.Version == 5){
		WriteString_16(360, 4,"3200W" ,  0);
	}
//	for(i=0;i<(sizeof(User_Check_main)/(sizeof(User_Check_main[0])));i++)
//	{
//		WriteString_16(LIST1+160*i, FIRSTLINE, User_Check_main[i],  0);

//	}
	
	if(calpage == 0)
	{
		for(i=0;i<10;i++)
		{
			WriteString_16(LIST1, FIRSTLINE+(SPACE1-2)*(i), User_Check_Item[i],  0);

		}
		
		for(i=0;i<6;i++)
		{
			WriteString_16(LIST2, FIRSTLINE+(SPACE1-2)*(i), User_Check_Item[i+10],  0);
		}
	}else if(calpage == 1){
		for(i=0;i<9;i++)
		{
			WriteString_16(LIST1, FIRSTLINE+(SPACE1-2)*(i), User_Check_Item1[i],  0);

		}
		
//		for(i=0;i<6;i++)
//		{
//			WriteString_16(LIST2, FIRSTLINE+(SPACE1-2)*(i), User_Check_Item[i+10],  0);
//		}
	}
//	else
//	{
//		WriteString_16(LIST2, FIRSTLINE+SPACE1*(i-sizeof(User_Check_Item)/(sizeof(User_Check_Item[0]))/2), User_Check_Item[i],  0);
//	}	
//	Disp_Button_TestSet(0);
}
int32_t Debug_Valuedata(Sort_TypeDef *Debug_value)
{
    if(Debug_value->Num>20000)
        Debug_value->Num/=10;
	return Debug_value->Num;//(pow(10,Debug_value->Dot));


}
vu32 Debug_Set_Num(Disp_Coordinates_Typedef *Coordinates)//
{
	Sort_TypeDef Sort_num;
//	vu8 i;
	vu32 num;
	Disp_button_Num_Freq();
	Sort_num=Disp_NumKeyboard_Set(Coordinates,1);
	num= Debug_Valuedata(&Sort_num);
	
	
		
	return num;
	
}
vu32 Debug_Set_Res(Disp_Coordinates_Typedef *Coordinates)
{
   Sort_TypeDef Sort_num;
    vu32 num; 
    Disp_button_Num_time();
    Sort_num=Disp_NumKeyboard_Set(Coordinates,0);
    num= Debug_Valuedata(&Sort_num);
    return num;
    

}
void Debug_stanedcomp(void)
{
    vu8 i;
   for(i=0;i<DEBUG_RANGE;i++)
    {
        if(Jk516save.Debug_Value[i].standard>Debug_Compvalue[i][1]||Jk516save.Debug_Value[i].standard<Debug_Compvalue[i][1])
        {
            if(i<DEBUG_RANGE-2)
            {
                
                 Jk516save.Debug_Value[i].standard=10000;
                    
                
            }
            else
            {
            
                Jk516save.Debug_Value[i].standard=60000;
            
            }
        }
		if(Jk516save.Debug_Value1[i].standard>Debug_Compvalue[i][1]||Jk516save.Debug_Value1[i].standard<Debug_Compvalue[i][1])
        {
            if(i<DEBUG_RANGE-2)
            {
                
                 Jk516save.Debug_Value1[i].standard=10000;
                    
                
            }
            else
            {
            
                Jk516save.Debug_Value1[i].standard=60000;
            
            }
        }
		if(Jk516save.Debug_Value2[i].standard>Debug_Compvalue[i][1]||Jk516save.Debug_Value2[i].standard<Debug_Compvalue[i][1])
        {
            if(i<DEBUG_RANGE-2)
            {
                
                 Jk516save.Debug_Value2[i].standard=10000;
                    
                
            }
            else
            {
            
                Jk516save.Debug_Value2[i].standard=60000;
            
            }
        }
        if(Jk516save.Debug_Value3[i].standard>Debug_Compvalue[i][1]||Jk516save.Debug_Value3[i].standard<Debug_Compvalue[i][1])
        {
            if(i<DEBUG_RANGE-2)
            {
                
                 Jk516save.Debug_Value3[i].standard=10000;
                    
                
            }
            else
            {
            
                Jk516save.Debug_Value3[i].standard=60000;
            
            }
        }
            
    
    }


}
//输入仪器编号
void input_num(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	uint8_t Disp_buff[12];
	uint8_t  key,i;
	char dispflag=1;

	char key_count=0;

	
	for(i=0;i<8;i++)
	Disp_buff[i]=' ';
	Disp_buff[8]=0;
    Disp_buff[9]=0;
    
	
	while(While_flag)
	{
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		
		if(key!=KEY_NONE)
		{
 //           password_flag=0;
           // Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 
                       //SetSystemStatus(SYS_STATUS_USERDEBUG);

					
				break;
				case Key_F2://取消
                    
					
				break;
				case Key_F3://确认保存机号和日期

					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
                   //return  &Disp_buff[0];
					
				break;

				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;

				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
                    if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='/';
						key_count++;
					}
					

				break;

				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            Colour.Fword=Red;
            LCD_DrawLine( Coordinates->xpos+13+80, Coordinates->ypos-2, 80, 0  );
            LCD_DrawLine( Coordinates->xpos+13+80, Coordinates->ypos+18, 80,0 );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2+20, 20, 1  );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
            WriteString_16(Coordinates->xpos-70, Coordinates->ypos, "出厂编号:",  0);
			WriteString_16(Coordinates->xpos+13, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
//            if(password_flag)
//                WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "密码错误",  0);
//            else
//               LCD_DrawRect( Coordinates->xpos, Coordinates->ypos+20,Coordinates->xpos+100 , Coordinates->ypos+40, LCD_COLOR_TEST_BACK );
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
            WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位出厂编号",  0);
		}
	
	}

}
void input_password(Disp_Coordinates_Typedef *Coordinates )
{
  	char While_flag=1;
	char Disp_buff[10];
	u8 key,i;
	char dispflag=1;
	char dot_num=0,dot_num1=0;
    char password_flag=0;
	char  key_count=0;
	for(i=0;i<9;i++)
	Disp_buff[i]=0;
	Disp_Button_value1(2);
	while(While_flag)
	{
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		if(key!=KEY_NONE)
		{
            //Key_Beep()
            password_flag=0;
//            Key_Beep();
			dispflag=1;
			switch(key)
			{
				case Key_F1://退出
					 While_flag=0;
                       SetSystemStatus(SYS_STATUS_TEST);

					
				break;
				case Key_F3://取消
                    
					
				break;
				case Key_F2://确认
                   if(strcmp(PASSWORD,Disp_buff))//比较函数  当相等时  返回0
                   {//密码错误
                       key_count=0;
                       for(i=0;i<8;i++)
                        Disp_buff[i]=' ';
                       password_flag=1;
                       
                   
                   
                   }
                   else //密码正确
                   {
                        While_flag=0;
                       SetSystemStatus(SYS_STATUS_DEBUG);
                   
                   }
                       
					
				break;
				case Key_F4:
					
				break;
				case Key_F5:
					
				break;

				case Key_LEFT:
				break;
				case Key_RIGHT:
				break;

				case Key_NUM1:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='1';
						
						
							
						
						key_count++;
							
					}
				break;
				case Key_NUM2:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='2';
						key_count++;
					}
				break;
				case Key_NUM3:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='3';
						key_count++;
					}
				break;
				case Key_NUM4:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='4';
						key_count++;
					}
					
				break;
				case Key_NUM5:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='5';
						key_count++;
					}
				break;
				case Key_NUM6:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='6';
						key_count++;
					}
				break;
				case Key_NUM7:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='7';
						key_count++;
					}
				break;
				case Key_NUM8:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='8';
						key_count++;
					}
				break;
				case Key_NUM9:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='9';
						key_count++;
					}
				break;
				case Key_NUM0:
					if(key_count<PASSWORD_LENTH)
					{
						Disp_buff[key_count]='0';
						key_count++;
					}
				break;
				case Key_DOT:
					
					if(key_count<PASSWORD_LENTH&&key_count>0)
					{
						if(dot_num==0)
						{
							if(key_count>0)
							{
								Disp_buff[key_count]='.';
								dot_num1=key_count;
								key_count++;
							
							
							}
							dot_num++;
						}
					}
//					else 
//					{
//						if(Disp_buff[key_count]==0)
//							Disp_buff[key_count]='-';
//						else if(Disp_buff[key_count]=='-')
//							Disp_buff[key_count]='+';
//						else
//							Disp_buff[key_count]='-';
//						key_count++;
//							
//					
//					
//					
//					
//					}
				break;
				case Key_ESC:
				{
					if(key_count>0)
					{	key_count--;
						Disp_buff[key_count]=' ';
						if(dot_num1==key_count)
						{
							dot_num=0;
							dot_num1=0;
							
						}
					
					}
				}break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
		if(dispflag)
		{
			dispflag=0;
            Colour.Fword=Red;
            LCD_DrawLine( Coordinates->xpos+13+80, Coordinates->ypos-2, 80, 0);
            LCD_DrawLine( Coordinates->xpos+13+80, Coordinates->ypos+18, 80, 0  );
            for(i=0;i<9;i++)
            LCD_DrawLine( Coordinates->xpos+13+i*10, Coordinates->ypos-2+20, 20, 1 );
            
			//LCD_DrawRect( Coordinates->xpos, Coordinates->ypos,Coordinates->xpos+Coordinates->lenth , Coordinates->ypos+16 , Red );
			Colour.Fword=White;
			Colour.black=LCD_COLOR_TEST_BACK;
            WriteString_16(Coordinates->xpos-38, Coordinates->ypos, "密码:",  0);
			WriteString_16(Coordinates->xpos+13, Coordinates->ypos, ( uint8_t *)Disp_buff,  0);
            if(password_flag)
                WriteString_16(Coordinates->xpos, Coordinates->ypos+20, "密码错误",  0);
            else
               LCD_DrawFullRect( Coordinates->xpos, Coordinates->ypos+20,100 ,40);
               // WriteString_16(Coordinates->xpos, Coordinates->ypos, "         ",  0);
            WriteString_16(Coordinates->xpos, Coordinates->ypos-20, "请输入8位密码",  0);
			//dispflag=0;
		}
	
	}

}

void lvbo_pingjun(void)
{
    uint16_t i;
    Ad_value=0;
    V_ad=0;
    I_ad=0;
    BubbleSort(scan_V, fit_allnum[Jk516save.Set_Data.speed]);
    for(i=fit_allnum[Jk516save.Set_Data.speed]/4;
    i<fit_allnum[Jk516save.Set_Data.speed]-fit_allnum[Jk516save.Set_Data.speed]/4;i++)
    {
        if(scan_V[i]>0x800000)
        {
            scan_V[i]=0xffffff-scan_V[i];
            V_ad-=scan_V[i];
        }
        else
        {
            V_ad+=scan_V[i];
        
        }
        
    }
    V_ad/=fit_allnum[Jk516save.Set_Data.speed]/2;
    BubbleSort(scan_I, fit_allnum[Jk516save.Set_Data.speed]);
    for(i=fit_allnum[Jk516save.Set_Data.speed]/4;
    i<fit_allnum[Jk516save.Set_Data.speed]-fit_allnum[Jk516save.Set_Data.speed]/4;i++)
    {
        if(scan_I[i]>0x800000)
        {
            scan_I[i]=0xffffff-scan_I[i];
            I_ad-=scan_I[i];
            
        }
        else
        {
            I_ad+=scan_I[i];
        
        }
        
    }
    I_ad/=fit_allnum[Jk516save.Set_Data.speed]/2;
                    
                   

}



//(x-100)/0.385  Pt100的计算公式  X是电阻值
/********************************
函数名称: void DispF(float f)
功能 ：用科学记数法显示浮点数，在float全范围内精确显示，超出范围给出提示。
说明 ：浮点数表示范围为+-1.175494E-38到+-3.402823E+38
入口参数：f为要显示的浮点数
返回值 ：无
********************************/

//（2）显示十进制尾数和阶的子函数。
/********************************
函数名称: void PrintW(unsigned long w,char j)
功能 ：科学记数法，显示十进制尾数和阶码
入口参数：w为尾数，j为阶码
返回值 ：无
********************************/

//==========================================================
//END
//==========================================================




