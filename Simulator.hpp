#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP
// pewnie inna nazwa pliku by sie przydala

#include "Model.hpp"
#include "Controller.hpp"
#include "View.hpp"

namespace zpr
{
	/**
	  * Main class for whole projects.
	  * Gathers entire MVC pattern.
	  * Singleton;
	  */
	class Simulator
		{
		public:
		// interface
		static void Create(); // create simulator object

		private:
		// Singleton implementation
		Simulator();
		~Simulator();
		Simulator(const Simulator &);
		const Simulator & operator=(const Simulator &);

		static Simulator & getInstance();

		// parsing functions
		void ParseMap();
		void ParseDispatcher();
		void ParseObjects();
		
		// Simulation data
		// MVC
		Model model_;
		Controller controller_;
		View view_;
		};
}
#endif // SIMULATOR_HPP