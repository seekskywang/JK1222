#include "pbdata.h"

u8 _4094_databuff[_4094NUM];

void HC595_Send(u8 *buff)
{
    u8 i;
    u8 j;
    //_SPI_595_SHCPL;
	for(j=0;j<HC595HC;j++)
	{
		for(i=0;i<8;i++)     //写八位数据进CD4094;
		{
			if(*(buff+j)&0x80)     //判断并发串行数据;
			{
				_SPI_595_DSH;      //发高电平;
			}
			else
			{
				_SPI_595_DSL;      //发低电平;
			}
			
			_SPI_595_SHCPL;       //形成上升延脉冲;
			delay_us(5);
			_SPI_595_SHCPH;
			*(buff+j)<<=1;      //数据移位,送下一位数据;
			
		}
        
	}
    _SPI_595_CSH;
    _SPI_595_STCPL;       //形成上升延脉冲;
    delay_us(5);
    _SPI_595_STCPH;
    
    _SPI_595_CSL;
    delay_ms(30);


}
void Out_4094(u8 *buff) //写CD4094子函数;
{
	u8 i;       //定义子函数;
	u8 j;
//   _SPI_4094_STROH;
	//_4094_STBL;       //保持然来的状态不变;
//	_SPI_4094_SCLKH;
//    delay_us(20);
	for(j=0;j<_4094NUM;j++)
	{
		for(i=0;i<8;i++)     //写八位数据进CD4094;
		{

			if(*(buff+j)&0x80)     //判断并发串行数据;
			{
				_SPI_4094_DATAH;      //发高电平;
			}
			else
			{
				_SPI_4094_DATAL;      //发低电平;
			}
			
			_SPI_4094_SCLKL;       //形成上升延脉冲;
			delay_us(10);
			_SPI_4094_SCLKH;
			*(buff+j)<<=1;      //数据移位,送下一位数据;
            delay_us(10);
			
		}
	}
//	_SPI_4094_STROL;       //数据送完;更新显示;
//	delay_us(1);
//	_SPI_4094_STROL; 
//    delay_us(100);
//    _SPI_4094_SCLKL;
//     delay_us(100);
//    _SPI_4094_STROH; 
//     delay_us(100);
////    _SPI_4094_SCLKH;
////    
////    delay_us(50);
//    _SPI_4094_STROL; 
//     delay_us(100);
//    _SPI_4094_STROH; 
}
