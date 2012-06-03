#include "Dispatcher.hpp"
#include "Model.hpp"
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
		cameras_.insert(std::pair<int, PCamera>(camera->id(), camera));
	}

	void Dispatcher::log(Model & model)
	{
		static boost::random::uniform_01<boost::random::mt19937> distribution(boost::random::mt19937(std::time(0)));

		BOOST_FOREACH(MCamera::value_type & it, cameras_)
		{
			BOOST_FOREACH(Model::PVoyager & object, model.objects_)
				if(it.second->spotted(*object))
				{
					

					double noise = it.second->noise() * CAMERA_DISTORTION_RATIO;
					std::string msg = "czas: "; // TODO tu wziac time z timera jak tam liczy w ogole , a jak nie to musi liczyc
					msg += "Camera (";
					msg += boost::lexical_cast<std::string>(it.second->id());
					msg += ") saw: ";
					msg += boost::lexical_cast<std::string>(object->id());
					msg += " on\t(" + (object->position() + Point(distribution() * noise, distribution() * noise)).str() + ")";
					msg += "\t{ real position: [";
					msg += object->position().str();
					msg += "] }";

					Logger::getInstance().message(msg);

					std::cout << "Camera " << it.second->id() << " logged " << object->id() << std::endl;
				}
		}
	}
	
}