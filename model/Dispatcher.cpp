#include "Dispatcher.hpp"
#include <boost/foreach.hpp>
#include <string> // debug

namespace zpr
{
	Dispatcher::Dispatcher() {}

	Dispatcher::~Dispatcher() {}

	void Dispatcher::addCamera(const PCamera camera)
	{
		// nie duplikujemy wartosci - jak jest juz o takim id to nie zamieni
		cameras_.insert(std::pair<int, PCamera>(camera->id(), camera));
	}

	void Dispatcher::reportObject(const Voyager & object)
	{
		BOOST_FOREACH(MCamera::value_type & it, cameras_)
		{
			if(it.second->range_ >= Point::distance(it.second->position_, object.position()))
			{
				if(inRange(-it.second->direction_ - it.second->angle_ / 2.0, -it.second->direction_ + it.second->angle_ / 2.0, Point::angle(it.second->position_, object.position())))
					std::cout << "Camera " << it.second->id() << " logged " << object.id() << std::endl;
			}
		}
	}
}