#include "Base.hpp"

namespace zpr
{
	Point::Point(double x, double y) : x_(x), y_(y)
	{
	}

	
	std::ostream& operator<<(std::ostream &os, Point &point)
	{
		os<<"("<<point.x_<<", "<<point.y_<<")";
		return os;
	}
}