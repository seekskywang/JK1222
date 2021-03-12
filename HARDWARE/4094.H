#ifndef _4094_H
#define _4094_H
#define _4094NUM   1
#define HC595HC  5


extern u8 _4094_databuff[_4094NUM];
void Out_4094(u8 *buff);
void HC595_Send(u8 *buff);
#endif 
