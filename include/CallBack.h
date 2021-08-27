#include <functional>

#ifndef CALLBACK_H
#define CALLBACK_H
namespace EmbeddedIOServices
{
	typedef std::function<void()> callback_t;
}
#endif