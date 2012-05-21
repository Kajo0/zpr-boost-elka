#include "View.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "Base.hpp"
#include "VehicleTrack.hpp"

namespace zpr
{
	View::View(Model model): model_(model)
	{
		display_ = NULL;

		if(!al_init()) // there is no view :(
		{
			fprintf(stderr, "failed to initialize allegro!\n");
			return;
		}

		display_ = al_create_display(640, 480);

		if(!display_)
		{
			fprintf(stderr, "failed to create display!\n");
			return;
		}

		eventQueue_ = al_create_event_queue();

		if(!eventQueue_)
		{
			fprintf(stderr, "failed to create event_queue!\n");
			al_destroy_display(display_);
			return;
		}
		
		al_set_window_title(display_, "Our great fancy View frame");
		al_init_primitives_addon();	// init dla prymitowow

		al_register_event_source(eventQueue_, al_get_display_event_source(display_)); // bind displaya do eventow kolejki
	}

	View::~View()
	{
		al_destroy_event_queue(eventQueue_);
		al_destroy_display(display_);
	}

	void View::loop()
	{
		ALLEGRO_EVENT ev;
		
		Graph g = model_.streets();

		while (true)
		{
			// czeka na event 0.001 czasu a potem sieka dalej, jak X to konczy petle
			// trzeba bedzie obczic allegro eventy bardziej :/
			// ogolnie to chyba wyswietlimy jedno okienko dla gui typu wprowadzanie a cala animacja bedzie w nowym
			// bo odswierzanie bedzie robil timer i tam na eventy nie trzeba czekac a wyklikanie to te kliki bedize trzeba obsluzyc
			if ( al_wait_for_event_timed(eventQueue_, &ev, 0.001) && ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
			{
				break;
			}


			al_clear_to_color( al_map_rgb(0,100,0) ); // clear na zielono

			for (Graph::MVertices::const_iterator it = g.vertices_.begin(); it != g.vertices_.end(); ++it)
			{
				// stala mowiaca o szerokosci drogi tez gdzies // ogolnie taki zarys bo friendem tego zrobic nie mozemy raczej :P
				double w = 60;
				for (Graph::MVertices::const_iterator i = it->second->edges_.begin(); i != it->second->edges_.end(); ++i)
				{
					al_draw_line(it->second->position_.x_, it->second->position_.y_, i->second->position_.x_, i->second->position_.y_, al_map_rgb(200,200,200), w);
				}
				al_draw_filled_circle(it->second->position_.x_, it->second->position_.y_, w / 2, al_map_rgb(200, 200, 200));
			}


			// to rysuje beziera z 4 pktow jbc
			float p[] = {150, 100, 320, 50, 320, 50, 500, 180};
			al_draw_spline(p, al_map_rgb(255,255,0), 5);

			// tam wyzej na zolto bezier od pkt do pkt a tu moj rozstapiony wierzcholek
			VehicleTrack t, tt, ttt;
			t.addPoint(Point(150, 100));
			t.addPoint(Point(320, 50));
			t.addPoint(Point(500, 180));
			for (double i = 0; i <= 1; i += 0.01)
				al_draw_filled_circle(t.getPosition(i).x_, t.getPosition(i).y_, 3, al_map_rgb(0,255,255));
			
			tt.addPoint(Point(150, 420));
			tt.addPoint(Point(150, 100));
			tt.addPoint(Point(500, 600));
			for (double i = 0; i <= 1; i += 0.01)
				al_draw_filled_circle(tt.getPosition(i).x_, tt.getPosition(i).y_, 3, al_map_rgb(255,0,0));
			
			ttt.addPoint(Point(0, 420));
			ttt.addPoint(Point(150, 420));
			ttt.addPoint(Point(150, 100));
			ttt.addPoint(Point(500, 600));
			ttt.addPoint(Point(500, 320));
			ttt.addPoint(Point(640, 320));
			ttt.addPoint(Point(640, 180));
			ttt.addPoint(Point(500, 180));
			ttt.addPoint(Point(320, 50));
			for (double i = 0; i <= 1; i += 0.01)
				al_draw_filled_circle(ttt.getPosition(i).x_, ttt.getPosition(i).y_, 3, al_map_rgb(0,0,255));
			


			al_flip_display();	// swap buffers
		}
 
	}
}