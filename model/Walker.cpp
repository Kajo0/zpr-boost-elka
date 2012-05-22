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

	const std::string& Walker::id()
	{
		return name_;
	}

	void Walker::reset()
	{
		percentDistanceTraveled = 0;
		position_ = track_->start();
	}

	void Walker::move(const long elapsed_time)
	{
		percentDistanceTraveled += ( velocity_ * elapsed_time ) / track_->length();	// oczywiscie wypada udoskonalic bo czas to milisenkundy
		position_ = track_->positionOnTrack( percentDistanceTraveled );
	}
}