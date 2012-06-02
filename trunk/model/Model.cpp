#include "Model.hpp"
#include "Logger.hpp"
#include <boost/foreach.hpp>

namespace zpr
{
	Model::Model() : elapsedMicroseconds_(-1), loop_(false), active_(false), log_(false)
	{
	}

	void Model::scheduleUpdate(long long int elapsedMicroseconds)
	{
		{
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

	void Model::addCamera(const Dispatcher::PCamera & camera)
	{
		dispatcher_.addCamera(camera); // tak wiem.. , ale nie iwem jak to inaczej :P
	}

	void Model::addObject(const PVoyager & object)
	{
		objects_.push_back(object);
	}

	Model::VTCamera Model::cameras()
	{
		Model::VTCamera tmp;
		BOOST_FOREACH(const Dispatcher::MCamera::value_type & camera, dispatcher_.cameras_)
			tmp.push_back(boost::make_tuple(camera.second->position(), camera.second->range(), camera.second->direction(), camera.second->angle()));

		return tmp;
	}

	// position, angle, type
	Model::VTObject Model::objects()
	{
		Model::VTObject tmp;
		BOOST_FOREACH(PVoyager & voyager, objects_)
			tmp.push_back(boost::make_tuple(voyager->position(), voyager->angle(), voyager->type(), voyager->id(), voyager->velocity()));

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
		std::for_each(objects_.begin(), objects_.end(), boost::bind(&Voyager::reset, _1));
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
		BOOST_FOREACH(PVoyager & voyager, objects_)
		{
			if(!voyager->finished())
				voyager->move(elapsed_time);
			else if(loop_)
				voyager->reset();
			dispatcher_.reportObject(*voyager);
		}
	}
}