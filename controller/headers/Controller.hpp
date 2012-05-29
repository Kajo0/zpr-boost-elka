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
	class Timer
	{
	public:
		typedef boost::chrono::high_resolution_clock::time_point TimePoint;
		typedef boost::chrono::high_resolution_clock::duration Duration;

		Timer(const Duration & frequency);
		TimePoint Now() const;
		Duration Elapsed(const TimePoint & since) const;
		
		void AddListener(boost::function<void ()> listener);
		void operator()();
		
	private:
		Duration frequency_;
		//std::deque<boost::thread*> listeners_;
		std::deque<boost::function<void ()> > listeners_;
	};

	class Controller
	{
		Model model_;
		View view_;
		boost::thread timer, modelThread, viewThread;

		public:
		Controller(const boost::filesystem::path & path);

		private:
		void mainLoop(); // tu bym to zapetlil calosc

		// parsing functions
		void parseMap(const boost::filesystem::path & path);
		void parseDispatcher(const boost::filesystem::path & path);
		void parseObjects(const boost::filesystem::path & path);
	};
}
#endif // CONTROLLER_HPP