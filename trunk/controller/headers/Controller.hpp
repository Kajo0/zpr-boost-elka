#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"

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
		void parseMap(const char *path);
		void parseDispatcher(const char *path);
		void parseObjects(const char *path);
	};
}
#endif // CONTROLLER_HPP