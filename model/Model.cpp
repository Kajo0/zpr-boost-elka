#include "Model.hpp"
#include "Logger.hpp"

namespace zpr
{
	Model::Model() : doUpdate_(false)
	{
	}

	Model::~Model() {}

	void Model::scheduleUpdate()
	{
		{ // ta klamra moze byc potrzebna dla locka, ale czy na pewno tego nie wiem.
			boost::lock_guard<boost::mutex> lock(updateMutex_);
			doUpdate_ = true;
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
					while(!doUpdate_)
						updateCondition_.wait(lock);
					
					doUpdate_ = false;
					nextStep(10);
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

	void Model::nextStep(const long elapsed_time)
	{
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->move(elapsed_time);
		}

		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->move(elapsed_time);
		}
	}
}