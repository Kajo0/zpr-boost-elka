#include "Camera.hpp"
#include <iostream>

namespace zpr
{
	Camera::Camera(int id, Point position, double direction, double angle, double range, double precision):
						id_(id), position_(position), direction_(direction), angle_(angle), range_(range), precision_(precision)
	{
	}

	Camera::~Camera()
	{
	}

	int Camera::id()
	{
		return id_;
	}
}