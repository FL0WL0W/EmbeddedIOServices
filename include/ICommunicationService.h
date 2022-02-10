#include "stdint.h"
#include "CallBack.h"
#include <forward_list>
#include <list>

#ifndef ICOMMUNICATIONSERVICE_H
#define ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	typedef std::function<size_t(void *data, size_t length)> communication_receive_callback_t;
	typedef std::list<communication_receive_callback_t *> communication_receive_callback_list_t;

	class ICommunicationService
	{
	protected:
		//// list of receive callback
		communication_receive_callback_list_t _receiveCallBackList;
	public:
		/**
		 * @brief Register a callback with the service that will be called when the service receives data.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void RegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack);

		/**
		 * @brief Unregister a callback with the service.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void UnRegisterReceiveCallBack(communication_receive_callback_t *receiveCallBack);

		/**
		 * @brief Called when the service receives data. This will loop through all of the register callbacks 
		 * until there is either no data left to be processed, or there are no callbacks that can handle the data
		 * @param data A pointer to the data that was received
		 * @param length Length of that data that was received
		 * @return size_t Number of bytes parsed from data.
		 */
        size_t Receive(void *data, size_t length);

		/**
		 * @brief Sends data on the communication bus.
		 * @param data A pointer to the data to be transmitted. It should be assumed that the pointer is invalid 
		 * after the method has returned
		 * @param length Length of that data to be transmitted
		 */
        virtual void Send(void *data, size_t length) = 0;
	};
}
#endif
