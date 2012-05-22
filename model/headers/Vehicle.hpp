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
		Vehicle(std::string &id, double acceleration, double weight, double maxSpeed);
		virtual ~Vehicle();

		const std::string& id();
		void move(const long elapsed_time);
	};
}

#endif // VEHICLE_HPP