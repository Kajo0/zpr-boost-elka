#ifndef VEHICLETRACK_HPP
#define VEHICLETRACK_HPP

#include "Track.hpp"

namespace zpr
{
	/**
	 * Sth about - bezier corners
	 */
	class VehicleTrack: public Track
	{
		public:
		VehicleTrack();
		~VehicleTrack();

		void addPoint(Point point);
	};
}

#endif // VEHICLETRACK_HPP