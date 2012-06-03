#ifndef TIMER_HPP
#define TIMER_HPP

#include <deque>
#include <boost/chrono.hpp>
#include <boost/function.hpp>

namespace zpr
{
	typedef boost::chrono::high_resolution_clock::time_point TimePoint;
	typedef boost::chrono::high_resolution_clock::duration Duration;
	
	/**
	 * Class representing single listener of timer events with its individual frequency.
	 * Uses observer pattern.
	 */
	class TimerListener
	{
	public:
		/**
		 * Constructor creating single listener.
		 * @param listener	function runned when event occurs - takes time since last occurence as parameter (approximately equals to frequency)
		 * @param frequency	timeout between two listener calls
		 */
		TimerListener(const boost::function<void (long long int)> & listener, const Duration & frequency);

		/**
		 * For each timers tick checks whether timeout between calls has ended.
		 * @param now	new tick considered as microseconds since simulation start
		 * @return	true if handler called (timeout passed), false otherwise
		 */
		bool check(const TimePoint & now);

	private:
		/**
		 * Point in time representing last listener call.
		 */
		TimePoint previousTrigger_;
		
		/**
		 * Timeout between subsequent listener calls.
		 */
		Duration frequency_;

		/**
		 * Events listener. Function called each time when timeout passed.
		 */
		boost::function<void (long long int)> listener_;
	};

	/**
	 * Main timer class. Describes one general timer with fixed resolution
	 * which has unlimited amount of attached TimerListener. After each minimal
	 * tick, Timer makes listeners check whether theirs individual timeouts passed.
	 * That approach enables different timeouts to be used accordingly to different 
	 * timer events handlers.
	 */
	class Timer
	{
	public:
		/**
		 * Default constructor. Fixes Timer's start time.
		 */
		Timer();

		/**
		 * Returns time since simulation start.
		 * @return	time since simulation's start in microseconds.
		 */
		long long int simulationTime() const;
		
		/**
		 * Adds new TimerListener to listeners set.
		 * @param listener New handler assigned to the Timer.
		 */
		void AddListener(const TimerListener & listener);

		/**
		 * Timer's entry point - main thread's function.
		 * Implements Timers loop and launching events.
		 * When finished, Timer is no longer usable.
		 */
		void operator()();
		
	private:
		/**
		 * Returns point in time representing current moment. Depends on clock's resolution.
		 * @return Point in time as internall representation, only inside class usage.
		 */
		TimePoint now() const;
		
		/**
		 * Time of simulation's start. 
		 */
		TimePoint started_;

		/**
		 * Set of Timer's listeners.
		 */
		std::deque<TimerListener> listeners_;
	};
}
#endif // TIMER_HPP