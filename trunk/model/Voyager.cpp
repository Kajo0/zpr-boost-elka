#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager() {}

	/**
	 * Empty abstract class' destructor
	 */
	Voyager::~Voyager() {}

	const Point& Voyager::Position() const
	{
		return position_;
	}
}