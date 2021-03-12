#ifndef _UART_H
#define _UART_H
//==========================================================
//调试配置
#define _HW_CONFIG_FALSE	(0)
#define _HW_CONFIG_TRUE		(1)

//==========================================================
//收发帧缓冲长度
#define REC_LEN_MAX  	(55)//接收帧长度
#define SEND_LEN_MAX 	(18)//发送帧长度

//串口发送结构
typedef struct
{
	u8 begin;//发送开始标志
	u8 ptr;//当前发送指针
	u8 len;//发送数据长度
	u8 buf[SEND_LEN_MAX];//数据发送缓冲
}ComSend_TypeDef;

//串口接收结构
typedef struct
{
	u8 end;//接收结束标志
	u8 ptr;	//当前接收指针
	u8 len;//接收数据长度
	u8 buf[REC_LEN_MAX];//数据接收缓冲
	u8 TimeOut;//接收超时(10ms周期)
}ComRec_TypeDef;

//串口收发结构
typedef struct
{
	ComSend_TypeDef send;
	ComRec_TypeDef  rec;
}Com_TypeDef;

//==========================================================
//全局变量
//#if HW_UART_SUPPORT
//	#if DEBUG_SUPPORT
//		extern idata Com_TypeDef ComBuf;//串口收发缓冲
//	#else
//		extern xdata Com_TypeDef ComBuf;//串口收发缓冲
//	#endif
//#endif
extern  Com_TypeDef ComBuf;
//==========================================================
//接收溢出时间,3*1000/50=60mS
#define REC_TIME_OUT	 (3*TICKS_PER_SEC/50)

//发送数据后等待应答时间,10mSx100=1S
#define COMMWAITTICKS	(100)

//设置串口接收溢出时间
#define SetRecTimeOut(time) (ComBuf.rec.TimeOut=time)

#define PC_DEFAULT_ADDR			(0)//PC地址
#define MACHINE_DEFAULT_ADDR	(1)//仪器默认地址

//==========================================================
//通信协议，8位，9600波特率
//==========================================================
//起始符  总长度  目标地址  命令字  数据包  累加和  结束符
//1字节	  1字节	  1字节	    1字节	N字节   1字节   1字节
//==========================================================
#define UART_SEND_BEGIN 		(0xAB)
#define UART_SEND_END 			(0xAF)
#define FRAME_LEN_MAX 			(REC_LEN_MAX)//一帧最大字节
#define FRAME_LEN_MIN 			(3)	//一帧最小字节
#define UART_REC_BEGIN 			(0xAA)
#define UART_REC_END 			(0xBF)

//帧类型定义
#define FRAME_READ_RESULT		(0x00)	//读取结果
#define FRAME_START				(0x01)	//启动
#define FRAME_RESET				(0x02)	//复位
#define FRAME_DATA				(0xF1)	//数据帧
#define FRAME_CLR_BOOT_NUM		(0xF2)	//清开机次数帧
#define FRAME_WRITE_SN			(0xF3)	//写序列号帧

//帧位置定义
#define PFRAMEHEAD				(0)	//帧头位置
#define PFRAMEKIND				(1)	//帧类型位置
#define PDATASTART				(2)	//数据包首地址位置

//通信状态定义
#define COM_SUCCESS				(0x00)	//成功
#define COM_ERR_BUSY	 	 	(0x01)	//信道忙
#define COM_ERR_CRC				(0x02)	//校验错误
#define COM_ERR_NACK 			(0x03)	//无应答
#define COM_ERR_UNEXPECT 		(0x04)	//非期望帧
#define COM_ERR_FORMAT			(0x05)	//数据格式错误
#define COM_ERR_SAVE 			(0x06)	//无法保存数据
#define COM_ERR_ADDR_FIRE	 	(0x07)	//地址冲突
#define COM_ERR_UNDEFINE		(0x08)	//未定义错误


//==========================================================
//函数列表
void Uart0RecTimeOut(void);//串口接收超时判别

#endif 
