#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager(double velocity, Point position): velocity_(velocity), position_(position) // to zerujemy i tak sie wyliczy point jako begin trasy a velocity zerowa w spoczynku
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
		velocity_ = 0;
		position_ = track_->start();
	}
}