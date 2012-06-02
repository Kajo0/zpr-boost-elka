#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Base.hpp"
#include "Voyager.hpp"

namespace zpr
{
	/**
	 * Class representing camera object, contains essential information about
	 */
	class Camera
	{
		private:
		const int id_;
		Point position_;
		double direction_;
		double angle_;
		double range_;
		double precision_;

		public:
		Camera(int id, Point position, double direction, double angle, double range, double precision);

		int id() const;
		double range() const;
		double angle() const;
		double direction() const;
		const Point position() const;
		bool spotted(const Voyager & object) const;
		double noise() const;
	};
}

#endif // CAMERA_HPP