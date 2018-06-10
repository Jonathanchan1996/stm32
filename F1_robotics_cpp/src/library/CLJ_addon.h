#ifndef __CLJ_ADDON_H
#define __CLJ_ADDON_H

#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
namespace APP{
	void GPIO_TogglePin(GPIO_TypeDef* _GPIOx, uint16_t _GPIO_Pin);
	uint8_t ClockCheck(void);
	uint8_t chr2int(uint8_t _input);
	u16 gray2binary(uint16_t grayCodeValue);
}

int std::fputc(int ch, FILE *f); /* std printf */
#endif	/*__CLJ_ADDON_H */
