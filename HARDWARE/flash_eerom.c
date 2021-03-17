#include "pbdata.h"
u32 GROUP_ADD[]={STORE_M1,STORE_M2,STORE_M3,STORE_M4,STORE_M5};
void Store_set_flash(void)
{
//	u16 i;
//	u32 addr;
	SaveData_Typedef *ptt;
    ptt=&LoadSave;
    SPI_FLASH_SectorErase(0);
//    delay_ms(10);
    SPI_FLASH_BufferWrite((u8 *)ptt, 0, sizeof(LoadSave));
}
//void Read_compvalue(u8 data)
//{
//	u32 addr;
//	u16 i;
//	System_TypeDef *pt;
//	pt= &System;
//	Parameter_valuecomp();
//	addr=GROUP_ADD[data];
//	for(i=0;i<sizeof(SaveData);i++)
//	{
//		*((u8*)pt+i)=*(((u8*)(addr))+i);//FMC_Read(addr+i);
//			
//	}

//}
void Read_set_flash(void)
{
//	u16 i;
	u32 addr;
	SaveData_Typedef *ptt;
    ptt=&LoadSave;
//	addr=STORESETADDR;
    addr=0;

    SPI_FLASH_BufferRead((u8*)ptt, addr, sizeof(LoadSave));
	//
	if(LoadSave.sence > 1)
	{
		LoadSave.sence = 0;
	}
	if(LoadSave.mode > 6)
	{
		LoadSave.mode = 0;
	}
	if(LoadSave.voltage > MAX_SET_VOLT)
	{
		LoadSave.voltage = 0;
	}
	if(LoadSave.current > MAX_SET_CURRENT)
	{
		LoadSave.current = 0;
	}
	if(LoadSave.risistence > MAX_SET_RES)
	{
		LoadSave.risistence = 0;
	}
	if(LoadSave.power > MAX_SET_POW)
	{
		LoadSave.power = 0;
	}
	if(LoadSave.ovp > MAX_SET_VOLT)
	{
		LoadSave.ovp = 0;
	}
	if(LoadSave.ocp > MAX_CURRENTPOOTE)
	{
		LoadSave.ocp = 0;
	}
	if(LoadSave.opp > MAX_POWERPOOTE)
	{
		LoadSave.opp = 0;
	}
	if(LoadSave.crange > 1)
	{
		LoadSave.crange = 0;
	}
	if(LoadSave.vrange > 1)
	{
		LoadSave.vrange = 0;
	}
	if(LoadSave.onvol > MAX_LoadVOLOTE)
	{
		LoadSave.onvol = 0;
	}
	if(LoadSave.offvol > MAX_FreeLoadVOLOTE)
	{
		LoadSave.offvol = 0;
	}
	LoadSave.crmode = 0;
	if(LoadSave.autooff > 999999)
	{
		LoadSave.offvol = 0;
	}
	if(LoadSave.maxv > MAX_SET_VOLT)
	{
		LoadSave.maxv = 0;
	}
	if(LoadSave.maxc > MAX_SET_CURRENT)
	{
		LoadSave.maxc = 0;
	}
	if(LoadSave.maxp > MAX_SET_POW)
	{
		LoadSave.maxp = 0;
	}
	if(LoadSave.crise > MAX_CURRENTUP)
	{
		LoadSave.crise = 0;
	}
	if(LoadSave.cdrop > MAX_CURRENTDOWN)
	{
		LoadSave.cdrop = 0;
	}
	if(LoadSave.cvdowntime > 3000)
	{
		LoadSave.cvdowntime = 0;
	}
	if(LoadSave.ledvo > MAX_SET_VOLT)
	{
		LoadSave.ledvo = 0;
	}
	if(LoadSave.ledio > MAX_SET_CURRENT)
	{
		LoadSave.ledio = 0;
	}
	if(LoadSave.ledrd > MAX_SET_RES)
	{
		LoadSave.ledrd = 0;
	}
	if(LoadSave.loadmode > 2)
	{
		LoadSave.loadmode = 0;
	}
	if(LoadSave.loadc1 > MAX_SET_CURRENT)
	{
		LoadSave.loadc1 = 0;
	}
	if(LoadSave.loadc2 > MAX_SET_CURRENT)
	{
		LoadSave.loadc2 = 0;
	}
	if(LoadSave.loadc3 > MAX_SET_CURRENT)
	{
		LoadSave.loadc3 = 0;
	}
	if(LoadSave.curvetime > 10)
	{
		LoadSave.curvetime = 0;
	}
	if(LoadSave.coffv1 > MAX_SET_VOLT)
	{
		LoadSave.coffv1 = 0;
	}
	if(LoadSave.coffv2 > MAX_SET_VOLT)
	{
		LoadSave.coffv2 = 0;
	}
	if(LoadSave.coffv3 > MAX_SET_VOLT)
	{
		LoadSave.coffv3 = 0;
	}
	if(LoadSave.loadr > MAX_SET_RES)
	{
		LoadSave.loadr = 0;
	}
	if(LoadSave.coffvr > MAX_SET_VOLT)
	{
		LoadSave.coffvr = 0;
	}
	if(LoadSave.testdis > 1)
	{
		LoadSave.testdis = 0;
	}
	if(LoadSave.valA > MAX_SET_CURRENT)
	{
		LoadSave.valA = 0;
	}
	if(LoadSave.valB > MAX_SET_CURRENT)
	{
		LoadSave.valB = 0;
	}
	if(LoadSave.timeA > 60000)
	{
		LoadSave.timeA = 0;
	}
	if(LoadSave.timeB > 60000)
	{
		LoadSave.timeB = 0;
	}
	if(LoadSave.dynaIRise > MAX_CURRENTUP)
	{
		LoadSave.dynaIRise = 0;
	}
	if(LoadSave.dynaIDrop > MAX_CURRENTUP)
	{
		LoadSave.dynaIDrop = 0;
	}
	if(LoadSave.dynamode > 1)
	{
		LoadSave.dynamode = 0;
	}
	if(LoadSave.qcmode > 1)
	{
		LoadSave.qcmode = 0;
	}
	if(LoadSave.Class_5 > 1)
	{
		LoadSave.Class_5 = 0;
	}
	if(LoadSave.Class_9 > 1)
	{
		LoadSave.Class_9 = 0;
	}
	if(LoadSave.Class_12 > 1)
	{
		LoadSave.Class_12 = 0;
	}
	if(LoadSave.Class_15 > 1)
	{
		LoadSave.Class_15 = 0;
	}
	if(LoadSave.Class_20 > 1)
	{
		LoadSave.Class_20 = 0;
	}
	if(LoadSave.vhigh > MAX_SET_VOLT)
	{
		LoadSave.vhigh = 0;
	}
	if(LoadSave.vlow > MAX_SET_VOLT)
	{
		LoadSave.vlow = 0;
	}
	if(LoadSave.chigh > MAX_SET_CURRENT)
	{
		LoadSave.chigh = 0;
	}
	if(LoadSave.clow > MAX_SET_CURRENT)
	{
		LoadSave.clow = 0;
	}
	if(LoadSave.phigh > MAX_SET_POW)
	{
		LoadSave.phigh = 0;
	}
	if(LoadSave.plow > MAX_SET_POW)
	{
		LoadSave.plow = 0;
	}
	if(LoadSave.limitdisp > 1)
	{
		LoadSave.limitdisp = 0;
	}
	if(LoadSave.limitbeep > 1)
	{
		LoadSave.limitbeep = 0;
	}
	if(LoadSave.vcomp > 1)
	{
		LoadSave.vcomp = 0;
	}
	if(LoadSave.ccomp > 1)
	{
		LoadSave.ccomp = 0;
	}
	if(LoadSave.pcomp > 1)
	{
		LoadSave.pcomp = 0;
	}
	if(LoadSave.language > 1)
	{
		LoadSave.language = 0;
	}
}

