#ifndef SMALLCAR_HPP
#define SMALLCAR_HPP

#include "Vehicle.hpp"

namespace zpr
{
	/**
	 * Class representing small car in simulation
	 */
	class SmallCar: public Vehicle
	{
		public:
		/**
		 * Construct small car object
		 * 
		 * @param id cars registration
		 * @param acceleration cars acceleration
		 * @param weight cars weight
		 * @param maxSpeed maximum cars velocity
		 * @param track object track
		 */
		SmallCar(std::string &id, double acceleration, double weight, double maxSpeed, const PTrack & track);
		
		/**
		 * Check object type (enum)
		 *
		 * @return this object type
		 */
		OBJECTS type() const;
	};
}

#endif // SMALLCAR_HPP