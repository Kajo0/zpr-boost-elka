#include "SmallCar.hpp"

namespace zpr
{
	SmallCar::SmallCar(std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track) : Vehicle(id, acceleration, weight, maxSpeed, track)
	{
	}
	
	OBJECTS SmallCar::type() const
	{
		return SMALLCAR;
	}
}