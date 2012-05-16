#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"

namespace zpr
{
	class Controller
	{
		Model model_;
		//View view_;

		public:
		Controller();
		virtual ~Controller();

		private:
		void mainLoop(); // tu bym to zapetlil calosc

		// parsing functions
		void parseMap(const char*);
		void parseDispatcher(const char*);
		void parseObjects(const char*);
	};
}
#endif // CONTROLLER_HPP