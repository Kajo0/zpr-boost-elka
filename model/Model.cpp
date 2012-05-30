#include "Model.hpp"
#include "Logger.hpp"

namespace zpr
{
	Model::Model() : elapsedMicroseconds_(-1)
	{
	}

	Model::~Model() {}

	void Model::scheduleUpdate(long long int elapsedMicroseconds)
	{
		{ // ta klamra moze byc potrzebna dla locka, ale czy na pewno tego nie wiem.
			boost::lock_guard<boost::mutex> lock(updateMutex_);
			elapsedMicroseconds_ = elapsedMicroseconds;
		}
		updateCondition_.notify_one();
	}

	void Model::operator()()
	{
		try
		{
			while(1)
			{
				//// Waiting for update signal
				{
					boost::unique_lock<boost::mutex> lock(updateMutex_);
					while(elapsedMicroseconds_ < 0)
						updateCondition_.wait(lock);
					
					nextStep(elapsedMicroseconds_/1000);
					elapsedMicroseconds_ = -1;
				}
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interruption
		}
		catch(...)
		{
			Logger::getInstance().message("Unknown Model exception.");
		}
		std::cout << "Model thread ending." << std::endl;
	}

	void Model::start()
	{
		// tak zeby dzialalo, bo nei rozkminialme dokladnie binda czy tam lambdy
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->reset();
		}
		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->reset();
		}
	}

	void Model::nextStep(long long int elapsed_time)
	{
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->move(elapsed_time);
			dispatcher_.reportObject(*it->second.get());
		}

		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->move(elapsed_time);
			dispatcher_.reportObject(*it->second.get());
		}
	}
}