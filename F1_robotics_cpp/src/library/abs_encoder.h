/*
Encoder: 	Bit0...Bit9 total 10 pin
Pin:			[PB3][PB4][PB6][PB7][PB9] [PB10][PB11][PC13][PC14][PC15]
 the abs encoder we use are in gray code
*/

#ifndef __ABS_ENCODER_H
#define __ABS_ENCODER_H

#ifdef __cplusplus
 extern "C" {
#endif 
#include "stm32f10x.h"
void abs_encoder_init(void);
uint16_t abs_encoder_read(void);
#ifdef __cplusplus
}
#endif
#endif	/* __CLJ_ABS_ENCODER_H */
/* End of library */
