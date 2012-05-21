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
		Point getPosition(double percent);
	};
}

#endif // VEHICLETRACK_HPP