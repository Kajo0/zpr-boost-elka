#include "Vehicle.hpp"

namespace zpr
{
	Vehicle::Vehicle(std::string &id, double acceleration, double weight, double maxSpeed):
						registration_(id), acceleration_(acceleration), weight_(weight), maxSpeed_(maxSpeed)
	{
	}

	Vehicle::~Vehicle() {}

	const std::string& Vehicle::id()
	{
		return registration_;
	}
}