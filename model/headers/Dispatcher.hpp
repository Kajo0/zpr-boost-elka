#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include "Camera.hpp"
#include "Voyager.hpp"
#include <boost/shared_ptr.hpp>

namespace zpr
{
	class Model; // tmp

	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		/** Size of maximum camera inaccuracy [m] */
		static const double CAMERA_DISTORTION_RATIO;

		public:
		typedef boost::shared_ptr<Camera> PCamera;
		typedef std::map<int, PCamera> MCamera;

	private:public: // TODO wywalic jakos
		/** Dispatcher's cameras */
		MCamera cameras_;

		public:
		/**
		 * Adds camera into dispatcher
		 *
		 * @param camera camera to add
		 */
		void addCamera(const PCamera camera);

		/**
		 * Check if there is any object in range of each camera
		 * If there is, calculate camera seeing distortion and log it out
		 *
		 * @param model simulation model
		 */
		void log(Model & model);
	};
}

#endif // DISPATCHER_HPP