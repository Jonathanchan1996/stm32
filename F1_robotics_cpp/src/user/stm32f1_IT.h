#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H
/* THis library is mainly for any system error irq  
 *	Copy from HAL library
 *
 */
#ifdef __cplusplus
 extern "C" {
#endif 
	 
void HardFault_Handler(void);
void NMI_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */
