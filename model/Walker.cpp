#include "Walker.hpp"
#include "WalkerTrack.hpp"
#include <iostream>
#include <string>

namespace zpr
{
	Walker::Walker(std::string &name, double velocity): name_(name), Voyager(velocity)
	{
		track_ = PTrack( new WalkerTrack() );
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

	

	void Walker::tellMeMore()
	{
		std::cout<<"id: "<<name_
				<<" pos: "<<position_
				<<" v: "<<velocity_
				<<" t.len: "<<track_->length()<<"\n";
	}
}