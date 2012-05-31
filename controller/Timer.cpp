#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	Timer::Timer(const Timer::Duration & frequency) : frequency_(frequency), active_(false), started_(Now())
	{}

	void Timer::start()
	{
		active_ = true;
	}

	void Timer::stop()
	{
		active_ = false;
	}
	
	void Timer::step(long long int elapsed)
	{
		BOOST_FOREACH(boost::function<void (long long int)> & current, listeners_)
			current(elapsed);
	}

	long long int Timer::elapsed() const
	{
		return boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - started_).count();
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
		try
		{
			while(!boost::this_thread::interruption_requested())
			{
				TimePoint prev = Now();
				TimePoint nextTick = prev + frequency_;
				while(nextTick > Now())
				{
					boost::this_thread::yield();
					boost::this_thread::sleep(boost::posix_time::milliseconds(1)); // DEBUG! tylko zeby zmniejszyc zuzycie procka, trzeba ladniej wymyslic
				}

				if(active_)
				{
					long long int elapsed = boost::chrono::duration_cast<boost::chrono::microseconds>(Now() - prev).count();
					BOOST_FOREACH(boost::function<void (long long int)> & current, listeners_)
						current(elapsed);	// timer ma wolac model co jakis czas a view co inny mniejszy czas - think about it czy zostawiamy jak jest
				}
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interrupted
		}
		catch(...)
		{
			std::cout<<"Unknown Timer exception.\n";
		}
	}
}