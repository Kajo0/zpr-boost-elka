#include "Vehicle.hpp"
#include "Track.hpp"
#include <iostream>
#include <string>

namespace zpr
{
	Vehicle::Vehicle(std::string &id, double acceleration, double weight, double maxSpeed): // przyspieszenie zbilem do ms, predkosc do m / s (chyba)
						registration_(id), acceleration_(acceleration * 0.000001), weight_(weight), maxSpeed_(maxSpeed * 0.277777777), break_(false)
	{
		track_ = PTrack( new VehicleTrack() );
	}

	Vehicle::~Vehicle() {}

	const std::string& Vehicle::id() const
	{
		return registration_;
	}

	void Vehicle::move(long long int elapsed_time) // co 10 ms
	{
		if (!track_->positionOnTrack( percentDistanceTraveled_ ).second && !break_)
		{
			velocity_ += acceleration_ * elapsed_time;
		}
		else if (!track_->positionOnTrack( percentDistanceTraveled_ ).second && break_)
		{
			velocity_ -= acceleration_ * elapsed_time;
		}

		if (track_->positionOnTrack( percentDistanceTraveled_ ).second)
		{
			//std::cout<<"\t na bezierze\n";
			break_ = false;
		}
		
		if (!break_)
		{
			// Vk -> max predkosc na bezierze
			// Vk = Vp - a*t
			// t = (Vp - Vk) / a
			
			// do wyhamowania do zera
			long long int t = (velocity_ - 0) / acceleration_;

			//std::cout<<"\t\tczas ham:"<<t<<"\n";
			double percent = percentDistanceTraveled_;
			double dx = 0;
			dx += acceleration_ * t * t / 2; // ile bedzie hamowal (droga)
			percent += ( dx ) / track_->length();


			// tu sprawdzam czy to bezier jest (socond-> kat beziera) musze pomyslec nad tym jeszcze bo jakos nie moge domyslic sie jak to zwalniac
			if (track_->positionOnTrack( percent ).second) {
				break_ = true;
				//std::cout<<"zaczne hamowac!!!!\n";
			}
		}

		//velocity_ += acceleration_ * elapsed_time * 0.00001;	// oczywiscie tu wchodzi fizyka w ktora sie teraz nie zaglebiam + zakrety
		if (velocity_ > maxSpeed_)
			velocity_ = maxSpeed_;
		//velocity_ = 0.11;

		Point prev_position = position_;
		percentDistanceTraveled_ += ( velocity_ * elapsed_time ) / track_->length();
		position_ = track_->positionOnTrack( percentDistanceTraveled_ ).first;
		angle_ = Point::angle(prev_position, position_);

		if(percentDistanceTraveled_ > 1.0)
			finished_ = true;
		//	reset(); // petla -> mialabyc 'mozliwosc' zapetlenia, domyslnie powinno tylko raz przeleciec
	}
}