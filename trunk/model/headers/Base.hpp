#ifndef BASE_HPP
#define BASE_HPP

namespace zpr
{
	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		double x_, y_;

		Point(double x = 0.0, double y = 0.0);
	};
}

#endif // BASE_HPP