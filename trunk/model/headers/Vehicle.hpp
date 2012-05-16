#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Track.hpp"
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
		//boost::shared_ptr<Track> track_; // do Voyager ? - kazdy ma swoja trase to po co w ogole ?!

		public:
		Vehicle(std::string&);
		virtual ~Vehicle();
	};
}

#endif // VEHICLE_HPP