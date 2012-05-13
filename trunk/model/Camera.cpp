#include "Camera.hpp"
#include <iostream>

namespace zpr
{
	Camera::Camera(int id): id_(id)
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
}