#include "CLJ_addon.h"
#include "ticks.h"
/* addon Function */

void APP::GPIO_TogglePin(GPIO_TypeDef* _GPIOx, uint16_t _GPIO_Pin){	//Additional function for GPIO
	if((_GPIOx->IDR & _GPIO_Pin) != (uint32_t)Bit_RESET)
		_GPIOx->BRR  |= _GPIO_Pin;
	else
		_GPIOx->BSRR |= _GPIO_Pin;
}
uint8_t APP::ClockCheck(void){
	RCC_ClocksTypeDef rcc_clocks;
	/* Clk check */
  RCC_GetClocksFreq(&rcc_clocks);
#ifdef debug
	printf("\nAPB1: %u\n", rcc_clocks.PCLK1_Frequency);
	printf("APB2: %u\n", rcc_clocks.PCLK2_Frequency);
#endif
	if(rcc_clocks.PCLK1_Frequency == 36000000||rcc_clocks.PCLK2_Frequency == 72000000){
		return 1;
	}
	else{return 0;}
	
}
uint8_t APP::chr2int(uint8_t _input){
	if(_input>=0x30&&_input<=0x39){
		_input = _input - 0x30;
	}
	else if(_input>=0x41&&_input<=0x46){
		_input = _input - 0x40 + 9;
	}
	else if(_input>=0x61&&_input<=0x66){
		_input = _input - 0x60 + 9;
	}
	else {
		_input = 0;
	}
	return _input;
}
u16 APP::gray2binary(u16 grayCodeValue){
	u16 binary=0;
	for(int i=16;i>0;i--){
		binary |= ((binary&(1<<i))>>1)^(grayCodeValue&(1<<(i-1)));
	}
	return binary;
}

int std::fputc(int ch, FILE *f) {	//For printf-function low level setup
	USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));			//delay until finish sending present char
  return (ch);
	//return -1;
}

/* End of library */
