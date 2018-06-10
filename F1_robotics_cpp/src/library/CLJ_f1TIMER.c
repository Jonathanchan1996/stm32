#include "CLJ_f1TIMER.h"
int32_t encoderCNT = 0;

void TIM2_init(void){				//Encoder mode
	  GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;      

    //PA0 as ch1 ,PA1 as ch2 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);                           

    TIM_DeInit(TIM2);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 3999; // maximum count according to gungir
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 		//1:1 when Prescaler is zero
    TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);              

    TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI1, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 0; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    //Reset counter
    TIM2->CNT = 0;//

    TIM_Cmd(TIM2, ENABLE);   //??TIM4???
	
}
void TIM3_init(void){				//PWM generation mode
	GPIO_InitTypeDef port;
	TIM_TimeBaseInitTypeDef timer;
	TIM_OCInitTypeDef timerPWM;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* IO initi */
	GPIO_StructInit(&port);
	port.GPIO_Mode = GPIO_Mode_AF_PP;
	port.GPIO_Pin = GPIO_Pin_0;
	port.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &port);
	/* Timer initi */
	TIM_TimeBaseStructInit(&timer);
	timer.TIM_Prescaler = 0;
	timer.TIM_Period = 719;
	timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &timer);
	/* PWM initi */
	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_Pulse = 0;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	//timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC3Init(TIM3, &timerPWM);
	TIM_OC3PreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
  TIM_CtrlPWMOutputs(TIM3,ENABLE);
}
void setPWM(uint16_t _pwm){		//PWM update 
	/* edit pwm for tim3 ch3 */
	TIM_OCInitTypeDef timerPWM;
	TIM_OCStructInit(&timerPWM);
	timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
	timerPWM.TIM_OutputState = TIM_OutputState_Enable;
	timerPWM.TIM_Pulse = _pwm;
	timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
	//timerPWM.TIM_OutputState = TIM_OutputState_Enab;
	TIM_OC3Init(TIM3, &timerPWM);
	TIM_OC3PreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
  TIM_CtrlPWMOutputs(TIM3,ENABLE);
	
}
int32_t getEncoder(void){
	encoderCNT += (int16_t)TIM2->CNT;	//2's complement to decimal
	TIM2->CNT = 0;
	return encoderCNT;
}

/* End of library */
