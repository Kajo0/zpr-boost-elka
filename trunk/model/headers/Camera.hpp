#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Base.hpp"

namespace zpr
{
	/**
	 * Class representing camera object, contains essential information about
	 */
	class Camera
	{
		const int id_;
		Point position_;
		double direction_;
		double angle_;
		double range_;
		double precision_;

		public:
		Camera(int id, Point position, double direction, double angle, double range, double precision);
		virtual ~Camera();

		int id();

		// TODO delete it - test function
		void tellMeSthAbout();
	};
}

#endif // CAMERA_HPP