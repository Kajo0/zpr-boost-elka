#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	Timer::Timer(const Timer::Duration & frequency) : frequency_(frequency)
	{}

	void Timer::AddListener(boost::function<void ()> listener)
	{
		listeners_.push_back(listener);
	}

	Timer::TimePoint Timer::Now() const
	{
		return boost::chrono::high_resolution_clock::now();
	}

	Timer::Duration Timer::Elapsed(const TimePoint & since) const
	{
		return Now() - since;
	}

	void Timer::operator()()
	{
		/*DEBUG*/ std::cout << "Timer starts." << std::endl;
		while(!boost::this_thread::interruption_requested())
		{
			/**/ TimePoint prev = Now();
			TimePoint nextTick = Now() + frequency_;
			while(nextTick > Now())
			{
				boost::this_thread::yield();
				boost::this_thread::sleep(boost::posix_time::milliseconds(1)); // DEBUG! tylko zeby zmniejszyc zuzycie procka, trzeba ladniej wymyslic
			}

			/*DEBUG*/// std::cout << "Timer:\t" << boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - prev) << " -> " << Now() << std::endl;

			BOOST_FOREACH(boost::function<void ()> & current, listeners_)
				current();
		}
		/*DEBUG*/ std::cout << "Timer gracefully ends." << std::endl;
	}
}