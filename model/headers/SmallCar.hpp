#ifndef SMALLCAR_HPP
#define SMALLCAR_HPP

#include "Vehicle.hpp"

namespace zpr
{
	/**
	 * Sth about
	 */
	class SmallCar: public Vehicle
	{
		public:
		SmallCar(std::string &id, double acceleration, double weight, double maxSpeed);
		~SmallCar();
		
		OBJECTS type();
	};
}

#endif // SMALLCAR_HPP