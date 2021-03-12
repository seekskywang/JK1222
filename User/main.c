#include "pbdata.h"
#include "bsp_exti.h"
#include "tm1650.h"

int main(void)
{  	
//	SetSystemStatus(SYS_STATUS_POWER);//�����ϵ�״̬
//    EXTI_Key_Config();
	SysTick_Init();
    g_LcdDirection=1;
    softswitch=0;
    Int_Pe3flag=0;
    SetSystemStatus(SYS_STATUS_POWER);
    GPIO_Configuration();//�˿ڳ�ʼ��
//	Beep_Off();
	while(1)
	{
        
		
		switch(GetSystemStatus())
		{
			case SYS_STATUS_POWER:
				Power_Process();//�����ϵ紦��
				break;
			case SYS_STATUS_SETUP:
				Setup_Process();//���ô���
				break;
			case SYS_STATUS_TEST:
				Test_Process();//���Դ���
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
			case SYS_STATUS_LIMITSET://��������
				Limit_Process();
			break;
            case SYS_STATUS_SYSSET : //ϵͳ����
                
                Use_SysSetProcess();
             break;
			case SYS_STATUS_DEBUG:
				Use_DebugProcess();//���Դ���
			break;
			case SYS_STATUS_LIST:   //�б������ʾ
				List_Process();
			break;

			
//            
//            case SYS_STATUS_SYS://ϵͳ��Ϣ��ʾ
//				
//				Sys_Process();
//				break;
//            case SYS_STATUS_CLEAR:
//                Clear_Process();
//            case SYS_STATUS_TOOL://����
//				Soft_Turnon();
			default:
//				Disp_Clr( );//����
				//Error_Process();//������
				break;
		}
		

	}	
}









