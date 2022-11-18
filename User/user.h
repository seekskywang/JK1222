//==========================================================
//�ļ����ƣ�User.h
//�����������û���������ͷ�ļ�
//�ļ��汾��Ver1.0
//�������ڣ�2014.12.15
//�޸����ڣ�2015.08.20 11:03
//�ļ���ע����
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
//��������ʾλ�ö���
#define SETUP_ADDR_X		(6)
#define SETUP_ADDR_Y		(27)
#define HEIGHT_OF_SETUP		(20)
#define SORTING_Y_DISP  180
#define SORTING_XDISP   400


#define TESTVALUE_X 64
#define TESTVALUE_Y 92  //�������ݵ�Y����ֵ
#define TEST_VALUE_YMID 55

#define TESTVALUE_X_UINT TESTVALUE_X+72
#define TESTVALUE_Y_UINT TESTVALUE_Y  //��λ

//��˸���
#define CURSORCHAR (' ')

////�˵������ı���󳤶�(12���ַ�)
//#define MENU_MAX_TEXT_LEN (12)
//
////ÿҳ�˵������ֵ
////#define MENU_LIST_MAX_PAGE_INDEX (10)
//#define MENU_LIST_MAX_PAGE_INDEX (12)
////X��˵�����
//#define MENU_LIST_X_INDEX (2)
////Y��˵�����
////#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX-1)
//#define MENU_LIST_Y_INDEX (MENU_LIST_MAX_PAGE_INDEX/MENU_LIST_X_INDEX)
//
////ϵͳ����,Y��˵�����
//#define SYSTEM_MENU_LIST_Y_INDEX (MENU_LIST_Y_INDEX-2)

//����λ��˸ʱ�䣬1000/3=333mS
#define BLINK_TICKS_SOFTTIMER (TICKS_PER_SEC_SOFTTIMER/3)

//==========================================================
//���ӡ�����
//extern const u16 POW_NUM[4];

////==========================================================
////��������ֵ�ϡ����ޡ�С����
//extern const u16 SetupLimit[][8][2];

////==========================================================
////��������ֵС����λ
//extern const u8 ParameterDotTab[][PARAMETER_MENU_MAX];

////==========================================================
////���
//extern const u8 NumTab[][3];

////==========================================================
////��Ŀ
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

//��ֵ�����Զ���
extern Test_ValueTypedef Test_Value;
extern Test_ValueTypedef Test_Value_V;
//�����б�
void Hex_Format(u32 dat , u8 dot , u8 len, u8 dispzero);//��ʽ��Hex��Bcd�ַ�����DispBuf
void InitGlobalValue(void);
void Parameter_valuecomp(void);
u16 Da_out(u16 v_value,u8 V_range);
void MissConDisp(void);
void Test_Disp(u8 vsw,u8 isw);
void Dynamic_Disp(u8 vsw,u8 isw);
void Bat_Disp(u8 vsw,u8 isw);
void Led_Disp(u8 vsw,u8 isw);
void Soft_Turnon(void);
//u8 Resistance_Setup(NumBox_TypeDef * pt);
//u8 ListBox_Setup(NumBox_TypeDef * pt);
//u8 Number_Setup(NumBox_TypeDef * pt);//��ֵ������
//u8 ListBox_Setup(NumBox_TypeDef * pt);//�б������

////void Disp_Kjjm( void );//��ʾ��������
//void Disp_PowerOn_Menu(void);//��ʾ��������

//void Disp_Group(u8 highlight);//��ʾ���
//void Disp_Frame(void);//��ʾ������

//void Disp_Idle_List(void);//��ʾ�����˵��б�
//void Disp_Idle_Info(void);//��ʾ������Ϣ

//void Step_Setup(void);//�������ô���
//void Disp_Step_Menu(void);//��ʾ����˵�
//void Disp_Step_Info(void);//��ʾ������Ϣ

//void Parameter_Setup(void);//�������ô���
//void Disp_Parameter_Menu(void);//��ʾ��������
//void Disp_Parameter_Info(u8 index);//��ʾ������Ϣ
//void Disp_Parameter_Value(u8 step ,u8 index, u8 highlight);//��ʾ�ڼ����ڼ������ֵ
//void Check_Parameter_Limit(u8 step);//����ֵ���
//void Check_Setup_Limit(void);//4������Ĳ���ֵ���

//void System_Setup(void);//ϵͳ���ô���
//void Disp_System_Menu(void);//��ʾϵͳ����
//void Disp_System_Info(u8 index);//��ʾϵͳ��Ϣ
//void Disp_System_Value(u8 index);//��ʾ�ڼ���ϵͳֵ
//void Check_System_Limit(void);//ϵͳֵ���

//void FactoryInfo_Setup(void);//������Ϣ���ô���
//void Disp_Factory_Info(void);//��ʾ������Ϣ
//void Disp_Software_Version(void);//��ʾ����汾
//void Disp_Hardware_Version(void);//��ʾӲ���汾
//void Disp_Boot_Number(void);//��ʾ��������
//void Disp_Compile_Date(void);//��ʾ��������

//void Disp_Instrument_Info(void);//��ʾ������Ϣ
//void Disp_Chip_ID(void);//��ʾоƬID��
//void Disp_Product_ID(void);//��ʾ��Ʒ���	
//void Disp_Product_Model(void);//��ʾ��Ʒ�ͺ�
//void Disp_Company_Name(void);//��ʾ��������
//void Disp_Company_Telephone(void);//��ʾ���̵绰
//void Disp_Company_Http(void);//��ʾ������ҳ

//void Config_Setup(void);//�������ô���
//void Disp_Config_Menu(void);//��ʾ���ý���
//void Disp_Config_Value(u8 index);//��ʾ����ֵ
//void Check_Config_Limit(void);//����ֵ���

//void Password_Setup(void);//��������
//Bool Password_Check(void);//����У��,TRUE��Ч

////==========================================================
////void Disp_SysStatus(void);//��ʾϵͳ״̬
//void Disp_SysMessage(void);//��ʾϵͳ��Ϣ
void Disp_Test_Item(void);
void Disp_Led_Item(void);
void Disp_Battery_Item(u8 type);
void Disp_Dynamic_Item(void);
void Disp_List_Item(void);
void Disp_Button_value1(uint32_t value);

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
void Disp_Res_Sheet(u8 num,u8 page);
void Disp_Up_Item(void);

void Use_SysSetProcess(void);

void Sys_Process(void);
vu32 Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_Step(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_TimeS(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_TimeMS(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_TimeP(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_List_Delay(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_CNum(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_VNum(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_RNum(Disp_Coordinates_Typedef *Coordinates);
vu32 Disp_Set_Addr(Disp_Coordinates_Typedef *Coordinates);

void DoubHex_Format(u32 dat , u8 Dot , u8 len , u8 dispzero);
Sort_TypeDef Disp_Set_CompNum(Disp_Coordinates_Typedef *Coordinates);
void BubbleSort(uint32_t A[], uint16_t n);//

void Disp_dateandtime(void);
void Disp_UserCheck_Item(void);
void Disp_Debug_value(vu8 list);
vu32 Debug_Set_Num(Disp_Coordinates_Typedef *Coordinates);//Ƶ������
void Debug_stanedcomp(void);
void Disp_Debug_Reference( vu8 list,Test_ValueTypedef eee,Test_ValueTypedef ddd);
vu32 Debug_Set_Res(Disp_Coordinates_Typedef *Coordinates);
void input_num(Disp_Coordinates_Typedef *Coordinates );
void input_password(Disp_Coordinates_Typedef *Coordinates );


//void V_Range_Control(u8 dat);

//Sort_TypeDef Disp_Set_Num(Disp_Coordinates_Typedef *Coordinates);

//==========================================================


#endif

//==========================================================
//END
//==========================================================
