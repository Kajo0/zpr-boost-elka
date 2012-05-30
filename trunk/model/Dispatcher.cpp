#include "Dispatcher.hpp"
#include "Logger.hpp"
#include <boost/foreach.hpp>
#include <string> // debug
#include <boost/lexical_cast.hpp>

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
				{
					std::string msg = "czas: "; // tu wziac time z timera jak tam liczy w ogole , a jak nie to musi liczyc
					msg += "Camera (";
					msg += boost::lexical_cast<std::string>(it.second->id());
					msg += ") saw: ";
					msg += boost::lexical_cast<std::string>(object.id());
					msg += " on (";
						// random jakis dokladnosciowy -> a jak na granicy jest to niedokladnosc ze go nie zauwazy czy ze go zauwazy nawet moze poza rangem
					msg += boost::lexical_cast<std::string>(object.position().x_);
					msg += ",";
					msg += boost::lexical_cast<std::string>(object.position().y_);
					msg += ")";

					Logger::getInstance().message(msg);

					std::cout << "Camera " << it.second->id() << " logged " << object.id() << std::endl;
				}
			}
		}
	}
}