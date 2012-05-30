#include "Controller.hpp"
#include "View.hpp"
#include "Logger.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include "Base.hpp"
#include "Track.hpp"
#include "Walker.hpp"
#include "SmallCar.hpp"

namespace zpr
{
	const int View::WINDOW_WIDTH		= 800;
	const int View::WINDOW_HEIGHT		= 600;
	const int View::VISUALISATION_WIDTH	= 600;
	const int View::VISUALISATION_HEIGHT	= 600;
	const int View::ALLEGRO_EVENT_TIMEOUT	= 10;

	AllegroException::AllegroException(const std::string & message) : std::exception(message.c_str())
	{}

	AllegroRectangle::AllegroRectangle(const Point & topLeft, const Point & bottomRight) :topLeft_(topLeft), bottomRight_(bottomRight) {}
	AllegroRectangle::AllegroRectangle(int x1, int y1, int x2, int y2) :topLeft_(Point(x1, y1)), bottomRight_(Point(x2, y2)) {}

	bool AllegroRectangle::inside(const Point & check) const
	{
		return inRange(topLeft_.x_, bottomRight_.x_, check.x_) && inRange(topLeft_.y_, bottomRight_.y_, check.y_);
	}

	void AllegroRectangle::drawFilled(const ALLEGRO_COLOR & color) const
	{
		al_draw_filled_rectangle(topLeft_.x_, topLeft_.y_, bottomRight_.x_, bottomRight_.y_, color);
	}

	View::View(Controller & controller, Model & model) : controller_(controller), model_(model), display_(NULL), eventQueue_(NULL), font_(NULL), elapsedMicroseconds_(-1)					
	{
		menuArea = AllegroRectangle(VISUALISATION_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		
		int xMargin = 20, yMargin = 20, height = 40;
		int yOffset = yMargin;
		startButton = AllegroRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height);
		yOffset += height + yMargin;
		stopButton = AllegroRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height);
		yOffset += height + yMargin;
		restartButton = AllegroRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height);
		yOffset += height + yMargin;
		loopButton = AllegroRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height);
		yOffset += height + yMargin;
		exitButton = AllegroRectangle(VISUALISATION_WIDTH + xMargin, yOffset, WINDOW_WIDTH - xMargin, yOffset + height);
	}

	void View::initializeGraphics()
	{
		if(!al_init()) // there is no view :(
			throw AllegroException("Failed to initialize allegro!");

		display_ = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
		if(!display_)
			throw AllegroException("Failed to create display!");

		eventQueue_ = al_create_event_queue();
		if(!eventQueue_)
			throw AllegroException("Failed to create event_queue!");
		
		if(!al_init_primitives_addon())	// init dla prymitowow
			throw AllegroException("Failed to initialize addons!");
		al_init_font_addon();
		if(!al_init_ttf_addon())
			throw AllegroException("Failed to initialize TTF addon!");
		//al_init_image_addon();

		// ponizej z nieznanych przyczyn current_path mi zwraca sciezke do katalogu z projektem a nie o poziom dalej do /debug...
		font_ = al_load_ttf_font((boost::filesystem::current_path() / "Consolas.ttf").string().c_str(), 10, 0);//al_load_ttf_font("times.ttf", 12, 0);
		if(!font_)
			throw AllegroException("Failed to initialize font!");

		if(!al_install_keyboard())
			throw AllegroException("Failed to install keyboard!");
		if(!al_install_mouse())
			throw AllegroException("Failed to install mouse!");

		al_set_window_title(display_, "ZPR::Symulator");
		al_register_event_source(eventQueue_, al_get_display_event_source(display_)); // bind displaya do eventow kolejki
		al_register_event_source(eventQueue_, al_get_mouse_event_source());
		al_register_event_source(eventQueue_, al_get_keyboard_event_source());
	}

	void View::closeGraphics()
	{
		al_destroy_event_queue(eventQueue_);
		al_destroy_display(display_);
	}

	void View::scheduleRefresh(long long int elapsedMicroseconds)
	{
		{ // ta klamra moze byc potrzebna dla locka, ale czy na pewno tego nie wiem.
			boost::lock_guard<boost::mutex> lock(refreshMutex_);
			elapsedMicroseconds_ = elapsedMicroseconds;
		}
		refreshCondition_.notify_one();
	}

	void View::operator()()
	{
		try
		{
			initializeGraphics();
			
			ALLEGRO_EVENT allegroEvent;
			bool run = true;

			refresh();

			while(run)
			{
				//// Waiting for refresh signal
				{
					boost::unique_lock<boost::mutex> lock(refreshMutex_);
					//while(elapsedMicroseconds_ < 0)
						refreshCondition_.timed_wait(lock, boost::posix_time::milliseconds(ALLEGRO_EVENT_TIMEOUT));

					if(elapsedMicroseconds_ >= 0)
					{
						elapsedMicroseconds_ = -1;
						refresh();
					}
				}
				
				//// Allegro event queue checking
				while(al_get_next_event(eventQueue_, &allegroEvent))
				{
					//std::cout << "event" << std::endl;
					if(allegroEvent.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
					{
						controller_.scheduleEvent(boost::make_shared<EventClose>());
						std::cout << "Close pressed" << std::endl;
						run = false;
					}
					else if(allegroEvent.type == ALLEGRO_EVENT_KEY_UP)
					{
						if(allegroEvent.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
						{
							controller_.scheduleEvent(boost::make_shared<EventClose>());
							std::cout << "Escape pressed" << std::endl;
							run = false;
						}
					}
					else if(allegroEvent.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
					{
						Point p(allegroEvent.mouse.x, allegroEvent.mouse.y);
						if(startButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventStart>());
						if(stopButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventStop>());
						if(restartButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventRestart>());
						if(exitButton.inside(p))
						{
							controller_.scheduleEvent(boost::make_shared<EventClose>());
							run = false;
						}
						if(loopButton.inside(p))
							controller_.scheduleEvent(boost::make_shared<EventLoop>());
					}
				}
			}
		}
		catch(zpr::AllegroException & e)
		{
			std::cout<<e.what()<<std::endl;
		}
		catch(boost::thread_interrupted)
		{
			// thread interruption
		}
		catch(...)
		{
			Logger::getInstance().message("Unknown View exception.");
		}
		closeGraphics();
		std::cout << "View thread ending." << std::endl;
	}


	// taki zarys tylk ozmienic dostep do tych rzeczy i mamy wizualizacje
	void View::refresh()
	{
		Graph g = model_.streets();
			
		al_clear_to_color( al_map_rgb(0,100,0) ); // clear na zielono

		for (Graph::MVertices::const_iterator it = g.vertices_.begin(); it != g.vertices_.end(); ++it)
		{
			double w = 60;
			for (Graph::MVertices::const_iterator i = it->second->edges_.begin(); i != it->second->edges_.end(); ++i)
			{
				al_draw_line(it->second->position_.x_, it->second->position_.y_, i->second->position_.x_, i->second->position_.y_, al_map_rgb(200,200,200), w);
			}
			al_draw_filled_circle(it->second->position_.x_, it->second->position_.y_, w / 2, al_map_rgb(200, 200, 200));
		}
		
		for (Model::MCar::const_iterator it = model_.cars_.begin(); it != model_.cars_.end(); ++it)
		{
			al_draw_filled_circle(it->second->position().x_, it->second->position().y_, 5, al_map_rgb(0, 53, 206));
			
			int height = al_get_font_line_height(font_);
			std::string position = it->second->position().str();
			std::string velocity = "v = " + (boost::format("%6.3f") % it->second->velocity_).str() + " maxV = " + (boost::format("%6.3f") % it->second->maxSpeed_).str();
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position().x_, it->second->position().y_, ALLEGRO_ALIGN_CENTRE, it->second->id().c_str());
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position().x_, it->second->position().y_ + height, ALLEGRO_ALIGN_CENTRE, position.c_str());
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position().x_, it->second->position().y_ + height + height, ALLEGRO_ALIGN_CENTRE, velocity.c_str());
		}

		for (Model::MWalker::const_iterator it = model_.walkers_.begin(); it != model_.walkers_.end(); ++it)
		{
			al_draw_filled_circle(it->second->position().x_, it->second->position().y_, 5, al_map_rgb(200, 53, 206));

			int height = al_get_font_line_height(font_);
			std::string description = "x = " + (boost::format("%6.3f") % it->second->position().x_).str() + " y = " + (boost::format("%6.3f") % it->second->position().y_).str();
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position().x_, it->second->position().y_, ALLEGRO_ALIGN_CENTRE, it->second->id().c_str());
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position().x_, it->second->position().y_ + height, ALLEGRO_ALIGN_CENTRE, description.c_str());
		}

		for (Dispatcher::MCamera::const_iterator it = model_.dispatcher_.cameras_.begin(); it != model_.dispatcher_.cameras_.end(); ++it)
		{
			al_draw_filled_circle(it->second->position_.x_, it->second->position_.y_, 5, al_map_rgb(255,0,0));

			al_draw_filled_pieslice(it->second->position_.x_, it->second->position_.y_, it->second->range_,
						it->second->direction_ - it->second->angle_ / 2, it->second->angle_,
						al_map_rgba(200,0,0,10));
			al_draw_text(font_, al_map_rgb(0,0,0), it->second->position_.x_, it->second->position_.y_, ALLEGRO_ALIGN_CENTRE, boost::lexical_cast<std::string>(it->second->id()).c_str());
		}

		drawMenu();
		al_flip_display();	// swap buffers
	}

	void View::drawMenu()
	{
		al_draw_filled_rectangle(VISUALISATION_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT, al_map_rgb(0, 0, 0));
		startButton.drawFilled(al_map_rgb(255,0,0));
		stopButton.drawFilled(al_map_rgb(255,0,0));
		restartButton.drawFilled(al_map_rgb(255,0,0));
		loopButton.drawFilled(al_map_rgb(255,0,0));
		exitButton.drawFilled(al_map_rgb(255,0,0));

		al_draw_text(font_, al_map_rgb(255,255,255), startButton.topLeft_.x_, startButton.topLeft_.y_, ALLEGRO_ALIGN_LEFT, "Start");
		al_draw_text(font_, al_map_rgb(255,255,255), stopButton.topLeft_.x_, stopButton.topLeft_.y_, ALLEGRO_ALIGN_LEFT, "Stop");
		al_draw_text(font_, al_map_rgb(255,255,255), restartButton.topLeft_.x_, restartButton.topLeft_.y_, ALLEGRO_ALIGN_LEFT, "Restart");
		al_draw_text(font_, al_map_rgb(255,255,255), loopButton.topLeft_.x_, loopButton.topLeft_.y_, ALLEGRO_ALIGN_LEFT, "Zapetlenie");
		al_draw_text(font_, al_map_rgb(255,255,255), exitButton.topLeft_.x_, exitButton.topLeft_.y_, ALLEGRO_ALIGN_LEFT, "Koniec");
	}

}