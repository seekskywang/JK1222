/* Includes ------------------------------------------------------------------*/
#include "./lcd/bsp_lcd.h"
#include "./font/fonts.h"
#include "bsp_bmp.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* R(8bit) G(8bit) B(8bit)  --> RGB565  */
#define RGB24TORGB16(R,G,B)              ((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3))

#define M_FileAlignAdj(pFile)            (4 - (512 - ((uint32_t)((pFile)->fptr) & 0x01FF)) & 0x0003)  

/* �������Ҫ��ӡbmp��ص���ʾ��Ϣ,��printfע�͵�����
 * ��Ҫ��printf()���轫���������ļ���������
 */
//#define BMP_DEBUG_PRINTF(FORMAT,...)     printf(FORMAT,##__VA_ARGS__)	
#define BMP_BUFF_SIZE                    1024*2*10      //ͼƬ���ݻ�������С  

BYTE ColorData[BMP_BUFF_SIZE];      //���ջ�����
BYTE * pColorData = (BYTE *)ColorData;					/* ����ָ�򻺳�����ָ�룬���M_FileAlignAdj��ʹ�� */
UINT f_num;

uint8_t Screen_shot(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height, 
const char * filename)
{
	/* bmp  �ļ�ͷ 54���ֽ� */
	uint8_t header[54] =
  {
		0x42, 0x4d, 0, 0, 0, 0, 
		0, 0, 0, 0, 54, 0,
		0, 0, 40,0, 0, 0, 
		0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 24, 0, 
		0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 
		0, 0, 0
	};	
	uint16_t i;
	uint16_t j;
	uint32_t file_size;     
	uint16_t width;
	uint16_t height;
	uint8_t r,g,b;	
	uint32_t read_data;
	uint8_t kk[4]={0,0,0,0};	
  
	uint8_t ucAlign=Width%4;	
	
	/* ��*�� +������ֽ� + ͷ����Ϣ */
	file_size = Width * Height * 3 + Height*(Width%4) + 54;		

	/* �ļ���С 4���ֽ� */
	header[2] = file_size&0x000000ff;
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	width=Width;	
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) &0x000000ff;
	header[20] = (width >> 16) &0x000000ff;
	header[21] = (width >> 24) &0x000000ff;
	
	/* λͼ�� 4���ֽ� */
	height = Height;
	header[22] = height &0x000000ff;
	header[23] = (height >> 8) &0x000000ff;
	header[24] = (height >> 16) &0x000000ff;
	header[25] = (height >> 24) &0x000000ff;
  
	/* �½�һ���ļ� */	
	result = f_open(&file,filename,FA_CREATE_ALWAYS|FA_WRITE);

	if(result==FR_OK)
	{    
		/* ��Ԥ�ȶ���õ�bmpͷ����Ϣд���ļ����� */
		result = f_write(&file, header,54, &f_num);		
    for(i=0; i<Height; i++)	    
    {    	
      if(!(Width % 4))
      {         
        for(j=Width;j>0;j--)  
        {					
          read_data=LCD_ReadPixel(y+j, x+i);			
          
          r =  GETR_FROM_RGB16(read_data);
          g =  GETG_FROM_RGB16(read_data);
          b =  GETB_FROM_RGB16(read_data);

          result = f_write(&file, &b,1, &f_num);
          result = f_write(&file, &g,1, &f_num);
          result = f_write(&file, &r,1, &f_num);
        }
      }
      else
      {        
        for(j=Width;j>0;j--)   
        {					
          read_data = LCD_ReadPixel(y+j, x+i);				
          
          r =  GETR_FROM_RGB16(read_data);
          g =  GETG_FROM_RGB16(read_data);
          b =  GETB_FROM_RGB16(read_data);

          result = f_write(&file, &b,1, &f_num);
          result = f_write(&file, &g,1, &f_num);
          result = f_write(&file, &r,1, &f_num);
        }          
        if( ucAlign )				/* �������4�ֽڶ��� */
          result = f_write ( & file, kk, ucAlign, & f_num );
       }
    }/* ������� */  
		f_close(&file); 
		return 0;
	}	
	else if(result==FR_EXIST)  //����ļ��Ѿ�����
		return FR_EXIST;	 					//8
	else/* ����ʧ�� */
		return 1;
}