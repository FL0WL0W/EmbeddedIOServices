#include "stdint.h"
#include "CallBack.h"
#include <forward_list>
#include <list>

#ifndef ICOMMUNICATIONSERVICE_H
#define ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	typedef std::function<size_t(void *data, size_t length)> communication_callback_t;
	typedef std::list<communication_callback_t *> communication_receive_callback_list_t;

	class ICommunicationService
	{
	protected:
		communication_receive_callback_list_t _handlerList;
	public:
		void RegisterHandler(communication_callback_t *handler);
		void UnRegisterHandler(communication_callback_t *handler);
        size_t Receive(void *data, size_t length);
        virtual void Send(void *data, size_t length) = 0;
	};
}
#endif
