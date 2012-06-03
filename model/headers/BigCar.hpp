#ifndef BIGCAR_HPP
#define BIGCAR_HPP

#include "Vehicle.hpp"

namespace zpr
{
	/**
	 * Class representing big car in simulation
	 */
	class BigCar: public Vehicle
	{
		public:
		/**
		 * Construct big car object
		 * 
		 * @param id cars registration
		 * @param acceleration cars acceleration
		 * @param weight cars weight
		 * @param maxSpeed maximum cars velocity
		 */
		BigCar(const std::string &id, double acceleration, double weight, double maxSpeed);
		~BigCar();
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		OBJECTS type() const;
	};
}

#endif // BIGCAR_HPP