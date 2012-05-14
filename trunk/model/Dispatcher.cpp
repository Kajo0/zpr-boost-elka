#include "Dispatcher.hpp"
#include <boost\foreach.hpp>

namespace zpr
{
	Dispatcher::Dispatcher() {}

	Dispatcher::~Dispatcher() {}

	void Dispatcher::addCamera(const PCamera camera)
	{
		// TODO spr cos wiecej o istnieniu
		cameras_[camera->getId()] = camera; // jezeli juz istnieje, nadpisze; przemyslec
		//cameras_.push_back(camera);
	}

	void Dispatcher::removeCamera(int id)
	{
		cameras_.erase(id);
		/*for (VCamera::const_iterator it = cameras_.begin(); it != cameras_.end(); ++it)
		{
			if ((**it).getId() == id)
			{
				cameras_.erase(it);
				break;
			}
		}*/
	}

	void Dispatcher::tellMeSthAbout()
	{
		BOOST_FOREACH(MCamera::value_type cam, cameras_) {
			cam.second->tellMeSthAbout();
		}
	}
}