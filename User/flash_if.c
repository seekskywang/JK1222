
#include "flash_if.h"

/**
  * @brief  解锁flash
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  FLASH_Unlock(); 
}

/**
  * @brief  擦除指定扇区
  * @param  StartSector:擦除的起始扇区
  * @retval 0: 擦除成功;1:擦除失败        
  */
int8_t FLASH_If_Erase(uint32_t StartSector)
{
  uint32_t FlashAddress;
  
  FlashAddress = StartSector;

  //设备电压范围2.7V-3.6V,字操作 
 
  if (FlashAddress <= (uint32_t) USER_FLASH_LAST_PAGE_ADDRESS)
  {
	  FLASH_EraseSector(FLASH_Sector_0, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_1, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3); // 128 Kbyte
	  FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3); // 128 Kbyte
//	  FLASH_EraseSector(FLASH_Sector_18, VoltageRange_3); // 128 Kbyte
//	  FLASH_EraseSector(FLASH_Sector_19, VoltageRange_3); // 128 Kbyte
//	  FLASH_EraseSector(FLASH_Sector_20, VoltageRange_3); // 128 Kbyte
//	  FLASH_EraseSector(FLASH_Sector_21, VoltageRange_3); // 128 Kbyte
//	  FLASH_EraseSector(FLASH_Sector_22, VoltageRange_3); // 128 Kbyte
   // FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3); // 128 Kbyte 
   //   FLASH_EraseSector(FlashAddress, VoltageRange_3); // 128 Kbyte 
//    FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3); // 128 Kbyte 
//    FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3); // 128 Kbyte 
//    FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3); // 128 Kbyte 
//    FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3); // 128 Kbyte   //共896KB
  }
  else
  {
    return (1);
  }

  return (0);
}

/**
  * @brief  写数据至flash,4字节对其
  * @param  FlashAddress: 起始地址;Data:写入数据 
  *         DataLength: 数据长度 (单位为32位)   
  * @retval 0: 写入成功
  *         1: 写入失败
  *         2: 数据校验错误
  */
uint32_t FLASH_If_Write(__IO uint32_t* FlashAddress, uint32_t* Data ,uint16_t DataLength)
{
  uint32_t i = 0;

  for (i = 0; (i < DataLength) && (*FlashAddress <= (USER_FLASH_END_ADDRESS-4)); i++)
  {
    if (FLASH_ProgramWord(*FlashAddress, *(uint32_t*)(Data+i)) == FLASH_COMPLETE)
    {
      if (*(uint32_t*)*FlashAddress != *(uint32_t*)(Data+i)) //检查数据写入的一致性
      {
        return(2);                             
      }
      *FlashAddress += 4;
    }
    else
    {
      return (1);                                            //写入出错
    }
  }

  return (0);
}

/**
  * @brief  开始执行IAP
  * @param  None     
  * @retval 0: 擦除成功
  *         1: 擦除失败
  */
u8 IAP_Start(void)
{
	u8 state;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	state=FLASH_If_Erase(FLASH_Sector_7);//擦除扇区,共128KB
  return state;
}

/**
  * @brief  写入flash
  * @param  add: 写入的地址  pbuf:指向数据缓冲区;size:写入的数据大小
  * @retval 0: 擦除成功
  *         1: 擦除失败
  */
uint32_t IAP_Flash_Write(uint32_t addr,uint8_t *pbuf,uint32_t size)
{
		uint32_t ret;
		ret=FLASH_If_Write(&addr,(uint32_t*)pbuf,size);	
		return ret;
}
