#include "SmallCar.hpp"

namespace zpr
{
	SmallCar::SmallCar(std::string &id, double acceleration, double weight, double maxSpeed): Vehicle(id, acceleration, weight, maxSpeed)
	{
	}

	SmallCar::~SmallCar() {}
}