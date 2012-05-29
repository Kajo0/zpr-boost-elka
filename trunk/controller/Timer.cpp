#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	Timer::Timer(const Timer::Duration & frequency) : frequency_(frequency), active_(true)
	{}

	void Timer::start()
	{
		active_ = true;
	}

	void Timer::stop()
	{
		active_ = false;
	}

	void Timer::AddListener(boost::function<void (long long int)> listener)
	{
		listeners_.push_back(listener);
	}

	Timer::TimePoint Timer::Now() const
	{
		return boost::chrono::high_resolution_clock::now();
	}

	void Timer::operator()()
	{
		/*DEBUG*/ std::cout << "Timer starts." << std::endl;
		while(!boost::this_thread::interruption_requested())
		{
			TimePoint prev = Now();
			TimePoint nextTick = prev + frequency_;
			while(nextTick > Now())
			{
				boost::this_thread::yield();
				boost::this_thread::sleep(boost::posix_time::milliseconds(1)); // DEBUG! tylko zeby zmniejszyc zuzycie procka, trzeba ladniej wymyslic
			}

			/*DEBUG*/// std::cout << "Timer:\t" << boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - prev) << " -> " << Now() << std::endl;
			if(active_)
			{
				long long int elapsed = boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - prev).count();
				BOOST_FOREACH(boost::function<void (long long int)> & current, listeners_)
					current(elapsed);
			}
		}
		/*DEBUG*/ std::cout << "Timer gracefully ends." << std::endl;
	}
}