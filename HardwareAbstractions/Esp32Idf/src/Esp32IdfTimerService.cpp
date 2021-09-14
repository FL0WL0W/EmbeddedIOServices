#include <stdint.h>
#include <math.h>
#include "CallBack.h"
#include "Esp32IdfTimerService.h"
#include "hal/clk_gate_ll.h"

using namespace EmbeddedIOServices;

namespace Esp32
{	
	void IRAM_ATTR RAMTimerInterrupt(void *arg) 
	{
		static_cast<Esp32IdfTimerService *>(arg)->TimerInterrupt();
	}

	Esp32IdfTimerService::Esp32IdfTimerService(timer_group_t group_num, timer_idx_t timer_num)
	{	
		//enable peripheral module
		periph_ll_enable_clk_clear_rst(timer_group_periph_signals.groups[group_num].module);

		//initialize hal variables
		timer_hal_init(&hal, group_num, timer_num);
		//disable tiumer interrupt while we setup
		timer_hal_intr_disable(&hal);
		//and clear the flag
		timer_hal_clear_intr_status(&hal);
		//don't autoreload on alarm compare
		timer_hal_set_auto_reload(&hal, TIMER_AUTORELOAD_DIS);
		//divide clock by 2 as that is the minimum divide amount
		timer_hal_set_divider(&hal, 2);
		//count up
		timer_hal_set_counter_increase(&hal, TIMER_COUNT_UP);
		//disable alarm 
		timer_hal_set_alarm_enable(&hal, TIMER_ALARM_DIS);
		//enable level interrupt
		timer_hal_set_level_int_enable(&hal, true);
		//set timer to 0. if you got real unlucky and the counter started almost max, then there would be a potential missed interrupt. setting it to 0 means it won't overlap for ~14,500 years
    	timer_hal_set_counter_value(&hal, 0);
		//start timer
		timer_hal_set_counter_enable(&hal, TIMER_START);
#if SOC_TIMER_GROUP_SUPPORT_XTAL
		//use APB as clock source
	    timer_hal_set_use_xtal(hal, TIMER_SRC_CLK_APB);
#endif

		//setup timer interrupt
		uint32_t intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM;
		uint32_t status_reg = 0;
		uint32_t mask = 0;
		timer_hal_get_status_reg_mask_bit(&hal, &status_reg, &mask);
		esp_intr_alloc_intrstatus(timer_group_periph_signals.groups[group_num].t0_irq_id + timer_num, intr_alloc_flags, status_reg, mask, RAMTimerInterrupt, (void *)this, &timer_isr_handle);

		Calibrate();
	}

	tick_t IRAM_ATTR Esp32IdfTimerService::GetTick()
	{
		uint64_t ret;
		timer_hal_get_counter_value(&hal, &ret);
		return static_cast<tick_t>(ret);
	}

	void IRAM_ATTR Esp32IdfTimerService::ScheduleCallBack(const tick_t tick)
	{
		uint64_t cnt;
		timer_hal_set_alarm_enable(&hal, TIMER_ALARM_DIS);
		timer_hal_get_counter_value(&hal, &cnt);
		timer_hal_set_alarm_value(&hal, (cnt & ((~static_cast<uint64_t>(0)) << (sizeof(tick_t) * 8))) | tick);
		timer_hal_set_alarm_enable(&hal, TIMER_ALARM_EN);
	}
	
	void IRAM_ATTR Esp32IdfTimerService::TimerInterrupt(void)
	{
		timer_hal_clear_intr_status(&hal);
		ReturnCallBack();
	}

	tick_t IRAM_ATTR Esp32IdfTimerService::GetTicksPerSecond()
	{
		return _ticksPerSecond;
	}
}