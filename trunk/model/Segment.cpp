#include "Segment.hpp"

namespace zpr
{
	/************************
	*	Segment				*
	************************/
	Segment::Segment(const Point begin, const Point end): begin_(begin), end_(end)
	{
	}

	Segment::~Segment() {}
	
	Point Segment::begin()
	{
		return begin_;
	}
	
	Point Segment::end()
	{
		return end_;
	}


	/************************
	*	BezierSegment		*
	*************************/
	BezierSegment::BezierSegment(const Point begin, const Point end, const Point control): control_(control), Segment(begin, end)
	{
	}

	BezierSegment::~BezierSegment() {}

	Point BezierSegment::position(double percent)
	{
		if ( percent < 0 )
		{
			percent = 0;
		}
		else if ( percent > 1 )
		{
			percent = 1;
		}

		Point tmp;
		tmp.x_ = (1 - percent) * (1 - percent) * begin_.x_ + 2 * (1 - percent) * percent * control_.x_ + percent * percent * end_.x_;
		tmp.y_ = (1 - percent) * (1 - percent) * begin_.y_ + 2 * (1 - percent) * percent * control_.y_ + percent * percent * end_.y_;

		return tmp;
	}

	Point BezierSegment::control()
	{
		return control_;
	}


	/************************
	*	StraightSegment		*
	*************************/
	StraightSegment::StraightSegment(const Point begin, const Point end): Segment(begin, end)
	{
	}

	StraightSegment::~StraightSegment() {}

	Point StraightSegment::position(double percent)
	{
		if ( percent < 0 )
		{
			percent = 0;
		}
		else if ( percent > 1 )
		{
			percent = 1;
		}

		Point tmp(begin_.x_, begin_.y_);
		
		tmp.x_ += (end_.x_ - begin_.x_) * percent;
		tmp.y_ += (end_.y_ - begin_.y_) * percent;

		return tmp;
	}
}