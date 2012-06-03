#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	TimerListener::TimerListener(const boost::function<void (long long int)> & listener, const Duration & frequency) : listener_(listener), frequency_(frequency)
	{}

	bool TimerListener::check(const TimePoint & now)
	{
		if((now - previousTrigger_) > frequency_)
		{
			listener_(boost::chrono::duration_cast<boost::chrono::microseconds>(now - previousTrigger_).count());
			previousTrigger_ = now;
			return true;
		}
		return false;
	}

	Timer::Timer() : started_(now())
	{}
	
	long long int Timer::simulationTime() const
	{
		return boost::chrono::duration_cast<boost::chrono::microseconds>(now() - started_).count();
	}

	void Timer::AddListener(const TimerListener & listener)
	{
		listeners_.push_back(listener);
	}

	TimePoint Timer::now() const
	{
		return boost::chrono::high_resolution_clock::now();
	}

	void Timer::operator()()
	{
		static const Duration TICK = boost::chrono::milliseconds(1);
		static const Duration SLEEP_TIME = boost::chrono::milliseconds(1);
		try
		{
			while(!boost::this_thread::interruption_requested())
			{
				TimePoint prev = now();
				TimePoint nextTick = prev + TICK;
				while(nextTick > now())
				{
					boost::this_thread::yield();
					boost::this_thread::sleep(boost::posix_time::milliseconds(SLEEP_TIME));
				}

				std::for_each(listeners_.begin(), listeners_.end(), boost::bind(&TimerListener::check, _1, now()));
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interrupted
			// closes gracefully
		}
		catch(...)
		{
			std::cout << "Unknown Timer exception." << std::endl;
		}
	}
}