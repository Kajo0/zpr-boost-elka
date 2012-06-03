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
	
	/**
	 * Class representing exceptions thrown while allegro working
	 */
	class AllegroException : public std::exception
	{
		public:
		/**
		 * Allegro exception c-tor
		 */
		//AllegroException(const std::string & message);
	};

	/**
	 * Struct representing rectange witch location
	 */
	struct AllegroRectangle
	{
		/** Top left corner */
		Point topLeft_;
		/** Bottom roight corner */
		Point bottomRight_;
		/**
		 * Rectange c-tor
		 *
		 * @param x1 top left corner x coordinate
		 * @param y1 top left corner y coordinate
		 * @param x2 bottom right corner x coordinate
		 * @param y2 bottom right corner y coordinate
		 */
		AllegroRectangle(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
		
		/**
		 * Rectange c-tor
		 *
		 * @param topLeft top left corner coordinate
		 * @param bottomRight bottom right corner coordinate
		 */
		AllegroRectangle(const Point & topLeft, const Point & bottomRight);
		
		/**
		 * Check if given point is inside of rectangle
		 *
		 * @param check point to check
		 * @return true if inside, false otherwise
		 */
		bool inside(const Point & check) const;

		/**
		 * Draws rectange on screen with given color
		 *
		 * @param color background rect. color
		 */
		void drawFilled(const ALLEGRO_COLOR & color) const;
	};

	//TODO remove ?!
	struct AllegroButton
	{
		AllegroRectangle area_;
		std::string title_;
		// TODO
		void draw(const ALLEGRO_COLOR & color) const;
	};

	/**
	 * Main class of simulation view screen info.
	 */
	class View
	{
		/** Width of window */
		const static int WINDOW_WIDTH;
		/** Height of window */
		const static int WINDOW_HEIGHT;
		/** Width of the part of window for visualisation simulation */
		const static int VISUALISATION_WIDTH;
		/** Height of the part of window for visualisation simulation */
		const static int VISUALISATION_HEIGHT;
		/** Timeout to check allegro events in view */
		const static int ALLEGRO_EVENT_TIMEOUT;
		/** Value to scale data on the view visualisation screen */
		const static double SCALER;

		/** Allegro display field */
		ALLEGRO_DISPLAY *display_;
		/** Allegro event queue */
		ALLEGRO_EVENT_QUEUE *eventQueue_;
		/** Allegro font field */
		ALLEGRO_FONT *font_;
		
		/** Simulator controller */
		Controller & controller_;
		/** Simulator model */
		Model & model_;

		/**
		 * Init allegro functionality
		 */
		void initializeGraphics();

		/**
		 * Destroz allegro functionality objects
		 */
		void closeGraphics();

		/**
		 * Refresh view window
		 */
		void refresh();
		
		/**
		 * Draw menu on the view window
		 */
		void drawMenu();

		/** View refresh condition */
		boost::condition_variable refreshCondition_;
		/** View refresh mutex */
		boost::mutex refreshMutex_;
		/** Elapsed microsconds from last refresh */
		long long int elapsedMicroseconds_;

		/** Menu area rect */
		AllegroRectangle menuArea;
		/** Resume button rect */
		AllegroRectangle startButton;
		/** Pause button rect */
		AllegroRectangle stopButton;
		/** Restart button rect */
		AllegroRectangle restartButton;
		/** Loop button rect */
		AllegroRectangle loopButton;
		/** Exit button rect */
		AllegroRectangle exitButton;

		public:
		/**
		 * View c-tor
		 *
		 * @param controller simulator controller
		 * @param model simulator model
		 */
		View(Controller & controller, Model & model);

		/**
		 * Main view loop
		 */
		void operator()();

		/**
		 * View refresh schedule notification
		 *
		 * @param elapsed_microseconds elapsed time in microseconds
		 */
		void scheduleRefresh(long long int elapsed_microseconds);
	};
}
#endif // VIEW_HPP