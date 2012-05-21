#include "VehicleTrack.hpp"

namespace zpr
{
	VehicleTrack::VehicleTrack()
	{
	}

	VehicleTrack::~VehicleTrack() {}

	void VehicleTrack::addPoint(Point point)
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
			// Mowiac krotko jak jest jeden segment to jest StraightSegment
			// jak dodajemy trzeci punkt to juz mozna beziera zrobic, wiec jest : straight -> bezier -> straight
			// jak kolejny dajemy to jest : straight -> bezier -> straight(mozliwe ze o zerowej dlugosci jak nie wyrabia sie zakretowo) -> bezier -> straight

			Point realBegin = segments_.back()->begin();
			Point lastBegin = realBegin;
			Point lastEnd = segments_.back()->end();
			
			segments_.pop_back();
			
			if (!segments_.empty())
			{
				realBegin = dynamic_cast<BezierSegment*>(segments_.back().get())->control();
			}

			Point p1 = Track::bezierBetween( lastEnd, realBegin );
			Point p2 = Track::bezierBetween( lastEnd, point );

			segments_.push_back( PSegment( new StraightSegment( lastBegin, p1 ) ) );
			segments_.push_back( PSegment( new BezierSegment( p1, p2, lastEnd ) ) );
			segments_.push_back( PSegment( new StraightSegment( p2, point ) ) );
		}

		recalculateLength();
	}

	// to tak na prawde tylko testowo napisalem do rysowania
	// aczkolwiek tu jest wstep ze poruszamy samochod po prostej a pozycje wyliczamy z segmentow
	Point VehicleTrack::getPosition(double percent)
	{
		double d = length_ * percent;
		double l = 0;
		for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
		{
			l += (*it)->length();
			if ( d <= l )
			{
				double z = 1 - ( ( l - d ) / (*it)->length() );
				return (*it)->position(z);
			}
		}
	}

}