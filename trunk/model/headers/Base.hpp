#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>

namespace zpr
{
	// to gdzies trzeba dac w ogole do tej abstract factory
	enum objects { WALKER, SMALLCAR, BIGCAR, CAMERA };

	/** Corner division amount */
	static const double BEZIER_DIVISION = 2;

	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		double x_, y_;

		Point(double x = 0.0, double y = 0.0);
		bool operator==(const Point &other);
		static double distance(const Point &p1, const Point &p2);

		// TODO - remove it test only
		friend std::ostream& operator<<(std::ostream&, Point &point);
	};
}

#endif // BASE_HPP