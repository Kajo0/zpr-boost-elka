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
	
	void Vehicle::reset()
	{
		break_ = false;
		finished_ = false;
		velocity_ = 0;
		percentDistanceTraveled_ = 0;
		position_ = track_->start();
	}

	void Vehicle::move(long long int elapsed_time) // co 10 ms
	{
		double on_bezier = track_->positionOnTrack( percentDistanceTraveled_ ).second;

		// nie jest na bezierze i nie hamuje to zwieksza
		if (!on_bezier && !break_)
		{
			velocity_ += acceleration_ * elapsed_time;
		} // nie jest na bezierze i hamuje to zmniejsza predkosc
		else if (!on_bezier && break_)
		{
			velocity_ -= acceleration_ * elapsed_time;
		} // na bezierze nie hamuj juz, tylko jedzie stala predkoscia
		else if (on_bezier)
		{
			break_ = false;
		}
		
		// spr hamowanie ale tylko jak nie hamuje juz i nie jest na bezierze
		if (!break_ && !on_bezier)
		{
			// Vk -> max predkosc na bezierze
			// Vk = Vp - a*t
			// t = (Vp - Vk) / a
			
			// tu mam procent drogi oznaczajacy poczatek beziera i kat zakretu (ostrzejszy zakret -> mniejszy kat)
			std::pair<double, double> nextangle = track_->nextBezierDistanceAngle(percentDistanceTraveled_);

			double max_velocity_on_bezier = nextangle.second * 0.01;
			// licze tu czas jak dlugo hamuje z liniowy hamowaniem- > hamuje tak szybko jak przyspiesza
			long long int breaking_time = (velocity_ - max_velocity_on_bezier) / acceleration_;

			if (velocity_ > max_velocity_on_bezier)
			{
				double percent = percentDistanceTraveled_;

				// procent trasy w ktorym wyhamuje do tej wyzej zadanej predkosci pt. max_velocity_on...
				percent += (acceleration_ * breaking_time * breaking_time / 2) / track_->length();

				// jak ten procent jest wiekszy od poczatku beziera to zaczyna hamowac
				if (percent >= nextangle.first)
				{
					break_ = true;
				}
			}
		}

		if (velocity_ > maxSpeed_)
			velocity_ = maxSpeed_;

		Point prev_position = position_;

		percentDistanceTraveled_ += ( velocity_ * elapsed_time ) / track_->length();
		position_ = track_->positionOnTrack( percentDistanceTraveled_ ).first;
		
		angle_ = Point::angle(prev_position, position_);

		if(percentDistanceTraveled_ > 1.0)
			finished_ = true;
	}

}