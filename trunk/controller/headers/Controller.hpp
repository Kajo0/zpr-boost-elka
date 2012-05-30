#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"
#include "Timer.hpp"
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
		Timer timer_;
		boost::thread timerThread, modelThread, viewThread;

		public:
		Controller(const boost::filesystem::path & path);	// prepare simulation data
		void scheduleEvent(const boost::shared_ptr<Event> & newEvent);	// add new event to queue

		void run();	// enter controllers main loop

		void Process(EventStart&);	// process start event
		void Process(EventStop&);	// process stop event
		void Process(EventRestart&);	// process restart event
		void Process(EventClose&);	// process close event

		private:
		bool run_;
		// parsing functions
		void parseMap(const boost::filesystem::path & path);
		void parseDispatcher(const boost::filesystem::path & path);
		void parseObjects(const boost::filesystem::path & path);

		void runThreads();	// starts all dependent threads - model, view and timer
		void endThreads();	// interrupts and waits for run threads to complete

		void start();	// make the simulation active
		void stop();	// pause the simulation

		boost::condition_variable eventCondition_;
		std::queue<boost::shared_ptr<Event> > eventQueue_;
		boost::mutex eventMutex_;
	};
}
#endif // CONTROLLER_HPP