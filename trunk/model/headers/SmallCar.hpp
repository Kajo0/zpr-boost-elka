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
		SmallCar(std::string&, double, double, double);
		~SmallCar();
	};
}

#endif // SMALLCAR_HPP