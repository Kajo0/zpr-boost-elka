#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "Camera.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <deque>

namespace zpr
{	
	typedef boost::tuple<Point, double, OBJECTS, std::string, double> TObject;	// position, angle, type/size, name/id, velocity
	typedef std::deque<TObject> DTObject;
		
	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		/** Size of maximum camera inaccuracy [m] */
		static const double CAMERA_DISTORTION_RATIO;

		public:
		typedef boost::shared_ptr<Camera> PCamera;

	private:public: // TODO wywalic jakos
		/** Dispatcher's cameras */
		std::deque<PCamera> cameras_;

		public:
		/**
		 * Adds camera into dispatcher
		 *
		 * @param camera camera to add
		 */
		void addCamera(const PCamera & camera);

		/**
		 * Check if there is any object in range of each camera
		 * If there is, calculate camera seeing distortion and log it out
		 *
		 * @param simulationTime time since start of the simulation in us
		 * @param objects set of objects' data, that should be checked whether in range
		 */
		void log(long long int simulationTime, const DTObject & objects);
	};
}

#endif // DISPATCHER_HPP