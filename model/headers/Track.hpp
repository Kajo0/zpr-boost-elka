#ifndef TRACK_HPP
#define TRACK_HPP

#include "Segment.hpp"
#include <deque>
#include <boost\shared_ptr.hpp>

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

		public:
		Track();
		virtual ~Track();
		virtual void addPoint(Point) = 0;
		// to ma liczyc to rozsuniecie - takie pomocnicze //
		static Point bezierBetween(const Point&, const Point&);

		// TODO remove it- test only
		void showMeEverything();
	};
}

#endif // TRACK_HPP