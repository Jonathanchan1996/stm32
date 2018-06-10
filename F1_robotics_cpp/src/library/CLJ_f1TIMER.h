#ifndef __CLJ_f1TIMER_H
#define __CLJ_f1TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
extern int32_t encoderCNT;
void TIM2_init(void);	//timer encoder
void TIM3_init(void);	//PWM timer
void setPWM(uint16_t _pwm);
int32_t getEncoder(void);
#ifdef __cplusplus
}
#endif
#endif	/* __CLJ_f1TIMER_H */
/* End of library */
