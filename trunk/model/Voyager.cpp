#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager(double velocity, Point position): velocity_(velocity * (10 / 36)), position_(position), angle_(0), percentDistanceTraveled_(0), finished_(false)
	{
	}

	/**
	 * Empty abstract class' destructor
	 */
	Voyager::~Voyager() {}

	const Point& Voyager::position() const
	{
		return position_;
	}

	double Voyager::angle() const
	{
		return angle_;
	}

	void Voyager::track(const PTrack track)
	{
		track_ = track;
	}

	bool Voyager::finished()
	{
		return finished_;
	}
}