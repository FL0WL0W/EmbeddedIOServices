#include "ICommunicationService.h"
#include <map>

#ifndef COMMUNICATIONHANDLER_PREFIX_H
#define COMMUNICATIONHANDLER_PREFIX_H
namespace EmbeddedIOServices
{
    struct communication_prefix_receive_callback_t
    {
    public:
        communication_prefix_receive_callback_t(communication_receive_callback_t callBack, const void *prefix, const size_t prefixLength, const bool handlesData) :
			CallBack(callBack),
            Prefix(prefix),
            PrefixLength(prefixLength),
			HandlesData(handlesData)
        {}
		
		communication_receive_callback_t CallBack;
        const void *Prefix;
        const size_t PrefixLength;
		const bool HandlesData;
    };
	typedef std::list<communication_prefix_receive_callback_t> communication_prefix_receive_callback_list_t;

	class CommunicationHandler_Prefix
	{
	protected:
		// list of receive callback
		communication_prefix_receive_callback_list_t _receiveCallBackList;
		
	public:
		/**
		 * @brief Called by the communication service when data is received
		 * @param data A pointer to the data that was received
		 * @param length Length of that data that was received
		 * @return size_t Number of bytes parsed from data.
		 */
        size_t Receive(communication_send_callback_t sendCallBack, void *data, size_t length);

		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param prefix Prefix to look for before calling callback
		 * @param prefixLength Prefix length
		 * @param receiveCallBack A pointer to the callback function
		 * @return Iterator to the list where receiveCallBack has been registered
		 */
		communication_prefix_receive_callback_list_t::iterator RegisterReceiveCallBack(communication_receive_callback_t receiveCallBack, const void *prefix, const size_t prefixLength, const bool handlesData = true);

		/**
		 * @brief Unregister a callback with the service.
		 * @param receiveCallBackIterator An iterator to the list where receiveCallBack has been registered
		 */
		void UnRegisterReceiveCallBack(communication_prefix_receive_callback_list_t::iterator receiveCallBackIterator);

		/**
		 * @brief Unregister a callback with the service.
		 * @param prefix Prefix to look for before calling callback
		 * @param prefixLength Prefix length
		 */
		void UnRegisterReceiveCallBack(const void *prefix, const size_t prefixLength);
	};
}
#endif
