#include "ICommunicationService.h"

#ifndef COMMUNICATIONHANDLER_PREFIX_H
#define COMMUNICATIONHANDLER_PREFIX_H
namespace EmbeddedIOServices
{
    struct communication_prefix_receive_callback_t
    {
    public:
        communication_prefix_receive_callback_t(const communication_receive_callback_t *callBack, const void *prefix, const size_t prefixLength, const bool handlesData) :
            CallBack(callBack),
            Prefix(prefix),
            PrefixLength(prefixLength),
			HandlesData(handlesData)
        {}

        const communication_receive_callback_t *CallBack;
        const void *Prefix;
        const size_t PrefixLength;
		const bool HandlesData;
    };
	typedef std::map<void *, communication_prefix_receive_callback_t> communication_prefix_receive_callback_map_t;

	class CommunicationHandler_Prefix : public ICommunicationHandler
	{
	protected:
		//// list of receive callback
		communication_prefix_receive_callback_map_t _receiveCallBackMap;
		
	public:
		// /**
		//  * @brief Called by the communication service when data is received
		//  * @param data A pointer to the data that was received
		//  * @param length Length of that data that was received
		//  * @return size_t Number of bytes parsed from data.
		//  */
        size_t Receive(communication_send_callback_t sendCallBack, void *data, size_t length);

		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void RegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack, const void *prefix, const size_t prefixLength, const bool handlesData = true);

		/**
		 * @brief Unregister a callback with the service.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void UnRegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack);

		/**
		 * @brief Register a handler with the service that will be called when the service receives data.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void RegisterHandler(ICommunicationHandler *handler, const void *prefix, const size_t prefixLength, const bool handlesData = true);

		/**
		 * @brief Unregister a callback with the service.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void UnRegisterHandler(ICommunicationHandler *handler);
	};
}
#endif