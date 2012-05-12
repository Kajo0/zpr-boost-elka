#include "Walker.hpp"

namespace zpr
{
	Walker::Walker(double weight, double maxSpeed): Voyager(weight, maxSpeed)
	{
	}

	Walker::~Walker()
	{
	}

	Walker::Walker(const Walker &other)
	{
	}

	Walker& Walker::operator=(const Walker &other)
	{
		return *this;
	}
}