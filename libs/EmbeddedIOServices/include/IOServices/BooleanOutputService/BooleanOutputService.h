#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IBooleanOutputService.h"
#include "Packed.h"
#include "stdint.h"

#if !defined(BOOLEANOUTPUTSERVICE_H) && defined(IBOOLEANOUTPUTSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H)
#define BOOLEANOUTPUTSERVICE_H
namespace IOServices
{
	PACK(
	struct BooleanOutputServiceConfig
	{
	public:
		constexpr const unsigned int Size() const
		{
			return sizeof(BooleanOutputServiceConfig);
		}

		uint16_t Pin;
		bool NormalOn;
		bool HighZ;
	});

	class BooleanOutputService : public IBooleanOutputService
	{
	protected:
		const HardwareAbstraction::HardwareAbstractionCollection *_hardwareAbstractionCollection;
		const BooleanOutputServiceConfig *_config;
		
	public:
		BooleanOutputService(const HardwareAbstraction::HardwareAbstractionCollection *, const BooleanOutputServiceConfig *);

		void OutputSet() override;
		void OutputReset() override;
		void OutputWrite(bool value) override;
	};
}
#endif
