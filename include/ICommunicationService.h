#include "stdint.h"
#include "CallBack.h"
#include <forward_list>
#include <list>

#ifndef ICOMMUNICATIONSERVICE_H
#define ICOMMUNICATIONSERVICE_H
namespace EmbeddedIOServices
{
	typedef std::function<bool(void *data, size_t length)> communication_callback_t;
    struct CommunicationHandler 
    {
        communication_callback_t CallBack;
        
		CommunicationHandler(communication_callback_t callBack) : 
			CallBack(callBack)
		{ }
    };
	typedef std::list<CommunicationHandler *> CommunicationHandlerList;

	class ICommunicationService
	{
	protected:
		CommunicationHandlerList _handlerList;
	public:
		void RegisterHandler(CommunicationHandler *handler);
		void UnRegisterHandler(CommunicationHandler *handler);
        void Receive(void *data, size_t length);
        virtual void Send(void *data, size_t length) = 0;
	};
}
#endif
