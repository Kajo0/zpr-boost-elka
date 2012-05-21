#include "Model.hpp"

namespace zpr
{
	Model::Model()
	{
	}

	Model::~Model() {}

	void Model::tellMeEverythingAboutObjects()
	{
		for (MCar::const_iterator it = cars_.begin(); it != cars_.end(); ++it)
		{
			it->second->tellMeMore();
		}
		
		for (MWalker::const_iterator it = walkers_.begin(); it != walkers_.end(); ++it)
		{
			it->second->tellMeMore();
		}
	}
}