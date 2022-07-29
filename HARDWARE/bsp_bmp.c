/* Includes ------------------------------------------------------------------*/
#include "./lcd/bsp_lcd.h"
#include "./font/fonts.h"
#include "bsp_bmp.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* R(8bit) G(8bit) B(8bit)  --> RGB565  */
#define RGB24TORGB16(R,G,B)              ((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3))

#define M_FileAlignAdj(pFile)            (4 - (512 - ((uint32_t)((pFile)->fptr) & 0x01FF)) & 0x0003)  

/* 如果不需要打印bmp相关的提示信息,将printf注释掉即可
 * 如要用printf()，需将串口驱动文件包含进来
 */
//#define BMP_DEBUG_PRINTF(FORMAT,...)     printf(FORMAT,##__VA_ARGS__)	
#define BMP_BUFF_SIZE                    1024*2*10      //图片数据缓冲区大小  

BYTE ColorData[BMP_BUFF_SIZE];      //接收缓冲区
BYTE * pColorData = (BYTE *)ColorData;					/* 用于指向缓冲区的指针，配合M_FileAlignAdj宏使用 */
UINT f_num;

uint8_t Screen_shot(uint16_t x,uint16_t y,uint16_t Width,uint16_t Height, 
const char * filename)
{
	/* bmp  文件头 54个字节 */
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
	
	/* 宽*高 +补充的字节 + 头部信息 */
	file_size = Width * Height * 3 + Height*(Width%4) + 54;		

	/* 文件大小 4个字节 */
	header[2] = file_size&0x000000ff;
	header[3] = (file_size >> 8) & 0x000000ff;
	header[4] = (file_size >> 16) & 0x000000ff;
	header[5] = (file_size >> 24) & 0x000000ff;
	
	/* 位图宽 4个字节 */
	width=Width;	
	header[18] = width & 0x000000ff;
	header[19] = (width >> 8) &0x000000ff;
	header[20] = (width >> 16) &0x000000ff;
	header[21] = (width >> 24) &0x000000ff;
	
	/* 位图高 4个字节 */
	height = Height;
	header[22] = height &0x000000ff;
	header[23] = (height >> 8) &0x000000ff;
	header[24] = (height >> 16) &0x000000ff;
	header[25] = (height >> 24) &0x000000ff;
  
	/* 新建一个文件 */	
	result = f_open(&file,filename,FA_CREATE_ALWAYS|FA_WRITE);

	if(result==FR_OK)
	{    
		/* 将预先定义好的bmp头部信息写进文件里面 */
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
        if( ucAlign )				/* 如果不是4字节对齐 */
          result = f_write ( & file, kk, ucAlign, & f_num );
       }
    }/* 截屏完毕 */  
		f_close(&file); 
		return 0;
	}	
	else if(result==FR_EXIST)  //如果文件已经存在
		return FR_EXIST;	 					//8
	else/* 截屏失败 */
		return 1;
}