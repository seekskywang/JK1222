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

//�����л�
// ����0 1 2 3  U16_4094
// 0  1000V
//1   100V
//2 10V 
//==========================================================
#define POWERON_DISP_TIME (100)	//������ʾ������ʱ20*100mS=2s
u8 U15_4094,U16_4094;

FRESULT result;
FATFS fs;
FIL file;
u8 keyvalue;
u16 spinvalue;
u8 mainswitch;
u8 spinbit;
u8 spinbitmax;
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
		RTC_ITConfig(RTC_IT_WUT,DISABLE);//�ر�WAKE UP ��ʱ���ж�
		RTC_WakeUpCmd( DISABLE);//�ر�WAKE UP ��ʱ����
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
}


//};
//==========================================================
//�������ƣ�Power_Process
//�������ܣ��ϵ紦��
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26
//�޸����ڣ�2015.10.26 08:53
//��ע˵������������SET����У׼����ģʽ
//==========================================================
//void DIS_Line(void)
//{
//  uint16_t i,j;
//  
//  uint32_t *p = (uint32_t *)(LCD_LAYER2_START_ADDR );
//  
//  for(j=0;j<480;j++)
//  for(i=0;i<272;i++)
//  {
//    //ARGB8888 
//    *p = 0x7Fff00;
//    p++;
//    }
//  
//  
//}  
void Power_Process(void)
{
	u16 i;
    u8 j;

    Disp_Coordinates_Typedef Debug_Cood;
   
	u8 key;
    Int_Pe3flag=0;
	
    RTC_CLK_Config();
    RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);		//����WAKE UP�ж�,1�����ж�һ��
    GPIO_Configuration();//�˿ڳ�ʼ��
     if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
      {
        /* ����ʱ������� */
            RTC_TimeAndDate_Set();
      }
      else
      {
        /* ����Ƿ��Դ��λ */   
        /* ʹ�� PWR ʱ�� */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
            /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
            PWR_BackupAccessCmd(ENABLE);
            /* �ȴ� RTC APB �Ĵ���ͬ�� */
            RTC_WaitForSynchro();   
      } 
    
//    delay_ms(10);
	 Debug_USART_Config(DEBUG_USART_BAUDRATE);
    Keyboard_Init();//������ʼ��
	Spin_Init();

    //////////////////////////////////////////Һ����ʼ��
    LCD_Init();
    LCD_LayerInit();
    LTDC_Cmd(ENABLE);
	/*�ѱ�����ˢ��ɫ*/
    LCD_SetLayer(LCD_BACKGROUND_LAYER);  
    LCD_Clear(LCD_COLOR_RED);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
//	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
    LCD_SetTransparency(0xff);
	LCD_Clear(LCD_COLOR_TEST_BACK);
    SPI_FLASH_Init();

	InitGlobalValue();//��ʼ��ȫ�ֱ���
	
    TIM6_Configuration();//��ʱ��6��ʱ10ms

	MenuIndex=0;//�����˵���
//    EXTI_Ads1251_Config();//�ж�û�п���

	i=0;//��ʾ��ʱ
	Read_set_flash();
//     USBH_Init(&USB_OTG_Core,
//			USB_OTG_HS_CORE_ID,
//            &USB_Host,
//            &USBH_MSC_cb,
//            &USR_cb);
//     USBH_Process(&USB_OTG_Core, &USB_Host);
	Beep_Off();
	READ_COMP();
	while(GetSystemStatus()==SYS_STATUS_POWER)
	{
		i++;
        j++;
       
		if(i>POWERON_DISP_TIME)//��ʱ20*100mS=2s���Զ��˳�
            SetSystemStatus(SYS_STATUS_TEST);//����״̬
//		else
//			SetSystemStatus(SYS_STATUS_TEST);//����״̬

        
		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//�ȴ�����(100*10ms/10=100ms)

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
//�������ƣ�Idle_Process
//�������ܣ�����������
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26
//�޸����ڣ�2015.10.26 08:59
//��ע˵������
//==========================================================
//void Idle_Process(void)
//{
//	u8 key;
//	u8 disp_flag=0;
//	u8 group;

//	//ϵͳ��Ϣ����
//	SetSystemMessage(MSG_IDLE);//ϵͳ��Ϣ-����
//	Test_value.Test_Time=0;

//	
//	while(GetSystemStatus()==SYS_STATUS_IDLE)
//	{
//		//Uart_Process();//���ڴ���
////		Led_Pass_On();
////		Led_Pass_Off();
////		Led_Fail_On();
////		Led_Fail_Off();
////		Led_HV_On();
//		//Range_Control(0);
//		//Range_Control(1);
//		//Range_Control(2);
//		//Range_Control(3);
////		LcdAddr.x=6;//��ʾ��ַ
////			LcdAddr.y=0;
////			Hex_Format(Test_Time,2,4,TRUE);//��ֵ��ʽ����4λ��ֵ
////			Disp_StrAt(DispBuf);//��ʾ�˵�ֵ
//		if(disp_flag)
//		{
//			disp_flag=0;
////			Read_compvalue(group-1);
////			Disp_Idle_Menu();//��ʾ�������
//			//Store_set_flash(SaveData.group-1);
//		
//		}
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/10);//�ȴ�����(100*10ms/10=100ms)
//		switch(key)
//		{
//			case KEY_SET:	//���ü�
//			case L_KEY_SET:	//�������ü�
//				SetSystemStatus(SYS_STATUS_SETUP);//����״̬
//				break;
//	
//			case KEY_UP:	//�ϼ�
//				group=group<5?++group:1;
//				SaveData.group=group;
//				disp_flag=1;
////			case L_KEY_UP:	//�����ϼ�
//				break;
//	
//			case KEY_DOWN:		//�¼�
//				group=group>1?--group:5;
//				SaveData.group=group;
//				disp_flag=1;
////			case L_KEY_DOWN:	//�����¼�
//				break;
//	
//			case KEY_LEFT:		//���
//			case L_KEY_LEFT:	//�������
//				break;

//			case KEY_RIGHT:		//�Ҽ�
//			case L_KEY_RIGHT:	//�����Ҽ�
//				break;

//			case KEY_START:		//������
//			case L_KEY_START:	//����������
////				if(SaveData.System.Uart!=TRUE)//���ڿ�ʼʱ��������Ч
//					SetSystemStatus(SYS_STATUS_TEST);//��������״̬
//				break;
//	
//			case KEY_ENTER:		//ȷ�ϼ�
//			case L_KEY_ENTER:	//����ȷ�ϼ�
//				break;
//	
//			case KEY_RESET:		//��λ��
//			case L_KEY_RESET:	//������λ��
////				Disp_Clr( );//����
////				Disp_Idle_Menu();//��ʾ�������
//				break;
//			
//			default:
//				break;
//		}
//	}
//}


//==========================================================
//�������ƣ�Setup_Process
//�������ܣ����ó���
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.28
//�޸����ڣ�2015.10.28 10:45
//��ע˵������
//==========================================================
void Setup_Process(void)
{

	u8 keynum=0;
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key;
	vu8 Disp_Flag=1;
	keynum=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
    Disp_Test_Set_Item();
 	while(GetSystemStatus()==SYS_STATUS_SETUP)
	{
	    
		if(Disp_Flag==1)
		{
			DispSet_value(keynum);
           
			Disp_Flag=0;
		
		}

        key=Key_Read();
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
						case 4://������ѹ
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.onvol=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 5://�ضϵ�ѹ
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.offvol=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 6://��ѹ�½�
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cvdowntime=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 8://������
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*1-2;
							Coordinates.lenth=76;
							LoadSave.maxc=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						break;
						case 9://����ѹ
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2-2;
							Coordinates.lenth=76;
							LoadSave.maxv=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						break;
						case 10://�����
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.maxp=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						break;
						case 11://��������
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.crise=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						break;
						case 12://�½�����
							Coordinates.xpos=LIST2+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.cdrop=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
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
								SetSystemStatus(SYS_STATUS_SYS);
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
							JumpBoot(55);
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
					}
					else if(keynum<7 && keynum>1){
						keynum+=5;
					}else if(keynum == 1){
						keynum = 0;
					}else if(keynum == 0){
						keynum = 12;
					}
						
				break;
				case Key_RIGHT:
					if(keynum>6 && keynum < 12)
					{
						keynum-=5;
					}
					else if(keynum<7 && keynum>0){
						keynum+=6;
					}else if(keynum == 0){
						keynum = 1;
					}else if(keynum == 12){
						keynum = 0;
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
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
			
		}
	 	
	
	
	
	}
}


//==========================================================
//�������ƣ�Limit_Process
//��������	��������
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.28
//�޸����ڣ�2015.10.28 10:45
//��ע˵������
//==========================================================
void Limit_Process(void)
{

	u8 keynum=0;
	Disp_Coordinates_Typedef  Coordinates;
//	Send_Ord_Typedef Uart;
	
	vu8 key;
	vu8 Disp_Flag=1;
	keynum=0;
    LCD_Clear(LCD_COLOR_TEST_BACK);
    Disp_Limit_Item();
 	while(GetSystemStatus()==SYS_STATUS_LIMITSET)
	{
	    
		if(Disp_Flag==1)
		{
			DispLimit_value(keynum);
           
			Disp_Flag=0;
		
		}

        key=Key_Read();
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
						case 1://��ѹ����
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+-2;
							Coordinates.lenth=76;
							LoadSave.vhigh=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 2://��ѹ����
						{
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1+-2;
							Coordinates.lenth=76;
							LoadSave.vlow=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
						}break;
						case 3://��������
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*2-2;
							Coordinates.lenth=76;
							LoadSave.chigh=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 4://��������
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*3-2;
							Coordinates.lenth=76;
							LoadSave.clow=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 5://��������
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*4-2;
							Coordinates.lenth=76;
							LoadSave.phigh=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
							break;
						case 6://��������
							Coordinates.xpos=LIST1+88;
							Coordinates.ypos=FIRSTLINE+SPACE1*5-2;
							Coordinates.lenth=76;
							LoadSave.plow=Disp_Set_Num(&Coordinates);
							Set_Para();
							Store_set_flash();
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
						case 1:
                            Jk516save.Set_Data.trip=2;//����
							break;
						case 2:
                            Jk516save.Set_Data.speed=2;
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
							Jk516save.Set_Data.beep=2;
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
						case 1:
                            Jk516save.Set_Data.trip=3;//����
							
									
							break;
						case 2:
                            Jk516save.Set_Data.speed=3;
							
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
							if(Jk516save.Set_Data.openbeep==0)
							{
								Jk516save.Set_Data.openbeep=1;
							}else{
								Jk516save.Set_Data.openbeep=0;
							}
							break;
						
						default:
							break;					
					}	
				
				break;
				case Key_F5:
					switch(keynum)
					{
						case 0:
							JumpBoot(55);
						break;
						case 4:
						case 9:
							break;
						case 7:	
							Jk516save.Set_Data.Range_Set=1;
                            if(Jk516save.Set_Data.Range)
							Jk516save.Set_Data.Range--;
                            else
                                Jk516save.Set_Data.Range=RANGE_MAX;
                                
                            Range=Jk516save.Set_Data.Range;
						break;
						
						default:
							break;
					
					
					}
                    
					
				break;

				
				case Key_LEFT:
					if(keynum>6)
					{
						keynum-=6;
					}
					else if(keynum<7 && keynum>1){
						keynum+=5;
					}else if(keynum == 1){
						keynum = 0;
					}else if(keynum == 0){
						keynum = 6;
					}
						
				break;
				case Key_RIGHT:
					if(keynum>6 && keynum < 12)
					{
						keynum-=5;
					}
					else if(keynum<6 && keynum>0){
						keynum+=6;
					}else if(keynum == 0){
						keynum = 1;
					}else if(keynum == 6){
						keynum = 0;
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
				break;
				case Key_REST:
				break;
				case Key_TRIG:
				break;
				default:
				break;
					
			}
		
		
		}
	 	
	
	
	
	}
}

//==========================================================
//�������ƣ�Test_Process
//�������ܣ�����������
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26
//�޸����ڣ�2015.10.29 14:18
//��ע˵����x.xx ms(12MHz/12T)
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


    u8 ry_flag=0;
	vu16 i;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//��ʾ��־
	u8 initset = 15;//ÿ�ν��������������ó�ʼ����
//    u8 Disp_Testflag=0;
//    u8 fit_num=0;
//	Check_Parameter_Limit();//����ֵ���
	F_100ms=FALSE;//100ms��ʱ	

    LCD_Clear(LCD_COLOR_TEST_BACK);


    Disp_Test_Item();  

    i=0;
	Set_Para();
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
	spinbit = 1;
	while(GetSystemStatus()==SYS_STATUS_TEST)
	{
        USB_Count++;
//        Key_Scan();
//		spinvalue = Encoder_Process();
//		spinvalue = TIM_GetCounter(TIM3);
		
		Encoder_Process(keynum);

		if(spinflag == 1)
		{
			Disp_Flag = 1;
		}
        Colour.Fword=White;
//		Set_Addr();
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1�����һ��
            Int_Pe3flag=0;
            
        }
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
		if(spinsend == 1)
		{
			spinsend = 0;
			Set_Para();
		}
        if(Disp_Flag==1 )//��ʾ���õ�ֵ
		{
			Disp_Test_value(keynum);
            
//			Disp_R_V("V");
			Disp_Flag = 0;
		}  
//		ReadData();
		if(F_100ms == TRUE/* && setflag == 0*/)
		{
			ReadData();
			F_100ms=FALSE;
		}
        Test_Disp(LoadSave.vrange,LoadSave.crange);
		
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									keynum=0;
										//SetSystemStatus(SYS_STATUS_TEST);
									break;
								case 1:
									LoadSave.mode=0;
									Mode_SW();
									Store_set_flash();
								break;
								case 2:
								{
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
									Set_Para();
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
									
										SetSystemStatus(SYS_STATUS_BATTERY);
									break;
								case 1:
									LoadSave.mode=1;
									Mode_SW();
									Store_set_flash();
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
									SetSystemStatus(SYS_STATUS_DYNAMIC);
								break;
								case 1:
									LoadSave.mode=2;
									Mode_SW();
									Store_set_flash();
								break;
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
								case 0:
									SetSystemStatus(SYS_STATUS_LIST);
							    break;
								case 1:
									LoadSave.mode=3;
									Mode_SW();
									Store_set_flash();
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
								{
									SetSystemStatus(SYS_STATUS_LIMITSET);
								}break;
								case 1:
									LoadSave.mode=6;
									Mode_SW();
									Store_set_flash();
								break;
								default:
									break;
							}
	  
						break;

						case Key_LEFT:
							if(keynum<1)
								keynum=2;
							else
								keynum--;
						  
						break;
						case Key_RIGHT:
							if(keynum>1)
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
//							if(inputflag == 1)
//							{
//								switch(keynum)
//								{
//									case 2:
//									{
//										Coordinates.xpos=LIST2+88;
//										Coordinates.ypos=FIRSTLINE*1;
//										Coordinates.lenth=76;
//										LoadSave.voltage=Disp_Set_Num(&Coordinates);
//									}
//									break;
//								}
//							}
						break;

						case Key_REST:
						break;
						case Key_ONOFF:							
							if(mainswitch == 0)
							{
								mainswitch = 1;
								SwitchLedOn();
								OnOff_SW(mainswitch);
							}else{
								mainswitch = 0;
								SwitchLedOff();
								OnOff_SW(mainswitch);
							}
						break;
						case Key_SET1:
						{
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						case Key_Ent:
						{
//							ReadData();
						}break;
						case Key_ESC:
						{
							
						}break;
						case PRESS_SPIN:
						{
							if(spinbit < spinbitmax)
							{
								spinbit ++;
							}else{
								spinbit = 1;
							}
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
//�������ƣ�Test_Process
//�������ܣ�����������
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26
//�޸����ڣ�2015.10.29 14:18
//��ע˵����x.xx ms(12MHz/12T)
//==========================================================
void Led_Process(void)
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
	vu16 i;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//��ʾ��־

    LCD_Clear(LCD_COLOR_TEST_BACK);
    pt=Send_To_U;

    Disp_Led_Item();  
    open_flag=0;    
    open_flag=1; 

    i=0;
    range_over=0;
    Send_ComBuff.send_head=0xaa;
    Send_ComBuff.sendend=0xbf;
    Send_ComBuff.over=0;
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
		Encoder_Process(keynum);
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1�����һ��
            Int_Pe3flag=0;
            
        }
         if(Disp_Flag==1 )//��ʾ���õ�ֵ
		{
			Disp_Led_value(keynum);
            Colour.black=LCD_COLOR_TEST_MID;
			Hex_Format(DispValue.Voltage,3,5,0);       
			WriteString_16(80,100,DispBuf,0);
			WriteString_16(180,100,"V",0);
			Hex_Format(DispValue.Voltage,3,5,0);  
			WriteString_16(80,100+30,DispBuf,0);
			WriteString_16(180,100+30,"A",0);
			Hex_Format(DispValue.Voltage,3,5,0);  
			WriteString_16(80,100+60,DispBuf,0);
			WriteString_16(180,100+60,"W",0);
//			Disp_R_V("V");
			Disp_Flag = 0;
		}  

             
//		Uart_Process();//���ڴ���
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									keynum=0;
										//SetSystemStatus(SYS_STATUS_TEST);
									break;
								case 1:
									LoadSave.mode=0;
								break;
								case 2:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									LoadSave.voltage=Disp_Set_Num(&Coordinates);
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
									SetSystemStatus(SYS_STATUS_BATTERY);
								break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
							   case 0:
										SetSystemStatus(SYS_STATUS_DYNAMIC);
									break;
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							switch(keynum)
							{
								case 1:
									LoadSave.mode=4;
								break;
								default:
									break;
							}
	  
						break;

						case Key_LEFT:
							if(keynum<1)
								keynum=6;
							else
								keynum--;
						  
						break;
						case Key_RIGHT:
							if(keynum>5)
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
								case 2:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									LoadSave.voltage=Disp_Set_Num(&Coordinates);
								}
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
						default:
							SetSystemStatus(SYS_STATUS_TEST);
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
	vu16 i;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//��ʾ��־

    LCD_Clear(LCD_COLOR_TEST_BACK);


    Disp_Battery_Item(0);  


    i=0;

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
		Encoder_Process(keynum);
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1�����һ��
            Int_Pe3flag=0;
            
        }
         if(Disp_Flag==1 )//��ʾ���õ�ֵ
		{
			Disp_Bat_value(keynum);
			Disp_Flag = 0;
		}  
		Bat_Disp(LoadSave.vrange,LoadSave.crange);
             
//		Uart_Process();//���ڴ���
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									keynum=0;
										SetSystemStatus(SYS_STATUS_TEST);
									break;
								case 1:
									LoadSave.loadmode=0;
									BatMode_SW();
									LCD_Clear(LCD_COLOR_TEST_BACK);
									Disp_Battery_Item(1);
									Store_set_flash();
								break;
								case 2:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*1;
										Coordinates.lenth=76;
	 									LoadSave.loadc1=Disp_Set_Num(&Coordinates);
									}else if(LoadSave.loadmode == 1){
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*1;
										Coordinates.lenth=76;
										LoadSave.loadr=Disp_Set_Num(&Coordinates);
									}
									Store_set_flash();
								}
								break;
								case 3:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*2;
										Coordinates.lenth=76;
										LoadSave.loadc2=Disp_Set_Num(&Coordinates);
									}
									Store_set_flash();
								}
								break;
								case 4:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST1+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*3;
										Coordinates.lenth=76;
										LoadSave.loadc3=Disp_Set_Num(&Coordinates);
									}
									Store_set_flash();
								}
								break;
								case 6:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*1;
										Coordinates.lenth=76;
										LoadSave.coffv1=Disp_Set_Num(&Coordinates);
									}else if(LoadSave.loadmode == 1){
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*1;
										Coordinates.lenth=76;
										LoadSave.coffvr=Disp_Set_Num(&Coordinates);
									}
									Store_set_flash();
								}
								break;
								case 7:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*2;
										Coordinates.lenth=76;
										LoadSave.coffv2=Disp_Set_Num(&Coordinates);
									}
									Store_set_flash();
								}
								break;
								case 8:
								{
									if(LoadSave.loadmode == 0)
									{
										Coordinates.xpos=LIST2+118;
										Coordinates.ypos=FIRSTLINE+SPACE1*3;
										Coordinates.lenth=76;
										LoadSave.coffv3=Disp_Set_Num(&Coordinates);
									}
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
									LoadSave.loadmode=1;
									BatMode_SW();
									LCD_Clear(LCD_COLOR_TEST_BACK);
									Disp_Battery_Item(1);
									Store_set_flash();
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
									SetSystemStatus(SYS_STATUS_DYNAMIC);
								break;
							
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
							   case 0:
										
									break;
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							switch(keynum)
							{
								case 1:
									LoadSave.mode=4;
								break;
								default:
									break;
							}
	  
						break;

						case Key_LEFT:
							if(keynum<1)
								keynum=8;
							else
								keynum--;
						  
						break;
						case Key_RIGHT:
							if(keynum>7)
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
						break;

						case Key_REST:
						break;
						case Key_ONOFF:							
							if(mainswitch == 0)
							{
								mainswitch = 1;
								OnOff_SW(mainswitch);
							}else{
								mainswitch = 0;
								OnOff_SW(mainswitch);
							}
						break;
						case Key_TRIG:
							
						break;
						case Key_SET1:
						{
							SetSystemStatus(SYS_STATUS_SETUP);
						}break;
						default:
							SetSystemStatus(SYS_STATUS_TEST);
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
	vu16 i;
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//��ʾ��־

    LCD_Clear(LCD_COLOR_TEST_BACK);
    pt=Send_To_U;
	F_100ms=FALSE;//100ms��ʱ
    Disp_Dynamic_Item();  


    i=0;


	while(GetSystemStatus()==SYS_STATUS_DYNAMIC)
	{
        USB_Count++;
//        Key_Scan();
		Encoder_Process(keynum);
//		spinvalue = TIM_GetCounter(TIM3);
        Colour.Fword=White;
        if(Disp_RTCflag)
        {
            Disp_RTCflag=0;
            Disp_dateandtime();//1�����һ��
            Int_Pe3flag=0;
            
        }
         if(Disp_Flag==1 )//��ʾ���õ�ֵ
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
		Dynamic_Disp(LoadSave.vrange,LoadSave.crange);
             
//		Uart_Process();//���ڴ���
        if(Keyboard.state==TRUE)
        {
			if(skipkey == 0 && Keyboard.continuance == FALSE)
			{
				Disp_Flag=1;
				key=Key_Read();
				if(key!=KEY_NONE)
				switch(key)
				{
						case Key_F1:
							
							switch(keynum)
							{
								case 0:
									keynum=0;
										SetSystemStatus(SYS_STATUS_TEST);
									break;
								case 1:
									LoadSave.testdis=0;
								break;
								case 2:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									LoadSave.valA=Disp_Set_Num(&Coordinates);
								}
								break;
								case 3:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*2;
									Coordinates.lenth=76;
									LoadSave.valA=Disp_Set_Num(&Coordinates);
								}break;
								case 4:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*2;
									Coordinates.lenth=76;
									LoadSave.dynaIRise=Disp_Set_Num(&Coordinates);
								}break;
								case 5:
								{
									LoadSave.dynamode = 0;
								}break;
								case 6:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*2;
									Coordinates.lenth=76;
									LoadSave.timeA=Disp_Set_Num(&Coordinates);
								}break;
								case 7:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*2;
									Coordinates.lenth=76;
									LoadSave.timeB=Disp_Set_Num(&Coordinates);
								}break;
								case 8:
								{
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*2;
									Coordinates.lenth=76;
									LoadSave.dynaIDrop=Disp_Set_Num(&Coordinates);
								}break;
								default:
									break;
								
							
							}
						break;
						case Key_F2:
							switch(keynum)
							{
								case 0:
									
										SetSystemStatus(SYS_STATUS_BATTERY);
									break;
								case 1:
									LoadSave.testdis=1;
								break;
								case 5:
								{
									LoadSave.dynamode = 1;
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
										
									break;
								case 5:
								{
									LoadSave.dynamode = 2;
								}break;
							
							}
		//					Savetoeeprom();
						break;
						case Key_F4:
							switch(keynum)//
							{
							   case 0:
										
									break;
	  
								default:
									break;
							
							
							}	
		//					Savetoeeprom();
						break;
						case Key_F5:
							switch(keynum)
							{
								case 1:
									LoadSave.mode=4;
								break;
								default:
									break;
							}
	  
						break;

						case Key_LEFT:
							if(keynum<5 && keynum>1)
							{
								keynum+=3;
							}
							else if(keynum>4){
								keynum-=4;
							}else if(keynum == 0){
								keynum = 8;
							}else if(keynum == 1){
								keynum = 0;
							}
						break;
						case Key_RIGHT:
						   if(keynum<5 && keynum>0)
							{
								keynum+=4;
							}
							else if(keynum>4 && keynum <8){
								keynum-=3;
							}else if(keynum == 0){
								keynum = 1;
							}else if(keynum == 8){
								keynum = 0;
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
									Coordinates.xpos=LIST2+88;
									Coordinates.ypos=FIRSTLINE*1;
									Coordinates.lenth=76;
									LoadSave.voltage=Disp_Set_Num(&Coordinates);
								}
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
    Send_To_UTypedef pt;
    Disp_Coordinates_Typedef  Coordinates;
	u8 Disp_Flag=1;//��ʾ��־
	u8 initset = 10;//ÿ�ν��������������ó�ʼ����
	DispValue.listrunstep = 0;//ÿ�ν����б���踴λ
	DispValue.liststep = 0;//ÿ�ν����б���踴λ
    LCD_Clear(LCD_COLOR_TEST_BACK);


    Disp_List_Item();  


    i=0;
	Set_Para();

	
	while(GetSystemStatus()==SYS_STATUS_LIST)
	{
        USB_Count++;
//        Key_Scan();
		keytrans=Encoder_Process(keynum);
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
            Disp_dateandtime();//1�����һ��
            Int_Pe3flag=0;
            
        }
		if(UART_Buffer_Rece_flag==1)
		{
			UART_Buffer_Rece_flag=0;
			Rec_Handle();
		}
         if(Disp_Flag==1 )//��ʾ���õ�ֵ
		{
			Disp_List_value(keynum);
			Disp_Flag = 0;
		}  
		if(resflag == 1)
		{
			Disp_Res_Sheet(LoadSave.ListNum);
			resdisp = 1;
			resflag = 0;
		}
		if(F_100ms == TRUE/* && setflag == 0*/)
		{
			ReadData();
			F_100ms=FALSE;
		}
//		if(mainswitch == 1)
//		{
		if(resdisp != 1)
		{
			Disp_List_Process();
		}
//		}
//		Uart_Process();//���ڴ���
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
						switch(key)
						{
								case Key_F1:
									
									switch(keynum)
									{
										case 0:
											keynum=0;
												SetSystemStatus(SYS_STATUS_TEST);
											break;
										case 1:
											Coordinates.xpos=LIST1+118;
											Coordinates.ypos=FIRSTLINE;
											Coordinates.lenth=76;
											LoadSave.ListNum=Disp_Set_Step(&Coordinates);
											Store_set_flash();
										break;
										case 2:
										{
											LoadSave.StepMode = 0;
											Store_set_flash();
										}
										break;
										case 3:
										{
											LoadSave.LoopTest = 0;
											Store_set_flash();
										}
										break;
										case 4:
										{
											LoadSave.ListBeep = 0;
											Store_set_flash();
										}
										break;
										case 5:
											Coordinates.xpos=LIST2+118;
											Coordinates.ypos=FIRSTLINE+SPACE1*1;
											Coordinates.lenth=76;
											LoadSave.listonvol=Disp_Set_Num(&Coordinates);
											Store_set_flash();
										break;
										case 6:
										{
											LoadSave.listmode[DispValue.liststep] = 0;
											Store_set_flash();
										}break;
										case 7:
										{
											Coordinates.xpos=100;
											Coordinates.ypos=26+4*22;
											Coordinates.lenth=76;
											LoadSave.listvalue[DispValue.liststep]=Disp_Set_Num(&Coordinates);
											Store_set_flash();
										}break;
										case 8:
										{
											Coordinates.xpos=100;
											Coordinates.ypos=26+5*22;
											Coordinates.lenth=76;
											LoadSave.delay[DispValue.liststep]=Disp_List_Delay(&Coordinates);
											Store_set_flash();
										}
										break;
										case 9:
										{
											LoadSave.listcomp[DispValue.liststep] = 0;
											Store_set_flash();
										}break;
										case 10:
										{
											Coordinates.xpos=100;
											Coordinates.ypos=26+7*22;
											Coordinates.lenth=76;
											LoadSave.listhigh[DispValue.liststep]=Disp_Set_Num(&Coordinates);
											Store_set_flash();
										}break;
										case 11:
										{
											Coordinates.xpos=100;
											Coordinates.ypos=26+8*22;
											Coordinates.lenth=76;
											LoadSave.listlow[DispValue.liststep]=Disp_Set_Num(&Coordinates);
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
											
										break;
										case 1:
										{
											LoadSave.loadmode=1;
											BatMode_SW();
											LCD_Clear(LCD_COLOR_TEST_BACK);
											Disp_Battery_Item(1);
											Store_set_flash();
										}break;
										case 2:
										{
											LoadSave.StepMode = 1;
											Store_set_flash();
										}
										break;
										case 3:
										{
											LoadSave.LoopTest = 1;
											Store_set_flash();
										}
										break;
										case 4:
										{
											LoadSave.ListBeep = 1;
											Store_set_flash();
										}
										break;
										case 6:
										{
											LoadSave.listmode[DispValue.liststep] =1;
											Store_set_flash();
										}break;
										
										case 9:
										{
											LoadSave.listcomp[DispValue.liststep] = 1;
											Store_set_flash();
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
											SetSystemStatus(SYS_STATUS_DYNAMIC);
										break;
										case 6:
										{
											LoadSave.listmode[DispValue.liststep] = 2;
											Store_set_flash();
										}break;
										case 9:
										{
											LoadSave.listcomp[DispValue.liststep] = 2;
											Store_set_flash();
										}break;
									
									}
				//					Savetoeeprom();
								break;
								case Key_F4:
									switch(keynum)//
									{
									   case 0:
												
											break;
										case 6:
										{
											LoadSave.listmode[DispValue.liststep] = 3;
											Store_set_flash();
										}break;
										case 9:
										{
											LoadSave.listcomp[DispValue.liststep] = 3;
											Store_set_flash();
										}break;
										default:
											break;
									
									
									}	
				//					Savetoeeprom();
								break;
								case Key_F5:
									switch(keynum)
									{
										case 1:
											LoadSave.mode=4;
										break;
										case 6:
										{
											LoadSave.listmode[DispValue.liststep] = 4;
											Store_set_flash();
										}break;
										default:
											break;
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
								break;

								case Key_REST:
								break;
								case Key_ONOFF:							
									if(mainswitch == 0)
									{
										mainswitch = 1;
										listreset();
										SwitchLedOn();
										Set_Para();
		//								OnOff_SW(mainswitch);
										DispValue.listdelay = LoadSave.delay[0];
									}else{
										mainswitch = 0;
										SwitchLedOff();
										Set_Para();
		//								OnOff_SW(mainswitch);
									}
								break;
								case Key_TRIG:
									
								break;
								case Key_SET1:
								{
									SetSystemStatus(SYS_STATUS_SETUP);
								}break;
								default:
									SetSystemStatus(SYS_STATUS_TEST);
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
							}
						}
					}
				}
         }
     }
}
//==========================================================
//�������ƣ�TestAbort_Process
//�������ܣ�������ֹ����
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26 
//�޸����ڣ�2015.10.29 13:35
//��ע˵������
//==========================================================
//void TestAbort_Process(void)
//{
//	u8 key;

//	//����ر�
////	Plc_Start_Off();//��Զ������

// 	//Ѷ�����
////	if(SaveData.Parameter.Beep)
////		Beep_One();//��һ��

//	//��ʾ��Ϣ
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//��������
////	Disp_SysMessage();//��ʾ��Ϣ
//	
//	//DAC_SetChannel1Data(DAC_Align_12b_R,0);
////	Led_HV_Off();//�ز��Ե�
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
////		Uart_Process();//���ڴ���
//		
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/20);//�ȴ�����(100*10ms/20=50ms)
//		switch(key)
//		{
//			case KEY_SET:	//���ü�
//			case L_KEY_SET:	//�������ü�
//				break;
//	
//			case KEY_UP:	//�ϼ�
//			case L_KEY_UP:	//�����ϼ�
//				break;
//	
//			case KEY_DOWN:		//�¼�
//			case L_KEY_DOWN:	//�����¼�
//				break;

//			case KEY_START:		//������
//			case L_KEY_START:	//����������
//				SetSystemStatus(SYS_STATUS_TEST);//���Կ�ʼ
//				break;
//	
//			case KEY_ENTER:		//ȷ�ϼ�
//			case L_KEY_ENTER:	//����ȷ�ϼ�
//				break;
//	
////			case KEY_RESET:		//��λ��
////			case L_KEY_RESET:	//������λ��
////				SetSystemStatus(SYS_STATUS_IDLE);//ϵͳ״̬-����
////				break;
//			
//			default:
//				break;
//		}
//	}
//	Test_value.Test_Time=0;
//}
//==========================================================
//�������ƣ�TestFinish_Process
//�������ܣ����Խ�������
//��ڲ�������
//���ڲ�������
//�������ڣ�2015.10.26
//�޸����ڣ�2015.10.29 13:38
//��ע˵������
//ע�������
//==========================================================
//void TestFinish_Process(void)
//{
//	u8 key;

//	//����ر�
////	Led_HV_Off();//�رո�ѹ��
////	Plc_Start_Off();//��Զ������
//	//��ʾ��Ϣ
//	//SetSystemMessage(MSG_PASS);
//	
////	LcdAddr.x=MSG_ADDR_X; LcdAddr.y=0;
////	Lcd_SetAddr();//��������
////	Disp_SysMessage();//��ʾ��Ϣ
//	//��ѡѶ�����

//	
//	Electro_discharge(0);
//	while(GetSystemStatus()==SYS_STATUS_TEST_FINISH)
//	{
////		Uart_Process();//���ڴ���
//		key=Key_Read_WithTimeOut(TICKS_PER_SEC_SOFTTIMER/50);//�ȴ�����(100*10ms/50=20ms)
//		switch(key)
//		{
//			case KEY_SET:	//���ü�
//			case L_KEY_SET:	//�������ü�
//				SetSystemStatus(SYS_STATUS_SETUP);//����״̬
//				break;
//	
//			case KEY_UP:	//�ϼ�
//			case L_KEY_UP:	//�����ϼ�
//				break;
//	
//			case KEY_DOWN:		//�¼�
//			case L_KEY_DOWN:	//�����¼�
//				break;
//	
//			case KEY_START:		//������
//			case L_KEY_START:	//����������
//				SetSystemStatus(SYS_STATUS_TEST);//����״̬
//				break;
//	
//			case KEY_ENTER:		//ȷ�ϼ�
////			case L_KEY_ENTER:	//����ȷ�ϼ�
////				SetSystemStatus(SYS_STATUS_IDLE);//����״̬
////				break;
////	
////			case KEY_RESET:		//��λ��
////			case L_KEY_RESET:	//������λ��
////				SetSystemStatus(SYS_STATUS_IDLE);//����״̬
////				break;
//			
//			default:
//				break;
//		}
//	}
//}	

//�û�У��
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
    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
    NVIC_EnableIRQ(EXTI3_IRQn);
 	while(GetSystemStatus()==SYS_STATUS_DEBUG)
	{
		
		Encoder_Process(list);
		if(Disp_flag==1)//��������ʱ��
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

				break;
				case Key_F2:

				break;
				case Key_F3:
					if(list == 1||list == 2)
					{
						LoadSave.vrange = 0;
						V_Gear_SW();
					}else if(list == 3||list == 4||list == 5){
						LoadSave.vrange = 1;
						V_Gear_SW();
					}else if(list == 6||list == 7){
						LoadSave.crange = 0;
						I_Gear_SW();
					}else if(list == 8||list == 9||list == 10){
						LoadSave.crange = 1;
						I_Gear_SW();
					}
				break;
				case Key_F4:
					Set_Dac(list);
				break;
				case Key_F5:
					CalHandle(list);
				break;

				
				case Key_LEFT:
					if(list<1){
						list=DEBUG_RANGE;
					}else{
						list --;
					}
					if(list < 6)
					{
						LoadSave.mode = 1;
					}else{
						LoadSave.mode = 0;
					}
					Mode_SW();
				break;
				case Key_RIGHT:
					if(list>9){
						list=0;
					}else{
						list ++;
					}
					if(list < 6)
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
					Coordinates.xpos=LIST1+160;
					Coordinates.ypos=FIRSTLINE+(SPACE1-2)*(list-1);
					Coordinates.lenth=70;
					DispValue.CalValue[list-1] = Disp_Set_Num(&Coordinates);
				break;
				
				case Key_REST:
                    
					
				break;
				case Key_TRIG:
				break;
				case Key_ONOFF:
				{
					if(mainswitch == 0)
					{
						mainswitch = 1;
						OnOff_SW(mainswitch);
					}else{
						mainswitch = 0;
						OnOff_SW(mainswitch);
					}
				}break;
				case Key_SET1:
				{
					Set_Para();
				}break;
				default:
				break;
					
			}
		
		
		}
	
	
	}
}

//��������л�
//���� �� 0 1 2 3
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

