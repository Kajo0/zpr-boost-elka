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

	void Camera::tellMeSthAbout()
	{
		std::cout<<"Id: "<<id_
			<<" ("<<position_.x_<<","<<position_.y_<<")"
			<<" dir: "<<direction_
			<<" ang: "<<angle_
			<<" rng: "<<range_
			<<" pre: "<<precision_<<"\n";
	}

	int Camera::id()
	{
		return id_;
	}
}