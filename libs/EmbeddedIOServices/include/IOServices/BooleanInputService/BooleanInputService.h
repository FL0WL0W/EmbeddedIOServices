#include "HardwareAbstraction/HardwareAbstractionCollection.h"
#include "IBooleanInputService.h"
#include "Packed.h"
#include "stdint.h"

#if !defined(BOOLEANINPUTSERVICE_H) && defined(IBOOLEANINPUTSERVICE_H) && defined(HARDWAREABSTRACTIONCOLLECTION_H)
#define BOOLEANINPUTSERVICE_H
namespace IOServices
{
	PACK(
	struct BooleanInputServiceConfig
	{
	public:
		constexpr const unsigned int Size() const
		{
			return sizeof(BooleanInputServiceConfig);
		}

		uint16_t Pin;
		bool Inverted;
	});

	class BooleanInputService : public IBooleanInputService
	{
	protected:
		HardwareAbstraction::IDigitalService *_digitalService;
		const BooleanInputServiceConfig *_config;
		
	public:
		BooleanInputService(HardwareAbstraction::IDigitalService *digitalService, const BooleanInputServiceConfig *config);
		void ReadValue() override;
	};
}
#endif
