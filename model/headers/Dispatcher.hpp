#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <map>
#include "Camera.hpp"
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		public:
		typedef boost::shared_ptr<Camera> PCamera;
		typedef std::map<int, PCamera> MCamera;

	private:public:
		MCamera cameras_;

		public:
		Dispatcher();
		virtual ~Dispatcher();

		void addCamera(const PCamera camera);
	};
}

#endif // DISPATCHER_HPP