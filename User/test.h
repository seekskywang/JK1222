
#ifndef __TEST_H
#define __TEST_H
//U22 µµŒªøÿ÷∆

//U13

//#define HW_HW_CY_MASK		((u8)((ClrD4&ClrD5&ClrD6)))
//#define HW_4051_C0			HW_HW_CY_MASK
//#define HW_4051_C1			((u8)(SetD6)))
//#define HW_4051_C2			((u8)(SetD5)))
//#define HW_4051_C3			((u8)(SetD5)|SetD6))
//#define HW_4051_C4			((u8)(SetD4)))
//#define HW_4051_C5			((u8)(SetD4)|SetD6))
//#define HW_4051_C6			((u8)(SetD4)|SetD5))
//#define HW_4051_C7			((u8)(SetD4)|SetD5|SetD6))
//#define HW_GYB_10V 			((u8)(SetD0|SetD7))
//#define HW_GYB_100V 		((u8)(SetD0))
//#define HW_GYB_1000V 		((u8)(ClrD0&ClrD7))
//#define HW_GYB_1000VFEED	((u8)(ClrD1&ClrD2))
//#define HW_GYB_100VFEED		((u8)(SetD1))
//#define HW_GYB_10VFEED		((u8)(SetD2))
//#define HW_GYBFD_ON			((u8)(SetD3))
//#define HW_GYBFD_OFF		((u8)(ClrD3))
//#define HW_HW_GYBDZ_MASK	((u8)((ClrD5&ClrD4)))
//#define HW_GYB_0R			((u8)(SetD4|SetD5))//≤‚ ‘
//#define HW_GYB_10KR			((u8)(ClrD4&ClrD5))//≤‚ ‘
//#define HW_GYB_100KR		((u8)(SetD4))//≤‚ ‘

extern u8 U15_4094,U16_4094;

void Internal_ResChange(u8 dat);
void V_Samping(u8 dat);
void Feedback_RY(u8 dat);
void Power_Process(void);
void LockProcess(void);
void Idle_Process(void);
void Setup_Process(void);
void Test_Process(void);
void TestAbort_Process(void);
void TestFinish_Process(void);
void Electro_discharge(u8 data);
void Electro_Test(u8 data);
void PRO_RY(u8 data);
void Use_DebugProcess(void);
void Clear_Process(void);
void Led_Process(void);
void Battery_Process(void);
void Dynamic_Process(void);
void Limit_Process(void);
void List_Process(void);
void lockcheck(void);
#endif


