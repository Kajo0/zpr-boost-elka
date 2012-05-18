#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>

namespace zpr
{
	/** Corner division amount */
	static const double BEZIER_DIVISION = 2;

	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		double x_, y_;

		Point(double x = 0.0, double y = 0.0);
		bool operator==(const Point&);
		static double length(const Point&, const Point&);

		// TODO - remove it test only
		friend std::ostream& operator<<(std::ostream&, Point&);
	};
}

#endif // BASE_HPP