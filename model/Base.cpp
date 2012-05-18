#include "Base.hpp"
#include <math.h>

namespace zpr
{
	Point::Point(double x, double y) : x_(x), y_(y)
	{
	}

	bool Point::operator==(const Point &other)
	{
		return ( x_ == other.x_ && y_ == other.y_ );
	}

	double Point::length(const Point &p1, const Point &p2)
	{
		return sqrt( fabs( (p2.x_ - p1.x_) * (p2.x_ - p1.x_) + (p2.y_ - p1.y_) * (p2.y_ - p1.y_) ) );
	}
	
	std::ostream& operator<<(std::ostream &os, Point &point)
	{
		os<<"("<<point.x_<<", "<<point.y_<<")";
		return os;
	}
}