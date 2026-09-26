#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <cstddef>
#include <cstdint>

namespace EmbeddedIOServices
{
	/**
	 * Fixed-capacity, lock-free Multiple Producer Multiple Consumer (MPMC) circular buffer.
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
		volatile uint32_t _enqueuePosition;
		volatile uint32_t _dequeuePosition;
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
			if(state == 0U)
				return false;
				
			state = (state << 2U) | 0x01;

			const uint32_t startPosition = _enqueuePosition;
			uint32_t position = startPosition;

			do
			{
				_enqueuePosition = (position + 1U) % CapacityValue;

				if(!(_slotState[position] & 0x01))
				{
					_slots[position] = value;
					_slotState[position] = state;
					return true;
				}

				position = (position + 1U) % CapacityValue;
			}
			while(position != startPosition);

			return false;
		}

		uint8_t Pop(T& value)
		{
			uint32_t position = _dequeuePosition;
			if(position == _enqueuePosition && !(_slotState[position] & 0x01)) //check if empty
				return 0;

			do
			{
				_dequeuePosition = (position + 1U) % CapacityValue;
				const uint8_t state = _slotState[position];

				if(!(state & 0x03))
				{
					_dequeuePosition = position;
					return 0;
				}

				if(state & 0x01)
				{
					value = _slots[position];
					_slotState[position] = 0x02;
					return state >> 2U;
				}

				position = (position + 1U) % CapacityValue;
			}
			while(position != _enqueuePosition);

			return 0;
		}
	};
}

#endif
