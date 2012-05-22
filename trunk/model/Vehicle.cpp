#include "Vehicle.hpp"
#include "VehicleTrack.hpp"
#include <iostream>
#include <string>

namespace zpr
{
	Vehicle::Vehicle(std::string &id, double acceleration, double weight, double maxSpeed):
						registration_(id), acceleration_(acceleration), weight_(weight), maxSpeed_(maxSpeed)
	{
		track_ = PTrack( new VehicleTrack() );
	}

	Vehicle::~Vehicle() {}

	const std::string& Vehicle::id()
	{
		return registration_;
	}

	void Vehicle::move(const long elapsed_time)
	{
		velocity_ += acceleration_ * elapsed_time * 0.001;	// oczywiscie tu wchodzi fizyka w ktora sie teraz nie zaglebiam + zakrety
		if (velocity_ > maxSpeed_)
			velocity_ = maxSpeed_;

		percentDistanceTraveled += ( velocity_ * elapsed_time ) / track_->length();
		position_ = track_->positionOnTrack( percentDistanceTraveled );
	}
}