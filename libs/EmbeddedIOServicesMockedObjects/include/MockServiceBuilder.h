#include "Service/ServiceBuilder.h"

#ifndef MOCKANALOGSERVICE_H
#define MOCKANALOGSERVICE_H
namespace Service
{
	class MockServiceBuilder : public ServiceBuilder
	{
	public:
		MOCK_METHOD3(Build, void(ServiceLocator *&, const void *, unsigned int &));
		MOCK_METHOD2(Register, void(uint16_t, void*(*)(const ServiceLocator * const &, const void *, unsigned int &)));
	};
}
#endif