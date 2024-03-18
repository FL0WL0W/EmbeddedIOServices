#include "stdint.h"
#include <functional>
#include <map>

#ifndef ICANSERVICE_H
#define ICANSERVICE_H
namespace EmbeddedIOServices
{
	struct CANData_t 
	{
	public:
		uint8_t data[8];
	};
	struct CANIdentifier_t 
	{
	public:
		uint32_t CANIdentifier : 29;
		uint8_t CANBusNumber : 3;

		bool operator==(CANIdentifier_t a) const
		{
			return CANBusNumber == a.CANBusNumber && CANIdentifier == a.CANIdentifier;
		}

		bool operator<(CANIdentifier_t a) const
		{
			return CANBusNumber < a.CANBusNumber || (CANBusNumber == a.CANBusNumber && CANIdentifier < a.CANIdentifier);
		}

		bool operator>(CANIdentifier_t a) const
		{
			return CANBusNumber > a.CANBusNumber || (CANBusNumber == a.CANBusNumber && CANIdentifier > a.CANIdentifier);
		}
	};
	typedef std::function<void(const CANIdentifier_t identifier, const CANData_t data)> can_send_callback_t;
	typedef std::function<void(can_send_callback_t, const CANData_t data)> can_receive_callback_t;
	typedef std::multimap<const CANIdentifier_t, can_receive_callback_t> can_receive_callback_map_t;


	class ICANService
	{
	protected:
		//// map of receive callback
		can_receive_callback_map_t _receiveCallBackMap;
		
	public:
		/**
		 * @brief Called when the service receives data. This will loop through all of the register callbacks 
		 * until there is either no data left to be processed
		 * @param identifier the identifier the data is received on
		 * @param data A 8 byte array of the data that was received
		 */
        void Receive(const CANIdentifier_t identifier, const CANData_t data);

		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param identifier the identifier to filter for the callback function
		 * @param receiveCallBack A pointer to the callback function
		 * @return Iterator to the map where receiveCallBack has been registered
		 */
		can_receive_callback_map_t::iterator RegisterReceiveCallBack(const CANIdentifier_t identifier, can_receive_callback_t receiveCallBack);

		/**
		 * @brief Unregister all callbacks with the service matching the pointer to the callback function.
		 * @param receiveCallBackIterator An iterator to the map where receiveCallBack has been registered
		 */
		void UnRegisterReceiveCallBack(can_receive_callback_map_t::iterator receiveCallBackIterator);

		/**
		 * @brief Unregister all callbacks with the service matching the identifier
		 * @param identifier the identifier to filter for the callback function
		 */
		void UnRegisterReceiveCallBack(const CANIdentifier_t identifier);

		/**
		 * @brief Sends data on the can bus.
		 * @param identifier the identifier to send the CAN data on
		 * @param data A 8 byte array of the data to be sent
		 */
        virtual void Send(const CANIdentifier_t identifier, const CANData_t data) = 0;
	};
}
#endif
