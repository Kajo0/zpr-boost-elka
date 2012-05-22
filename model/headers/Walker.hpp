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
		Walker(std::string &name, double velocity);
		~Walker();
		Walker(const Walker &other);
		Walker& operator=(const Walker &other);
		
		const std::string& id();
		void reset();
		void move(const long elapsed_time);
	};
}

#endif // WALKER_HPP