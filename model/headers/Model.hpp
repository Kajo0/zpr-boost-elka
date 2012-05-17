#ifndef MODEL_HPP
#define MODEL_HPP

#include "Dispatcher.hpp"
#include "Vehicle.hpp"
#include "Walker.hpp"
#include "Graph.hpp"
#include <map>
#include <boost\shared_ptr.hpp>

namespace zpr
{
	class Controller;
	/**
	 * Sth about model
	 */
	class Model
	{
		friend class Controller;

		typedef boost::shared_ptr<Vehicle> PCar;
		typedef std::map<std::string, PCar> MCar;
		typedef boost::shared_ptr<Walker> PWalker;
		typedef std::map<std::string, PWalker> MWalker;

		Dispatcher dispatcher_;
		MCar cars_;
		MWalker walkers_;
		Graph streets_;

		public:
			// Pytanie za 100 pkt , gdzie komentujemy metody ? przy deklaracji czy deifincji ?!
			// odpowiedŸ za 200: krotki koment, ocb i po co to jest, przy deklaracji; w definicji komentuj tylko co sie dzieje w ktorej linijce, jak masz potrzebe, albo wypada

			// pytanie za 300 -> kto odpowiada za komunikacje z userem. tudziez z plikami xml? controller? jezeli tak, to on powinien czytac pliki, a model tylko cos w stylu addCam(Camera cam)...
		Model();
		virtual ~Model();
	};
}

#endif // MODEL_HPP