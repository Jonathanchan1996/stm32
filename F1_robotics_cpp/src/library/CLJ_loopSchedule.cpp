#include "CLJ_loopSchedule.h"
#include "ticks.h"
namespace loopsch{
	looptime::looptime(unsigned long _interval){
		interval = _interval;
	}

	bool looptime::run(void){
		//unsigned long current_ms = get_ticks();
		if((get_ticks() - last_conut) >= interval){
			last_conut = get_ticks();
			return 1;
		}
		else return 0;
	}
}
