#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include "Model.hpp"
#include "View.hpp"

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/signal.hpp>
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
		
		void AddListener(boost::thread & listener);
		void operator()();
		
	private:
		Duration frequency_;
		std::deque<boost::thread*> listeners_;
	};

	class ModelUpdater
	{
	public:
		//ModelUpdater();
		Model *model;
		View *view;
		void operator()();

	private:
		//boost::signal<void ()> process_;
	};

	class Controller
	{
		Model model_;
		View view_;
		boost::thread timer, modelUpdater;

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