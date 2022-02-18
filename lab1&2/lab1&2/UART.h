#ifndef __UART_H
#define __UART_H
#include "sys.h"
#include "system.h"

//UART Port definition
#define UART_TASK_PRIO	3    
#define UART_STK_SIZE 		128   
/*----------------------------------*/

void uart_task(void *pvParameters);
void uart_receive_task(void *pvParameters);
void uart_init(u32 baudRate);
void usart3_send(u8 data);
int USART3_IRQHandler(void);

#endif
