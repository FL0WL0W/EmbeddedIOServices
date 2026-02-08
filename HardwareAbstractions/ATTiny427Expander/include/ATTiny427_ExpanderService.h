#include "stddef.h"
#include "stdint.h"
#include "IDigitalService.h"
#include <map>
#include <list>
#include <vector>
#include <cstring>

#ifndef ATTINY427_EXPANDERSERVICE_H
#define ATTINY427_EXPANDERSERVICE_H

namespace EmbeddedIOServices
{
    //c++ 2011 and below don't include std::index_sequence_for<T...>; and std::index_sequence<Ints...>;
#if __cplusplus > 201103L
	template<std::size_t... Ints>
	using index_sequence = std::index_sequence<Ints...>;

	template<std::size_t N>
	using make_index_sequence = std::make_index_sequence<N>;
#else
	template<typename T, T... Ints>
	struct integer_sequence
	{
		typedef T value_type;
		static constexpr std::size_t size() { return sizeof...(Ints); }
	};
	
	template<std::size_t... Ints>
	using index_sequence = integer_sequence<std::size_t, Ints...>;
	
	template<typename T, std::size_t N, T... Is>
	struct make_integer_sequence : make_integer_sequence<T, N-1, N-1, Is...> {};
	
	template<typename T, T... Is>
	struct make_integer_sequence<T, 0, Is...> : integer_sequence<T, Is...> {};
	
	template<std::size_t N>
	using make_index_sequence = make_integer_sequence<std::size_t, N>;
#endif

	enum ATTiny427_ExpanderComm : uint8_t
	{
		ATTiny427_ExpanderComm_UART0,
		ATTiny427_ExpanderComm_UART0Alternate,
		ATTiny427_ExpanderComm_UART1,
		ATTiny427_ExpanderComm_UART1Alternate,
		ATTiny427_ExpanderComm_SPI,
		ATTiny427_ExpanderComm_SPIAlternate
	};

	typedef std::function<void(uint8_t *data)> ATTiny427_ExpanderPoller_callback_t;

	enum ATTiny427_ExpanderCommandType : uint8_t
	{
		ATTiny427_ExpanderCommandType_Read = 1,
		ATTiny427_ExpanderCommandType_ReadDelete = 2,
		ATTiny427_ExpanderCommandType_Write = 3,
		ATTiny427_ExpanderCommandType_Skip = 4,
		ATTiny427_ExpanderCommandType_Executed = 5
	};

	class ATTiny427_ExpanderService;

	class ATTiny427_ExpanderService
	{
	public:
		class Attiny427_ExpanderRegister
		{
		protected:
			union
			{
				struct
				{
					union 
					{
						uint16_t _address;
						struct
						{
							uint16_t _reservedAddress0_13 :14;
							bool _dataValid :1;
							uint16_t _reservedAddress15 :1;
						};
					};
					uint8_t _data;
					uint8_t _index;
				};
				ATTiny427_ExpanderService* _expanderService;
			};

			inline ATTiny427_ExpanderService* getExpanderService() const
			{
				return (this - (_index + 1))->_expanderService;
			}

			public:
			Attiny427_ExpanderRegister(ATTiny427_ExpanderService* expanderService);
			Attiny427_ExpanderRegister(uint8_t index);

			inline uint16_t GetAddress() const
			{
				return _address & 0xBFFF;
			}
			inline void SetAddress(uint16_t address)
			{
				_address = (address & 0xBFFF) | (_dataValid << 14);
			}
			inline bool IsDataValid() const
			{
				return _dataValid;
			}

			Attiny427_ExpanderRegister& operator=(uint8_t value);
			inline operator uint8_t() const
			{
				return _data;
			}
			inline Attiny427_ExpanderRegister& operator|=(uint8_t a)
			{
				if(!_dataValid || (_data | a) != _data)
					return *this = _data | a;
				return *this;
			}
			inline Attiny427_ExpanderRegister& operator&=(uint8_t a)
			{
				if(!_dataValid || (_data & a) != _data)
					return *this = _data & a;
				return *this;
			}
		};

		class ATTiny427_ExpanderPoller
		{
		protected:
			ATTiny427_ExpanderService* const _expanderService;
			size_t _readIdx;
			const ATTiny427_ExpanderPoller_callback_t _callback;
			union 
			{
				const uint16_t _address;
				struct
				{
					const uint16_t _reservedAddress0_13 :14;
					bool _enabled :1;
					const uint16_t _reservedAddress15 :1;
				};
			};
			uint8_t _length = 0;
			const uint8_t _maxLength;
			uint8_t * const _buffer;
		public:
			ATTiny427_ExpanderPoller(ATTiny427_ExpanderService* expanderService, uint16_t address, uint8_t maxLength, ATTiny427_ExpanderPoller_callback_t callback);
			~ATTiny427_ExpanderPoller();

			inline bool IsEnabled() const { return _enabled; }
			void SetEnabled(bool enabled);
			inline uint8_t GetLength() const { return _length; }
			inline void SetLength(uint8_t length) 
			{ 
				const bool prevEnabled = IsEnabled();
				if(prevEnabled)
					SetEnabled(false);
				_length = std::min<uint8_t>(length, _maxLength);
				if(prevEnabled)
					SetEnabled(true);
			}
			inline uint16_t GetAddress() const { return _address & 0xBFFF; }
			inline void SetReadIdx(size_t readIdx) { _readIdx = readIdx; _enabled = true; }
			inline void Receive(const uint8_t *data, size_t size, size_t readIdx)
			{
				//if not enabled, skip
				if(IsEnabled() == false)
					return;

				//calculate start indexes
				const size_t bufferIdxStart = ((readIdx - _readIdx) & (0x1 << (sizeof(size_t) * 8 - 1))) ? 0 : readIdx - _readIdx;
				const size_t dataIdxStart = ((_readIdx - readIdx) & (0x1 << (sizeof(size_t) * 8 - 1))) ? 0 : _readIdx - readIdx;

				//if past data, skip
				if(bufferIdxStart >= _length || dataIdxStart >= size)
					return;

				const size_t dataLength = size > dataIdxStart ? std::min<size_t>(_length - bufferIdxStart, size - dataIdxStart) : 0;

				//copy data to buffer
				std::memcpy(&_buffer[bufferIdxStart], &data[dataIdxStart], dataLength);

				//if full, call callback
				if(bufferIdxStart + dataLength >= _length)
					_callback((uint8_t*)_buffer);
			}
		};

	protected:
		struct ATTiny427_ExpanderCommand
		{
			public:
				ATTiny427_ExpanderCommand() : Address(0), Data(0), Type(ATTiny427_ExpanderCommandType_Executed) { }
				union
				{
					ATTiny427_ExpanderService::ATTiny427_ExpanderPoller *Poller;
					struct
					{
						uint16_t Address;
						uint8_t Data;
					};
				};
				ATTiny427_ExpanderCommandType Type;
		};

		template<typename T, std::size_t... Is>
		static std::array<T, sizeof...(Is) + 1> initRegistersPollersImpl(ATTiny427_ExpanderService *expanderService, index_sequence<Is...>)
		{
			return std::array<T, sizeof...(Is) + 1>{{ 
				expanderService, 
				uint8_t(Is)... 
			}};
		}
		template<typename T, std::size_t N>
		static std::array<T, N> initRegistersPollers(ATTiny427_ExpanderService *expanderService)
		{
			return initRegistersPollersImpl<T>(expanderService, make_index_sequence<N - 1>{});
		}

		std::array<ATTiny427_ExpanderService::Attiny427_ExpanderRegister, 100> _registers = initRegistersPollers<ATTiny427_ExpanderService::Attiny427_ExpanderRegister, 100>(this);
		std::array<ATTiny427_ExpanderService::ATTiny427_ExpanderCommand, 256> _commands;
		std::list<ATTiny427_ExpanderService::ATTiny427_ExpanderPoller *> _pollers;
		size_t _readIdx = 0;
		size_t _writeIdx = 0;
		uint8_t _commandHead = 0;
		uint8_t _commandTail = 0;
		
	public:
		const ATTiny427_ExpanderComm Comm;

		ATTiny427_ExpanderService(ATTiny427_ExpanderComm comm);

		//dont call from isr
		inline ATTiny427_ExpanderService::Attiny427_ExpanderRegister &GetRegister(uint16_t address)
		{
			uint8_t index = 0;
			ATTiny427_ExpanderService::Attiny427_ExpanderRegister *reg = nullptr;
			//need to make sure only the corerct number of registers are created. otherwise the last created registers will overlap with each other
			while(((reg = &_registers[++index])->GetAddress() != 0x33FF && reg->GetAddress() != address) && index < _registers.size() - 1) ;
			reg->SetAddress(address);
			return *reg;
		}

		inline void Write(uint16_t address, uint8_t value)
		{
			//grab a command slot. while loop make sures an isr adding commands wont cause issues
			uint8_t originalTailMinus1 = _commandTail - 1;
			uint8_t tail;
			while(_commands[tail = _commandTail++].Type != ATTiny427_ExpanderCommandType_Executed && originalTailMinus1 != tail);

			//if we got a slot, queue the write
			if(originalTailMinus1 != tail) 
			{
				auto &command = _commands[tail];
				command.Address = address;
				command.Data = value;
				command.Type = ATTiny427_ExpanderCommandType_Write;
			}
		}

		size_t Transmit(uint8_t data[256]);
		void Receive(const uint8_t *data, size_t size);
	};
}
#endif
