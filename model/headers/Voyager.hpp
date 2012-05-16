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
		protected:
		Point position_;	// kazdy ma pozycje, predkosc(tylko ze pieszemu nie zmieniamy) i trase - i walker i vehicle, wiec tu moze byc
		double velocity_;
		// size? - nie, na razie pkty materialne :P
		Track track_;

		public:
		Voyager();
		virtual ~Voyager() = 0;
		
		const Point& Position() const;
	};
}

#endif // VOYAGER_HPP