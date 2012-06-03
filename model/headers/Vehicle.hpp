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
		/** Vehicle acceleration [m/s] */
		double acceleration_;
		/** Vehicle weight [kg] */
		double weight_;
		/** Vehicle maximum velocity [m/s] */
		double maxSpeed_;
		/** Vehicle registration */
		const std::string registration_;
		/** Breaking flag */
		bool break_;

		public:
		/**
		 * Vehicle constructor
		 *
		 * @param id vehicle registration
		 * @param acceleration vehicle acceleration [m/s]
		 * @param weight vehicle weight [kg]
		 * @param maxSpeed vehicle maximum velocity [km/h]
		 */
		Vehicle(const std::string &id, double acceleration, double weight, double maxSpeed);
		virtual ~Vehicle();
		
		/**
		 *
		 * @return vehicle registration
		 */
		const std::string& id() const;

		/**
		 * Reset vehicle state, back it on the beginning of track
		 */
		void reset();
		
		/**
		 * Next simulator iteration
		 * Vehicle moves depend on elapsed time
		 *
		 * @param elapsed_time time which elapsed since last update (or to simulate movement)
		 */
		void move(long long int elapsed_time);
	};
}

#endif // VEHICLE_HPP