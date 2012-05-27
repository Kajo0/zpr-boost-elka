#include "Model.hpp"

namespace zpr
{
	Model::Model()
	{
	}

	Model::~Model() {}

	void Model::start()
	{
		// tak zeby dzialalo, bo nei rozkminialme dokladnie binda czy tam lambdy
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->reset();
		}
		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->reset();
		}
	}

	void Model::nextStep(const long elapsed_time)
	{
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->move(elapsed_time);
		}

		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->move(elapsed_time);
		}
	}
}