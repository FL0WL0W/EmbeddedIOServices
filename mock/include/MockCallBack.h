#include "ICallBack.h"

#ifndef MOCKCALLBACK_H
#define MOCKCALLBACK_H
namespace EmbeddedIOServices
{
	class MockCallBack : public ICallBack
	{
	public:
		MOCK_METHOD0(Execute, void());
	};
}
#endif