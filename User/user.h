//==========================================================
//文件名称：User.h
//功能描述：用户操作程序头文件
//文件版本：Ver1.0
//创建日期：2014.12.15
//修改日期：2015.08.20 11:03
//文件备注：无
//==========================================================
#ifndef __User_H__
#define __User_H__
#include "pbdata.h"
//==========================================================

#define FIRSTLINE	26
#define	SPACE1		22
#define LIST1		0
#define LIST2		250
#define SELECT_1END 192
#define SELECT_2END (410+32)
//设置项显示位置定义
#define SETUP_ADDR_X		(6)
#define SETUP_ADDR_Y		(27)
#define HEIGHT_OF_SETUP		(20)
#define SORTING_Y_DISP  180
#define SORTING_XDISP   400


#define TESTVALUE_X 64
#define TESTVALUE_Y 92  //测试数据的Y坐标值
#define TEST_VALUE_YMID 55

#define TESTVALUE_X_UINT TESTVALUE_X+72
#define TESTVALUE_Y_UINT TESTVALUE_Y  //单位

//闪烁光标
#define CURSORCHAR (' ')

////菜单标题文本最大长度(12个字符)
//#define MENU_MAX_TEXT_LEN (12)
//
////每页菜单项最大值
////#define MENU_LIST_MAX_PAGE_INDEX (10)
//#define MENU_LIST_MAX_PAGE_INDEX (12)
////X轴菜单栏数
//#define MENU_LIST_X_INDEX (2)
////Y轴菜单项数
////#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX-1)
//#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX)
//
////系统设置,Y轴菜单项数
//#define SYSTEM_MENU_LIST_Y_INDEX (MENU_LIST_Y_INDEX-2)

//设置位闪烁时间，1000/3=333mS
#define BLINK_TICKS_SOFTTIMER (TICKS_PER_SEC_SOFTTIMER/3)

//==========================================================
//连加、连减
//extern const u16 POW_NUM[4];

////==========================================================
////参数设置值上、下限、小数点
//extern const u16 SetupLimit[][8][2];

////==========================================================
////参数设置值小数点位
//extern const u8 ParameterDotTab[][PARAMETER_MENU_MAX];

////==========================================================
////编号
//extern const u8 NumTab[][3];

////==========================================================
////项目
//extern const u8 ItemTab[][9];
//extern const u8 ItemCharTab[][3];

//==========================================================

typedef struct
{
    u32 res;
    u8 dot;
    u8 uint;
    u8 polar;
    

} Test_ValueTypedef;
typedef struct 
{
	vu8 sign;
	vu32 Num;
	vu32 Dot;
	vu32 Unit;
	vu32 Updata_flag;
	vu32 Order;
	vu32 mid_data;
}Sort_TypeDef;
typedef struct
{
	vu16 xpos;
	vu16 ypos;
	vu16 lenth;


}Disp_Coordinates_Typedef;

//数值框属性定义
extern Test_ValueTypedef Test_Value;
extern Test_ValueTypedef Test_Value_V;
//函数列表
void Hex_Format(u32 dat , u8 dot , u8 len, u8 dispzero);//格式化Hex到Bcd字符串到DispBuf
void Disp_SysMessage(void);
void Disp_PowerOn_Menu(void);
void Check_Parameter_Limit(void);

void Parameter_Setup(void);
void Disp_Setup_Menu(u8 index);
void System_Setup(void);
void Read_Ad(void);
void Range_Changecomp(void);
void VRange_Changecomp(void);
void Get_FFT(void);
void Test_Debug(void);
void _4094_data(void);
void InitGlobalValue(void);
void Parameter_valuecomp(void);
u16 Da_out(u16 v_value,u8 V_range);
void DispF(float f);
void Get_Res(void);
void Comp_test(u16 comp_data);
Test_ValueTypedef Datacov(s32 value,u8 range);
Test_ValueTypedef V_Datacov(s32 value ,u8 range);
void Uart_Process(void);
void Disp_Testvalue(Test_ValueTypedef value,Test_ValueTypedef value_v,u8 speed);
void Test_Disp(u8 vsw,u8 isw);
void Dynamic_Disp(u8 vsw,u8 isw);
void Bat_Disp(u8 vsw,u8 isw);
u8 Comp_choice(void);
void Beep_Out(u8 flag);
void lvbo_jisu(void);
void Soft_Turnon(void);
//u8 Resistance_Setup(NumBox_TypeDef * pt);
//u8 ListBox_Setup(NumBox_TypeDef * pt);
//u8 Number_Setup(NumBox_TypeDef * pt);//数值框设置
//u8 ListBox_Setup(NumBox_TypeDef * pt);//列表框设置

////void Disp_Kjjm( void );//显示开机界面
//void Disp_PowerOn_Menu(void);//显示开机界面

//void Disp_Group(u8 highlight);//显示组别
//void Disp_Frame(void);//显示界面框架

//void Disp_Idle_List(void);//显示待机菜单列表
void Disp_Idle_Menu(void);//显示待机界面
//void Disp_Idle_Info(void);//显示待机信息

//void Step_Setup(void);//步骤设置处理
//void Disp_Step_Menu(void);//显示步骤菜单
//void Disp_Step_Info(void);//显示步骤信息

//void Parameter_Setup(void);//参数设置处理
//void Disp_Parameter_Menu(void);//显示参数界面
//void Disp_Parameter_Info(u8 index);//显示参数信息
//void Disp_Parameter_Value(u8 step ,u8 index, u8 highlight);//显示第几步第几项参数值
//void Check_Parameter_Limit(u8 step);//参数值检查
//void Check_Setup_Limit(void);//4个步骤的参数值检查

//void System_Setup(void);//系统设置处理
//void Disp_System_Menu(void);//显示系统界面
//void Disp_System_Info(u8 index);//显示系统信息
//void Disp_System_Value(u8 index);//显示第几项系统值
//void Check_System_Limit(void);//系统值检查

//void FactoryInfo_Setup(void);//出厂信息设置处理
//void Disp_Factory_Info(void);//显示出厂信息
//void Disp_Software_Version(void);//显示软件版本
//void Disp_Hardware_Version(void);//显示硬件版本
//void Disp_Boot_Number(void);//显示开机次数
//void Disp_Compile_Date(void);//显示编译日期

//void Disp_Instrument_Info(void);//显示仪器信息
//void Disp_Chip_ID(void);//显示芯片ID号
//void Disp_Product_ID(void);//显示产品编号	
//void Disp_Product_Model(void);//显示产品型号
//void Disp_Company_Name(void);//显示厂商名称
//void Disp_Company_Telephone(void);//显示厂商电话
//void Disp_Company_Http(void);//显示厂商主页

//void Config_Setup(void);//配置设置处理
//void Disp_Config_Menu(void);//显示配置界面
//void Disp_Config_Value(u8 index);//显示配置值
//void Check_Config_Limit(void);//配置值检查

//void Password_Setup(void);//密码设置
//Bool Password_Check(void);//密码校验,TRUE有效

////==========================================================
////void Disp_SysStatus(void);//显示系统状态
//void Disp_SysMessage(void);//显示系统信息
void Disp_xxx(u16 data,u16 pos);
void Disp_Test_Item(void);
void Disp_Led_Item(void);
void Disp_Battery_Item(u8 type);
void Disp_Dynamic_Item(void);
void Disp_List_Item(void);

void Disp_Test_value(u8 num);
void Disp_Led_value(u8 num);
void Disp_Bat_value(u8 num);
void Disp_Dyn_value(u8 num);
void Disp_List_value(u8 num);
void Disp_List_Process(void);

void Disp_Fastbutton(void);
void Disp_Test_Set_Item(void);
void Disp_Limit_Item(void);
void DispSet_value(u8 keynum);
void DispLimit_value(u8 keynum);
void Disp_Res_Sheet(u8 num);

void Use_SysSetProcess(void);

void Sys_Process(void);
vu32 Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_Step(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_TimeS(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_TimeP(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_List_Delay(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_CNum(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_VNum(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_RNum(Disp_Coordinates_Typedef *Coordinates);

void DoubHex_Format(u32 dat , u8 Dot , u8 len , u8 dispzero);
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates);
void Disp_Range(u8 hand,u8 range);
void Disp_Open(void);
void BubbleSort(uint32_t A[], uint16_t n);//

uint8_t R_Comp(void);//电阻比较
uint8_t V_Comp(void);//电压比较
void Disp_dateandtime(void);
uint8_t Jisuan_Range(Sort_TypeDef date);
uint8_t Jisuan_V_Range(Sort_TypeDef date);
void Disp_UserCheck_Item(void);
void Disp_Debug_value(vu8 list);
vu32 Debug_Set_Num(Disp_Coordinates_Typedef *Coordinates);//频率设置
void Debug_stanedcomp(void);
void Disp_Debug_Reference( vu8 list,Test_ValueTypedef eee,Test_ValueTypedef ddd);
vu32 Debug_Set_Res(Disp_Coordinates_Typedef *Coordinates);
void input_num(Disp_Coordinates_Typedef *Coordinates );
void input_password(Disp_Coordinates_Typedef *Coordinates );
void AD_ads2516(uint16_t i);
void lvbo_pingjun(void);
//void V_Range_Control(u8 dat);
void read_adV_1(void);
void read_adI_1(void);
void read_adV_2(void);
void read_adI_2(void);
void read_adV_3(void);
void read_adI_3(void);
void read_adV_4(void);
void read_adI_4(void);
//Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);

//==========================================================


#endif

//==========================================================
//END
//==========================================================
