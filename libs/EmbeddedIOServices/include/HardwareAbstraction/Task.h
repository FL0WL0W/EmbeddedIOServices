#include <list>

#ifndef ITASK_H
#define ITASK_H
namespace HardwareAbstraction
{
	class ICallBack
	{
	public:
		virtual void Execute() = 0;
	};
	
	struct CallBack : public ICallBack
	{
		CallBack(void(*callBackPointer)(void *), void *parameters)
		{
			CallBackPointer = callBackPointer;
			Parameters = parameters;
		}

		void Execute()
		{
			CallBackPointer(Parameters);
		}

		void(*CallBackPointer)(void *);
		void *Parameters;
	};
	
	class CallBackGroup : public ICallBack
	{
	protected:
		std::list<ICallBack *> _callBackList;
	public:		
		void Execute();
		void Add(ICallBack *callBack);
		void Add(void(*callBackPointer)(void *), void *parameters);
		void Remove(ICallBack *callBack);
		void Clear();
	};

	struct Task
	{
	public:
		Task() {}
		Task(void(*callBack)(void *), void *parameters, bool deleteOnExecution)
		{
			CallBackInstance = new CallBack(callBack, parameters);
			DeleteOnExecution = deleteOnExecution;
		}
		Task(ICallBack *callBack, bool deleteOnExecution)
		{
			CallBackInstance = callBack;
			DeleteOnExecution = deleteOnExecution;
		}

		void Execute()
		{
			CallBackInstance->Execute();
		}

		ICallBack *CallBackInstance;
		bool DeleteOnExecution;
		//only let TimerService edit these values
		unsigned int Tick;
	};
}
#endif