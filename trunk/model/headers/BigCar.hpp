#ifndef BIGCAR_HPP
#define BIGCAR_HPP

#include "Vehicle.hpp"

namespace zpr
{
	/**
	 * Sth about
	 */
	class BigCar: public Vehicle
	{
		public:
		BigCar(std::string &id, double acceleration, double weight, double maxSpeed);
		~BigCar();
	};
}

#endif // BIGCAR_HPP