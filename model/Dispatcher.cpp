#include "Dispatcher.hpp"
#include <boost\foreach.hpp>

namespace zpr
{
	Dispatcher::Dispatcher() {}

	Dispatcher::~Dispatcher() {}

	void Dispatcher::addCamera(const PCamera camera)
	{
		// nie duplikujemy wartosci - jak jest juz o takim id to nie zamieni
		cameras_.insert(std::pair<int, PCamera>(camera->id(), camera));
	}

	void Dispatcher::tellMeSthAbout()
	{
		BOOST_FOREACH(MCamera::value_type cam, cameras_) {
			cam.second->tellMeSthAbout();
		}
	}
}