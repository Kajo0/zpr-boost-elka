#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"

#include <boost/filesystem.hpp>

namespace zpr
{
	class Controller
	{
		Model model_;
		View view_;

		public:
		Controller(const boost::filesystem::path & path);
		virtual ~Controller();

		private:
		void mainLoop(); // tu bym to zapetlil calosc

		// parsing functions
		void parseMap(const boost::filesystem::path & path);
		void parseDispatcher(const boost::filesystem::path & path);
		void parseObjects(const boost::filesystem::path & path);
	};
}
#endif // CONTROLLER_HPP