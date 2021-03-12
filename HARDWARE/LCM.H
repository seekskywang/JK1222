#ifndef _lcm_H
#define _lcm_H
#define Bank1_LCD_C        ((u32)0x6C000002)
#define Bank1_LCD_D        ((u32)0x6C000000)

#define LCD_REG            *(__IO uint8_t *)(Bank1_LCD_D)
#define LCD_RAM            *(__IO uint8_t *)(Bank1_LCD_C)

//==========================================================
//LCD操作命令
#define LCD_CMD_CURSOR_POS	(u8)(0x21)	//光标位置寄存器(AddrX(0-4F),AddrY(0-0F))
#define LCD_CMD_OFFSET		(u8)(0x22)	//External CGRAM偏移寄存器(AddrL,AddrH)
										//D4-D0:A15-A11
										//00000-Extern CGRAM Addr (0000-07FFH)
										//00001-Extern CGRAM Addr (0800-0FFFH)
										//00010-Extern CGRAM Addr (1000-17FFH)
										//11100-Extern CGRAM Addr (E000-E7FFH)
										//11101-Extern CGRAM Addr (E800-EFFFH)
										//11110-Extern CGRAM Addr (F000-F7FFH)
										//11111-Extern CGRAM Addr (F800-FFFFH)
#define LCD_CMD_ADDR		(u8)(0x24)	//读写地址寄存器(AddrL,AddrH)

#define LCD_CMD_TXT_ADDR	(u8)(0x40)	//文本首地址(AddrL,AddrH)
#define LCD_CMD_TXT_WIDTH	(u8)(0x41)	//文本宽度(Columns,0)
#define LCD_CMD_GRH_ADDR	(u8)(0x42)	//图形首地址(AddrL,AddrH)
#define LCD_CMD_GRH_WIDTH	(u8)(0x43)	//图形宽度(Columns,0)

#define LCD_CMD_MODE_INTERNAL_CGROM	(u8)(0x80)	//内部CGROM，D2-D0:模式控制
#define LCD_CMD_MODE_EXTERNAL_CGRAM	(u8)(0x88)	//外部CGRAM，D2-D0:模式控制
#define LCD_CMD_MODE_OR		(u8)(0x00)	//或模式
#define LCD_CMD_MODE_XOR	(u8)(0x02)	//异或模式
#define LCD_CMD_MODE_AND	(u8)(0x03)	//与模式
#define LCD_CMD_MODE_TXT	(u8)(0x04)	//文本属性

#define LCD_CMD_DISP_OFF	(u8)(0x90)	//显示关，D3-D0:显示控制
#define LCD_CMD_DISP_BLINK	(u8)(SetD0)	//D0=1/0:1=光标闪烁开
#define LCD_CMD_DISP_CURSOR	(u8)(SetD1)	//D1=1/0:1=光标显示开
#define LCD_CMD_DISP_TXT	(u8)(SetD2)	//D2=1/0:1=文本显示开
#define LCD_CMD_DISP_GRH	(u8)(SetD3)	//D3=1/0:1=图形显示开

#define LCD_CMD_CURSOR_1LINE (u8)(0xA0)	//1行光标
#define LCD_CMD_CURSOR_2LINE (u8)(0xA1)	//2行光标
#define LCD_CMD_CURSOR_3LINE (u8)(0xA2)	//3行光标
#define LCD_CMD_CURSOR_4LINE (u8)(0xA3)	//4行光标
#define LCD_CMD_CURSOR_5LINE (u8)(0xA4)	//5行光标
#define LCD_CMD_CURSOR_6LINE (u8)(0xA5)	//6行光标
#define LCD_CMD_CURSOR_7LINE (u8)(0xA6)	//7行光标

#define LCD_CMD_AUTO_WRITE	(u8)(0xB0)	//数据自动写
#define LCD_CMD_AUTO_READ	(u8)(0xB1)	//数据自动读
#define LCD_CMD_AUTO_RESET	(u8)(0xB2)	//自动读写复位

#define LCD_CMD_WR_INC_ADDR	(u8)(0xC0)	//写一数据，ADP地址加一
#define LCD_CMD_RD_INC_ADDR	(u8)(0xC1)	//读一数据，ADP地址加一
#define LCD_CMD_WR_DCC_ADDR	(u8)(0xC2)	//写一数据，ADP地址减一
#define LCD_CMD_RD_DEC_ADDR	(u8)(0xC3)	//读一数据，ADP地址减一
#define LCD_CMD_WR_NOC_ADDR	(u8)(0xC4)	//写一数据，ADP地址不变
#define LCD_CMD_RD_NOC_ADDR	(u8)(0xC5)	//读一数据，ADP地址不变

#define LCD_CMD_SCREEN_PEEK	(u8)(0xE0)	//从当前地址读取一字节数据
#define LCD_CMD_SCREEN_COPY	(u8)(0xE8)	//整屏复制

#define LCD_CMD_BIT			(u8)(0xF0)	//对当前地址的数据进行位操作
#define LCD_CMD_BIT_RESET	(u8)(ClrD3)	//D3=0，Bit Reset
#define LCD_CMD_BIT_SET		(u8)(SetD3)	//D3=1，Bit Set
										//D2-D0位选择，7-0对应D7-D0位

//==========================================================
//LCD常数定义
#define LCD_ADDR_X_MAX ((u8)(30)) //X坐标最大值(30*8=240)
#define LCD_ADDR_Y_MAX ((u8)(128))//Y坐标最大值(128)

#define LCD_GRH_ADDR_START ((u16)(0))//图形起始地址
#define LCD_GRH_ADDR_WIDTH ((u8)(LCD_ADDR_X_MAX))//图形宽度

#define LCD_TXT_ADDR_START ((u16)((LCD_ADDR_X_MAX)*(LCD_ADDR_Y_MAX)))  //文本起始地址
#define LCD_TXT_ADDR_WIDTH ((u8)(LCD_ADDR_X_MAX))//文本宽度

#define LCD_CGRAM_ADDR_OFFSET ((u8)(2))//对应外部CGRAM的高5位地址(A15-A11)
//文本字符80H在外部CGRAM的首地址为1400H
//Offset(A15-A11=00010)+Code(A10-A3=10000000)+Vertical counter(A2-A0=000) = 1400H
#define LCD_CGRAM_ADDR_START (((u16)(LCD_CGRAM_ADDR_OFFSET<<11)+(u16)(0x80<<3))+0)//字符80H对应的外部CGRAM的起始地址

#define LCD_1608_CODE_START	((u8)(' '))//16*8文本字符字库的起始字符
//LCD坐标结构体定义
typedef struct 
{
	u8 x;	//X坐标
	u8 y;	//Y坐标
}LcdAddr_TypeDef;

extern LcdAddr_TypeDef LcdAddr;//LCD地址结构体

//==========================================================
//函数列表
//void Lcd_ReadBF(void);//读取液晶的状态位直至BF标志有效(读200次后自动退出)
void Lcd_WriteCmd(u8);//写命令
void Lcd_WriteData(u8);//写数据
void Lcd_AutoWriteData(u8);//自动模式下写数据
void Lcd_Set_Grh_Addr(void);//LCD图形坐标LcdAdr设置
void Lcd_Set_Txt_Addr(void);//LCD文本坐标LcdAdr设置
void Lcd_Clr(void);//全屏清零
void Lcd_Init(void);//液晶初始化
void Lcd_Wr_Txt1608(u8 ch);//写文本模式的8*16字符
void Lcd_Wr_1608(u8 ch, u8 highlight);//写8*16字符
void Lcd_Wr_1616(u8 *pt, u8 highlight);//写16*16字符
//void Lcd_Wr_1212A(u8 *pt, u8 highlight);//写12*12字符
//void Lcd_Wr_1212B(u8 *pt, u8 highlight);//写12*12字符
//void Lcd_WR_CGRAM(void);//写字符数据到外部CGRAM(code:80H-FFH)
void Lcd_Draw_Line(u8 x, u8 y, u8 len);//X=0:画横线，否则画竖线，len:长度或高度

#endif 
