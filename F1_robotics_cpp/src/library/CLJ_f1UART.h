/**********************************************************
 * @file   	CLJ_f1UART.h
 * @author 	Jonathan Chan - RBC Hardware team 2018
 * @contact 21800000 call me
 * @date   	3-March-2018
 * @brief  	This library is for HKUST Robocon Any f103 board
 * @version v1.0
 * @update
 *		2018/03/03 - first release
 * @Features
 *		1. either of Interrupt Mode or DMA Mode is fine. No need both
 * @Rules
 *		0. If you edit this library, please write documentary and leave your name here 
 *		1. Pinout : PA9-Tx, PA10-Rx as UART1
 *		2. No matter what case, UART_init() is needed
 *		2. you can edit UART_DMA_Tx_size & UART_Rx_size for the buffer size
 *		3. if you want to run in DMA mode, add "#define DMAmode" in this file. If you delete it, compiler will change it to interrupt mode
 *		4. DON'T run UART1_transmit() continuously in DMA mode, it needs some delay for UART perph to finish last transmission 
 *		6. UART_IT_RX_Buffer a circular buffer. UART_IT_RX_pt is the pointer for this 
 **********************************************************
 */
#ifndef __CLJ_f1UART_H
#define __CLJ_f1UART_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"

 /* variable that you can edit: */
#define UART1_DMAmode					//enable DMA mode or not. If you dont want DMA mode, comment this line
#define UART_DMA_Tx_size 64		//Maximum length of txData in DMA mode,the length of tx data array, final length is depends on data
#define UART_Rx_size 32				//Maximum length of RxData, the length of Rxceive data array
/* Don't edit the functions and variable below */
extern uint8_t UART_DMA_TX_Buffer[];	//this buffer should not be edited in other libraries(eg. main.c)
extern uint8_t UART_Rx_Buffer[];			//For both DMA or Interrupt mode, received data will store in this buffer. Please use it
extern uint8_t UART_IT_RX_pt;
extern uint8_t isUART1rxUpdated;	//debug use
void UART1_init(uint32_t _BaudRate);	//run the function at begin
void UART1_transmit(uint8_t *TxData, uint16_t len);	//enter Txdata and length of data
void UART1rx_Task(void);	//put this to main or where you want
	 
#ifdef __cplusplus
}
#endif
#endif	/* __CLJ_f1UART_H */
/* End of library */
