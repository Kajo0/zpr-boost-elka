#ifndef WALKER_H
#define WALKER_H

namespace zpr
	{
	class Voyager;
	/**
	 * Class representing walkers in city.
	 */
	class Walker : public Voyager
		{
		private:

		public:
		Walker(double weight, double maxSpeed);
		~Walker();
		Walker & Walker(const Walker & other);
		Walker & operator=(const Walker & other);
		};
	}

#endif // WALKER_H