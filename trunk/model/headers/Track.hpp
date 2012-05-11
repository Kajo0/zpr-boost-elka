#ifndef TRACK_HPP
#define TRACK_HPP

namespace zpr
	{
	/**
	 * Sth about track
	 */
	class Track
		{
		private:
		std::deque<Segment*> segments_; // tutaj moze przechowywac shared_ptr
		};
	}

#endif