#ifndef VIEW_HPP
#define VIEW_HPP

#include <allegro5/allegro.h>
#include "Model.hpp"

namespace zpr
{
	class View
	{
		ALLEGRO_DISPLAY *display_; // tu mamy nasz frame i wsio chyba w jednym
		ALLEGRO_EVENT_QUEUE *eventQueue_; // tu eventy typu tez X on close
		
		Model model_;

		public:
		View(Model model); // tylko to tesow gui
		virtual ~View();

		void loop();
	};
}
#endif // VIEW_HPP