#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
#include "Events.hpp"

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/function.hpp>
#include <boost/chrono.hpp>
#include <queue>

namespace zpr
{
	class Controller
	{
		Model model_;
		View view_;
		boost::thread timerThread, modelThread, viewThread;

		public:
		Controller(const boost::filesystem::path & path);
		void scheduleEvent(const boost::shared_ptr<Event> & newEvent);

		void start(); // tu bym to zapetlil calosc

		void Process(EventStart&);
		void Process(EventStop&);
		void Process(EventClose&);

		private:
		bool run_;
		// parsing functions
		void parseMap(const boost::filesystem::path & path);
		void parseDispatcher(const boost::filesystem::path & path);
		void parseObjects(const boost::filesystem::path & path);

		boost::condition_variable eventCondition_;
		std::queue<boost::shared_ptr<Event> > eventQueue_;
		boost::mutex eventMutex_;
	};
}
#endif // CONTROLLER_HPP