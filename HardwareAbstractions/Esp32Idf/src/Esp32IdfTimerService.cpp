#include <stdint.h>
#include <math.h>
#include "CallBack.h"
#include "Esp32IdfTimerService.h"
#include "Esp32IdfConf.h"

using namespace EmbeddedIOServices;

namespace Esp32
{	
	Esp32IdfTimerService::Esp32IdfTimerService(timer_group_t group_num, timer_idx_t timer_num) :
		dev(TIMER_LL_GET_HW(group_num)),
		idx(timer_num)
	{	
		timer_config_t config;
		config.divider = 2;
		config.counter_dir = TIMER_COUNT_UP;
		config.counter_en = TIMER_START;
		config.alarm_en = TIMER_ALARM_DIS;
		config.auto_reload = TIMER_AUTORELOAD_DIS;
		config.intr_type = TIMER_INTR_LEVEL;
		timer_init(group_num, timer_num, &config);

		timer_isr_register(group_num, timer_num, [](void *args) { static_cast<Esp32IdfTimerService *>(args)->TimerInterrupt(); }, (void *)this, 0, &timer_isr_handle);

		Calibrate();
	}

	tick_t Esp32IdfTimerService::GetTick()
	{
		uint64_t ret;
		timer_ll_get_counter_value(dev, idx, &ret);
		return static_cast<tick_t>(ret);
	}

	void Esp32IdfTimerService::ScheduleCallBack(const tick_t tick)
	{
		printf( "get tick\n\r");
		uint64_t cnt;
		timer_ll_get_counter_value(dev, idx, &cnt);
		printf( "set alarm\n\r");
		// timer_ll_set_alarm_value(dev, idx, cnt + (tick - static_cast<tick_t>(cnt)));
		timer_ll_set_alarm_value(dev, idx, (cnt & ((~static_cast<uint64_t>(0)) << (sizeof(tick_t) * 8))) | tick);
		printf( "enable\n\r");
		timer_ll_set_alarm_enable(dev, idx, TIMER_ALARM_EN);
		printf( "after\n\r");
	}
	
	void Esp32IdfTimerService::TimerInterrupt(void)
	{
		timer_ll_clear_intr_status(dev, idx);
		ReturnCallBack();
	}

	tick_t Esp32IdfTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}