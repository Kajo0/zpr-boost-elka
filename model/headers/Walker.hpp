#ifndef WALKER_HPP
#define WALKER_HPP

#include "Voyager.hpp"

namespace zpr
{
	/**
	 * Class representing walkers in city.
	 */
	class Walker : public Voyager
	{
		protected:
		const std::string name_;

		public:
		Walker(double weight, double maxSpeed);
		~Walker();
		Walker(const Walker &other);
		Walker& operator=(const Walker &other);
	};
}

#endif // WALKER_HPP