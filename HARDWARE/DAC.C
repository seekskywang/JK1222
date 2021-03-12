#include "pbdata.h"
void DAC_Configuration(void)
{
   DAC_InitTypeDef DAC_InitStructure;

   DAC_InitStructure.DAC_Trigger=DAC_Trigger_None;//不使用出发功能
   DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用三角波
   //屏蔽 幅值设置
   DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
   //关闭缓存
   DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;

   DAC_Init(DAC_Channel_1,&DAC_InitStructure);//初始化DAC通道1

   DAC_Cmd(DAC_Channel_1,ENABLE);//使能DAC1

   DAC_SetChannel1Data(DAC_Align_12b_R,0);//12位 右对齐
}
