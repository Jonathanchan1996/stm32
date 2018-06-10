#ifndef __TICKS_H
#define __TICKS_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"

#include "stm32f10x.h" 

void ticks_init(void);
void ticks_reset(void);
u32 get_ticks(void);
void delay(u32 ms);
#ifdef __cplusplus
}
#endif
#endif	/* __TICKS_H */
