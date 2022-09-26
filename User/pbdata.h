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





//���庯��

void RCC_HSE_Configuration(void);
void delay(u32 nCount);
//void delay_us(u32 nus);
//void delay_ms(u16 nms);
void SetSoftTimer(u8 id, u8 ticks);
u8 GetSoftTimerOut(u8 id);
void Para_Set_Comp(void);



#define PASSWORD  "20185260"

#define BMP_SWITCH  0      //��ͼ���ܿ���

//==========================================================

#define MAXTSOFTTIMER (2)		//��ʱ����Ŀ


//#define BASTARDLY  //�������Ժ� Ҫ�������������ֲű仯��
//==========================================================
//����汾��10
#define SOFTWARE_VERSION		(0x10)

//==========================================================
//Ĭ�ϳ������(0)
#define DEFAULT_MACHINE_ID 		(0)

//Ĭ�ϳ�������(0)
#define DEFAULT_PASSWORD 		(0)

//Ĭ�Ͽ�������(0)
#define SET_BOOT_DEFAULT		(0)

#define CHANNEL_NUM             (15)

//ADֵ�˲����ã����������ȶ���
#define AD_FILTER_SUPPORT		(_DEBUG_CONFIG_TRUE)

//==========================================================
#define SETUP_MENU_MAX  	(2)//������ò˵�

//#define PARAMETER_MENU_MAX	(5)//�������˵�

//#define SYSTEM_MENU_MAX		(2)//���ϵͳ�˵�

#define CONFIG_MENU_MAX		(3)//���������(0-2)

#define PW_LENGTH 			(4)//�������λ��

#define RANGE_MAX 			(6)//�������

#define RANGE_V_MAX         (1)

#define AD_BUF_LENGTH		(8)//A/Dֵ����������

#define KEY_NUM				(3)	//����������

#define SYS_MAX				(12)

#define PASSWORD_LENTH  8

#define NUM_LENTH 			(6)//��ʾ����
#define NUM_LENTH_V 			(8)//��ʾ����
#define MAX_DOTNUM          (5)//���С����

//==========================================================


#define DEBUG_RANGE 16


//��ʱ������
#define KEYBOARD_SOFTTIMER	(0) //������ʱ��ʱ��
#define DISP_SOFTTIMER 		(1) //��ʾ��ʱ��ʱ��

//==========================================================
//���ⳤ��
#define LIST_TITLE_LEN		(8)
#define TITLE_LEN_MAX		(8)
#define SetSystemStatus(status) (SystemStatus=status)
#define GetSystemStatus() (SystemStatus)
#define GetSystemMessage() (SystemMessage)
#define SetSystemMessage(msg) (SystemMessage=msg)


#define		MAX_SET_CURRENT       600000    //60.0000A  ������õ��� (�͵� 2.00000A  �ߵ� 20.0000)
#define		MAX_SET_VOLT          1200000    //120.000V  ������õ�ѹ (�͵� 15.0000V  �ߵ� 150.000)
//#ifdef LOADVER1200
//#define		powermax           12000000    //1200.0000W  �����
//#elif define LOADVER800
//#define		powermax           8000000    //800.0000W  �����
//#elif define LOADVER600
//#define		powermax           6000000    //600.0000W  �����
//#endif
#define		MAX_SET_RES           2000000   //200000.0R  ������


#define		MAX_CURRENT    				20000			//������ ( 2A )��ͬ��λ����С���㲻ͬ ( 0- 2.0000, 0-20.000)
#define		MIN_CURRENT    				1000			//��С����  �����жϵ�λ����


#define		MAX_CURRENTUP    			30000			//������������
#define		MIN_CURRENTUP   			6			    //��С����������
#define		MAX_CURRENTDOWN    		30000			//�������½���
#define		MIN_CURRENTDOWN  			6			    //��С�����½���
#define		MAX_CURRENTPOOTE  		210000		//����������ֵ ��21.0000��

#define		MAX_POWERPOOTE  		 	15000			//����ʱ���ֵ ��150.00��

#define		MAX_VOLOTE    				150000		//����ѹ  (0-15.0000�� 0-150.000)
#define		MIN_VOLOTE    				100			  //��С��ѹ

#define		MAX_LoadVOLOTE    		1200000		//���ʼ���ص�ѹֵ 150.000
#define		MIN_LoadVOLOTE    		0			    //��С��ʼ���ص�ѹֵ

#define		MAX_FreeLoadVOLOTE    1200000		//���ʼж�ص�ѹֵ
#define		MIN_FreeLoadVOLOTE    0			    //��С��ʼж�ص�ѹֵ


#define		DYN_HIGH_MAX          210000    //��̬ģʽ��׼λ���ص��� 21.0000  MAX
#define		DYN_HIGH_MIN          0         //��̬ģʽ��׼λ���ص��� 0        MIN
#define		DYN_LOW_MAX           210000    //��̬ģʽ��׼λ���ص��� 21.0000  MAX
#define		DYN_LOW_MIN           0         //��̬ģʽ��׼λ���ص��� 0        MIN


#define		DYN_HIGH_DELLMAX      50000000    //��̬ģʽ��׼λ���ص���ʱ�� 50.00000S  MAX  >10s���һλ��Ч
#define		DYN_HIGH_DELLMIN      10         //��̬ģʽ��׼λ���ص���ʱ�� 0.000010S  MIN

#define		DYN_LOW_DELLMAX      50000000    //��̬ģʽ��׼λ���ص���ʱ�� 50.00000S  MAX >10s���һλ��Ч
#define		DYN_LOW_DELLMIN      10         //��̬ģʽ��׼λ���ص���ʱ�� 0.000010S  MIN

#define		DYN_SLEW_RISEMAX     30000      //��̬ģʽ���������� MAX  3.0000
#define		DYN_SLEW_RISEMIN     6          //��̬ģʽ���������� MAX  0.0006

#define		DYN_SLEW_FALLMAX     30000      //��̬ģʽ�����½��� MAX  3.0000
#define		DYN_SLEW_FALLMIN     6          //��̬ģʽ�����½��� MAX  0.0006

//��ʾˢ���˲�����
#define DISP_FILTER		10

#define STARTDELAY		80

#define SWITCH_DELAY          (3)//����֮��Ķ�ȡ��ʱ
//�������
extern uint8_t softswitch;
extern vu8 SystemStatus;//ϵͳ״̬
extern vu8 SystemMessage;//ϵͳ��Ϣ
extern u32  g_LcdDirection;
extern u8 Int_Pe3flag;
extern u8 buttonpage1;
extern u8 calpage;
extern FRESULT result;
extern FATFS fs;
extern FIL file;
extern u8 bmpname[30];
//==========================================================
//ϵͳ״̬
enum SysStatusEnum
{	
	SYS_STATUS_ERROR ,			//�쳣
	SYS_STATUS_RESET ,			//��λ
	SYS_STATUS_POWER ,			//����
//	SYS_STATUS_POWER ,
	SYS_STATUS_DATASTORE ,		//���ݱ���
	SYS_STATUS_FILE ,			//�ļ�����
	SYS_STATUS_SETUPTEST ,			//��������
	SYS_STATUS_TEST ,			//����
	SYS_STATUS_RANGE ,		//������ʾ
	SYS_STATUS_RANGECOUNT,	//��������ʾ
	SYS_STATUS_ITEM ,			//�б�ɨ����ʾ
	SYS_STATUS_USERDEBUG,//�û�У�� 
	SYS_STATUS_FACRDEBUG, //����У��
	SYS_STATUS_SYSSET,     //ϵͳ����
    SYS_STATUS_LIMITSET,     //��������
    SYS_STATUS_ITEMSET,//�б�ɨ������
	SYS_STATUS_TOOL,//����
	SYS_STATUS_SYS,
    SYS_STATUS_CLEAR,
    SYS_STATUS_SETUP,
    SYS_STATUS_DEBUG,
	SYS_STATUS_LED,
	SYS_STATUS_BATTERY,
	SYS_STATUS_DYNAMIC,
	SYS_STATUS_LIST,		//�б����
	SYS_STATUS_LOCK
};

//==========================================================




  

typedef struct
{
	vu8 sence;//Զ�˲���
	vu8 mode;//ģʽ
	vu32 voltage;//���õ�ѹ
	vu32 current;//���õ���
	vu32 risistence;//���õ���
	vu32 power;//���ù���
	vu32 ovp;//����ѹ����
	vu32 ocp;//����������
	vu32 opp;//�����ʱ���
//	vu8 remote;//Զ�˲���
	vu8 crange;//��������
	vu8 vrange;//��ѹ����
	vu32 onvol;//������ѹ
	vu32 offvol;//�ضϵ�ѹ
	vu8 crmode;//����ģʽ
	vu32 autooff;//��ʱ�ض�
	vu32 maxc;//������
	vu32 maxv;//����ѹ
	vu32 maxp;//�����
	vu32 crise;//����������
	vu32 cdrop;//�����½���
	vu32 cvdowntime; //CVģʽ��ѹ�½�ʱ��
	
	vu32 ledvo;//ledvo
	vu32 ledio;//ledio
	vu32 ledrd;//ledrd
	
	
//��ز���
	vu8 loadmode;//��ز��Էŵ�ģʽ
	vu32 loadc1;//�ŵ����1
	vu32 loadc2;//�ŵ����2
	vu32 loadc3;//�ŵ����3
	vu8 curvetime;//���߲������
	vu32 coffv1;//��ֹ��ѹ1
	vu32 coffv2;//��ֹ��ѹ2
	vu32 coffv3;//��ֹ��ѹ3
	vu32 loadr;//�ŵ����
	vu32 coffvr;//����ģʽ��ֹ��ѹ
//��̬����
	vu8 testdis;//������ʾ
	vu32 valA;//��̬ģʽ���ص���A
	vu32 valB;//��̬ģʽ���ص���B
	vu32 timeA;//��̬ģʽ���س���ʱ��
	vu8 dynamode;//��̬ģʽ����ģʽѡ��
	vu32 dynaIRise;//��̬ģʽ����������
	vu32 dynaIDrop;//��̬ģʽ�����½���
	vu32 timeB;//��̬ģʽ���س���ʱ��B
	
	vu8 qcmode;//���ģʽѡ�񿪹�
	vu32 Class_5;//5V  
	vu32 Class_9;//9V
	vu32 Class_12;//12V
	vu32 Class_15;//15V
	vu32 Class_20;//20V
//��������
	u32 vhigh;//��ѹ����
	u32 vlow;//��ѹ����
	u32 chigh;//��������
	u32 clow;//��������
	u32 phigh;//��������
	u32 plow;//��������
	u8 limitdisp;//������ʾ
	u8 limitbeep;//����Ѷ��
	u8 vcomp;//��ѹ�ж�
	u8 ccomp;//�����ж�
	u8 pcomp;//�����ж�
//ϵͳ����
	u8 language;
//�б����
	u8 ListNum;//�б���
	u8 StepMode;//����ģʽ
	u8 LoopTest;//ѭ������
	u8 ListBeep;//�б�Ѷ��
	vu32 listonvol;//������ѹ
	vu32 gatev;//�ż���ѹ
	vu8 listmode[15];//�б���ģʽ
	vu32 listvalue[15];//�б���ֵ
	vu16 delay[15];//�б�����ʱ
	vu8 listcomp[15];//�б��رȽ�����
	vu32 listhigh[15];//�б�������
	vu32 listlow[15];//�б�������
//ϵͳ����
	u16 Baudrate;//������
	u16 Addr;//������ַ
//���Ӽ�¼
	u16 ErrCnt[4];
//�汾����
	u8 Version;//0-1200W;1-800W;2-600W;
//ͨѶ�ӿ�
	u8 COMM;//0-232;1-485;
//logo
	u8 jkflag;
//����ģʽ 0-���� 1-�ӻ�
	u8 devmode;
//�ӻ�����
	u8 devnum;
//�ӻ����
	u8 slaveNo;
}SaveData_Typedef;
extern SaveData_Typedef LoadSave;



//==========================================================


//==========================================================
//ϵͳ��(2��)


typedef struct 
{	
//��ʾ����
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
////У׼�������Ƶ�ѹ�͵�
//	u32 VLOW1;
//	u32 SETVLOW1;
//	u32 VLOW2;
//	u32 SETVLOW2;
////У׼�������Ƶ�ѹ�ߵ�
//	u32 VHIGH1;
//	u32 SETVHIGH1;
//	u32 VHIGH2;
//	u32 SETVHIGH2;
//	u32 VHIGH3;
//	u32 SETVHIGH3;
////У׼�������Ƶ����͵�
//	u32 ILOW1;
//	u32 SETILOW1;
//	u32 ILOW2;
//	u32 SETILOW2;
////У׼�������Ƶ����ߵ�
//	u32 IHIGH1;
//	u32 SETIHIGH1;
//	u32 IHIGH2;
//	u32 SETIHIGH2;
//	u32 IHIGH3;
//	u32 SETIHIGH3;	
	
}DispValues_TypeDef;

typedef struct 
{	
//�ӻ�����
	u32 Voltage[15];
	u32 Current[15];
	u32 Rdata[15];
	u32 Power[15];
	u32 vrange[15];
	u32 crange[15];
}SlaveData_TypeDef;

typedef struct 
{	
//�ӻ�����
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

extern u8 F_Fail;//����ʧ�ܱ�־
extern u8 F_100ms;//100ms��ʱ��־
extern u8 F_set;//�������ʱ��־
extern u8 F_1s;//1s��ʱ��־
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
