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
		Model();
		virtual ~Model();

		private:
		void loadCameras();
	};
}

#endif // MODEL_HPP