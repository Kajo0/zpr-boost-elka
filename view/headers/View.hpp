#ifndef VIEW_HPP
#define VIEW_HPP

#include <allegro5/allegro.h>

namespace zpr
{
	class View
	{
		ALLEGRO_DISPLAY *display_; // tu mamy nasz frame i wsio chyba w jednym
		ALLEGRO_EVENT_QUEUE *eventQueue_; // tu eventy typu tez X on close

		public:
		View();
		virtual ~View();

		void loop();
	};
}
#endif // VIEW_HPP