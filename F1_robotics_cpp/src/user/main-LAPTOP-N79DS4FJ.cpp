#define STM32F103_PCA9685_Dimmer
#include "main.h"
int i=0;
void begin(void){
	ticks_init();
	UART1_init(115200);	
	I2C2_init(200000);
	ClockCheck();
	int tem=0;
	//I2C2_read(0x41<<1,(uint8_t *)tem);
	pca9685_init(I2C2,0x41);
	//I2C2_WriteArray(0x41, (uint8_t*)"ABC", 3);
}
int main(void){
	begin();
	for(;;){
		while(r1ms->run()){
			if(r10ms->run()){
				
			}
			if(r100ms->run()){
				//I2C2_write(0x33,'E');
				//I2C2_WriteArray(0x41, (uint8_t*)"ABC", 3);
				if(i==4095)	i=0;
				pca9685_pwm(I2C2,0x41,0,i,4096-i);
				i++;
			}
			if(r200ms->run()){
				//pca1.setPWM(0, 1024, 3072);

				UART1_transmit((uint8_t*)"200ms\n", 6);
			}
			if(r1000ms->run()){

			}
		}
	}
}

/* END of file */
