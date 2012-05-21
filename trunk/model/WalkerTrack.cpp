#include "WalkerTrack.hpp"

namespace zpr
{
	WalkerTrack::WalkerTrack()
	{
	}

	WalkerTrack::~WalkerTrack() {}

	void WalkerTrack::addPoint(Point point)
	{
		if (segments_.empty())
		{
			segments_.push_back( PSegment( new StraightSegment( point, point ) ) );
		}
		else if ( segments_.size() == 1 && segments_.front()->begin() == segments_.front()->end() )
		{
			segments_.front().reset( new StraightSegment( segments_.front()->begin(), point ) );
		}
		else
		{
			segments_.push_back( PSegment( new StraightSegment( segments_.back()->end(), point ) ) );
		}

		recalculateLength();
	}
}