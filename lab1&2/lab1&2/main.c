#include "system.h"

#define START_TASK_PRIO		3
#define START_STK_SIZE		256
TaskHandle_t StartTask_Handler;
TimerHandle_t xTimer;

void start_task(void *pvParameters);
int USART3_IRQHandler(void);


int main(void)
{
	xTimer = xTimerCreate("timer", 5000, pdTRUE, 0, led_task);
	
	systemInit();
	
	xTimerStart(xTimer, 0);
	
	xTaskCreate((TaskFunction_t )start_task,
							(const char*		)"start_task",
							(uint16_t       )START_STK_SIZE,
							(void*					)NULL,
							(UBaseType_t		)START_TASK_PRIO,
							(TaskHandle_t*	)&StartTask_Handler);
	vTaskStartScheduler();
	
}

void start_task(void *pbParameters)
{
	// const char* data = "You are wonderful!";
	
	taskENTER_CRITICAL();
	
	// xTaskCreate(led_task, "led_task", LED_STK_SIZE, NULL, LED_TASK_PRIO, NULL);
	
	// xTaskCreate(buzzer_task, "buzzer_task", BUZZER_STK_SIZE, NULL, BUZZER_TASK_PRIO, NULL);
	
	xTaskCreate(show_task, "show_task", SHOW_STK_SIZE, NULL, SHOW_TASK_PRIO, NULL);
	
	xTaskCreate(button_task, "button_task", BUTTON_STK_SIZE, NULL, BUTTON_TASK_PRIO, NULL);
	
	xTaskCreate(uart_task, "uart_send", UART_STK_SIZE, NULL, UART_TASK_PRIO, NULL);
	
	vTaskDelete(StartTask_Handler);
	
	taskEXIT_CRITICAL();
	
}

int USART3_IRQHandler(void)
{
	static u8 Count=0, i=0;
	u8 Usart_Receive;
	static u8 rxbuf[10];
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		Usart_Receive = USART_ReceiveData(USART3);
		  // USART_Receive is the received data
		if (Usart_Receive <= '9' && Usart_Receive >= '0')
			xTimerChangePeriod(xTimer, (Usart_Receive-'0')*1000, 100);
		
		rxbuf[i++ % 10] = Usart_Receive;
		// OLED_ShowString(10,10, "Hello!");
	}
	show_rec(rxbuf);
	return 0;
}