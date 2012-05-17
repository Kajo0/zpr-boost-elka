#ifndef TRACK_HPP
#define TRACK_HPP

#include <deque>
#include <boost\shared_ptr.hpp>

namespace zpr
{
	class Segment;

	/**
	 * Sth about track
	 */
	class Track
	{
		public:
		typedef boost::shared_ptr<Segment> PSegment;
		typedef std::deque<PSegment> DSegments;

		private:
		DSegments segments_;
	};
}

#endif // TRACK_HPP