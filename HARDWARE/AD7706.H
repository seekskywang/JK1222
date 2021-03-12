#ifndef _ad7706_H
#define _ad7706_H
void AD7705_init(u8 data);
u16 Read_Channel(u8 data );
void AD7705_write_Reg(u8 data_byte);
void INIT_AD_Convert(u8 gpio,u8 pga,u8 ch);
//void INIT_AD_Convert(void);//ads1216初始化
void AD_Convert_read(void);//读AD值
void ch_select(u8 ch,u8 gain);//通道选择,PGA倍率
void ADS1216_Set_IO(u8 port,u8 gain);//端口控制
void ADS_1216_PGA(u8 gain);

s32 Read__Convert_read(uint8_t num);
u32 Read_Convert_read(void);
#endif 
