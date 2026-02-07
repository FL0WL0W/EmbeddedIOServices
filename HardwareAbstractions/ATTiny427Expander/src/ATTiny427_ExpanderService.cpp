#include "ATTiny427_ExpanderService.h"
#include <cstring>
#include <algorithm>

#ifdef ATTINY427_EXPANDERSERVICE_H
namespace EmbeddedIOServices
{
	ATTiny427_ExpanderService::Attiny427_ExpanderRegister::Attiny427_ExpanderRegister(ATTiny427_ExpanderService* expanderService) :
		_expanderService(expanderService)
	{
	}
	ATTiny427_ExpanderService::Attiny427_ExpanderRegister::Attiny427_ExpanderRegister(uint8_t index) :
		_address(0x33FF),
		_data(0),
		_index(index)
	{
	}
	ATTiny427_ExpanderService::Attiny427_ExpanderRegister& ATTiny427_ExpanderService::Attiny427_ExpanderRegister::operator=(uint8_t value)
	{
		getExpanderService()->Write(_address & 0xBFFF, _data = value);
		_dataValid = true;
		return *this;
	}

	ATTiny427_ExpanderService::ATTiny427_ExpanderPoller::ATTiny427_ExpanderPoller(ATTiny427_ExpanderService* expanderService, uint16_t address, uint8_t maxLength, ATTiny427_ExpanderPoller_callback_t callback) :
		_expanderService(expanderService),
		_callback(callback),
		_address(address & 0xBFFF), //clear enable bit
		_maxLength(maxLength),
		_buffer(new uint8_t[maxLength])
	{
	}
	ATTiny427_ExpanderService::ATTiny427_ExpanderPoller::~ATTiny427_ExpanderPoller()
	{
		SetEnabled(false);

		//grab a command slot. while loop make sures an isr adding commands wont cause issues
		const uint8_t originalTail = _expanderService->_commandTail;
		uint8_t tail;
		while(_expanderService->_commands[tail = (_expanderService->_commandTail)++].Type != ATTiny427_ExpanderCommandType_Executed && originalTail != tail);

		//it doesn't matter if we got a slot or not, we are deleting so just send a ReadDelete command
		auto &command = _expanderService->_commands[tail];
		command.Poller = this;
		command.Type = ATTiny427_ExpanderCommandType_ReadDelete;

		delete[] _buffer;
	}
	void ATTiny427_ExpanderService::ATTiny427_ExpanderPoller::SetEnabled(bool enabled)
	{
		//remove any pending read commands for this poller
		uint8_t head =  _expanderService->_commandHead;
		while(head != _expanderService->_commandTail)
		{
			ATTiny427_ExpanderService::ATTiny427_ExpanderCommand &command = _expanderService->_commands[head++];
			if(command.Type == ATTiny427_ExpanderCommandType_Read && command.Poller == this)
				command.Type = ATTiny427_ExpanderCommandType_Skip;
		}

		//if disabling, just set flag and return
		if(enabled == false)
		{
			_enabled = false;
			return;
		}

		//grab a command slot. while loop make sures an isr adding commands wont cause issues
		const uint8_t originalTailMinus1 = _expanderService->_commandTail - 1;
		uint8_t tail;
		while(_expanderService->_commands[tail = (_expanderService->_commandTail)++].Type != ATTiny427_ExpanderCommandType_Executed && originalTailMinus1 != tail);

		//if we got a slot, queue the read. the read will enable the poller when processed
		if(originalTailMinus1 != tail) 
		{
			auto &command = _expanderService->_commands[tail];
			command.Poller = this;
			_readIdx = _expanderService->_readIdx - _length; //set the read idx to current - length so any pending reads are voided
			command.Type = ATTiny427_ExpanderCommandType_Read;
		}
	}

	ATTiny427_ExpanderService::ATTiny427_ExpanderService(ATTiny427_ExpanderComm comm) : 
		_writeIdx((comm == ATTiny427_ExpanderComm_SPI || comm == ATTiny427_ExpanderComm_SPIAlternate) ? 2 : 0),
		Comm(comm)
	{
	}
	size_t ATTiny427_ExpanderService::Transmit(uint8_t data[256])
	{
		//remove all pollers that have been deleted
		for(uint8_t commandIndex = _commandHead; commandIndex != _commandTail; commandIndex++)
		{
			auto &command = _commands[commandIndex];
			if(command.Type == ATTiny427_ExpanderCommandType_ReadDelete)
			{
				_pollers.remove(command.Poller);
				command.Type = ATTiny427_ExpanderCommandType_Skip;
			}
		}

		//remove all disabled pollers
		auto pollerIt = _pollers.begin();
		while(pollerIt != _pollers.end() && (*pollerIt)->IsEnabled() == false) _pollers.remove(*(pollerIt++)); 

		// process queued commands and pollers
		const size_t capacity = 256 - ((Comm == ATTiny427_ExpanderComm_SPI || Comm == ATTiny427_ExpanderComm_SPIAlternate) ? 2 : 0);
		size_t idx = 0;
		size_t pollerStartIdx = 0;
		size_t writeCmdIdx = -1;
		uint16_t currentWriteAddress = 0x00;
		uint8_t currentReadAddressHigh = 0x00;
		pollerIt = _pollers.begin();

		while(_commandHead != _commandTail)
		{
			auto &command = _commands[_commandHead];

			//skip and mark executed
			if(command.Type == ATTiny427_ExpanderCommandType_Skip)
			{
				command.Type = ATTiny427_ExpanderCommandType_Executed;
				_commandHead++;
				continue;
			}

			//stop processing if we hit an unfinished command
			if(command.Type == ATTiny427_ExpanderCommandType_Executed)
			{
				break;
			}

			if(pollerIt != _pollers.end())
			{
				const uint16_t addr = (*pollerIt)->GetAddress();
				int16_t length = (*pollerIt)->GetLength();

				if(length > 0 && (pollerStartIdx == 0 || pollerStartIdx < idx + ((addr < 0xFF || (addr >> 8) == currentReadAddressHigh)? 2 : 3)))
				{
					uint8_t cmd = 0x00; //read
					if(addr <= 0xFF)
						cmd = 0x20 | 0x10; //read 8 bit address high byte 0
					else if((addr >> 8) == currentReadAddressHigh)
						cmd = 0x20; //read 8 bit address high byte same

					if(cmd == 0x00)
					{
						if((idx + 3 + length) > capacity)
							break;
						data[idx++] = cmd | (length & 0x0F);
						data[idx++] = (addr >> 8) & 0xFF;
					}
					else
					{
						if((idx + 2 + length) > capacity)
							break;
						data[idx++] = cmd | (length & 0x0F);
					}
					data[idx++] = addr & 0xFF;
					pollerStartIdx = idx + length;
					currentReadAddressHigh = addr >> 8;

					(*pollerIt)->SetReadIdx(_writeIdx + idx);
					
					length -= 0x0F;
					while(length > 0)
					{
						if(length > 0x3F)
							data[idx++] = 0x40 | 0x3F;
						else
							data[idx++] = 0x10 | length;
						length -= 0x3F;
					}

					pollerIt++;
					writeCmdIdx = -1; // reset write command coalescing
					continue;
				}
			}
			if(command.Type == ATTiny427_ExpanderCommandType_Write)
			{
				const uint16_t addr = command.Address;
				if(addr == currentWriteAddress && writeCmdIdx != -1)
				{
					// can be combined into previous write
					const bool isContinue = data[writeCmdIdx] & 0x40;
					const uint8_t count = isContinue? data[writeCmdIdx] & 0x3F : data[writeCmdIdx] & 0x0F;
					if((isContinue && count == 0x3F) || (!isContinue && count == 0x0F))
					{
						if((idx + 2) > capacity)
							break;
						//previous write full, need to do a continue cmd
						data[writeCmdIdx = idx++] = 0x40; // continue
					}
					if((idx + 1) > capacity)
						break;
				}
				else
				{
					uint8_t cmd = 0x80; //write
					if(addr <= 0xFF)
						cmd = 0x80 | 0x20 | 0x10; //write 8 bit address high byte 0
					else if((addr & 0xFF00) == (currentWriteAddress & 0xFF00))
						cmd = 0x80 | 0x20; //write 8 bit address high byte same

					if(cmd == 0x80)
					{
						if((idx + 4) > capacity)
							break;
						data[writeCmdIdx = idx++] = cmd;
						data[idx++] = (addr >> 8) & 0xFF;
					}
					else
					{
						if((idx + 3) > capacity)
							break;
						data[writeCmdIdx = idx++] = cmd;
					}
					data[idx++] = addr & 0xFF;
					currentWriteAddress = addr;
				}
				data[writeCmdIdx] += 1; // increase count
				data[idx++] = command.Data;
				currentWriteAddress++;
			}
			else if(command.Type == ATTiny427_ExpanderCommandType_Read)// read (poller attachment)
			{
				// add to active pollers
				_pollers.push_back(command.Poller);
				if(pollerIt == _pollers.end()) 
					pollerIt--;
			}
			_commandHead++;
			command.Type = ATTiny427_ExpanderCommandType_Executed;
		}

		while(pollerIt != _pollers.end())
		{
			const uint16_t addr = (*pollerIt)->GetAddress();

			while(pollerStartIdx != 0 && (pollerStartIdx > idx + ((addr < 0xFF || (addr >> 8) == currentReadAddressHigh)? 2 : 3)))
				data[idx++] = 0x00;

			int16_t length = (*pollerIt)->GetLength();
			if(length == 0)
			{
				pollerIt++;
				continue;
			}

			uint8_t cmd = 0x00; //read
			if(addr <= 0xFF)
				cmd = 0x20 | 0x10; //read 8 bit address high byte 0
			else if((addr >> 8) == currentReadAddressHigh)
				cmd = 0x20; //read 8 bit address high byte same

			if(cmd == 0x00)
			{
				if((idx + 3 + length) > capacity)
					break;
				data[idx++] = cmd | (length & 0x0F);
				data[idx++] = (addr >> 8) & 0xFF;
			}
			else
			{
				if((idx + 2 + length) > capacity)
					break;
				data[idx++] = cmd | (length & 0x0F);
			}
			data[idx++] = addr & 0xFF;
			pollerStartIdx = idx + length;
			currentReadAddressHigh = addr >> 8;
			(*pollerIt)->SetReadIdx(_writeIdx + idx);

			length -= 0x0F;
			while(length > 0)
			{
				if(length > 0x3F)
					data[idx++] = 0x40 | 0x3F;
				else
					data[idx++] = 0x10 | length;
				length -= 0x3F;
			}

			pollerIt++;
		}

		if(idx == 0)
			return 0;

		if(Comm == ATTiny427_ExpanderComm_SPI || Comm == ATTiny427_ExpanderComm_SPIAlternate)
			pollerStartIdx += 2;

		while(idx < pollerStartIdx) data[idx++] = 0x00; // pad to expected read start

		_writeIdx += idx;
		return idx;
	}
	void ATTiny427_ExpanderService::Receive(const uint8_t *data, size_t size)
	{
		//remove all pollers that have been deleted
		for(uint8_t commandIndex = _commandHead; commandIndex != _commandTail; commandIndex++)
		{
			auto &command = _commands[commandIndex];
			if(command.Type == ATTiny427_ExpanderCommandType_ReadDelete)
			{
				_pollers.remove(command.Poller);
				command.Type = ATTiny427_ExpanderCommandType_Skip;
			}
		}

		for(auto pollerIt = _pollers.begin(); pollerIt != _pollers.end(); pollerIt++ )
			(*pollerIt)->Receive(data, size, _readIdx);
		_readIdx += size;
	}
}
#endif
