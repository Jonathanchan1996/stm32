#define STM32F103_PCA9685_Dimmer
#include "ticks.h"
#include <stdio.h>
#include <stdlib.h>
#include "CLJ_f1SPI.h"
#include "CLJ_f1UART.h"
#include "CLJ_f1TIMER.h"

void GPIO_TogglePin(GPIO_TypeDef* _GPIOx, uint16_t _GPIO_Pin);
void GPIO_init(void);
volatile uint32_t previousMS = 0;
volatile uint32_t previous10ms = 0;
volatile uint32_t previous100ms = 0;
volatile uint32_t previous200ms = 0;
uint8_t isSPIupdated=0;
uint8_t motorDir = 0;
uint16_t motorSpeed=0;
void begin(void){
	ticks_init();
	UART1_init(115200);
	RCC_ClocksTypeDef rcc_clocks;
  RCC_GetClocksFreq(&rcc_clocks);
	printf("APB1: %u\n", rcc_clocks.PCLK1_Frequency);
	printf("APB2: %u\n", rcc_clocks.PCLK2_Frequency);
	printf("STM Start\n");
}
int main(void){
	begin();
	for(;;){
		/* NOTE: sysTicks changed to update every 1ms */
		uint32_t currentTime = get_ticks();
		if(currentTime !=previousMS){
			previousMS = currentTime;
			if(currentTime - previous10ms>10){
				previous10ms = currentTime;
				
			}
			if(currentTime - previous100ms>100){
				previous100ms = currentTime;

			}
			if(currentTime - previous200ms>200){
				previous200ms = currentTime;
				UART1_transmit("Jonathan\n", 9);
				UART1_transmit("1234567890\n", 11);
			}
		}
	}
}
/* System Function */
int fputc(int ch, FILE *f) {	//For printf-function low level setup
	USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));			//delay until finish sending present char
  return (ch);
	//return -1;
}
void GPIO_TogglePin(GPIO_TypeDef* _GPIOx, uint16_t _GPIO_Pin){	//Additional function for GPIO
	if((_GPIOx->IDR & _GPIO_Pin) != (uint32_t)Bit_RESET)
		_GPIOx->BRR  |= _GPIO_Pin;
	else
		_GPIOx->BSRR |= _GPIO_Pin;
}

/* END of file */
