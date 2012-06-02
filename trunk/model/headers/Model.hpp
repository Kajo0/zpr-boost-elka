#ifndef MODEL_HPP
#define MODEL_HPP

#include "Dispatcher.hpp"
#include "Vehicle.hpp"
#include "Walker.hpp"
#include "Graph.hpp"
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>

namespace zpr
{
	/**
	 * Sth about model
	 */
	class Model
	{
		public:
		static const long long int TIME_SCALE = 1000; // 1ms symulacji == 1s prawdziwa

		typedef boost::shared_ptr<Vehicle> PCar;
		typedef std::map<std::string, PCar> MCar;
		typedef boost::shared_ptr<Walker> PWalker;
		typedef std::map<std::string, PWalker> MWalker;
		typedef boost::shared_ptr<Graph> PGraph;
		typedef boost::tuple<Point, double, double, double> TCamera; // pos, range, dir, angle
		typedef std::vector<TCamera> VTCamera;
		typedef boost::tuple<Point, double, OBJECTS> TObject;	// pos, angle, size
		typedef std::vector<TObject> VTObject;

		private:
		Dispatcher dispatcher_;
		PGraph streets_;
		MCar cars_;
		MWalker walkers_;
		
		boost::condition_variable updateCondition_;
		long long int elapsedMicroseconds_;
		boost::mutex updateMutex_;
		bool loop_, active_, log_;
	friend class Dispatcher;
		public:
		Model();

		void start(); // wyzerowanie wszystkiego, ustawienie na pozycjach begin wszystkich obiektow
		void switchLoop();
		void setActive(bool active);
		void nextStep(long long int elapsed_time);
		void addCamera(const Dispatcher::PCamera camera);
		void addObject(const PCar car);
		void addObject(const PWalker walker);

		PGraph streets();
		void streets(const PGraph graph);
		VTCamera cameras();
		VTObject objects();

		void operator()();
		void scheduleUpdate(long long int elapsedMicroseconds);
		void scheduleLog(long long int elapsedMicroseconds);
	};
}

#endif // MODEL_HPP