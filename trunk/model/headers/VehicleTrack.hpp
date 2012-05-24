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

		std::pair<Point, Point> bothBezierBetween(const Point &start, const Point &final, const Point &control);
		// to ma liczyc to rozsuniecie - takie pomocnicze //
		Point bezierBetween(const Point &control, const Point &final);
	};
}

#endif // VEHICLETRACK_HPP