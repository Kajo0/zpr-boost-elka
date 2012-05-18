#include "Voyager.hpp"

namespace zpr
{
	Voyager::Voyager() {}

	/**
	 * Empty abstract class' destructor
	 */
	Voyager::~Voyager() {}

	const Point& Voyager::position() const
	{
		return position_;
	}
}