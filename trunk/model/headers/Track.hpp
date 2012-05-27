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
		virtual ~Track();
		virtual void addPoint(Point point) = 0;
		Point positionOnTrack(double percent);
		Point start();
		Point finish();
		double length();

		protected:
		void recalculateLength();
	};
}

#endif // TRACK_HPP