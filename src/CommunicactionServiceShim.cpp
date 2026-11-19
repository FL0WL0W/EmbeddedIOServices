#include "CommunicationServiceShim.h"

namespace EmbeddedIOServices
{
    CommunicationServiceShim::CommunicationServiceShim(std::function<void(const void *data, size_t length)> sendFunction, bool ready) :
        SendFunction(sendFunction),
        IsReady(ready)
    {
    }
        
    void CommunicationServiceShim::Send(const void *data, size_t length)
    {
        SendFunction(data, length);
    }

    bool CommunicationServiceShim::Ready()
    {
        return IsReady;
    }
}