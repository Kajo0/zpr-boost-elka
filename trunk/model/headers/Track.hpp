#ifndef TRACK_HPP
#define TRACK_HPP

#include <deque>

namespace zpr
{
	class Segment;

	/**
	 * Sth about track
	 */
	class Track
	{
		private:
		std::deque<Segment*> segments_; // tutaj moze przechowywac shared_ptr
	};
}

#endif // TRACK_HPP