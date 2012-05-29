#ifndef VIEW_HPP
#define VIEW_HPP

#include "Model.hpp"
#include <allegro5/allegro.h>
#include <boost/thread.hpp>

namespace zpr
{
	class AllegroException : public std::exception
	{
	public:
		AllegroException(const std::string & message);
	};

	class View
	{
		const static int WINDOW_WIDTH;
		const static int WINDOW_HEIGHT;
		const static int VISUALISATION_WIDTH;	// damy kwadrat do dyspozycji wizualizacji i po prawo jakies menu typu timer aktualny, stop, start itp
		const static int VISUALISATION_HEIGHT;
		const static int ALLEGRO_EVENT_TIMEOUT; //// in milliseconds

		ALLEGRO_DISPLAY *display_; // tu mamy nasz frame i wsio chyba w jednym
		ALLEGRO_EVENT_QUEUE *eventQueue_; // tu eventy typu tez X on close
		
		Model & model_;

		void initializeGraphics();
		void closeGraphics();
		void refresh();

		boost::condition_variable refreshCondition_;
		bool doRefresh_;
		boost::mutex refreshMutex_;

		public:
		View(Model & model); // tylko to tesow gui

		//void model(Model *model) { model_ = model; }
		//void loop();

		void operator()();
		void scheduleRefresh();
	};
}
#endif // VIEW_HPP