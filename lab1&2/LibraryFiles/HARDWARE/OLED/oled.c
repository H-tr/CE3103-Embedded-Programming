#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
		  
	
u8 OLED_GRAM[128][8];	 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    
		OLED_WR_Byte (0x00,OLED_CMD);      
		OLED_WR_Byte (0x10,OLED_CMD);        
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}

void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_RS_Set();   	  
} 
 /**************************************************************************
Turn On Display
**************************************************************************/  
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC Command
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
/**************************************************************************
Turn Off Display
**************************************************************************/  
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC Command
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}	
/**************************************************************************
Clear OLED
**************************************************************************/  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//Refresh
}
/**************************************************************************
Draw A Point
**************************************************************************/ 
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//Out of reach
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
/**************************************************************************
Show Char
**************************************************************************/
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	chr=chr-' ';				   
    for(t=0;t<size;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //1206 Size
		else temp=oled_asc2_1608[chr][t];		 //1608 Size	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

/**************************************************************************
Show Two Number
**************************************************************************/
void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
/**************************************************************************
Show The String
**************************************************************************/
void OLED_ShowString(u8 x,u8 y,const u8 *p)
{
#define MAX_CHAR_POSX 122
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,12,1);	 
        x+=8;
        p++;
    }  
}	 
/**************************************************************************
OLED Initization
**************************************************************************/	    
void OLED_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  //APB Peripheral Clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //SPI Port
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;      
	GPIO_Init(GPIOE, &GPIO_InitStructure);					      


	PWR_BackupAccessCmd(ENABLE); //Enable access to the RTC and Backup Register
	RCC_LSEConfig(RCC_LSE_OFF);  //LSE oscillator switch off to let PC13 PC14 PC15 be IO
	BKP_TamperPinCmd(DISABLE);   //disables the Tamper Pin activation, let PC13 be IO
	PWR_BackupAccessCmd(DISABLE);//disables access to the RTC and backup registers

	OLED_RST_Clr();
	delay_ms(100);
	OLED_RST_Set(); 
				  
	OLED_WR_Byte(0xAE,OLED_CMD); //Off Display
	
	OLED_WR_Byte(0xD5,OLED_CMD); //Set Oscillator Division
	OLED_WR_Byte(80,OLED_CMD);    //[3:0]: divide ratio of the DCLK, [7:4], set the oscillator frequency. Reset
	OLED_WR_Byte(0xA8,OLED_CMD); //multiplex ratio
	OLED_WR_Byte(0X3F,OLED_CMD); //duty = 0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD);  //set display offset
	OLED_WR_Byte(0X00,OLED_CMD); //0

	OLED_WR_Byte(0x40,OLED_CMD); //set display start line [5:0]- from 0-63. RESET
													
	OLED_WR_Byte(0x8D,OLED_CMD); //Set charge pump
	OLED_WR_Byte(0x14,OLED_CMD); //Enable Charge Pump
	OLED_WR_Byte(0x20,OLED_CMD); //Set Memory Addressing Mode
	OLED_WR_Byte(0x02,OLED_CMD); //Page Addressing Mode (RESET)
	OLED_WR_Byte(0xA1,OLED_CMD); //Set segment remap, bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //Set COM Output Scan Direction
	OLED_WR_Byte(0xDA,OLED_CMD); //Set COM Pins
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4] setting
	 
	OLED_WR_Byte(0x81,OLED_CMD); //Contrast Control
	OLED_WR_Byte(0xEF,OLED_CMD); //1~256; Default: 0X7F
	OLED_WR_Byte(0xD9,OLED_CMD); //Set Pre-charge Period
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //Set VCOMH
	OLED_WR_Byte(0x30,OLED_CMD);  //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //Enable display outputs according to the GDDRAM contents
	OLED_WR_Byte(0xA6,OLED_CMD); //Set normal display   						   
	OLED_WR_Byte(0xAF,OLED_CMD); //DISPLAY ON	 
	OLED_Clear();
}  



