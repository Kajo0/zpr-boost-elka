#ifndef VOYAGER_HPP
#define VOYAGER_HPP

#include "Base.hpp"
#include "Track.hpp"
#include <boost/shared_ptr.hpp>

namespace zpr
{
	/**
	 * Abstract base class for all objects moving around city.
	 */
	class Voyager
	{
		public:
		typedef boost::shared_ptr<Track> PTrack;

		protected:
		Point position_;	// kazdy ma pozycje, predkosc(tylko ze pieszemu nie zmieniamy) i trase - i walker i vehicle, wiec tu moze byc
		double velocity_;
		double angle_;
		double percentDistanceTraveled_;
		PTrack track_;

		bool finished_;	// czy skonczyl trase

		public:
		Voyager(double velocity = 0.0, Point position = Point());
		virtual ~Voyager() = 0;

		virtual OBJECTS type() = 0;
		
		virtual const std::string& id() const = 0;
		const Point& position() const;
		double angle() const;
		double velocity() const;
		void track(const PTrack track);
		virtual void reset() = 0; // daje na poczatek i stopuje
		virtual void move(long long int elapsed_time) = 0;	// oczywiscie w milisekundach
		bool finished();
	};
}

#endif // VOYAGER_HPP