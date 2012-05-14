#include "Simulator.hpp"

namespace zpr
{
	
	Simulator::Simulator()
	{

	}

	Simulator::~Simulator()
	{

	}

	Simulator & Simulator::getInstance()
	{
		static Simulator simulator;
		return simulator;
	}

	void Simulator::Create()
	{
		getInstance();
	}


}