#include "Dispatcher.hpp"
#include "Logger.hpp"
#include <boost/foreach.hpp>
#include <string> // debug
#include <boost/lexical_cast.hpp>

#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
namespace zpr
{
	const double Dispatcher::CAMERA_DISTORTION_RATIO = 100.0;

	void Dispatcher::addCamera(const PCamera camera)
	{
		// nie duplikujemy wartosci - jak jest juz o takim id to nie zamieni
		cameras_.insert(std::pair<int, PCamera>(camera->id(), camera));
	}

	void Dispatcher::reportObject(const Voyager & object)
	{
		static boost::random::uniform_01<boost::random::mt19937> distribution(boost::random::mt19937(std::time(0)));

		BOOST_FOREACH(MCamera::value_type & it, cameras_)
			if(it.second->spotted(object))
			{
				double noise = it.second->noise() * CAMERA_DISTORTION_RATIO;
				std::string msg = "czas: "; // tu wziac time z timera jak tam liczy w ogole , a jak nie to musi liczyc
				//msg += 
				msg += "Camera (";
				msg += boost::lexical_cast<std::string>(it.second->id());
				msg += ") saw: ";
				msg += boost::lexical_cast<std::string>(object.id());
				msg += " on (" + (object.position() + Point(distribution() * noise, distribution() * noise)).str() + ")";

				Logger::getInstance().message(msg);

				//std::cout << "Camera " << it.second->id() << " logged " << object.id() << std::endl;
			}
	}
}