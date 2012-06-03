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
	 * Main simulation model
	 * Model is responsible for calculating positions of object
	 * Holds information about simulation objects
	 * Manage whole simulation
	 */
	class Model
	{
		public:
		static const long long int TIME_SCALE = 1000; // TODO uzyc tego ?!

		typedef boost::shared_ptr<Vehicle> PCar;
		typedef boost::shared_ptr<Walker> PWalker;
		typedef boost::shared_ptr<Graph> PGraph;
		typedef boost::tuple<Point, double, double, double> TCamera; // position, range, direction, angle
		typedef std::vector<TCamera> VTCamera;
		typedef boost::tuple<Point, double, OBJECTS, std::string, double> TObject;	// position, angle, type/size, name/id, velocity
		typedef std::vector<TObject> VTObject;

		typedef boost::shared_ptr<Voyager> PVoyager;

		private:
		/** Dispatcher to managing cameras */
		Dispatcher dispatcher_;
		/** Graph representing streets in model */
		PGraph streets_;
		/** Simulation objects, both Vehicles & Walkers */
		std::deque<PVoyager> objects_;
		/** Time from start of simulation [us] */
		long long int elapsedMicroseconds_;
		/** Model's update condition */
		boost::condition_variable updateCondition_;
		/** Model's update mutex */
		boost::mutex updateMutex_;
		/** Simulation loop flag */
		bool loop_;
		/** Simulation activity flag */
		bool active_;
		/** Simulation logging flag */
		bool log_;

		friend class Dispatcher; // TODO ?

		public:
		/**
		 * Model c-tor
		 */
		Model();

		/**
		 * Set each model object on it's own start position
		 */
		void start();

		/**
		 * Switch model looping
		 */
		void switchLoop();

		/**
		 * Enable / disable model updating
		 *
		 * @param active true to resume, false to pause
		 */
		void setActive(bool active);

		/**
		 * Update each object of given elapsed time
		 *
		 * @param elapsed time time which elapsed in milliseconds
		 */
		void nextStep(long long int elapsed_time);

		/**
		 * Add given camera into model's dispatcher
		 *
		 * @param camera camera to add
		 */
		void addCamera(const Dispatcher::PCamera & camera);

		/**
		 * Add given object into model
		 *
		 * @param object object to add
		 */
		void addObject(const PVoyager & object);

		/**
		 * @return graph of streets
		 */
		PGraph streets() const;

		/**
		 * Set new streets graph into model
		 *
		 * @param graph graph of streets
		 */
		void streets(const PGraph graph);

		/**
		 * Get essential information about cameras to draw them on screen
		 *
		 * @return tuple of infromation:
		 *			<position, range, direction, angle>
		 */
		VTCamera cameras() const;
		
		/**
		 * Get essential information about objects to draw them on screen
		 *
		 * @return tuple of infromation:
		 *			<position, angle, type/size, name/id, velocity>
		 */
		VTObject objects() const;

		/**
		 * Model main loop
		 */
		void operator()();

		/**
		 * Notify object to update
		 *
		 * @param elapsed_microseconds elapsed time in microseconds
		 */
		void scheduleUpdate(long long int elapsed_microseconds);
		
		/**
		 * Notify logger to update
		 */
		void scheduleLog(long long int elapsed_microseconds); // TODO po co te us ?
	};
}

#endif // MODEL_HPP