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

	void Vehicle::tellMeMore()
	{
		std::cout<<"id: "<<registration_
				<<" pos: "<<position_
				<<" v: "<<velocity_
				<<" a: "<<acceleration_
				<<" weight: "<<weight_
				<<" mSpeed: "<<maxSpeed_<<"\n";
	}
}