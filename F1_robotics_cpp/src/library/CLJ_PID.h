#ifndef __CLJ_PID_H
#define __CLJ_PID_H
#ifdef __cplusplus
 extern "C" {
#endif 
#include "stm32f10x.h"
class MotorPID{
	public:
		void begin(double _K_p, double _K_i, double _K_d);
		void setPidPWM(long _speed);
		void PIDLoop();
		int32_t getSpeed();
		//bool run(void);
	private:	
		int32_t encoderCNT;

		int32_t rpm;
		int32_t K_p;
		int32_t K_i;
		int32_t K_d;
		int32_t LastCNT;
		uint32_t LastTick;
		
};

#ifdef __cplusplus
}
#endif

#endif	/*__CLJ_PID_H */
