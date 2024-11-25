#include "stdint.h"
#include <functional>
#include <memory>
#include <map>

#ifndef ICANSERVICE_H
#define ICANSERVICE_H
namespace EmbeddedIOServices
{
	struct CANData_t 
	{
	public:
		uint8_t Data[8];
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

		CANIdentifier_t operator&(CANIdentifier_t a) const
		{
			return { static_cast<const uint32_t>(CANIdentifier & a.CANIdentifier), static_cast<const uint8_t>(CANBusNumber & a.CANBusNumber) };
		}
	};
	typedef std::function<void(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)> can_send_callback_t;
	typedef std::function<void(can_send_callback_t, const CANData_t data, const uint8_t dataLength)> can_receive_callback_t;
	typedef std::function<void(can_send_callback_t, const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength)> can_receive_mask_callback_t;
	struct can_receive_callback_mask_t 
	{
		CANIdentifier_t Identifier; 
		CANIdentifier_t Mask;
		can_receive_mask_callback_t CallBack;
	};
	typedef uint32_t can_receive_callback_id_t;
	
	class ICANService
	{
	protected:
		//// map of receive callback
		std::multimap<const CANIdentifier_t, std::shared_ptr<can_receive_callback_t>> _receiveCallBackIdentifierIndex;
		std::multimap<const can_receive_callback_id_t, std::shared_ptr<can_receive_callback_t>> _receiveCallBackMap;
		std::multimap<const can_receive_callback_id_t, can_receive_callback_mask_t> _receiveCallBackMaskMap;
		can_receive_callback_id_t _nextId = 0;
		
	public:
		/**
		 * @brief Called when the service receives data. This will loop through all of the register callbacks 
		 * until there is either no data left to be processed
		 * @param identifier the identifier the data is received on
		 * @param data A 8 byte array of the data that was received
		 * @param dataLength Length in bytes of data
		 */
        void Receive(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength);

		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param identifier the identifier to filter for the callback function
		 * @param mask The mask for which to match the identifier
		 * @param receiveCallBack A pointer to the callback function
		 * @return Id which the receiveCallBack has been registered to
		 */
		can_receive_callback_id_t RegisterReceiveCallBack(const CANIdentifier_t identifier, const CANIdentifier_t mask, can_receive_mask_callback_t receiveCallBack);

		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param identifier the identifier to filter for the callback function
		 * @param receiveCallBack A pointer to the callback function
		 * @return Id which the receiveCallBack has been registered to
		 */
		can_receive_callback_id_t RegisterReceiveCallBack(const CANIdentifier_t identifier, can_receive_callback_t receiveCallBack);

		/**
		 * @brief Unregister all callbacks with the service matching the pointer to the callback function.
		 * @param receiveCallBackId Id which the receiveCallBack has been registered to
		 */
		void UnRegisterReceiveCallBack(can_receive_callback_id_t receiveCallBackId);
		
		/**
		 * @brief Unregister all callbacks with the service matching the identifier and mask
		 * @param identifier the identifier to filter for the callback function
		 * @param mask the mask for which to match the identifier
		 */
		void UnRegisterReceiveCallBack(const CANIdentifier_t identifier, const CANIdentifier_t mask);

		/**
		 * @brief Unregister all callbacks with the service matching the identifier
		 * @param identifier the identifier to filter for the callback function
		 */
		void UnRegisterReceiveCallBack(const CANIdentifier_t identifier);

		/**
		 * @brief Sends data on the can bus.
		 * @param identifier the identifier to send the CAN data on
		 * @param data A 8 byte array of the data to be sent
		 * @param dataLength Length in bytes of data
		 */
        virtual void Send(const CANIdentifier_t identifier, const CANData_t data, const uint8_t dataLength) = 0;
	};
}
#endif
