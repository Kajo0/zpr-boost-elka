#include "VehicleTrack.hpp"

namespace zpr
{
	const double VehicleTrack::BEZIER_DIVISION = 50;

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

			Point p1 = VehicleTrack::bezierBetween( lastEnd, realBegin );
			Point p2 = VehicleTrack::bezierBetween( lastEnd, point );

			// optymalizacja(jeszcze nie ma :P): jak begin i end na tym samym x'ie lub y'ku to tylko straight
			segments_.push_back( PSegment( new StraightSegment( lastBegin, p1 ) ) );
			segments_.push_back( PSegment( new BezierSegment( p1, p2, lastEnd ) ) );
			segments_.push_back( PSegment( new StraightSegment( p2, point ) ) );
		}

		recalculateLength();
	}

	/**
	 * To rozsuniecie o ktorym mowilem - tylko ze dla pary tzn. majac kropki jako pkty, da wsp. punktu albo oddalonego
	 * o BEZIER_DIVISION ('promien' skretu aktualnie 2)
	 *
	 *         .
	 *
	 *     ,
	 *   .
	 */
	Point VehicleTrack::bezierBetween(const Point &control, const Point &final)
	{
		Point tmp = control;
		
		if ( Point::distance(control, final) <= 2 * BEZIER_DIVISION )
		{
			tmp.x_ += (final.x_ - control.x_) / 2;
			tmp.y_ += (final.y_ - control.y_) / 2;
		}
		else
		{
			tmp.x_ += (final.x_ - control.x_) * ( BEZIER_DIVISION / Point::distance(control, final) );
			tmp.y_ += (final.y_ - control.y_) * ( BEZIER_DIVISION / Point::distance(control, final) );
		}

		return tmp;
	}
}