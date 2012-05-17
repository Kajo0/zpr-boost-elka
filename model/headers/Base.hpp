#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>

namespace zpr
{
	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		double x_, y_;

		Point(double x = 0.0, double y = 0.0);

		// TODO - remove it test only
		friend std::ostream& operator<<(std::ostream&, Point&);
	};
}

#endif // BASE_HPP