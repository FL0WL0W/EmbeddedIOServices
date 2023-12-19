#include <stdint.h>
#include <math.h>
#include "CallBack.h"
#include "Esp32IdfTimerService.h"
#include "esp_attr.h"
#include "esp_intr_alloc.h"
#include "stdio.h"
#include "esp_cpu.h"
#include "esp_clk_tree.h"

using namespace EmbeddedIOServices;

namespace Esp32
{	
	bool IRAM_ATTR RAMTimerInterrupt(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
	{
		static_cast<Esp32IdfTimerService *>(user_data)->TimerInterrupt();
		return true;
	}

	Esp32IdfTimerService::Esp32IdfTimerService()
	{
    	uint32_t apb_clk_freq = 0;
    	uint32_t cpu_clk_freq = 0;
		esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_APB, ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, &apb_clk_freq);
		esp_clk_tree_src_get_freq_hz(SOC_MOD_CLK_CPU, ESP_CLK_TREE_SRC_FREQ_PRECISION_EXACT, &cpu_clk_freq);

		_ticksPerSecond = apb_clk_freq / 2;

		gptimer_config_t timer_config = {
			.clk_src = GPTIMER_CLK_SRC_DEFAULT,
			.direction = GPTIMER_COUNT_UP,
			.resolution_hz = apb_clk_freq / 2, // 40MHz, 1 tick=25ns
			.intr_priority = 2
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
		uint64_t cnt;
		gptimer_get_raw_count(gptimer, &cnt);
		return static_cast<tick_t>(cnt);
	}

	void IRAM_ATTR Esp32IdfTimerService::ScheduleCallBack(const tick_t tick)
	{
		uint64_t cnt;
		gptimer_get_raw_count(gptimer, &cnt);
		cnt = (cnt & ((~static_cast<uint64_t>(0)) << (sizeof(tick_t) * 8))) | tick;
		const gptimer_alarm_config_t alarm_config = {
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