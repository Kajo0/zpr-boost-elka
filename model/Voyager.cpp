#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager(double velocity, Point position): velocity_(velocity), position_(position), percentDistanceTraveled(0), finished_(false)
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

	void Voyager::addTrackPoint(const Point point)
	{
		track_->addPoint(point);
	}
	
	void Voyager::reset()
	{
		finished_ = false;
		velocity_ = 0;
		percentDistanceTraveled = 0;
		position_ = track_->start();
	}

	bool Voyager::finished()
	{
		return finished_;
	}
}