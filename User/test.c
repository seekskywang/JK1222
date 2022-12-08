#include "pbdata.h"
#include <math.h>
#include "ff.h"
#include "cpld.h"
#include "./APP/usbh_bsp.h"
#include "./RTC/bsp_rtc.h"
#include "bsp_exti.h"
#include "open.h"
#include "flash_if.h"
#include "tm1650.h"
#include "bsp_bmp.h"

//反馈切换
// 输入0 1 2 3  U16_4094
// 0  1000V
//1   100V
//2 10V 
//==========================================================
#define POWERON_DISP_TIME (100)	//开机显示界面延时20*100mS=2s
u8 U15_4094,U16_4094;

static u8 filesendbuf[PACKAGE_SIZE+2];//升级文件发送分包缓存，256字节+2字节CRC
static u8 filedatabuf[64*1024];//升级文件数据缓存，最大64K
static u32 sendcount;//升级文件发送计数
FRESULT result=FR_NOT_READY;
FATFS fs;
FIL file;
UINT readcount;

u8 keyvalue;
u16 spinvalue;
u8 mainswitch;
u8 spinbit;
u8 spinbitmax;
u8 switchdelay;
u8 setcount;
u8 setslavecount;
u8 slaveonoffcount;
u8 inputtrans;
u8 coder_flag=0;
u8 buttonpage1=1;
u8 calpage;
u16 dacctrl=100;
u32 powermax;
u8 startdelay;
u8 bmpname[30];
u8 slaveID=1;
u32 upfilesize;
u16 resendcount;
u16 listonoffdelay;
//const u8 RANGE_UNIT[11]=
//{
//	4,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3,
//	2,
//	1,
//	3
//	
void JumpBoot(u8 flag)
{
  	void (*pUserApp)(void);
  uint32_t JumpAddress;
	if(flag==55)
  {		
	__asm("CPSID  I");
        
		JumpAddress = *(volatile uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS+4);
		pUserApp = (void (*)(void)) JumpAddress;
		TIM_Cmd(BASIC_TIM, DISABLE);	
		TIM_DeInit(TIM2);
		TIM_Cmd(TIM2,DISABLE);
		TIM_DeInit(BASIC_TIM);
		TIM_ITConfig(BASIC_TIM,TIM_IT_Update,DISABLE);
		TIM_Cmd(BASIC_TIM, DISABLE);	
		USART_DeInit(DEBUG_USART);
		USART_ITConfig(DEBUG_USART, USART_IT_RXNE, DISABLE);		
		USART_Cmd(DEBUG_USART,DISABLE);
		RCC_DeInit();
		RCC_RTCCLKCmd(DISABLE);
		EXTI_DeInit();
		SysTick->CTRL = 0;
		RTC_DeInit();
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//关闭WAKE UP 定时器中断
		RTC_WakeUpCmd( DISABLE);//关闭WAKE UP 定时器　
		Disable_Extiint();
		USBH_DeInit(&USB_OTG_Core,&USB_Host);
		__disable_irq();
		NVIC_DisableIRQ(OTG_FS_IRQn);
		NVIC_DisableIRQ(OTG_FS_WKUP_IRQn);
		NVIC_DisableIRQ(OTG_HS_IRQn);
		NVIC_DisableIRQ(OTG_HS_WKUP_IRQn);
		__ASM volatile ("cpsid i");
		/* Initialize user application's Stack Pointer */
		__set_PSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		__set_CONTROL(0);
		__set_MSP(*(volatile uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
		
        
		
//		NVIC_SystemReset();
		pUserApp();
	}
}

void READ_COMP(void)
{
	u8 i;
	
	if(LoadSave.ListNum < 2 || LoadSave.ListNum > 15)
	{
		LoadSave.ListNum = 2;
	}
	for(i=0;i<15;i++)
	{
		if(LoadSave.listmode[i] > 4)
		{
			LoadSave.listmode[i] = 0;
		}
	}
	if(LoadSave.devmode != 0 && LoadSave.devmode != 1 && LoadSave.devmode != 2)
		LoadSave.devmode=0;
}

void Para_Set_Comp(void)
{
	u8 i;
	if(LoadSave.devmode > 2)
	{
		LoadSave.devmode=0;
	}
	if(LoadSave.COMM > 2)
	{
		LoadSave.COMM=0;
	}
	if(LoadSave.Version > 6)
	{
		LoadSave.Version=0;
	}
	if(LoadSave.Version == 0)
	{
		powermax = 12000000;
	}else if(LoadSave.Version == 1){
		powermax = 8000000;
	}else if(LoadSave.Version == 2){
		powermax = 6000000;
	}else if(LoadSave.Version == 3){
		powermax = 4000000;
	}else if(LoadSave.Version == 4){
		powermax = 24000000;
	}else if(LoadSave.Version == 5){
		powermax = 32000000;
	}else if(LoadSave.Version == 5){
		powermax = 2000000;
	}
	if(LoadSave.sence > 1)
	{
		LoadSave.sence = 0;
	}
	if(LoadSave.crange > 1)
	{
		LoadSave.crange = 0;
	}
	if(LoadSave.vrange > 1)
	{
		LoadSave.vrange = 0;
	}
	if(LoadSave.limitdisp > 1)
	{
		LoadSave.limitdisp = 0;
	}
	if(LoadSave.limitbeep > 1)
	{
		LoadSave.limitbeep = 0;
	}
	if(LoadSave.vcomp > 1)
	{
		LoadSave.vcomp = 0;
	}
	if(LoadSave.ccomp > 1)
	{
		LoadSave.ccomp = 0;
	}
	if(LoadSave.pcomp > 1)
	{
		LoadSave.pcomp = 0;
	}
	if(LoadSave.language > 1)
	{
		LoadSave.language = 0;
	}
	if(LoadSave.ListNum > 15)
	{
		LoadSave.ListNum = 1;
	}
	if(LoadSave.StepMode > 2)
	{
		LoadSave.StepMode = 1;
	}
	if(LoadSave.LoopTest > 1)
	{
		LoadSave.LoopTest = 1;
	}
	if(LoadSave.ListBeep > 1)
	{
		LoadSave.ListBeep = 1;
	}
	if(LoadSave.Version == 5)
	{
		if(LoadSave.voltage > 1500000)
		{
			LoadSave.voltage = 1500000;
		}
		if(LoadSave.ledvo > 1500000)
		{
			LoadSave.ledvo = 1500000;
		}
	}else if(LoadSave.Version == 6){
		if(LoadSave.voltage > 5000000)
		{
			LoadSave.voltage = 5000000;
		}
		if(LoadSave.ledvo > 5000000)
		{
			LoadSave.ledvo = 5000000;
		}
	}else{
		if(LoadSave.voltage > MAX_SET_VOLT)
		{
			LoadSave.voltage = MAX_SET_VOLT;
		}
		if(LoadSave.ledvo > MAX_SET_VOLT)
		{
			LoadSave.ledvo = MAX_SET_VOLT;
		}
	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.current > 400000)
		{
			LoadSave.current = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.current > 2400000)
		{
			LoadSave.current = 2400000;
		}
	}else{
		if(LoadSave.current > MAX_SET_CURRENT)
		{
			LoadSave.current = MAX_SET_CURRENT;
		}
	}
	
	if(LoadSave.risistence > MAX_SET_RES)
	{
		LoadSave.risistence = MAX_SET_RES;
	}
	if(LoadSave.power > powermax)
	{
		LoadSave.power = powermax;
	}

	if(LoadSave.onvol > MAX_LoadVOLOTE)
	{
		LoadSave.onvol = 0;
	}
	if(LoadSave.offvol > MAX_FreeLoadVOLOTE)
	{
		LoadSave.offvol = 0;
	}
	LoadSave.crmode = 0;
	if(LoadSave.autooff > 999999)
	{
		LoadSave.autooff = 999999;
	}
	if(LoadSave.Version == 5)
	{
		if(LoadSave.maxv > 1500000)
		{
			LoadSave.maxv = 1500000;
		}
	}else if(LoadSave.Version == 6){
		if(LoadSave.maxv > 5000000)
		{
			LoadSave.maxv = 5000000;
		}
	}else{
		if(LoadSave.maxv > MAX_SET_VOLT)
		{
			LoadSave.maxv = MAX_SET_VOLT;
		}
	}
	
//	if(LoadSave.maxc > MAX_SET_CURRENT)
//	{
//		LoadSave.maxc = MAX_SET_CURRENT;
//	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.maxc > 400000)
		{
			LoadSave.maxc = 400000;
		}
		if(LoadSave.ledio > 400000)
		{
			LoadSave.ledio = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.maxc > 2400000)
		{
			LoadSave.maxc = 2400000;
		}
		if(LoadSave.ledio > 2400000)
		{
			LoadSave.ledio = 2400000;
		}
	}else{
		if(LoadSave.maxc > MAX_SET_CURRENT)
		{
			LoadSave.maxc = MAX_SET_CURRENT;
		}
		if(LoadSave.ledio > MAX_SET_CURRENT)
		{
			LoadSave.ledio = MAX_SET_CURRENT;
		}
	}
	if(LoadSave.maxp > powermax)
	{
		LoadSave.maxp = powermax;
	}
	if(LoadSave.crise > MAX_CURRENTUP)
	{
		LoadSave.crise = 0;
	}
	if(LoadSave.cdrop > MAX_CURRENTDOWN)
	{
		LoadSave.cdrop = MAX_CURRENTDOWN;
	}
	if(LoadSave.crange == 0)
	{
		if(LoadSave.crise < 1)
		{
			LoadSave.crise = 1;
		}
		if(LoadSave.cdrop < 1)
		{
			LoadSave.cdrop = 1;
		}
	}else{
		if(LoadSave.crise < 10)
		{
			LoadSave.crise = 10;
		}
		if(LoadSave.cdrop < 10)
		{
			LoadSave.cdrop = 10;
		}
	}
	if(LoadSave.cvdowntime > 30000)
	{
		LoadSave.cvdowntime = 30000;
	}
	if(LoadSave.vrange == 0)
	{
		if(LoadSave.cvdowntime < 1)
		{
			LoadSave.cvdowntime = 1;
		}
	}else{
		if(LoadSave.cvdowntime < 10)
		{
			LoadSave.cvdowntime = 10;
		}
	}

	if(LoadSave.ledvo > MAX_SET_VOLT)
	{
		LoadSave.ledvo = 0;
	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.ledio > 400000)
		{
			LoadSave.ledio = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.ledio > 2400000)
		{
			LoadSave.ledio = 2400000;
		}
	}else{
		if(LoadSave.ledio > MAX_SET_CURRENT)
		{
			LoadSave.ledio = MAX_SET_CURRENT;
		}
	}
//	if(LoadSave.ledio > MAX_SET_CURRENT)
//	{
//		LoadSave.ledio = 0;
//	}
	if(LoadSave.ledrd > MAX_SET_RES)
	{
		LoadSave.ledrd = 0;
	}
	if(LoadSave.loadmode > 2)
	{
		LoadSave.loadmode = 0;
	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.loadc1 > 400000)
		{
			LoadSave.loadc1 = 400000;
		}
		if(LoadSave.loadc2 > 400000)
		{
			LoadSave.loadc2 = 400000;
		}
		if(LoadSave.loadc3 > 400000)
		{
			LoadSave.loadc3 = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.loadc1 > 2400000)
		{
			LoadSave.loadc1 = 2400000;
		}
		if(LoadSave.loadc2 > 2400000)
		{
			LoadSave.loadc2 = 2400000;
		}
		if(LoadSave.loadc3 > 2400000)
		{
			LoadSave.loadc3 = 2400000;
		}
	}else{
		if(LoadSave.loadc1 > MAX_SET_CURRENT)
		{
			LoadSave.loadc1 = MAX_SET_CURRENT;
		}
		if(LoadSave.loadc2 > MAX_SET_CURRENT)
		{
			LoadSave.loadc2 = MAX_SET_CURRENT;
		}
		if(LoadSave.loadc3 > MAX_SET_CURRENT)
		{
			LoadSave.loadc3 = MAX_SET_CURRENT;
		}
	}
//	if(LoadSave.loadc1 > MAX_SET_CURRENT)
//	{
//		LoadSave.loadc1 = MAX_SET_CURRENT;
//	}
//	if(LoadSave.loadc2 > MAX_SET_CURRENT)
//	{
//		LoadSave.loadc2 = MAX_SET_CURRENT;
//	}
//	if(LoadSave.loadc3 > MAX_SET_CURRENT)
//	{
//		LoadSave.loadc3 = MAX_SET_CURRENT;
//	}
	if(LoadSave.curvetime > 10)
	{
		LoadSave.curvetime = 0;
	}
	if(LoadSave.Version == 5)
	{
		if(LoadSave.coffv1 > 1500000)
		{
			LoadSave.coffv1 = 1500000;
		}
		if(LoadSave.coffv2 > 1500000)
		{
			LoadSave.coffv2 = 1500000;
		}
		if(LoadSave.coffv3 > 1500000)
		{
			LoadSave.coffv3 = 1500000;
		}
		if(LoadSave.loadr > MAX_SET_RES)
		{
			LoadSave.loadr = MAX_SET_RES;
		}
		if(LoadSave.coffvr > 1500000)
		{
			LoadSave.coffvr = 1500000;
		}
	}else if(LoadSave.Version == 6){
		if(LoadSave.coffv1 > 5000000)
		{
			LoadSave.coffv1 = 5000000;
		}
		if(LoadSave.coffv2 > 5000000)
		{
			LoadSave.coffv2 = 5000000;
		}
		if(LoadSave.coffv3 > 5000000)
		{
			LoadSave.coffv3 = 5000000;
		}
		if(LoadSave.loadr > MAX_SET_RES)
		{
			LoadSave.loadr = MAX_SET_RES;
		}
		if(LoadSave.coffvr > 5000000)
		{
			LoadSave.coffvr = 5000000;
		}
	}else{
		if(LoadSave.coffv1 > MAX_SET_VOLT)
		{
			LoadSave.coffv1 = MAX_SET_VOLT;
		}
		if(LoadSave.coffv2 > MAX_SET_VOLT)
		{
			LoadSave.coffv2 = MAX_SET_VOLT;
		}
		if(LoadSave.coffv3 > MAX_SET_VOLT)
		{
			LoadSave.coffv3 = MAX_SET_VOLT;
		}
		if(LoadSave.loadr > MAX_SET_RES)
		{
			LoadSave.loadr = MAX_SET_RES;
		}
		if(LoadSave.coffvr > MAX_SET_VOLT)
		{
			LoadSave.coffvr = MAX_SET_VOLT;
		}
	}
	if(LoadSave.testdis > 1)
	{
		LoadSave.testdis = 0;
	}
	if(LoadSave.dynamode > 2)
	{
		LoadSave.dynamode = 0;
	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.valA > 400000)
		{
			LoadSave.valA = 400000;
		}
		if(LoadSave.valB > 400000)
		{
			LoadSave.valB = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.valA > 2400000)
		{
			LoadSave.valA = 2400000;
		}
		if(LoadSave.valB > 2400000)
		{
			LoadSave.valB = 2400000;
		}
	}else{
		if(LoadSave.valA > MAX_SET_CURRENT)
		{
			LoadSave.valA = MAX_SET_CURRENT;
		}
		if(LoadSave.valB > MAX_SET_CURRENT)
		{
			LoadSave.valB = MAX_SET_CURRENT;
		}
	}
	
	if(LoadSave.timeA > 600000)
	{
		LoadSave.timeA = 600000;
	}
	if(LoadSave.timeB > 600000)
	{
		LoadSave.timeB = 600000;
	}
	if(LoadSave.dynaIRise > MAX_CURRENTUP)
	{
		LoadSave.dynaIRise = MAX_CURRENTUP;
	}
	if(LoadSave.dynaIDrop > MAX_CURRENTUP)
	{
		LoadSave.dynaIDrop = MAX_CURRENTUP;
	}
	
	if(LoadSave.vhigh > MAX_SET_VOLT)
	{
		LoadSave.vhigh = MAX_SET_VOLT;
	}
	if(LoadSave.vlow > LoadSave.vhigh)
	{
		LoadSave.vlow = 0;
	}
	if(LoadSave.Version == 3)
	{
		if(LoadSave.chigh > 400000)
		{
			LoadSave.chigh = 400000;
		}
	}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
		if(LoadSave.chigh > 2400000)
		{
			LoadSave.chigh = 2400000;
		}
	}else{
		if(LoadSave.chigh > MAX_SET_CURRENT)
		{
			LoadSave.chigh = MAX_SET_CURRENT;
		}
	}
	
	if(LoadSave.clow > LoadSave.chigh)
	{
		LoadSave.clow = 0;
	}
	if(LoadSave.phigh > powermax)
	{
		LoadSave.phigh = powermax;
	}
	if(LoadSave.plow > LoadSave.phigh)
	{
		LoadSave.plow = 0;
	}
	if(LoadSave.ListNum > 15)
	{
		LoadSave.ListNum = 15;
	}
	for(i = 0;i<15;i++)
	{
		if(LoadSave.listmode[i] > 4)
		{
			LoadSave.listmode[i] = 4;
		}
		if(LoadSave.listmode[i] == 0)
		{
			if(LoadSave.Version == 3)
			{
				if(LoadSave.listvalue[i] > 400000)
				{
					LoadSave.listvalue[i] = 400000;
				}
				if(LoadSave.listhigh[i] > 400000)
				{
					LoadSave.listhigh[i] = 400000;
				}
				if(LoadSave.listlow[i] > 400000)
				{
					LoadSave.listlow[i] = 400000;
				}
			}else if(LoadSave.Version == 4 || LoadSave.Version == 5){
				if(LoadSave.listvalue[i] > 2400000)
				{
					LoadSave.listvalue[i] = 2400000;
				}
				if(LoadSave.listhigh[i] > 2400000)
				{
					LoadSave.listhigh[i] = 2400000;
				}
				if(LoadSave.listlow[i] > 2400000)
				{
					LoadSave.listlow[i] = 2400000;
				}
			}else{
				if(LoadSave.listvalue[i] > MAX_SET_CURRENT)
				{
					LoadSave.listvalue[i] = MAX_SET_CURRENT;
				}
				if(LoadSave.listhigh[i] > MAX_SET_CURRENT)
				{
					LoadSave.listhigh[i] = MAX_SET_CURRENT;
				}
				if(LoadSave.listlow[i] > MAX_SET_CURRENT)
				{
					LoadSave.listlow[i] = MAX_SET_CURRENT;
				}
			}
			
		}else if(LoadSave.listmode[i] == 1){
			if(LoadSave.Version == 5){
				if(LoadSave.listvalue[i] > 1500000)
				{
					LoadSave.listvalue[i] = 1500000;
				}
				if(LoadSave.listhigh[i] > 1500000)
				{
					LoadSave.listhigh[i] = 1500000;
				}
				if(LoadSave.listlow[i] > 1500000)
				{
					LoadSave.listlow[i] = 1500000;
				}
			}else if(LoadSave.Version == 6){
				if(LoadSave.listvalue[i] > 5000000)
				{
					LoadSave.listvalue[i] = 5000000;
				}
				if(LoadSave.listhigh[i] > 5000000)
				{
					LoadSave.listhigh[i] = 5000000;
				}
				if(LoadSave.listlow[i] > 5000000)
				{
					LoadSave.listlow[i] = 5000000;
				}
			}else{
				if(LoadSave.listvalue[i] > MAX_SET_VOLT)
				{
					LoadSave.listvalue[i] = MAX_SET_VOLT;
				}
				if(LoadSave.listhigh[i] > MAX_SET_VOLT)
				{
					LoadSave.listhigh[i] = MAX_SET_VOLT;
				}
				if(LoadSave.listlow[i] > MAX_SET_VOLT)
				{
					LoadSave.listlow[i] = MAX_SET_VOLT;
				}
			}
			
		}else if(LoadSave.listmode[i] == 2){
			if(LoadSave.listvalue[i] > MAX_SET_RES)
			{
				LoadSave.listvalue[i] = MAX_SET_RES;
			}
			if(LoadSave.listhigh[i] > MAX_SET_RES)
			{
				LoadSave.listhigh[i] = MAX_SET_RES;
			}
			if(LoadSave.listlow[i] > MAX_SET_RES)
			{
				LoadSave.listlow[i] = MAX_SET_RES;
			}
		}else if(LoadSave.listmode[i] == 3){
			if(LoadSave.listvalue[i] > powermax)
			{
				LoadSave.listvalue[i] = powermax;
			}
			if(LoadSave.listhigh[i] > powermax)
			{
				LoadSave.listhigh[i] = powermax;
			}
			if(LoadSave.listlow[i] > powermax)
			{
				LoadSave.listlow[i] = powermax;
			}
		}
		if(LoadSave.listcomp[i] > 3)
		{
			LoadSave.listcomp[i] = 3;
		}
		if(LoadSave.delay[i] > 36000)
		{
			LoadSave.delay[i] = 36000;
		}
		if(LoadSave.Baudrate > 4)
		{
			LoadSave.Baudrate = 1;
		}
		if(LoadSave.Addr > 255)
		{
			LoadSave.Addr = 1;
		}
		if(LoadSave.Version == 5)
		{
			if(LoadSave.listonvol > 1500000)
			{
				LoadSave.listonvol = 0;
			}
			if(LoadSave.gatev > 1500000)
			{
				LoadSave.gatev = 0;
			}
		}else if(LoadSave.Version == 6)
		{
			if(LoadSave.listonvol > 5000000)
			{
				LoadSave.listonvol = 0;
			}
			if(LoadSave.gatev > 5000000)
			{
				LoadSave.gatev = 0;
			}
		}else{
			if(LoadSave.listonvol > MAX_SET_VOLT)
			{
				LoadSave.listonvol = 0;
			}
			if(LoadSave.gatev > MAX_SET_VOLT)
			{
				LoadSave.gatev = 0;
			}
		}
	}
}


void lockcheck(void)
{
	if(DispValue.Operation_MODE == 1)
	{
		SetSystemStatus(SYS_STATUS_LOCK);
	}
}

void UpError(u8 code)
{
	u8 textbuf[50];
//	filesendflag=0;
	UpPara.updatestep=0;
	UpPara.filesendflag=0;
//	sendresflag=0;
	UpPara.package=0;
	Colour.black=LCD_COLOR_TEST_BACK;
//	LCD_DrawRect( 2, 30, 479, 130 , LCD_COLOR_TEST_BACK);
	LCD_DrawFullRect( 2, 30, 477, 100   ) ;

	Colour.Fword=Red;
	if(code == 1)
		WriteString_16(2,110,"升级出错,请重启仪器",  0);
	else if(code == 2)
		WriteString_16(2,110,"读取文件出错,请检查U盘文件并重启仪器",  0);
	
}

void Bin_Send(u16 size)
{
//	result=f_read(&file,filedatabuf,size-2, &readcount);
//	if(result == FR_OK)
//	{
		memcpy(filesendbuf,&filedatabuf[sendcount],size-2);
		filesendbuf[size-2]=Hardware_CRC(filesendbuf,size-2)>>8;
		filesendbuf[size-1]=Hardware_CRC(filesendbuf,size-2);
	//	delay_ms(100);
		Uart1SendBuff(filesendbuf,size);
//	}else{
//		UpError(1);
//	}
}



void UpdateProcess(void)
{

	uint8_t Disp_flag=1;
	u8 key;
  LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_Up_Item();
	USBH_Init(&USB_OTG_Core,
	USB_OTG_FS_CORE_ID,
				&USB_Host,
				&USBH_MSC_cb,
				&USR_cb);
 USBH_Process(&USB_OTG_Core, &USB_Host);
 	while(GetSystemStatus()==SYS_STATUS_UPDATE)
	{
		if(result != FR_OK)
			USBH_Process(&USB_OTG_Core, &USB_Host);
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		if(Disp_flag == 1)
		{  
			Disp_flag = 0;
			if(result == FR_OK/* && usbcount > 20*/)
			{
				Disp_Usbflag(1);
//				WriteString_16(50,70,pt[1],0);
//				WriteString_16(50,90,pt[2],0);
//				WriteString_16(50,110,pt[3],0);
//				File_IAP();			
			}else{
				Disp_Usbflag(2);
			}
//			WriteString_16(LIST1, FIRSTLINE+2, "与上位机通信中 为防止误操作 下位机暂时不可操作",0);//	
//			WriteString_16(LIST1, FIRSTLINE+SPACE1+2, "如需恢复下位机 请关机后重启",0);//
		}
		if(UpPara.filesendflag == 1)
		{
			
			if(UpPara.sendresflag == 0)
			{
				Colour.black=LCD_COLOR_TEST_BACK;
				UpPara.sendprog=(u8)(((float)UpPara.package/(float)(upfilesize/PACKAGE_SIZE))*100);
				
				Hex_Format(UpPara.sendprog,0,3,0);
				WriteString_16(2,70,DispBuf,  0);
				if(UpPara.package == upfilesize/PACKAGE_SIZE)
				{
					Bin_Send((upfilesize%PACKAGE_SIZE)+2);
					UpPara.filesendflag=0;
					sendcount=0;
				}else{
					Bin_Send(PACKAGE_SIZE+2);
					sendcount+=PACKAGE_SIZE;
				}
				UpPara.sendresflag=1;
				UpPara.package++;
				if(UpPara.sendprog == 100)
				{
					UpPara.package=0;
					WriteString_16(2,90,"发送完成,请等待升级完成",  0);
					UpPara.updatestep=0;
//					updatefinishflag=1;
				}
				
			}
		}
		key=Key_Read();
 		inputtrans = key;
        if(key!=KEY_NONE)
		{	
			Disp_flag=1;
			switch(key)
			{
				case Key_F1:
					if(UpPara.updatestep == 1)
						Set_Boot();
				break;
				case Key_F2:
					if(UpPara.updatestep == 2)
					{
						result=f_open(&file, "0:99XXUP103.bin",FA_OPEN_EXISTING | FA_READ);
						if(result==FR_OK)
						{
							
							upfilesize=file.fsize;
							result=f_read(&file,filedatabuf,upfilesize, &readcount);
							if(result==FR_OK)
							{
								UP_FILESIZE();
							}else{
								UpError(2);
							}
						}else{
							UpError(2);
						}
					}
				break;
				case Key_F3:
					if(UpPara.updatestep == 3)
					{
//						Bin_Send(1024+2);
						UpPara.filesendflag=1;
						UpPara.package=0;
					}
				break;
				case Key_F4:

				
				break;
				case Key_F5:
					result = f_mount(&fs,"0:",1);
					if(result == FR_OK)
					{
						UpPara.updatestep=1;
					}else{
						UpPara.updatestep=0;
					}
				break;

				
				case Key_LEFT:

				break;
				case Key_RIGHT:

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
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				case PRESS_SPIN:
				{
					Uart1SendBuff(filedatabuf,1026);
				}break;
				case Key_Ent:
				{
					
				}break;
				case Key_ESC:
				{
					if(mainswitch == 0)
						SetSystemStatus(SYS_STATUS_TEST);
				}break;

				default:
				break;
					
			}
			
		}
	}
}



void LockProcess(void)
{

	uint8_t Disp_flag=1;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	Colour.black=LCD_COLOR_TEST_BACK;
	Colour.Fword=LCD_COLOR_SELECT;
	
 	while(GetSystemStatus()==SYS_STATUS_LOCK)
	{
		if(Disp_flag == 1)
		{
			Disp_flag = 0;
			WriteString_16(LIST1, FIRSTLINE+2, "与上位机通信中 为防止误操作 下位机暂时不可操作",0);//	
			WriteString_16(LIST1, FIRSTLINE+SPACE1+2, "如需恢复下位机 请关机后重启",0);//
		}
		
	}
}


void Power_Process(void)
{
	u16 i;
    u8 j;

    Disp_Coordinates_Typedef Debug_Cood;
   
	u8 key;
    Int_Pe3flag=0;
	
    RTC_CLK_Config();
    RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//配置WAKE UP中断,1秒钟中断一次
    GPIO_Configuration();//端口初始化
     if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
      {
        /* 设置时间和日期 */
            RTC_TimeAndDate_Set();
      }
      else
      {
        /* 检查是否电源复位 */   
        /* 使能 PWR 时钟 */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
            /* PWR_CR:DBF置1，使能RTC、RTC备份寄存器和备份SRAM的访问 */
            PWR_BackupAccessCmd(ENABLE);
            /* 等待 RTC APB 寄存器同步 */
            RTC_WaitForSynchro();   
      } 
    
//    delay_ms(10);
	 Debug_USART_Config(DEBUG_USART_BAUDRATE);
	 HS_USART_Config(DEBUG_USART_BAUDRATE);
    Keyboard_Init();//按键初始化
	Spin_Init();

    //////////////////////////////////////////液晶初始化
    LCD_Init();
    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	/*把背景层刷黑色*/
    LCD_SetLayer(LCD_BACKGROUND_LAYER);  
    LCD_Clear(LCD_COLOR_RED);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
//	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
    LCD_SetTransparency(0xff);
	LCD_Clear(LCD_COLOR_TEST_BACK);
	if(LoadSave.jkflag == 0)
	{
    lcd_image((uint8_t *)gImage_open);
	}
    SPI_FLASH_Init();

	
    TIM6_Configuration();//定时器6定时10ms

//    EXTI_Ads1251_Config();//中断没有开启

	i=0;//显示延时
	Read_set_flash();
// USBH_Init(&USB_OTG_Core,
//	USB_OTG_FS_CORE_ID,
//				&USB_Host,
//				&USBH_MSC_cb,
//				&USR_cb);
// USBH_Process(&USB_OTG_Core, &USB_Host);
	Beep_Off();
	READ_COMP();
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{
		i++;
        j++;
       
		if(i>POWERON_DISP_TIME)//延时20*100mS=2s后自动退出
            SetSystemStatus(SYS_STATUS_TEST);//待测状态
//		else
//			SetSystemStatus(SYS_STATUS_TEST);//待测状态

        
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//等待按键(100*10ms/10=100ms)

		switch(key)
		{
            case	    Key_F1:
                
                break;
            case		Key_F2:
                
                break;
            case 	    Key_F3:
                
                break;
            case 	    Key_F4:
                
                break;
            case		Key_F5:
                break;
			case Key_Ent:
			{
				LCD_Clear(LCD_COLOR_TEST_BACK);
				Debug_Cood.xpos=180;
				Debug_Cood.ypos =120;
				Debug_Cood.lenth=120;
				input_password(&Debug_Cood);
			}break;
            case		Key_LEFT:
                break;
            case		Key_RIGHT:
                break;

            case		Key_NUM7:
                break;
            case		Key_NUM8:
                break;
            case		Key_NUM9:
                break;
            case		Key_NUM4:
                break;
            case		Key_NUM5:
                break;
            case		Key_NUM6:
                break;
            case		Key_NUM1:
                break;
            case		Key_NUM2:
                break;
            case		Key_NUM3:
                break;
            case		Key_NUM0:
                break;
            case		Key_DOT:
                break;


		}
		//Range_Control(3);
	}
//	BeepOff();
}



//==========================================================
//函数名称：Setup_Process
//函数功能：设置程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.28
//修改日期：2015.10.28 10:45
//备注说明：无
//==========================================================
void Setup_Process(void)
{

	u8 keynum=0;
	u8 keytrans=0;
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key;
	vu8 Disp_Flag=1;
	keynum=0;
	
    LCD_Clear(LCD_COLOR_TEST_BACK);
    Disp_Test_Set_Item();
 	while(GetSystemStatus()==SYS_STATUS_SETUP)
	{
//		DispValue.Operation_MODE = 1;
	    keytrans=Encoder_Process(keynum);

		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
		
		if(Disp_Flag==1)
		{
			DispSet_value(keynum);
           
			Disp_Flag=0;
		
		}
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		if(setflag != 0)
		{
			if(setcount == 5)
			{
				Set_Para();
				setcount = 0;
			}else{
				setcount++;
			}
		}
        key=Key_Read();
 		inputtrans = key;
        if(key!=KEY_NONE)
		{	
			Disp_Flag=1;
			switch(key)
			{
				case Key_F1:

					switch(keynum)
					{
						case 0:
							SetSystemStatus(SYS_STATUS_TEST);//
							break;
						case 1:
						{
							LoadSave.sence = 0;
							Sence_SW();
							Store_set_flash();
						}break;
						case 2:
						{
							LoadSave.crange = 0;
							I_Gear_SW();
							Store_set_flash();
						}break;
						case 3:
						{
							LoadSave.vrange = 0;
							V_Gear_SW();
							Store_set_flash();
						}break;	
						default:
						break;
					
					
					}

				break;
				case Key_F2:
					

					switch(keynum)
					{
						case 0:
							//if(Button_Page.page==0)
								SetSystemStatus(SYS_STATUS_LIMITSET);
//							else
//								SetSystemStatus(SYS_STATUS_SYSSET);
								
							break;
						case 1:
						{
							LoadSave.sence = 1;
							Sence_SW();
							Store_set_flash();
						}break;
						case 2:
						{
							LoadSave.crange = 1;
							I_Gear_SW();
							Store_set_flash();
						}break;
						case 3:
						{
							LoadSave.vrange = 1;
							V_Gear_SW();
							Store_set_flash();
						}break;	
						
						default:
						break;
					}				
			
				break;
				case Key_F3:
					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SYSSET);
							break;
						case 1:
							break;
						case 2:
							break;
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
                            
							
							break;
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
						case 4:
						case 9:
							break;
						case 7:	
							
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;

				
				case Key_LEFT:
					if(keynum>6)
					{
						keynum-=6;
					}else if(keynum == 1){
						keynum = 0;
					}
				break;
				case Key_RIGHT:
					if(keynum<7 && keynum>0){
						keynum+=6;
					}else if(keynum == 0){
						keynum = 1;
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
						case 4://启动电压
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.onvol=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 5://关断电压
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.offvol=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 6://电压下降
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cvdowntime=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 7://延时关断
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE-2;
							Coordinates.lenth=76;
							LoadSave.autooff=Disp_Set_TimeS(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 8://最大电流
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*1-2;
							Coordinates.lenth=76;
							LoadSave.maxc=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 9://最大电压
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2-2;
							Coordinates.lenth=76;
							LoadSave.maxv=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 10://最大功率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.maxp=Disp_Set_TimeP(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 11://上升速率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.crise=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 12://下降速率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cdrop=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
					}
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				case PRESS_SPIN:
				{
					switch(keynum)
					{
						case 1:
						{
							if(LoadSave.sence == 0)
								LoadSave.sence = 1;
							else
								LoadSave.sence = 0;
							Sence_SW();
							Store_set_flash();
						}break;
						case 2:
						{
							if(LoadSave.crange == 0)
								LoadSave.crange = 1;
							else
								LoadSave.crange = 0;
							I_Gear_SW();
							Store_set_flash();
						}break;
						case 3:
						{
							if(LoadSave.vrange == 0)
								LoadSave.vrange = 1;
							else
								LoadSave.vrange = 0;
							V_Gear_SW();
							Store_set_flash();
						}break;
					}
				}break;
				case Key_Ent:
				{
					switch(keynum)
					{
						case 4://启动电压
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.onvol=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 5://关断电压
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.offvol=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 6://电压下降
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cvdowntime=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 7://延时关断
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE-2;
							Coordinates.lenth=76;
							LoadSave.autooff=Disp_Set_TimeS(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
							break;
						case 8://最大电流
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*1-2;
							Coordinates.lenth=76;
							LoadSave.maxc=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 9://最大电压
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2-2;
							Coordinates.lenth=76;
							LoadSave.maxv=Disp_Set_VNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 10://最大功率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.maxp=Disp_Set_TimeP(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 11://上升速率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.crise=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						case 12://下降速率
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cdrop=Disp_Set_CNum(&Coordinates);
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;
						default:
						break;
					
					
					}
				}break;
				case Key_ESC:
				{
					if(mainswitch == 0)
						SetSystemStatus(SYS_STATUS_TEST);
				}break;
				case Key_SAVE:
				{
					
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif				
				}break;
				default:
				break;
					
			}
			
		}
	 	
	
	
	
	}
}


//==========================================================
//函数名称：Limit_Process
//函数功能	极限设置
//入口参数：无
//出口参数：无
//创建日期：2015.10.28
//修改日期：2015.10.28 10:45
//备注说明：无
//==========================================================
void Limit_Process(void)
{

	u8 keynum=0;
	u8 keytrans = 0;
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key;
	vu8 Disp_Flag=1;    
	LCD_Clear(LCD_COLOR_TEST_BACK);
    Disp_Limit_Item();
 	while(GetSystemStatus()==SYS_STATUS_LIMITSET)
	{
	    keytrans=Encoder_Process(keynum);
		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
		
		if(Disp_Flag==1)
		{
			DispLimit_value(keynum);
           
			Disp_Flag=0;
		
		}

        key=Key_Read();
		inputtrans = key;
        if(key!=KEY_NONE)
		{	Disp_Flag=1;
			switch(key)
			{
				case Key_F1:

					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_TEST);//
						break;
						case 7:
						{
							LoadSave.limitdisp = 0;
						}break;
						case 8:
						{
							LoadSave.limitbeep = 0;
						}break;
						case 9:
						{
							LoadSave.vcomp = 0;
						}break;
						case 10:
						{
							LoadSave.ccomp = 0;
						}break;
						case 11:
						{
							LoadSave.pcomp = 0;
						}break;
						default:
							break;
					
					
					}

				break;
				case Key_F2:
					

					switch(keynum)
					{
						case 0:
								SetSystemStatus(SYS_STATUS_SETUP);//
						break;
						case 7:
						{
							LoadSave.limitdisp = 1;
						}break;
						case 8:
						{
							LoadSave.limitbeep = 1;
						}break;
						case 9:
						{
							LoadSave.vcomp = 1;
						}break;
						case 10:
						{
							LoadSave.ccomp = 1;
						}break;
						case 11:
						{
							LoadSave.pcomp = 1;
						}break;
						default:
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
						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(keynum)
					{
						
						
						default:
							break;
					
					
					}
                    
					
				break;

				
				case Key_LEFT:
					if(keynum>6)
					{
						keynum-=6;
					}else if(keynum == 1){
						keynum = 0;
					}
				break;
				case Key_RIGHT:
					if(keynum<6 && keynum>0){
						keynum+=6;
					}else if(keynum == 0){
						keynum = 1;
					}
				break;

				case Key_DOT:

					break;
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
						case 1://电压上限
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE;
							Coordinates.lenth=76;
							LoadSave.vhigh=Disp_Set_VNum(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 2://电压下限
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1+-2;
							Coordinates.lenth=76;
							LoadSave.vlow=Disp_Set_VNum(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 3://电流上限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2;
							Coordinates.lenth=76;
							LoadSave.chigh=Disp_Set_CNum(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 4://电流下限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.clow=Disp_Set_CNum(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 5://功率上限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.phigh=Disp_Set_TimeP(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 6://功率下限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.plow=Disp_Set_TimeP(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						default:
							break;
					}
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				case Key_ESC:
				{
					if(mainswitch == 0)
						SetSystemStatus(SYS_STATUS_TEST);
				}break;
				case PRESS_SPIN:
				{
					switch(keynum)
					{
						case 7:
						{
							if(LoadSave.limitdisp == 0)
								LoadSave.limitdisp = 1;
							else
								LoadSave.limitdisp = 0;
						}break;
						case 8:
						{
							if(LoadSave.limitbeep == 0)
								LoadSave.limitbeep = 1;
							else
								LoadSave.limitbeep = 0;
						}break;
						case 9:
						{
							if(LoadSave.vcomp == 0)
								LoadSave.vcomp = 1;
							else
								LoadSave.vcomp = 0;
						}break;
						case 10:
						{
							if(LoadSave.ccomp == 0)
								LoadSave.ccomp = 1;
							else
								LoadSave.ccomp = 0;
						}break;
						case 11:
						{
							if(LoadSave.pcomp == 0)
								LoadSave.pcomp = 1;
							else
								LoadSave.pcomp = 0;
						}break;
						default:
							break;
					
					}
				}break;
				case Key_Ent:
				{
					switch(keynum)
					{

						case 1://电压上限
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE;
							Coordinates.lenth=76;
							LoadSave.vhigh=Disp_Set_VNum(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 2://电压下限
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1+-2;
							Coordinates.lenth=76;
							LoadSave.vlow=Disp_Set_VNum(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 3://电流上限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2;
							Coordinates.lenth=76;
							LoadSave.chigh=Disp_Set_CNum(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 4://电流下限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.clow=Disp_Set_CNum(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 5://功率上限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.phigh=Disp_Set_TimeP(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 6://功率下限
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.plow=Disp_Set_TimeP(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						default:
							break;
					
					
					}
				}break;
				case Key_SAVE:
				{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
				}break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
}

//常规分选
void Test_Comp(void)
{
	u8 i;
	DispValue.testbeep = 0;
	if(LoadSave.vcomp == 1)
	{
		if(LoadSave.vrange == 0)
		{
			if(DispValue.Voltage > LoadSave.vhigh ||
			   DispValue.Voltage < LoadSave.vlow)
			{
				DispValue.testcomp[0] = 1;
			}else{
				DispValue.testcomp[0] = 0;
			}
		}else if(LoadSave.vrange == 1){
			if(DispValue.Voltage*10 > LoadSave.vhigh ||
			   DispValue.Voltage*10 < LoadSave.vlow)
			{
				DispValue.testcomp[0] = 1;
			}else{
				DispValue.testcomp[0] = 0;
			}
		}
//		if(LoadSave.limitbeep == DispValue.testcomp[0])
//		{
//			DispValue.testbeep = 1;
//		}
	}else{
		DispValue.testcomp[0] = 0;
	}

	
	if(LoadSave.ccomp == 1)
	{
		if(LoadSave.crange == 0)
		{
			if(DispValue.Current > LoadSave.chigh ||
			   DispValue.Current < LoadSave.clow)
			{
				DispValue.testcomp[1] = 1;
			}else{
				DispValue.testcomp[1] = 0;
			}
		}else if(LoadSave.crange == 1){
			if(DispValue.Current*10 > LoadSave.chigh ||
			   DispValue.Current*10 < LoadSave.clow)
			{
				DispValue.testcomp[1] = 1;
			}else{
				DispValue.testcomp[1] = 0;
			}
		}
//		if(LoadSave.limitbeep == DispValue.testcomp[1])
//		{
//			DispValue.testbeep = 1;
//		}
	}else{
		DispValue.testcomp[1] = 0;
	}
	
	
	
	if(LoadSave.pcomp == 1)
	{

		if(DispValue.Power*10 > LoadSave.phigh ||
		   DispValue.Power*10 < LoadSave.plow)
		{
			DispValue.testcomp[2] = 1;
		}else{
			DispValue.testcomp[2] = 0;
		}
//		if(LoadSave.limitbeep == DispValue.testcomp[2])
//		{
//			DispValue.testbeep = 1;
//		}
	}else{
		DispValue.testcomp[2] = 0;
	}
	if(LoadSave.pcomp == 1 || LoadSave.vcomp == 1 || LoadSave.ccomp == 1)
	{
		for(i=0;i<3;i++)
		{
			if(LoadSave.limitbeep == 0)
			{
				if(DispValue.testcomp[i] == 1)
				{
					DispValue.testbeep = 0;
					break;
				}else{
					DispValue.testbeep = 1;
				}
			}else if(LoadSave.limitbeep == 1){
				if(DispValue.testcomp[i] == 1)
				{
					DispValue.testbeep = 1;
					break;
				}else{
					DispValue.testbeep = 0;
				}
			}
		}
	}else{
		DispValue.testbeep = 0;
	}
	
}
void Test_Beep(void)
{
	if(DispValue.testbeep == 1)
	{
		Beep_On();
	}else if(DispValue.testbeep == 0){
		Beep_Off();
	}
}

//==========================================================
//函数名称：Test_Process
//函数功能：测试主程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 14:18
//备注说明：x.xx ms(12MHz/12T)
//==========================================================
 void Test_Process(void)
{   
	static u8 skipkey;
	static u32 oldskip;
	char sendbuf[24];
	vu8 key;
    vu16 USB_Count=0;
    UINT fnum;
    vu8 test_Vsorting,test_Rsorting;
    u32 color;
    u8 keynum=0;
	u8 keytrans=0;
    
	vu16 i;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志
//    u8 Disp_Testflag=0;
//    u8 fit_num=0;
//	Check_Parameter_Limit();//设置值检查
	F_100ms=FALSE;//100ms定时	
	coder_flag = 0;
	LCD_Clear(LCD_COLOR_TEST_BACK);


	Disp_Test_Item();  

	i=0;
	Set_Para();
	spinbit = 1;
	coder_flag = 1;
	if(LoadSave.devmode==1)
		keynum = 0;
	else
		keynum = 2;
	buttonpage1 = 1;
//	DispValue.protectflag = 2;
//	DispValue.alertdisp=1;
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
        USB_Count++;
		keytrans=Encoder_Process(keynum);
//		USBH_Process(&USB_OTG_Core, &USB_Host);
//							USB_Count=0;

//							result = f_mount(&fs,"0:",1);
//		Colour.Fword=White;
//		Colour.black=LCD_COLOR_TEST_BACK;//User_Check_main
//		Hex_Format(DispValue.protectflag,0,2,0);
//		WriteString_16(170, 4,DispBuf ,  0);
		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
        Colour.Fword=White;
//		Set_Addr();
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		if(UART3_Buffer_Rece_flag==1)
		{
			UART3_Buffer_Rece_flag=0;
			Rec3_Handle();
		}
		if(flag_spin!=0)
		{
			flag_spin--;
			if(flag_spin==1)
				spinsend = 1;
		}
		if(spinsend == 1)
		{
			spinsend = 0;
			Set_Para();
			if(LoadSave.devmode==0)
				setslaveflag=LoadSave.devnum;
		}
    if(Disp_Flag==1 )//显示设置的值
		{
			Disp_Test_value(keynum);
            
//			Disp_R_V("V");
			Disp_Flag = 0;
		}
//		ReadData();
		if(F_100ms == TRUE/* && setflag == 0*/)
		{
			ReadData();//读本机数据
			if(LoadSave.devmode==0 && setslaveflag == 0)//主机模式
			{
//				ReadSlaveData(slaveID);//读取从机数据
				if(slaveID<LoadSave.devnum)
					slaveID++;
				else
					slaveID=1;
			}
			F_100ms=FALSE;
		}
		if(setflag != 0)
		{
			if(setcount == 5)
			{
				Set_Para();
				setcount = 0;
			}else{
				setcount++;
			}
		}
		if(setslaveflag != 0)
		{
			if(setslavecount == 3)
			{
				SetSlavePara(setslaveflag);
				setslavecount = 0;
			}else{
				setslavecount++;
			}
		}
		if(slaveonoffflag != 0)
		{
			if(slaveonoffcount == 3)
			{
				SlaveOnOff(slaveonoffflag);
				slaveonoffcount = 0;
			}else{
				slaveonoffcount++;
			}
		}
		if(mainswitch == 0)
		{
			DispValue.testbeep = 0;
			DispValue.testcomp[0] = 0;
			DispValue.testcomp[1] = 0;
			DispValue.testcomp[2] = 0;
//			Test_Comp();//分选
			Test_Beep();//讯响
		}else{
			Test_Comp();//分选
			Test_Beep();//讯响
		}
		if(DispValue.alertdisp == 0)
		{
			Test_Disp(LoadSave.vrange,LoadSave.crange);
		}else if(DispValue.alertdisp == 1){
			MissConDisp();
			Disp_Fastbutton();
			Colour.Fword=White;//
			Colour.black=LCD_COLOR_TEST_BUTON;
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
			DispValue.alertdisp = 2;
			LoadSave.ErrCnt[0]++;
			Store_set_flash();
		}
		
		
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				inputtrans = key;
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
//							switch(keynum)
//							{
//								case 0:
//									keynum=0;
//										//SetSystemStatus(SYS_STATUS_TEST);
//									break;
//								case 1:
							if(buttonpage1 == 1)
							{
								if(LoadSave.devmode != 1)
								{
									if(mainswitch == 0)
									{
										LoadSave.mode=0;
										Mode_SW();
										Store_set_flash();
										if(LoadSave.devmode == 0)
											setslaveflag=LoadSave.devnum;
									}
								}
							}else if(buttonpage1 == 2){
								if(mainswitch == 0)
								{
									LoadSave.mode=6;
									Mode_SW();
									Store_set_flash();
								}
							}else if(buttonpage1 == 3){
								if(mainswitch == 0)
								{
									SetSystemStatus(SYS_STATUS_LED);
								}
							}
//								break;

//								default:
//									break;
//							
//							
//							}
						break;
						case Key_F2:
//							switch(keynum)
//							{
//								case 0:
//									if(mainswitch == 0)
//										SetSystemStatus(SYS_STATUS_BATTERY);
//									break;
//								case 1:
							if(buttonpage1 == 1)
							{
								if(LoadSave.devmode != 1)
								{
									if(mainswitch == 0)
									{
										LoadSave.mode=1;
										Mode_SW();
										Store_set_flash();
										if(LoadSave.devmode == 0)
											setslaveflag=LoadSave.devnum;
									}
								}
							}else{
								SetSystemStatus(SYS_STATUS_BATTERY);
							}
//								break;
//								default:
//									break;
//							
//							
//							}
		//					Savetoeeprom();
						break;
						case Key_F3:
//							switch(keynum)
//							{
//								case 0:
//									if(mainswitch == 0)
//									{
//										SetSystemStatus(SYS_STATUS_DYNAMIC);
//									}
//								break;
//								case 1:
							if(buttonpage1 == 1)
							{
								if(LoadSave.devmode != 1)
								{
									if(mainswitch == 0)
									{
										LoadSave.mode=2;
										Mode_SW();
										Store_set_flash();
										if(LoadSave.devmode == 0)
											setslaveflag=LoadSave.devnum;
									}
								}
							}else{
								SetSystemStatus(SYS_STATUS_DYNAMIC);
							}
//								break;
//							
//							}
		//					Savetoeeprom();
						break;
						case Key_F4:
//							switch(keynum)//
//							{
//								case 0:
//									if(mainswitch == 0)
//									SetSystemStatus(SYS_STATUS_LIST);
//							    break;
//								case 1:
							if(buttonpage1 == 1)
							{
								if(LoadSave.devmode != 1)
								{
									if(mainswitch == 0)
									{
										LoadSave.mode=3;
										Mode_SW();
										Store_set_flash();
										if(LoadSave.devmode == 0)
											setslaveflag=LoadSave.devnum;
									}
								}
							}else{
								SetSystemStatus(SYS_STATUS_LIST);
							}
//								break;
//								default:
//									break;
//							
//							
//							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
//							switch(keynum)
//							{
//								case 0:
//								{
//									if(mainswitch == 0)
//									SetSystemStatus(SYS_STATUS_LIMITSET);
//								}break;
//								case 1:
							if(DispValue.alertdisp == 0)
							{
								if(LoadSave.devmode == 2)
								{
									buttonpage1++;
									if(buttonpage1 > 2)
										buttonpage1 = 1;
//									if(buttonpage1 == 1)
//									{
//										buttonpage1=2;
//									}else{
//										buttonpage1=1;
//									}
								}
							}else{
								if(DispValue.protectflag == 0)
								{
									DispValue.alertdisp = 0;
									LCD_Clear(LCD_COLOR_TEST_BACK);
									Disp_Test_Item();
								}
							}
//									if(mainswitch == 0)
//									{
//										LoadSave.mode=6;
//										Mode_SW();
//										Store_set_flash();
//									}
//								break;
//								default:
//									break;
//							}
	  
						break;

						case Key_LEFT:
							if(coder_flag == 1/* && keynum == 2*/)
							{
								if(spinbit < spinbitmax)
								{
									spinbit ++;
								}else{
									spinbit = 1;
								}
							}
						  
						break;
						case Key_RIGHT:
							if(coder_flag == 1/* && keynum == 2*/)
							{
								if(spinbit > 1)
								{
									spinbit --;
								}else{
									spinbit = spinbitmax;
								}
							}
						break;
					 


						
						case Key_NUM1:
//							Range++;
//						break;
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
//						break;
						case Key_DOT:
//						switch(keynum)
//						{
//							case 2:
//							{
								if(LoadSave.devmode!=1)
								{
									switch(LoadSave.mode)
									{
										case 0:
										{
											Coordinates.xpos=LIST2+88;
											Coordinates.ypos=FIRSTLINE*1;
											Coordinates.lenth=76;
											LoadSave.current=Disp_Set_Num(&Coordinates);

										}break;
										case 1:
										{
											Coordinates.xpos=LIST2+88;
											Coordinates.ypos=FIRSTLINE*1;
											Coordinates.lenth=76;
											LoadSave.voltage=Disp_Set_Num(&Coordinates);
										}break;
										case 2:
										{
											Coordinates.xpos=LIST2+88;
											Coordinates.ypos=FIRSTLINE*1;
											Coordinates.lenth=76;
											LoadSave.risistence=Disp_Set_RNum(&Coordinates);
										}break;
										case 3:
										{
											Coordinates.xpos=LIST2+88-10;
											Coordinates.ypos=FIRSTLINE*1;
											Coordinates.lenth=76;
											LoadSave.power=Disp_Set_Num(&Coordinates);
										}break;
									}
									Para_Set_Comp();
									Set_Para();
									if(LoadSave.devmode == 0)
										setslaveflag=LoadSave.devnum;
								}

//							}
//							break;
//							default:
//							break;
//						}
						break;

						case Key_REST:
							
						break;
						case Key_ONOFF:							
							if(mainswitch == 0)
							{
								switchdelay = SWITCH_DELAY;
								mainswitch = 1;
								SwitchLedOn();
//								OnOff_SW(mainswitch);
								Set_Para();
								if(LoadSave.devmode==0)
									slaveonoffflag=LoadSave.devnum;
							}else{
								switchdelay = SWITCH_DELAY;
								mainswitch = 0;
								SwitchLedOff();
//								OnOff_SW(mainswitch);
								Set_Para();
								if(LoadSave.devmode==0)
									slaveonoffflag=LoadSave.devnum;
							}
						break;
						case Key_SET1:
						{
							if(mainswitch == 0)
								
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						case Key_Ent:
						{
//							case Key_F1:
							
//							switch(keynum)
//							{
//								case 2:
//								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									switch(LoadSave.mode)
									{
										case 0:
										{
											LoadSave.current=Disp_Set_Num(&Coordinates);

										}break;
										case 1:
										{
											LoadSave.voltage=Disp_Set_Num(&Coordinates);
										}break;
										case 2:
										{
											LoadSave.risistence=Disp_Set_Num(&Coordinates);
										}break;
										case 3:
										{
											LoadSave.power=Disp_Set_Num(&Coordinates);
										}break;
									}
									Para_Set_Comp();
									Set_Para();
//								}
//								break;

//								default:
//									break;
//							
//							
//							}
						}break;
						case Key_ESC:
						{
//							if(coder_flag == 1)
//							{
//								coder_flag = 0;
//							}
						}break;
						case PRESS_SPIN:
						{
//							if(coder_flag == 0  && keynum == 2)
//							{
//								coder_flag = 1;
//							}
							if(coder_flag == 1/* && keynum == 2*/)
							{
								if(spinbit < spinbitmax)
								{
									spinbit ++;
								}else{
									spinbit = 1;
								}
							}
//							if(keynum == 1)
//							{
//								if(mainswitch == 0)
//								{
//									if(LoadSave.mode < 3)
//									{
//										LoadSave.mode++;
//									}else if(LoadSave.mode==3){
//										LoadSave.mode = 6;
//									}else{
//										LoadSave.mode = 0;
//									}
//									Mode_SW();
//									Store_set_flash();
//								}
//							}
						}break;
						case Key_SAVE:
						{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
						}break;
						case Key_ON:
						{
//							ReadSlaveData(1);
//							ReadData();
						}break;
						default:
							SetSystemStatus(SYS_STATUS_TEST);
						break;
							
					}
				}
         }
     }
}


//==========================================================
//函数名称：Test_Process
//函数功能：测试主程序
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 14:18
//备注说明：x.xx ms(12MHz/12T)
//==========================================================
void Led_Process(void)
{   
	static u8 skipkey;
	static u32 oldskip;
	char sendbuf[24];
	vu8 key;
	u8 keytrans=0;
    vu16 USB_Count=0;
    UINT fnum;
    u32 color;
    u8 keynum=0;
	vu16 i;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志

    LCD_Clear(LCD_COLOR_TEST_BACK);

    Disp_Led_Item();  
		Set_Para();
//    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
//    NVIC_EnableIRQ(EXTI3_IRQn);
//    if(Jk516save.Sys_Setvalue.u_flag)
//    {
//        i=200;
//        while(i--)
//        {
//        USBH_Process(&USB_OTG_Core, &USB_Host);
//            delay_ms(1);
//            
//        }
//    }
	while(GetSystemStatus()==SYS_STATUS_LED)
	{
        USB_Count++;
//        Key_Scan();
		keytrans=Encoder_Process(keynum);
		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
				if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
    if(Disp_Flag==1 )//显示设置的值
		{
			Disp_Led_value(keynum);
			Disp_Flag = 0;
		}  
		if(F_100ms == TRUE/* && setflag == 0*/)
		{
			ReadData();
			F_100ms=FALSE;
		}
		if(DispValue.alertdisp == 0)
		{
			Led_Disp(LoadSave.vrange,LoadSave.crange);
		}else if(DispValue.alertdisp == 1){
			MissConDisp();
			Disp_Fastbutton();
			Colour.Fword=White;//
			Colour.black=LCD_COLOR_TEST_BUTON;
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
			DispValue.alertdisp = 2;
			LoadSave.ErrCnt[0]++;
			Store_set_flash();
		}

//		Uart_Process();//串口处理
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				inputtrans = key;
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_TEST);
									}
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
								default:
									break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F3:
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_DYNAMIC);
									}
								break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
							   case 0:
										if(mainswitch == 0)
										{
											SetSystemStatus(SYS_STATUS_LIST);
										}	
									break;
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
											SetSystemStatus(SYS_STATUS_LIMITSET);
								break;
								default:
									break;
							}
	  
						break;

						case Key_LEFT:
							if(keynum<1)
								keynum=3;
							else
								keynum--;
						  
						break;
						case Key_RIGHT:
							if(keynum>2)
								keynum=0;
							else
								keynum++;
						   
					 
								
						break;

						
						case Key_NUM1:
//							Range++;
//						break;
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
//						break;
						case Key_DOT:
							switch(keynum)
							{
								case 1:
								{
									Coordinates.xpos=LIST1+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									LoadSave.ledvo=Disp_Set_VNum(&Coordinates);
								}
								break;
								case 2:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE;
									Coordinates.lenth=76;
									LoadSave.ledrd=Disp_Set_Num(&Coordinates);
								}
								break;
								case 3:
								{
									Coordinates.xpos=LIST1+88;
									Coordinates.ypos=FIRSTLINE+SPACE1*1;
									Coordinates.lenth=76;
									LoadSave.ledio=Disp_Set_CNum(&Coordinates);
								}
								break;
							}
							Para_Set_Comp();
							Set_Para();
							Store_set_flash();
						break;

						case Key_REST:
						break;
						case Key_TRIG:
							
						break;
						case Key_SET1:
						{
							if(mainswitch == 0)
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						case Key_ESC:
						{
							if(mainswitch == 0)
								SetSystemStatus(SYS_STATUS_TEST);
						}break;
						case Key_Ent:
						{
							
						}break;
						case Key_ONOFF:
						{
							if(mainswitch == 0)
							{
								switchdelay = SWITCH_DELAY;
								SwitchLedOn();
								batstep = 1;
								DispValue.Capraw = 0;
								mainswitch = 1;
								Set_Para();
							}else{
								switchdelay = SWITCH_DELAY;
								mainswitch = 0;
								Set_Para();
								SwitchLedOff();
							}
						}break;
						default:
//							SetSystemStatus(SYS_STATUS_TEST);
						break;
							
					}
				}
         }
     }
}

void Battery_Process(void)
{   
	static u8 skipkey;
	static u32 oldskip;
	char sendbuf[24];
	vu8 key;
    vu16 USB_Count=0;
    UINT fnum;
    vu8 test_Vsorting,test_Rsorting;
    u32 color;
    u8 keynum=0;
	u8 keytrans=0;
	vu16 i;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志
	F_100ms=FALSE;//100ms定时
	F_1s = FALSE;//1s定时
    LCD_Clear(LCD_COLOR_TEST_BACK);


    Disp_Battery_Item(0);  


    i=0;
	Set_Para();
//    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
//    NVIC_EnableIRQ(EXTI3_IRQn);
//    if(Jk516save.Sys_Setvalue.u_flag)
//    {
//        i=200;
//        while(i--)
//        {
//        USBH_Process(&USB_OTG_Core, &USB_Host);
//            delay_ms(1);
//            
//        }
//    }
	while(GetSystemStatus()==SYS_STATUS_BATTERY)
	{
        USB_Count++;
//        Key_Scan();
		keytrans=Encoder_Process(keynum);
		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
         if(Disp_Flag==1 )//显示设置的值
		{
			Disp_Bat_value(keynum);
			Disp_Flag = 0;
		}  
		if(F_100ms == TRUE/* && setflag == 0*/)
		{
			ReadData();
			F_100ms=FALSE;
		}
		if(DispValue.alertdisp == 0)
		{
			Bat_Disp(LoadSave.vrange,LoadSave.crange);
		}else if(DispValue.alertdisp == 1){
			MissConDisp();
			Disp_Fastbutton();
			Colour.Fword=White;//
			Colour.black=LCD_COLOR_TEST_BUTON;
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
			DispValue.alertdisp = 2;
			LoadSave.ErrCnt[0]++;
			Store_set_flash();
		}
		
        if(setflag != 0)
		{
			if(setcount == 5)
			{
				Set_Para();
				setcount = 0;
			}else{
				setcount++;
			}
		}
//		Uart_Process();//串口处理
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				inputtrans = key;
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_TEST);
									}
									break;
								case 1:
									if(mainswitch == 0)
									{
										LoadSave.loadmode=0;
										Set_Para();
										LCD_Clear(LCD_COLOR_TEST_BACK);
										Disp_Battery_Item(1);
										Store_set_flash();
									}
								break;
								default:
									break;
							
							
							}
						break;
						case Key_F2:
							switch(keynum)
							{
								case 0:
									
								break;
								case 1:
								{
									if(mainswitch == 0)
									{
										LoadSave.loadmode=2;
										Set_Para();
										LCD_Clear(LCD_COLOR_TEST_BACK);
										Disp_Battery_Item(1);
										Store_set_flash();
									}
								}break;
								default:
								break;
							
							}
		//					Savetoeeprom();
						break;
						case Key_F3:
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_DYNAMIC);
									}
								break;
								default:
								break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
							   case 0:
								if(mainswitch == 0)
								{
									SetSystemStatus(SYS_STATUS_LIST);
								}									
								break;
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							if(DispValue.alertdisp == 0)
							{
								switch(keynum)
								{
									case 0:
									{
										if(mainswitch == 0)
											SetSystemStatus(SYS_STATUS_LIMITSET);
									}break;
									case 1:
										LoadSave.mode=4;
									break;
									default:
										break;
								}
							}else{
								if(DispValue.protectflag == 0)
								{
									DispValue.alertdisp = 0;
									LCD_Clear(LCD_COLOR_TEST_BACK);
									 Disp_Battery_Item(0); 
								}
							}
	  
						break;

						case Key_LEFT:
							if(LoadSave.loadmode == 0)
							{
								if(keynum>5){
									keynum-=4;
								}else if(keynum < 3 && keynum > 0){
									keynum --;
								}
							}else if(LoadSave.loadmode == 2){
								if(keynum==6)
								{
									keynum=2;
								}else if(keynum == 5){
									keynum = 2;
								}else if(keynum == 2){
									keynum--;
								}else if(keynum == 1){
									keynum--;
								}
							}
						  
						break;
						case Key_RIGHT:
							if(LoadSave.loadmode == 0)
							{
								if(keynum<5 && keynum>1)
									keynum+=4;
								else if(keynum<2)
									keynum++;
							}else if(LoadSave.loadmode == 2){
								if(keynum == 2){
									keynum=6;
								}else if(keynum == 1){
									keynum=2;
								}else if(keynum == 0){
									keynum=1;
								}
							}
						   
					 
								
						break;

						
						case Key_NUM1:
//							Range++;
//						break;
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
//						break;
						case Key_DOT:
							switch(keynum)
							{
								case 2:
								{
									
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.loadc1=Disp_Set_CNum(&Coordinates);
										}else if(LoadSave.loadmode == 2){
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=80;
											LoadSave.loadr=Disp_Set_RNum(&Coordinates);
										}
										Para_Set_Comp();
										Set_Para();
									}
									Store_set_flash();
								}
								break;
								case 3:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*2;
											Coordinates.lenth=76;
											LoadSave.loadc2=Disp_Set_CNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 4:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*3;
											Coordinates.lenth=76;
											LoadSave.loadc3=Disp_Set_CNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 6:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.coffv1=Disp_Set_VNum(&Coordinates);
										}else if(LoadSave.loadmode == 2){
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.coffvr=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 7:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*2;
											Coordinates.lenth=76;
											LoadSave.coffv2=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 8:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*3;
											Coordinates.lenth=76;
											LoadSave.coffv3=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								default:
									break;
							
							
							}
						break;

						case Key_REST:
						break;
						case Key_ONOFF:							
							if(mainswitch == 0)
							{
								switchdelay = SWITCH_DELAY;
								SwitchLedOn();
								batstep = 1;
								DispValue.Capraw = 0;
								mainswitch = 1;
								Set_Para();
							}else{
								switchdelay = SWITCH_DELAY;
								mainswitch = 0;
								Set_Para();
								SwitchLedOff();
							}
						break;
						case Key_TRIG:
							
						break;
						case Key_SET1:
						{
							if(mainswitch == 0)
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						case Key_ESC:
						{
							if(mainswitch == 0)
								SetSystemStatus(SYS_STATUS_TEST);
						}break;
						case PRESS_SPIN:
						{
							switch(keynum)
							{
								case 1:
								{
									if(LoadSave.loadmode == 0)
									{
										if(mainswitch == 0)
										{
											LoadSave.loadmode=2;
											Set_Para();
											LCD_Clear(LCD_COLOR_TEST_BACK);
											Disp_Battery_Item(1);
											Store_set_flash();
										}
									}else if(LoadSave.loadmode == 2){
										if(mainswitch == 0)
										{
											LoadSave.loadmode=0;
											Set_Para();
											LCD_Clear(LCD_COLOR_TEST_BACK);
											Disp_Battery_Item(1);
											Store_set_flash();
										}
									}
								}
							}
						}break;
						case Key_Ent:
						{
							switch(keynum)
							{
								case 2:
								{
									
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.loadc1=Disp_Set_CNum(&Coordinates);
										}else if(LoadSave.loadmode == 2){
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=80;
											LoadSave.loadr=Disp_Set_RNum(&Coordinates);
										}
										Para_Set_Comp();
										Set_Para();
									}
									Store_set_flash();
								}
								break;
								case 3:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*2;
											Coordinates.lenth=76;
											LoadSave.loadc2=Disp_Set_CNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 4:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*3;
											Coordinates.lenth=76;
											LoadSave.loadc3=Disp_Set_CNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 6:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.coffv1=Disp_Set_VNum(&Coordinates);
										}else if(LoadSave.loadmode == 2){
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.coffvr=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 7:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*2;
											Coordinates.lenth=76;
											LoadSave.coffv2=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 8:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.loadmode == 0)
										{
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*3;
											Coordinates.lenth=76;
											LoadSave.coffv3=Disp_Set_VNum(&Coordinates);
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								default:
									break;
							
							
							}
						}break;
						case Key_SAVE:
						{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
						}break;
						default:
//							SetSystemStatus(SYS_STATUS_TEST);
						break;
							
					}
				}
         }
     }
}

void Dynamic_Process(void)
{   
	static u8 skipkey;
	static u32 oldskip;
	char sendbuf[24];
	vu8 key;
    vu16 USB_Count=0;
    UINT fnum;
    vu8 test_Vsorting,test_Rsorting;
    u32 color;
    u8 keynum=0;
	u8 keytrans=0;
	vu16 i;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志

    LCD_Clear(LCD_COLOR_TEST_BACK);
	F_100ms=FALSE;//100ms定时
    Disp_Dynamic_Item();  


    i=0;
	
//	Set_Para();

	while(GetSystemStatus()==SYS_STATUS_DYNAMIC)
	{
        USB_Count++;
//        Key_Scan();
		keytrans=Encoder_Process(keynum);
		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
         if(Disp_Flag==1 )//显示设置的值
		{
			Disp_Dyn_value(keynum);
			Disp_Flag = 0;
			
		} 
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		if(F_100ms == TRUE)
		{
			ReadData();
			F_100ms=FALSE;
		}
		if(DispValue.alertdisp == 0)
		{
			Dynamic_Disp(LoadSave.vrange,LoadSave.crange);
		}else if(DispValue.alertdisp == 1){
			MissConDisp();
			Disp_Fastbutton();
			Colour.Fword=White;//
			Colour.black=LCD_COLOR_TEST_BUTON;
			WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
			DispValue.alertdisp = 2;
			LoadSave.ErrCnt[0]++;
			Store_set_flash();
		}
		
             
//		Uart_Process();//串口处理
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				inputtrans = key;
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_TEST);
									}
									break;
								case 1:
									if(mainswitch == 0)
									{
										LoadSave.testdis=0;
										Para_Set_Comp();
										Store_set_flash();
									}
								break;
								case 5:
								{
									if(mainswitch == 0)
									{
										LoadSave.dynamode = 0;
										Para_Set_Comp();
										Set_Para();
										Store_set_flash();
									}
								}break;
								default:
									break;
								
							
							}
						break;
						case Key_F2:
							switch(keynum)
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_BATTERY);
									}
									break;
								case 1:
									if(mainswitch == 0)
									{
										LoadSave.testdis=1;
										Para_Set_Comp();
										Store_set_flash();
									}
								break;
								case 5:
								{
									if(mainswitch == 0)
									{
										LoadSave.dynamode = 1;
										Para_Set_Comp();
										Set_Para();
										Store_set_flash();
									}
								}break;
								default:
									break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F3:
							switch(keynum)
							{
								
								case 5:
								{
									if(mainswitch == 0)
									{
										LoadSave.dynamode = 2;
										Para_Set_Comp();
										Set_Para();
										Store_set_flash();
									}
								}break;
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
								case 0:
									if(mainswitch == 0)
									{
										SetSystemStatus(SYS_STATUS_LIST);
									}									
								break;
							    
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							if(DispValue.alertdisp == 0)
							{
								switch(keynum)
								{
									case 0:
									{
										if(mainswitch == 0)
											SetSystemStatus(SYS_STATUS_LIMITSET);
									}break;
	//								case 1:
	//									LoadSave.mode=4;
	//								break;
									default:
										break;
								}
							}else{
								if(DispValue.protectflag == 0)
								{
									DispValue.alertdisp = 0;
									LCD_Clear(LCD_COLOR_TEST_BACK);
									Disp_Dynamic_Item();
								}
							}
							
	  
						break;

						case Key_LEFT:
							if(keynum>4){
								keynum-=4;
							}else if(keynum == 1){
								keynum = 0;
							}
						break;
						case Key_RIGHT:
						   if(keynum<5 && keynum>0)
							{
								keynum+=4;
							}else if(keynum == 0){
								keynum = 1;
							}
						break;

						
						case Key_NUM1:
//							Range++;
//						break;
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
//						break;
						case Key_DOT:
							switch(keynum)
							{
								case 2:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1;
										Coordinates.lenth=76;
										LoadSave.valA=Disp_Set_CNum(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}
								break;
								case 3:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*2;
										Coordinates.lenth=76;
										LoadSave.valB=Disp_Set_CNum(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}break;
								case 4:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*3;
										Coordinates.lenth=76;
										LoadSave.dynaIRise=Disp_Set_CNum(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}break;
								case 6:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1;
										Coordinates.lenth=76;
										LoadSave.timeA=Disp_Set_TimeMS(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}break;
								case 7:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*2;
										Coordinates.lenth=76;
										LoadSave.timeB=Disp_Set_TimeMS(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}break;
								case 8:
								{
									if(mainswitch == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*3;
										Coordinates.lenth=76;
										LoadSave.dynaIDrop=Disp_Set_CNum(&Coordinates);
										Para_Set_Comp();
										Store_set_flash();
									}
								}break;
								default:
									break;
								
							
							}
						break;

						case Key_REST:
						break;
						case Key_TRIG:
							
						break;
						case Key_SET1:
						{
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						case Key_ONOFF:
						{
							if(mainswitch == 0)
							{
								switchdelay = SWITCH_DELAY;
								SwitchLedOn();
								batstep = 1;
								DispValue.Capraw = 0;
								mainswitch = 1;
								Set_Para();
							}else{
								switchdelay = SWITCH_DELAY;
								mainswitch = 0;
								Set_Para();
								SwitchLedOff();
							}
						}break;
						case Key_Ent:
						{
							if(mainswitch == 1 && (LoadSave.dynamode == 1 || 
								LoadSave.dynamode == 2))
							{
								Set_Para();
							}
						}break;
						case Key_ESC:
						{
							if(mainswitch == 0)
								SetSystemStatus(SYS_STATUS_TEST);
						}break;
						case PRESS_SPIN:
						{
							switch(keynum)
							{
								case 1:
									if(mainswitch == 0)
									{
										if(LoadSave.testdis == 1)
										{
											LoadSave.testdis=0;
										}else if(LoadSave.testdis == 0){
											LoadSave.testdis=1;
										}
										Para_Set_Comp();
										Store_set_flash();
									}
								break;
								case 5:
								{
									if(mainswitch == 0)
									{
										if(LoadSave.dynamode < 2)
										{
											LoadSave.dynamode ++;
										}else{
											LoadSave.dynamode = 0;
										}
										Para_Set_Comp();
										Set_Para();
										Store_set_flash();
									}
								}break;
							}
						}break;
						case Key_SAVE:
						{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
						}break;
						default:
//							SetSystemStatus(SYS_STATUS_TEST);
						break;
							
					}
				}
         }
     }
}

void listreset(void)
{
	u8 i;
	for(i=0;i<15;i++)
	{
		DispValue.listcompres[i] = 0;
	}
	resdisp = 0;
}
void ReSend(u8 cmd)
{
	if(cmd == 1)
	{
		Set_Para();
	}else if(cmd == 2){
		mainswitch = 0;
		OnOff_SW(mainswitch);
	}else if(cmd == 3){
		mainswitch = 1;
		OnOff_SW(mainswitch);
	}
}
void List_Process(void)
{
	static u8 skipkey;
	static u32 oldskip;
	char sendbuf[24];
	vu8 key;
	vu16 USB_Count=0;
	UINT fnum;
	vu8 test_Vsorting,test_Rsorting;
	u32 color;
	u8 keynum=0;
	u8 keytrans=0;
	vu16 i;
  Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//显示标志
	DispValue.listrunstep = 0;//每次进入列表后步骤复位
	DispValue.liststep = 0;//每次进入列表后步骤复位
  LCD_Clear(LCD_COLOR_TEST_BACK);


  Disp_List_Item();  


    i=0;
	Set_Para();

	startdelay=STARTDELAY;
	while(GetSystemStatus()==SYS_STATUS_LIST)
	{
        USB_Count++;
//        Key_Scan();
		if(resdisp == 0)
		{
			keytrans=Encoder_Process(keynum);
		}
//		spinvalue = TIM_GetCounter(TIM3);

		if(spinflag == 1)
		{
			Disp_Flag = 1;
			keynum = keytrans;
		}
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1秒更新一次
            Int_Pe3flag=0;
            
        }
		
    if(Disp_Flag==1 )//显示设置的值
		{
			Disp_List_value(keynum);
			Disp_Flag = 0;
		}  
		if(resflag == 1)
		{
			DispValue.respage = 0;
			Disp_Res_Sheet(LoadSave.ListNum,DispValue.respage);
			resdisp = 1;
			resflag = 0;
			if(listbeep == 1)
			{
				ListCompBeep();
				listbeep = 0;
			}
		}
		
		if(setflag != 0/* && readflag == 0*/)
		{
			
			if(setcount == 5)
			{
				resendcount++;
				ReSend(setflag);
//				setflag=0;
				setcount = 0;
			}else{
				setcount++;
			}
		}
		if(F_100ms == TRUE && setflag == 0/* && resdisp == 0 && setflag == 0*/)
		{
			ReadData();
			readflag=1;
			F_100ms=FALSE;
		}
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		
//		if(mainswitch == 1)
//		{
		if(resdisp != 1)
		{
			if(DispValue.alertdisp == 0)
			{
				Disp_List_Process();
				if(LoadSave.StepMode == 2 && mainswitch == 0 && LoadSave.gatev > 500)
				{
					if(startdelay == 0)
					{
						if(LoadSave.vrange == 0)
						{
							if(DispValue.Voltage >= LoadSave.gatev)
							{
								if(mainswitch == 0)
								{
									switchdelay = SWITCH_DELAY;
									mainswitch = 1;
									listreset();
									SwitchLedOn();
									Set_Para();
				//								OnOff_SW(mainswitch);
									DispValue.listdelay = LoadSave.delay[0];
								}
							}
						}else if(LoadSave.vrange == 1){
							if(DispValue.Voltage*10 >= LoadSave.gatev)
							{
								if(mainswitch == 0)
								{
									switchdelay = SWITCH_DELAY;
									mainswitch = 1;
									listreset();
									SwitchLedOn();
									Set_Para();
				//								OnOff_SW(mainswitch);
									DispValue.listdelay = LoadSave.delay[0];
								}
							}
						}
					}else{
						startdelay--;
					}
				}
			}else if(DispValue.alertdisp == 1){
				MissConDisp();
				Disp_Fastbutton();
				Colour.Fword=White;//
				Colour.black=LCD_COLOR_TEST_BUTON;
				WriteString_16(BUTTOM_X_VALUE+4*BUTTOM_MID_VALUE, BUTTOM_Y_VALUE, "确认",  0);
				DispValue.alertdisp = 2;
				LoadSave.ErrCnt[0]++;
				Store_set_flash();
			}
		}else{
			if(LoadSave.StepMode == 2)
			{
				startdelay=STARTDELAY;
				if(LoadSave.vrange == 0)
				{
					if(DispValue.Voltage < LoadSave.gatev)
						{
							resdisp = 0;
							List_Process();
						}
					}else if(LoadSave.vrange == 1){
						if(DispValue.Voltage*10 < LoadSave.gatev)
						{
							resdisp = 0;
							List_Process();
						}
				}
			}
		}
//		else{
//			if(LoadSave.StepMode == 2)
//			{
//				if(DispValue.Voltage < LoadSave.gatev)
//					{
//						resdisp = 0;
//						List_Process();
//					}
//				}else if(LoadSave.vrange == 1){
//					if(DispValue.Voltage*10 < LoadSave.gatev)
//					{
//						resdisp = 0;
//						List_Process();
//					}
//			}
//		}
		
//		}
//		Uart_Process();//串口处理
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				if(key!=KEY_NONE)
				{
					if(resdisp == 0)
					{
						inputtrans = key;
						switch(key)
						{
								case Key_F1:
									
									switch(keynum)
									{
										case 0:
//											if(mainswitch == 0)
//												SetSystemStatus(SYS_STATUS_TEST);
											break;

										case 2:
										{
											if(mainswitch == 0)
											{
												LoadSave.StepMode = 0;
												Store_set_flash();
											}
										}
										break;
										case 13:
										{
											if(mainswitch == 0)
											{
												LoadSave.LoopTest = 0;
												Store_set_flash();
											}
										}
										break;
										case 4:
										{
											if(mainswitch == 0)
											{
												LoadSave.ListBeep = 0;
												Store_set_flash();
											}
										}
										break;
										
										case 7:
										{
											if(mainswitch == 0)
											{
												LoadSave.listmode[DispValue.liststep] = 0;
												Store_set_flash();
											}
										}break;
										
										case 10:
										{
											if(mainswitch == 0)
											{
												LoadSave.listcomp[DispValue.liststep] = 0;
												Store_set_flash();
											}
										}break;
										
										default:
											break;
									
									
									}
								break;
								case Key_F2:
									switch(keynum)
									{
										case 0:
//											if(mainswitch == 0)
//												SetSystemStatus(SYS_STATUS_BATTERY);
										break;
										case 1:
										{
											if(mainswitch == 0)
											{
												LoadSave.loadmode=2;
												BatMode_SW();
												LCD_Clear(LCD_COLOR_TEST_BACK);
												Disp_Battery_Item(1);
												Store_set_flash();
											}
										}break;
										case 2:
										{
											if(mainswitch == 0)
											{
												LoadSave.StepMode = 1;
												Store_set_flash();
											}
										}
										break;
										case 13:
										{
											if(mainswitch == 0)
											{
												LoadSave.LoopTest = 1;
												Store_set_flash();
											}
										}
										break;
										case 4:
										{
											if(mainswitch == 0)
											{
												LoadSave.ListBeep = 1;
												Store_set_flash();
											}
										}
										break;
										case 7:
										{
											if(mainswitch == 0)
											{
												LoadSave.listmode[DispValue.liststep] =1;
												Store_set_flash();
											}
										}break;
										
										case 10:
										{
											if(mainswitch == 0)
											{
												LoadSave.listcomp[DispValue.liststep] = 1;
												Store_set_flash();
											}
										}break;
										default:
										break;
									
									}
				//					Savetoeeprom();
								break;
								case Key_F3:
									switch(keynum)
									{
										case 0:
//											if(mainswitch == 0)
//											{
//												SetSystemStatus(SYS_STATUS_DYNAMIC);
//											}
										break;
										case 2:
										{
											if(mainswitch == 0)
											{
												LoadSave.StepMode = 2;
												Store_set_flash();
											}
										}
										case 7:
										{
											if(mainswitch == 0)
											{
												LoadSave.listmode[DispValue.liststep] = 2;
												Store_set_flash();
											}
										}break;
										case 10:
										{
											if(mainswitch == 0)
											{
												LoadSave.listcomp[DispValue.liststep] = 2;
												Store_set_flash();
											}
										}break;
									
									}
				//					Savetoeeprom();
								break;
								case Key_F4:
									switch(keynum)//
									{
									   case 0:
												resflag=1;
											break;
										case 7:
										{
											if(mainswitch == 0)
											{
												LoadSave.listmode[DispValue.liststep] = 3;
												Store_set_flash();
											}
										}break;
										case 10:
										{
											if(mainswitch == 0)
											{
												LoadSave.listcomp[DispValue.liststep] = 3;
												Store_set_flash();
											}
										}break;
										default:
											break;
									
									
									}	
				//					Savetoeeprom();
								break;
								case Key_F5:
									if(DispValue.alertdisp == 0)
									{
										switch(keynum)
										{
											case 0:
											{
												if(mainswitch == 0)
													SetSystemStatus(SYS_STATUS_TEST);
											}
											case 1:
	//											LoadSave.mode=4;
											break;
											case 7:
											{
												if(mainswitch == 0)
												{
													LoadSave.listmode[DispValue.liststep] = 4;
													Store_set_flash();
												}
											}break;
											default:
												break;
										}
									}else{
										if(DispValue.protectflag == 0)
										{
											DispValue.alertdisp = 0;
											LCD_Clear(LCD_COLOR_TEST_BACK);
											Disp_List_Item();
										}
									}
									
			  
								break;

								case Key_LEFT:
		//							if(keynum<1)
		//								keynum=11;
		//							else
		//								keynum--;
									if(DispValue.liststep > 0)
									{
										DispValue.liststep --;
									}
								break;
								case Key_RIGHT:
									if(DispValue.liststep < LoadSave.ListNum-1)
									{
										DispValue.liststep ++;
									}
		//							if(keynum>10)
		//								keynum=0;
		//							else
		//								keynum++;
								   
							 
										
								break;

								
								case Key_NUM1:
		//							Range++;
		//						break;
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
		//						break;
								case Key_DOT:
									switch(keynum)
									{

										case 1:
											if(mainswitch == 0)
											{
												Coordinates.xpos=LIST1+118;
												Coordinates.ypos=FIRSTLINE;
												Coordinates.lenth=76;
												LoadSave.ListNum=Disp_Set_Step(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										break;
										case 3:
										if(mainswitch == 0)
										{
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*2;
											Coordinates.lenth=76;
											LoadSave.gatev=Disp_Set_VNum(&Coordinates);
											Para_Set_Comp();
											Store_set_flash();
										}
										break;

										case 5:
											if(mainswitch == 0)
											{
												Coordinates.xpos=LIST2+118;
												Coordinates.ypos=FIRSTLINE+SPACE1*1;
												Coordinates.lenth=76;
												LoadSave.listonvol=Disp_Set_VNum(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										break;


										case 8:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+4*22;
												Coordinates.lenth=76;
												if(LoadSave.listmode[DispValue.liststep] == 0)
												{
													LoadSave.listvalue[DispValue.liststep]=Disp_Set_CNum(&Coordinates);
												}else if(LoadSave.listmode[DispValue.liststep] == 1){
													LoadSave.listvalue[DispValue.liststep]=Disp_Set_VNum(&Coordinates);
												}else if(LoadSave.listmode[DispValue.liststep] == 2){
													LoadSave.listvalue[DispValue.liststep]=Disp_Set_RNum(&Coordinates);
												}else if(LoadSave.listmode[DispValue.liststep] == 3){
													LoadSave.listvalue[DispValue.liststep]=Disp_Set_TimeP(&Coordinates);
												}
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										case 9:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+5*22;
		 										Coordinates.lenth=76;
												LoadSave.delay[DispValue.liststep]=Disp_List_Delay(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										}
										break;

										case 11:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+7*22;
												Coordinates.lenth=76;
												if(LoadSave.listcomp[DispValue.liststep] == 0)
												{
												}else if(LoadSave.listcomp[DispValue.liststep] == 1){
													LoadSave.listhigh[DispValue.liststep]=Disp_Set_CNum(&Coordinates);
												}else if(LoadSave.listcomp[DispValue.liststep] == 2){
													LoadSave.listhigh[DispValue.liststep]=Disp_Set_VNum(&Coordinates);
												}else if(LoadSave.listcomp[DispValue.liststep] == 3){
													LoadSave.listhigh[DispValue.liststep]=Disp_Set_TimeP(&Coordinates);
												}
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										case 12:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+8*22;
												Coordinates.lenth=76;
												if(LoadSave.listcomp[DispValue.liststep] == 0)
												{
												}else if(LoadSave.listcomp[DispValue.liststep] == 1){
													LoadSave.listlow[DispValue.liststep]=Disp_Set_CNum(&Coordinates);
												}else if(LoadSave.listcomp[DispValue.liststep] == 2){
													LoadSave.listlow[DispValue.liststep]=Disp_Set_VNum(&Coordinates);
												}else if(LoadSave.listcomp[DispValue.liststep] == 3){
													LoadSave.listlow[DispValue.liststep]=Disp_Set_TimeP(&Coordinates);
												}
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										default:
											break;
									
									
									}
								break;

								case Key_REST:
								break;
								case Key_ONOFF:	
									if(listonoffdelay == 0)
									{
										listonoffdelay=50;
										if(mainswitch == 0)
										{
											if(LoadSave.StepMode == 0)
											{
												switchdelay = SWITCH_DELAY;
												mainswitch = 1;
												setflag=1;
	//											Set_Para();
												listreset();
												SwitchLedOn();
												
				//								OnOff_SW(mainswitch);
												DispValue.listdelay = LoadSave.delay[0];
												keynum=0;
											}else{
												if(DispValue.listrunstep != 0)
												{
													mainswitch = 1;
													SwitchLedOn();
													setflag=1;
	//												Set_Para();
												}else{
													switchdelay = SWITCH_DELAY;
													mainswitch = 1;
													setflag=1;
													listreset();
													SwitchLedOn();
	//												Set_Para();
					//								OnOff_SW(mainswitch);
													DispValue.listdelay = LoadSave.delay[0];
												}
											}
										}else{
											startdelay=STARTDELAY;
											DispValue.listrunstep = 0;
											listtime=0;
											mainswitch = 0;
											setflag=1;
											SwitchLedOff();
	//										Set_Para();
			//								OnOff_SW(mainswitch);
										}
									}
								break;
								case Key_TRIG:
									
								break;
								case Key_SET1:
								{
									if(mainswitch == 0)
									SetSystemStatus(SYS_STATUS_SETUP);
								}break;
								case Key_ESC:
								{
									if(mainswitch == 0)
										SetSystemStatus(SYS_STATUS_TEST);
								}break;
								case Key_Ent:
								{									
									switch(keynum)
									{
										case 1:
											if(mainswitch == 0)
											{
												Coordinates.xpos=LIST1+118;
												Coordinates.ypos=FIRSTLINE;
												Coordinates.lenth=76;
												LoadSave.ListNum=Disp_Set_Step(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										break;
										case 5:
											if(mainswitch == 0)
											{
												Coordinates.xpos=LIST2+118;
												Coordinates.ypos=FIRSTLINE+SPACE1*1;
												Coordinates.lenth=76;
												LoadSave.listonvol=Disp_Set_Num(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										break;
										case 8:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+4*22;
												Coordinates.lenth=76;
												LoadSave.listvalue[DispValue.liststep]=Disp_Set_Num(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										case 9:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+5*22;
												Coordinates.lenth=76;
												LoadSave.delay[DispValue.liststep]=Disp_List_Delay(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										}
										break;
										case 11:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+7*22;
												Coordinates.lenth=76;
												LoadSave.listhigh[DispValue.liststep]=Disp_Set_Num(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										case 12:
										{
											if(mainswitch == 0)
											{
												Coordinates.xpos=100;
												Coordinates.ypos=26+8*22;
												Coordinates.lenth=76;
												LoadSave.listlow[DispValue.liststep]=Disp_Set_Num(&Coordinates);
												Para_Set_Comp();
												Store_set_flash();
											}
										}break;
										default:
											break;
									
									
									}
								}break;
								case PRESS_SPIN:
								{
									if(mainswitch == 0)
									{
										switch(keynum)
										{
											case 2:
											{
												if(LoadSave.StepMode < 2)
													LoadSave.StepMode ++;
												else
													LoadSave.StepMode = 0;
												Store_set_flash();
											}break;
											case 4:
											{
												if(LoadSave.ListBeep == 0)
													LoadSave.ListBeep = 1;
												else
													LoadSave.ListBeep = 0;
												Store_set_flash();
											}break;
											case 6:
											{
												if(DispValue.liststep < LoadSave.ListNum-1)
												{
													DispValue.liststep ++;
												}else{
													DispValue.liststep = 0;
												}
											}break;
											case 7:
											{
												if(LoadSave.listmode[DispValue.liststep] < 4)
													LoadSave.listmode[DispValue.liststep] ++;
												else
													LoadSave.listmode[DispValue.liststep] = 0;
												Store_set_flash();
											}break;
											case 10:
											{
												if(LoadSave.listcomp[DispValue.liststep] < 3)
													LoadSave.listcomp[DispValue.liststep] ++;
												else
													LoadSave.listcomp[DispValue.liststep] = 0;
												Store_set_flash();
											}break;
										}
									}
								}break;
								case Key_SAVE:
								{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
								}break;
								default:
//									SetSystemStatus(SYS_STATUS_TEST);
								break;
									
							}
						}else if(resdisp == 1){
							switch(key)
							{
								case Key_F5:
								{
									resdisp = 0;
									List_Process();
								}break;
								case Key_F3:
								{
									Disp_Flag = 0;
									if(DispValue.respage > 0)
									{
										DispValue.respage--;
									}
									Disp_Res_Sheet(LoadSave.ListNum,DispValue.respage);
								}break;
								case Key_F4:
								{
									Disp_Flag = 0;
									if(LoadSave.ListNum > 5 && LoadSave.ListNum <= 10)
									{
										if(DispValue.respage < 1)
										{
											DispValue.respage++;
										}
									}else if(LoadSave.ListNum > 10){
										if(DispValue.respage < 2)
										{
											DispValue.respage++;
										}
									}
									Disp_Res_Sheet(LoadSave.ListNum,DispValue.respage);
								}break;
								case Key_ONOFF:
								{
									if(listonoffdelay == 0)
									{
										listonoffdelay=50;
										resdisp = 0;
										
										switchdelay = SWITCH_DELAY;
										mainswitch = 1;
										listreset();
										SwitchLedOn();
										Set_Para();
										DispValue.listdelay = LoadSave.delay[0];
										List_Process();
									}
								}break;
								case Key_SAVE:
								{
#if(BMP_SWITCH)
					result=Screen_shot(0,0,480,272,(const char *)bmpname);
#endif
								}break;
								default:
									Disp_Flag = 0;
								break;
							}
						}
					}
				}
         }
     }
}
//==========================================================
//函数名称：TestAbort_Process
//函数功能：测试中止处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26 
//修改日期：2015.10.29 13:35
//备注说明：无
//==========================================================
//void TestAbort_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Plc_Start_Off();//关远控启动

// 	//讯响控制
////	if(SaveData.Parameter.Beep)
////		Beep_One();//响一声

//	//显示信息
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	
//	//DAC_SetChannel1Data(DAC_Align_12b_R,0);
////	Led_HV_Off();//关测试灯
////	delay_ms(30);
////	Electro_Test(0);
////	delay_ms(30);
////	
////	Electro_discharge(0);
////	delay_ms(30);
////	BeepOff();
////	
//	while(GetSystemStatus()==SYS_STATUS_TEST_ABORT)
//	{
////		Uart_Process();//串口处理
//		
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/20);//等待按键(100*10ms/20=50ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;

//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试开始
//				break;
//	
//			case KEY_ENTER:		//确认键
//			case L_KEY_ENTER:	//长按确认键
//				break;
//	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//系统状态-待机
////				break;
//			
//			default:
//				break;
//		}
//	}
//	Test_value.Test_Time=0;
//}
//==========================================================
//函数名称：TestFinish_Process
//函数功能：测试结束处理
//入口参数：无
//出口参数：无
//创建日期：2015.10.26
//修改日期：2015.10.29 13:38
//备注说明：无
//注意事项：无
//==========================================================
//void TestFinish_Process(void)
//{
//	u8 key;

//	//输出关闭
////	Led_HV_Off();//关闭高压灯
////	Plc_Start_Off();//关远控启动
//	//显示信息
//	//SetSystemMessage(MSG_PASS);
//	
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//设置坐标
////	Disp_SysMessage();//显示信息
//	//分选讯响控制

//	
//	Electro_discharge(0);
//	while(GetSystemStatus()==SYS_STATUS_TEST_FINISH)
//	{
////		Uart_Process();//串口处理
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/50);//等待按键(100*10ms/50=20ms)
//		switch(key)
//		{
//			case KEY_SET:	//设置键
//			case L_KEY_SET:	//长按设置键
//				SetSystemStatus(SYS_STATUS_SETUP);//设置状态
//				break;
//	
//			case KEY_UP:	//上键
//			case L_KEY_UP:	//长按上键
//				break;
//	
//			case KEY_DOWN:		//下键
//			case L_KEY_DOWN:	//长按下键
//				break;
//	
//			case KEY_START:		//启动键
//			case L_KEY_START:	//长按启动键
//				SetSystemStatus(SYS_STATUS_TEST);//测试状态
//				break;
//	
//			case KEY_ENTER:		//确认键
////			case L_KEY_ENTER:	//长按确认键
////				SetSystemStatus(SYS_STATUS_IDLE);//待测状态
////				break;
////	
////			case KEY_RESET:		//复位键
////			case L_KEY_RESET:	//长按复位键
////				SetSystemStatus(SYS_STATUS_IDLE);//待机状态
////				break;
//			
//			default:
//				break;
//		}
//	}
//}	

//用户校正
void Use_DebugProcess(void)
{
	
	vu32 keynum=0;
    float a,b;
	vu8 key;
//    u8 Disp_Testflag=0;
    vu8 range_flag=0;
//    float i_mid;
//	float ddd,eee;
	Disp_Coordinates_Typedef Coordinates;
    Disp_Coordinates_Typedef Debug_Cood;
    vu8 i;
	vu8 Disp_flag=1;
    vu8 list=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
	Disp_UserCheck_Item();
	Set_Para();
//    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
//    NVIC_EnableIRQ(EXTI3_IRQn);
 	while(GetSystemStatus()==SYS_STATUS_DEBUG)
	{
		
		Encoder_Process(list);
		if(Disp_flag==1)//不启动的时候
		{
			Disp_Debug_value(list);
			Disp_flag=0;	
		}
    
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);
		
		if(key!=KEY_NONE)
		{
			Disp_flag=1;
			//Key_Beep();
			switch(key)
			{
				case Key_F1:
					calpage = 0;
					LCD_Clear(LCD_COLOR_TEST_BACK);
					Disp_UserCheck_Item();
				break;
				case Key_F2:
					calpage = 1;
					LCD_Clear(LCD_COLOR_TEST_BACK);
					Disp_UserCheck_Item();
				break;
				case Key_F3:
					if(calpage == 0)
					{
						if(list == 1||list == 2)
						{
							LoadSave.vrange = 0;
							V_Gear_SW();
						}else if(list >= 3&&list <= 8){
							LoadSave.vrange = 1;
							V_Gear_SW();
						}else if(list == 9||list == 10){
							LoadSave.vrange = 0;
							V_Gear_SW();
						}else if(list >= 11&&list <= 16){
							LoadSave.vrange = 1;
							V_Gear_SW();
						}
					}else if(calpage == 1){
						if(list >= 1&&list <= 4)
						{
							LoadSave.crange = 0;
							I_Gear_SW();
						}else if(list >= 5&&list <= 9){
							LoadSave.crange = 1;
							I_Gear_SW();
						}
					}
				break;
				case Key_F4:
					Set_Dac(list);
				break;
				case Key_F5:
					CalHandle(list);
				break;

				
				case Key_LEFT:
					if(calpage == 0)
					{
						if(list<1){
							list=DEBUG_RANGE;
						}else{
							list --;
						}
					}else if(calpage == 1){
						if(list<1){
							list=9;
						}else{
							list --;
						}
					}
					if(calpage == 0)
					{
						LoadSave.mode = 1;
					}else{
						LoadSave.mode = 0;
					}
					Mode_SW();
				break;
				case Key_RIGHT:
					if(calpage == 0)
					{
						if(list>15){
							list=0;
						}else{
							list ++;
						}
					}else if(calpage == 1){
						if(list>8){
							list=0;
						}else{
							list ++;
						}
					}
					if(calpage == 0)
					{
						LoadSave.mode = 1;
					}else{
						LoadSave.mode = 0;
					}
					Mode_SW();
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
					if(calpage == 0)
					{
						if(list < 11)
						{
							Coordinates.xpos=LIST1+160;
							Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list-1);
							Coordinates.lenth=70;
							DispValue.CalValue[list-1] = Disp_Set_Num(&Coordinates);
						}else{
							Coordinates.xpos=LIST2+100;
							Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list-11);
							Coordinates.lenth=70;
							DispValue.CalValue[list-1] = Disp_Set_Num(&Coordinates);
						}
					}else if(calpage == 1){
						Coordinates.xpos=LIST1+160;
						Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list-1);
						Coordinates.lenth=70;
						DispValue.CalValue[list+16-1] = Disp_Set_Num(&Coordinates);
					}
				break;
				
				case Key_REST:
          LoadSave.jkflag=!LoadSave.jkflag;          
					Store_set_flash();
				break;
				case Key_TRIG:
				break;
				case Key_ONOFF:
				{
					if(mainswitch == 0)
					{
						mainswitch = 1;
						OnOff_SW(mainswitch);
						SwitchLedOn();
					}else{
						mainswitch = 0;
						OnOff_SW(mainswitch);
						SwitchLedOff();
					}
				}break;
				case Key_SET1:
				{
					LoadSave.ErrCnt[0] = 0;//超功率报警计数
					Store_set_flash();
					LCD_Clear(LCD_COLOR_TEST_BACK);
					Disp_UserCheck_Item();
				}break;
				case Key_SET2:
				{
					Debug_Cood.xpos=70;
					Debug_Cood.ypos =272-70;
					Debug_Cood.lenth=120;
					input_num(&Debug_Cood);
				}break;
				case PRESS_SPIN:
				{
					if(dacctrl == 100)
					{
						dacctrl = 1000;
					}else if(dacctrl == 1000){
						dacctrl = 100;
					}
				}break;
				case Key_SHIFT:
				{
					if(LoadSave.Version < 6)//0-1200;1-800;2-600;3-400;4-2400;5-3500;6-500V
					{
						LoadSave.Version++;
					}else{
						LoadSave.Version=0;
					}
					Store_set_flash();
					LCD_Clear(LCD_COLOR_TEST_BACK);
					Disp_UserCheck_Item();
				}break;
				case Key_ESC:
				{
					if(mainswitch == 0)
						SetSystemStatus(SYS_STATUS_TEST);
				}break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}

//输出电阻切换
//输入 ： 0 1 2 3
// 0  0R  1  10K  2  100K
//
//
//
//void Internal_ResChange(u8 dat)
//{
//	u8 value;
//	value=U16_4094;
//	value&=HW_GYB_10KR;
//	switch(dat)
//	{
//		case 0:
//			value|=HW_GYB_0R;
//			break;
//		case 1:
//			
//			break;
//		case 2:
//			value|=HW_GYB_100KR;
//			break;
//			
//		default:
//			value&=HW_GYB_10KR;
//			break;
//	}
//	U16_4094=value;
//	_4094_data( );
//	Out_4094(_4094_databuff);
//	delay_ms(20);;

//}
//void Disp_Overtest(void)
//{


//}

