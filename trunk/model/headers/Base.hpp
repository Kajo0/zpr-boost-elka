#ifndef BASE_HPP
#define BASE_HPP

#include <iostream>

namespace zpr
{
	// to gdzies trzeba dac w ogole do tej abstract factory
	enum OBJECTS { WALKER, SMALLCAR, BIGCAR, CAMERA };

	/**
	  * Basic structure identyfing object's position
	  */
	struct Point
	{
		double x_, y_;

		Point(double x = 0.0, double y = 0.0);
		bool operator==(const Point & other);
		Point & operator+=(const Point & add);
		static double distance(const Point &p1, const Point & p2);
		static double angle(const Point &p1, const Point & p2);

		std::string str() const;
		// TODO - remove it test only
		friend std::ostream& operator<<(std::ostream&, const Point & point);
	};

	Point operator+(Point p1, const Point & p2);

	template<typename T>
	bool inRange(const T & downBound, const T & upBound, const T & value)
	{
		return downBound <= value && value <= upBound;
	}
}

#endif // BASE_HPP