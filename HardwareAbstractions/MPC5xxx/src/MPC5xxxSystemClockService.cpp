#include "MPC5xxxSystemClockService.h"
namespace MPC5xxx {
const MPC5xxxSystemClockService* MPC5xxxSystemClockService::_instance = nullptr;
bool MPC5xxxSystemClockService::HasInstance() { return _instance != nullptr; }
const MPC5xxxSystemClockService& MPC5xxxSystemClockService::Instance() { return *_instance; }
}
