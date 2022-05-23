#include "IAnalogService.h"

#ifndef ANALOGSERVICE_H
#define ANALOGSERVICE_H
namespace EmbeddedIOServices
{
	class AnalogService : public IAnalogService
	{
	public:
		void InitPin(analogpin_t pin);
		float ReadPin(analogpin_t pin);
	};
}
#endif
