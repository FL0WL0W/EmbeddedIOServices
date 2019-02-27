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
		constexpr const unsigned int Size() const
		{
			return sizeof(FloatOutputService_StepperInterpolatedTableConfig) +
				(sizeof(int) * Resolution);
		}
		
		constexpr const int32_t*Table() const { return reinterpret_cast<const int32_t*>(this + 1); }

		float MinValue;
		float MaxValue;
		uint8_t Resolution;
	});

	class FloatOutputService_StepperInterpolatedTable : public IFloatOutputService
	{
	protected:
		const FloatOutputService_StepperInterpolatedTableConfig *_config;

		IStepperOutputService *_stepperService;
		int32_t _currentStepPosition;

	public:
		FloatOutputService_StepperInterpolatedTable(const FloatOutputService_StepperInterpolatedTableConfig *config, IStepperOutputService *stepperService);
		
		void SetOutput(float value) override;
		void Calibrate() override;
	};
}
#endif
