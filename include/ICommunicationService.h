#include "stdint.h"
#include <functional>
#include <map>

#ifndef ICOMMUNICATIONSERVICE_H
#define ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	class ICommunicationHandler;
	typedef std::function<void(const void *data, size_t length)> communication_send_callback_t;
	typedef std::function<size_t(communication_send_callback_t, void *data, size_t length)> communication_receive_callback_t;
	typedef std::map<void *, communication_receive_callback_t *> communication_receive_callback_map_t;

	class ICommunicationService
	{
	protected:
		//// list of receive callback
		communication_receive_callback_map_t _receiveCallBackMap;
		
	public:
		/**
		 * @brief Called when the service receives data. This will loop through all of the register callbacks 
		 * until there is either no data left to be processed, or there are no callbacks that can handle the data
		 * @param data A pointer to the data that was received
		 * @param length Length of that data that was received
		 * @return size_t Number of bytes parsed from data.
		 */
        size_t Receive(void *data, size_t length);

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
		 * @brief Register a handler with the service that will be called when the service receives data.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void RegisterHandler(ICommunicationHandler *handler);

		/**
		 * @brief Unregister a callback with the service.
		 * @param receiveCallBack A pointer to the callback function
		 */
		void UnRegisterHandler(ICommunicationHandler *handler);

		/**
		 * @brief Sends data on the communication bus.
		 * @param data A pointer to the data to be transmitted. It should be assumed that the pointer is invalid 
		 * after the method has returned
		 * @param length Length of that data to be transmitted
		 */
        virtual void Send(const void *data, size_t length) = 0;
	};

	class ICommunicationHandler 
	{
	public:
		/**
		 * @brief Called by the communication service when data is received
		 * @param data A pointer to the data that was received
		 * @param length Length of that data that was received
		 * @return size_t Number of bytes parsed from data.
		 */
        virtual size_t Receive(communication_send_callback_t sendCallBack, void *data, size_t length) = 0;
	};
}
#endif
