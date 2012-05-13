#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <vector>
#include "Camera.hpp"
#include <boost\shared_ptr.hpp>

namespace zpr
{
	typedef boost::shared_ptr<Camera> PCamera;
	// moze map ? bo i tak jedno id jedna kamera trzeba jakos rozwiazac
	// np wczytane 2 razy to samo id -> ignore ?
	typedef std::vector<PCamera> VCamera;

	/**
	 * Holds cameras and manages them
	 */
	class Dispatcher
	{
		VCamera cameras_;

		public:
		Dispatcher();
		virtual ~Dispatcher();

		void addCamera(const PCamera);
		void removeCamera(int);

		// TODO delet e- test only
		void tellMeSthAbout();
	};
}

#endif // DISPATCHER_HPP