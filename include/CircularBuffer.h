#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <cstddef>
#include <cstdint>

namespace EmbeddedIOServices
{
	/**
	 * Fixed-capacity, lock-free Multiple Producer Single Consumer (MPSC) circular buffer.
	 *
	 * Producers reserve a slot by advancing _enqueuePosition, populate it, and
	 * publish it by updating that slot's state. The Consumer performs the
	 * inverse operation. A producer may therefore be interrupted at any point by
	 * another producer or consumer without requiring interrupts to be disabled.
	 *
	 * T must be default constructible and assignable. Its assignment must not
	 * throw and must be safe in every context that calls the buffer.
	 *
	 * This implementation targets nested interrupt producers on a single core.
	 * Correctness under preemption requires the free-slot count at the start of a
	 * Push to exceed the number of successful Push calls that can nest inside it.
	 * This prevents a nested producer from wrapping back to a slot that an
	 * interrupted producer has selected but not yet published.
	 */
	template<typename T, size_t CapacityValue>
	class CircularBuffer final
	{
		static constexpr size_t MaxPosition = (static_cast<size_t>(-1) / CapacityValue) * CapacityValue;
		std::ptrdiff_t PositionDifference(size_t position, size_t reference) const
		{
			const size_t forward = position >= reference? position - reference : MaxPosition - (reference - position);

			if (forward <= MaxPosition / 2U)
				return static_cast<std::ptrdiff_t>(forward);

			return -static_cast<std::ptrdiff_t>(MaxPosition - forward);
		}


		volatile size_t _enqueuePosition;
		volatile size_t _dequeuePosition;
		volatile uint8_t _slotState[CapacityValue];
		T _slots[CapacityValue];
	public:
		CircularBuffer() : _enqueuePosition(0U), _dequeuePosition(0U)
		{
			for (size_t i = 0U; i < CapacityValue; ++i)
				_slotState[i] = 0;
		}

		bool Push(T& value, uint8_t state = 1U)
		{
			if (state == 0U || state > 63U)
				return false;
				
			state = (state << 2U) | 0x01;

			size_t position = _enqueuePosition;

			while(PositionDifference(position, _dequeuePosition) < static_cast<std::ptrdiff_t>(CapacityValue))
			{
				_enqueuePosition = (position + 1U) % MaxPosition;
				const size_t index = position % CapacityValue;

				if(!(_slotState[index] & 0x01) && PositionDifference(position, _dequeuePosition) >= 0)
				{
					_slots[index] = value;
					_slotState[index] = state;
					return true;
				}

				position = (position + 1U) % MaxPosition;
			}

			return false;
		}

		uint8_t Pop(T& value)
		{
			const size_t index = _dequeuePosition % CapacityValue;
			const uint8_t state = _slotState[index];
			if(_dequeuePosition == _enqueuePosition && !(state & 0x01)) //check if empty
				return 0;

			if(state & 0x01)
			{
				value = _slots[index];
				_slotState[index] = 0x02;
				_dequeuePosition = (_dequeuePosition + 1U) % MaxPosition;
				return state >> 2U;
			}

			return 0;
		}

		bool Full() const
		{
			for (size_t i = 0U; i < CapacityValue; ++i)
				if (!(_slotState[i] & 0x01U))
					return false;
			return true;
		}
	};
}

#endif
