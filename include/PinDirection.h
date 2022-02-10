#include "stdint.h"

#ifndef PINDIRECTION_H
#define PINDIRECTION_H
namespace EmbeddedIOServices
{
	//// Used to set the direction of a pin during initialization
	enum PinDirection : uint8_t
	{
		In = 0, //// Pin is an input.
		Out = 1 //// Pin is an output.
	};
}
#endif
