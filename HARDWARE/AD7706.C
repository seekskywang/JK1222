#include "pbdata.h"
#include <CORE_CMINSTR.h>
/******根据写周期时序，向输入移位寄存器写入指令函数******/
void AD7705_write_Reg(u8 data_byte)
{
	u8 i;
	_7706_CSL;
	delay_us(50);
	_SPI_SCLKH;
	delay_us(50);
	for(i=0;i<8;i++)
		{
			_SPI_SCLKL;
			delay_us(50);
			if(data_byte&0x80)
				_SPI_DINH;
			else 
				_SPI_DINL;
			delay_us(50);
			_SPI_SCLKH;
			
			data_byte<<=1;
			delay_us(50);
			
		}
	_SPI_DINH;	
	delay_us(50);
	_7706_CSH;		
}

/******根据读周期时序,读输出移位寄存器中数据函数******/
u8 AD7705_read_Reg()
{
	u8 i;
	u8 data;
	u8 DATA_OUT;
	//while(DRDY);//等待完成数据的更新  增加读端口 查询
	//while((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)==GPIO_Pin_3));
	//_7706_CSH;
	//delay_us(20);
	//_SPI_SCLKH;
	//delay_us(20);
	_7706_CSL;
	delay_us(20);
	_SPI_SCLKH;
	delay_us(20);
	//P3=0xff;//读数据之前，对P3口进行写1操作
	/*读数据*/
	for(i=0;i<8;i++)
		{
			DATA_OUT<<=1;
			_SPI_SCLKL;
			delay_us(20);
			if(_SPI_DOUT==Bit_SET)
				data=1;
			else
				data=0;
			DATA_OUT=DATA_OUT|data;
			delay_us(20);
			_SPI_SCLKH;
			delay_us(20);
		}
		_SPI_DINH;
	_7706_CSH;
	
	//DRDY=1;
		//GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	return (DATA_OUT);
}
void ad7705_rest(void)
{

	u8 i;
	_SPI_DINH;
	for(i=0;i<38;i++)
	{
		_SPI_SCLKL;
		delay_us(2);
		_SPI_SCLKH;
		delay_us(2);

	}
	delay_ms(1);

}
/******读出结果函数******/
u16 AD7705_read_result(u8 data)
{
	u8 Data_MSB,Data_LSB;
	u8 i=0;
	u16 AD_result;
	AD7705_init(data);
	AD7705_write_Reg(0x38+data);//通信寄存器中，设置为读数据寄存器
	delay_us(100);
	while((GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)==Bit_SET))
	{
		i++;
		delay_ms(1);
		if(i>50)
		{
			ad7705_rest();
			break;
		}
			
	}
	//delay_ms(25);
	/*数据寄存器为16位的只读寄存器，其包含了来自AD7705的最新转换结果*/
	Data_MSB=AD7705_read_Reg();//读出高8位数据
	Data_LSB=AD7705_read_Reg();//读出低8位数据
	AD_result=(u16)Data_MSB;
	AD_result<<=8;
	AD_result=AD_result | (u16)Data_LSB;//位转换为数
	/*转换为电压*/
	if(AD_result==0||AD_result==0xffff)
		ad7705_rest();
	return AD_result;
}
/******配置AD7705函数******/
void AD7705_init(u8 data)//根据具体情况，初始化AD7705  15385742739
{
//	u32 i;
	_SPI_DINH;

	AD7705_write_Reg(0x20+data);//写入通信寄存器;设置为写时钟寄存器
	/*时钟寄存器设置*/
	AD7705_write_Reg(1);//(0x06-2*data);//MCLKIN引脚处的时钟频率在被AD7705使用前，进行2分频;20HZ输出更新率，即每50ms输出一个新字
//	/*设置寄存器设置*/
	AD7705_write_Reg(0x10+data);//写入通信寄存器，设置为写设置寄存器
//	/*设置寄存器设置*/
	AD7705_write_Reg(0x44);//自校准、增益为1、单极性模拟输入、片内缓冲器，FSYNC清0
}
//读通道值  0  通道0  1  通道1
u16 Read_Channel(u8 data )
{
	u16 ad_data;
//	u32 i;
	ad_data=AD7705_read_result(data);
	return ad_data;

}


//void Spi_nop(void)
//{
//	//_nop = 0;
//	DrvSYS_Delay(3);
//}
/* ************************************ */
/*函数名：SpisendByte(INT8U dat)*/
/*输入：发送的数据*/
/*输出：无*/
/*功能描述：SPI发送一个字节*/
/* ************************************ */
void SpiTxByte(u8 dat)
{
	u8 i;
    _SPI_SCLKH;
    delay_us(1);
	for(i=0; i<8; i++)
    {		
        _SPI_SCLKL;
//        delay_us(1);
        if(dat & 0x80)
            _SPI_DINH;
		else
			 _SPI_DINL;
		dat <<= 1;
        delay_us(1);
		_SPI_SCLKH;
        delay_us(1);
		
	}
}
/* ************************************ */
/*函数名：SpisendByte(INT8U dat)*/
/*输入：发送的数据*/
/*输出：无*/
/*功能描述：SPI接收一个字节*/
/* ************************************ */
u8 SpiRxByte(u8 rdry)
{	u8 i,temp;
//	temp = 0;
	if(rdry)
		_SPI_DINH;
	else
		_SPI_DINL;
   //delay_1us(1);
	for(i=0; i<8; i++)
	{		
        _SPI_SCLKL;
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
       // delay_1us(1);

		_SPI_SCLKH;
        temp <<= 1;
		if(_SPI_DOUT)  /* CPU_IN */
			temp++;
       // delay_1us(1);
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        
        __NOP();
        __NOP();
        __NOP();
	}
    _SPI_SCLKL;
	return temp;
    
}
//*command+data*/
void Wr_CmDat(u8 cmd,u8 dat)
{
	SpiTxByte(cmd);
	SpiTxByte(dat);
	}
/* ********************************************************************** */
/* 函数名void AD(void) */
/* 输入:无 */
/* 输出:无 */
/* 功能描述:AD转换结果送 u8 AD0_VALUE[]*/
/* ********************************************************************** */
void INIT_AD_Convert(u8 gpio,u8 pga,u8 ch)
//    void INIT_AD_Convert(void)
	{
        _ADS1216_CS_L;
        delay_us(1);
        SpiTxByte(0xfe);//复位
        _ADS1216_CS_H;
		delay_us(50);//选择ADS1216Y
        _ADS1216_CS_L;
        delay_us(1);
		Wr_CmDat(0x50,00);
		SpiTxByte(0x0c);//内部基准 2.5V
        _ADS1216_CS_H;
        delay_us(1);
        _ADS1216_CS_L;
        delay_us(1);
        
        
        Wr_CmDat(0x57,02);        
		SpiTxByte(0xfc);//端口都置低电平
              
		SpiTxByte(0x80);//采样速度c0

		SpiTxByte(0x07);//03
        _ADS1216_CS_H;
        delay_ms(13);
        
        _ADS1216_CS_L;
        delay_us(1);
        Wr_CmDat(0x51,01);
		SpiTxByte(ch);//通道0

		SpiTxByte(pga);//5

        Wr_CmDat(0x56,00);
		SpiTxByte(gpio);
		_ADS1216_CS_H;
        delay_ms(100);
//		_ADS1216_CS_L;
//		Wr_CmDat(0x19,00);
//		SpiTxByte(0x00);
//        delay_us(1);
//		_ADS1216_CS_H;
}
void ADS1216_Set_IO(u8 port,u8 gain) // 0,1 2
{
    //INIT_AD_Convert();
    _ADS1216_CS_L;
    delay_us(2);
    
    Wr_CmDat(0x56,00);
    delay_us(2);
    SpiTxByte(port);
    delay_us(2);
    _ADS1216_CS_H;
    //delay_ms(20);
    delay_us(2);
    _ADS1216_CS_L;
    
    Wr_CmDat(0x52,00);
    delay_us(2);
    SpiTxByte(gain);
    delay_us(2);
    _ADS1216_CS_H;


}    
void ADS_1216_PGA(u8 gain)
{
    _ADS1216_CS_L;
    delay_us(1);
    Wr_CmDat(0x52,00);
    SpiTxByte(gain);
    _ADS1216_CS_H;
}
void ch_select(u8 ch,u8 gain)
{
  	_ADS1216_CS_L;
    delay_us(1);
    Wr_CmDat(0x56,00);
    SpiTxByte(ch + 0);//08h为通道0(0aH,0cH,0eH)
	_ADS1216_CS_H;
	delay_us(1);
    ADS_1216_PGA(gain);
	
}
/* ********************************************************************** */
/* 函数名void AD(void) */
/* 输入:无 */
/* 输出:无 */
/* 功能描述:AD转换结果送 u8 AD0_VALUE[]*/
/* ********************************************************************** */
//================
void AD_Convert_read(void)
{
    u8  temp;
   // vu8 *pt;
  //  pt=(vu8 *)Ad_value;
    Ad_value=0;
    _ADS1216_CS_L;
    delay_us(1);
    Wr_CmDat(0x19,00);//读M/DEC1,取/DRDY
    delay_us(1);
    temp=SpiRxByte(0);
    //_ADS1216_CS_H;
    temp=temp&0x80;
//    if(temp!=0)
//    {
//        delay_us(1);
//    
//    }
    if(temp==0)
    {
        //_ADS1216_CS_L;
       // delay_us(10);
        SpiTxByte(0x01);
        //delay_us(7);

        
        Ad_value|=SpiRxByte(1);
        Ad_value<<=8;
        Ad_value|=SpiRxByte(1);
        Ad_value<<=8;
        Ad_value|=SpiRxByte(1);
        Ad_value<<=8;
        
//        b=SpiRxByte(1);
//        c=SpiRxByte(1);
//        Ad_value=((u32)c<<24)||((u32)b<<16)||((u32)a<<8);
        Ad_over=1;
	}
    _ADS1216_CS_H;
}
u32 Read_Convert_read(void)
{
    u32 temp=0;
    delay_1us(20);
    temp|=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
    return temp;
    
}

s32 Read__Convert_read(uint8_t num)
{
    s32  temp;
    //vu8 *pt;
    //pt=(vu8 *)Ad_value;
    delay_1us(120);
    temp=0;
    Select_V_I(num);
    temp|=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
    temp<<=8;
    temp|=SpiRxByte(1);
//    temp<<=8;

    return temp;

}
void Ads1251_Int(void)
{
   _SPI_SCLKL; 

}


