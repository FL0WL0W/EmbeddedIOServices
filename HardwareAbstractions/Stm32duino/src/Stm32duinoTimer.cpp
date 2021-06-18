#include "Stm32duinoTimer.h"

namespace Stm32
{
	bool _timFrequencyLocked[TimerIndex::Num];
	HardwareTimer *_hardwareTimer[TimerIndex::Num];
	std::function<void()> _timCallBack[TimerIndex::Num];

	uint16_t _timPreviousCC[TimerIndex::Num][4];
	uint16_t _timCurrentCC[TimerIndex::Num][4];
#ifdef TIM1_BASE
	void TIM1_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index1] != 0)
			_timCallBack[TimerIndex::Index1]();
		_timPreviousCC[TimerIndex::Index1][0] = _timCurrentCC[TimerIndex::Index1][0];
		_timCurrentCC[TimerIndex::Index1][0] = TIM1->CCR1;
	}
	void TIM1_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index1][1] = _timCurrentCC[TimerIndex::Index1][1];
		_timCurrentCC[TimerIndex::Index1][1] = TIM1->CCR2;
	}
	void TIM1_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index1][2] = _timCurrentCC[TimerIndex::Index1][2];
		_timCurrentCC[TimerIndex::Index1][2] = TIM1->CCR3;
	}
	void TIM1_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index1][3] = _timCurrentCC[TimerIndex::Index1][3];
		_timCurrentCC[TimerIndex::Index1][3] = TIM1->CCR4;
	}
#endif
#ifdef TIM2_BASE
	void TIM2_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index2] != 0)
			_timCallBack[TimerIndex::Index2]();
		_timPreviousCC[TimerIndex::Index2][0] = _timCurrentCC[TimerIndex::Index2][0];
		_timCurrentCC[TimerIndex::Index2][0] = TIM2->CCR1;
	}
	void TIM2_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index2][1] = _timCurrentCC[TimerIndex::Index2][1];
		_timCurrentCC[TimerIndex::Index2][1] = TIM2->CCR2;
	}
	void TIM2_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index2][2] = _timCurrentCC[TimerIndex::Index2][2];
		_timCurrentCC[TimerIndex::Index2][2] = TIM2->CCR3;
	}
	void TIM2_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index2][3] = _timCurrentCC[TimerIndex::Index2][3];
		_timCurrentCC[TimerIndex::Index2][3] = TIM2->CCR4;
	}
#endif
#ifdef TIM3_BASE
	void TIM3_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index3] != 0)
			_timCallBack[TimerIndex::Index3]();
		_timPreviousCC[TimerIndex::Index3][0] = _timCurrentCC[TimerIndex::Index3][0];
		_timCurrentCC[TimerIndex::Index3][0] = TIM3->CCR1;
	}
	void TIM3_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index3][1] = _timCurrentCC[TimerIndex::Index3][1];
		_timCurrentCC[TimerIndex::Index3][1] = TIM3->CCR2;
	}
	void TIM3_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index3][2] = _timCurrentCC[TimerIndex::Index3][2];
		_timCurrentCC[TimerIndex::Index3][2] = TIM3->CCR3;
	}
	void TIM3_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index3][3] = _timCurrentCC[TimerIndex::Index3][3];
		_timCurrentCC[TimerIndex::Index3][3] = TIM3->CCR4;
	}
#endif
#ifdef TIM4_BASE
	void TIM4_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index4] != 0)
			_timCallBack[TimerIndex::Index4]();
		_timPreviousCC[TimerIndex::Index4][0] = _timCurrentCC[TimerIndex::Index4][0];
		_timCurrentCC[TimerIndex::Index4][0] = TIM4->CCR1;
	}
	void TIM4_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index4][1] = _timCurrentCC[TimerIndex::Index4][1];
		_timCurrentCC[TimerIndex::Index4][1] = TIM4->CCR2;
	}
	void TIM4_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index4][2] = _timCurrentCC[TimerIndex::Index4][2];
		_timCurrentCC[TimerIndex::Index4][2] = TIM4->CCR3;
	}
	void TIM4_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index4][3] = _timCurrentCC[TimerIndex::Index4][3];
		_timCurrentCC[TimerIndex::Index4][3] = TIM4->CCR4;
	}
#endif
#ifdef TIM5_BASE
	void TIM5_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index5] != 0)
			_timCallBack[TimerIndex::Index5]();
		_timPreviousCC[TimerIndex::Index5][0] = _timCurrentCC[TimerIndex::Index5][0];
		_timCurrentCC[TimerIndex::Index5][0] = TIM5->CCR1;
	}
	void TIM5_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index5][1] = _timCurrentCC[TimerIndex::Index5][1];
		_timCurrentCC[TimerIndex::Index5][1] = TIM5->CCR2;
	}
	void TIM5_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index5][2] = _timCurrentCC[TimerIndex::Index5][2];
		_timCurrentCC[TimerIndex::Index5][2] = TIM5->CCR3;
	}
	void TIM5_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index5][3] = _timCurrentCC[TimerIndex::Index5][3];
		_timCurrentCC[TimerIndex::Index5][3] = TIM5->CCR4;
	}
#endif
#ifdef TIM6_BASE
	void TIM6_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index6] != 0)
			_timCallBack[TimerIndex::Index6]();
		_timPreviousCC[TimerIndex::Index6][0] = _timCurrentCC[TimerIndex::Index6][0];
		_timCurrentCC[TimerIndex::Index6][0] = TIM6->CCR1;
	}
	void TIM6_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index6][1] = _timCurrentCC[TimerIndex::Index6][1];
		_timCurrentCC[TimerIndex::Index6][1] = TIM6->CCR2;
	}
	void TIM6_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index6][2] = _timCurrentCC[TimerIndex::Index6][2];
		_timCurrentCC[TimerIndex::Index6][2] = TIM6->CCR3;
	}
	void TIM6_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index6][3] = _timCurrentCC[TimerIndex::Index6][3];
		_timCurrentCC[TimerIndex::Index6][3] = TIM6->CCR4;
	}
#endif
#ifdef TIM7_BASE
	void TIM7_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index7] != 0)
			_timCallBack[TimerIndex::Index7]();
		_timPreviousCC[TimerIndex::Index7][0] = _timCurrentCC[TimerIndex::Index7][0];
		_timCurrentCC[TimerIndex::Index7][0] = TIM7->CCR1;
	}
	void TIM7_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index7][1] = _timCurrentCC[TimerIndex::Index7][1];
		_timCurrentCC[TimerIndex::Index7][1] = TIM7->CCR2;
	}
	void TIM7_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index7][2] = _timCurrentCC[TimerIndex::Index7][2];
		_timCurrentCC[TimerIndex::Index7][2] = TIM7->CCR3;
	}
	void TIM7_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index7][3] = _timCurrentCC[TimerIndex::Index7][3];
		_timCurrentCC[TimerIndex::Index7][3] = TIM7->CCR4;
	}
#endif
#ifdef TIM8_BASE
	void TIM8_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index8] != 0)
			_timCallBack[TimerIndex::Index8]();
		_timPreviousCC[TimerIndex::Index8][0] = _timCurrentCC[TimerIndex::Index8][0];
		_timCurrentCC[TimerIndex::Index8][0] = TIM8->CCR1;
	}
	void TIM8_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index8][1] = _timCurrentCC[TimerIndex::Index8][1];
		_timCurrentCC[TimerIndex::Index8][1] = TIM8->CCR2;
	}
	void TIM8_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index8][2] = _timCurrentCC[TimerIndex::Index8][2];
		_timCurrentCC[TimerIndex::Index8][2] = TIM8->CCR3;
	}
	void TIM8_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index8][3] = _timCurrentCC[TimerIndex::Index8][3];
		_timCurrentCC[TimerIndex::Index8][3] = TIM8->CCR4;
	}
#endif
#ifdef TIM9_BASE
	void TIM9_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index9] != 0)
			_timCallBack[TimerIndex::Index9]();
		_timPreviousCC[TimerIndex::Index9][0] = _timCurrentCC[TimerIndex::Index9][0];
		_timCurrentCC[TimerIndex::Index9][0] = TIM9->CCR1;
	}
	void TIM9_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index9][1] = _timCurrentCC[TimerIndex::Index9][1];
		_timCurrentCC[TimerIndex::Index9][1] = TIM9->CCR2;
	}
	void TIM9_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index9][2] = _timCurrentCC[TimerIndex::Index9][2];
		_timCurrentCC[TimerIndex::Index9][2] = TIM9->CCR3;
	}
	void TIM9_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index9][3] = _timCurrentCC[TimerIndex::Index9][3];
		_timCurrentCC[TimerIndex::Index9][3] = TIM9->CCR4;
	}
#endif
#ifdef TIM10_BASE
	void TIM10_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index10] != 0)
			_timCallBack[TimerIndex::Index10]();
		_timPreviousCC[TimerIndex::Index10][0] = _timCurrentCC[TimerIndex::Index10][0];
		_timCurrentCC[TimerIndex::Index10][0] = TIM10->CCR1;
	}
	void TIM10_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index10][1] = _timCurrentCC[TimerIndex::Index10][1];
		_timCurrentCC[TimerIndex::Index10][1] = TIM10->CCR2;
	}
	void TIM10_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index10][2] = _timCurrentCC[TimerIndex::Index10][2];
		_timCurrentCC[TimerIndex::Index10][2] = TIM10->CCR3;
	}
	void TIM10_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index10][3] = _timCurrentCC[TimerIndex::Index10][3];
		_timCurrentCC[TimerIndex::Index10][3] = TIM10->CCR4;
	}
#endif
#ifdef TIM11_BASE
	void TIM11_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index11] != 0)
			_timCallBack[TimerIndex::Index11]();
		_timPreviousCC[TimerIndex::Index11][0] = _timCurrentCC[TimerIndex::Index11][0];
		_timCurrentCC[TimerIndex::Index11][0] = TIM11->CCR1;
	}
	void TIM11_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index11][1] = _timCurrentCC[TimerIndex::Index11][1];
		_timCurrentCC[TimerIndex::Index11][1] = TIM11->CCR2;
	}
	void TIM11_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index11][2] = _timCurrentCC[TimerIndex::Index11][2];
		_timCurrentCC[TimerIndex::Index11][2] = TIM11->CCR3;
	}
	void TIM11_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index11][3] = _timCurrentCC[TimerIndex::Index11][3];
		_timCurrentCC[TimerIndex::Index11][3] = TIM11->CCR4;
	}
#endif
#ifdef TIM12_BASE
	void TIM12_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index12] != 0)
			_timCallBack[TimerIndex::Index12]();
		_timPreviousCC[TimerIndex::Index12][0] = _timCurrentCC[TimerIndex::Index12][0];
		_timCurrentCC[TimerIndex::Index12][0] = TIM12->CCR1;
	}
	void TIM12_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index12][1] = _timCurrentCC[TimerIndex::Index12][1];
		_timCurrentCC[TimerIndex::Index12][1] = TIM12->CCR2;
	}
	void TIM12_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index12][2] = _timCurrentCC[TimerIndex::Index12][2];
		_timCurrentCC[TimerIndex::Index12][2] = TIM12->CCR3;
	}
	void TIM12_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index12][3] = _timCurrentCC[TimerIndex::Index12][3];
		_timCurrentCC[TimerIndex::Index12][3] = TIM12->CCR4;
	}
#endif
#ifdef TIM13_BASE
	void TIM13_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index13] != 0)
			_timCallBack[TimerIndex::Index13]();
		_timPreviousCC[TimerIndex::Index13][0] = _timCurrentCC[TimerIndex::Index13][0];
		_timCurrentCC[TimerIndex::Index13][0] = TIM13->CCR1;
	}
	void TIM13_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index13][1] = _timCurrentCC[TimerIndex::Index13][1];
		_timCurrentCC[TimerIndex::Index13][1] = TIM13->CCR2;
	}
	void TIM13_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index13][2] = _timCurrentCC[TimerIndex::Index13][2];
		_timCurrentCC[TimerIndex::Index13][2] = TIM13->CCR3;
	}
	void TIM13_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index13][3] = _timCurrentCC[TimerIndex::Index13][3];
		_timCurrentCC[TimerIndex::Index13][3] = TIM13->CCR4;
	}
#endif
#ifdef TIM14_BASE
	void TIM14_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index14] != 0)
			_timCallBack[TimerIndex::Index14]();
		_timPreviousCC[TimerIndex::Index14][0] = _timCurrentCC[TimerIndex::Index14][0];
		_timCurrentCC[TimerIndex::Index14][0] = TIM14->CCR1;
	}
	void TIM14_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index14][1] = _timCurrentCC[TimerIndex::Index14][1];
		_timCurrentCC[TimerIndex::Index14][1] = TIM14->CCR2;
	}
	void TIM14_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index14][2] = _timCurrentCC[TimerIndex::Index14][2];
		_timCurrentCC[TimerIndex::Index14][2] = TIM14->CCR3;
	}
	void TIM14_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index14][3] = _timCurrentCC[TimerIndex::Index14][3];
		_timCurrentCC[TimerIndex::Index14][3] = TIM14->CCR4;
	}
#endif
#ifdef TIM15_BASE
	void TIM15_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index15] != 0)
			_timCallBack[TimerIndex::Index15]();
		_timPreviousCC[TimerIndex::Index15][0] = _timCurrentCC[TimerIndex::Index15][0];
		_timCurrentCC[TimerIndex::Index15][0] = TIM15->CCR1;
	}
	void TIM15_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index15][1] = _timCurrentCC[TimerIndex::Index15][1];
		_timCurrentCC[TimerIndex::Index15][1] = TIM15->CCR2;
	}
	void TIM15_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index15][2] = _timCurrentCC[TimerIndex::Index15][2];
		_timCurrentCC[TimerIndex::Index15][2] = TIM15->CCR3;
	}
	void TIM15_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index15][3] = _timCurrentCC[TimerIndex::Index15][3];
		_timCurrentCC[TimerIndex::Index15][3] = TIM15->CCR4;
	}
#endif
#ifdef TIM16_BASE
	void TIM16_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index16] != 0)
			_timCallBack[TimerIndex::Index16]();
		_timPreviousCC[TimerIndex::Index16][0] = _timCurrentCC[TimerIndex::Index16][0];
		_timCurrentCC[TimerIndex::Index16][0] = TIM16->CCR1;
	}
	void TIM16_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index16][1] = _timCurrentCC[TimerIndex::Index16][1];
		_timCurrentCC[TimerIndex::Index16][1] = TIM16->CCR2;
	}
	void TIM16_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index16][2] = _timCurrentCC[TimerIndex::Index16][2];
		_timCurrentCC[TimerIndex::Index16][2] = TIM16->CCR3;
	}
	void TIM16_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index16][3] = _timCurrentCC[TimerIndex::Index16][3];
		_timCurrentCC[TimerIndex::Index16][3] = TIM16->CCR4;
	}
#endif
#ifdef TIM17_BASE
	void TIM17_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index17] != 0)
			_timCallBack[TimerIndex::Index17]();
		_timPreviousCC[TimerIndex::Index17][0] = _timCurrentCC[TimerIndex::Index17][0];
		_timCurrentCC[TimerIndex::Index17][0] = TIM17->CCR1;
	}
	void TIM17_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index17][1] = _timCurrentCC[TimerIndex::Index17][1];
		_timCurrentCC[TimerIndex::Index17][1] = TIM17->CCR2;
	}
	void TIM17_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index17][2] = _timCurrentCC[TimerIndex::Index17][2];
		_timCurrentCC[TimerIndex::Index17][2] = TIM17->CCR3;
	}
	void TIM17_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index17][3] = _timCurrentCC[TimerIndex::Index17][3];
		_timCurrentCC[TimerIndex::Index17][3] = TIM17->CCR4;
	}
#endif
#ifdef TIM18_BASE
	void TIM18_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index18] != 0)
			_timCallBack[TimerIndex::Index18]();
		_timPreviousCC[TimerIndex::Index18][0] = _timCurrentCC[TimerIndex::Index18][0];
		_timCurrentCC[TimerIndex::Index18][0] = TIM18->CCR1;
	}
	void TIM18_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index18][1] = _timCurrentCC[TimerIndex::Index18][1];
		_timCurrentCC[TimerIndex::Index18][1] = TIM18->CCR2;
	}
	void TIM18_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index18][2] = _timCurrentCC[TimerIndex::Index18][2];
		_timCurrentCC[TimerIndex::Index18][2] = TIM18->CCR3;
	}
	void TIM18_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index18][3] = _timCurrentCC[TimerIndex::Index18][3];
		_timCurrentCC[TimerIndex::Index18][3] = TIM18->CCR4;
	}
#endif
#ifdef TIM19_BASE
	void TIM19_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index19] != 0)
			_timCallBack[TimerIndex::Index19]();
		_timPreviousCC[TimerIndex::Index19][0] = _timCurrentCC[TimerIndex::Index19][0];
		_timCurrentCC[TimerIndex::Index19][0] = TIM19->CCR1;
	}
	void TIM19_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index19][1] = _timCurrentCC[TimerIndex::Index19][1];
		_timCurrentCC[TimerIndex::Index19][1] = TIM19->CCR2;
	}
	void TIM19_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index19][2] = _timCurrentCC[TimerIndex::Index19][2];
		_timCurrentCC[TimerIndex::Index19][2] = TIM19->CCR3;
	}
	void TIM19_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index19][3] = _timCurrentCC[TimerIndex::Index19][3];
		_timCurrentCC[TimerIndex::Index19][3] = TIM19->CCR4;
	}
#endif
#ifdef TIM20_BASE
	void TIM20_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index20] != 0)
			_timCallBack[TimerIndex::Index20]();
		_timPreviousCC[TimerIndex::Index20][0] = _timCurrentCC[TimerIndex::Index20][0];
		_timCurrentCC[TimerIndex::Index20][0] = TIM20->CCR1;
	}
	void TIM20_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index20][1] = _timCurrentCC[TimerIndex::Index20][1];
		_timCurrentCC[TimerIndex::Index20][1] = TIM20->CCR2;
	}
	void TIM20_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index20][2] = _timCurrentCC[TimerIndex::Index20][2];
		_timCurrentCC[TimerIndex::Index20][2] = TIM20->CCR3;
	}
	void TIM20_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index20][3] = _timCurrentCC[TimerIndex::Index20][3];
		_timCurrentCC[TimerIndex::Index20][3] = TIM20->CCR4;
	}
#endif
#ifdef TIM21_BASE
	void TIM21_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index21] != 0)
			_timCallBack[TimerIndex::Index21]();
		_timPreviousCC[TimerIndex::Index21][0] = _timCurrentCC[TimerIndex::Index21][0];
		_timCurrentCC[TimerIndex::Index21][0] = TIM21->CCR1;
	}
	void TIM21_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index21][1] = _timCurrentCC[TimerIndex::Index21][1];
		_timCurrentCC[TimerIndex::Index21][1] = TIM21->CCR2;
	}
	void TIM21_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index21][2] = _timCurrentCC[TimerIndex::Index21][2];
		_timCurrentCC[TimerIndex::Index21][2] = TIM21->CCR3;
	}
	void TIM21_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index21][3] = _timCurrentCC[TimerIndex::Index21][3];
		_timCurrentCC[TimerIndex::Index21][3] = TIM21->CCR4;
	}
#endif
#ifdef TIM22_BASE
	void TIM22_CC1(void)
	{
		if (_timCallBack[TimerIndex::Index22] != 0)
			_timCallBack[TimerIndex::Index22]();
		_timPreviousCC[TimerIndex::Index22][0] = _timCurrentCC[TimerIndex::Index22][0];
		_timCurrentCC[TimerIndex::Index22][0] = TIM22->CCR1;
	}
	void TIM22_CC2(void)
	{
		_timPreviousCC[TimerIndex::Index22][1] = _timCurrentCC[TimerIndex::Index22][1];
		_timCurrentCC[TimerIndex::Index22][1] = TIM22->CCR2;
	}
	void TIM22_CC3(void)
	{
		_timPreviousCC[TimerIndex::Index22][2] = _timCurrentCC[TimerIndex::Index22][2];
		_timCurrentCC[TimerIndex::Index22][2] = TIM22->CCR3;
	}
	void TIM22_CC4(void)
	{
		_timPreviousCC[TimerIndex::Index22][3] = _timCurrentCC[TimerIndex::Index22][3];
		_timCurrentCC[TimerIndex::Index22][3] = TIM22->CCR4;
	}
#endif

	void TimInit(TimerIndex index, uint32_t prescaler, uint32_t period)
	{
		if(_hardwareTimer[index] == 0)
			_hardwareTimer[index] = new HardwareTimer(TimIndexToTIM(index));
		_hardwareTimer[index]->setPrescaleFactor(prescaler);
		_hardwareTimer[index]->setOverflow(period);
		
		TIM_HandleTypeDef TIM_HandleStruct;

		//set mode
		TIM_HandleStruct.Instance = TimIndexToTIM(index);
		TIM_HandleStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
		TIM_HandleStruct.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  		TIM_HandleStruct.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		TIM_HandleStruct.Init.Prescaler = prescaler;
  		TIM_HandleStruct.Init.Period = period; // autoreload at max
		
		//init
		HAL_TIM_Base_Init(&TIM_HandleStruct);

		//configure clock
		TIM_ClockConfigTypeDef TIM_ClockConfigStruct = {0};
		TIM_ClockConfigStruct.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_ClockConfigStruct);

		//start timer
		HAL_TIM_Base_Start(&TIM_HandleStruct);
	}

	TIM_TypeDef *TimIndexToTIM(TimerIndex index)
	{
		switch (index)
		{
#ifdef TIM1_BASE
		case TimerIndex::Index1:
			return TIM1;
#endif
#ifdef TIM2_BASE
		case TimerIndex::Index2:
			return TIM2;
#endif
#ifdef TIM3_BASE
		case TimerIndex::Index3:
			return TIM3;
#endif
#ifdef TIM4_BASE
		case TimerIndex::Index4:
			return TIM4;
#endif
#ifdef TIM5_BASE
		case TimerIndex::Index5:
			return TIM5;
#endif
#ifdef TIM6_BASE
		case TimerIndex::Index6:
			return TIM6;
#endif
#ifdef TIM7_BASE
		case TimerIndex::Index7:
			return TIM7;
#endif
#ifdef TIM8_BASE
		case TimerIndex::Index8:
			return TIM8;
#endif
#ifdef TIM9_BASE
		case TimerIndex::Index9:
			return TIM9;
#endif
#ifdef TIM10_BASE
		case TimerIndex::Index10:
			return TIM10;
#endif
#ifdef TIM11_BASE
		case TimerIndex::Index11:
			return TIM11;
#endif
#ifdef TIM12_BASE
		case TimerIndex::Index12:
			return TIM12;
#endif
#ifdef TIM13_BASE
		case TimerIndex::Index13:
			return TIM13;
#endif
#ifdef TIM14_BASE
		case TimerIndex::Index14:
			return TIM14;
#endif
#ifdef TIM15_BASE
		case TimerIndex::Index15:
			return TIM15;
#endif
#ifdef TIM16_BASE
		case TimerIndex::Index16:
			return TIM16;
#endif
#ifdef TIM17_BASE
		case TimerIndex::Index17:
			return TIM17;
#endif
#ifdef TIM18_BASE
		case TimerIndex::Index18:
			return TIM18;
#endif
#ifdef TIM19_BASE
		case TimerIndex::Index19:
			return TIM19;
#endif
#ifdef TIM20_BASE
		case TimerIndex::Index20:
			return TIM20;
#endif
#ifdef TIM21_BASE
		case TimerIndex::Index21:
			return TIM21;
#endif
#ifdef TIM22_BASE
		case TimerIndex::Index22:
			return TIM22;
#endif
		}
	}

    void EnableTimerInterrupts(TimerIndex index)
	{
		if(_hardwareTimer[index] == 0)
			_hardwareTimer[index] = new HardwareTimer(TimIndexToTIM(index));

		switch (index)
		{
#ifdef TIM1_BASE
		case TimerIndex::Index1:
			_hardwareTimer[TimerIndex::Index1]->attachInterrupt(1, TIM1_CC1);
			_hardwareTimer[TimerIndex::Index1]->attachInterrupt(2, TIM1_CC2);
			_hardwareTimer[TimerIndex::Index1]->attachInterrupt(3, TIM1_CC3);
			_hardwareTimer[TimerIndex::Index1]->attachInterrupt(4, TIM1_CC4);
			break;
#endif
#ifdef TIM2_BASE
		case TimerIndex::Index2:
			_hardwareTimer[TimerIndex::Index2]->attachInterrupt(1, TIM2_CC1);
			_hardwareTimer[TimerIndex::Index2]->attachInterrupt(2, TIM2_CC2);
			_hardwareTimer[TimerIndex::Index2]->attachInterrupt(3, TIM2_CC3);
			_hardwareTimer[TimerIndex::Index2]->attachInterrupt(4, TIM2_CC4);
			break;
#endif
#ifdef TIM3_BASE
		case TimerIndex::Index3:
			_hardwareTimer[TimerIndex::Index3]->attachInterrupt(1, TIM3_CC1);
			_hardwareTimer[TimerIndex::Index3]->attachInterrupt(2, TIM3_CC2);
			_hardwareTimer[TimerIndex::Index3]->attachInterrupt(3, TIM3_CC3);
			_hardwareTimer[TimerIndex::Index3]->attachInterrupt(4, TIM3_CC4);
			break;
#endif
#ifdef TIM4_BASE
		case TimerIndex::Index4:
			_hardwareTimer[TimerIndex::Index4]->attachInterrupt(1, TIM4_CC1);
			_hardwareTimer[TimerIndex::Index4]->attachInterrupt(2, TIM4_CC2);
			_hardwareTimer[TimerIndex::Index4]->attachInterrupt(3, TIM4_CC3);
			_hardwareTimer[TimerIndex::Index4]->attachInterrupt(4, TIM4_CC4);
			break;
#endif
#ifdef TIM5_BASE
		case TimerIndex::Index5:
			_hardwareTimer[TimerIndex::Index5]->attachInterrupt(1, TIM5_CC1);
			_hardwareTimer[TimerIndex::Index5]->attachInterrupt(2, TIM5_CC2);
			_hardwareTimer[TimerIndex::Index5]->attachInterrupt(3, TIM5_CC3);
			_hardwareTimer[TimerIndex::Index5]->attachInterrupt(4, TIM5_CC4);
			break;
#endif
#ifdef TIM6_BASE
		case TimerIndex::Index6:
			_hardwareTimer[TimerIndex::Index6]->attachInterrupt(1, TIM6_CC1);
			_hardwareTimer[TimerIndex::Index6]->attachInterrupt(2, TIM6_CC2);
			_hardwareTimer[TimerIndex::Index6]->attachInterrupt(3, TIM6_CC3);
			_hardwareTimer[TimerIndex::Index6]->attachInterrupt(4, TIM6_CC4);
			break;
#endif
#ifdef TIM7_BASE
		case TimerIndex::Index7:
			_hardwareTimer[TimerIndex::Index7]->attachInterrupt(1, TIM7_CC1);
			_hardwareTimer[TimerIndex::Index7]->attachInterrupt(2, TIM7_CC2);
			_hardwareTimer[TimerIndex::Index7]->attachInterrupt(3, TIM7_CC3);
			_hardwareTimer[TimerIndex::Index7]->attachInterrupt(4, TIM7_CC4);
			break;
#endif
#ifdef TIM8_BASE
		case TimerIndex::Index8:
			_hardwareTimer[TimerIndex::Index8]->attachInterrupt(1, TIM8_CC1);
			_hardwareTimer[TimerIndex::Index8]->attachInterrupt(2, TIM8_CC2);
			_hardwareTimer[TimerIndex::Index8]->attachInterrupt(3, TIM8_CC3);
			_hardwareTimer[TimerIndex::Index8]->attachInterrupt(4, TIM8_CC4);
			break;
#endif
#ifdef TIM9_BASE
		case TimerIndex::Index9:
			_hardwareTimer[TimerIndex::Index9]->attachInterrupt(1, TIM9_CC1);
			_hardwareTimer[TimerIndex::Index9]->attachInterrupt(2, TIM9_CC2);
			_hardwareTimer[TimerIndex::Index9]->attachInterrupt(3, TIM9_CC3);
			_hardwareTimer[TimerIndex::Index9]->attachInterrupt(4, TIM9_CC4);
			break;
#endif
#ifdef TIM10_BASE
		case TimerIndex::Index10:
			_hardwareTimer[TimerIndex::Index10]->attachInterrupt(1, TIM10_CC1);
			_hardwareTimer[TimerIndex::Index10]->attachInterrupt(2, TIM10_CC2);
			_hardwareTimer[TimerIndex::Index10]->attachInterrupt(3, TIM10_CC3);
			_hardwareTimer[TimerIndex::Index10]->attachInterrupt(4, TIM10_CC4);
			break;
#endif
#ifdef TIM11_BASE
		case TimerIndex::Index11:
			_hardwareTimer[TimerIndex::Index11]->attachInterrupt(1, TIM11_CC1);
			_hardwareTimer[TimerIndex::Index11]->attachInterrupt(2, TIM11_CC2);
			_hardwareTimer[TimerIndex::Index11]->attachInterrupt(3, TIM11_CC3);
			_hardwareTimer[TimerIndex::Index11]->attachInterrupt(4, TIM11_CC4);
			break;
#endif
#ifdef TIM12_BASE
		case TimerIndex::Index12:
			_hardwareTimer[TimerIndex::Index12]->attachInterrupt(1, TIM12_CC1);
			_hardwareTimer[TimerIndex::Index12]->attachInterrupt(2, TIM12_CC2);
			_hardwareTimer[TimerIndex::Index12]->attachInterrupt(3, TIM12_CC3);
			_hardwareTimer[TimerIndex::Index12]->attachInterrupt(4, TIM12_CC4);
			break;
#endif
#ifdef TIM13_BASE
		case TimerIndex::Index13:
			_hardwareTimer[TimerIndex::Index13]->attachInterrupt(1, TIM13_CC1);
			_hardwareTimer[TimerIndex::Index13]->attachInterrupt(2, TIM13_CC2);
			_hardwareTimer[TimerIndex::Index13]->attachInterrupt(3, TIM13_CC3);
			_hardwareTimer[TimerIndex::Index13]->attachInterrupt(4, TIM13_CC4);
			break;
#endif
#ifdef TIM14_BASE
		case TimerIndex::Index14:
			_hardwareTimer[TimerIndex::Index14]->attachInterrupt(1, TIM14_CC1);
			_hardwareTimer[TimerIndex::Index14]->attachInterrupt(2, TIM14_CC2);
			_hardwareTimer[TimerIndex::Index14]->attachInterrupt(3, TIM14_CC3);
			_hardwareTimer[TimerIndex::Index14]->attachInterrupt(4, TIM14_CC4);
			break;
#endif
#ifdef TIM15_BASE
		case TimerIndex::Index15:
			_hardwareTimer[TimerIndex::Index15]->attachInterrupt(1, TIM15_CC1);
			_hardwareTimer[TimerIndex::Index15]->attachInterrupt(2, TIM15_CC2);
			_hardwareTimer[TimerIndex::Index15]->attachInterrupt(3, TIM15_CC3);
			_hardwareTimer[TimerIndex::Index15]->attachInterrupt(4, TIM15_CC4);
			break;
#endif
#ifdef TIM16_BASE
		case TimerIndex::Index16:
			_hardwareTimer[TimerIndex::Index16]->attachInterrupt(1, TIM16_CC1);
			_hardwareTimer[TimerIndex::Index16]->attachInterrupt(2, TIM16_CC2);
			_hardwareTimer[TimerIndex::Index16]->attachInterrupt(3, TIM16_CC3);
			_hardwareTimer[TimerIndex::Index16]->attachInterrupt(4, TIM16_CC4);
			break;
#endif
#ifdef TIM17_BASE
		case TimerIndex::Index17:
			_hardwareTimer[TimerIndex::Index17]->attachInterrupt(1, TIM17_CC1);
			_hardwareTimer[TimerIndex::Index17]->attachInterrupt(2, TIM17_CC2);
			_hardwareTimer[TimerIndex::Index17]->attachInterrupt(3, TIM17_CC3);
			_hardwareTimer[TimerIndex::Index17]->attachInterrupt(4, TIM17_CC4);
			break;
#endif
#ifdef TIM18_BASE
		case TimerIndex::Index18:
			_hardwareTimer[TimerIndex::Index18]->attachInterrupt(1, TIM18_CC1);
			_hardwareTimer[TimerIndex::Index18]->attachInterrupt(2, TIM18_CC2);
			_hardwareTimer[TimerIndex::Index18]->attachInterrupt(3, TIM18_CC3);
			_hardwareTimer[TimerIndex::Index18]->attachInterrupt(4, TIM18_CC4);
			break;
#endif
#ifdef TIM19_BASE
		case TimerIndex::Index19:
			_hardwareTimer[TimerIndex::Index19]->attachInterrupt(1, TIM19_CC1);
			_hardwareTimer[TimerIndex::Index19]->attachInterrupt(2, TIM19_CC2);
			_hardwareTimer[TimerIndex::Index19]->attachInterrupt(3, TIM19_CC3);
			_hardwareTimer[TimerIndex::Index19]->attachInterrupt(4, TIM19_CC4);
			break;
#endif
#ifdef TIM20_BASE
		case TimerIndex::Index20:
			_hardwareTimer[TimerIndex::Index20]->attachInterrupt(1, TIM20_CC1);
			_hardwareTimer[TimerIndex::Index20]->attachInterrupt(2, TIM20_CC2);
			_hardwareTimer[TimerIndex::Index20]->attachInterrupt(3, TIM20_CC3);
			_hardwareTimer[TimerIndex::Index20]->attachInterrupt(4, TIM20_CC4);
			break;
#endif
#ifdef TIM21_BASE
		case TimerIndex::Index21:
			_hardwareTimer[TimerIndex::Index21]->attachInterrupt(1, TIM21_CC1);
			_hardwareTimer[TimerIndex::Index21]->attachInterrupt(2, TIM21_CC2);
			_hardwareTimer[TimerIndex::Index21]->attachInterrupt(3, TIM21_CC3);
			_hardwareTimer[TimerIndex::Index21]->attachInterrupt(4, TIM21_CC4);
			break;
#endif
#ifdef TIM22_BASE
		case TimerIndex::Index22:
			_hardwareTimer[TimerIndex::Index22]->attachInterrupt(1, TIM22_CC1);
			_hardwareTimer[TimerIndex::Index22]->attachInterrupt(2, TIM22_CC2);
			_hardwareTimer[TimerIndex::Index22]->attachInterrupt(3, TIM22_CC3);
			_hardwareTimer[TimerIndex::Index22]->attachInterrupt(4, TIM22_CC4);
			break;
#endif
		}
	}
}
