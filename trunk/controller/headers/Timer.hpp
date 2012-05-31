#ifndef TIMER_HPP
#define TIMER_HPP

#include <boost/function.hpp>
#include <boost/chrono.hpp>
#include <deque>

namespace zpr
{
	class Timer
	{
	public:
		typedef boost::chrono::high_resolution_clock::time_point TimePoint;
		typedef boost::chrono::high_resolution_clock::duration Duration;

		Timer(const Duration & frequency);
		
		void start();
		void stop();
		void step(long long int elapsed);
		long long int elapsed() const; // microseconds
		
		void AddListener(boost::function<void (long long int)> listener); // microseconds
		void operator()();
		
	private:
		TimePoint Now() const;
		
		bool active_;
		Duration frequency_;
		TimePoint started_;
		std::deque<boost::function<void (long long int)> > listeners_;
	};
}
#endif // TIMER_HPP