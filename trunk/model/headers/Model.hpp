#ifndef MODEL_HPP
#define MODEL_HPP

#include "Dispatcher.hpp"
#include "Vehicle.hpp"
#include "Walker.hpp"
#include "Graph.hpp"
#include <map>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

namespace zpr
{
	class Controller;
	/**
	 * Sth about model
	 */
	class Model
	{
	
	public:
		static const long long int TIME_SCALE = 1000; // 1ms symulacji == 1s prawdziwa
		friend class Controller;
		friend class View; // to tylko teraz do ulatwienia sporego testu

		typedef boost::shared_ptr<Vehicle> PCar;
		typedef std::map<std::string, PCar> MCar;
		typedef boost::shared_ptr<Walker> PWalker;
		typedef std::map<std::string, PWalker> MWalker;

		Dispatcher dispatcher_;
		MCar cars_;
		MWalker walkers_;
		Graph streets_;

		bool loop_; // zapetlenie wlasnie

		boost::condition_variable updateCondition_;
		long long int elapsedMicroseconds_;
		boost::mutex updateMutex_;

		public:
		Model();

		void start(); // wyzerowanie wszystkiego, ustawienie na pozycjach begin wszystkich obiektow
		void switchLoop();
		void nextStep(long long int elapsed_time);

		Graph& streets() { return streets_; } // testowe do view

		void operator()();
		void scheduleUpdate(long long int elapsedMicroseconds);
	};
}

#endif // MODEL_HPP