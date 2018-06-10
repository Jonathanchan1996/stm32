/* Written by Jonathan Chan 
		for library 3.5 use
		Refer to blink without delay
		last Update:2018/03/18
*/
#ifndef __CLJ_LOOPSCHEDULE_H
#define __CLJ_LOOPSCHEDULE_H
#ifdef __cplusplus
 extern "C" {
#endif 
namespace loopsch{
	extern unsigned long current_us;
		 
	class looptime{
		public:
			looptime(unsigned long _interval);
			bool run(void);
		private:	
			volatile unsigned long last_conut;
			unsigned long interval;
	};
}
#ifdef __cplusplus
}
#endif

#endif	/*__CLJ_LOOPSCHEDULE_H */
