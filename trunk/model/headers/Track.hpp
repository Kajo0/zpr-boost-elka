#ifndef TRACK_HPP
#define TRACK_HPP

#include "Segment.hpp"
#include <deque>
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Sth about track
	 */
	class Track
	{
		public:
		typedef boost::shared_ptr<Segment> PSegment;
		typedef std::deque<PSegment> DSegments;

		protected:
		DSegments segments_;
		double length_; // dlugosc calej trasy

		public:
		Track();
		virtual ~Track() = 0;
		virtual void addPoint(Point point) = 0;
		std::pair<Point, double> positionOnTrack(double percent);
		std::pair<double, double> nextBezierDistanceAngle(double percent);
		Point start();
		Point finish();
		double length();

		protected:
		void recalculateLength();
	};

	/**
	 * Sth about - bezier corners
	 */
	class VehicleTrack : public Track
	{
		public:
		/** Corner division amount */
		static const double BEZIER_DIVISION; // 1.0 = 1m a miasto jak robimy wielkosciowo ?

		void addPoint(Point point);

		std::pair<Point, Point> bothBezierBetween(const Point &start, const Point &final, const Point &control);
		// to ma liczyc to rozsuniecie - takie pomocnicze //
		Point bezierBetween(const Point &control, const Point &final);
	};

	/**
	 * Sth about - acute corners
	 */
	class WalkerTrack : public Track
	{
		public:
		void addPoint(Point point);
	};
}

#endif // TRACK_HPP