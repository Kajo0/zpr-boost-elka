#ifndef VIEW_HPP
#define VIEW_HPP

#include "Model.hpp"
#include "Base.hpp"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <boost/thread.hpp>

namespace zpr
{
	class Controller;

	class AllegroException : public std::exception
	{
	public:
		AllegroException(const std::string & message);
	};

	struct AllegroRectangle
	{
	public:
		Point topLeft_, bottomRight_;
		AllegroRectangle(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
		AllegroRectangle(const Point & topLeft, const Point & bottomRight);
		
		bool inside(const Point & check) const;
		void drawFilled(const ALLEGRO_COLOR & color) const;
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
		ALLEGRO_FONT *font_;
		
		Controller & controller_; // moze to jeszcze jakos inaczej, moze funkcja globalna
		Model & model_;

		void initializeGraphics();
		void closeGraphics();
		void refresh();
		void drawMenu();

		boost::condition_variable refreshCondition_;
		long long int elapsedMicroseconds_;
		boost::mutex refreshMutex_;

		AllegroRectangle menuArea, startButton, stopButton, restartButton, exitButton;

		public:
		View(Controller & controller, Model & model); // tylko to tesow gui

		//void model(Model *model) { model_ = model; }
		//void loop();

		void operator()();
		void scheduleRefresh(long long int elapsedMicroseconds);
	};
}
#endif // VIEW_HPP