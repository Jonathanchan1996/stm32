/* STM32F103_temp
*	@author Jonathan CCHan
*/
#define debug
#include "main.h"
uint8_t u8tenc[2];	//encoder reading in two 8bit
uint16_t u16enc;	//encoder reading in 16bit
uint16_t lastenc;	//last value
int32_t testing_rpm = 0;
void GPIO_init(void);
void begin(void){
	ticks_init();
	UART1_init(115200);
	//ClockCheck();	//uart will return someting if u define "debug"
	GPIO_init();	//LED
	//MotorA.begin(1,0,0);
	TIM3_init();	//PWM
	TIM2_init();	//encoder
}
int main(void){
	begin();
	for(;;){
		while(r1ms->run()){
			if(r10ms->run()){

			}
			if(r100ms->run()){
				
			}
			if(r200ms->run()){
				APP::GPIO_TogglePin(GPIOA, 1<<2);	//LEDA	
				APP::GPIO_TogglePin(GPIOB, 1<<8);	//LEDB
				//testing_rpm = MotorA.getSpeed();
				testing_rpm = getEncoder();
				//APP::GPIO_TogglePin(GPIOE, 1<<1);
			}
			if(r1000ms->run()){
				
				UART1_transmit((uint8_t*)"1000ms\n", 7);
				APP::GPIO_TogglePin(GPIOC, (1|1<<1|1<<2|1<<3));
				
			}
		}
	}
}
void UART1rx_Task(void){	//work in both DMA and interrupt mode
	/*  
		IT mode:This function will run only when received '~' 
			protocal:
				[data*n][~]
		DMA mode:
			only run when receive_array full
	*/
//	u16enc = abs_encoder_read();
//	u8tenc[0] = u16enc>>8;
//	u8tenc[1] = u16enc;
	UART1_transmit((uint8_t*)"received\n", 9);
}

void GPIO_init(void){	//LED use
		/* LED A pin initi */
	RCC->APB2ENR |= RCC_APB2Periph_GPIOA;
	GPIOA->CRL	 &= 0xfffff0ff;	//CRL for pin0 to pin7
	GPIOA->CRL	 |= 0x00000300;	//GPIO_Mode_Out_PP, GPIO_Speed_50MHz
	GPIOA->BRR   |= 1<<2;				//reset pin
	/* LED B pin initi*/
	RCC->APB2ENR |= RCC_APB2Periph_GPIOB;
	GPIOB->CRH 	 &= 0xfffffff0;	//CRH for pin8 to pin15
	GPIOB->CRH	 |= 0x00000003; //PB13 - Input with pull-up / pull-down, Input mode (reset state)
	GPIOB->BRR	 |= 1<<8;			//set low
	/* F103VET-2018 controller testing use */
	RCC->APB2ENR |= RCC_APB2Periph_GPIOC;
	GPIOC->CRL 	 &= 0xffff0000;	
	GPIOC->CRL	 |= 0x00003333;
	GPIOC->BRR	 |= (1|1<<1|1<<2|1<<3);	
	
	RCC->APB2ENR |= RCC_APB2Periph_GPIOE;	
	GPIOE->CRL 	 &= 0xffffff0f;	
	GPIOE->CRL	 |= 0x00000030;
	GPIOE->BRR	 |= 1<<1;
}
/* END of file */
