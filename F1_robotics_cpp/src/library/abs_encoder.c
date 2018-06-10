#include "abs_encoder.h" 
void abs_encoder_init(void){
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	//for PB3, STD library default PB3 as JTag use. so this function is going to disable all JTag pin

	GPIO_InitTypeDef GPIO_InitStructure;
	/* Port B */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_Mode 	=  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6
																	|GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_10
																	|GPIO_Pin_11;;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Port C */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 	=  GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint16_t abs_encoder_read(void){	//return abs encoder gray code value
	uint16_t bits=0;
	bits = 
	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) <<9) |
	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) <<8) |
	(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) <<7) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) <<6) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) <<5) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)  <<4) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)  <<3) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)  <<2) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)  <<1) |
	(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)	 		);
	return bits;
}

/* End of library */
