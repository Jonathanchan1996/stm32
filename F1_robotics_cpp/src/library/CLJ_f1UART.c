#include "CLJ_f1UART.h"
#include "stm32f10x_dma.h"

uint8_t UART_DMA_TX_Buffer[UART_DMA_Tx_size];		//buffer to contain transmit data
uint8_t UART_Rx_Buffer[UART_Rx_size];						//both DMA or Interrupt mode received data will store in this buffer
uint8_t UART_IT_RX_pt  = 0; 										//pointer for circlar buffer(UART_Rx_Buffe) in IT mode
uint8_t isUART1rxUpdated = 0;
/* Private Function */
void UART1_IT(void);
void UART1_DMA(void);
/* Public Function */
/**
  * @brief  Uart perph and pinout initialization
  */
void UART1_init(uint32_t _BaudRate){
	/* UART1 init */
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);	//enable APB clock
	/* TX pin initi */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//update config
	/* RX pin initi */
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//update config
	/* UART perph ininti */
	USART_InitTypeDef USART_InitStructure;					//enable UART
	USART_InitStructure.USART_BaudRate 	 = _BaudRate;	//setable
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits	 = USART_StopBits_1;
	USART_InitStructure.USART_Parity	   = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 			 = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
#ifdef UART1_DMAmode
	UART1_DMA();	//enable DMA mode only when UART1_DMAmode defined 
#else
	UART1_IT();		//run IT mode only when NOT DMA mode
#endif
}
void UART1_IT(void){
	/* USART1 interrupt setup. What happen will launch interrupt */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//interrupt when receive
  /* Enable USART1 global interrupt */
  NVIC_EnableIRQ(USART1_IRQn);
}
void UART1_DMA(void){
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/* NVIC for DMA */
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	/* Enable the DMA Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd 							 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	/* DMA Clock enable*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  // USART Tx DMA Config
  DMA_DeInit(DMA1_Channel4);
  DMA_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t)&USART1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr 			= (uint32_t)UART_DMA_TX_Buffer;
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralDST; //memory to perph
  DMA_InitStructure.DMA_BufferSize 					= UART_DMA_Tx_size;	//customable
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Normal;	//For Tx, normal mode is fine
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M 								= DMA_M2M_Disable;
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  // USART Rx DMA Config
	DMA_DeInit(DMA1_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&USART1->DR; 
  DMA_InitStructure.DMA_MemoryBaseAddr 			= (uint32_t)UART_Rx_Buffer;
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralSRC; //perph to memory
  DMA_InitStructure.DMA_BufferSize 					= UART_Rx_size;
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M 								= DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	// Enable perphs
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	// Interrupt of DMA for clear flag use 
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);	//DMA TC(Transfer Complete) interrupt
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);	//Interrupt when receive buffer is full
}
/**
  * @brief  Transmits data by dma Mode or Normal Mode
  */
void UART1_transmit(uint8_t *TxData, uint16_t len){
#ifdef UART1_DMAmode
	/* These codes will run in DMA mode */
	#define txDMAChannel DMA1_Channel4
	while(DMA_GetITStatus(DMA1_FLAG_TC4)&!(USART1->SR & USART_FLAG_TXE));
	for(uint16_t i=0;i<len;i++){
		UART_DMA_TX_Buffer[i] = TxData[i];
	}

	/* memory to DMA */
	txDMAChannel->CMAR 	= (uint32_t)UART_DMA_TX_Buffer;	//put buffer to dma
  txDMAChannel->CNDTR = len;				//data size, how many data will be tranmitted this time
  DMA_Cmd(txDMAChannel, ENABLE);		//send to dma
#else
	/* These code will run in Normal mode */
	for(uint16_t i=0;i<len;i++){
		USART_SendData(USART1, (unsigned char) TxData[i]);
		while (!(USART1->SR & USART_FLAG_TXE));			//delay until finish sending present char
	}
#endif
}
#ifdef UART1_DMAmode	//these functions are only enable in DMA mode. otherwise it will jam other DMA libraries(eg SPI2) when UART DMA is not use 
											//because UART1 and SPI2 shares same DMA channels
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
		DMA1_Channel5->CNDTR = UART_Rx_size;
		DMA_Cmd(DMA1_Channel5, ENABLE);
  }
	UART1rx_Task();
}
#else
void USART1_IRQHandler(void) {				//UART interrupt mode use
	/* RXNE handler */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		uint8_t _temp = USART_ReceiveData(USART1);
		UART_Rx_Buffer[UART_IT_RX_pt] = _temp;
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){};
		UART_IT_RX_pt++; 		//pointer increment 
		if(UART_IT_RX_pt>=UART_Rx_size){
			UART_IT_RX_pt = 0; //reset pointer to zero
		}
		//isUART1rxUpdated = 1;
		/* Application level */
		if(_temp == '~'){
			UART1rx_Task();
		}
	}

}
#endif
/* End of library */
