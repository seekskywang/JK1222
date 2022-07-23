/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 


#include "./usart/bsp_debug_usart.h"
#include "pbdata.h"
vu8 sendbuff[200];
u8 setflag;
vu16 Hardware_CRC(vu8 *p_buffer,vu8 count);
//u32 dacvalue[10] = {1000,7000,1000,2000,3000,20000,50000,10000,20000,30000};
u32 dacvalue[17] = {
10000,30000,//��ѹ�͵�λ
3000,10000,20000,30000,40000,50000,//��ѹ�ߵ�λ
5000,10000,30000,50000,//�����͵�λ
10000,20000,30000,40000,50000};//�����ߵ�λ
u32 vsum,isum,rsum,psum;
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  DEBUG_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void Debug_USART_Config(u32 baud)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd( DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE, DEBUG_USART_RX_AF);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
			
  /* ���ô�DEBUG_USART ģʽ */
  USART_InitStructure.USART_BaudRate = baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
  NVIC_Configuration();
  
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
  
   USART_Cmd(DEBUG_USART, ENABLE);
}

///�ض���c�⺯��printf������DEBUG_USART���ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�����DEBUG_USART */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf������DEBUG_USART����д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}
/*********************************************END OF FILE**********************/
/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	
}
/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!=0xbf);
  Usart_SendByte( pUSARTx, 0xbf );
  
  /* �ȴ�������� */
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
  /* �ȴ�������� */
  while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET)
  {}
}


void Send_Uart3str(char *str)
{
    Usart_SendString( USART3, str);


}
//У׼
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
	Usart1_Send((char *)sendbuff,9);
}

//����DAC
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
	Usart1_Send((char *)sendbuff,9);

}
//���õ�ַ
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//����
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//ģʽ����
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//�б�ģʽ����
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//���ģʽ����
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//������λ
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//��ѹ��λ
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//Զ�˲�������
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//������־��λ
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//���ò�����
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//����ͨѶ��ʽ
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
	Usart1_Send((char *)sendbuff,10);
	setflag = 1;
}

//���ò���
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
	sendbuff[10] = mainswitch;//����
	
	sendbuff[15] = 0x00;
	sendbuff[16] = 0x00;
	sendbuff[17] = 0x00;
	sendbuff[18] = LoadSave.crange;//������λ
	
	sendbuff[19] = 0x00;
	sendbuff[20] = 0x00;
	sendbuff[21] = 0x00;
	sendbuff[22] = LoadSave.vrange;//��ѹ��λ
	
	sendbuff[23] = 0x00;
	sendbuff[24] = 0x00;
	sendbuff[25] = 0x00;
	sendbuff[26] = LoadSave.Addr;//��ַ
	
	sendbuff[27] = 0x00;
	sendbuff[28] = 0x00;
	sendbuff[29] = 0x00;
	sendbuff[30] = LoadSave.Baudrate;//������
	
	sendbuff[31] = 0x00;
	sendbuff[32] = 0x00;
	sendbuff[33] = 0x00;
	sendbuff[34] = LoadSave.sence;//Զ�˲���
	
	if(GetSystemStatus()==SYS_STATUS_LIST)//�б�ģʽ���ò���
	{
		
		
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		if(LoadSave.listmode[DispValue.listrunstep] == 4)
		{
			sendbuff[14] = 6;
		}else{
			sendbuff[14] = LoadSave.listmode[DispValue.listrunstep];//ģʽ
		}
		
		if(LoadSave.listmode[DispValue.listrunstep] == 1)
		{
			if(LoadSave.vrange == 1)
			{
				sendbuff[7+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[8+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[9+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[10+28] = LoadSave.listvalue[DispValue.listrunstep];//���õ�ѹֵ
			}else{
				sendbuff[7+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[8+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[9+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[10+28] = LoadSave.listvalue[DispValue.listrunstep];//���õ�ѹֵ
			}
		}else if(LoadSave.listmode[DispValue.listrunstep] == 0){
			if(LoadSave.crange == 1)
			{
				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep];//���õ���ֵ
//				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep]/10;//���õ���ֵ
			}else{
				sendbuff[11+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
				sendbuff[12+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
				sendbuff[13+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
				sendbuff[14+28] = LoadSave.listvalue[DispValue.listrunstep];//���õ���ֵ
			}
		}else if(LoadSave.listmode[DispValue.listrunstep] == 2){
			sendbuff[15+28] = LoadSave.listvalue[DispValue.listrunstep]>>24;
			sendbuff[16+28] = LoadSave.listvalue[DispValue.listrunstep]>>16;
			sendbuff[17+28] = LoadSave.listvalue[DispValue.listrunstep]>>8;
			sendbuff[18+28] = LoadSave.listvalue[DispValue.listrunstep];//���õ���ֵ
		}else if(LoadSave.listmode[DispValue.listrunstep] == 3){
			sendbuff[19+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
			sendbuff[20+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
			sendbuff[21+28] = LoadSave.listvalue[DispValue.listrunstep]/10>>8; 
			sendbuff[22+28] = LoadSave.listvalue[DispValue.listrunstep]/10;//���ù���ֵ
		}
	}else if(GetSystemStatus()==SYS_STATUS_TEST){//����ģʽ���ò���
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = LoadSave.mode;//ģʽ
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//���õ�ѹֵ
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
		}
		
		if(LoadSave.crange == 1)
		{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//���õ���ֵ
//			sendbuff[11+28] = LoadSave.current/10>>24;
//			sendbuff[12+28] = LoadSave.current/10>>16;
//			sendbuff[13+28] = LoadSave.current/10>>8;
//			sendbuff[14+28] = LoadSave.current/10;//���õ���ֵ
		}else{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//���õ���ֵ
		}
		
		
		sendbuff[15+28] = LoadSave.risistence>>24;
		sendbuff[16+28] = LoadSave.risistence>>16;
		sendbuff[17+28] = LoadSave.risistence>>8;
		sendbuff[18+28] = LoadSave.risistence;//���õ���ֵ
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//���ù���ֵ
	}else if(GetSystemStatus()==SYS_STATUS_BATTERY){//���ģʽ���ò���
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = LoadSave.loadmode;//��ز���ģʽ
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//���õ�ѹֵ
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
		}
		
		if(LoadSave.crange == 1)
		{
			if(batstep == 2)
			{
				sendbuff[11+28] = LoadSave.loadc2>>24;
				sendbuff[12+28] = LoadSave.loadc2>>16;
				sendbuff[13+28] = LoadSave.loadc2>>8;
				sendbuff[14+28] = LoadSave.loadc2;//���õ���ֵ
//				sendbuff[11+28] = LoadSave.loadc2/10>>24;
//				sendbuff[12+28] = LoadSave.loadc2/10>>16;
//				sendbuff[13+28] = LoadSave.loadc2/10>>8;
//				sendbuff[14+28] = LoadSave.loadc2/10;//���õ���ֵ
			}else if(batstep == 3){
				sendbuff[11+28] = LoadSave.loadc3>>24;
				sendbuff[12+28] = LoadSave.loadc3>>16;
				sendbuff[13+28] = LoadSave.loadc3>>8;
				sendbuff[14+28] = LoadSave.loadc3;//���õ���ֵ
//				sendbuff[11+28] = LoadSave.loadc3/10>>24;
//				sendbuff[12+28] = LoadSave.loadc3/10>>16;
//				sendbuff[13+28] = LoadSave.loadc3/10>>8;
//				sendbuff[14+28] = LoadSave.loadc3/10;//���õ���ֵ
			}else{
				sendbuff[11+28] = LoadSave.loadc1>>24;
				sendbuff[12+28] = LoadSave.loadc1>>16;
				sendbuff[13+28] = LoadSave.loadc1>>8;
				sendbuff[14+28] = LoadSave.loadc1;//���õ���ֵ
//				sendbuff[11+28] = LoadSave.loadc1/10>>24;
//				sendbuff[12+28] = LoadSave.loadc1/10>>16;
//				sendbuff[13+28] = LoadSave.loadc1/10>>8;
//				sendbuff[14+28] = LoadSave.loadc1/10;//���õ���ֵ
			}
		}else{
			if(batstep == 2)
			{
				sendbuff[11+28] = LoadSave.loadc2>>24;
				sendbuff[12+28] = LoadSave.loadc2>>16;
				sendbuff[13+28] = LoadSave.loadc2>>8;
				sendbuff[14+28] = LoadSave.loadc2;//���õ���ֵ
			}else if(batstep == 3){
				sendbuff[11+28] = LoadSave.loadc3>>24;
				sendbuff[12+28] = LoadSave.loadc3>>16;
				sendbuff[13+28] = LoadSave.loadc3>>8;
				sendbuff[14+28] = LoadSave.loadc3;//���õ���ֵ
			}else{
				sendbuff[11+28] = LoadSave.loadc1>>24;
				sendbuff[12+28] = LoadSave.loadc1>>16;
				sendbuff[13+28] = LoadSave.loadc1>>8;
				sendbuff[14+28] = LoadSave.loadc1;//���õ���ֵ
			}
		}
		
		
		sendbuff[15+28] = LoadSave.loadr>>24;
		sendbuff[16+28] = LoadSave.loadr>>16;
		sendbuff[17+28] = LoadSave.loadr>>8;
		sendbuff[18+28] = LoadSave.loadr;//���õ���ֵ
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//���ù���ֵ
	}else if(GetSystemStatus()==SYS_STATUS_DYNAMIC){//��̬ģʽ���ò���
		sendbuff[11] = 0x00;
		sendbuff[12] = 0x00;
		sendbuff[13] = 0x00;
		sendbuff[14] = 4;//ģʽ
		
		
		if(LoadSave.vrange == 1)
		{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
//			sendbuff[7+28] = LoadSave.voltage/10>>24;
//			sendbuff[8+28] = LoadSave.voltage/10>>16;
//			sendbuff[9+28] = LoadSave.voltage/10>>8;
//			sendbuff[10+28] = LoadSave.voltage/10;//���õ�ѹֵ
		}else{
			sendbuff[7+28] = LoadSave.voltage>>24;
			sendbuff[8+28] = LoadSave.voltage>>16;
			sendbuff[9+28] = LoadSave.voltage>>8;
			sendbuff[10+28] = LoadSave.voltage;//���õ�ѹֵ
		}
		
		if(LoadSave.crange == 1)
		{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//���õ���ֵ
//			sendbuff[11+28] = LoadSave.current/10>>24;
//			sendbuff[12+28] = LoadSave.current/10>>16;
//			sendbuff[13+28] = LoadSave.current/10>>8;
//			sendbuff[14+28] = LoadSave.current/10;//���õ���ֵ
		}else{
			sendbuff[11+28] = LoadSave.current>>24;
			sendbuff[12+28] = LoadSave.current>>16;
			sendbuff[13+28] = LoadSave.current>>8;
			sendbuff[14+28] = LoadSave.current;//���õ���ֵ
		}
		
		
		sendbuff[15+28] = LoadSave.risistence>>24;
		sendbuff[16+28] = LoadSave.risistence>>16;
		sendbuff[17+28] = LoadSave.risistence>>8;
		sendbuff[18+28] = LoadSave.risistence;//���õ���ֵ
		
		sendbuff[19+28] = LoadSave.power/10>>24;
		sendbuff[20+28] = LoadSave.power/10>>16;
		sendbuff[21+28] = LoadSave.power/10>>8; 
		sendbuff[22+28] = LoadSave.power/10;//���ù���ֵ
	}
	
	sendbuff[23+28] = LoadSave.ovp>>24;
	sendbuff[24+28] = LoadSave.ovp>>16;
	sendbuff[25+28] = LoadSave.ovp>>8;
	sendbuff[26+28] = LoadSave.ovp;//����ѹ����
	
	sendbuff[27+28] = LoadSave.ocp>>24;
	sendbuff[28+28] = LoadSave.ocp>>16;
	sendbuff[29+28] = LoadSave.ocp>>8;
	sendbuff[30+28] = LoadSave.ocp;//����������
	
	sendbuff[31+28] = LoadSave.opp>>24;
	sendbuff[32+28] = LoadSave.opp>>16;
	sendbuff[33+28] = LoadSave.opp>>8;
	sendbuff[34+28] = LoadSave.opp;//�����ʱ���
	
	sendbuff[35+28] = LoadSave.maxp/10>>24;
	sendbuff[36+28] = LoadSave.maxp/10>>16;
	sendbuff[37+28] = LoadSave.maxp/10>>8;
	sendbuff[38+28] = LoadSave.maxp/10;//������ƹ���
	
	sendbuff[39+28] = LoadSave.maxv/10>>24;
	sendbuff[40+28] = LoadSave.maxv/10>>16;
	sendbuff[41+28] = LoadSave.maxv/10>>8;
	sendbuff[42+28] = LoadSave.maxv/10;//������Ƶ�ѹ
	
	sendbuff[43+28] = LoadSave.maxc/10>>24;
	sendbuff[44+28] = LoadSave.maxc/10>>16;
	sendbuff[45+28] = LoadSave.maxc/10>>8;
	sendbuff[46+28] = LoadSave.maxc/10;//������Ƶ���
	
	if(GetSystemStatus()==SYS_STATUS_LIST)
	{
		sendbuff[47+28] = LoadSave.listonvol/10>>24;
		sendbuff[48+28] = LoadSave.listonvol/10>>16;
		sendbuff[49+28] = LoadSave.listonvol/10>>8;
		sendbuff[50+28] = LoadSave.listonvol/10;//���ص�ѹ
	}else{
		sendbuff[47+28] = LoadSave.onvol/10>>24;
		sendbuff[48+28] = LoadSave.onvol/10>>16;
		sendbuff[49+28] = LoadSave.onvol/10>>8;
		sendbuff[50+28] = LoadSave.onvol/10;//���ص�ѹ
	}
	
	sendbuff[51+28] = LoadSave.offvol/10>>24;
	sendbuff[52+28] = LoadSave.offvol/10>>16;
	sendbuff[53+28] = LoadSave.offvol/10>>8;
	sendbuff[54+28] = LoadSave.offvol/10;//ж�ص�ѹ
	
	if(LoadSave.crange == 1)
	{
		sendbuff[55+28] = LoadSave.crise>>24;
		sendbuff[56+28] = LoadSave.crise>>16;
		sendbuff[57+28] = LoadSave.crise>>8;
		sendbuff[58+28] = LoadSave.crise;//����������
		
		sendbuff[59+28] = LoadSave.cdrop>>24;
		sendbuff[60+28] = LoadSave.cdrop>>16;
		sendbuff[61+28] = LoadSave.cdrop>>8;
		sendbuff[62+28] = LoadSave.cdrop;//�����½���
		
//		sendbuff[55+28] = LoadSave.crise/10>>24;
//		sendbuff[56+28] = LoadSave.crise/10>>16;
//		sendbuff[57+28] = LoadSave.crise/10>>8;
//		sendbuff[58+28] = LoadSave.crise/10;//����������
//		
//		sendbuff[59+28] = LoadSave.cdrop/10>>24;
//		sendbuff[60+28] = LoadSave.cdrop/10>>16;
//		sendbuff[61+28] = LoadSave.cdrop/10>>8;
//		sendbuff[62+28] = LoadSave.cdrop/10;//�����½���
	}else if(LoadSave.crange == 0){
		sendbuff[55+28] = LoadSave.crise>>24;
		sendbuff[56+28] = LoadSave.crise>>16;
		sendbuff[57+28] = LoadSave.crise>>8;
		sendbuff[58+28] = LoadSave.crise;//����������
		
		sendbuff[59+28] = LoadSave.cdrop>>24;
		sendbuff[60+28] = LoadSave.cdrop>>16;
		sendbuff[61+28] = LoadSave.cdrop>>8;
		sendbuff[62+28] = LoadSave.cdrop;//�����½���
	}
	
	if(LoadSave.vrange == 1)
	{
		sendbuff[63+28] = LoadSave.cvdowntime>>24;
		sendbuff[64+28] = LoadSave.cvdowntime>>16;
		sendbuff[65+28] = LoadSave.cvdowntime>>8;
		sendbuff[66+28] = LoadSave.cvdowntime;//CVģʽ��ѹ�½�ʱ��
//		sendbuff[63+28] = LoadSave.cvdowntime/10>>24;
//		sendbuff[64+28] = LoadSave.cvdowntime/10>>16;
//		sendbuff[65+28] = LoadSave.cvdowntime/10>>8;
//		sendbuff[66+28] = LoadSave.cvdowntime/10;//CVģʽ��ѹ�½�ʱ��
	}else if(LoadSave.vrange == 0){
		sendbuff[63+28] = LoadSave.cvdowntime>>24;
		sendbuff[64+28] = LoadSave.cvdowntime>>16;
		sendbuff[65+28] = LoadSave.cvdowntime>>8;
		sendbuff[66+28] = LoadSave.cvdowntime;//CVģʽ��ѹ�½�ʱ��
	}
	
	
	sendbuff[67+28] = LoadSave.ledvo>>24;
	sendbuff[68+28] = LoadSave.ledvo>>16;
	sendbuff[69+28] = LoadSave.ledvo>>8;
	sendbuff[70+28] = LoadSave.ledvo;//LEDģʽ˳������ѹ
	
	sendbuff[71+28] = LoadSave.ledio>>24;
	sendbuff[72+28] = LoadSave.ledio>>16;
	sendbuff[73+28] = LoadSave.ledio>>8;
	sendbuff[74+28] = LoadSave.ledio;//LEDģʽ˳��������
	
	sendbuff[75+28] = LoadSave.ledrd>>24;
	sendbuff[76+28] = LoadSave.ledrd>>16;
	sendbuff[77+28] = LoadSave.ledrd>>8;
	sendbuff[78+28] = LoadSave.ledrd;//LEDģʽRDϵ�����˲���������������ص�ѹ��ֵ��ΧΪ0-100%�����磺����VOΪ30V RD-0.2  ��ʱ���ص�ѹӦΪ30*0.2=6��VI=30-6=24V��
	
	if(LoadSave.crange == 1)
	{
		sendbuff[79+28] = LoadSave.valA>>24;
		sendbuff[80+28] = LoadSave.valA>>16;
		sendbuff[81+28] = LoadSave.valA>>8;
		sendbuff[82+28] = LoadSave.valA;//��̬ģʽ���ص���A
		
		sendbuff[83+28] = LoadSave.valB>>24;
		sendbuff[84+28] = LoadSave.valB>>16;
		sendbuff[85+28] = LoadSave.valB>>8;
		sendbuff[86+28] = LoadSave.valB;//��̬ģʽ���ص���B
		
//		sendbuff[79+28] = LoadSave.valA/10>>24;
//		sendbuff[80+28] = LoadSave.valA/10>>16;
//		sendbuff[81+28] = LoadSave.valA/10>>8;
//		sendbuff[82+28] = LoadSave.valA/10;//��̬ģʽ���ص���A
//		
//		sendbuff[83+28] = LoadSave.valB/10>>24;
//		sendbuff[84+28] = LoadSave.valB/10>>16;
//		sendbuff[85+28] = LoadSave.valB/10>>8;
//		sendbuff[86+28] = LoadSave.valB/10;//��̬ģʽ���ص���B
	}else if(LoadSave.crange == 0){
		sendbuff[79+28] = LoadSave.valA>>24;
		sendbuff[80+28] = LoadSave.valA>>16;
		sendbuff[81+28] = LoadSave.valA>>8;
		sendbuff[82+28] = LoadSave.valA;//��̬ģʽ���ص���A
		
		sendbuff[83+28] = LoadSave.valB>>24;
		sendbuff[84+28] = LoadSave.valB>>16;
		sendbuff[85+28] = LoadSave.valB>>8;
		sendbuff[86+28] = LoadSave.valB;//��̬ģʽ���ص���B
	}
	
	sendbuff[87+28] = LoadSave.timeA>>24;
	sendbuff[88+28] = LoadSave.timeA>>16;
	sendbuff[89+28] = LoadSave.timeA>>8;
	sendbuff[90+28] = LoadSave.timeA;//��̬ģʽ���س���ʱ��A
	
	sendbuff[91+28] = LoadSave.timeB>>24;
	sendbuff[92+28] = LoadSave.timeB>>16;
	sendbuff[93+28] = LoadSave.timeB>>8;
	sendbuff[94+28] = LoadSave.timeB;//��̬ģʽ���س���ʱ��B
	
	sendbuff[95+28] = LoadSave.dynaIRise>>24;
	sendbuff[96+28] = LoadSave.dynaIRise>>16;
	sendbuff[97+28] = LoadSave.dynaIRise>>8;
	sendbuff[98+28] = LoadSave.dynaIRise;//��̬ģʽ����������
	
	sendbuff[99+28] = LoadSave.dynaIDrop>>24;
	sendbuff[100+28] = LoadSave.dynaIDrop>>16;
	sendbuff[101+28] = LoadSave.dynaIDrop>>8;
	sendbuff[102+28] = LoadSave.dynaIDrop;//��̬ģʽ�����½���
	
	sendbuff[103+28] = LoadSave.dynamode>>24;
	sendbuff[104+28] = LoadSave.dynamode>>16;
	sendbuff[105+28] = LoadSave.dynamode>>8;
	sendbuff[106+28] = LoadSave.dynamode;//��̬����ģʽ
	
	sendbuff[107+28] = LoadSave.COMM>>24;
	sendbuff[108+28] = LoadSave.COMM>>16;
	sendbuff[109+28] = LoadSave.COMM>>8;
	sendbuff[110+28] = LoadSave.COMM;//ͨѶѡ��
	
	sendbuff[111+28] = LoadSave.Class_5>>24;
	sendbuff[112+28] = LoadSave.Class_5>>16;
	sendbuff[113+28] = LoadSave.Class_5>>8;
	sendbuff[114+28] = LoadSave.Class_5;//���ģʽѡ�񿪹�
	
	sendbuff[115+28] = LoadSave.Class_9>>24;
	sendbuff[116+28] = LoadSave.Class_9>>16;
	sendbuff[117+28] = LoadSave.Class_9>>8;
	sendbuff[118+28] = LoadSave.Class_9;//���ģʽѡ�񿪹�
	
	sendbuff[119+28] = LoadSave.Class_12>>24;
	sendbuff[120+28] = LoadSave.Class_12>>16;
	sendbuff[121+28] = LoadSave.Class_12>>8;
	sendbuff[122+28] = LoadSave.Class_12;//���ģʽѡ�񿪹�
	
	sendbuff[123+28] = LoadSave.Class_15>>24;
	sendbuff[124+28] = LoadSave.Class_15>>16;
	sendbuff[125+28] = LoadSave.Class_15>>8; 
	sendbuff[126+28] = LoadSave.Class_15;//���ģʽѡ�񿪹�
	
	sendbuff[127+28] = LoadSave.Class_20>>24;
	sendbuff[128+28] = LoadSave.Class_20>>16;
	sendbuff[129+28] = LoadSave.Class_20>>8;
	sendbuff[130+28] = LoadSave.Class_20;//���ģʽѡ�񿪹�
	sendbuff[131+28] = Hardware_CRC(sendbuff,131+28)>>8;
	sendbuff[132+28] = Hardware_CRC(sendbuff,131+28);
	Usart1_Send((char *)sendbuff,133+28);
	setflag = 1;
}

////���ò���
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
//	if(GetSystemStatus()==SYS_STATUS_LIST)//�б�ģʽ���ò���
//	{
//		if(LoadSave.listmode[DispValue.listrunstep] == 1)
//		{
//			if(LoadSave.vrange == 1)
//			{
//				sendbuff[7] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[8] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[9] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[10] = LoadSave.listvalue[DispValue.listrunstep]/10;//���õ�ѹֵ
//			}else{
//				sendbuff[7] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//				sendbuff[8] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//				sendbuff[9] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//				sendbuff[10] = LoadSave.listvalue[DispValue.listrunstep];//���õ�ѹֵ
//			}
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 0){
//			if(LoadSave.crange == 1)
//			{
//				sendbuff[11] = LoadSave.listvalue[DispValue.listrunstep]/10>>24;
//				sendbuff[12] = LoadSave.listvalue[DispValue.listrunstep]/10>>16;
//				sendbuff[13] = LoadSave.listvalue[DispValue.listrunstep]/10>>8;
//				sendbuff[14] = LoadSave.listvalue[DispValue.listrunstep]/10;//���õ���ֵ
//			}else{
//				sendbuff[11] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//				sendbuff[12] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//				sendbuff[13] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//				sendbuff[14] = LoadSave.listvalue[DispValue.listrunstep];//���õ���ֵ
//			}
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 2){
//			sendbuff[15] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//			sendbuff[16] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//			sendbuff[17] = LoadSave.listvalue[DispValue.listrunstep]>>8;
//			sendbuff[18] = LoadSave.listvalue[DispValue.listrunstep];//���õ���ֵ
//		}else if(LoadSave.listmode[DispValue.listrunstep] == 3){
//			sendbuff[19] = LoadSave.listvalue[DispValue.listrunstep]>>24;
//			sendbuff[20] = LoadSave.listvalue[DispValue.listrunstep]>>16;
//			sendbuff[21] = LoadSave.listvalue[DispValue.listrunstep]>>8; 
//			sendbuff[22] = LoadSave.listvalue[DispValue.listrunstep];//���ù���ֵ
//		}
//	}else if(GetSystemStatus()==SYS_STATUS_TEST){//����ģʽ���ò���
//		if(LoadSave.vrange == 1)
//		{
//			sendbuff[7] = LoadSave.voltage/10>>24;
//			sendbuff[8] = LoadSave.voltage/10>>16;
//			sendbuff[9] = LoadSave.voltage/10>>8;
//			sendbuff[10] = LoadSave.voltage/10;//���õ�ѹֵ
//		}else{
//			sendbuff[7] = LoadSave.voltage>>24;
//			sendbuff[8] = LoadSave.voltage>>16;
//			sendbuff[9] = LoadSave.voltage>>8;
//			sendbuff[10] = LoadSave.voltage;//���õ�ѹֵ
//		}
//		
//		if(LoadSave.crange == 1)
//		{
//			sendbuff[11] = LoadSave.current/10>>24;
//			sendbuff[12] = LoadSave.current/10>>16;
//			sendbuff[13] = LoadSave.current/10>>8;
//			sendbuff[14] = LoadSave.current/10;//���õ���ֵ
//		}else{
//			sendbuff[11] = LoadSave.current>>24;
//			sendbuff[12] = LoadSave.current>>16;
//			sendbuff[13] = LoadSave.current>>8;
//			sendbuff[14] = LoadSave.current;//���õ���ֵ
//		}
//		
//		
//		sendbuff[15] = LoadSave.risistence>>24;
//		sendbuff[16] = LoadSave.risistence>>16;
//		sendbuff[17] = LoadSave.risistence>>8;
//		sendbuff[18] = LoadSave.risistence;//���õ���ֵ
//		
//		sendbuff[19] = LoadSave.power>>24;
//		sendbuff[20] = LoadSave.power>>16;
//		sendbuff[21] = LoadSave.power>>8; 
//		sendbuff[22] = LoadSave.power;//���ù���ֵ
//	}
//	
//	sendbuff[23] = LoadSave.ovp>>24;
//	sendbuff[24] = LoadSave.ovp>>16;
//	sendbuff[25] = LoadSave.ovp>>8;
//	sendbuff[26] = LoadSave.ovp;//����ѹ����
//	
//	sendbuff[27] = LoadSave.ocp>>24;
//	sendbuff[28] = LoadSave.ocp>>16;
//	sendbuff[29] = LoadSave.ocp>>8;
//	sendbuff[30] = LoadSave.ocp;//����������
//	
//	sendbuff[31] = LoadSave.opp>>24;
//	sendbuff[32] = LoadSave.opp>>16;
//	sendbuff[33] = LoadSave.opp>>8;
//	sendbuff[34] = LoadSave.opp;//�����ʱ���
//	
//	sendbuff[35] = LoadSave.maxp>>24;
//	sendbuff[36] = LoadSave.maxp>>16;
//	sendbuff[37] = LoadSave.maxp>>8;
//	sendbuff[38] = LoadSave.maxp;//������ƹ���
//	
//	sendbuff[39] = LoadSave.maxv>>24;
//	sendbuff[40] = LoadSave.maxv>>16;
//	sendbuff[41] = LoadSave.maxv>>8;
//	sendbuff[42] = LoadSave.maxv;//������Ƶ�ѹ
//	
//	sendbuff[43] = LoadSave.maxc>>24;
//	sendbuff[44] = LoadSave.maxc>>16;
//	sendbuff[45] = LoadSave.maxc>>8;
//	sendbuff[46] = LoadSave.maxc;//������Ƶ���
//	
//	sendbuff[47] = LoadSave.onvol>>24;
//	sendbuff[48] = LoadSave.onvol>>16;
//	sendbuff[49] = LoadSave.onvol>>8;
//	sendbuff[50] = LoadSave.onvol;//���ص�ѹ
//	
//	sendbuff[51] = LoadSave.offvol>>24;
//	sendbuff[52] = LoadSave.offvol>>16;
//	sendbuff[53] = LoadSave.offvol>>8;
//	sendbuff[54] = LoadSave.offvol;//ж�ص�ѹ
//	
//	sendbuff[55] = LoadSave.crise>>24;
//	sendbuff[56] = LoadSave.crise>>16;
//	sendbuff[57] = LoadSave.crise>>8;
//	sendbuff[58] = LoadSave.crise;//����������
//	
//	sendbuff[59] = LoadSave.cdrop>>24;
//	sendbuff[60] = LoadSave.cdrop>>16;
//	sendbuff[61] = LoadSave.cdrop>>8;
//	sendbuff[62] = LoadSave.cdrop;//�����½���
//	
//	sendbuff[63] = LoadSave.cvdowntime>>24;
//	sendbuff[64] = LoadSave.cvdowntime>>16;
//	sendbuff[65] = LoadSave.cvdowntime>>8;
//	sendbuff[66] = LoadSave.cvdowntime;//CVģʽ��ѹ�½�ʱ��
//	
//	sendbuff[67] = LoadSave.ledvo>>24;
//	sendbuff[68] = LoadSave.ledvo>>16;
//	sendbuff[69] = LoadSave.ledvo>>8;
//	sendbuff[70] = LoadSave.ledvo;//LEDģʽ˳������ѹ
//	
//	sendbuff[71] = LoadSave.ledio>>24;
//	sendbuff[72] = LoadSave.ledio>>16;
//	sendbuff[73] = LoadSave.ledio>>8;
//	sendbuff[74] = LoadSave.ledio;//LEDģʽ˳��������
//	
//	sendbuff[75] = LoadSave.ledrd>>24;
//	sendbuff[76] = LoadSave.ledrd>>16;
//	sendbuff[77] = LoadSave.ledrd>>8;
//	sendbuff[78] = LoadSave.ledrd;//LEDģʽRDϵ�����˲���������������ص�ѹ��ֵ��ΧΪ0-100%�����磺����VOΪ30V RD-0.2  ��ʱ���ص�ѹӦΪ30*0.2=6��VI=30-6=24V��
//	
//	sendbuff[79] = LoadSave.valA>>24;
//	sendbuff[80] = LoadSave.valA>>16;
//	sendbuff[81] = LoadSave.valA>>8;
//	sendbuff[82] = LoadSave.valA;//��̬ģʽ���ص���A
//	
//	sendbuff[83] = LoadSave.valB>>24;
//	sendbuff[84] = LoadSave.valB>>16;
//	sendbuff[85] = LoadSave.valB>>8;
//	sendbuff[86] = LoadSave.valB;//��̬ģʽ���ص���B
//	
//	sendbuff[87] = LoadSave.timeA>>24;
//	sendbuff[88] = LoadSave.timeA>>16;
//	sendbuff[89] = LoadSave.timeA>>8;
//	sendbuff[90] = LoadSave.timeA;//��̬ģʽ���س���ʱ��A
//	
//	sendbuff[91] = LoadSave.timeB>>24;
//	sendbuff[92] = LoadSave.timeB>>16;
//	sendbuff[93] = LoadSave.timeB>>8;
//	sendbuff[94] = LoadSave.timeB;//��̬ģʽ���س���ʱ��B
//	
//	sendbuff[95] = LoadSave.dynaIRise>>24;
//	sendbuff[96] = LoadSave.dynaIRise>>16;
//	sendbuff[97] = LoadSave.dynaIRise>>8;
//	sendbuff[98] = LoadSave.dynaIRise;//��̬ģʽ����������
//	
//	sendbuff[99] = LoadSave.dynaIDrop>>24;
//	sendbuff[100] = LoadSave.dynaIDrop>>16;
//	sendbuff[101] = LoadSave.dynaIDrop>>8;
//	sendbuff[102] = LoadSave.dynaIDrop;//��̬ģʽ�����½���
//	
//	sendbuff[103] = LoadSave.dynamode>>24;
//	sendbuff[104] = LoadSave.dynamode>>16;
//	sendbuff[105] = LoadSave.dynamode>>8;
//	sendbuff[106] = LoadSave.dynamode;//��̬����ģʽ
//	
//	sendbuff[105] = LoadSave.qcmode>>24;
//	sendbuff[106] = LoadSave.qcmode>>16;
//	sendbuff[107] = LoadSave.qcmode>>8;
//	sendbuff[108] = LoadSave.qcmode;//���ģʽѡ�񿪹�
//	
//	sendbuff[109] = LoadSave.Class_5>>24;
//	sendbuff[110] = LoadSave.Class_5>>16;
//	sendbuff[111] = LoadSave.Class_5>>8;
//	sendbuff[112] = LoadSave.Class_5;//���ģʽѡ�񿪹�
//	
//	sendbuff[115] = LoadSave.Class_9>>24;
//	sendbuff[116] = LoadSave.Class_9>>16;
//	sendbuff[117] = LoadSave.Class_9>>8;
//	sendbuff[118] = LoadSave.Class_9;//���ģʽѡ�񿪹�
//	
//	sendbuff[119] = LoadSave.Class_12>>24;
//	sendbuff[120] = LoadSave.Class_12>>16;
//	sendbuff[121] = LoadSave.Class_12>>8;
//	sendbuff[122] = LoadSave.Class_12;//���ģʽѡ�񿪹�
//	
//	sendbuff[123] = LoadSave.Class_15>>24;
//	sendbuff[124] = LoadSave.Class_15>>16;
//	sendbuff[125] = LoadSave.Class_15>>8; 
//	sendbuff[126] = LoadSave.Class_15;//���ģʽѡ�񿪹�
//	
//	sendbuff[127] = LoadSave.Class_20>>24;
//	sendbuff[128] = LoadSave.Class_20>>16;
//	sendbuff[129] = LoadSave.Class_20>>8;
//	sendbuff[130] = LoadSave.Class_20;//���ģʽѡ�񿪹�
//	sendbuff[131] = Hardware_CRC(sendbuff,131)>>8;
//	sendbuff[132] = Hardware_CRC(sendbuff,131);
//	Usart1_Send((char *)sendbuff,133);
//	setflag = 1;
//}

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
	
	Usart1_Send((char *)sendbuff,8);
}

void Rec_Handle(void)	
{
	static u8 sumcount;
	vu16 crc_result;
	u32 readbuf;
	if (UART_Buffer_Rece[0] == 0x01)
	{
		if(UART_Buffer_Rece[1] == 0x03)
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
		}else if(UART_Buffer_Rece[1] == 0x06){
			setflag = 0;
		}else if(UART_Buffer_Rece[1] == 0x10){
			setflag = 0;
		}
	}
}
//-----------------------------CRC���--------------------------------------------//
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