#include "Base.hpp"
#include <cmath>
#include <boost/format.hpp>

namespace zpr
{
	Point::Point(double x, double y) : x_(x), y_(y)
	{
	}

	bool Point::operator==(const Point &other)
	{
		return ( x_ == other.x_ && y_ == other.y_ );
	}

	Point & Point::operator+=(const Point & add)
	{
		x_ += add.x_;
		y_ += add.y_;
		return *this;
	}

	double Point::distance(const Point &p1, const Point &p2)
	{
		return sqrt( fabs( (p2.x_ - p1.x_) * (p2.x_ - p1.x_) + (p2.y_ - p1.y_) * (p2.y_ - p1.y_) ) );
	}

	double Point::angle(const Point &p1, const Point &p2)
	{
		return std::atan2(p2.y_ - p1.y_, p2.x_ - p1.x_);
	}

	Point operator+(Point p1, const Point & p2)
	{
		return p1 += p2;
	}

	std::string Point::str() const
	{
		return "x = " + (boost::format("%6.3f") % x_).str() + ", y = " + (boost::format("%6.3f") % y_).str();
	}

	std::ostream& operator<<(std::ostream &os, const Point &point)
	{
		os<< point.str();
		return os;
	}
}