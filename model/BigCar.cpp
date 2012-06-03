#include "BigCar.hpp"

namespace zpr
{
	BigCar::BigCar(const std::string &id, double acceleration, double weight, double maxSpeed): Vehicle(id, acceleration, weight, maxSpeed)
	{
	}

	BigCar::~BigCar() {}
	
	OBJECTS BigCar::type() const
	{
		return BIGCAR;
	}
}