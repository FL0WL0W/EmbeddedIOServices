#include <list>
#include "stdint.h"

#ifndef ITASK_H
#define ITASK_H
namespace HardwareAbstraction
{
	class ICallBack
	{
	public:
		virtual void Execute() = 0;
	};
	
	struct CallBackWithParameters : public ICallBack
	{
		CallBackWithParameters(void(*callBackPointer)(void *), void *parameters)
		{
			CallBackPointer = callBackPointer;
			Parameters = parameters;
		}

		void Execute() override
		{
			CallBackPointer(Parameters);
		}

		void(*CallBackPointer)(void *);
		void *Parameters;
	};
	
	template<typename INSTANCETYPE>
	struct CallBack : public ICallBack
	{
		CallBack(INSTANCETYPE *instance, void(INSTANCETYPE::*function)())
		{
			Instance = instance;
			Function = function;
		}

		void Execute() override
		{
			(Instance->*Function)();
		}

		INSTANCETYPE *Instance;
		void(INSTANCETYPE::*Function)();
	};
	
	class CallBackGroup : public ICallBack
	{
	protected:
		std::list<ICallBack *> _callBackList;
	public:		
		void Execute() override;
		void Add(ICallBack *callBack);
		void Remove(ICallBack *callBack);
		void Clear();
	};
}
#endif
