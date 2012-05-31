#include "Walker.hpp"
#include "Track.hpp"
#include <iostream>
#include <string>

namespace zpr
{
	Walker::Walker(std::string &name, double velocity): name_(name), Voyager(velocity)
	{
		track_ = PTrack( new WalkerTrack() );
		velocity_ = velocity;
	}

	Walker::~Walker() {}

	Walker::Walker(const Walker &other)
	{
	}

	Walker& Walker::operator=(const Walker &other)
	{
		return *this;
	}

	const std::string& Walker::id() const
	{
		return name_;
	}

	void Walker::reset()
	{
		finished_ = false;
		percentDistanceTraveled_ = 0;
		position_ = track_->start();
	}

	void Walker::move(long long int elapsed_time)
	{
		Point prev_position = position_;
		percentDistanceTraveled_ += ( velocity_ * elapsed_time * 0.05 / 3.6 ) / track_->length();	// oczywiscie wypada udoskonalic bo czas to milisenkundy
		position_ = track_->positionOnTrack( percentDistanceTraveled_ ).first;
		angle_ = Point::angle(prev_position, position_);

		if(percentDistanceTraveled_ > 1.0)
			finished_ = true;
		//	reset(); // petla -> mozliwosc zapetlenia a nie zapetlenie.
	}
}