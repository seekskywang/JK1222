#include "pbdata.h"
#define _5618C  0x8000
void tlc5618_write(u16 dazhi)	//5618写
{
	u8 i;
	u16 dig;

	_SPI_SCLKH;
	_5618_CSL;
	for(i=0;i<16;i++)        //还不明白
	{
		dig=dazhi&0x8000;
		if(dig==0)
		_SPI_DINL;
		else 
		_SPI_DINH;
		_SPI_SCLKL;
		_SPI_SCLKH;
		dazhi=dazhi<<1;
	}
	_SPI_SCLKH;
	_5618_CSH;	  

}
//0  通道0  1  通道1
void Writer_Channel(u8 data,u16 value)
{
	if(data)
		value|=_5618C;
	tlc5618_write(value);

}
