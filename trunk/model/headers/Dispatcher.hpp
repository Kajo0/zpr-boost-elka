#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include "Camera.hpp"
#include "Voyager.hpp"
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		static const double CAMERA_DISTORTION_RATIO;

		public:
		typedef boost::shared_ptr<Camera> PCamera;
		typedef std::map<int, PCamera> MCamera;

	private:public: // TODO wywalic jakos
		MCamera cameras_;

		public:
		void addCamera(const PCamera camera);
		void reportObject(const Voyager & object);
	};
}

#endif // DISPATCHER_HPP