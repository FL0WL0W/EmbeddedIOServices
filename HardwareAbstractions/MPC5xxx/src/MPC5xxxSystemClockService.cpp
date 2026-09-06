#include "MPC5xxxSystemClockService.h"
namespace MPC5xxx {
const MPC5xxxSystemClockService* MPC5xxxSystemClockService::_instance = nullptr;
MPC5xxxSystemClockService::MPC5xxxSystemClockService() { _instance = this; }
bool MPC5xxxSystemClockService::HasInstance() { return _instance != nullptr; }
bool MPC5xxxSystemClockService::Ready()
{
	return _instance != nullptr && _instance->ReadyImplementation();
}
std::uint32_t MPC5xxxSystemClockService::GetExternalCrystalFrequency()
{
	return _instance == nullptr ? 0U : _instance->GetExternalCrystalFrequencyImplementation();
}
std::uint32_t MPC5xxxSystemClockService::SystemClockHz()
{
	return _instance == nullptr ? 0U : _instance->SystemClockHzImplementation();
}
std::uint32_t MPC5xxxSystemClockService::PeripheralClockHz()
{
	return _instance == nullptr ? 0U : _instance->PeripheralClockHzImplementation();
}
}
