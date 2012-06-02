#include "Model.hpp"
#include "Logger.hpp"

namespace zpr
{
	Model::Model() : elapsedMicroseconds_(-1), loop_(false), active_(false), log_(false)
	{
	}

	void Model::scheduleUpdate(long long int elapsedMicroseconds)
	{
		{ // ta klamra moze byc potrzebna dla locka, ale czy na pewno tego nie wiem.
			boost::lock_guard<boost::mutex> lock(updateMutex_);
			elapsedMicroseconds_ = elapsedMicroseconds;
		}
		updateCondition_.notify_one();
	}

	void Model::scheduleLog(long long int elapsedMicroseconds)
	{
		{
			boost::lock_guard<boost::mutex> lock(updateMutex_);
			log_ = true;
		}
		updateCondition_.notify_one();
	}

	Model::PGraph Model::streets()
	{
		return streets_;
	}

	void Model::streets(const PGraph graph)
	{
		streets_ = graph;
	}

	void Model::addCamera(const Dispatcher::PCamera camera)
	{
		dispatcher_.addCamera(camera); // tak wiem.. , ale nie iwem jak to inaczej :P
	}

	void Model::addObject(const PCar car)
	{
		cars_.insert(std::make_pair(car->id(), car));
	}

	void Model::addObject(const PWalker walker)
	{
		walkers_.insert(std::make_pair(walker->id(), walker));
	}

	// position, range, direction, angle
	Model::VTCamera Model::cameras()
	{
		Model::VTCamera tmp;

		for (Dispatcher::MCamera::const_iterator it = dispatcher_.cameras_.begin(); it != dispatcher_.cameras_.end(); ++it)
			tmp.push_back(boost::make_tuple(it->second->position(), it->second->range(), it->second->direction(), it->second->angle()));

		return tmp;
	}

	// position, angle, type
	Model::VTObject Model::objects()
	{
		Model::VTObject tmp;

		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
			tmp.push_back(boost::make_tuple(it->second->position(), it->second->angle(), it->second->type()));

		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
			tmp.push_back(boost::make_tuple(it->second->position(), it->second->angle(), it->second->type()));

		return tmp;
	}

	void Model::operator()()
	{
		try
		{
			while(!boost::this_thread::interruption_requested())
			{
				//// Waiting for update signal
				{
					boost::unique_lock<boost::mutex> lock(updateMutex_);
					while(elapsedMicroseconds_ < 0 && log_ == false)
						updateCondition_.wait(lock);
					
					if(active_)
					{
						if(elapsedMicroseconds_ >= 0)
							nextStep(elapsedMicroseconds_/1000);
						else if(log_)
							dispatcher_.log(*this);
					}
					elapsedMicroseconds_ = -1;
					log_ = false;
				}
			}
		}
		catch(boost::thread_interrupted)
		{
			// thread interruption
		}
		catch(...)
		{
			std::cout<<"Unknown Model exception.\n";
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

	void Model::switchLoop()
	{
		loop_ = !loop_;
	}

	void Model::setActive(bool active)
	{
		active_ = active;
	}

	void Model::nextStep(long long int elapsed_time)
	{
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			if (loop_ && it->second->finished())
				it->second->reset();

			if (!it->second->finished())
				it->second->move(elapsed_time);
			
			dispatcher_.reportObject(*it->second.get());
		}

		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			if (loop_ && it->second->finished())
				it->second->reset();

			if (!it->second->finished())
				it->second->move(elapsed_time);

			dispatcher_.reportObject(*it->second.get());
		}
	}
}