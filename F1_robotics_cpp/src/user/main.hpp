#ifndef __MAIN_HPP
#define __MAIN_HPP


/* standard C language*/
#include <stdio.h>
#include <stdlib.h>
using namespace std;
/* Jonathan's function */
#include "stm32f1_it.h"
#include "ticks.h"	//time
#include "CLJ_f1SPI.h"
#include "CLJ_f1UART.h"
#include "CLJ_f1TIMER.h"
#include "CLJ_loopSchedule.h"
using namespace loopsch;
void GPIO_TogglePin(GPIO_TypeDef* _GPIOx, uint16_t _GPIO_Pin);
void GPIO_init(void);
/* initial items */
looptime r1ms,r10ms,r100ms,r200ms;
#endif	/* __MAIN_HPP */
