#ifndef VEHICLE_HPP
#define VEHICLE_HPP

#include "Track.hpp"

/**
 * Sth about cars
 */
class Vehicle abstract {
protected:
	/*
	float mass_;
	float velocity_;
	float acceleration_;
	*/
	const std::string registration_;
	boost::shared_ptr<Track> track_;

public:
	Vehicle(std::string&);
	virtual ~Vehicle();
};

#endif