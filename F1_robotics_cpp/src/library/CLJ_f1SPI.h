#include "stm32f10x.h"
 /**********************************************************
 * @file   	CLJ_f1SPI.h
 * @author 	Jonathan Chan - RBC Hardware team 2018
 * @contact 81777778
 * @date   	3-March-2018
 * @brief  	This library is for HKUST Robocon Any f103 board. Pinouts follows Motor Controller V5.1
 * @version v1.0
 * @update
 *		2018/03/03 - first release
 * @Features
 *		1. either of Interrupt Mode or DMA Mode is fine. No need both
 * 		2. Slave mode
 * @Rules
 *		0. If you edit this library, please write documentary and leave your name here 
 *		1. By experiment, 2 errors would make SPI perph die(Fault).
 *		2. CPOL=1, CPHA=1	<-theoretically, no different. By experiment, this setup is more stable
 *		3. use HARDWARE NSS
 *		4. By experiment, GPIO interrupt(chip select) is fine, no need SPI interrupt for multiple slaves cicuit
 * @notes
 *		2018/03/03:
 *		- For SPI interrupt Mode, interrupt by detecting Clock. Even if the chip is not selected, 
 *			it would be interrupted as selected
 *		- According to Reference manual page 709 - 25.3.8 if SPI run in interrupt mode, 
 *			when Chip is selected,set SPE bit(SPI Enable). When Chip is deselected, reset SPE to disable SPI perph.  
 **********************************************************
 */
/* variable that you can edit: */
//#define SPI2_DMAmode		//enable DMA mode or not. If you dont want DMA mode, comment this line
#define SPI_size 8			//recall a rule of SPI, exchange data, so No. of TxData = No. of RxData 
extern uint8_t SPI2_Rx_Buffer[SPI_size];	//Buffer contain Received data
extern uint8_t SPI2_Tx_Buffer[SPI_size];	//Buffer contain Transmitted data
#define SPI2_NSS_Port GPIOB
#define SPI2_NSS_Pin GPIO_Pin_12
/* Don't edit the functions and variable below: */
extern uint8_t isSPIgg;
/* Functions you can use */
void SPI2_init(void);
uint8_t SPI2_transmit_1byte(uint8_t ch);	//include SPI_I2S_SendData(), delay and check functions 
#define SPI2NssPinSelected() !GPIO_ReadInputDataBit(SPI2_NSS_Port, SPI2_NSS_Pin)	//chip select pin(NSS)
/* Functions for debug */
void resetSPIerror(void);
void setSPIerror(void);
uint8_t getSPIerror(void);
void setSPIfault(void);			//add 1 to total time of fault
uint8_t getSPIfault(void);	//return how many time of fault
void SPINssSelected(void);	//put this function in EXTI15_10_IRQHandler()

/* End of Library */
