#include "BigCar.hpp"

namespace zpr
{
	BigCar::BigCar(std::string &id, double acceleration, double weight, double maxSpeed): Vehicle(id, acceleration, weight, maxSpeed)
	{
	}

	BigCar::~BigCar() {}
}