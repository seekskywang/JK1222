/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./usart/bsp_debug_usart.h"
#include "pbdata.h"
u8 sendbuff[BUFFSIZEMAX];
u8 u3sendbuff[BUFFSIZEMAX];
u8 setflag;
u8 readflag;
u8 setslaveflag;
u8 slaveonoffflag;
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
//u32 dacvalue[10] = {1000,7000,1000,2000,3000,20000,50000,10000,20000,30000};
u32 dacvalue[17] = {
10000,30000,//电压低档位
3000,10000,20000,30000,40000,50000,//电压高档位
5000,10000,30000,50000,//电流低档位
10000,20000,30000,40000,50000};//电流高档位
u32 vsum,isum,rsum,psum;
u8 usart1rxbuff[BUFFSIZEMAX];
u8 usart1txbuff[BUFFSIZEMAX];

u8 usart3rxbuff[BUFFSIZEMAX];
u8 usart3txbuff[BUFFSIZEMAX];

u16 uart3sendlen;

uint16_t BEBufToUint16(uint8_t *_pBuf)
{
    return (((uint16_t)_pBuf[0] << 8) | _pBuf[1]);
}

static uint8_t MODS_ReadRegValue(uint16_t reg_addr, uint8_t *reg_value)
{
    uint16_t value;

	switch (reg_addr)									/* ??????? */
	{
		case SLAVE_REG_P00://电压1
			value = DispValue.Voltage>>16;
			break;
		case SLAVE_REG_P01://电压2
			value = (u16)DispValue.Voltage;
			break;
	
		case SLAVE_REG_P02://电流1
			value = DispValue.Current>>16;
			break;
		case SLAVE_REG_P03: //电流2
			value = (u16)DispValue.Current;
			break;

		case SLAVE_REG_P04://电阻1
			value = DispValue.Rdata>>16;
			break;
		case SLAVE_REG_P05://电阻2
			value = (u16)DispValue.Rdata;
			break;
		case SLAVE_REG_P06://功率1
			value = DispValue.Power>>16;
			break;
		case SLAVE_REG_P07://功率2
			value = (u16)DispValue.Power;
			break;
		case SLAVE_REG_P08://
			value = 0;
			break;
		case SLAVE_REG_P09://
			value = 0;
			break;
		case SLAVE_REG_P10://
			value = 0;
			break;
		case SLAVE_REG_P11://
			value = 0;
			break;
		case SLAVE_REG_P12://
			value = 0;
		break;

		case SLAVE_REG_P13://
			value = 0;
		break;
		case SLAVE_REG_P14://
			value = 0;
		break;

		case SLAVE_REG_P15://
			value = 0;
		break;
		case SLAVE_REG_P16://
			value = 0;
		break;

		case SLAVE_REG_P17://
			value = 0;
		break;
		case SLAVE_REG_P18://
			value = 0;
		break;	
		case 0x0013://
			value = 0;
		break;
		case 0x0014://操作模式1
			value = 0;
			break;
		case 0x0015://操作模式2
			value = 0;
		break;
		case 0x0016://保护标志1
			value = 0;
		break;
		case 0x0017://保护标志1
			value = 0;
		break;
		case 0x0018://开关1
			value =	mainswitch>>16;
		break;
		case 0x0019://开关2
			value =	(u16)mainswitch;
		break;
		case 0x001A://模式1
			value =	LoadSave.mode>>16;
		break;
		case 0x001B://模式2
			value =	(u16)LoadSave.mode;
		break;
		case 0x001C://电流档位1
			value =	LoadSave.crange>>16;
		break;
		case 0x001D://电流档位2
			value =	(u16)LoadSave.crange;
		break;
		case 0x001E://电压档位1
			value =	LoadSave.vrange>>16;
		break;
		case 0x001F://电压档位2
			value =	(u16)LoadSave.vrange;
		break;
		case 0x0020://本机地址1
			value =	LoadSave.Addr>>16;
		break;
		case 0x0021://本机地址2
			value =	(u16)LoadSave.Addr;
		break;
		case 0x0022://波特率1
			value =	0;
		break;
		case 0x0023://波特率2
			value =	0;
		break;
		case 0x0024://远端测量开关1
			value =	LoadSave.sence>>16;
		break;
		case 0x0025://远端测量开关2
			value =	(u16)LoadSave.sence;
		break;
		case 0x0026://设置电压1
			value =	LoadSave.voltage>>16;
		break;
		case 0x0027://设置电压2
			value =	(u16)LoadSave.voltage;
		break;
		case 0x0028://设置电流1
			value =	LoadSave.current>>16;
		break;
		case 0x0029://设置电流2
			value =	(u16)LoadSave.current;
		break;
		case 0x002A://设置电阻1
			value =	LoadSave.risistence>>16;
		break;
		case 0x002B://设置电阻2
			value =	(u16)LoadSave.risistence;
		break;
		case 0x002C://设置功率1
			value =	LoadSave.power>>16;
		break;
		case 0x002D://设置功率2
			value =	(u16)LoadSave.power;
		break;
		case 0x002E://OVP1
			value =	0;
		break;
		case 0x002F://OVP2
			value =	0;
		break;
		case 0x0030://OCP1
			value =	0;
		break;
		case 0x0031://OCP2
			value =	0;
		break;
		case 0x0032://OPP1
			value =	0;
		break;
		case 0x0033://OPP2	
			value =	0;
		break;
		case 0x0034://最大限制功率1
			value =	LoadSave.maxp>>16;
		break;
		case 0x0035://最大限制功率2
			value =	(u16)LoadSave.maxp;
		break;
		case 0x0036://最大限制电压1
			value =	LoadSave.maxv>>16;
		break;
		case 0x0037://最大限制电压2
			value =	(u16)LoadSave.maxv;
		break;
		case 0x0038://最大限制电流1
			value =	LoadSave.maxc>>16;
		break;
		case 0x0039://最大限制电流2
			value =	(u16)LoadSave.maxc;
		break;
		case 0x003A://加载电压1
			value =	LoadSave.onvol>>16;
		break;
		case 0x003B://加载电压2
			value =	(u16)LoadSave.onvol;
		break;
		case 0x003C://卸载电压1
			value =	LoadSave.offvol>>16;
		break;
		case 0x003D://卸载电压2
			value =	(u16)LoadSave.offvol;
		break;
		case 0x003E://电流上升率1
			value =	LoadSave.crise>>16;
		break;
		case 0x003F://电流上升率2
			value =	(u16)LoadSave.crise;
		break;
		case 0x0040://电流下降率1
			value =	LoadSave.cdrop>>16;
		break;
		case 0x0041://电流下降率2
			value =	(u16)LoadSave.cdrop;
		break;
		case 0x0042://CV电压下降率1
			value =	LoadSave.cvdowntime>>16;
		break;
		case 0x0043://CV电压下降率2
			value =	(u16)LoadSave.cvdowntime;
		break;
		case 0x0044://ledvo1
			value =	LoadSave.ledvo>>16;
		break;
		case 0x0045://ledvo2
			value = (u16)LoadSave.ledvo;
		break;
		case 0x0046://ledio1
			value =	LoadSave.ledio>>16;
		break;
		case 0x0047://ledio2
			value = (u16)LoadSave.ledio;
		break;
		case 0x0048://ledrd1
			value =	LoadSave.ledrd>>16;
		break;
		case 0x0049://ledrd2	
			value = (u16)LoadSave.ledrd;
		break;
		case 0x004A://动态模式拉载电流A1
			value =	LoadSave.valA>>16;
		break;
		case 0x004B://动态模式拉载电流A2
			value = (u16)LoadSave.valA;
		break;
		case 0x004C://动态模式拉载电流B1
			value =	LoadSave.valB>>16;
		break;
		case 0x004D://动态模式拉载电流B2
			value = (u16)LoadSave.valB;
		break;
		case 0x004E://动态模式拉载持续时间A1
			value =	LoadSave.timeA>>16;
		break;
		case 0x004F://动态模式拉载持续时间A2
			value = (u16)LoadSave.timeA;
		break;
		case 0x0050://动态模式拉载持续时间B1
			value =	LoadSave.timeB>>16;
		break;
		case 0x0051://动态模式拉载持续时间B2
			value = (u16)LoadSave.timeB;
		break;
		case 0x0052://动态模式电流上升率1
			value =	LoadSave.dynaIRise>>16;
		break;
		case 0x0053://动态模式电流上升率2
			value = (u16)LoadSave.dynaIRise;
		break;
		case 0x0054://动态模式电流下降率1
			value =	LoadSave.dynaIDrop>>16;
		break;
		case 0x0055://动态模式电流下降率2
			value = (u16)LoadSave.dynaIDrop;
		break;
		case 0x0056://动态模式触发模式选择1
			value =	LoadSave.dynamode>>16;
		break;
		case 0x0057://动态模式触发模式选择2
			value = (u16)LoadSave.dynamode;
		break;
		default:
			return 0;
	}
	reg_value[0] = value >> 8;
	reg_value[1] = value;

	return 1;											/* ???? */
}

static uint8_t MODS_WriteRegValue(uint16_t reg_addr, uint16_t reg_value)
{


	switch (reg_addr)							/* 判断寄存器地址 */
	{		
		case 0x0014://操作模式1
			break;
		case 0x0015://操作模式2
		break;
		case 0x0016://保护标志1
		break;
		case 0x0017://保护标志1
		break;
		case 0x0018://开关1
			mainswitch = (u32)reg_value << 16;
		break;
		case 0x0019://开关2
			mainswitch = mainswitch + reg_value;
		break;
		case 0x001A://模式1
			LoadSave.mode = (u32)reg_value << 16;
		break;
		case 0x001B://模式2
			LoadSave.mode = LoadSave.mode + reg_value;
			if(LoadSave.mode == 4)
			{
				SetSystemStatus(SYS_STATUS_DYNAMIC);
			}else if(LoadSave.mode == 5){
				SetSystemStatus(SYS_STATUS_LED);
			}else{
				SetSystemStatus(SYS_STATUS_TEST);
			}
		break;
		case 0x001C://电流档位1
			LoadSave.crange = (u32)reg_value << 16;
		break;
		case 0x001D://电流档位2
			LoadSave.crange = LoadSave.crange + reg_value;
		break;
		case 0x001E://电压档位1
			LoadSave.vrange = (u32)reg_value << 16;
		break;
		case 0x001F://电压档位2
			LoadSave.vrange = LoadSave.vrange + reg_value;
		break;
		case 0x0020://本机地址1
			LoadSave.Addr = (u32)reg_value << 16;
		break;
		case 0x0021://本机地址2
			LoadSave.Addr = LoadSave.Addr + reg_value;
		break;
		case 0x0022://波特率1
		break;
		case 0x0023://波特率2
		break;
		case 0x0024://远端测量开关1
			LoadSave.sence = (u32)reg_value << 16;
		break;
		case 0x0025://远端测量开关2
			LoadSave.sence = LoadSave.sence + reg_value;
		break;
		case 0x0026://设置电压1
			LoadSave.voltage = (u32)reg_value << 16;
		break;
		case 0x0027://设置电压2
			LoadSave.voltage = LoadSave.voltage + reg_value;
		break;
		case 0x0028://设置电流1
			LoadSave.current = (u32)reg_value << 16;
		break;
		case 0x0029://设置电流2
			LoadSave.current = LoadSave.current + reg_value;
		break;
		case 0x002A://设置电阻1
			LoadSave.risistence = (u32)reg_value << 16;
		break;
		case 0x002B://设置电阻2
			LoadSave.risistence = LoadSave.risistence + reg_value;
		break;
		case 0x002C://设置功率1
			LoadSave.power = (u32)reg_value << 16;
		break;
		case 0x002D://设置功率2
			LoadSave.power = LoadSave.power + reg_value;
		break;
		case 0x002E://OVP1
		break;
		case 0x002F://OVP2
		break;
		case 0x0030://OCP1
		break;
		case 0x0031://OCP2
		break;
		case 0x0032://OPP1
		break;
		case 0x0033://OPP2	
		break;
		case 0x0034://最大限制功率1
			LoadSave.maxp = (u32)reg_value << 16;
		break;
		case 0x0035://最大限制功率2
			LoadSave.maxp = LoadSave.maxp + reg_value;
		break;
		case 0x0036://最大限制电压1
			LoadSave.maxv = (u32)reg_value << 16;
		break;
		case 0x0037://最大限制电压2
			LoadSave.maxv = LoadSave.maxv + reg_value;
		break;
		case 0x0038://最大限制电流1
			LoadSave.maxc = (u32)reg_value << 16;
		break;
		case 0x0039://最大限制电流2
			LoadSave.maxc = LoadSave.maxc + reg_value;
		break;
		case 0x003A://加载电压1
			LoadSave.onvol = (u32)reg_value << 16;
		break;
		case 0x003B://加载电压2
			LoadSave.onvol = LoadSave.onvol + reg_value;
		break;
		case 0x003C://卸载电压1
			LoadSave.offvol = (u32)reg_value << 16;
		break;
		case 0x003D://卸载电压2
			LoadSave.offvol = LoadSave.offvol + reg_value;
		break;
		case 0x003E://电流上升率1
			LoadSave.crise = (u32)reg_value << 16;
		break;
		case 0x003F://电流上升率2
			LoadSave.crise = LoadSave.crise + reg_value;
		break;
		case 0x0040://电流下降率1
			LoadSave.cdrop = (u32)reg_value << 16;
		break;
		case 0x0041://电流下降率2
			LoadSave.cdrop = LoadSave.cdrop + reg_value;
		break;
		case 0x0042://CV电压下降率1
			LoadSave.cvdowntime = (u32)reg_value << 16;
		break;
		case 0x0043://CV电压下降率2
			LoadSave.cvdowntime = LoadSave.cvdowntime + reg_value;
		break;
		case 0x0044://ledvo1
			LoadSave.ledvo = (u32)reg_value << 16;
		break;
		case 0x0045://ledvo2
			LoadSave.ledvo = LoadSave.ledvo + reg_value;
		break;
		case 0x0046://ledio1
			LoadSave.ledio = (u32)reg_value << 16;
		break;
		case 0x0047://ledio2
			LoadSave.ledio = LoadSave.ledio + reg_value;
		break;
		case 0x0048://ledrd1
			LoadSave.ledrd = (u32)reg_value << 16;
		break;
		case 0x0049://ledrd2	
			LoadSave.ledrd = LoadSave.ledrd + reg_value;
		break;
		case 0x004A://动态模式拉载电流A1
			LoadSave.valA = (u32)reg_value << 16;
		break;
		case 0x004B://动态模式拉载电流A2
			LoadSave.valA = LoadSave.valA + reg_value;
		break;
		case 0x004C://动态模式拉载电流B1
			LoadSave.valB = (u32)reg_value << 16;
		break;
		case 0x004D://动态模式拉载电流B2
			LoadSave.valB = LoadSave.valB + reg_value;
		break;
		case 0x004E://动态模式拉载持续时间A1
			LoadSave.timeA = (u32)reg_value << 16;
		break;
		case 0x004F://动态模式拉载持续时间A2
			LoadSave.timeA = LoadSave.timeA + reg_value;
		break;
		case 0x0050://动态模式拉载持续时间B1
			LoadSave.timeB = (u32)reg_value << 16;
		break;
		case 0x0051://动态模式拉载持续时间B2
			LoadSave.timeB = LoadSave.timeB + reg_value;
		break;
		case 0x0052://动态模式电流上升率1
			LoadSave.dynaIRise = (u32)reg_value << 16;
		break;
		case 0x0053://动态模式电流上升率2
			LoadSave.dynaIRise = LoadSave.dynaIRise + reg_value;
		break;
		case 0x0054://动态模式电流下降率1
			LoadSave.dynaIDrop = (u32)reg_value << 16;
		break;
		case 0x0055://动态模式电流下降率2
			LoadSave.dynaIDrop = LoadSave.dynaIDrop + reg_value;
		break;
		case 0x0056://动态模式触发模式选择1
			LoadSave.dynamode = (u32)reg_value << 16;
		break;
		case 0x0057://动态模式触发模式选择2
			LoadSave.dynamode = LoadSave.dynamode + reg_value;
		break;
		
		default:
			return 0;		/* 参数异常，返回 0 */
	}
}

static void MODS_03H(void)
{
    uint16_t reg;
	uint16_t num;
	uint16_t i;
	uint8_t reg_value[256]; 

    

//	if (g_tModS.RxCount != 8)								/* 03H????ˇ8??? */
//	{
//		g_tModS.RspCode = RSP_ERR_VALUE;					/* ?????? */
//		goto err_ret;
//	}

	reg = BEBufToUint16(&UART3_Buffer_Rece[2]); 				/* ???? */
	num = BEBufToUint16(&UART3_Buffer_Rece[4])*2;					/* ????? */


	for (i = 0; i < num/2; i++)
	{
		if (MODS_ReadRegValue(reg, &reg_value[2 * i]) == 0)	/* ????????reg_value */
		{
//			g_tModS.RspCode = RSP_ERR_REG_ADDR;				/* ??????? */
			break;
		}
		reg++;
	}

//err_ret:
		uart3sendlen = 0;
		u3sendbuff[uart3sendlen++] = UART3_Buffer_Rece[0];
		u3sendbuff[uart3sendlen++] = UART3_Buffer_Rece[1];
		u3sendbuff[uart3sendlen++] = num;			/* ????? */

		for (i = 0; i < num/2; i++)
		{
			u3sendbuff[uart3sendlen++] = reg_value[2*i];
			u3sendbuff[uart3sendlen++] = reg_value[2*i+1];
		}
		u3sendbuff[uart3sendlen++] = Hardware_CRC(u3sendbuff,num+3);
		u3sendbuff[uart3sendlen++] = Hardware_CRC(u3sendbuff,num+3)>>8;
		
		Uart3SendBuff(u3sendbuff,uart3sendlen);

    
}

static void MODS_06H(void)
{
  uint16_t reg;
	uint16_t value;
	uint8_t i;

	reg = BEBufToUint16(&UART3_Buffer_Rece[2]); 	/* 寄存器号 */
	value = BEBufToUint16(&UART3_Buffer_Rece[4]);		/* 寄存器个数 */
    
 	if (MODS_WriteRegValue(reg, value) == 1)	/* ????ёд???????? */
 	{
 		
 	}
	uart3sendlen = 0;
	for (i = 0; i < 6; i++)
	{
		u3sendbuff[i] = UART3_Buffer_Rece[i];
	}
	u3sendbuff[6] = Hardware_CRC(u3sendbuff,6);
	u3sendbuff[7] = Hardware_CRC(u3sendbuff,6)>>8;
	
	Uart3SendBuff(u3sendbuff,8);


}

/*
*********************************************************************************************************
*	函 数 名: MODS_10H
*	功能说明: 连续写多个寄存器.  进用于改写时钟
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MODS_10H(void)
{
	uint16_t reg_addr;
	uint16_t reg_num;
	uint8_t byte_num;
	uint8_t i;
	uint16_t value;
	


	/** 第2步： 数据解析 ===========================================================================*/
	/* 数据是大端，要转换为小端 */
	reg_addr = BEBufToUint16(&UART3_Buffer_Rece[2]); 	/* 寄存器号 */
	reg_num = BEBufToUint16(&UART3_Buffer_Rece[4]);		/* 寄存器个数 */
	byte_num = UART3_Buffer_Rece[6];					/* 后面的数据体字节数 */

	/* 判断寄存器个数和后面数据字节数是否一致 */
	if (byte_num != 2 * reg_num)
	{
		;
	}
	
	/* 数据写入 */
	for (i = 0; i < reg_num; i++)
	{
		value = BEBufToUint16(&UART3_Buffer_Rece[7 + 2 * i]);	/* 寄存器值 */

		if (MODS_WriteRegValue(reg_addr + i, value) == 1)
		{
			;
		}
	}
	uart3sendlen = 0;
	for (i = 0; i < 6; i++)
	{
		u3sendbuff[i] = UART3_Buffer_Rece[i];
	}
	u3sendbuff[6] = Hardware_CRC(u3sendbuff,6);
	u3sendbuff[7] = Hardware_CRC(u3sendbuff,6)>>8;
	
	Uart3SendBuff(u3sendbuff,8);
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

static void HSNVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = HS_USART_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

//串口1
//A10(RX1)		DMA2_Stream2		chanel4	
//A9(TX1)			DMA2_Stream7		chanel4	

static void Debug_USART_DMA_INIT(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);

	//uart1dma发送		DMA2_Stream7
	DMA_DeInit(DMA2_Stream7);
	DMA_StructInit(&DMA_InitStructure);
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}
	/*  DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DEBUG_USART->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)usart1txbuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 0;//Uart1TXbuffMax;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	

	//uart1dma接收		DMA2_Stream2
	DMA_DeInit(DMA2_Stream2);  
	while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE);					//等待DMA可配置	
	/* DMA Stream */  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;						//通道选择	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DEBUG_USART->DR));	//DMA外设地址  
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)usart1rxbuff;			//DMA 存储器0地址	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;			//外设到存储器模式	
	DMA_InitStructure.DMA_BufferSize = BUFFSIZEMAX;					//数据传输量   
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设非增量模式  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//存储器增量模式  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据长度:8位  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//存储器数据长度:8位  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						// 使用普通模式   
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;					//中等优先级  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;			 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输  
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//外设突发单次传输  
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);							//初始化DMA Stream  
	//接收完成中断DMA NVIC	  
//	DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 13;	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);   
	
	DMA_Cmd(DMA2_Stream2, ENABLE);										//开启DMA传输	

//	ucRTUBuf[9]=0x55;	
//	modbus_com_send(10);	
//	delay_10ms(100);
//	
//	ucRTUBuf[9]=0xaa;	
//	modbus_com_send(10);	
//	delay_10ms(100);

}

 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(u32 baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串DEBUG_USART 模式 */
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
  NVIC_Configuration();
  
	/* 使能串口接收中断 */
	USART_ITConfig(DEBUG_USART, USART_IT_IDLE, ENABLE);
  
  USART_Cmd(DEBUG_USART, ENABLE);
	 
	USART_DMACmd(DEBUG_USART,USART_DMAReq_Rx,ENABLE); //使用rx dma   
	USART_DMACmd(DEBUG_USART,USART_DMAReq_Tx,ENABLE);//使用tx dma
	Debug_USART_DMA_INIT();
}

//返回1串口发送成功
char Uart1SendBuff(u8 *arr,u16 data_len)
{	
	u16 wait_count=0;

	while(DMA_GetCurrDataCounter(DMA2_Stream7))//等待上一次传输完成
	{ 
		delay_ms(1);
		++wait_count;
		if(wait_count>100)//1s
		{
			return 0;
		}
	}
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//清除传输完成标志位
	if(arr) 
      memcpy(usart1txbuff, arr, data_len);
	DMA_Cmd(DMA2_Stream7, DISABLE);   	//该函数必须在DMA_SetCurrDataCounter之前调用,否则不传输
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
	{
		delay_ms(1);
	}	
	
	DMA_SetCurrDataCounter(DMA2_Stream7,data_len);    //设置传送字节数      
 
	DMA_Cmd(DMA2_Stream7, ENABLE);    

	return 1;
}



//串口1
//B11(RX3)		DMA1_Stream1		chanel4	
//B10(TX3)		DMA1_Stream3		chanel4	

static void HS_USART_DMA_INIT(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);

	//uart3dma发送		DMA1_Stream3
	DMA_DeInit(DMA1_Stream3);
	DMA_StructInit(&DMA_InitStructure);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE){}
	/*  DMA Stream */
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(HS_USART->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)usart3txbuff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 0;//Uart1TXbuffMax;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;		
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);
	

	//uart1dma接收		DMA1_Stream1
	DMA_DeInit(DMA1_Stream1);  
	while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE);					//等待DMA可配置	
	/* DMA Stream */  
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;						//通道选择	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(HS_USART->DR));	//DMA外设地址  
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)usart3rxbuff;			//DMA 存储器0地址	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;			//外设到存储器模式	
	DMA_InitStructure.DMA_BufferSize = BUFFSIZEMAX;					//数据传输量   
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设非增量模式  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//存储器增量模式  
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//外设数据长度:8位  
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//存储器数据长度:8位  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;						// 使用普通模式   
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;					//中等优先级  
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;			 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;  
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;			//存储器突发单次传输  
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	//外设突发单次传输  
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);							//初始化DMA Stream  
	//接收完成中断DMA NVIC	  
//	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
	NVIC_Init(&NVIC_InitStructure);   
	
	DMA_Cmd(DMA1_Stream1, ENABLE);										//开启DMA传输	

//	ucRTUBuf[9]=0x55;	
//	modbus_com_send(10);	
//	delay_10ms(100);
//	
//	ucRTUBuf[9]=0xaa;	
//	modbus_com_send(10);	
//	delay_10ms(100);

}


 /**
  * @brief  HS_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void HS_USART_Config(u32 baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( HS_USART_RX_GPIO_CLK|HS_USART_TX_GPIO_CLK, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(HS_USART_CLK, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(HS_USART_RX_GPIO_PORT,HS_USART_RX_SOURCE, HS_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(HS_USART_TX_GPIO_PORT,HS_USART_TX_SOURCE,HS_USART_TX_AF);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = HS_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HS_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = HS_USART_RX_PIN;
  GPIO_Init(HS_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* 配置串DEBUG_USART 模式 */
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(HS_USART, &USART_InitStructure); 
  HSNVIC_Configuration();
  
	/* 使能串口接收中断 */
	USART_ITConfig(HS_USART, USART_IT_IDLE, ENABLE);
  
	USART_Cmd(HS_USART, ENABLE);
	 
	USART_DMACmd(HS_USART,USART_DMAReq_Rx,ENABLE); //使用rx dma   
	USART_DMACmd(HS_USART,USART_DMAReq_Tx,ENABLE);//使用tx dma
	HS_USART_DMA_INIT();
}

//返回3串口发送成功
char Uart3SendBuff(u8 *arr,u8 data_len)
{	
	u16 wait_count=0;

	while(DMA_GetCurrDataCounter(DMA1_Stream3))//等待上一次传输完成
	{ 
		delay_ms(1);
		++wait_count;
		if(wait_count>100)//1s
		{
			return 0;
		}
	}
	DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3);//清除传输完成标志位
	if(arr) 
      memcpy(usart3txbuff, arr, data_len);
	DMA_Cmd(DMA1_Stream3, DISABLE);   	//该函数必须在DMA_SetCurrDataCounter之前调用,否则不传输
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE)
	{
		delay_ms(1);
	}	
	
	DMA_SetCurrDataCounter(DMA1_Stream3,data_len);    //设置传送字节数      
 
	DMA_Cmd(DMA1_Stream3, ENABLE);    

	return 1;
}

//读取从机数据
void ReadSlaveData(u8 id)
{
	memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
	u3sendbuff[0] = id;
	u3sendbuff[1] = 0x03;
	u3sendbuff[2] = 0x00;
	u3sendbuff[3] = 0x00;
	u3sendbuff[4] = 0x00;
	u3sendbuff[5] = 0x06;
	u3sendbuff[6] = Hardware_CRC(u3sendbuff,6)>>8;
	u3sendbuff[7] = Hardware_CRC(u3sendbuff,6);
	Uart3SendBuff(u3sendbuff,8);
//	Usart1_Send((char *)hostsendbuff,8);
}

//设置从机参数
void SetSlavePara(u8 id)
{
	u8 sendsetnum=0;
	memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
	u3sendbuff[sendsetnum++] = id;
	u3sendbuff[sendsetnum++] = 0x10;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x04;//5
	
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = LoadSave.mode;//9
	
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetCurrent>>24;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetCurrent>>16;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetCurrent>>8;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetCurrent;//13
	
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetRdata>>24;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetRdata>>16;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetRdata>>8;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetRdata;//17
	
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetPower>>24;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetPower>>16;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetPower>>8;
	u3sendbuff[sendsetnum++] = SlaveSetPara.SetPower;//21
	
	u3sendbuff[sendsetnum++] = Hardware_CRC(u3sendbuff,u3sendbuff[5]*4+6)>>8;
	u3sendbuff[sendsetnum++] = Hardware_CRC(u3sendbuff,u3sendbuff[5]*4+6);
	Uart3SendBuff(u3sendbuff,sendsetnum);
//	Usart1_Send((char *)hostsendbuff,8);
}


//设置从机开关
void SlaveOnOff(u8 id)
{
	u8 sendsetnum=0;
	memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
	u3sendbuff[sendsetnum++] = id;
	u3sendbuff[sendsetnum++] = 0x06;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = 0x00;
	u3sendbuff[sendsetnum++] = mainswitch;//5
	
	u3sendbuff[sendsetnum++] = Hardware_CRC(u3sendbuff,6)>>8;
	u3sendbuff[sendsetnum++] = Hardware_CRC(u3sendbuff,6);
	Uart3SendBuff(u3sendbuff,sendsetnum);
//	Usart1_Send((char *)hostsendbuff,8);
}

///重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口DEBUG_USART */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口DEBUG_USART，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	
}
/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!=0xbf);
  Usart_SendByte( pUSARTx, 0xbf );
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
void Usart1_Send(char *str,u16 num)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( USART1, *(str + k) );
      k++;
  } while(k != num);
  /* 等待发送完成 */
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
  {}
}


void Send_Uart3str(char *str)
{
    Usart_SendString( USART3, str);


}
//校准
void CalHandle(u8 step)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	
	sendbuff[0] = 0x01;
	if(calpage == 0)
	{
		sendbuff[2] = step;
	}else if(calpage == 1){
		sendbuff[2] = step+16;
	}
	sendbuff[1] = 0xA5;
	if(calpage == 0)
	{
		if(step == 1 || step == 2 || step == 9 || step == 10)
		{
			sendbuff[3] = DispValue.CalValue[step-1]>>24;
			sendbuff[4] = DispValue.CalValue[step-1]>>16;
			sendbuff[5] = DispValue.CalValue[step-1]>>8;
			sendbuff[6] = DispValue.CalValue[step-1];
		}else{
			sendbuff[3] = DispValue.CalValue[step-1]/10>>24;
			sendbuff[4] = DispValue.CalValue[step-1]/10>>16;
			sendbuff[5] = DispValue.CalValue[step-1]/10>>8;
			sendbuff[6] = DispValue.CalValue[step-1]/10;
		}
	}else if(calpage == 1){
		if(step == 1 || step == 2 || step == 3 || step == 4)
		{
			sendbuff[3] = DispValue.CalValue[step+16-1]>>24;
			sendbuff[4] = DispValue.CalValue[step+16-1]>>16;
			sendbuff[5] = DispValue.CalValue[step+16-1]>>8;
			sendbuff[6] = DispValue.CalValue[step+16-1];
		}else{
			sendbuff[3] = DispValue.CalValue[step+16-1]/10>>24;
			sendbuff[4] = DispValue.CalValue[step+16-1]/10>>16;
			sendbuff[5] = DispValue.CalValue[step+16-1]/10>>8;
			sendbuff[6] = DispValue.CalValue[step+16-1]/10;
		}
	}
	sendbuff[7] = Hardware_CRC(sendbuff,7)>>8;
	sendbuff[8] = Hardware_CRC(sendbuff,7);
	Uart1SendBuff(sendbuff,9);
//	Usart1_Send((char *)sendbuff,9);
}

//设置DAC
void Set_Dac(u8 step)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[2] = 0xEE;
	sendbuff[1] = 0xA5;
	if(calpage == 0)
	{
		sendbuff[3] = dacvalue[step-1-8]>>24;
		sendbuff[4] = dacvalue[step-1-8]>>16;
		sendbuff[5] = dacvalue[step-1-8]>>8;
		sendbuff[6] = dacvalue[step-1-8];
	}else if(calpage == 1){
		sendbuff[3] = dacvalue[step+7]>>24;
		sendbuff[4] = dacvalue[step+7]>>16;
		sendbuff[5] = dacvalue[step+7]>>8;
		sendbuff[6] = dacvalue[step+7];
	}
	sendbuff[7] = Hardware_CRC(sendbuff,7)>>8;
	sendbuff[8] = Hardware_CRC(sendbuff,7);
	Uart1SendBuff(sendbuff,9);
//	Usart1_Send((char *)sendbuff,9);

}
//设置地址
void Set_Addr(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x00;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x10;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = 0x01;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//开关
void OnOff_SW(u8 value)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0C;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = value;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = value+2;
}

//模式设置
void Mode_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0D;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.mode;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//列表模式设置
void List_Mode_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0D;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.listmode[DispValue.listrunstep];
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//电池模式设置
void BatMode_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0D;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.loadmode;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//电流档位
void I_Gear_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0E;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.crange;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//电压档位
void V_Gear_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0F;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.vrange;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//远端测量开关
void Sence_SW(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x12;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.sence;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//保护标志复位
void ProtectRst(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0B;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = 0x00;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//设置波特率
void Set_Baudrate(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x11;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.Baudrate;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//设置通讯方式
void Set_Comm(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x2C;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.COMM;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//设置通讯方式
void Set_Tcp(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x2E;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = LoadSave.TCP;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//发送跳转boot命令
void Set_Boot(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x32;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x00;
	sendbuff[6] = 0x00;
	sendbuff[7] = 0x01;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//发送升级文件大小信息
void UP_FILESIZE(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x06;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x33;
	sendbuff[4] = upfilesize>>24;
	sendbuff[5] = upfilesize>>16;
	sendbuff[6] = upfilesize>>8;
	sendbuff[7] = upfilesize;
	sendbuff[8] = Hardware_CRC(sendbuff,8)>>8;
	sendbuff[9] = Hardware_CRC(sendbuff,8);
	Uart1SendBuff(sendbuff,10);
//	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//根据主机参数和从机数量算出从机参数
static void HostParaProc(void)
{
	double para;
	
	para=((double)LoadSave.current)/((double)LoadSave.devnum+1);
	SlaveSetPara.SetCurrent=(u32)para;
	
	para=((double)LoadSave.risistence)*((double)LoadSave.devnum+1);
	SlaveSetPara.SetRdata=(u32)para;
	
	para=((double)LoadSave.power)/((double)LoadSave.devnum+1);
	SlaveSetPara.SetPower=(u32)para;
}

//设置参数
void Set_Para(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x10;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x0C;
	sendbuff[4] = 0x00;
//	sendbuff[5] = 0x7C;
//	sendbuff[6] = 0x1F;
	sendbuff[5] = 0x98;
	sendbuff[6] = 0x26;
	
	sendbuff[7] = 0x00;
	sendbuff[8] = 0x00;
	sendbuff[9] = 0x00;
	sendbuff[10] = mainswitch;//开关
	
	sendbuff[15] = 0x00;
	sendbuff[16] = 0x00;
	sendbuff[17] = 0x00;
	sendbuff[18] = LoadSave.crange;//电流档位
	
	sendbuff[19] = 0x00;
	sendbuff[20] = 0x00;
	sendbuff[21] = 0x00;
	sendbuff[22] = LoadSave.vrange;//电压档位
	
	sendbuff[23] = 0x00;
	sendbuff[24] = 0x00;
	sendbuff[25] = 0x00;
	sendbuff[26] = LoadSave.Addr;//地址
	
	sendbuff[27] = 0x00;
	sendbuff[28] = 0x00;
	sendbuff[29] = 0x00;
	sendbuff[30] = LoadSave.Baudrate;//波特率
	
	sendbuff[31] = 0x00;
	sendbuff[32] = 0x00;
	sendbuff[33] = 0x00;
	sendbuff[34] = LoadSave.sence;//远端测量
	
	if(GetSystemStatus()==SYS_STATUS_LIST)//列表模式设置参数
	{
		
		
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		if(LoadSave.listmode[DispValue.listrunstep] == 4)
		{
			sendbuff[14] = 6;
		}else{
			sendbuff[14] = LoadSave.listmode[DispValue.listrunstep];//模式
		}
		
		if(LoadSave.listmode[DispValue.listrunstep] == 1)
		{
			if(LoadSave.vrange == 1)
			{
				sendbuff[7+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[8+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[9+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[10+28] = LoadSave.listvalue[DispValue.listrunstep];//设置电压值
			}else{
				sendbuff[7+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[8+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[9+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[10+28] = LoadSave.listvalue[DispValue.listrunstep];//设置电压值
			}
		}else if(LoadSave.listmode[DispValue.listrunstep] == 0){
			if(LoadSave.crange == 1)
			{
				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep];//设置电流值
//				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep]/10;//设置电流值
			}else{
				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep];//设置电流值
			}
		}else if(LoadSave.listmode[DispValue.listrunstep] == 2){
			sendbuff[15+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
			sendbuff[16+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
			sendbuff[17+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
			sendbuff[18+28] = LoadSave.listvalue[DispValue.listrunstep];//设置电阻值
		}else if(LoadSave.listmode[DispValue.listrunstep] == 3){
			sendbuff[19+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
			sendbuff[20+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
			sendbuff[21+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>8; 
			sendbuff[22+28] = LoadSave.listvalue[DispValue.listrunstep]/10;//设置功率值
		}
	}else if(GetSystemStatus()==SYS_STATUS_TEST){//常规模式设置参数
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = LoadSave.mode;//模式-
		if(LoadSave.devmode==0)//主机模式
		{
			HostParaProc();
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
			
			sendbuff[11+28] = SlaveSetPara.SetCurrent>>24;
			sendbuff[12+28] = SlaveSetPara.SetCurrent>>16;
			sendbuff[13+28] = SlaveSetPara.SetCurrent>>8;
			sendbuff[14+28] = SlaveSetPara.SetCurrent;//设置电流值
			
			sendbuff[15+28] = SlaveSetPara.SetRdata>>24;
			sendbuff[16+28] = SlaveSetPara.SetRdata>>16;
			sendbuff[17+28] = SlaveSetPara.SetRdata>>8;
			sendbuff[18+28] = SlaveSetPara.SetRdata;//设置电阻值
			
			sendbuff[19+28] = SlaveSetPara.SetPower/10>>24;
			sendbuff[20+28] = SlaveSetPara.SetPower/10>>16;
			sendbuff[21+28] = SlaveSetPara.SetPower/10>>8; 
			sendbuff[22+28] = SlaveSetPara.SetPower/10;//设置功率值
		}else{
		
			if(LoadSave.vrange == 1)
			{
				sendbuff[7+28] = LoadSave.voltage>>24;
				sendbuff[8+28] = LoadSave.voltage>>16;
				sendbuff[9+28] = LoadSave.voltage>>8;
				sendbuff[10+28] = LoadSave.voltage;//设置电压值
	//			sendbuff[7+28] = LoadSave.voltage/10>>24;
	//			sendbuff[8+28] = LoadSave.voltage/10>>16;
	//			sendbuff[9+28] = LoadSave.voltage/10>>8;
	//			sendbuff[10+28] = LoadSave.voltage/10;//设置电压值
			}else{
				sendbuff[7+28] = LoadSave.voltage>>24;
				sendbuff[8+28] = LoadSave.voltage>>16;
				sendbuff[9+28] = LoadSave.voltage>>8;
				sendbuff[10+28] = LoadSave.voltage;//设置电压值
			}
			
			if(LoadSave.crange == 1)
			{
				sendbuff[11+28] = LoadSave.current>>24;
				sendbuff[12+28] = LoadSave.current>>16;
				sendbuff[13+28] = LoadSave.current>>8;
				sendbuff[14+28] = LoadSave.current;//设置电流值
	//			sendbuff[11+28] = LoadSave.current/10>>24;
	//			sendbuff[12+28] = LoadSave.current/10>>16;
	//			sendbuff[13+28] = LoadSave.current/10>>8;
	//			sendbuff[14+28] = LoadSave.current/10;//设置电流值
			}else{
				sendbuff[11+28] = LoadSave.current>>24;
				sendbuff[12+28] = LoadSave.current>>16;
				sendbuff[13+28] = LoadSave.current>>8;
				sendbuff[14+28] = LoadSave.current;//设置电流值
			}
			
			
			sendbuff[15+28] = LoadSave.risistence>>24;
			sendbuff[16+28] = LoadSave.risistence>>16;
			sendbuff[17+28] = LoadSave.risistence>>8;
			sendbuff[18+28] = LoadSave.risistence;//设置电阻值
			
			sendbuff[19+28] = LoadSave.power/10>>24;
			sendbuff[20+28] = LoadSave.power/10>>16;
			sendbuff[21+28] = LoadSave.power/10>>8; 
			sendbuff[22+28] = LoadSave.power/10;//设置功率值
		}
	}else if(GetSystemStatus()==SYS_STATUS_BATTERY){//电池模式设置参数
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = LoadSave.loadmode;//电池测试模式
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//设置电压值
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
		}
		
		if(LoadSave.crange == 1)
		{
			if(batstep == 2)
			{
				sendbuff[11+28] = LoadSave.loadc2>>24;
				sendbuff[12+28] = LoadSave.loadc2>>16;
				sendbuff[13+28] = LoadSave.loadc2>>8;
				sendbuff[14+28] = LoadSave.loadc2;//设置电流值
//				sendbuff[11+28] = LoadSave.loadc2/10>>24;
//				sendbuff[12+28] = LoadSave.loadc2/10>>16;
//				sendbuff[13+28] = LoadSave.loadc2/10>>8;
//				sendbuff[14+28] = LoadSave.loadc2/10;//设置电流值
			}else if(batstep == 3){
				sendbuff[11+28] = LoadSave.loadc3>>24;
				sendbuff[12+28] = LoadSave.loadc3>>16;
				sendbuff[13+28] = LoadSave.loadc3>>8;
				sendbuff[14+28] = LoadSave.loadc3;//设置电流值
//				sendbuff[11+28] = LoadSave.loadc3/10>>24;
//				sendbuff[12+28] = LoadSave.loadc3/10>>16;
//				sendbuff[13+28] = LoadSave.loadc3/10>>8;
//				sendbuff[14+28] = LoadSave.loadc3/10;//设置电流值
			}else{
				sendbuff[11+28] = LoadSave.loadc1>>24;
				sendbuff[12+28] = LoadSave.loadc1>>16;
				sendbuff[13+28] = LoadSave.loadc1>>8;
				sendbuff[14+28] = LoadSave.loadc1;//设置电流值
//				sendbuff[11+28] = LoadSave.loadc1/10>>24;
//				sendbuff[12+28] = LoadSave.loadc1/10>>16;
//				sendbuff[13+28] = LoadSave.loadc1/10>>8;
//				sendbuff[14+28] = LoadSave.loadc1/10;//设置电流值
			}
		}else{
			if(batstep == 2)
			{
				sendbuff[11+28] = LoadSave.loadc2>>24;
				sendbuff[12+28] = LoadSave.loadc2>>16;
				sendbuff[13+28] = LoadSave.loadc2>>8;
				sendbuff[14+28] = LoadSave.loadc2;//设置电流值
			}else if(batstep == 3){
				sendbuff[11+28] = LoadSave.loadc3>>24;
				sendbuff[12+28] = LoadSave.loadc3>>16;
				sendbuff[13+28] = LoadSave.loadc3>>8;
				sendbuff[14+28] = LoadSave.loadc3;//设置电流值
			}else{
				sendbuff[11+28] = LoadSave.loadc1>>24;
				sendbuff[12+28] = LoadSave.loadc1>>16;
				sendbuff[13+28] = LoadSave.loadc1>>8;
				sendbuff[14+28] = LoadSave.loadc1;//设置电流值
			}
		}
		
		
		sendbuff[15+28] = LoadSave.loadr>>24;
		sendbuff[16+28] = LoadSave.loadr>>16;
		sendbuff[17+28] = LoadSave.loadr>>8;
		sendbuff[18+28] = LoadSave.loadr;//设置电阻值
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//设置功率值
	}else if(GetSystemStatus()==SYS_STATUS_DYNAMIC){//动态模式设置参数
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = 4;//模式
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//设置电压值
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
		}
		
		if(LoadSave.crange == 1)
		{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//设置电流值
//			sendbuff[11+28] = LoadSave.current/10>>24;
//			sendbuff[12+28] = LoadSave.current/10>>16;
//			sendbuff[13+28] = LoadSave.current/10>>8;
//			sendbuff[14+28] = LoadSave.current/10;//设置电流值
		}else{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//设置电流值
		}
		
		
		sendbuff[15+28] = LoadSave.risistence>>24;
		sendbuff[16+28] = LoadSave.risistence>>16;
		sendbuff[17+28] = LoadSave.risistence>>8;
		sendbuff[18+28] = LoadSave.risistence;//设置电阻值
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//设置功率值
	}else if(GetSystemStatus()==SYS_STATUS_LED){//动态模式设置参数
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = 5;//模式
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//设置电压值
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//设置电压值
		}
		
		if(LoadSave.crange == 1)
		{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//设置电流值
//			sendbuff[11+28] = LoadSave.current/10>>24;
//			sendbuff[12+28] = LoadSave.current/10>>16;
//			sendbuff[13+28] = LoadSave.current/10>>8;
//			sendbuff[14+28] = LoadSave.current/10;//设置电流值
		}else{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//设置电流值
		}
		
		
		sendbuff[15+28] = LoadSave.risistence>>24;
		sendbuff[16+28] = LoadSave.risistence>>16;
		sendbuff[17+28] = LoadSave.risistence>>8;
		sendbuff[18+28] = LoadSave.risistence;//设置电阻值
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//设置功率值
	}
	
	sendbuff[23+28] = LoadSave.ovp>>24;
	sendbuff[24+28] = LoadSave.ovp>>16;
	sendbuff[25+28] = LoadSave.ovp>>8;
	sendbuff[26+28] = LoadSave.ovp;//过电压保护
	
	sendbuff[27+28] = LoadSave.ocp>>24;
	sendbuff[28+28] = LoadSave.ocp>>16;
	sendbuff[29+28] = LoadSave.ocp>>8;
	sendbuff[30+28] = LoadSave.ocp;//过电流保护
	
	sendbuff[31+28] = LoadSave.opp>>24;
	sendbuff[32+28] = LoadSave.opp>>16;
	sendbuff[33+28] = LoadSave.opp>>8;
	sendbuff[34+28] = LoadSave.opp;//过功率保护
	
	sendbuff[35+28] = LoadSave.maxp/10>>24;
	sendbuff[36+28] = LoadSave.maxp/10>>16;
	sendbuff[37+28] = LoadSave.maxp/10>>8;
	sendbuff[38+28] = LoadSave.maxp/10;//最大限制功率
	
	sendbuff[39+28] = LoadSave.maxv/10>>24;
	sendbuff[40+28] = LoadSave.maxv/10>>16;
	sendbuff[41+28] = LoadSave.maxv/10>>8;
	sendbuff[42+28] = LoadSave.maxv/10;//最大限制电压
	
	sendbuff[43+28] = LoadSave.maxc/10>>24;
	sendbuff[44+28] = LoadSave.maxc/10>>16;
	sendbuff[45+28] = LoadSave.maxc/10>>8;
	sendbuff[46+28] = LoadSave.maxc/10;//最大限制电流
	
	if(GetSystemStatus()==SYS_STATUS_LIST)
	{
		if(LoadSave.gatev == 0)
		{
			sendbuff[47+28] = LoadSave.listonvol/10>>24;
			sendbuff[48+28] = LoadSave.listonvol/10>>16;
			sendbuff[49+28] = LoadSave.listonvol/10>>8;
			sendbuff[50+28] = LoadSave.listonvol/10;//加载电压
		}else{
			sendbuff[47+28] = 0;
			sendbuff[48+28] = 0;
			sendbuff[49+28] = 0;
			sendbuff[50+28] = 0;//加载电压
		}
	}else{
		sendbuff[47+28] = LoadSave.onvol/10>>24;
		sendbuff[48+28] = LoadSave.onvol/10>>16;
		sendbuff[49+28] = LoadSave.onvol/10>>8;
		sendbuff[50+28] = LoadSave.onvol/10;//加载电压
	}
	
	sendbuff[51+28] = LoadSave.offvol/10>>24;
	sendbuff[52+28] = LoadSave.offvol/10>>16;
	sendbuff[53+28] = LoadSave.offvol/10>>8;
	sendbuff[54+28] = LoadSave.offvol/10;//卸载电压
	
	if(LoadSave.crange == 1)
	{
		sendbuff[55+28] = LoadSave.crise>>24;
		sendbuff[56+28] = LoadSave.crise>>16;
		sendbuff[57+28] = LoadSave.crise>>8;
		sendbuff[58+28] = LoadSave.crise;//电流爬升率
		
		sendbuff[59+28] = LoadSave.cdrop>>24;
		sendbuff[60+28] = LoadSave.cdrop>>16;
		sendbuff[61+28] = LoadSave.cdrop>>8;
		sendbuff[62+28] = LoadSave.cdrop;//电流下降率
		
//		sendbuff[55+28] = LoadSave.crise/10>>24;
//		sendbuff[56+28] = LoadSave.crise/10>>16;
//		sendbuff[57+28] = LoadSave.crise/10>>8;
//		sendbuff[58+28] = LoadSave.crise/10;//电流爬升率
//		
//		sendbuff[59+28] = LoadSave.cdrop/10>>24;
//		sendbuff[60+28] = LoadSave.cdrop/10>>16;
//		sendbuff[61+28] = LoadSave.cdrop/10>>8;
//		sendbuff[62+28] = LoadSave.cdrop/10;//电流下降率
	}else if(LoadSave.crange == 0){
		sendbuff[55+28] = LoadSave.crise>>24;
		sendbuff[56+28] = LoadSave.crise>>16;
		sendbuff[57+28] = LoadSave.crise>>8;
		sendbuff[58+28] = LoadSave.crise;//电流爬升率
		
		sendbuff[59+28] = LoadSave.cdrop>>24;
		sendbuff[60+28] = LoadSave.cdrop>>16;
		sendbuff[61+28] = LoadSave.cdrop>>8;
		sendbuff[62+28] = LoadSave.cdrop;//电流下降率
	}
	
	if(LoadSave.vrange == 1)
	{
		sendbuff[63+28] = LoadSave.cvdowntime>>24;
		sendbuff[64+28] = LoadSave.cvdowntime>>16;
		sendbuff[65+28] = LoadSave.cvdowntime>>8;
		sendbuff[66+28] = LoadSave.cvdowntime;//CV模式电压下降时间
//		sendbuff[63+28] = LoadSave.cvdowntime/10>>24;
//		sendbuff[64+28] = LoadSave.cvdowntime/10>>16;
//		sendbuff[65+28] = LoadSave.cvdowntime/10>>8;
//		sendbuff[66+28] = LoadSave.cvdowntime/10;//CV模式电压下降时间
	}else if(LoadSave.vrange == 0){
		sendbuff[63+28] = LoadSave.cvdowntime>>24;
		sendbuff[64+28] = LoadSave.cvdowntime>>16;
		sendbuff[65+28] = LoadSave.cvdowntime>>8;
		sendbuff[66+28] = LoadSave.cvdowntime;//CV模式电压下降时间
	}
	
	
	sendbuff[67+28] = LoadSave.ledvo>>24;
	sendbuff[68+28] = LoadSave.ledvo>>16;
	sendbuff[69+28] = LoadSave.ledvo>>8;
	sendbuff[70+28] = LoadSave.ledvo;//LED模式顺向工作电压
	
	sendbuff[71+28] = LoadSave.ledio>>24;
	sendbuff[72+28] = LoadSave.ledio>>16;
	sendbuff[73+28] = LoadSave.ledio>>8;
	sendbuff[74+28] = LoadSave.ledio;//LED模式顺向工作电流
	
	sendbuff[75+28] = LoadSave.ledrd>>24;
	sendbuff[76+28] = LoadSave.ledrd>>16;
	sendbuff[77+28] = LoadSave.ledrd>>8;
	sendbuff[78+28] = LoadSave.ledrd;//LED模式RD系数，此参数是用来计算加载电压阈值范围为0-100%，例如：设置VO为30V RD-0.2  此时加载电压应为30*0.2=6，VI=30-6=24V。
	
	if(LoadSave.crange == 1)
	{
		sendbuff[79+28] = LoadSave.valA>>24;
		sendbuff[80+28] = LoadSave.valA>>16;
		sendbuff[81+28] = LoadSave.valA>>8;
		sendbuff[82+28] = LoadSave.valA;//动态模式拉载电流A
		
		sendbuff[83+28] = LoadSave.valB>>24;
		sendbuff[84+28] = LoadSave.valB>>16;
		sendbuff[85+28] = LoadSave.valB>>8;
		sendbuff[86+28] = LoadSave.valB;//动态模式拉载电流B
		
//		sendbuff[79+28] = LoadSave.valA/10>>24;
//		sendbuff[80+28] = LoadSave.valA/10>>16;
//		sendbuff[81+28] = LoadSave.valA/10>>8;
//		sendbuff[82+28] = LoadSave.valA/10;//动态模式拉载电流A
//		
//		sendbuff[83+28] = LoadSave.valB/10>>24;
//		sendbuff[84+28] = LoadSave.valB/10>>16;
//		sendbuff[85+28] = LoadSave.valB/10>>8;
//		sendbuff[86+28] = LoadSave.valB/10;//动态模式拉载电流B
	}else if(LoadSave.crange == 0){
		sendbuff[79+28] = LoadSave.valA>>24;
		sendbuff[80+28] = LoadSave.valA>>16;
		sendbuff[81+28] = LoadSave.valA>>8;
		sendbuff[82+28] = LoadSave.valA;//动态模式拉载电流A
		
		sendbuff[83+28] = LoadSave.valB>>24;
		sendbuff[84+28] = LoadSave.valB>>16;
		sendbuff[85+28] = LoadSave.valB>>8;
		sendbuff[86+28] = LoadSave.valB;//动态模式拉载电流B
	}
	
	sendbuff[87+28] = LoadSave.timeA>>24;
	sendbuff[88+28] = LoadSave.timeA>>16;
	sendbuff[89+28] = LoadSave.timeA>>8;
	sendbuff[90+28] = LoadSave.timeA;//动态模式拉载持续时间A
	
	sendbuff[91+28] = LoadSave.timeB>>24;
	sendbuff[92+28] = LoadSave.timeB>>16;
	sendbuff[93+28] = LoadSave.timeB>>8;
	sendbuff[94+28] = LoadSave.timeB;//动态模式拉载持续时间B
	
	sendbuff[95+28] = LoadSave.dynaIRise>>24;
	sendbuff[96+28] = LoadSave.dynaIRise>>16;
	sendbuff[97+28] = LoadSave.dynaIRise>>8;
	sendbuff[98+28] = LoadSave.dynaIRise;//动态模式电流上升率
	
	sendbuff[99+28] = LoadSave.dynaIDrop>>24;
	sendbuff[100+28] = LoadSave.dynaIDrop>>16;
	sendbuff[101+28] = LoadSave.dynaIDrop>>8;
	sendbuff[102+28] = LoadSave.dynaIDrop;//动态模式电流下降率
	
	sendbuff[103+28] = LoadSave.dynamode>>24;
	sendbuff[104+28] = LoadSave.dynamode>>16;
	sendbuff[105+28] = LoadSave.dynamode>>8;
	sendbuff[106+28] = LoadSave.dynamode;//动态触发模式
	
	sendbuff[107+28] = LoadSave.COMM>>24;
	sendbuff[108+28] = LoadSave.COMM>>16;
	sendbuff[109+28] = LoadSave.COMM>>8;
	sendbuff[110+28] = LoadSave.COMM;//通讯选择
	
	sendbuff[111+28] = LoadSave.Version>>24;
	sendbuff[112+28] = LoadSave.Version>>16;
	sendbuff[113+28] = LoadSave.Version>>8;
	sendbuff[114+28] = LoadSave.Version;//版本
	
	sendbuff[115+28] = LoadSave.TCP>>24;
	sendbuff[116+28] = LoadSave.TCP>>16;
	sendbuff[117+28] = LoadSave.TCP>>8;
	sendbuff[118+28] = LoadSave.TCP;//协议选择
	
	sendbuff[119+28] = LoadSave.facmaxpow>>24;
	sendbuff[120+28] = LoadSave.facmaxpow>>16;
	sendbuff[121+28] = LoadSave.facmaxpow>>8;
	sendbuff[122+28] = LoadSave.facmaxpow;//快充模式选择开关
	
	sendbuff[123+28] = LoadSave.facmaxvol>>24;
	sendbuff[124+28] = LoadSave.facmaxvol>>16;
	sendbuff[125+28] = LoadSave.facmaxvol>>8; 
	sendbuff[126+28] = LoadSave.facmaxvol;//快充模式选择开关
	
	sendbuff[127+28] = LoadSave.facmaxcur>>24;
	sendbuff[128+28] = LoadSave.facmaxcur>>16;
	sendbuff[129+28] = LoadSave.facmaxcur>>8;
	sendbuff[130+28] = LoadSave.facmaxcur;//快充模式选择开关
	sendbuff[131+28] = Hardware_CRC(sendbuff,131+28)>>8;
	sendbuff[132+28] = Hardware_CRC(sendbuff,131+28);
	Uart1SendBuff(sendbuff,133+28);
//	Usart1_Send((char *)sendbuff,133+28);
	setflag = 1;
}

////设置参数
//void Set_Para(void)
//{
//	memset((char *)sendbuff,0,sizeof(sendbuff));
//	sendbuff[0] = 0x01;
//	sendbuff[1] = 0x10;
//	sendbuff[2] = 0x00;
//	sendbuff[3] = 0x13;
//	sendbuff[4] = 0x00;
//	sendbuff[5] = 0x7C;
//	sendbuff[6] = 0x1F;
//	if(GetSystemStatus()==SYS_STATUS_LIST)//列表模式设置参数
//	{
//		if(LoadSave.listmode[DispValue.listrunstep] == 1)
//		{
//			if(LoadSave.vrange == 1)
//			{
//				sendbuff[7] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[8] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[9] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[10] = LoadSave.listvalue[DispValue.listrunstep]/10;//设置电压值
//			}else{
//				sendbuff[7] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//				sendbuff[8] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//				sendbuff[9] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//				sendbuff[10] = LoadSave.listvalue[DispValue.listrunstep];//设置电压值
//			}
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 0){
//			if(LoadSave.crange == 1)
//			{
//				sendbuff[11] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[12] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[13] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[14] = LoadSave.listvalue[DispValue.listrunstep]/10;//设置电流值
//			}else{
//				sendbuff[11] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//				sendbuff[12] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//				sendbuff[13] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//				sendbuff[14] = LoadSave.listvalue[DispValue.listrunstep];//设置电流值
//			}
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 2){
//			sendbuff[15] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//			sendbuff[16] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//			sendbuff[17] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//			sendbuff[18] = LoadSave.listvalue[DispValue.listrunstep];//设置电阻值
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 3){
//			sendbuff[19] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//			sendbuff[20] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//			sendbuff[21] = LoadSave.listvalue[DispValue.listrunstep]>>8; 
//			sendbuff[22] = LoadSave.listvalue[DispValue.listrunstep];//设置功率值
//		}
//	}else if(GetSystemStatus()==SYS_STATUS_TEST){//常规模式设置参数
//		if(LoadSave.vrange == 1)
//		{
//			sendbuff[7] = LoadSave.voltage/10>>24;
//			sendbuff[8] = LoadSave.voltage/10>>16;
//			sendbuff[9] = LoadSave.voltage/10>>8;
//			sendbuff[10] = LoadSave.voltage/10;//设置电压值
//		}else{
//			sendbuff[7] = LoadSave.voltage>>24;
//			sendbuff[8] = LoadSave.voltage>>16;
//			sendbuff[9] = LoadSave.voltage>>8;
//			sendbuff[10] = LoadSave.voltage;//设置电压值
//		}
//		
//		if(LoadSave.crange == 1)
//		{
//			sendbuff[11] = LoadSave.current/10>>24;
//			sendbuff[12] = LoadSave.current/10>>16;
//			sendbuff[13] = LoadSave.current/10>>8;
//			sendbuff[14] = LoadSave.current/10;//设置电流值
//		}else{
//			sendbuff[11] = LoadSave.current>>24;
//			sendbuff[12] = LoadSave.current>>16;
//			sendbuff[13] = LoadSave.current>>8;
//			sendbuff[14] = LoadSave.current;//设置电流值
//		}
//		
//		
//		sendbuff[15] = LoadSave.risistence>>24;
//		sendbuff[16] = LoadSave.risistence>>16;
//		sendbuff[17] = LoadSave.risistence>>8;
//		sendbuff[18] = LoadSave.risistence;//设置电阻值
//		
//		sendbuff[19] = LoadSave.power>>24;
//		sendbuff[20] = LoadSave.power>>16;
//		sendbuff[21] = LoadSave.power>>8; 
//		sendbuff[22] = LoadSave.power;//设置功率值
//	}
//	
//	sendbuff[23] = LoadSave.ovp>>24;
//	sendbuff[24] = LoadSave.ovp>>16;
//	sendbuff[25] = LoadSave.ovp>>8;
//	sendbuff[26] = LoadSave.ovp;//过电压保护
//	
//	sendbuff[27] = LoadSave.ocp>>24;
//	sendbuff[28] = LoadSave.ocp>>16;
//	sendbuff[29] = LoadSave.ocp>>8;
//	sendbuff[30] = LoadSave.ocp;//过电流保护
//	
//	sendbuff[31] = LoadSave.opp>>24;
//	sendbuff[32] = LoadSave.opp>>16;
//	sendbuff[33] = LoadSave.opp>>8;
//	sendbuff[34] = LoadSave.opp;//过功率保护
//	
//	sendbuff[35] = LoadSave.maxp>>24;
//	sendbuff[36] = LoadSave.maxp>>16;
//	sendbuff[37] = LoadSave.maxp>>8;
//	sendbuff[38] = LoadSave.maxp;//最大限制功率
//	
//	sendbuff[39] = LoadSave.maxv>>24;
//	sendbuff[40] = LoadSave.maxv>>16;
//	sendbuff[41] = LoadSave.maxv>>8;
//	sendbuff[42] = LoadSave.maxv;//最大限制电压
//	
//	sendbuff[43] = LoadSave.maxc>>24;
//	sendbuff[44] = LoadSave.maxc>>16;
//	sendbuff[45] = LoadSave.maxc>>8;
//	sendbuff[46] = LoadSave.maxc;//最大限制电流
//	
//	sendbuff[47] = LoadSave.onvol>>24;
//	sendbuff[48] = LoadSave.onvol>>16;
//	sendbuff[49] = LoadSave.onvol>>8;
//	sendbuff[50] = LoadSave.onvol;//加载电压
//	
//	sendbuff[51] = LoadSave.offvol>>24;
//	sendbuff[52] = LoadSave.offvol>>16;
//	sendbuff[53] = LoadSave.offvol>>8;
//	sendbuff[54] = LoadSave.offvol;//卸载电压
//	
//	sendbuff[55] = LoadSave.crise>>24;
//	sendbuff[56] = LoadSave.crise>>16;
//	sendbuff[57] = LoadSave.crise>>8;
//	sendbuff[58] = LoadSave.crise;//电流爬升率
//	
//	sendbuff[59] = LoadSave.cdrop>>24;
//	sendbuff[60] = LoadSave.cdrop>>16;
//	sendbuff[61] = LoadSave.cdrop>>8;
//	sendbuff[62] = LoadSave.cdrop;//电流下降率
//	
//	sendbuff[63] = LoadSave.cvdowntime>>24;
//	sendbuff[64] = LoadSave.cvdowntime>>16;
//	sendbuff[65] = LoadSave.cvdowntime>>8;
//	sendbuff[66] = LoadSave.cvdowntime;//CV模式电压下降时间
//	
//	sendbuff[67] = LoadSave.ledvo>>24;
//	sendbuff[68] = LoadSave.ledvo>>16;
//	sendbuff[69] = LoadSave.ledvo>>8;
//	sendbuff[70] = LoadSave.ledvo;//LED模式顺向工作电压
//	
//	sendbuff[71] = LoadSave.ledio>>24;
//	sendbuff[72] = LoadSave.ledio>>16;
//	sendbuff[73] = LoadSave.ledio>>8;
//	sendbuff[74] = LoadSave.ledio;//LED模式顺向工作电流
//	
//	sendbuff[75] = LoadSave.ledrd>>24;
//	sendbuff[76] = LoadSave.ledrd>>16;
//	sendbuff[77] = LoadSave.ledrd>>8;
//	sendbuff[78] = LoadSave.ledrd;//LED模式RD系数，此参数是用来计算加载电压阈值范围为0-100%，例如：设置VO为30V RD-0.2  此时加载电压应为30*0.2=6，VI=30-6=24V。
//	
//	sendbuff[79] = LoadSave.valA>>24;
//	sendbuff[80] = LoadSave.valA>>16;
//	sendbuff[81] = LoadSave.valA>>8;
//	sendbuff[82] = LoadSave.valA;//动态模式拉载电流A
//	
//	sendbuff[83] = LoadSave.valB>>24;
//	sendbuff[84] = LoadSave.valB>>16;
//	sendbuff[85] = LoadSave.valB>>8;
//	sendbuff[86] = LoadSave.valB;//动态模式拉载电流B
//	
//	sendbuff[87] = LoadSave.timeA>>24;
//	sendbuff[88] = LoadSave.timeA>>16;
//	sendbuff[89] = LoadSave.timeA>>8;
//	sendbuff[90] = LoadSave.timeA;//动态模式拉载持续时间A
//	
//	sendbuff[91] = LoadSave.timeB>>24;
//	sendbuff[92] = LoadSave.timeB>>16;
//	sendbuff[93] = LoadSave.timeB>>8;
//	sendbuff[94] = LoadSave.timeB;//动态模式拉载持续时间B
//	
//	sendbuff[95] = LoadSave.dynaIRise>>24;
//	sendbuff[96] = LoadSave.dynaIRise>>16;
//	sendbuff[97] = LoadSave.dynaIRise>>8;
//	sendbuff[98] = LoadSave.dynaIRise;//动态模式电流上升率
//	
//	sendbuff[99] = LoadSave.dynaIDrop>>24;
//	sendbuff[100] = LoadSave.dynaIDrop>>16;
//	sendbuff[101] = LoadSave.dynaIDrop>>8;
//	sendbuff[102] = LoadSave.dynaIDrop;//动态模式电流下降率
//	
//	sendbuff[103] = LoadSave.dynamode>>24;
//	sendbuff[104] = LoadSave.dynamode>>16;
//	sendbuff[105] = LoadSave.dynamode>>8;
//	sendbuff[106] = LoadSave.dynamode;//动态触发模式
//	
//	sendbuff[105] = LoadSave.qcmode>>24;
//	sendbuff[106] = LoadSave.qcmode>>16;
//	sendbuff[107] = LoadSave.qcmode>>8;
//	sendbuff[108] = LoadSave.qcmode;//快充模式选择开关
//	
//	sendbuff[109] = LoadSave.Class_5>>24;
//	sendbuff[110] = LoadSave.Class_5>>16;
//	sendbuff[111] = LoadSave.Class_5>>8;
//	sendbuff[112] = LoadSave.Class_5;//快充模式选择开关
//	
//	sendbuff[115] = LoadSave.Class_9>>24;
//	sendbuff[116] = LoadSave.Class_9>>16;
//	sendbuff[117] = LoadSave.Class_9>>8;
//	sendbuff[118] = LoadSave.Class_9;//快充模式选择开关
//	
//	sendbuff[119] = LoadSave.Class_12>>24;
//	sendbuff[120] = LoadSave.Class_12>>16;
//	sendbuff[121] = LoadSave.Class_12>>8;
//	sendbuff[122] = LoadSave.Class_12;//快充模式选择开关
//	
//	sendbuff[123] = LoadSave.Class_15>>24;
//	sendbuff[124] = LoadSave.Class_15>>16;
//	sendbuff[125] = LoadSave.Class_15>>8; 
//	sendbuff[126] = LoadSave.Class_15;//快充模式选择开关
//	
//	sendbuff[127] = LoadSave.Class_20>>24;
//	sendbuff[128] = LoadSave.Class_20>>16;
//	sendbuff[129] = LoadSave.Class_20>>8;
//	sendbuff[130] = LoadSave.Class_20;//快充模式选择开关
//	sendbuff[131] = Hardware_CRC(sendbuff,131)>>8;
//	sendbuff[132] = Hardware_CRC(sendbuff,131);
//	Usart1_Send((char *)sendbuff,133);
//	setflag = 1;
//}
//读取数据
void ReadData(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x03;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x00;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x13;
	sendbuff[6] = Hardware_CRC(sendbuff,6)>>8;
	sendbuff[7] = Hardware_CRC(sendbuff,6);
	Uart1SendBuff(sendbuff,8);
//	Usart1_Send((char *)sendbuff,8);
}

//读取启动模式
void ReadBootMode(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x03;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x34;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x01;
	sendbuff[6] = Hardware_CRC(sendbuff,6)>>8;
	sendbuff[7] = Hardware_CRC(sendbuff,6);
	Uart1SendBuff(sendbuff,8);
//	Usart1_Send((char *)sendbuff,8);
}

//读取采集板版本
void ReadVersoin(void)
{
	memset((char *)sendbuff,0,sizeof(sendbuff));
	sendbuff[0] = 0x01;
	sendbuff[1] = 0x03;
	sendbuff[2] = 0x00;
	sendbuff[3] = 0x35;
	sendbuff[4] = 0x00;
	sendbuff[5] = 0x01;
	sendbuff[6] = Hardware_CRC(sendbuff,6)>>8;
	sendbuff[7] = Hardware_CRC(sendbuff,6);
	Uart1SendBuff(sendbuff,8);
//	Usart1_Send((char *)sendbuff,8);
}

void DispRead(void)
{
	u8 textbuf[50];
	Colour.black=LCD_COLOR_TEST_BACK;
	sprintf((char *)textbuf,"读取成功,文件大小为%d字节,请按发送文件",upfilesize);
	WriteString_16(2,50,textbuf,  0);
}

void Rec_Handle(void)	
{
	static u8 sumcount;
	vu16 crc_result;
	u32 readbuf;
	memcpy(UART_Buffer_Rece, usart1rxbuff, 256);
	if (UART_Buffer_Rece[0] == 0x01)
	{
		if(UART_Buffer_Rece[1] == 0x03)//读取采集板数据响应
		{
			crc_result = (UART_Buffer_Rece[79] << 8) + UART_Buffer_Rece[80];
			if ((crc_result == Hardware_CRC(UART_Buffer_Rece,79)) ||(crc_result == 0) )
			{
				readbuf = 0;
				readbuf += UART_Buffer_Rece[3] << 24;
				readbuf += UART_Buffer_Rece[4] << 16;
				readbuf += UART_Buffer_Rece[5] << 8;
				readbuf += UART_Buffer_Rece[6];
//				if(sumcount < DISP_FILTER)
//				{
//					vsum += readbuf;
//				}else{
//					DispValue.Voltage = vsum/DISP_FILTER;
//					vsum = 0;
//				}
				DispValue.Voltage = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[7] << 24;
				readbuf += UART_Buffer_Rece[8] << 16;
				readbuf += UART_Buffer_Rece[9] << 8;
				readbuf += UART_Buffer_Rece[10];
//				if(sumcount < DISP_FILTER)
//				{
//					isum += readbuf;
//				}else{
//					DispValue.Current = isum/DISP_FILTER;
//					isum = 0;
//				}
				DispValue.Current = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[11] << 24;
				readbuf += UART_Buffer_Rece[12] << 16;
				readbuf += UART_Buffer_Rece[13] << 8;
				readbuf += UART_Buffer_Rece[14];
//				if(sumcount < DISP_FILTER)
//				{
//					rsum += readbuf;
//				}else{
//					DispValue.Rdata = rsum/DISP_FILTER;
//					rsum = 0;
//				}
				DispValue.Rdata = readbuf;
				
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[15] << 24;
				readbuf += UART_Buffer_Rece[16] << 16;
				readbuf += UART_Buffer_Rece[17] << 8;
				readbuf += UART_Buffer_Rece[18];			
//				if(sumcount < DISP_FILTER)
//				{
//					psum += readbuf;
//				}else{
//					DispValue.Power = psum/DISP_FILTER;
//					psum = 0;
//				}
				DispValue.Power = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[19] << 24;
				readbuf += UART_Buffer_Rece[20] << 16;
				readbuf += UART_Buffer_Rece[21] << 8;
				readbuf += UART_Buffer_Rece[22];	
				DispValue.version = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[43] << 24;
				readbuf += UART_Buffer_Rece[44] << 16;
				readbuf += UART_Buffer_Rece[45] << 8;
				readbuf += UART_Buffer_Rece[46];
				DispValue.Operation_MODE = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[47] << 24;
				readbuf += UART_Buffer_Rece[48] << 16;
				readbuf += UART_Buffer_Rece[49] << 8;
				readbuf += UART_Buffer_Rece[50];
				DispValue.protectflag = readbuf;
				if(DispValue.protectflag == 2)
				{
					DispValue.alertdisp = 1;
				}
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[51] << 24;
				readbuf += UART_Buffer_Rece[52] << 16;
				readbuf += UART_Buffer_Rece[53] << 8;
				readbuf += UART_Buffer_Rece[54];
				if(switchdelay == 0)
				{
					mainswitch = readbuf;
					if(mainswitch == 0)
					{
						SwitchLedOff();
					}
					if(mainswitch == 1)
					{
						SwitchLedOn();
					}
				}else{
					switchdelay --;
				}
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[59] << 24;
				readbuf += UART_Buffer_Rece[60] << 16;
				readbuf += UART_Buffer_Rece[61] << 8;
				readbuf += UART_Buffer_Rece[62];
				LoadSave.crange = readbuf;
				
				readbuf = 0;
				readbuf += UART_Buffer_Rece[63] << 24;
				readbuf += UART_Buffer_Rece[64] << 16;
				readbuf += UART_Buffer_Rece[65] << 8;
				readbuf += UART_Buffer_Rece[66];
				LoadSave.vrange = readbuf;
				
				if(sumcount < DISP_FILTER)
				{
					sumcount ++;
				}else{
					sumcount = 0;
				}
			}  
			readflag=0;
		}else if(UART_Buffer_Rece[1] == 0x06){//采集板设置单个寄存器响应
			setflag = 0;
			if(sendbuff[3] == 0x32)
			{
				UpPara.updatestep=2;
				Colour.Fword=Red;
				Colour.black=LCD_COLOR_TEST_BACK;
				WriteString_16(2, 30, "跳转成功,请按读取文件",  0);
			}else if(sendbuff[3] == 0x33){
				UpPara.updatestep=3;
				DispRead();
			}
		}else if(UART_Buffer_Rece[1] == 0x10){//采集板设置多个寄存器响应
			setflag = 0;
		}else if(UART_Buffer_Rece[1] == 0x50){//采集板接收升级包数据校验结果响应
			if(UART_Buffer_Rece[7] == 0x01)
			{
				UpPara.sendresflag = 0;
			}else if(UART_Buffer_Rece[7] == 0xFF){
				UpError(1);
			}
		}else if(UART_Buffer_Rece[1] == 0x51){//采集板升级完成响应
			WriteString_16(2, 120, "升级成功",  0);
		}
	}
}

void Rec3_Handle(void)	
{
	vu16 crc_result;
	u32 readbuf;
	u8 sendnum=0;
	memcpy(UART3_Buffer_Rece, usart3rxbuff, 256);
	
	if(LoadSave.devmode==2)
		crc_result = (UART3_Buffer_Rece[Uart3RXbuff_len-1] << 8) + UART3_Buffer_Rece[Uart3RXbuff_len-2];
	else
		crc_result = (UART3_Buffer_Rece[Uart3RXbuff_len-2] << 8) + UART3_Buffer_Rece[Uart3RXbuff_len-1];
	if(crc_result == Hardware_CRC(UART3_Buffer_Rece,Uart3RXbuff_len-2))//
	{
		if(UART3_Buffer_Rece[1] == 0x03)//判断功能码
		{
			if(LoadSave.devmode==0)//主机模式
			{
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[3] << 24;
				readbuf += UART3_Buffer_Rece[4] << 16;
				readbuf += UART3_Buffer_Rece[5] << 8;
				readbuf += UART3_Buffer_Rece[6];
				SlaveValue.Voltage[UART3_Buffer_Rece[0]-1] = readbuf;
				
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[7] << 24;
				readbuf += UART3_Buffer_Rece[8] << 16;
				readbuf += UART3_Buffer_Rece[9] << 8;
				readbuf += UART3_Buffer_Rece[10];
				SlaveValue.Current[UART3_Buffer_Rece[0]-1] = readbuf;
				
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[11] << 24;
				readbuf += UART3_Buffer_Rece[12] << 16;
				readbuf += UART3_Buffer_Rece[13] << 8;
				readbuf += UART3_Buffer_Rece[14];
				SlaveValue.Rdata[UART3_Buffer_Rece[0]-1] = readbuf;
				
				
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[15] << 24;
				readbuf += UART3_Buffer_Rece[16] << 16;
				readbuf += UART3_Buffer_Rece[17] << 8;
				readbuf += UART3_Buffer_Rece[18];			
				SlaveValue.Power[UART3_Buffer_Rece[0]-1] = readbuf;
				
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[19] << 24;
				readbuf += UART3_Buffer_Rece[20] << 16;
				readbuf += UART3_Buffer_Rece[21] << 8;
				readbuf += UART3_Buffer_Rece[22];
				SlaveValue.vrange[UART3_Buffer_Rece[0]-1] = readbuf;
				
				
				readbuf = 0;
				readbuf += UART3_Buffer_Rece[23] << 24;
				readbuf += UART3_Buffer_Rece[24] << 16;
				readbuf += UART3_Buffer_Rece[25] << 8;
				readbuf += UART3_Buffer_Rece[26];			
				SlaveValue.crange[UART3_Buffer_Rece[0]-1] = readbuf;
			}else if(LoadSave.devmode==1){//从机模式
				if(UART3_Buffer_Rece[0] == LoadSave.slaveNo)
				{
					memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
					u3sendbuff[sendnum++] = UART3_Buffer_Rece[0];
					u3sendbuff[sendnum++] = UART3_Buffer_Rece[1];;
					u3sendbuff[sendnum++] = UART3_Buffer_Rece[5]*4;
					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>24);
					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>16);
					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>8);
					u3sendbuff[sendnum++] = (u8)DispValue.Voltage;
					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>24);
					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>16);
					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>8);
					u3sendbuff[sendnum++] = (u8)DispValue.Current;
					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>24);
					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>16);
					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>8);
					u3sendbuff[sendnum++] = (u8)DispValue.Rdata;
					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>24);
					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>16);
					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>8);
					u3sendbuff[sendnum++] = (u8)DispValue.Power;
					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>24);
					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>16);
					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>8);
					u3sendbuff[sendnum++] = (u8)LoadSave.vrange;
					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>24);
					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>16);
					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>8);
					u3sendbuff[sendnum++] = (u8)LoadSave.crange;
					u3sendbuff[sendnum++] = Hardware_CRC(u3sendbuff,UART3_Buffer_Rece[5]*4+3)>>8;
					u3sendbuff[sendnum++] = Hardware_CRC(u3sendbuff,UART3_Buffer_Rece[5]*4+3);
					Uart3SendBuff(u3sendbuff,sendnum);
				}
			}else if(LoadSave.devmode==2){//普通模式
				memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
				lockflag = 1;
				DrawLock(lockflag);
				MODS_03H();
			}
		}else if(UART3_Buffer_Rece[1] == 0x10){
			if(LoadSave.devmode==0)//主机模式
			{
				setslaveflag--;
			}else if(LoadSave.devmode==1){//从机模式
				if(UART3_Buffer_Rece[0] == LoadSave.slaveNo)
				{
					LoadSave.mode=UART3_Buffer_Rece[9];
					
					readbuf = 0;
					readbuf += UART3_Buffer_Rece[10] << 24;
					readbuf += UART3_Buffer_Rece[11] << 16;
					readbuf += UART3_Buffer_Rece[12] << 8;
					readbuf += UART3_Buffer_Rece[13];
					LoadSave.current = readbuf;
					
					readbuf = 0;
					readbuf += UART3_Buffer_Rece[14] << 24;
					readbuf += UART3_Buffer_Rece[15] << 16;
					readbuf += UART3_Buffer_Rece[16] << 8;
					readbuf += UART3_Buffer_Rece[17];
					LoadSave.risistence = readbuf;
					
					readbuf = 0;
					readbuf += UART3_Buffer_Rece[18] << 24;
					readbuf += UART3_Buffer_Rece[19] << 16;
					readbuf += UART3_Buffer_Rece[20] << 8;
					readbuf += UART3_Buffer_Rece[21];
					LoadSave.power = readbuf;
					
					Uart3SendBuff(UART3_Buffer_Rece,Uart3RXbuff_len);
					Set_Para();
					Store_set_flash();
					Test_Process();
				}
			}else if(LoadSave.devmode==2){//普通模式
				memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
				lockflag = 1;
				DrawLock(lockflag);
				MODS_10H();
				Para_Set_Comp();
				Set_Para();
			}
		}else if(UART3_Buffer_Rece[1] == 0x06){
			if(LoadSave.devmode==0)//主机模式
			{
				slaveonoffflag--;
			}else if(LoadSave.devmode==1){//从机模式
				if(UART3_Buffer_Rece[0] == LoadSave.slaveNo)
				{
					switchdelay = SWITCH_DELAY;
					mainswitch = UART3_Buffer_Rece[5];
					if(mainswitch==1)
						SwitchLedOn();
					else
						SwitchLedOff();
					
					Uart3SendBuff(UART3_Buffer_Rece,Uart3RXbuff_len);
					Set_Para();
				}
			}else if(LoadSave.devmode==2){
				memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
				lockflag = 1;
				DrawLock(lockflag);
				MODS_06H();
				Para_Set_Comp();
				Set_Para();
			}
		}
		
	}
//	if (UART3_Buffer_Rece[0] == 0x01)
//	{
//		if(UART3_Buffer_Rece[1] == 0x03)
//		{
//			if(LoadSave.devmode==0)//主机模式
//			{
//				crc_result = (UART3_Buffer_Rece[Uart3RXbuff_len-2] << 8) + UART3_Buffer_Rece[Uart3RXbuff_len-1];
//				if(crc_result == Hardware_CRC(UART3_Buffer_Rece,Uart3RXbuff_len-2))
//				{
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[3] << 24;
//					readbuf += UART3_Buffer_Rece[4] << 16;
//					readbuf += UART3_Buffer_Rece[5] << 8;
//					readbuf += UART3_Buffer_Rece[6];
//					SlaveValue.Voltage[UART3_Buffer_Rece[0]-1] = readbuf;
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[7] << 24;
//					readbuf += UART3_Buffer_Rece[8] << 16;
//					readbuf += UART3_Buffer_Rece[9] << 8;
//					readbuf += UART3_Buffer_Rece[10];
//					SlaveValue.Current[UART3_Buffer_Rece[0]-1] = readbuf;
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[11] << 24;
//					readbuf += UART3_Buffer_Rece[12] << 16;
//					readbuf += UART3_Buffer_Rece[13] << 8;
//					readbuf += UART3_Buffer_Rece[14];
//					SlaveValue.Rdata[UART3_Buffer_Rece[0]-1] = readbuf;
//					
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[15] << 24;
//					readbuf += UART3_Buffer_Rece[16] << 16;
//					readbuf += UART3_Buffer_Rece[17] << 8;
//					readbuf += UART3_Buffer_Rece[18];			
//					SlaveValue.Power[UART3_Buffer_Rece[0]-1] = readbuf;
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[19] << 24;
//					readbuf += UART3_Buffer_Rece[20] << 16;
//					readbuf += UART3_Buffer_Rece[21] << 8;
//					readbuf += UART3_Buffer_Rece[22];
//					SlaveValue.vrange[UART3_Buffer_Rece[0]-1] = readbuf;
//					
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[23] << 24;
//					readbuf += UART3_Buffer_Rece[24] << 16;
//					readbuf += UART3_Buffer_Rece[25] << 8;
//					readbuf += UART3_Buffer_Rece[26];			
//					SlaveValue.crange[UART3_Buffer_Rece[0]-1] = readbuf;
//				}
//				
//			}else if(LoadSave.devmode==1){//从机模式
//				crc_result = (UART3_Buffer_Rece[6] << 8) + UART3_Buffer_Rece[7];
//				if(crc_result == Hardware_CRC(UART3_Buffer_Rece,6))
//				{
//					memset((char *)u3sendbuff,0,sizeof(u3sendbuff));
//					u3sendbuff[sendnum++] = UART3_Buffer_Rece[0];
//					u3sendbuff[sendnum++] = UART3_Buffer_Rece[1];;
//					u3sendbuff[sendnum++] = UART3_Buffer_Rece[5]*4;
//					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>24);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>16);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Voltage>>8);
//					u3sendbuff[sendnum++] = (u8)DispValue.Voltage;
//					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>24);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>16);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Current>>8);
//					u3sendbuff[sendnum++] = (u8)DispValue.Current;
//					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>24);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>16);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Rdata>>8);
//					u3sendbuff[sendnum++] = (u8)DispValue.Rdata;
//					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>24);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>16);
//					u3sendbuff[sendnum++] = (u8)(DispValue.Power>>8);
//					u3sendbuff[sendnum++] = (u8)DispValue.Power;
//					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>24);
//					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>16);
//					u3sendbuff[sendnum++] = (u8)(LoadSave.vrange>>8);
//					u3sendbuff[sendnum++] = (u8)LoadSave.vrange;
//					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>24);
//					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>16);
//					u3sendbuff[sendnum++] = (u8)(LoadSave.crange>>8);
//					u3sendbuff[sendnum++] = (u8)LoadSave.crange;
//					u3sendbuff[sendnum++] = Hardware_CRC(u3sendbuff,UART3_Buffer_Rece[5]*4+3)>>8;
//					u3sendbuff[sendnum++] = Hardware_CRC(u3sendbuff,UART3_Buffer_Rece[5]*4+3);
//					Uart3SendBuff(u3sendbuff,sendnum);
//				}
//			}
//		}else if(UART3_Buffer_Rece[1] == 0x10){
//			if(LoadSave.devmode==0)//主机模式
//			{
//				crc_result = (UART3_Buffer_Rece[Uart3RXbuff_len-2] << 8) + UART3_Buffer_Rece[Uart3RXbuff_len-1];
//				if(crc_result == Hardware_CRC(UART3_Buffer_Rece,Uart3RXbuff_len-2))
//				{
//					setslaveflag--;
//				}
//			}else if(LoadSave.devmode==1){//从机模式
//				crc_result = (UART3_Buffer_Rece[Uart3RXbuff_len-2] << 8) + UART3_Buffer_Rece[Uart3RXbuff_len-1];
//				if(crc_result == Hardware_CRC(UART3_Buffer_Rece,Uart3RXbuff_len-2))
//				{
//					LoadSave.mode=UART3_Buffer_Rece[9];
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[10] << 24;
//					readbuf += UART3_Buffer_Rece[11] << 16;
//					readbuf += UART3_Buffer_Rece[12] << 8;
//					readbuf += UART3_Buffer_Rece[13];
//					LoadSave.current = readbuf;
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[14] << 24;
//					readbuf += UART3_Buffer_Rece[15] << 16;
//					readbuf += UART3_Buffer_Rece[16] << 8;
//					readbuf += UART3_Buffer_Rece[17];
//					LoadSave.risistence = readbuf;
//					
//					readbuf = 0;
//					readbuf += UART3_Buffer_Rece[18] << 24;
//					readbuf += UART3_Buffer_Rece[19] << 16;
//					readbuf += UART3_Buffer_Rece[20] << 8;
//					readbuf += UART3_Buffer_Rece[21];
//					LoadSave.power = readbuf;
//					
//					Uart3SendBuff(UART3_Buffer_Rece,Uart3RXbuff_len);
//					Set_Para();
//					Store_set_flash();
//					Test_Process();
//				}
//			}
//		}
//	}
	
}
//-----------------------------CRC检测--------------------------------------------//
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count)    //CRC16
{
	vu16 CRC_Result=0xffff;
	vu8 i;
	if(count==0)
	{
		count=1;
	}
	while(count--)
	{
		CRC_Result^=*p_buffer;
		for(i=0;i<8;i++)
		{
			if(CRC_Result&1)
			{
				CRC_Result>>=1;
				CRC_Result^=0xA001;
			}
			else 
			{
				CRC_Result>>=1; 
			}
		}
		p_buffer++;
	}
	return CRC_Result;
}
