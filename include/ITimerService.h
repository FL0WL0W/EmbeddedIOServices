#include "stdint.h"
#include "CallBack.h"
#include <forward_list>
#include <list>

#ifndef ITIMERSERVICE_H
#define ITIMERSERVICE_H
namespace EmbeddedIOServices
{
	typedef uint32_t tick_t;

	/**
	 * @brief Structure that contains the details of the Task and contains the CallBack function
	 */
	struct Task
	{
		public:
		//// function containing the CallBack that will be executed at ScheduledTick
		callback_t CallBack;
		//// The tick when the Task will be executed. Only valid when scheduled or during callback.
		tick_t ScheduledTick;
		//// The tick when the Task was executed
		volatile tick_t ExecutedTick;
		//// Whether or not the task is scheduled
		volatile bool Scheduled : 1;
		//// Whether or not to delete the task after the CallBack is executed
		const bool DeleteAfterExecution : 1;

		//// Creates a task from a callback with explicit direction on wether or not to delete the task after it has been executed.
		Task(callback_t callBack, bool deleteAfterExecution) : 
			CallBack(callBack),
			ScheduledTick(0),
			ExecutedTick(0),
			Scheduled(false),
			DeleteAfterExecution(deleteAfterExecution)
		{ }

		//// Creates a task from a callback. Implicitly assumes that task will not be deleted after being executed.
		Task(callback_t callBack) : 
			Task(callBack, false)
		{ }

	};

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
	/**
	 * @brief Structure that contains the pointer to the task and the Tick at which to schedule it
	 */
	struct ScheduleRequest
	{
		public:
		//// pointer to the task to be scheduled/unscheduled
		Task *TaskToSchedule;
		//// tick when the task is to be scheduled to
		tick_t Tick;

		//// Constructor that sets up the 2 variables
		ScheduleRequest(Task *task, tick_t tick) : TaskToSchedule(task), Tick(tick) { }
	};

	typedef std::forward_list<ScheduleRequest> ScheduleRequestList;
	typedef std::forward_list<Task *> RemoveRequestList;
#endif

	typedef std::list<Task *> TaskList;

	class ITimerService
	{
	private:
#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		//// stores the schedule task requests for the next time the schedul is unlocked
		ScheduleRequestList _scheduleRequestList;
		//// stores the remove task requests for the next time the schedul is unlocked
		RemoveRequestList _removeRequestList;
		//// if reading the list then the schedule is locked
		bool _scheduleLock = false;
#endif
		//// the latency it takes between when a task is scheduled and what is sent to the timer
		uint16_t _latency = 0;
		//// the minimum time it takes to execute a task after scheduling
		uint16_t _minTick = 0;

		/**
		 * @brief This is a private helper method. It is used to remove all unscheduled tasks from the beginning 
		 * of the list and will return the new beginning of the list.
		 */
		TaskList::iterator RemoveUnscheduledTasksAndReturnBegin();

	protected:
		//// Stores all of the scheduled tasks in order of execution
		TaskList _taskList;
		
		/**
		 * @brief Used to calibrate the latency and the minimum number of ticks that a task can be scheduled in advance.
		 * This should be called at the end of the child abstraction's constructor after all timer setup is complete.
		 */
		virtual void Calibrate() = 0;

		/**
		 * @brief This should be implemented by the child abstraction to set the timer execution tick.
		 * When the timer reaches the tick specified, ReturnCallBack should be called.
		 * @see ReturnCallBack
		 */
		virtual void ScheduleCallBack(const tick_t tick) = 0;

		/**
		 * @brief This should be called by the child abstraction when the timer reaches the tick specified
		 * in protected method ScheduleCallBack.
		 * @see ScheduleCallBack
		 */
		void ReturnCallBack();

	public:
		/**
		 * @brief This should be implemented by the child abstraction and should return the current tick
		 * @return tick_t of the current tick
		 */
		virtual tick_t GetTick() = 0;
		
		/**
		 * @brief This should be implemented by the child abstraction and should return the number of
		 * ticks per second (frequency) that the tick increments.
		 * @return tick_t of the number of ticks per second
		 */
		virtual tick_t GetTicksPerSecond() = 0;

		/**
		 * @brief Schedules a callback to be called at the specified tick. This callback is non recurring.
		 * Callback cannot be unscheduled or rescheduled.
		 * @param callBack The callback function
		 * @param tick The tick where the callback function should be executed
		 */
		void ScheduleCallBack(callback_t, tick_t);

		/**
		 * @brief Schedules a task to be called at the specified tick. Tasks are non recurring and will be 
		 * deleted if DeleteAfterExecution is set durring the creation of the task. Tasks may be unscheduled 
		 * by calling the UnScheduleTask method or may be rescheduled by calling this method again.
		 * @param task pointer to the task to be scheduled
		 * @param tick The tick where task should be executed
		 */
		void ScheduleTask(Task *, tick_t);

		/**
		 * @brief Unschedules a task. Tasks may be rescheduled by calling the ScheduleTask method.
		 * @param task pointer to the task to be unscheduled
		 */
		void UnScheduleTask(Task *);

#ifdef ALLOW_TASK_TO_SCHEDULE_IN_CALLBACK
		/**
		 * @brief Flush the schedule requests list. This is used whenever tasks are allowed to be scheduled during a callback.
		 * This function incorporates some locking features to allow safe memory modifications of the lists.
		 */
		void FlushScheduleRequests();
#endif
	
		/**
		 * @brief This function is to compare if one tick is less than another in terms of its execution time.
		 * For example, 4294967200 is less than 10
		 * @param i tick of first value
		 * @param j tick of second value
		 * @return bool. True if i is less than j.
		 */
		constexpr static bool TickLessThanTick(const tick_t i, const tick_t j)
		{
			return (i - j) & (static_cast<tick_t>(1)<<(sizeof(tick_t) * 8 - 1));
		}

		/**
		 * @brief This function is the same as TickLessThanTick, but also checks if they are equal
		 * @param i tick of first value
		 * @param j tick of second value
		 * @return bool. True if i is less than or equal to j.
		 */
		constexpr static bool TickLessThanEqualToTick(const tick_t i, const tick_t j)
		{
			return !TickLessThanTick(j, i);
		}
	};
}
#endif
