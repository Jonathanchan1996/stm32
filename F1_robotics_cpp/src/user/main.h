#ifndef __MAIN_H
#define __MAIN_H
/* Place for including and config */
/* standard C libraries */
#include <stdio.h>
#include <stdlib.h>
using namespace std;
/* CLJ function */
#include "stm32f1_IT.h"
#include "ticks.h"	//time
#include "CLJ_f1UART.h"
//#include "CLJ_f1I2C.h"
#include "CLJ_loopSchedule.h"
#include <CLJ_f1TIMER.h>
using namespace loopsch;
#include "CLJ_addon.h"
using namespace APP;
//#include "abs_encoder.h"	//<-Robotics team use
#include "CLJ_PID.h"

/* addon Function */

/* initial items */
//sequence schedule
looptime *r1ms		= new looptime(1);
looptime *r10ms 	= new looptime(10);
looptime *r100ms  = new looptime(100);
looptime *r200ms  = new looptime(200);
looptime *r1000ms = new looptime(1000);
//PID
MotorPID MotorA;
#endif	/* __MAIN_H */
