#include "Walker.hpp"
#include "WalkerTrack.hpp"
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
		percentDistanceTraveled = 0;
		position_ = track_->start();
	}

	void Walker::move(long long int elapsed_time)
	{
		percentDistanceTraveled += ( velocity_ * elapsed_time * 0.1 / 3.6 ) / track_->length();	// oczywiscie wypada udoskonalic bo czas to milisenkundy
		position_ = track_->positionOnTrack( percentDistanceTraveled );

		if(percentDistanceTraveled > 1.0)
			reset(); // petla
	}
}