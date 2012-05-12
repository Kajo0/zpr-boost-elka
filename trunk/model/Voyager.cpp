#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager() {}

	Voyager::Voyager(double weight, double maxSpeed) : weight_(weight), maxSpeed_(maxSpeed), acceleration_(0.0), velocity_(0.0)
	{

	}

	/**
	 * Empty abstract class' destructor
	 */
	Voyager::~Voyager() {}

	const Point& Voyager::Position() const
	{
		return position_;
	}
}