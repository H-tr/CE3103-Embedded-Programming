#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include "system.h"

/*----------------------------------*/

const TickType_t xTimerPeriod = 5000;
const UBaseType_t uxAutoReload = pdTRUE;
void vCallbackFunction( TimerHandle_t xTimer );
#endif
