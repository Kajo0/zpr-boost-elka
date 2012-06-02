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

		OBJECTS type();
		
		const std::string& id() const;
		void reset();
		void move(long long int elapsed_time);
	};
}

#endif // WALKER_HPP