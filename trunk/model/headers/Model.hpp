#ifndef MODEL_HPP
#define MODEL_HPP

#include "Dispatcher.hpp"

namespace zpr
{
	/**
	 * Sth about model
	 */
	class Model
	{
		Dispatcher dispatcher_;

		public:
			// Pytanie za 100 pkt , gdzie komentujemy metody ? przy deklaracji czy deifincji ?!
			// odpowiedŸ za 200: krotki koment, ocb i po co to jest, przy deklaracji; w definicji komentuj tylko co sie dzieje w ktorej linijce, jak masz potrzebe, albo wypada

			// pytanie za 300 -> kto odpowiada za komunikacje z userem. tudziez z plikami xml? controller? jezeli tak, to on powinien czytac pliki, a model tylko cos w stylu addCam(Camera cam)...
		Model();
		virtual ~Model();

		private:
		void loadCameras();
	};
}

#endif // MODEL_HPP