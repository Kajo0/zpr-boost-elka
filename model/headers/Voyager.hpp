#ifndef VOYAGER_HPP
#define VOYAGER_HPP

#include "Base.hpp"
#include "Track.hpp"
#include <boost\shared_ptr.hpp>

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
		// size? - nie, na razie pkty materialne :P
		PTrack track_;

		public:
		Voyager(double velocity = 0.0, Point position = Point());
		virtual ~Voyager() = 0;
		
		virtual const std::string& id() = 0;
		const Point& position() const;
		void addTrackPoint(const Point point);


		// TODO delete it - test only
		virtual void tellMeMore(){};
	};
}

#endif // VOYAGER_HPP