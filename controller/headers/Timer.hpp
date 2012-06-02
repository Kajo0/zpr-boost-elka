#ifndef TIMER_HPP
#define TIMER_HPP

#include <boost/function.hpp>
#include <boost/chrono.hpp>
#include <deque>

namespace zpr
{
	typedef boost::chrono::high_resolution_clock::time_point TimePoint;
	typedef boost::chrono::high_resolution_clock::duration Duration;
		
	class TimerListener
	{
	public:
		TimerListener(boost::function<void (long long int)> listener, const Duration & frequency);
		bool check(const TimePoint & now); // microseconds since simulation start

	private:
		TimePoint previousTrigger_;
		Duration frequency_;
		boost::function<void (long long int)> listener_;
	};

	class Timer
	{
	public:
		Timer();
		
		//void start();
		//void stop();
		long long int elapsed() const; // microseconds
		
		void AddListener(TimerListener listener); // microseconds
		void operator()();
		
	private:
		TimePoint now() const;
		
		//bool active_;
		TimePoint started_;
		std::deque<TimerListener> listeners_;
	};
}
#endif // TIMER_HPP