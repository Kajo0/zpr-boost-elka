#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/function.hpp>
#include <boost/chrono.hpp>

namespace zpr
{
	class Controller
	{
		Model model_;
		View view_;
		boost::thread timerThread, modelThread, viewThread;

		public:
		Controller(const boost::filesystem::path & path);
		void start(); // tu bym to zapetlil calosc

		private:
		// parsing functions
		void parseMap(const boost::filesystem::path & path);
		void parseDispatcher(const boost::filesystem::path & path);
		void parseObjects(const boost::filesystem::path & path);
	};
}
#endif // CONTROLLER_HPP