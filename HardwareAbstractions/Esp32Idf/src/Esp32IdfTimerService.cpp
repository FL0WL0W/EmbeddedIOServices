#include <stdint.h>
#include <math.h>
#include "CallBack.h"
#include "Esp32IdfTimerService.h"
#include "esp_attr.h"

using namespace EmbeddedIOServices;

namespace Esp32
{	
	bool IRAM_ATTR RAMTimerInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
	{
		static_cast<Esp32IdfTimerService *>(user_data)->TimerInterrupt();
		return true;
	}

	Esp32IdfTimerService::Esp32IdfTimerService(uint32_t group_num, uint32_t timer_num)
	{
		gptimer_config_t timer_config = {
			.clk_src = GPTIMER_CLK_SRC_DEFAULT,
			.direction = GPTIMER_COUNT_UP,
			.resolution_hz = 40000000, // 40MHz, 1 tick=25ns
		};
		gptimer_new_timer(&timer_config, &gptimer);

		gptimer_event_callbacks_t cbs = {
			.on_alarm = RAMTimerInterrupt,
		};
		gptimer_register_event_callbacks(gptimer, &cbs, this);

		gptimer_enable(gptimer);
		gptimer_start(gptimer);

		Calibrate();
	}

	tick_t IRAM_ATTR Esp32IdfTimerService::GetTick()
	{
		uint64_t ret;
		gptimer_get_raw_count(gptimer, &ret);
		return static_cast<tick_t>(ret);
	}

	void IRAM_ATTR Esp32IdfTimerService::ScheduleCallBack(const tick_t tick)
	{
		uint64_t cnt;
		gptimer_get_raw_count(gptimer, &cnt);
		cnt = (cnt & ((~static_cast<uint64_t>(0)) << (sizeof(tick_t) * 8))) | tick;
		gptimer_alarm_config_t alarm_config = {
			.alarm_count = cnt,
		};
		gptimer_set_alarm_action(gptimer, &alarm_config);
	}

	tick_t IRAM_ATTR Esp32IdfTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}

	void IRAM_ATTR Esp32IdfTimerService::TimerInterrupt() 
	{
		ReturnCallBack();
	}
}