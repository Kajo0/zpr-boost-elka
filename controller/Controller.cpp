#include "Controller.hpp"
#include "Logger.hpp"
#include <boost\foreach.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>

namespace zpr
{
	Controller::Controller()
	{
		mainLoop();
	}

	Controller::~Controller() {}

	void Controller::mainLoop()
	{
		// wczytanie pathow, odpalenie symulacji, ewentualnie przed wyklikanie, proste menu tu na razie, potem zdarzenia z view jakos tu przewalic
		parseDispatcher("xml_data\\dispatcher.xml");
		model_.dispatcher_.tellMeSthAbout();
	}

	void Controller::parseDispatcher(const char *path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Dispatcher::PCamera cam;
	
		try
		{
			read_xml(path, pt);

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("dispatcher") )
			{
				// get cam parameteres
				int id = v.second.get<int>("id");
				Point position( v.second.get<double>("position.<xmlattr>.x"), v.second.get<double>("position.<xmlattr>.y") );
				double direction = v.second.get<double>("direction") / 360.0;
				double angle = v.second.get<double>("angle") / 360;
				double range = v.second.get<double>("range");
				double precision = v.second.get<double>("precision");

				// add camera
				model_.dispatcher_.addCamera( Dispatcher::PCamera(new Camera(id, position, direction, angle, range, precision)) );
			}
		}	// TODO sprytnie jakos obsluzyc te exceptiony - moze lapac je w loopie i pytac o poprawienie albo o poprawna sciezke lub czy uruchomic bez kamer
		catch (boost::property_tree::xml_parser_error&)
		{
			Logger::getInstance().message("No files with cameras !!");
		}
		catch (boost::property_tree::ptree_bad_path&)
		{
			Logger::getInstance().message("Error while reading tree nodes: node doesn't exist !");
		}
		catch (std::exception &e)
		{
			Logger::getInstance().message(e.what());
		}
	}
}