#include "CLJ_f1SPI.h"

uint8_t SPIerror = 0;	//continous error. Error is 
uint8_t SPINumFault = 0;	//total Fault time. Fault means made MCU's SPI dead.
uint8_t SPI2_Rx_Buffer[SPI_size];
uint8_t SPI2_Tx_Buffer[SPI_size];
uint8_t isSPIgg = 1;
/* Private function */
void SPI2_DMA(void);
/* Public Function */
/**
  * @brief  SPI perph and pinout initialization
  */
void SPI2_init(void){	//Slave, No remapping pinout
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;	//interrupt	
	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_I2S_DeInit(SPI2);
	/* CS init */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//pull up, input mode
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);	//external interrupt use
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	/* SPI init */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
   
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL =  SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
	//SPI_CalculateCRC(SPI2, ENABLE);
  SPI_Cmd(SPI2, ENABLE);
//	if(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) != RESET){
//		SPI_I2S_ReceiveData(SPI2);
//		SPI_I2S_ClearFlag(SPI2, SPI_FLAG_CRCERR);
//  }
#ifdef SPI2_DMAmode	
	SPI2_DMA();
#endif
}  
#ifdef SPI2_DMAmode		
	//these functions are only enable in DMA mode. otherwise it will jam other DMA libraries(eg USART1) when UART DMA is not use 		
	//because UART1 and SPI2 shares same DMA channels
void SPI2_DMA(void){
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* NVIC for DMA */
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	/* Enable the DMA1_Channel4 & DMA1_Channel5 Interrupt */
	//SPI Rx
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//SPI Tx
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	/* DMA Clock enable*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// SPI2 Rx DMA Config
	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI2_Rx_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //perph to memory
	DMA_InitStructure.DMA_BufferSize = SPI_size; //customable
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	/* DMA1 Channel5 Configures for SPI2 Send */
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&SPI2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI2_Tx_Buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //memory to perph
	DMA_InitStructure.DMA_BufferSize = SPI_size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	// Enable perphs
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);
	// Interrupt of DMA for clear flag use 
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
	DMA_ClearITPendingBit(DMA1_IT_TC4);
	DMA_ClearITPendingBit(DMA1_IT_TC5);	
}
void DMA1_Channel4_IRQHandler(void){ 	//interrupt when DMA Transfer Complete - Tx of UART
	if(DMA_GetITStatus(DMA1_FLAG_TC4)){	//Clear flag after finish
    DMA_ClearFlag(DMA1_FLAG_GL4);			//GL = global flag
    DMA_Cmd(DMA1_Channel4, DISABLE);
  }
}
void DMA1_Channel5_IRQHandler(void){ 	//interrupt when DMA Transfer Complete - Rx of UART
	if(DMA_GetITStatus(DMA1_FLAG_TC5)){	//Clear flag after finish
    DMA_Cmd(DMA1_Channel5, DISABLE);
		DMA_ClearFlag(DMA1_FLAG_GL5);			//GL = global flag
		DMA1_Channel5->CNDTR = SPI_size;
		DMA_Cmd(DMA1_Channel5, ENABLE);
  }
}
#endif
uint8_t SPI2_transmit_1byte(uint8_t ch){	//For SPI full duplex mode, input transmitted data and return received data
	//transmit and receive simultaneously. 
	while ((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET));
	SPI_I2S_SendData(SPI2,ch);																						//set data into data register
	while ((SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)||		//delay until this transmission finish
				 (GPIO_ReadInputDataBit(SPI2_NSS_Port, SPI2_NSS_Pin)));					//when missing clock, stop SPI depends on CS pin
	return SPI_I2S_ReceiveData(SPI2);																			//get data from data register
}
void SPINssSelected(void){
#ifndef SPI2_DMAmode	//start commumication when is not dma mode 
	if(SPI2NssPinSelected()){
		for(int i=0;i<SPI_size;i++){
			SPI2_Rx_Buffer[i] = SPI2_transmit_1byte(SPI2_Tx_Buffer[i]);
		}
		resetSPIerror();
	}
	if(SPI2_Rx_Buffer[0]!=8){	//check error receive
		isSPIgg = 0;
//		setSPIerror();
//		if(getSPIerror()>=2){	//if Fault is acted
//			SPI_I2S_DeInit(SPI2);
//			SPI2_init();
//			setSPIfault();
//		}
	}
	else isSPIgg = 1;
#endif
}
/* Debug Functions */
void resetSPIerror(void){
	SPIerror=0;
}
void setSPIerror(void){
	SPIerror++;
}
uint8_t getSPIerror(void){
	return SPIerror;
}
void setSPIfault(void){
	SPINumFault++;
}

uint8_t getSPIfault(void){
	return SPINumFault;
}
/* End of library */
