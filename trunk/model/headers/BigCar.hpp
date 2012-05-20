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
		BigCar(std::string&, double, double, double);
		~BigCar();
	};
}

#endif // BIGCAR_HPP