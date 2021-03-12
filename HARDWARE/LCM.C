#include "pbdata.h"
#include "hzk.h"
//==========================================================
//全局变量
LcdAddr_TypeDef LcdAddr;//LCD坐标结构体
//写寄存器函数
//regval:寄存器值

//==========================================================
//函数名称：Lcd_ReadBF
//函数功能：读取液晶的状态位直至BF标志有效
//入口参数：无
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 11:18
//备注说明：读满200次后自动退出
//			D1:读写允许位，为1才可操作
//			D0:BF位，为1才可操作
//==========================================================
void Lcd_ReadBF(void)
{
	register u16 i;
	register u8 dat;

	i=2000;
	while(i>0)
	{
		dat=LCD_RAM;
		if((dat&((u8)(SetD1|SetD0)))==(u8)(SetD1|SetD0))	//D1,D0=1,Lcd Ready
			break;
		i--;
	}
}
//==========================================================
//函数名称：Lcd_WriteData
//函数功能：写一个字节的数据到LCD
//入口参数：u8 待写数据
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 11:18
//备注说明：无
//==========================================================
void Lcd_WriteData(u8 dat)
{
	Lcd_ReadBF();	//等待Lcd准备好
	LCD_REG=dat;	//写数据
}
//==========================================================
//函数名称：Lcd_WriteCmd
//函数功能：写一个字节命令
//入口参数：u8 待写命令
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 11:18
//备注说明：无
//==========================================================
void Lcd_WriteCmd(u8 dat)
{
	Lcd_ReadBF();	//等待Lcd准备好
	LCD_RAM=dat;	//写命令
}
//==========================================================
//函数名称：Lcd_AutoWriteData
//函数功能：自动写模式下写一个字节的数据到LCD
//入口参数：u8 待写数据
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 11:18
//备注说明：D3:自动写允许位，为1才可操作
//			D2:自动读允许位，为1才可操作
//==========================================================
void Lcd_AutoWriteData(u8 dat)
{
	register u8 i;
	register u8 t;

	i=200;
	while(i>0)
	{
		t=LCD_RAM;
		if((t&((u8)SetD3))==(u8)(SetD3))	//D3=1,Lcd Auto Ready
			break;
		i--;
	}
	LCD_REG=dat;	//写数据
}

//==========================================================
//函数名称：Lcd_Set_Grh_Addr
//函数功能：设定图形坐标
//入口参数：无
//出口参数：无
//创建日期：2014.12.17
//修改日期：2014.12.17 09:22
//备注说明：图形坐标LcdAddr
//==========================================================
void Lcd_Set_Grh_Addr( void )
{
	register u16 addr;

	if(LcdAddr.x>=LCD_ADDR_X_MAX)//X地址判别
	{
		LcdAddr.x=0;//下一行
		LcdAddr.y++;
	}
	if(LcdAddr.y>=LCD_ADDR_Y_MAX)//Y地址判别
	{
		LcdAddr.y=0;//第一行
	}

//	addr=LCD_GRH_ADDR_START;
//	addr+=(LCD_GRH_ADDR_WIDTH*LcdAddr.y+LcdAddr.x);
	addr=LCD_GRH_ADDR_WIDTH*LcdAddr.y+LcdAddr.x;
	Lcd_WriteData(addr&0xFF);//地址L
	Lcd_WriteData((addr>>8)&0xFF);//地址H
	Lcd_WriteCmd(LCD_CMD_ADDR);//读写地址
}
//==========================================================
//函数名称：Lcd_Set_Txt_Addr
//函数功能：设定文本坐标
//入口参数：无
//出口参数：无
//创建日期：2014.12.26
//修改日期：2014.12.26 14:04
//备注说明：图形坐标LcdAddr
//==========================================================
void Lcd_Set_Txt_Addr( void )
{
	register u16 addr;

	if(LcdAddr.x>=LCD_ADDR_X_MAX)//X地址判别
	{
		LcdAddr.x=0;//下一行
		LcdAddr.y++;
	}
	if(LcdAddr.y>=LCD_ADDR_Y_MAX)//Y地址判别
	{
		LcdAddr.y=0;//第一行
	}

	addr=LCD_TXT_ADDR_START;
	addr+=(LcdAddr.x+(LcdAddr.y/8)*LCD_TXT_ADDR_WIDTH);
	Lcd_WriteData(addr&0xFF);//地址L
	Lcd_WriteData((addr>>8)&0xFF);//地址H
	Lcd_WriteCmd(LCD_CMD_ADDR);//读写地址
}
//==========================================================
//函数名称：Lcd_Clr
//函数功能：液晶屏清屏
//入口参数：无
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 15:33
//备注说明：无
//==========================================================
void Lcd_Clr( void )
{
	u16 i;

	LcdAddr.x=0;LcdAddr.y=0;//首地址
	Lcd_Set_Grh_Addr();//更新当前读写地址
	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//自动写数据
//	for(i=0;i<((LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX)+(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX/8));i++)//图形+文本
//	{
//		Lcd_AutoWriteData(0x00);//清空			
//	}
	for(i=0;i<(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX);i++)//图形
	{
		Lcd_AutoWriteData(0x00);//清空			
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//自动写数据复位

	Lcd_Set_Txt_Addr();//更新当前读写地址
	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//自动写数据
	for(i=0;i<(LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX/8);i++)//文本
	{
		Lcd_AutoWriteData(0x00);//清空			
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//自动写数据复位
}
//==========================================================
//函数名称：Lcd_WR_CGRAM
//函数功能：写数据到外部CGRAM
//入口参数：无
//出口参数：无
//创建日期：2014.12.17
//修改日期：2014.12.17 13:03
//备注说明：显示字符80H-FFH自动调用外部CGRAM，CGRAM起始地址参考偏移量设置
//注意事项：T6963C内部CGROM只支持8*8的低128的字符，且编码和ASCII不对应
//			外部CGRAM通常写入16*8、16*16等字库点阵数据，加快显示速度
//应用说明：外部CGRAM写入16*8的数字(0-9)点阵数据
//			显示首个16*8字符时只需在当前读写地址写入80H，下一行写入81H即可
//==========================================================
void Lcd_WR_CGRAM(void)
{
	u8 i;
	u8 j;

	Lcd_WriteData(LCD_CGRAM_ADDR_START&0xFF);//地址L，和偏移量设置值对应
	Lcd_WriteData((LCD_CGRAM_ADDR_START>>8)&0xFF);//地址H
	Lcd_WriteCmd(LCD_CMD_ADDR);//读写地址

	Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//自动写数据
	for(j=0;j<sizeof(TxtArray)/sizeof(Txt1608_TypeDef);j++)//写入字库数据
	{
		for(i=0;i<16;i++)
		{
			Lcd_AutoWriteData(AsciiArray[TxtArray[j].Ch[0]-LCD_1608_CODE_START].Dot[i]);//写数据
		}
	}
	Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//自动写数据复位
}

//==========================================================
//函数名称：Lcd_Init
//函数功能：液晶屏初始化
//入口参数：无
//出口参数：无
//创建日期：2014.12.16
//修改日期：2014.12.16 11:39
//备注说明：带参数命令，应先发数据后发命令
//==========================================================
void Lcd_Init(void)
{
	Lcd_WriteCmd(LCD_CMD_DISP_OFF);//显示关

	Lcd_WriteData(LCD_GRH_ADDR_START&0xFF);//图形首地址L
	Lcd_WriteData((LCD_GRH_ADDR_START>>8)&0xFF);//图形首地址H
	Lcd_WriteCmd(LCD_CMD_GRH_ADDR);//图形首地址
	
	Lcd_WriteData(LCD_GRH_ADDR_WIDTH);//图形宽度L
	Lcd_WriteData(0);//图形宽度H
	Lcd_WriteCmd(LCD_CMD_GRH_WIDTH);//图形宽度

	Lcd_WriteData(LCD_TXT_ADDR_START&0xFF);//文本首地址L
	Lcd_WriteData((LCD_TXT_ADDR_START>>8)&0xFF);//文本首地址H
	Lcd_WriteCmd(LCD_CMD_TXT_ADDR);//文本首地址
	
	Lcd_WriteData(LCD_TXT_ADDR_WIDTH);//文本宽度L
	Lcd_WriteData(0);//文本宽度H
	Lcd_WriteCmd(LCD_CMD_TXT_WIDTH);//文本宽度
	
	Lcd_WriteCmd(LCD_CMD_MODE_INTERNAL_CGROM|LCD_CMD_MODE_OR);//内部CGROM，或模式

//	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_TXT);//文本显示开，图形和光标关
//	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_GRH);//图形显示开，文本和光标关
	Lcd_WriteCmd(LCD_CMD_DISP_OFF|LCD_CMD_DISP_TXT|LCD_CMD_DISP_GRH);//文本和图形显示开，光标关

	Lcd_WriteData(LCD_CGRAM_ADDR_OFFSET);//偏移量，高5位地址
	Lcd_WriteData(0);
	Lcd_WriteCmd(LCD_CMD_OFFSET);//External CGRAM偏移寄存器(D4-D0)，对应高5位地址
	Lcd_WR_CGRAM();//写入字符0-9的16*8点阵库

//	Lcd_WriteData(0);//光标地址L
//	Lcd_WriteData(0);//光标地址H
//	Lcd_WriteCmd(LCD_CMD_CURSOR_POS);//光标地址
//	Lcd_WriteCmd(LCD_CMD_CURSOR_1LINE);//光标形状1行
	
	Lcd_Clr();//清屏
}

//==========================================================
//函数名称：Lcd_Wr_Txt1608
//函数功能：显示8*16的文本字符
//入口参数：u8 ch 字符
//出口参数：无
//创建日期：2014.12.26
//修改日期：2014.12.26 14:56
//备注说明：显示地址按文本地址处理
//==========================================================
void Lcd_Wr_Txt1608(u8 ch)
{
	register u8 i;
	register u8 t;
	
	t=LcdAddr.y;//备份
	for(i=0;i<sizeof(TxtArray)/sizeof(TxtArray[0]);i++)
	{
		if(ch==TxtArray[i].Ch[0])//查找字符
			break;
	}
	if(i<sizeof(TxtArray)/sizeof(TxtArray[0]))
	{
		Lcd_Set_Txt_Addr();//写地址
		Lcd_WriteData(TxtArray[i].Index[0]);//写数据
		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
		LcdAddr.y+=8;//下一行
		Lcd_Set_Txt_Addr();//写地址
		Lcd_WriteData(TxtArray[i].Index[1]);//写数据
		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
	}
	LcdAddr.y=t;//恢复
	LcdAddr.x++;//右移1个字节
}

//==========================================================
//函数名称：Lcd_Wr_1608
//函数功能：显示8*16的字符
//入口参数：u8 ch 字符, u8 highlight !=0高亮
//出口参数：无
//创建日期：2014.12.18
//修改日期：2014.12.18 15:31
//备注说明：无
//==========================================================
void Lcd_Wr_1608(u8 ch, u8 highlight)
{
	register u8 i;
	register u8 t;
	
	t=LcdAddr.y;//备份
	for(i=0;i<16;i++)
	{
		Lcd_Set_Grh_Addr();//写地址
		if(highlight==0)
			Lcd_WriteData(AsciiArray[ch-' '].Dot[i]);//写数据
		else
			Lcd_WriteData(~AsciiArray[ch-' '].Dot[i]);//写数据
//		Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
		Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//写命令,地址不变
		LcdAddr.y++;//下一行
	}
	LcdAddr.y=t;//恢复
	LcdAddr.x++;//右移1个字节
}

//==========================================================
//函数名称：Lcd_Wr_1616
//函数功能：显示16*16的字符
//入口参数：u8 ch 字符, u8 highlight !=0高亮
//出口参数：无
//创建日期：2014.12.18
//修改日期：2014.12.18 15:02
//备注说明：无
//==========================================================
void Lcd_Wr_1616(u8 *pt, u8 highlight)
{
	register u8 i;
	register u8 index;
	register u8 t;
	
	t=LcdAddr.y;//备份

	for(i=0;i<sizeof(Hanzi16Array)/sizeof(Hanzi16Array[0]);i++)
	{
		if((*pt==Hanzi16Array[i].Str[0])&&(*(pt+1)==Hanzi16Array[i].Str[1]))//查找汉字
			break;
	}
	if(i<sizeof(Hanzi16Array)/sizeof(Hanzi16Array[0]))
	{
		index=i;//字符索引
		for(i=0;i<32;)
		{
			Lcd_Set_Grh_Addr();//写地址
			if(highlight==0)
				Lcd_WriteData(Hanzi16Array[index].Dot[i++]);//写数据
			else
				Lcd_WriteData(~Hanzi16Array[index].Dot[i++]);//写数据
			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//写命令,地址加一
			if(highlight==0)
				Lcd_WriteData(Hanzi16Array[index].Dot[i++]);//写数据
			else
				Lcd_WriteData(~Hanzi16Array[index].Dot[i++]);//写数据
			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
			LcdAddr.y++;//下一行
		}
	}
	LcdAddr.y=t;//恢复
	LcdAddr.x+=2;//右移2个字节
}

//==========================================================
//函数名称：Lcd_Draw_Line
//函数功能：显示直线
//入口参数：x,y:起始点，len:长度或高度
//出口参数：无
//创建日期：2014.12.26
//修改日期：2014.12.26 15:42
//备注说明：X=0:画横线，X!=0:画竖线
//==========================================================
void Lcd_Draw_Line(u8 x, u8 y, u8 len)
{
	register u8 i;
	u8 addrx,addry;

	addrx=LcdAddr.x;//备份
	addry=LcdAddr.y;
	LcdAddr.x=x;LcdAddr.y=y;

	if(x==0)//画横线
	{
		Lcd_Set_Grh_Addr();//写地址
		Lcd_WriteCmd(LCD_CMD_AUTO_WRITE);//自动写数据
		if(len>LCD_ADDR_X_MAX)//最大长度
			len=LCD_ADDR_X_MAX;
		for(i=0;i<len;i++)
		{
			Lcd_AutoWriteData(0xFF);
		}
		Lcd_WriteCmd(LCD_CMD_AUTO_RESET);//自动写数据复位
	}
	else//画竖线
	{
		if(y>LCD_ADDR_Y_MAX)//最大高度
			y=LCD_ADDR_Y_MAX;
		if((len+y)>LCD_ADDR_Y_MAX)//最大高度
			len=LCD_ADDR_Y_MAX-y;//最大高度;
		for(i=addry;i<len;i++)
		{
			Lcd_Set_Grh_Addr();//写地址
			Lcd_AutoWriteData(0x80);//D7-D0，最左边置位
			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
			LcdAddr.y++;//下一行
		}
	}
	LcdAddr.x=addrx;//恢复
	LcdAddr.y=addry;//恢复
}

//==========================================================
//函数名称：Lcd_Wr_1212
//函数功能：显示12*12的字符
//入口参数：u8 ch 字符, u8 highlight !=0高亮
//出口参数：无
//创建日期：2014.12.18
//修改日期：2014.12.18 15:02
//备注说明：因驱动器x地址按字节(8位)寻址，需要读屏处理
//==========================================================
//void Lcd_Wr_1212A(u8 *pt, u8 highlight)
//{
//	register u8 i;
//	register u8 index;
//	register u8 t;
//	register u8 dat;
//
//	t=LcdAddr.y;//备份
//
//	for(i=0;i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]);i++)
//	{
//		if((*pt==Hanzi12Array[i].Str[0])&&(*(pt+1)==Hanzi12Array[i].Str[1]))//查找汉字
//			break;
//	}
//	if(i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]))//字符有效
//	{
//		index=i;//字符索引
//		for(i=0;i<24;)//写数据
//		{
//			Lcd_Set_Grh_Addr();//写地址
//			if(highlight==0)
//				Lcd_WriteData(Hanzi12Array[index].Dot[i++]);//写数据
//			else
//				Lcd_WriteData(~Hanzi12Array[index].Dot[i++]);//写数据
//			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//写命令,地址加一
//			//后半个字更新，8位中只更新高4位，低4位不变
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat&=0xF0;//更新高4位
//			Lcd_WriteCmd(LCD_CMD_RD_NOC_ADDR);//读命令,地址不变
//			dat|=(LCD_DAT_RD_BUS&0x0F);//读取当前地址的屏内容,保留低4位
//			Lcd_WriteData(dat);//写数据
//			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
//			LcdAddr.y++;//下一行
//		}
//	}
//	LcdAddr.y=t;//恢复
//	LcdAddr.x+=2;//右移2个字节
//}
////==========================================================
//void Lcd_Wr_1212B(u8 *pt, u8 highlight)
//{
//	register u8 i;
//	register u8 index;
//	register u8 t;
//	register u8 dat,dat2;
//
//	t=LcdAddr.y;//备份
//
//	for(i=0;i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]);i++)
//	{
//		if((*pt==Hanzi12Array[i].Str[0])&&(*(pt+1)==Hanzi12Array[i].Str[1]))//查找汉字
//			break;
//	}
//	if(i<sizeof(Hanzi12Array)/sizeof(Hanzi12Array[0]))//字符有效
//	{
//		index=i;//字符索引
//		for(i=0;i<24;)//写数据
//		{
//			Lcd_Set_Grh_Addr();//写地址
//			//前半个字更新，8位中只更新低4位，高4位不变
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat2=dat;//备份
//			dat&=0xF0;//更新低4位
//			dat>>=4;
//			Lcd_WriteCmd(LCD_CMD_RD_NOC_ADDR);//读命令,地址不变
//			dat|=(LCD_DAT_RD_BUS&0xF0);//读取当前地址的屏内容,保留高4位
//			Lcd_WriteData(dat);//写数据
//			Lcd_WriteCmd(LCD_CMD_WR_INC_ADDR);//写命令,地址加一
//			if(highlight==0)
//				dat=(Hanzi12Array[index].Dot[i++]);
//			else
//				dat=(~Hanzi12Array[index].Dot[i++]);
//			dat&=0xF0;//保留高4位
//			dat>>=4;
//			dat2&=0x0F;//更新低4位
//			dat2<<=4;
//			dat|=dat2;
//			Lcd_WriteData(dat);//写数据
//			Lcd_WriteCmd(LCD_CMD_WR_NOC_ADDR);//写命令,地址不变
//			LcdAddr.y++;//下一行
//		}
//	}
//	LcdAddr.y=t;//恢复
//	LcdAddr.x+=2;//右移2个字节
//}

//void LCD_WR_REG(u8 regval)
//{ 
//	LCD_REG=regval;//写入要写的寄存器序号	 
//}
////写LCD数据
////data:要写入的值
//void LCD_WR_DATA(u8 data)
//{										    	   
//	LCD_RAM=data;		 
//}
//读LCD数据
//返回值:读到的值
//u16 LCD_RD_DATA(void)
//{										    	   
//	return LCD_RAM;		 
//}					   
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
//void LCD_WriteReg(u8 LCD_Reg, u8 LCD_RegValue)
//{	
//	LCD_REG = LCD_Reg;		//写入要写的寄存器序号	 
//	LCD_RAM = LCD_RegValue;//写入数据	    		 
//}

//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
//u16 LCD_ReadReg(u8 LCD_Reg)
//{										   
//	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
//	delay_us(5);		  
//	return LCD_RD_DATA();		//返回读到的值
//}  
