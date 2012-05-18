#include "Track.hpp"
#include <math.h>

namespace zpr
{
	Track::Track()
	{
	}

	Track::~Track() {}

	/**
	 * To rozsuniecie o ktorym mowilem - tylko ze dla pary tzn. majac kropki jako pkty, da wsp. punktu albo oddalonego
	 * o BEZIER_DIVISION ('promien' skretu aktualnie 2)
	 *
	 *         .
	 *
	 *     ,
	 *   .
	 */
	Point Track::bezierBetween(const Point &control, const Point &final)
	{
		Point tmp = control;
		
		if ( Point::length(control, final) <= 2 * BEZIER_DIVISION )
		{
			tmp.x_ += (final.x_ - control.x_) / 2;
			tmp.y_ += (final.y_ - control.y_) / 2;
		}
		else
		{
			tmp.x_ += (final.x_ - control.x_) * ( BEZIER_DIVISION / Point::length(control, final) );
			tmp.y_ += (final.y_ - control.y_) * ( BEZIER_DIVISION / Point::length(control, final) );
		}

		return tmp;
	}

	void Track::showMeEverything()
	{
		if (!segments_.empty())
		{
			Point b, e;
			std::cout<<"\t";
			
			for (DSegments::const_iterator it = segments_.begin(); it != segments_.end(); ++it)
			{
				b = (*it)->begin();

				std::cout<<b<<" - ";

				if ( dynamic_cast<BezierSegment*>((*it).get()) != NULL )
					std::cout<<"control"<<dynamic_cast<BezierSegment*>((*it).get())->control()<<" - ";
			}
			
			e = segments_.back()->end();
			std::cout<<e<<"\n";
		}
	}
}