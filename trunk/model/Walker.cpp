#include "Walker.hpp"

namespace zpr
{
	Walker::Walker(std::string &name, double velocity): name_(name), Voyager(velocity)
	{
	}

	Walker::~Walker() {}

	Walker::Walker(const Walker &other)
	{
	}

	Walker& Walker::operator=(const Walker &other)
	{
		return *this;
	}

	const std::string& Walker::id()
	{
		return name_;
	}
}