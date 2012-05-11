#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Track.hpp"

namespace zpr
	{
	/**
	 * Sth about cars
	 */

	class Vehicle : public Voyager {
	protected:
		/*
		float mass_;
		float velocity_;
		float acceleration_;
		*/
		const std::string registration_; // to pewnie tez
		boost::shared_ptr<Track> track_; // do Voyager ?

	public:
		Vehicle(std::string&);
		virtual ~Vehicle();
	};
	}

#endif