#include "Controller.hpp"
#include "Logger.hpp"
#include <boost\foreach.hpp>
#include <boost\property_tree\ptree.hpp>
#include <boost\property_tree\xml_parser.hpp>
#include <vector>
//TODO fabryczka jakos
#include "SmallCar.hpp"
#include "BigCar.hpp"
#include "Walker.hpp"

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
		std::cout<<"\n";

		// wczytanie mapy drog
		parseMap("xml_data\\streets.xml");
		model_.streets_.printAllData();
		std::cout<<"\n";

		// wczyutanie obiektow
		parseObjects("xml_data\\objects.xml");
		model_.tellMeEverythingAboutObjects();
		std::cout<<"\n";

		// tu mamy wczytane niby wsio, trza upieknic komunikacje pomiedzy modulami,
		// wypada uruchomic jakos timera i zeby rozpoczynal symulacje gdzies itp
		// ogolnie to czytanie tych xmli , fabryka, tworzenie obiektow do modelu trzeba pokminic i ladnie to zrobic
	}

	void Controller::parseDispatcher(const char *path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Dispatcher::PCamera cam;
	
		try
		{
			read_xml(path, pt, boost::property_tree::xml_parser::no_comments);

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

	void Controller::parseMap(const char *path)
	{
		using boost::property_tree::ptree;
		ptree pt;
	
		try
		{
			read_xml(path, pt, boost::property_tree::xml_parser::no_comments);
			//std::vector<std::pair<int, Point>> vertices;
			//std::vector<std::pair<int, int>> edges;

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("map") )
			{
				if ( v.first == "nodes" )
				{
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
					{
						int id = vv.second.get<int>("");
						Point position( vv.second.get<double>("<xmlattr>.x"), vv.second.get<double>("<xmlattr>.y") );
						//std::cout<<"\tID: "<<id<<" x: "<<position.x_<<" y: "<<position.y_<<"\n";
						//vertices.push_back(std::make_pair(id, position));

						model_.streets_.addVertex(id, position);
					}
				}
				else if ( v.first == "edges" )
				{
					BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("") )
					{
						int from = vv.second.get<int>("<xmlattr>.from");
						int to = vv.second.get<int>("<xmlattr>.to");
						//std::cout<<"\tFrom: "<<from<<" To: "<<to<<"\n";
						//edges.push_back(std::make_pair(from, to));

						model_.streets_.addEdge(from, to);
					}
				}
			}
		}	// TODO sprytnie jakos obsluzyc te exceptiony - moze lapac je w loopie i pytac o poprawienie albo o poprawna sciezke lub czy uruchomic bez kamer
		catch (boost::property_tree::xml_parser_error&)
		{
			Logger::getInstance().message("No files with map !!");
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

	void Controller::parseObjects(const char *path)
	{
		using boost::property_tree::ptree;
		ptree pt;
		Model::PCar car;
		Model::PWalker walker;
	
		try
		{
			read_xml(path, pt, boost::property_tree::xml_parser::no_comments);

			BOOST_FOREACH( ptree::value_type &v, pt.get_child("objects") )
			{
				if ( v.second.get<std::string>("<xmlattr>.type") == "car" )
				{
					std::string id = v.second.get<std::string>("id");
					double weight = v.second.get<double>("parameters.weight");
					std::string size = v.second.get<std::string>("parameters.size"); // potem enum jakis
					double mspeed = v.second.get<double>("parameters.maxspeed");

					// TODO - tu wstaw fabryke jakos, ja tego nie widze za bardzo dla naszej aplikacyjki ale trzeba ladniej to zrobic :/
					// przyspieszenie jakos z masy i max predkosci wyliczamy czy kak ?
					if (size == "big")
						car.reset( new BigCar( id, 0/*?!?!*/, weight, mspeed ) );
					else if (size == "small")
						car.reset( new SmallCar( id, 0/*?!?!*/, weight, mspeed ) );

					// jak nie powtorka to rob trase
					if ( model_.cars_.insert( std::pair<std::string, Model::PCar>( id, car ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );

							model_.cars_[id]->addTrackPoint(point);
						}
					}
				}
				else if ( v.second.get<std::string>("<xmlattr>.type") == "walker" )
				{
					std::string id = v.second.get<std::string>("id");
					double mspeed = v.second.get<double>("parameters.maxspeed");

					walker.reset( new Walker( id, mspeed ) );
					// jak nie powtorka to rob trase
					if ( model_.walkers_.insert( std::pair<std::string, Model::PWalker>( id, walker ) ).second )
					{
						BOOST_FOREACH( ptree::value_type &vv, v.second.get_child("track") )
						{
							Point point( vv.second.get<double>("<xmlattr>.x"),
										vv.second.get<double>("<xmlattr>.y") );

							model_.walkers_[id]->addTrackPoint(point);
						}
					}
				}
			}
		}	// TODO sprytnie jakos obsluzyc te exceptiony - moze lapac je w loopie i pytac o poprawienie albo o poprawna sciezke lub czy uruchomic bez kamer
		catch (boost::property_tree::xml_parser_error&)
		{
			Logger::getInstance().message("No files with objets !!");
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