#include "CLJ_f1I2C.h"
#include "ticks.h"
void I2C2_init(uint32_t _CLKspd){
	// Initialization struct
	I2C_InitTypeDef I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// Step 1: Initialize I2C
	I2C_DeInit(I2Cx);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = _CLKspd;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2Cx, &I2C_InitStruct);
	I2C_Cmd(I2Cx, ENABLE);

	// Step 2: Initialize GPIO as open drain alternate function
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct.GPIO_Pin = I2C_PIN_SCL | I2C_PIN_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStruct);
}

void I2C2_address_direction(uint8_t address, uint8_t direction){
    // Send slave address
    I2C_Send7bitAddress(I2Cx, address, direction);

    // Wait for I2C EV6
    // It means that a slave acknowledges his address
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx,
            I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if (direction == I2C_Direction_Receiver)
    { 
        while (!I2C_CheckEvent(I2Cx,
            I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

uint8_t I2C2_receive_ack(void){
    // Enable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}

uint8_t I2C2_receive_nack(void){
    // Disable ACK of received data
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    // Wait for I2C EV7
    // It means that the data has been received in I2C data register
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read and return data byte from I2C data register
    return I2C_ReceiveData(I2Cx);
}


void I2C2_read(uint8_t address, uint8_t* data){
    //I2C2_start();
    I2C2_address_direction(address<<1 , I2C_Direction_Receiver);
    *data = I2C2_receive_nack();
    //I2C2_stop();
}
int I2C2_WriteArray(uint8_t address, uint8_t* data, uint8_t len){
	address = address<<1;
	/* Start */
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){};
  I2C_GenerateSTART(I2Cx, ENABLE);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	/* Send Address */
  I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Transmitter);
  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
		for(uint32_t i=0;i<0xff;i++){};
	/* Send data */
	while(len--) {
		I2C_SendData(I2Cx, *data++);
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	/* End */
	I2C_GenerateSTOP(I2Cx, ENABLE);
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
	/* Real End */
	return 1;
}
//void I2C_WriteNACK(uint8_t address, uint8_t data){
//	address = address<<1;
//	/* Start */
//	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY)){};
//  I2C_GenerateSTART(I2Cx, ENABLE);
//  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
//	/* Send Address */
//  I2C_Send7bitAddress(I2Cx, address, I2C_Direction_Transmitter);
//  while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
//		for(uint32_t i=0;i<0xff;i++){};
//	/* Send data */
//	I2C_SendData(I2Cx, data);
//	delay(1);
//	/* End */
//	I2C_GenerateSTOP(I2Cx, ENABLE);
//	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
//	/* Real End */
//}
/* End of library */
