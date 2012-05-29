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
		TimePoint Now() const;
		Duration Elapsed(const TimePoint & since) const;
		
		void AddListener(boost::function<void ()> listener);
		void operator()();
		
	private:
		Duration frequency_;
		std::deque<boost::function<void ()> > listeners_;
	};
}
#endif // TIMER_HPP