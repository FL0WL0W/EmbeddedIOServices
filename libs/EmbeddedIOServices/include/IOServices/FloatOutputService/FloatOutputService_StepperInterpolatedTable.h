#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IOServices/StepperOutputService/IStepperOutputService.h"
#include "IFloatOutputService.h"
#include "math.h"
#include "Packed.h"

using namespace HardwareAbstraction;

#if !defined(FLOATOUTPUTSERVICE_STEPPERINTERPOLATEDTABLE_H) && defined(IFLOATOUTPUTSERVICE_H) && defined(ISTEPPEROUTPUTSERVICE_H)
#define FLOATOUTPUTSERVICE_STEPPERINTERPOLATEDTABLE_H
namespace IOServices
{
	PACK(
	struct FloatOutputService_StepperInterpolatedTableConfig
	{
	private:
		FloatOutputService_StepperInterpolatedTableConfig()
		{
			
		}
		
	public:
		static FloatOutputService_StepperInterpolatedTableConfig* Cast(void *p)
		{
			FloatOutputService_StepperInterpolatedTableConfig *ret = (FloatOutputService_StepperInterpolatedTableConfig *)p;

			ret->Table = (int *)(ret + 1);

			return ret;
		}
			
		unsigned int Size()
		{
			return sizeof(FloatOutputService_StepperInterpolatedTableConfig) +
				(sizeof(int) * Resolution);
		}
		
		float MinValue;
		float MaxValue;
		unsigned char Resolution;
		int *Table;
	});

	class FloatOutputService_StepperInterpolatedTable : public IFloatOutputService
	{
	protected:
		const FloatOutputService_StepperInterpolatedTableConfig *_config;

		IStepperOutputService *_stepperService;
		int _currentStepPosition;

	public:
		FloatOutputService_StepperInterpolatedTable(const FloatOutputService_StepperInterpolatedTableConfig *config, IStepperOutputService *stepperService);
		
		void SetOutput(float value);
		void Calibrate();
	};
}
#endif