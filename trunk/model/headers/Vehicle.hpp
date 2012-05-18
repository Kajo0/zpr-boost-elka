#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Voyager.hpp"

namespace zpr
{
	/**
	 * Sth about cars
	 */
	class Vehicle : public Voyager
	{
		protected:
		double acceleration_;
		double weight_;
		double maxSpeed_;
		const std::string registration_;

		public:
		Vehicle(std::string&);
		virtual ~Vehicle();
	};
}

#endif // VEHICLE_HPP