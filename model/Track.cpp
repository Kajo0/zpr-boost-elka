#include "Track.hpp"
#include <math.h>

namespace zpr
{
	Track::Track(): length_(0)
	{
	}

	Track::~Track() {}
	
	/**
	 * @return Begin track point
	 */
	Point Track::start()
	{
		return segments_.empty() ? Point() : segments_.front()->begin();
	}

	/**
	 * @return End track point
	 */
	Point Track::finish()
	{
		return segments_.empty() ? Point() : segments_.back()->end();
	}

	/**
	 * @return Actual track length
	 */
	double Track::length()
	{
		return length_;
	}

	/**
	 * Calculates position on track from given track percent
	 *
	 * @param percent Percent of track
	 * @return Request position
	 */
	Point Track::positionOnTrack(double percent)
	{
		if ( percent < 0 )
			percent = 0;
		else if ( percent > 1 )
			percent = 1;

		double traveled_length = length_ * percent;
		double temp_length = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			temp_length += (*it)->length();

			if ( traveled_length <= temp_length )
			{
				// zwraca pozycje z wyliczonego procenta procenta segmetnu na ktorym sie znajduje na tym zadanym procencie
				return (*it)->position( 1 - ( ( temp_length - traveled_length ) / (*it)->length() ) );
			}
		}

		return Point();
	}

	/**
	 * Calculates track length - sum of each segment length
	 */
	void Track::recalculateLength()
	{
		length_ = 0;

		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			length_ += (*it)->length();
		}
	}
}