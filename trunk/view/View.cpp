#include "View.hpp"

#include <iostream>
#include <stdio.h>
#include <allegro5/allegro_primitives.h>

namespace zpr
{
	View::View()
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
		static float i = 0.5f; // test
		ALLEGRO_EVENT ev;
		
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


			al_clear_to_color( al_map_rgb(0,0,0) ); // clear na czarno

			al_draw_line(10, 0, 10, 60, al_map_rgb(255,255,255), 5);	// linia xD
			al_draw_filled_circle(i, i, 10, al_map_rgba(0, 255, 0, 1));
			i = ( i > 480 ) ? 0 : i + 0.5f;
 
			al_flip_display();	// swap buffers
		}
 
	}
}