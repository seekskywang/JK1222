
#ifndef __FLASH_IF_H
#define __FLASH_IF_H

#include <stm32f4xx.h>
#include "flash_if.h"

#define USER_FLASH_FIRST_PAGE_ADDRESS 0x080C0000 //Æ«ÒÆ128*2KB
#define USER_FLASH_LAST_PAGE_ADDRESS  0x080E0000  //Íùºó0xA0000BÎªAPP´æ´¢Çø
#define USER_FLASH_END_ADDRESS        0x080FFFFF  
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - USER_FLASH_FIRST_PAGE_ADDRESS)

uint32_t FLASH_If_Write(__IO uint32_t* Address, uint32_t* Data, uint16_t DataLength);
int8_t FLASH_If_Erase(uint32_t StartSector);
void FLASH_If_Init(void);
u8 IAP_Start(void);
uint32_t IAP_Flash_Write(uint32_t addr,uint8_t *pbuf,uint32_t size);
#endif 


