#ifndef MPC5XXXETPUGPIOCODE_H
#define MPC5XXXETPUGPIOCODE_H

#include <cstddef>
#include <cstdint>

namespace MPC5xxx
{
	namespace ETPUGPIOCode
	{
		extern const std::uint32_t Code[];
		extern const std::size_t CodeWordCount;
		extern const std::uint32_t Globals[];
		extern const std::size_t GlobalWordCount;

		constexpr std::uint32_t MiscSignature = 0x019FF9F8U;
		constexpr std::uint8_t FunctionNumber = 0U;
		constexpr std::uint8_t InputEitherEdgesHSR = 2U;
		constexpr std::uint16_t ParameterBytes = 16U;
	}
}

#endif
