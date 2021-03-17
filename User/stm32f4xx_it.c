/**
  ******************************************************************************
  * @file    FMC_SDRAM/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *         This file provides template for all exceptions handler and
  *         peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "pbdata.h"
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include    "bsp_exti.h"
#include "./tim/bsp_basic_tim.h"
#include "stdlib.h"
#include "math.h"

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
struct MODS_T g_tModS;
u8 g_mods_timeout = 0;
u32 Tick_10ms=0;
char scpinum[20],scpinum1[20];
u8 scpidot,scpiunit,scpidot1,scpiunit1;
u8 scpnum = 0;
u8 scpnum1 = 0;
u8 resflag;
u8 resdisp;
u8 batstep;
u8 listbeep;
extern Sort_TypeDef SCPI_SET_R(void),SCPI_SET_V(void),SCPI_SET_R1(void),SCPI_SET_V1(void);
/* Private function prototypes -----------------------------------------------*/
extern void USB_OTG_BSP_TimerIRQ (void);
static void MODS_03H(void);
static void MODS_06H(void);
static void RemTrig(void);
u8 UART_Buffer_Rece[200];
u8 flag_Tim_USART;
u8 flag_spin;
u16 t_USART;
u16 t_SPIN;
u8 UART_Buffer_Size;
u8 UART_Buffer_Rece_flag;
u8 jumpflag;
u32 listtime;
u8 spinsend;
//extern void Read__Convert_read(void);
/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup FMC_SDRAM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
    
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
	if(flag_Tim_USART==1)//串口清零计数
	{
		t_USART++;
	}
	if(t_USART>30)
	{
		t_USART=0;
		flag_Tim_USART=0;
		UART_Buffer_Size=0;	
	}
	if(flag_spin == 1)
	{
		t_SPIN++;
	}
	if(t_SPIN>200)
	{
		t_SPIN=0;
		flag_spin=0;
		spinsend = 1;
	}
}


void USART1_IRQHandler(void)
{
	flag_Tim_USART=1;
 	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit( USART1, USART_IT_RXNE );
		UART_Buffer_Rece[UART_Buffer_Size]=USART_ReceiveData(USART1);

		if( UART_Buffer_Rece[0]== 0x01)
		{
			if(UART_Buffer_Rece[1] == 0x03)
			{
				if(UART_Buffer_Size==80)//
				{
					UART_Buffer_Size=0;	  	   		   
					UART_Buffer_Rece_flag=1;
					flag_Tim_USART=0;
					t_USART=0;
					return ;
				}
			}else if(UART_Buffer_Rece[1] == 0x06){
				if(UART_Buffer_Size==9)//
				{
					UART_Buffer_Size=0;	  	   		   
					UART_Buffer_Rece_flag=1;
					flag_Tim_USART=0;
					t_USART=0;
					return ;
				}
			}else if(UART_Buffer_Rece[1] == 0x10){
				if(UART_Buffer_Size==7)//
				{
					UART_Buffer_Size=0;	  	   		   
					UART_Buffer_Rece_flag=1;
					flag_Tim_USART=0;
					t_USART=0;
					return ;
				}
			}
		}
		

		UART_Buffer_Size++;
		if(UART_Buffer_Size>200)
		{
			UART_Buffer_Size=0;
			UART_Buffer_Rece_flag=0;
			t_USART=0;	
		}		
	}
}







void  TIM2_IRQHandler (void)//U盘用了定时器2
{
    USB_OTG_BSP_TimerIRQ();
	 	
}

void ListComp(void)
{
	if(LoadSave.listcomp[DispValue.listrunstep] != 0)
	{
		if(LoadSave.listcomp[DispValue.listrunstep] == 1)
		{
			if(LoadSave.crange == 1)
			{
				if(DispValue.Current*10 > LoadSave.listhigh[DispValue.listrunstep]
				|| DispValue.Current*10 < LoadSave.listlow[DispValue.listrunstep])
				{
					DispValue.listcompres[DispValue.listrunstep] = 1;
				}
			}else if(LoadSave.crange == 0){
				if(DispValue.Current > LoadSave.listhigh[DispValue.listrunstep]
				|| DispValue.Current < LoadSave.listlow[DispValue.listrunstep])
				{
					DispValue.listcompres[DispValue.listrunstep] = 1;
				}
			}
		}else if(LoadSave.listcomp[DispValue.listrunstep] == 2){
			if(LoadSave.vrange == 1)
			{
				if(DispValue.Voltage*10 > LoadSave.listhigh[DispValue.listrunstep]
				|| DispValue.Voltage*10 < LoadSave.listlow[DispValue.listrunstep])
				{
					DispValue.listcompres[DispValue.listrunstep] = 2;
				}
			}else if(LoadSave.vrange == 0){
				if(DispValue.Voltage > LoadSave.listhigh[DispValue.listrunstep]
				|| DispValue.Voltage < LoadSave.listlow[DispValue.listrunstep])
				{
					DispValue.listcompres[DispValue.listrunstep] = 2;
				}
			}
		}else if(LoadSave.listcomp[DispValue.listrunstep] == 2){
			if(DispValue.Power > LoadSave.listhigh[DispValue.listrunstep]
			|| DispValue.Power < LoadSave.listlow[DispValue.listrunstep])
			{
				DispValue.listcompres[DispValue.listrunstep] = 3;
			}
		}
	}else{
		DispValue.listcompres[DispValue.listrunstep] = 4;
	}
}

u8 ListBeep(void)
{
	u8 i;
//	if(LoadSave.ListBeep != 0)
//	{
	for(i=0;i<LoadSave.ListNum;i++)
	{
		if(DispValue.listcompres[i] != 0)
		{
			if(LoadSave.ListBeep == 1)
			{
				return 1;
			}
		}
	}
	if(LoadSave.ListBeep == 0)
	{
		return 1;
	}
//	}
	return 0;
}

void  BASIC_TIM_IRQHandler (void)
{
    static u8 num,cnum=0;
	static u32 autooffcount=0;
	static u16 countdown=0;
    u8 i;
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		
		if(LoadSave.autooff != 0)
		{
			if(mainswitch == 1)
			{
				autooffcount++;
				if(autooffcount > (LoadSave.autooff*100-1))
				{
					mainswitch = 0;
					OnOff_SW(mainswitch);
					SwitchLedOff();
					autooffcount = 0;
				}
			}
		}
        if(SystemStatus==SYS_STATUS_TEST || SystemStatus==SYS_STATUS_BATTERY
			|| SystemStatus==SYS_STATUS_LIST)
        {
            num++;
            if(num>9)//10mS??
            {
                num=0;
                F_100ms=TRUE;//100ms????
            }

        }
        else
        {
            if(num!=0)
                num=0;
        }
		
		if(SystemStatus==SYS_STATUS_BATTERY)
		{
			cnum++;
            if(cnum>99)//10mS??
            {
                cnum=0;
                F_1s=TRUE;//100ms????
            }
		}else{
			if(cnum!=0)
				cnum = 0;
		}
		
		if(SystemStatus==SYS_STATUS_LIST)
		{
			if(mainswitch == 1)
			{
				if(DispValue.listdelay == 1)//列表分选和结果
				{
					ListComp();
					DispValue.listv[DispValue.listrunstep] = DispValue.Voltage;
					DispValue.listc[DispValue.listrunstep] = DispValue.Current;
					DispValue.listp[DispValue.listrunstep] = DispValue.Power;
					DispValue.listvrange[DispValue.listrunstep] = LoadSave.vrange;
					DispValue.listcrange[DispValue.listrunstep] = LoadSave.crange;
				}
				countdown ++;
				if(countdown > 9)
				{
					countdown = 0;
					if(DispValue.listdelay > 0)
						DispValue.listdelay--;
				}
				listtime++;
				if(listtime > LoadSave.delay[DispValue.listrunstep]*10)
				{
					jumpflag = 1;
					if(LoadSave.StepMode == 0)
					{
						if(DispValue.listrunstep < LoadSave.ListNum-1)
						{
							listtime = 0;
							DispValue.listrunstep ++;
							DispValue.listdelay = LoadSave.delay[DispValue.listrunstep];
							Set_Para();	
						}else{
							listtime = 0;
							DispValue.listrunstep = 0;
							mainswitch = 0;
							OnOff_SW(mainswitch);
							SwitchLedOff();
							resflag = 1;
							listbeep = ListBeep();
						}
					}else if(LoadSave.StepMode == 1){
						if(DispValue.listrunstep < LoadSave.ListNum-1)
						{
							listtime = 0;
							DispValue.listrunstep ++;
							mainswitch = 0;
							SwitchLedOff();
							DispValue.listdelay = LoadSave.delay[DispValue.listrunstep];
							Set_Para();	
						}else{
							listtime = 0;
							DispValue.listrunstep = 0;
							mainswitch = 0;
							OnOff_SW(mainswitch);
							SwitchLedOff();
							resflag = 1;
							listbeep = ListBeep();
						}
					}
				}
			}
		}else if(SystemStatus==SYS_STATUS_BATTERY){
			if(LoadSave.loadmode == 0)
			{
				if(mainswitch == 1 && F_1s == TRUE)
				{
					F_1s = FALSE;
					if(LoadSave.crange == 0)
					{
						DispValue.Capraw += (float)DispValue.Current/10 * 1/3600;
					}else{
						DispValue.Capraw += (float)DispValue.Current * 1/3600;
					}
					if(batstep == 1)
					{
						if(LoadSave.vrange == 0)
						{
							if(DispValue.Voltage < LoadSave.coffv1)
							{
								if(LoadSave.loadc2 == 0)
								{
									SwitchLedOff();
									mainswitch = 0;
									Set_Para();
								}else{
									batstep = 2;
									Set_Para();
								}
							}
						}else{
							if(DispValue.Voltage*10 < LoadSave.coffv1)
							{
								if(LoadSave.loadc2 == 0)
								{
									SwitchLedOff();
									mainswitch = 0;
									Set_Para();
								}else{
									batstep = 2;
									Set_Para();
								}
							}
						}
					}else if(batstep == 2){
						if(LoadSave.vrange == 0)
						{
							if(DispValue.Voltage < LoadSave.coffv2)
							{
								if(LoadSave.loadc3 == 0)
								{
									SwitchLedOff();
									mainswitch = 0;
									Set_Para();
								}else{
									batstep = 3;
									Set_Para();
								}
							}
						}else{
							if(DispValue.Voltage*10 < LoadSave.coffv2)
							{
								if(LoadSave.loadc3 == 0)
								{
									SwitchLedOff();
									mainswitch = 0;
									Set_Para();
								}else{
									batstep = 3;
									Set_Para();
								}
							}
						}
					}else if(batstep == 3){
						if(LoadSave.vrange == 0)
						{
							if(DispValue.Voltage < LoadSave.coffv3)
							{
								SwitchLedOff();
								mainswitch = 0;
								Set_Para();
							}
						}else{
							if(DispValue.Voltage*10 < LoadSave.coffv3)
							{
								SwitchLedOff();
								mainswitch = 0;
								Set_Para();
							}
						}
					}
					DispValue.Capacity = (long)DispValue.Capraw;
				}
				
				
				
			}else if(LoadSave.loadmode == 2){
				if(mainswitch == 1 && F_1s == TRUE){
					F_1s = FALSE;
					if(LoadSave.crange == 0)
					{
						DispValue.Capraw += (float)DispValue.Current/10 * 1/3600;
					}else{
						DispValue.Capraw += (float)DispValue.Current * 1/3600;
					}
					if(LoadSave.vrange == 0)
					{
						if(DispValue.Voltage < LoadSave.coffvr)
						{
							SwitchLedOff();
							mainswitch = 0;
							Set_Para();
						}
					}else{
						if(DispValue.Voltage*10 < LoadSave.coffvr)
						{
							SwitchLedOff();
							mainswitch = 0;
							Set_Para();
						}
					}
				}
				
			}
		}
        Key_Scan();	 
       
        for (i=0;i<MAXTSOFTTIMER;i++)
        {
            if (SoftTimer[i])
                SoftTimer[i]--;
            
        }	
        
	
	}		 	
}
void KEY1_IRQHandler(void)//开关机
{
  //确保是否产生了EXTI Line中断
//    u32 i;
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
//		delay_ms(1000);
       
        if(GPIO_ReadInputDataBit( GPIOE,  GPIO_Pin_3)==0)
        {
            Int_Pe3flag=0;
            if(softswitch)
            {
                softswitch=0;
                SetSystemStatus(SYS_STATUS_TOOL);
            
            }else
            {
                softswitch=1;
                
                SetSystemStatus(SYS_STATUS_POWER);
            
            }
            NVIC_DisableIRQ(EXTI3_IRQn);
        }
		//GPIO_ClearInt(0, 1<<19);
		
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
}

void EXTI15_10_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
	{
		// LED2 取反		
		//LED2_TOGGLE;
    //清除中断标志位
        //Read__Convert_read();
		EXTI_ClearITPendingBit(EXTI_Line13);     
	}  
}
#ifdef USE_USB_OTG_FS  
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
    
#endif
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

/**	过流检测，硬件不支持
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
#if 0
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
      USB_Host.usr_cb->OverCurrentDetected();
      EXTI_ClearITPendingBit(EXTI_Line1);
  }
}


#endif
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).                         */
/******************************************************************************/

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
