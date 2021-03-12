#include "pbdata.h"
#include "bsp_exti.h"
#include "tm1650.h"

int main(void)
{  	
//	SetSystemStatus(SYS_STATUS_POWER);//开机上电状态
//    EXTI_Key_Config();
	SysTick_Init();
    g_LcdDirection=1;
    softswitch=0;
    Int_Pe3flag=0;
    SetSystemStatus(SYS_STATUS_POWER);
    GPIO_Configuration();//端口初始化
//	Beep_Off();
	while(1)
	{
        
		
		switch(GetSystemStatus())
		{
			case SYS_STATUS_POWER:
				Power_Process();//开机上电处理
				break;
			case SYS_STATUS_SETUP:
				Setup_Process();//设置处理
				break;
			case SYS_STATUS_TEST:
				Test_Process();//测试处理
				break;
			case SYS_STATUS_LED:
			{
				Led_Process();
			}break;
			case SYS_STATUS_BATTERY:
			{
				Battery_Process();
			}break;
			case SYS_STATUS_DYNAMIC:
			{
				Dynamic_Process();
			}break;
			case SYS_STATUS_LIMITSET://极限设置
				Limit_Process();
			break;
            case SYS_STATUS_SYSSET : //系统设置
                
                Use_SysSetProcess();
             break;
			case SYS_STATUS_DEBUG:
				Use_DebugProcess();//调试处理
			break;
			case SYS_STATUS_LIST:   //列表测试显示
				List_Process();
			break;

			
//            
//            case SYS_STATUS_SYS://系统信息显示
//				
//				Sys_Process();
//				break;
//            case SYS_STATUS_CLEAR:
//                Clear_Process();
//            case SYS_STATUS_TOOL://工具
//				Soft_Turnon();
			default:
//				Disp_Clr( );//清屏
				//Error_Process();//错误处理
				break;
		}
		

	}	
}









