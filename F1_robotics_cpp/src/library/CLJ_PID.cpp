#include <CLJ_PID.h>
#include <CLJ_f1TIMER.h>
#include <ticks.h>
void MotorPID::begin(double _K_p, double _K_i, double _K_d){
	TIM3_init();	//PWM
	TIM2_init();	//encoder
	MotorPID::K_p = _K_p;
	MotorPID::K_i = _K_i;
	MotorPID::K_d = _K_d;
	LastCNT = 0;
	setPWM(0);
}
void MotorPID::setPidPWM(long _speed){

}
void MotorPID::PIDLoop(){
	
}
int32_t MotorPID::getSpeed(){
	uint32_t ThisTicks = get_ticks();
	int32_t ThisCNT = getEncoder();
	rpm = ((LastCNT - ThisCNT)/100) / ((ThisTicks-LastTick));
	LastCNT = ThisCNT;
	LastTick = ThisTicks;
	return rpm;
}
/* End of library */
