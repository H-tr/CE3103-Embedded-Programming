#include "show.h"
int Show_Count = 1000;
int min = 0;
int second = 0;

void show_task(void *pvParameters)
{
   u32 lastWakeTime = getSysTickCnt();
   while(1)
    {	

			vTaskDelayUntil(&lastWakeTime, F2T(RATE_50_HZ));

			oled_show();    
    }
}  

void oled_show(void)
{  
     //To DO
	 // OLED_ShowString(10,10,"Hu Tianrun");
	
	 OLED_ShowNumber( 10, 25, min, 10, 12);
	 OLED_ShowNumber( 10, 50, second, 10, 12);
	 if (second < 59)
		 second++;
	 else {
		 second = 0;
		 min++;
	 }
	 vTaskDelay(Show_Count); //Delay 
	 //call some function in oled to display something
	 
	 OLED_Refresh_Gram(); //refresh the OLED RAM

	  		
	}

	void show_rec(u8* dis) {
		OLED_ShowString(10,10,dis);
	}

