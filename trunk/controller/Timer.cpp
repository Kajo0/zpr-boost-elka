#include "Timer.hpp"
#include "Logger.hpp"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

namespace zpr
{
	TimerListener::TimerListener(boost::function<void (long long int)> listener, const Duration & frequency) : listener_(listener), frequency_(frequency)
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

	Timer::Timer() : /*active_(false), */started_(now())
	{}

	/*void Timer::start()
	{
		active_ = true;
	}

	void Timer::stop()
	{
		active_ = false;
	}*/

	long long int Timer::elapsed() const
	{
		return boost::chrono::duration_cast<boost::chrono::microseconds>(now() - started_).count();
	}

	void Timer::AddListener(TimerListener listener)
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
		try
		{
			while(!boost::this_thread::interruption_requested())
			{
				TimePoint prev = now();
				TimePoint nextTick = prev + TICK;
				while(nextTick > now())
				{
					boost::this_thread::yield();
					boost::this_thread::sleep(boost::posix_time::milliseconds(1)); // DEBUG! tylko zeby zmniejszyc zuzycie procka, trzeba ladniej wymyslic
				}

				//if(active_)
				{
					BOOST_FOREACH(TimerListener & current, listeners_)
						current.check(now());	
					//long long int elapsed = boost::chrono::duration_cast<boost::chrono::microseconds>(now() - prev).count();
					//BOOST_FOREACH(boost::function<void (long long int)> & current, listeners_)
					//	current(elapsed);	// timer ma wolac model co jakis czas a view co inny mniejszy czas - think about it czy zostawiamy jak jest
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
	std::cout << "Timer thread ending." << std::endl;
	}
}