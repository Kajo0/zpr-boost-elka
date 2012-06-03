#include "Controller.hpp"
#include "View.hpp"
#include "Logger.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace zpr
{
	const int View::WINDOW_WIDTH			= 800;
	const int View::WINDOW_HEIGHT			= 600;
	const int View::VISUALISATION_WIDTH		= 600;
	const int View::VISUALISATION_HEIGHT	= 600;
	const int View::ALLEGRO_EVENT_TIMEOUT	= 10;
	const int View::SCALER = VISUALISATION_WIDTH / 200;

	AllegroException::AllegroException(const std::string & message) : std::exception(message.c_str()) {}

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
		font_ = al_load_ttf_font((boost::filesystem::current_path() / "Consolas.ttf").string().c_str(), 15, 0);//al_load_ttf_font("times.ttf", 12, 0);
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

	void View::scheduleRefresh(long long int elapsed_microseconds)
	{
		{ // ta klamra moze byc potrzebna dla locka, ale czy na pewno tego nie wiem.
			boost::lock_guard<boost::mutex> lock(refreshMutex_);
			elapsedMicroseconds_ = elapsed_microseconds;
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

			while(run && !boost::this_thread::interruption_requested())
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
		Model::PGraph g = model_.streets();
		ALLEGRO_TRANSFORM rotate_transformation;

		static ALLEGRO_COLOR WALKER_COLOR		= al_map_rgb(200, 53, 206);
		static ALLEGRO_COLOR BIGCAR_COLOR		= al_map_rgb(0, 53, 206);
		static ALLEGRO_COLOR SMALLCAR_COLOR		= al_map_rgb(255, 126, 0);
		static ALLEGRO_COLOR CAMERA_COLOR		= al_map_rgba(200,0,0,10);
		static ALLEGRO_COLOR GROUND_COLOR		= al_map_rgb(0,100,0);
		static ALLEGRO_COLOR STREETS_COLOR		= al_map_rgb(200, 200, 200);

		al_clear_to_color(GROUND_COLOR);

		for (Graph::MVertices::const_iterator it = g->vertices_.begin(); it != g->vertices_.end(); ++it)
		{
			double w = 10 * SCALER;
			for (Graph::MVertices::const_iterator i = it->second->edges_.begin(); i != it->second->edges_.end(); ++i)
			{
				al_draw_line(it->second->position_.x_ * SCALER, it->second->position_.y_ * SCALER,
								i->second->position_.x_ * SCALER, i->second->position_.y_ * SCALER, STREETS_COLOR, w);
			}
			al_draw_filled_circle(it->second->position_.x_ * SCALER, it->second->position_.y_ * SCALER, w / 2, STREETS_COLOR);
		}

		Model::VTObject objects = model_.objects();
		for (Model::VTObject::const_iterator it = objects.begin(); it != objects.end(); ++it)
		{
			al_identity_transform(&rotate_transformation);
			al_translate_transform(&rotate_transformation, -it->get<0>().x_ * SCALER, -it->get<0>().y_ * SCALER);
			al_rotate_transform(&rotate_transformation, it->get<1>());
			al_translate_transform(&rotate_transformation, it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER);
			al_use_transform(&rotate_transformation);

			switch (it->get<2>())
			{
				case SMALLCAR: al_draw_filled_rectangle((it->get<0>().x_ - 1.5) * SCALER, (it->get<0>().y_ - 1) * SCALER,
														(it->get<0>().x_ + 1.5) * SCALER, (it->get<0>().y_ + 1) * SCALER,
														SMALLCAR_COLOR);
					break;
				case BIGCAR: al_draw_filled_rectangle((it->get<0>().x_ - 2) * SCALER, (it->get<0>().y_ - 1.5) * SCALER,
														(it->get<0>().x_ + 2) * SCALER, (it->get<0>().y_ + 1.5) * SCALER,
														BIGCAR_COLOR);
					break;
				case WALKER: al_draw_filled_triangle((it->get<0>().x_ - 1) * SCALER, (it->get<0>().y_ - 1) * SCALER,
													(it->get<0>().x_ + 3) * SCALER, it->get<0>().y_ * SCALER,
													(it->get<0>().x_ - 1) * SCALER, (it->get<0>().y_ + 1) * SCALER,
													WALKER_COLOR);
					break;
			}			
			al_identity_transform(&rotate_transformation);
			al_use_transform(&rotate_transformation);
			al_draw_text(font_, al_map_rgb(0, 0, 0), it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER, ALLEGRO_ALIGN_CENTRE, it->get<3>().c_str());
			//al_draw_text(font_, al_map_rgb(0, 0, 0), it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER + 15, ALLEGRO_ALIGN_CENTRE, boost::lexical_cast<std::string>(it->get<4>()).c_str());
		}

		Model::VTCamera cameras = model_.cameras();
		for (Model::VTCamera::const_iterator it = cameras.begin(); it != cameras.end(); ++it)
		{
			al_draw_filled_circle(it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER, 5, CAMERA_COLOR);

			al_draw_filled_pieslice(it->get<0>().x_ * SCALER, it->get<0>().y_ * SCALER, it->get<1>() * SCALER,
									it->get<2>() - it->get<3>() / 2, it->get<3>(),
									CAMERA_COLOR);
		}

		drawMenu();
		al_flip_display();	// swap buffers
	}

	void View::drawMenu()
	{
		static ALLEGRO_COLOR BUTTON_COLOR	= al_map_rgb(110, 190, 230);
		static ALLEGRO_COLOR MENU_BCG_COLOR	= al_map_rgb(100, 100, 100);
		static ALLEGRO_COLOR FONT_COLOR		= al_map_rgb(255, 255, 255);

		al_draw_filled_rectangle(VISUALISATION_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT, MENU_BCG_COLOR);
		startButton.drawFilled(BUTTON_COLOR);
		stopButton.drawFilled(BUTTON_COLOR);
		restartButton.drawFilled(BUTTON_COLOR);
		loopButton.drawFilled(BUTTON_COLOR);
		exitButton.drawFilled(BUTTON_COLOR);

		al_draw_text(font_, FONT_COLOR, (startButton.topLeft_.x_ + startButton.bottomRight_.x_) / 2, (startButton.topLeft_.y_ + startButton.bottomRight_.y_) / 2 - font_->height / 2, ALLEGRO_ALIGN_CENTRE, "Start");
		al_draw_text(font_, FONT_COLOR, (stopButton.topLeft_.x_ + stopButton.bottomRight_.x_) / 2, (stopButton.topLeft_.y_ + stopButton.bottomRight_.y_) / 2 - font_->height / 2, ALLEGRO_ALIGN_CENTRE, "Pause");
		al_draw_text(font_, FONT_COLOR, (restartButton.topLeft_.x_ + restartButton.bottomRight_.x_) / 2, (restartButton.topLeft_.y_ + restartButton.bottomRight_.y_) / 2 - font_->height / 2, ALLEGRO_ALIGN_CENTRE, "Restart");
		al_draw_text(font_, FONT_COLOR, (loopButton.topLeft_.x_ + loopButton.bottomRight_.x_) / 2, (loopButton.topLeft_.y_ + loopButton.bottomRight_.y_) / 2 - font_->height / 2, ALLEGRO_ALIGN_CENTRE, "Loop");
		al_draw_text(font_, FONT_COLOR, (exitButton.topLeft_.x_ + exitButton.bottomRight_.x_) / 2, (exitButton.topLeft_.y_ + exitButton.bottomRight_.y_) / 2 - font_->height / 2, ALLEGRO_ALIGN_CENTRE, "Finish");
	}

}