#include "Dispatcher.hpp"
#include "Logger.hpp"
#include <boost/foreach.hpp>
#include <string> // debug
#include <boost/lexical_cast.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

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
		static boost::random::uniform_01<boost::random::mt19937> distribution(boost::random::mt19937(std::time(0)));
		//std::cout << "aaaaaa" << std::endl;
		BOOST_FOREACH(MCamera::value_type & it, cameras_)
		{
			if(it.second->range_ >= Point::distance(it.second->position_, object.position()))
			{
				if(inRange(-it.second->direction_ - it.second->angle_ / 2.0, -it.second->direction_ + it.second->angle_ / 2.0, Point::angle(it.second->position_, object.position())))
				{
					std::cout << distribution() << std::endl;
					if(distribution() <= it.second->precision_)
					{
					std::string msg = "czas: "; // tu wziac time z timera jak tam liczy w ogole , a jak nie to musi liczyc
					msg += "Camera (";
					msg += boost::lexical_cast<std::string>(it.second->id());
					msg += ") saw: ";
					msg += boost::lexical_cast<std::string>(object.id());
					msg += " on (" + object.position().str() + ")";

					Logger::getInstance().message(msg);

					std::cout << "Camera " << it.second->id() << " logged " << object.id() << std::endl;
					}
					else
						std::cout << "Camera " << it.second->id() << " failed" << std::endl;
				}
			}
		}
	}
}