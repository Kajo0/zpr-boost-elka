#ifndef WALKERTRACK_HPP
#define WALKERTRACK_HPP

#include "Track.hpp"

namespace zpr
{
	/**
	 * Sth about - acute corners
	 */
	class WalkerTrack: public Track
	{
		public:
		WalkerTrack();
		~WalkerTrack();

		void addPoint(Point point);
	};
}

#endif // WALKERTRACK_HPP