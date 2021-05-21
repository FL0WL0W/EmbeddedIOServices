#include <list>
#include "stdint.h"
#include <tuple>

#ifndef ITASK_H
#define ITASK_H
namespace EmbeddedIOServices
{
	class ICallBack
	{
	public:
		virtual void Execute() = 0;
	};
	
	template<typename INSTANCETYPE, typename... PARAMS>
	class CallBackWithParameters : public ICallBack
	{
		std::tuple<PARAMS...> _params;

		public:
		CallBackWithParameters(INSTANCETYPE *instance, void(INSTANCETYPE::*function)(PARAMS...), PARAMS... params)
		{
			Instance = instance;
			Function = function;
			_params = std::tuple<PARAMS...>(params...);
		}

		void Execute() override
		{
			(Instance->*Function)(std::get<PARAMS>(_params)...);
		}

		INSTANCETYPE *Instance;
		void(INSTANCETYPE::*Function)(PARAMS...);
	};
	
	template<typename INSTANCETYPE>
	class CallBack : public ICallBack
	{
		public:
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
