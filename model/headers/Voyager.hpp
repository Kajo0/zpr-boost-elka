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
		private:
		Point position_;
		double velocity_;
		double acceleration_;
		double weight_;
		double maxSpeed_;
		// size?
		Track track_;

		public:
		Voyager();
		Voyager(double weight, double maxSpeed);
		virtual ~Voyager() = 0;
		
		const Point& Position() const;
	};
}

#endif // VOYAGER_HPP