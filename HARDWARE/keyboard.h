#ifndef _KEYBOARD_H
#define _KEYBOARD_H
//==========================================================
//调试配置
#define _HW_CONFIG_FALSE	(0)
#define _HW_CONFIG_TRUE		(1)

#define TICKS_PER_SEC 100	//系统时钟每秒节拍数，100x10mS(T0)=1S
#define TICKS_PER_SEC_SOFTTIMER 20	//软定时器时钟每秒节拍数，100x10mS=1S
//Lcd是否支持持续按键
#define HW_KEYBOARD_CONTINUE_SUPPORT	(_HW_CONFIG_TRUE)

//==========================================================
//按键常数定义

#define 	Key_F1  	(20)
#define		Key_F2  	(19)
#define 	Key_F3  	(18)
#define 	Key_F4  	(17)
#define		Key_F5 		(16)
#define		Key_Ent 	 (4)
#define		Key_SET1	(5)
#define		Key_ONOFF	(15)
#define		Key_LEFT	(25)
#define		Key_RIGHT  	(26)
#define		Key_SET2	(22)
#define		Key_SHIFT	(21)
#define		Key_NUM7	(9)
#define		Key_NUM8	(10)
#define		Key_NUM9	(11)
#define		Key_NUM4	(8)
#define		Key_NUM5	(7)
#define		Key_NUM6	(6)
#define		Key_NUM1	(1)
#define		Key_NUM2	(2)
#define		Key_NUM3	(3)
#define		Key_NUM0	(14)
#define		Key_DOT		(13)
#define		Key_ESC		(12)
#define		Key_SAVE	(24)//
#define		Key_ON	    (22)
#define		PRESS_SPIN	(28)
#define		Key_REST	(23)
#define		Key_TRIG	(31)
#define		Key_POWER	(55)//

#define _Hc164CpL GPIO_ResetBits(GPIOC,GPIO_Pin_8)//GPIO_ClearValue(1, (1<<1))
#define _Hc164CpH GPIO_SetBits(GPIOC,GPIO_Pin_8)//GPIO_SetValue(1, (1<<1))
#define _Hc164DL GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define _Hc164DH GPIO_SetBits(GPIOC,GPIO_Pin_12)


#define LONGKEYCOUNT 	(TICKS_PER_SEC)	//长按键节拍数(1s)
//#define SHORTKEYCOUNT 	(TICKS_PER_SEC/20)	//短按键节拍数(50ms)
#define SHORTKEYCOUNT 	(TICKS_PER_SEC/30)	//短按键节拍数(33ms)
#define CONTINUEKEYCOUNT (TICKS_PER_SEC/7)	//持续按键节拍数(100ms)
//键值定义
#define HW_KEY_SET			((u16)(ClrD5))
#define HW_KEY_UP			((u16)(ClrD3))
#define HW_KEY_DOWN			((u16)(ClrD4))
#define HW_KEY_LEFT			((u16)(ClrD7))
#define HW_KEY_RIGHT		((u16)(ClrD8))
#define HW_KEY_ENTER		((u16)(ClrD6))
#define HW_KEY_RESET		((u16)(ClrD2))//pc4
#define HW_KEY_START		((u16)(ClrD1))
#define HW_KEY_RESET1		((u16)(ClrD0))//pc4
#define HW_KEY_START1		((u16)(ClrD15))

#define DIG1L GPIO_ResetBits(DIG1_GPIO_PORT,DIG1_PIN)
#define DIG1H GPIO_SetBits(DIG1_GPIO_PORT,DIG1_PIN)
#define DIG2L GPIO_ResetBits(DIG2_GPIO_PORT,DIG2_PIN)
#define DIG2H GPIO_SetBits(DIG2_GPIO_PORT,DIG2_PIN)
#define DIG3L GPIO_ResetBits(DIG3_GPIO_PORT,DIG3_PIN)
#define DIG3H GPIO_SetBits(DIG3_GPIO_PORT,DIG3_PIN)
#define DIG4L GPIO_ResetBits(DIG4_GPIO_PORT,DIG4_PIN)
#define DIG4H GPIO_SetBits(DIG4_GPIO_PORT,DIG4_PIN)

#define READKI1 GPIO_ReadInputDataBit(KI1_GPIO_PORT,KI1_PIN)
#define READKI2 GPIO_ReadInputDataBit(KI2_GPIO_PORT,KI2_PIN)
#define READKI3 GPIO_ReadInputDataBit(KI3_GPIO_PORT,KI3_PIN)
#define READKI4 GPIO_ReadInputDataBit(KI4_GPIO_PORT,KI4_PIN)
#define READKI5 GPIO_ReadInputDataBit(KI5_GPIO_PORT,KI5_PIN)
#define READKI6 GPIO_ReadInputDataBit(KI6_GPIO_PORT,KI6_PIN)
#define READKI7 GPIO_ReadInputDataBit(KI7_GPIO_PORT,KI7_PIN)



#define DIG1_PIN                  GPIO_Pin_5                 
#define DIG1_GPIO_PORT            GPIOC                      
#define DIG1_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define DIG2_PIN                  GPIO_Pin_2                 
#define DIG2_GPIO_PORT            GPIOA                      
#define DIG2_GPIO_CLK             RCC_AHB1Periph_GPIOA

#define DIG3_PIN                  GPIO_Pin_1                 
#define DIG3_GPIO_PORT            GPIOC                      
#define DIG3_GPIO_CLK             RCC_AHB1Periph_GPIOC

#define DIG4_PIN                  GPIO_Pin_1                 
#define DIG4_GPIO_PORT            GPIOI                      
#define DIG4_GPIO_CLK             RCC_AHB1Periph_GPIOI

#define KI1_PIN                   GPIO_Pin_13                
#define KI1_GPIO_PORT             GPIOG                      
#define KI1_GPIO_CLK              RCC_AHB1Periph_GPIOG

//#define KI2_PIN                   GPIO_Pin_15                 
//#define KI2_GPIO_PORT             GPIOB                      
//#define KI2_GPIO_CLK              RCC_AHB1Periph_GPIOB

#define KI2_PIN                   GPIO_Pin_2                 
#define KI2_GPIO_PORT             GPIOG                      
#define KI2_GPIO_CLK              RCC_AHB1Periph_GPIOG

//#define KI3_PIN                   GPIO_Pin_7                 
//#define KI3_GPIO_PORT             GPIOB                      
//#define KI3_GPIO_CLK              RCC_AHB1Periph_GPIOB

#define KI3_PIN                   GPIO_Pin_3                
#define KI3_GPIO_PORT             GPIOG                      
#define KI3_GPIO_CLK              RCC_AHB1Periph_GPIOG

#define KI4_PIN                   GPIO_Pin_6                
#define KI4_GPIO_PORT             GPIOI                      
#define KI4_GPIO_CLK              RCC_AHB1Periph_GPIOI

#define KI5_PIN                   GPIO_Pin_3                 
#define KI5_GPIO_PORT             GPIOD                      
#define KI5_GPIO_CLK              RCC_AHB1Periph_GPIOD

#define KI6_PIN                   GPIO_Pin_14                
#define KI6_GPIO_PORT             GPIOH                      
#define KI6_GPIO_CLK              RCC_AHB1Periph_GPIOH

#define KI7_PIN                   GPIO_Pin_12                
#define KI7_GPIO_PORT             GPIOH                      
#define KI7_GPIO_CLK              RCC_AHB1Periph_GPIOH
/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

#define CMP_VALUE    (800)
#define FAST_RANK_2  (200)
#define FAST_RANK_1  (30)
 /*******************************************************/

 /** 键值
	* 
	*/
#define KEY_1		0x16
#define KEY_2		0x11
#define KEY_3		0x0C
#define KEY_4		0x17
#define KEY_5		0x12
#define KEY_6		0x0F
#define KEY_7		0x19
#define KEY_8		0x13
#define KEY_9		0x0E
#define KEY_0		0x07
#define KEY_DOT		0x09
#define KEY_BACK	0x0A
#define KEY_FUNC1	0x15
#define KEY_FUNC2	0x10
#define KEY_FUNC3	0x0B
#define KEY_FUNC4	0x06
#define KEY_FUNC5	0x05
#define KEY_UP		0x14
#define KEY_DOWN	0x08
#define KEY_LEFT	0x18
#define KEY_RIGHT	0x03
#define KEY_ESC		0x01
#define KEY_ENTER	0x02
#define KEY_ACC		0x0D

#define BIT2		2
#define BIT0		1

//按键编码
#define LONG_PRESS		((u8)0x80)
#define KEY_NONE		((u8)0x00)
#define KEY_SET			((u8)('a'))
//#define KEY_UP			((u8)('b'))
//#define KEY_DOWN		((u8)('c'))
//#define KEY_LEFT		((u8)('d'))
//#define KEY_RIGHT		((u8)('e'))
//#define KEY_ENTER		((u8)('f'))
#define KEY_RESET		((u8)('g'))
#define KEY_START		((u8)('h'))

#define L_KEY_SET		(LONG_PRESS | KEY_SET)		//设置
#define L_KEY_UP		(LONG_PRESS | KEY_UP)		//上
#define L_KEY_DOWN		(LONG_PRESS | KEY_DOWN)		//下
#define L_KEY_LEFT		(LONG_PRESS | KEY_LEFT)		//左
#define L_KEY_RIGHT		(LONG_PRESS | KEY_RIGHT)	//右
#define L_KEY_ENTER		(LONG_PRESS | KEY_ENTER)	//确认
#define L_KEY_RESET		(LONG_PRESS | KEY_RESET)	//复位
#define L_KEY_START		(LONG_PRESS | KEY_START)	//启动
//==========================================================
//按键结构体声明
typedef struct 
{
	u8 value;	//键值
	u8 state;	//键有按下=TRUE
	u8 continuance;//持续按键有效=TRUE
}Key_TypeDef;

extern  uint32_t CurrentTextColor   ;
extern  uint32_t CurrentBackColor   ;
//==========================================================
//全局变量
extern Key_TypeDef Keyboard;//键码结构体

//==========================================================
//函数声明
extern u8 Key_Identiry(void);//按键识别，返回键值
extern void Keyboard_Init(void);//按键结构体初始化
extern void Key_Scan(void);//按键扫描
extern u8 Key_Read(void);//读取按键，返回键值
extern u8 Key_Read_WithTimeOut(u8 timeticks);//阻塞式读取按键，返回键值
void Key_Scan(void);
void Spin_Init(void);
uint16_t Encoder_Process(u8 list);
#endif 
