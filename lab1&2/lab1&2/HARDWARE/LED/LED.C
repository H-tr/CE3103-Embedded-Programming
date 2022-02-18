#include "led.h"

int Led_Count = 5000;//LED blink Delay 

//LED initialization
void LED_Init(void)
{
	//Led_Count = 5;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //Enable APB Clock
	
	GPIO_InitStructure.GPIO_Pin = LED_PIN;				//LED Pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//Push pull output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(LED_PORT, &GPIO_InitStructure);			//Initialize LED GPIO
	GPIO_SetBits(LED_PORT,LED_PIN);
}

void led_task(TimerHandle_t xTimer)
{
	LED=~LED;
}  

