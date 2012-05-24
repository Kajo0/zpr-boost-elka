#include "View.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include "Base.hpp"
#include "VehicleTrack.hpp"
#include "Walker.hpp"
#include "SmallCar.hpp"

namespace zpr
{
	const int View::WINDOW_WIDTH			= 800;
	const int View::WINDOW_HEIGHT			= 600;
	const int View::VISUALISATION_WIDTH		= 600;
	const int View::VISUALISATION_HEIGHT	= 600;

	View::View(Model model): model_(model)
	{
		display_ = NULL;

		if(!al_init()) // there is no view :(
		{
			fprintf(stderr, "failed to initialize allegro!\n");
			return;
		}

		display_ = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);

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

		//Walker w(std::string("Ja"), 5);
		//w.addTrackPoint( Point(10, 10) );
		//w.addTrackPoint( Point(200, 10) );
		//w.addTrackPoint( Point(100, 200) );
		//w.reset();
		//
		//SmallCar s(std::string("WF-JA"), 50, 1000, 15);
		//s.addTrackPoint(Point(0, 450));
		//s.addTrackPoint(Point(150, 450));
		//s.addTrackPoint(Point(150, 150));
		//s.addTrackPoint(Point(400, 800));
		//s.addTrackPoint(Point(500, 400));
		//s.addTrackPoint(Point(600, 400));
		//s.addTrackPoint(Point(600, 200));
		//s.addTrackPoint(Point(500, 200));
		//s.addTrackPoint(Point(300, 100));
		//s.reset();

		al_install_keyboard();
		al_install_mouse();
		al_register_event_source(eventQueue_, al_get_mouse_event_source());
		al_register_event_source(eventQueue_, al_get_keyboard_event_source());

		VehicleTrack m;	// wstep do wyklikania trasy

		while (true)
		{
			VehicleTrack mm;
			mm.addPoint( Point(100, 100) );
			static int xx = 0, yy = 0;
			// czeka na event 0.001 czasu a potem sieka dalej, jak X to konczy petle
			// trzeba bedzie obczic allegro eventy bardziej :/
			// ogolnie to chyba wyswietlimy jedno okienko dla gui typu wprowadzanie a cala animacja bedzie w nowym
			// bo odswierzanie bedzie robil timer i tam na eventy nie trzeba czekac a wyklikanie to te kliki bedize trzeba obsluzyc
			if ( al_wait_for_event_timed(eventQueue_, &ev, 0.01) )
			{
				if ( ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE )
					break;
				else if ( ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN )
					m.addPoint( Point(ev.mouse.x, ev.mouse.y) );
				else if ( ev.type == ALLEGRO_EVENT_MOUSE_AXES ) {
					xx = ev.mouse.x;
					yy = ev.mouse.y;
				}
			}
			
			mm.addPoint( Point(xx, 100) );
			mm.addPoint( Point(xx, yy) );

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

			// przyklad beziera jak sie zmienia bezier na prostym kacie
			for (double i = 0; i <= 1; i += 0.01)
				al_draw_filled_circle(mm.positionOnTrack(i).x_, mm.positionOnTrack(i).y_, 2, al_map_rgb(0,0,0));
			
			// dodawana recznie klikana trasa, do wyklikania ewentualnie
			for (double i = 0; i <= 1; i += 0.01)
				al_draw_filled_circle(m.positionOnTrack(i).x_, m.positionOnTrack(i).y_, 2, al_map_rgb(0,0,0));

			//// to rysuje beziera z 4 pktow jbc
			//float p[] = {150, 150, 300, 100, 300, 100, 500, 200};
			//al_draw_spline(p, al_map_rgb(255,255,0), 15);

			//// tam wyzej na zolto bezier od pkt do pkt a tu moj rozstapiony wierzcholek
			//VehicleTrack t, tt, ttt;
			//t.addPoint(Point(150, 150));
			//t.addPoint(Point(300, 100));
			//t.addPoint(Point(500, 200));
			//for (double i = 0; i <= 1; i += 0.01)
			//	al_draw_filled_circle(t.positionOnTrack(i).x_, t.positionOnTrack(i).y_, 6, al_map_rgb(0,255,255));
			//
			//tt.addPoint(Point(150, 450));
			//tt.addPoint(Point(150, 150));
			//tt.addPoint(Point(400, 800));
			//for (double i = 0; i <= 1; i += 0.001)
			//	al_draw_filled_circle(tt.positionOnTrack(i).x_, tt.positionOnTrack(i).y_, 6, al_map_rgb(0,0,255));
			//
			//ttt.addPoint(Point(0, 450));
			//ttt.addPoint(Point(150, 450));
			//ttt.addPoint(Point(150, 150));
			//ttt.addPoint(Point(400, 800));
			//ttt.addPoint(Point(500, 400));
			//ttt.addPoint(Point(600, 400));
			//ttt.addPoint(Point(600, 200));
			//ttt.addPoint(Point(500, 200));
			//ttt.addPoint(Point(300, 100));
			//for (double i = 0; i <= 1; i += 0.001)
			//	al_draw_filled_circle(ttt.positionOnTrack(i).x_, ttt.positionOnTrack(i).y_, 3, al_map_rgb(255,0,0));


			//al_clear_to_color( al_map_rgb(0,0,0) );
			//Sleep(1);

			//w.move(100);
			//al_draw_filled_circle(w.position().x_, w.position().y_, 5, al_map_rgb(0,255,0));
			//s.move(1);
			//al_draw_filled_circle(s.position().x_, s.position().y_, 5, al_map_rgb(0,0,255));

			al_flip_display();	// swap buffers
		}
 
	}
}