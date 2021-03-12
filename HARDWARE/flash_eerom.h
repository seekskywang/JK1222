
#ifndef _flash_eer_H
#define _flash_eer_H

#define STORESETADDR   			0x08060000
#define STORE_M1				(STORESETADDR+0x800)
#define STORE_M2				(STORE_M1+0x800)
#define STORE_M3				(STORE_M2+0x800)
#define STORE_M4				(STORE_M3+0x800)
#define STORE_M5				(STORE_M4+0x800)

void Read_set_flash(void);
void Store_set_flash(void);
void Read_compvalue(u8 data);

#endif 
