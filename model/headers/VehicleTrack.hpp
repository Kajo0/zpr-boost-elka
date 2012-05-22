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
		/** Corner division amount */
		static const double BEZIER_DIVISION; // 1.0 = 1m a miasto jak robimy wielkosciowo ?

		VehicleTrack();
		~VehicleTrack();

		void addPoint(Point point);

		// to ma liczyc to rozsuniecie - takie pomocnicze //
		static Point bezierBetween(const Point &control, const Point &final);
	};
}

#endif // VEHICLETRACK_HPP