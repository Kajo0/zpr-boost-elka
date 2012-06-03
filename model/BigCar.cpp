#include "BigCar.hpp"

namespace zpr
{
	BigCar::BigCar(const std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track): Vehicle(id, acceleration, weight, maxSpeed, track)
	{
	}
	
	OBJECTS BigCar::type() const
	{
		return BIGCAR;
	}
}