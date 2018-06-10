#ifndef __CLJ_f1I2C_H
#define __CLJ_f1I2C_H

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#define I2C_TIMEOUT_MAX 100000
#define I2Cx I2C2
#define I2C_GPIO GPIOB
#define I2C_PIN_SCL GPIO_Pin_10
#define I2C_PIN_SDA GPIO_Pin_11
	 
void I2C2_init(uint32_t _CLKspd);

void I2C2_address_direction(uint8_t address, uint8_t direction);
uint8_t I2C2_receive_ack(void);
uint8_t I2C2_receive_nack(void);
void I2C2_read(uint8_t address, uint8_t* data);
int I2C2_WriteArray(uint8_t address, uint8_t* data, uint8_t len);
//void I2C_WriteNACK(uint8_t address, uint8_t data);
#ifdef __cplusplus
}
#endif
#endif	/* __CLJ_f1I2C_H */
/* End of library */
